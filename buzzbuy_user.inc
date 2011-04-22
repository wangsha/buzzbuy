<?php
require_once drupal_get_path("module", BUZZBUY_MODULE_NAME) ."/buzzbuy_api.inc";

function buzzbuy_followed() {
	global $user;


	$table = buzzbuy_get_followed();
	$form = array();
	$form['result'] = array(
		'#type'=>'markup',
		'#value' => $table,
	);
	return $form;
}

function buzzbuy_follow($follower, $followed, $backurl) {
	$sql = "INSERT INTO ".BUZZBUY_FOLLOW." (follower, followed, timestamp) VALUES (%d, %d, NOW())";
	$res = db_query($sql, $follower, $followed);
	if($res) {
		drupal_set_message("Sucessfully follow " .buzzbuy_get_username($followed));
	}
	drupal_goto(buzzbuy_unexplode_url($backurl));
}

function buzzbuy_user() {
	global $user;
	$uid = $user->uid;
	$sql = "SELECT u.uid, u.name, u.picture FROM users u WHERE uid>0 AND uid<>".$user->uid;
	$result = pager_query($sql, 10);

	while($u = db_fetch_array($result)) {
		$stats = buzzbuy_user_basic_stats($u['uid']);
		$row = array();
		$form['image_'.$u['uid']] = array(
			'#type' => 'markup',
			'#value' => '<a href="'.buzzbuy_get_base_url().'buzzbuy/user/profile/'.$u['uid'].'">
			<img src="'.$u['picture'].'" alt="'.$u['name'].'" width="60px"/></a>',
		);
		$row['image_'.$u['uid']] = drupal_render($form['image_'.$u['uid']]);
		$form['name_'.$u['uid']] = array(
			'#type' => 'item',
			'#value' => l($u['name'], 'buzzbuy/user/profile/'.$u['uid']),
		);
		$row['name_'.$u['uid']] = drupal_render($form['name_'.$u['uid']]);

		$text = '<span style="font-size:80%">Following ' . $stats['following'];
		$text .= '<br> Follower ' . $stats['followed'];
		$text .= '<br> Reviews ' . $stats['reviews'];
		$text .= '</span>';
		$row['stat_'.$u['uid']] = $text;

		if(buzzbuy_is_following($user->uid, $u['uid'])) {
			//yes, unfollow
			$f = l('Unfollow',  'buzzbuy/user/unfollow/' . $uid.'/'.$u['uid'].'/'.buzzbuy_explode_url($_GET['q']));
		}else{
			//no, follow
			$f = l('Follow',  'buzzbuy/user/follow/' . $uid.'/'.$u['uid'].'/'.buzzbuy_explode_url($_GET['q']));
		}

		$row['option'] = $f;

		$rows[] = $row;
	}
	$output = theme_table(array(), $rows);
	$output .= theme_pager(NULL, 10);

	$form['list'] = array(
		'#type' => 'markup',
		'#value' => $output,
	);

	return $form;
}

function buzzbuy_unfollow($follower, $followed, $backurl) {
	$sql = "DELETE FROM ". BUZZBUY_FOLLOW ." WHERE follower = %d AND followed = %d";
	$res = db_query($sql, $follower, $followed);
	if($res) {
		drupal_set_message("Sucessfully unfollow " .buzzbuy_get_username($followed));
	}
	drupal_goto(buzzbuy_unexplode_url($backurl));
}

function buzzbuy_follower() {
	$table = buzzbuy_get_follower();
	$form = array();
	$form['result'] = array(
		'#type'=>'markup',
		'#value' => $table,
	);
	return $form;
}

function buzzbuy_user_profile($form_state, $uid = NULL) {
	global $user;

	if(!$uid) {
		$uid = $user->uid;
	}

	$person =db_fetch_array(db_query("SELECT picture, name
						FROM users WHERE uid = %d", $uid));
//	drupal_set_title('User Profile: '. $person['name']);
	

	if($uid != $user->uid) {
		if(buzzbuy_is_following($user->uid, $uid)) {
			//yes, unfollow
			$f = l('Unfollow',  'buzzbuy/user/unfollow/' . $user->uid.'/'.$uid.'/'.buzzbuy_explode_url($_GET['q']));
		}else{
			//no, follow
			$f = l('Follow',  'buzzbuy/user/follow/' . $user->uid.'/'.$uid.'/'.buzzbuy_explode_url($_GET['q']));
		}
	}
	$u = buzzbuy_user_basic_stats($uid);
	$markup  = '<div><div style="float:left"><img src="'.$person['picture'].'" width="70px" /></div>';
	$markup .= '<div style="padding-left:100px"><b>'.$person['name'].'</b><br>';
	$markup .= 'Following '.l($u['following'],$_GET['q']). ', Followers '.l($u['followed'],$_GET['q']);
	$markup .= ', Reviews '. l($u['reviews'],$_GET['q']) .'<br>'.$f.'</div></div>';
	$form['img'] = array(
		'#type' => 'markup',
		'#value' => $markup,
	);

	$sql =  "SELECT DISTINCT c.pid, c.comment, c.timestamp,
					i.title, i.detailpageurl, i.productgroup,  m.url
					FROM " .BUZZBUY_COMMENT ." c, amazon_item i, amazon_item_image m
					WHERE c.pid = i.asin AND i.asin = m.asin AND c.uid=%d 
					GROUP BY c.pid ORDER BY c.timestamp DESC";


	$result = db_query($sql, $uid);
	$rows = array();
	while($data = db_fetch_array($result)) {
		$row = array();
		$link = buzzbuy_get_base_url().'buzzbuy/review/view/'.$data['pid'];
		$form['image_'.$data['pid']] = array(
			'#type' => 'markup',
			'#value' => '<a href="'.$link.'">
			<img src="'.$data['url'].'" alt="'.$data['title'].'" width="160px"></a>',
		);
		$row['image_'.$data['pid']] = drupal_render($form['image_'.$data['pid']]);
		$text = '<h3>'. l($data['title'], $link) .'</h3>';
		$text .= '<br><b>' . $data['comment'] .'</b>';
		$text .= '<br><span style="color:#999"> '.Agotime($data['timestamp']) ;
		$text .= ' '. l('Original Site', $data['detailpageurl'], array('attributes'=>array('target'=>'_blank'))) .'</span>';
		$row['text_'.$data['pid']] = $text;

		$rows[] = $row;
	}

	$form['reviews'] = array(
		'#type' => 'markup',
		'#value' => theme_table(array(), $rows),
	);

	return $form;

}
?>