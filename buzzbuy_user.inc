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

function buzzbuy_follow($follower, $followed, $backurl) {
	$sql = "INSERT INTO ".BUZZBUY_FOLLOW." (follower, followed, timestamp) VALUES (%d, %d, NOW())";
	$res = db_query($sql, $follower, $followed);
	if($res) {
		drupal_set_message("Sucessfully follow " .buzzbuy_get_username($followed));
	}
	drupal_goto(buzzbuy_unexplode_url($backurl));
}

function buzzbuy_unfollow($follower, $followed, $backurl) {
	drupal_set_message('lalas');
	$sql = "DELETE FROM ". BUZZBUY_FOLLOW ." WHERE follower = %d AND followed = %d";
	db_query($sql, $follower, $followed);
	if($res) {
		drupal_set_message("Unfollow " .buzzbuy_get_username($followed));
	}
	drupal_goto(buzzbuy_unexplode_url($backurl));
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