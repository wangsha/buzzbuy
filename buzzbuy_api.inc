<?php


function buzzbuy_get_activity_list(&$form_state, &$form, $mode, $uid = null) {
	if(empty($uid)) {
		global $user;
		$uid = $user->uid;
	}
	$sql = "SELECT c.pid, c.uid, c.comment, c.recommend, c.timestamp,
					i.title, i.detailpageurl, i.productgroup,  m.url, u.picture, u.name, u.uid
					FROM " .BUZZBUY_COMMENT ." c, amazon_item i, amazon_item_image m, users u 
					WHERE c.pid = i.asin AND i.asin = m.asin AND u.uid = c.uid";

	switch($mode) {
		case 'all':
			break;
		case 'followed':
			$sql .= " AND c.uid IN (SELECT follower FROM " .BUZZBUY_FOLLOW. " WHERE followed =".$uid.")";
			break;
		case 'following':
			$sql .= " AND c.uid IN (SELECT followed FROM " .BUZZBUY_FOLLOW. " WHERE follower =".$uid.")";
			break;
	}
	$sql .=" ORDER BY timestamp ASC";

	$result = pager_query($sql, 20);
	$rows = array();
	$form_state['products'] = array();
	while($data = db_fetch_array($result)) {
		$row = array();
		if(!array_key_exists($data['pid'], $form_state['products'])) {
			$form_state['products'][$data['pid']] = $data;
		}
		$form['image_'.$data['pid']] = array(
			'#type' => 'markup',
			'#value' => '<a href="'.$data['detailpageurl'].'">
			<img src="'.$data['url'].'" alt="'.$data['title'].'" width="160px"></a>',
		);
		$row['image_'.$data['pid']] = drupal_render($form['image_'.$data['pid']]);



		if(array_key_exists($data['pid'], $rows)) {
			$text = '<img src="'. $data['picture'] .'" width="30px" height="30px"> ';
			$text .= l($data['name'], 'buzzbuy/user/profile/'.$data['uid']);
			$text .= ' '.$data['comment'];
			$text .= ' <span style="color:#999">'.Agotime($data['timestamp']) .'</span><br>';

			$rows[$data['pid']]['text_'.$data['pid']] .= $text;
		}else{
			$text = '<h3>'. l($data['title'], $data['detailpageurl']) .'</h3>';
			$text .= '<br><b>' . $data['comment'] .'</b>';
			$text .= '<br> <span style="color:#999">review by <img src="'. $data['picture'] .'" width="15px" height="15px"> ';
			$text .= l($data['name'], 'buzzbuy/user/profile/'.$data['uid']);
			$text .= ' '.Agotime($data['timestamp']) .'</span><hr>';
			$row['text_'.$data['pid']] = $text;

			$rows[$data['pid']] = $row;
		}
	}

	//add comment box
	foreach($rows as $pid => $row) {
		$form['comment_'.$pid] = array(
			'#type' => 'textarea',
			'#id' => 'comment_'.$pid,
			'#name' => 'comment_'.$pid,
			'#cols' => 70,
			'#rows' => 2,
		);
		$form['submit_'.$pid] = array(
			'#type' => 'submit',
			'#name' => 'submit_'.$pid,
			'#id' => 'submit_'.$pid,
			'#value' => 'Write a Review',
			'#prefix' => '<div style="float:left">',
			'#suffix' => '</div>',
		);



		$rows[$pid]['text_'.$pid] .= drupal_render($form['comment_'.$pid]);
		$rows[$pid]['text_'.$pid] .= drupal_render($form['submit_'.$pid]);
		if(_is_user_facebook_connected()) {
			$fb_favicon = theme('stream_publish_favicon');
			$form['fb_'.$pid] = array(
			'#type' => 'checkbox',
			'#title' => $fb_favicon . ' Share on Facebook',
			'#name' => 'fb_'.$pid,
			'#id' => 'fb_'.$pid,
 			'#value' => 1,
			'#prefix' => '<div style="float:left; color:#999; padding-top:5px; padding-left:5px">',
			'#suffix' => '</div>',
			);
			$rows[$pid]['text_'.$pid] .= drupal_render($form['fb_'.$pid]);
		}
		if(_is_user_twitter_connected()) {
			$tt_favicon = '<img src="'. drupal_get_path("module", 'buzzbuy') .'/twitter.ico" />';
			$form['tt_'.$pid] = array(
			'#type' => 'checkbox',
			'#title' => $tt_favicon . ' Share on Twitter',
			'#name' => 'tt_'.$pid,
			'#id' => 'tt_'.$pid,
			'#value' => 1,
		  '#prefix' => '<div style="float:left; color:#999; padding-top:4px; padding-left:5px">',
			'#suffix' => '</div>',
			);
			$rows[$pid]['text_'.$pid] .= drupal_render($form['tt_'.$pid]);
		}


	}

	$output = theme_table(array(), $rows);
	$output .= theme_pager(NULL, $pagelimit);
	return $output;
}



