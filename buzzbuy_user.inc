<?php
require_once drupal_get_path("module", BUZZBUY_MODULE_NAME) ."/buzzbuy_api.inc";

function buzzbuy_followed() {
	drupal_set_message("following form");
	$table = buzzbuy_get_followed();
	$form = array();
	$form['result'] = array(
		'#type'=>'markup',
		'#value' => $table,
	);
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

function buzzbuy_follower() {
	drupal_set_message("followed form");
	$table = buzzbuy_get_follower();
	$form = array();
	$form['result'] = array(
		'#type'=>'markup',
		'#value' => $table,
	);
	return $form;
}

function buzzbuy_user_profile($form_state, $uid) {
	drupal_set_message('buzzbuy_user_profile'.$uid);
}
?>