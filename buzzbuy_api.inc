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
	$sql = "SELECT cname, cid FROM ". BUZZBUY_CONNECTIONS .
				 " WHERE uid IN (SELECT follower FROM ". BUZZBUY_FOLLOW . " WHERE followed = %d)".
				 " AND ctype=%d";
	$result = db_query($sql, $uid, BUZZBUY_FB_CONNECT);
	$rows = array();
	while($data = db_fetch_array($result)) {
		$rows[] = $data;
	}
	return $rows;
}

function buzzbuy_get_followed($uid = null) {
	if(empty($uid)) {
		global $user;
		$uid = $user->uid;
	}
	$sql = "SELECT cname, cid FROM ". BUZZBUY_CONNECTIONS .
				 " WHERE uid IN (SELECT followed FROM ". BUZZBUY_FOLLOW . " WHERE follower = %d)".
				 " AND ctype=%d";
	$result = db_query($sql, $uid, BUZZBUY_FB_CONNECT);
	$rows = array();
	while($data = db_fetch_array($result)) {
		$rows[] = $data;
	}
	return $rows;
}

?>