function buzzbuy_add_review($values, $uid = null) {

}

function buzzbuy_update_review($values, $uid = null) {

}

function buzzbuy_delete_review($id) {

}

function buzzbuy_get_follower($uid = null) {
	if(empty($uid)) {
		global $user;
		$uid = $user->uid;
	}
	$sql = "SELECT cname, cid, uid FROM ". BUZZBUY_CONNECTIONS .
				 " WHERE uid IN (SELECT follower FROM ". BUZZBUY_FOLLOW . " WHERE followed = %d)".
				 " AND ctype=%d";
	$result = db_query($sql, $uid, BUZZBUY_FB_CONNECT);
	$rows = array();
	while($data = db_fetch_array($result)) {
		$rows[] = $data;
	}
	return buzzbuy_get_follow_table($uid, $rows, BUZZBUY_FOLLOW_MODE_FOLLOWER);
}

function buzzbuy_get_followed($uid = null) {
	if(empty($uid)) {
		global $user;
		$uid = $user->uid;
	}
	$sql = "SELECT cname, cid, uid FROM ". BUZZBUY_CONNECTIONS .
				 " WHERE uid IN (SELECT followed FROM ". BUZZBUY_FOLLOW . " WHERE follower = %d)".
				 " AND ctype=%d";
	$result = db_query($sql, $uid, BUZZBUY_FB_CONNECT);
	$rows = array();
	while($data = db_fetch_array($result)) {
		$rows[] = $data;
	}
	return buzzbuy_get_follow_table($uid, $rows, BUZZBUY_FOLLOW_MODE_FOLLOWED);
}

function buzzbuy_get_follow_table($uid, $data, $mode) {

	$rows = array();
	foreach($data as $id => $u) {
		$stats = buzzbuy_user_basic_stats($u['uid']);
		$row = array();
		$form['image_'.$u['uid']] = array(
			'#type' => 'markup',
			'#value' => '<a href="buzzbuy/user/profile/'.$u['uid'].'">
			<img src="http://graph.facebook.com/'.$u['cid'].'/picture" alt="'.$u['cname'].'"></a>',
		);
		$row['image_'.$u['uid']] = drupal_render($form['image_'.$u['uid']]);
		$form['name_'.$u['uid']] = array(
			'#type' => 'item',
			'#value' => l($u['cname'], 'buzzbuy/user/profile/'.$u['uid']),
		);
		$row['name_'.$u['uid']] = drupal_render($form['name_'.$u['uid']]);

		$text = '<span style="font-size:80%">Following ' . l($stats['following'], 'buzzbuy/user/follower/'.$u['uid']);
		$text .= '<br> Follower ' . l($stats['followed'], 'buzzbuy/user/followed/'.$u['uid']);
		$text .= '<br> Reviews ' .l($stats['reviews'], 'buzzbuy/user/profile/'. $u['uid']);
		$text .= '</span>';
		$row['stat_'.$u['uid']] = $text;

		$follow_path = 'buzzbuy/user/unfollow';
		$follow_text = 'Unfollow';

		if($mode == BUZZBUY_FOLLOW_MODE_FOLLOWED && !buzzbuy_is_following($uid, $u['uid'])) {
			$follow_path = 'buzzbuy/user/follow';
			$follow_text = 'Follow Back';
		}
		$form['unfollow_'.$u['uid']] = array(
			'#type' => 'markup',
			'#value' => l($follow_text, $follow_path. '/' . $uid.'/'.$u['uid'].'/'.buzzbuy_explode_url($_GET['q'])),
		);
		$row['unfollow_'.$u['uid']] = drupal_render($form['unfollow_'.$u['uid']]);

		$rows[] = $row;
	}

	$table = theme_table(array(), $rows);
	return $table;
}

