<?php 
require_once drupal_get_path("module", BUZZBUY_MODULE_NAME) ."/buzzbuy_api.inc";

function buzzbuy_following() {
	global $user;
	$uid = $user->uid;
	drupal_set_message("following form");
	$data = buzzbuy_get_following();
	debug_base($data);
	$form = array();

	foreach($data as $id => $u) {
		$form['unfollow_'.$u['uid']] = array(
			'#type' => 'markup',
			'#value' => l('Unfollow', 'buzzbuy/follow/'.$uid.'/'.$u['uid']),
		);
		$form['activity_'.$u['uid']] = array(
			'#type' => 'submit',
			'#value' => 'View Activity',
		);
		$form['profile_'.$u['uid']] = array(
			'#type' => 'submit',
			'#value' => 'Profile',
		);
	}
	
	return $form;
}

function buzzbuy_follow($follower, $followed) {
	$sql = "INSERT INTO ".BUZZBUY_FOLLOW." (follower, followed) VALUES (%d, %d)";
	$res = db_query($sql, $follower, $followed);
}

function buzzbuy_unfollow($uid1, $uid2) {
	drupal_set_message('buzzbuy_unfollow');
	$sql = "DELETE FROM ". BUZZBUY_FOLLOW ." WHERE follower = %d AND followed = %d";
	db_query($sql, $follower, $followed);
	drupal_goto('buzzbuy/user/following/'.$uid1);
}

function buzzbuy_followed() {
	drupal_set_message("followed form");
}
?>