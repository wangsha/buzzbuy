<?php
require_once drupal_get_path("module", "buzzbuy").'/buzzbuy_constants.php';
require_once drupal_get_path("module", "buzzbuy").'/buzzbuy_api.inc';

function buzzbuy_connect() {
	/* Build an image link to start the redirect process. */
	$content = '<a href="?q=buzzbuy/connect/blippy/redirect"><img src="'.BLIPPY_LIB_PATH.'/blippy_signin-solo.png" alt="Sign in with blippy"/></a>';
	$form = array();
	$form['link'] = array(
		'#type' => 'markup',
		'#value' => $content,
	);

	return $form;
}

function buzzbuy_blippy_redirect() {
	drupal_set_message('buzzbuy_blippy_redirect');
	session_start();
	require_once(BLIPPY_LIB_PATH.'/lib/blippy.php');
	/* Build TwitterOAuth object with client credentials. */
	$connection = new BlippyOAuth(BLIPPY_API_KEY, BLIPPY_API_SECRET);

	/* Get temporary credentials. */
	$callback = buzzbuy_get_base_url().'buzzbuy/connect/blippy/callback/';
	$request_token = $connection->getRequestToken($callback);


	/* Save temporary credentials to session. */
	$_SESSION['oauth_token'] = $token = $request_token['oauth_token'];
	$_SESSION['oauth_token_secret'] = $request_token['oauth_token_secret'];

	/* If last connection failed don't display authorization link. */
	switch ($connection->http_code) {
		case 200:
			/* Build authorize URL and redirect user to Twitter. */
			$url = $connection->getAuthorizeURL($token);
			header('Location: ' . $url);
			break;
		default:
			/* Show notification if something went wrong. */
			echo 'Could not connect to Blippy. Refresh the page or try again later.';
	}
}

function buzzbuy_blippy_callback() {
	global $user;
	$uid = $user->uid;
	/* Start session and load lib */
	session_start();
	require_once(BLIPPY_LIB_PATH.'/lib/blippy.php');

	/* If the oauth_token is old redirect to the connect page. */
	if (isset($_REQUEST['oauth_token']) && $_SESSION['oauth_token'] !== $_REQUEST['oauth_token']) {
		$_SESSION['oauth_status'] = 'oldtoken';
		header('Location: ./clearsessions.php');
	}

	/* Create BlippyAuth object with app key/secret and token key/secret from default phase */
	$connection = new BlippyOAuth(BLIPPY_API_KEY, BLIPPY_API_SECRET, $_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);
	/* Request access tokens from blippy */
	$access_token = $connection->getAccessToken($_REQUEST['oauth_verifier']);

	/* Save the access tokens. Normally these would be saved in a database for future use. */
	$_SESSION['access_token'] = $access_token;

	/* If HTTP response is 200 continue otherwise send to connect page to retry */
	if (200 == $connection->http_code) {
		/* The user has been verified and the access tokens can be saved for future use */
		$_SESSION['status'] = 'verified';
		drupal_set_message('Your Blippy account has been successfully verified.');
		$b = new Blippy($connection);
		$c = $b->verifyCredentials();
		buzzbuy_add_connection($uid, $c['id'], BUZZBUY_BLIPPY_CONNECT, $c['name'], $_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);
		grab_blippy_feed($b);
	} else {
		/* Save HTTP status for error dialog on connnect page.*/
		session_destroy();
	}
	/* Remove no longer needed request tokens */
	unset($_SESSION['oauth_token']);
	unset($_SESSION['oauth_token_secret']);
}

function grab_blippy_feed($b) {

	$feed = $b->feed(array('count' => 50));
	foreach($feed as $id => $item) {
		if($item['klass'] == "Item") {
			$u = $item['user'];
			$p = $item['product'];
			$c = $item['comments'];
				
			$p['asin'] = 'b_'.$p['id'];
			$p['detailpageurl'] = $p['url'];
			$p['releasedate'] = $p['year'];
			$p['productgroup'] = ucfirst($p['type']);
			$p['timestamp'] = $p['created_at'];
			$p['img'] = $p['image_url'];
			buzzbuy_insert_product($p);
			if($u['klass'] == 'User') {
				$newuser['name'] = $u['username'];
				$newuser['mail'] = $u['username'] . '@buzzbuy.com';
				$newuser['picture'] = $u['profile_image_url'];
				buzzbuy_insert_user($newuser);
			}
					
			$review['uid'] =  buzzbuy_get_userid($newuser['name']);
			$review['pid'] = $p['asin'];
			$review['comment'] = $p['review'];
			$review['recommend'] = BUZZBUY_RECOMMEND;
			$review['timestamp'] = $p['timestamp'];
			buzzbuy_insert_comment($review);
			
			foreach($c as $cid => $com) {
				if($com['klass'] == 'Comment') {
						
					if($com['author']['klass'] == 'User') {
						$author = $com['author'];
						$newuser['name'] = $author['username'];
						$newuser['mail'] = $author['username'] . '@buzzbuy.com';
						$newuser['picture'] = $author['profile_image_url'];
						buzzbuy_insert_user($newuser);
					}
					$comment['pid'] = $p['asin'];
					$comment['uid'] = buzzbuy_get_userid($newuser['name']);
					$comment['comment'] = $com['body'];
					$comment['recommend'] = BUZZBUY_RECOMMEND;
					$comment['timestamp'] = $com['created_at'];
					buzzbuy_insert_comment($comment);
				}
			}
				
		}
	}
}
?>