function buzzbuy_is_following($follower, $followed) {
	$sql = "SELECT followed FROM ". BUZZBUY_FOLLOW ." WHERE follower=%d AND followed=%d";
	return db_result(db_query($sql, $follower, $followed));
}
function buzzbuy_user_basic_stats($uid) {
	$follower_sql = "SELECT COUNT(follower) as follower FROM ". BUZZBUY_FOLLOW ." WHERE followed=%d";
	$followed_sql = "SELECT COUNT(followed) as following FROM ". BUZZBUY_FOLLOW ." WHERE follower=%d";
	$activity_sql = "SELECT COUNT(asid) as reviews FROM ". BUZZBUY_ACTIVITY_LIST ." WHERE uid=%d";

	$result['following'] = db_result(db_query($follower_sql, $uid));
	$result['followed'] = db_result(db_query($followed_sql, $uid));
	$result['reviews'] = db_result(db_query($activity_sql, $uid));
	return $result;
}

function buzzbuy_explode_url($url) {
	$arr = explode("/", $url);
	$res = implode("_", $arr);
	return $res;
}

function buzzbuy_unexplode_url($str) {
	$arr = explode("_", $str);
	$res = implode("/", $arr);
	return $res;
}

function buzzbuy_get_username($uid) {
	$sql = "SELECT cname FROM ". BUZZBUY_CONNECT." WHERE uid=%d AND ctype=%d";
	return db_result(db_query($sql, $uid, BUZZBUY_FB_CONNECT));
}

function buzzbuy_get_base_url() {
	return "http://" . $_SERVER['HTTP_HOST'] .'/buzzbuy/?q=';
}

function buzzbuy_add_connection($uid, $cid, $ctype, $cname, $token, $secret = null) {
	$delete_sql = "DELETE FROM ".BUZZBUY_CONNECTIONS." WHERE uid=%d AND ctype=%d";
	db_query($delete_sql, $uid, $ctype);
	$sql = "INSERT INTO ". BUZZBUY_CONNECTIONS . " (uid, cid, ctype, cname, ctoken, csecret, timestamp)
				  VALUES (%d, %d, %d, '%s', '%s', '%s', NOW())";
	return db_query($sql, $uid, $cid, $ctype, $cname, $token, $secret);
}



function buzzbuy_insert_product($data) {
	$product_sql = "INSERT IGNORE INTO amazon_item (asin, title, detailpageurl, releasedate, productgroup, timestamp)
				VALUES ('%s', '%s', '%s', '%s', '%s', '%s')";
	db_query($product_sql, $data['asin'], $data['title'], $data['detailpageurl'], $data['releasedate'], ucfirst($data['productgroup']), date("Y-m-d H:i:s", $data['timestamp']));

	$product_img_sql = "INSERT IGNORE INTO amazon_item_image (asin, size, url, height, width)
	VALUES ('%s', 'mediumimage', '%s', 160, 160)";
	db_query($product_img_sql, $data['asin'], $data['img']);
}

function buzzbuy_insert_user($data) {
	$user_sql = "INSERT IGNORE INTO users (name, pass, mail, created, status, picture)
							 VALUES ('%s', '68cf1d36f7adc68531f527b4f9cb63cb', '%s', NOW(), %d, '%s')";
	return db_query($user_sql, $data['name'], $data['mail'],$data['status'], $data['picture']);
}

function buzzbuy_insert_comment($data) {
	if(strlen($data['comment']) == 0) return;

	$c_sql = "INSERT INTO ". BUZZBUY_COMMENT ." (pid, uid, bp_item_id, comment, recommend, timestamp)
						VALUES ('%s', %d, %d, '%s', %d, '%s')";
	db_query($c_sql, $data['pid'], $data['uid'], $data['bp_item_id'], $data['comment'], $data['recommend'], date("Y-m-d H:i:s", $data['timestamp']));
}

function buzzbuy_get_userid($name) {
	return db_result(db_query("SELECT uid FROM users WHERE name='%s'", $name));
}
function buzzbuy_post_to_blippy($item_id, $message) {

}

