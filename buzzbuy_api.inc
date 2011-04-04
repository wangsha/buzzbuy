<?php

function debug_base($array, $str=NULL){
	if(true){
		drupal_set_message("<pre>" . " (" . $str . ") " . print_r($array, true). "</pre>");
	}
}

function buzzbuy_get_activity_list($mode, $uid = null) {
	if(empty($uid)) {
		global $user;
		$uid = $user->uid;
	}
	$sql = "SELECT asid, activity, rating, review, timestamp FROM " .BUZZBUY_ACTIVITY_LIST;

	switch($mode) {
		case 'all':
			break;
		case 'followed':
			$sql .= " WHERE uid IN (SELECT follower FROM " .BUZZBUY_FOLLOW. " WHERE followed =".$uid.")";
			break;
		case 'following':
			$sql .= " WHERE uid IN (SELECT followed FROM " .BUZZBUY_FOLLOW. " WHERE follower =".$uid.")";
			break;
	}
	$sql .=" ORDER BY timestamp DESC";

	$result = pager_query($sql);
	$rows = array();
	while($data = db_fetch_object($result)) {
		$rows[] = $data;
	}
	return $rows;
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

	$follow_path = 'buzzbuy/follow';
	$follow_text = 'Follow';
	if($mode == BUZZBUY_FOLLOW_MODE_FOLLOWER) {
		$follow_path = 'buzzbuy/unfollow';
		$follow_text = 'Unfollow';
	}
	$rows = array();
	foreach($data as $id => $u) {
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

		$form['unfollow_'.$u['uid']] = array(
			'#type' => 'markup',
			'#value' => l($follow_text, $follow_path. '/' . $uid.'/'.$u['uid']),
		);
		$row['unfollow_'.$u['uid']] = drupal_render($form['unfollow_'.$u['uid']]);

		$rows[] = $row;
	}

	$table = theme_table(array(), $rows);
	return $table;
}

?>