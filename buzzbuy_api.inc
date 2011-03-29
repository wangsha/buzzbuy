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

function buzzbuy_follow($follower, $followed) {
	$sql = "INSERT INTO ".BUZZBUY_FOLLOW." (follower, followed) VALUES (%d, %d)";
	$res = db_query($sql, $follower, $followed);
	return $res;
}

function buzzbuy_unfollow($follower, $followed) {
	$sql = "DELETE FROM ".BUZZBUY_FOLLOW ." WHERE follower=%d AND followed=%d";
	$res = db_query($sql, $follower, $followed);
	return $res;
}


?>