function _is_user_twitter_connected($uid = NULL) {
	if(!$uid) {
		global $user;
		$uid = $user->uid;
	}
	$twitter_accounts = twitter_get_user_accounts($uid);
	return $twitter_accounts[0]->id;

}

function _is_user_facebook_connected() {

	global $user;
	return $user->fb_visible;

}
function buzzbuy_post_to_twitter($data) {
	global $user;
	$twitter_accounts = twitter_get_user_accounts($user->uid);
	$twitter_account = $twitter_accounts[0];
	$status = $data['message'];
	$data['link']	= twitter_shorten_url($data['link']);
	$status .= ' '.$data['link'];
	$status .= ' @buzz_buy';
	$status .= ' #'.strtr($data['name'], array(" " => ""));

	try {
		$result = twitter_set_status($twitter_account, $status);
		drupal_set_message(t('Successfully posted to Twitter'));
	}
	catch (TwitterException $e) {
		drupal_set_message(t('An error occurred when posting to twitter: %code %error',
		array('%code' => $result->code, '%error' => $result->error)), 'warning');
	}
}

function buzzbuy_post_to_facebook($data) {
	// Create our Application instance (replace this with your appId and secret).
	$facebook = facebook_client();

	$attachment = array(
								'message' => $data['message'],
                'name' => $data['name'],
                'caption' => 'Product review shared on Buzzbuy',
                'description' => '',
                'link' => $data['link'],
								'picture' => $data['picture'],
                'actions' => array(
	array('name' => 'View Detail',
        'link' => $data['link']))
	);

	$session = $facebook->getSession();
	$result = $facebook->api(
		'/me/feed/',
    'post',
	$attachment);
	if($result)
	drupal_set_message(t('Successfully posted to Facebook'));
}


function Agotime($date)
{
	if(empty($date)) {
		return "No date provided";
	}

	$periods         = array("second", "minute", "hour", "day", "week", "month", "year", "decade");
	$lengths         = array("60","60","24","7","4.35","12","10");

	$now             = time();
	$unix_date         = strtotime($date);

	// check validity of date
	if(empty($unix_date)) {
		return "Bad date";
	}

	// is it future date or past date
	if($now > $unix_date) {
		$difference     = $now - $unix_date;
		$tense         = "ago";

	} else {
		$difference     = $unix_date - $now;
		$tense         = "from now";
	}

	for($j = 0; $difference >= $lengths[$j] && $j < count($lengths)-1; $j++) {
		$difference /= $lengths[$j];
	}

	$difference = round($difference);

	if($difference != 1) {
		$periods[$j].= "s";
	}

	return "$difference $periods[$j] {$tense}";
}

function buzzbuy_refresh_blippy_data() {
	global $user;
	db_query("TRUNCATE TABLE buzzbuy_comment");
	$blippyuser = $user->blippy;
	if(!empty($blippyuser->blippy_uid)) {
		$blippy = blippy_client();
		$following = $blippy->getUserFollowing($blippyuser->username);

		//insert friends
		foreach($following as $key =>$val) {
			$f = $val['friend'];
			$newuser['name'] = $f['username'];
			$newuser['mail'] = $f['username'] . '@buzzbuy.com';
			$newuser['picture'] = $f['profile_image_url'];
			$newuser['status'] = -1; //flag fake user
			$id = buzzbuy_insert_user($newuser);
			buzzbuy_set_follow($user->uid, $id);
		}

		//insert feed
		$feed = $blippy->feed(array('count' => 50));
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
					$newuser['status'] = -1;
					buzzbuy_insert_user($newuser);
				}
					
				$review['uid'] =  buzzbuy_get_userid($newuser['name']);
				$review['pid'] = $p['asin'];
				$review['comment'] = $item['review'];
				$review['recommend'] = BUZZBUY_RECOMMEND;
				$review['timestamp'] = $p['timestamp'];
				buzzbuy_insert_comment($review);
			}
		}
	}
}

function buzzbuy_set_follow($follower, $followed) {
	$sql = "INSERT IGNORE INTO ".BUZZBUY_FOLLOW." (follower, followed, timestamp) VALUES (%d, %d, NOW())";
	$res = db_query($sql, $follower, $followed);
}

?>