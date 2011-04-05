<?php 
require_once drupal_get_path("module", "buzzbuy") ."/buzzbuy_api.inc";

function buzzbuy_list(&$form_state, $mode) {
	drupal_set_message("buzzbuy_list ".$mode);
	$form['markup'] = array(
		'#type' => 'markup',
		'#value' => buzzbuy_get_activity_list($form, $mode),
	);
	return $form;
	//debug_base(buzzbuy_unfollow(1,2), 'unfollow');
	//$books = amazon_search_simple_search("drupal");
	//debug_base($books);
	//$amazon="[amazon 0596515804 publicationyear]";
	//$filter = _amazon_filter_process_text($amazon);
	//debug_base($filter);
}

function buzzbuy_list_submit($form, $form_state) {
	//debug_base($form_state);
	$bn = $form_state['clicked_button'];
	
	if($bn['#value'] == 'Add Comment') {
		$pid = substr($bn['#id'], 7);
		$data['pid'] = $pid;
		
		$fb = $bn['#post']['fb_'.$pid];
		$bp = $bn['#post']['bp_'.$pid];
		$data['comment'] = $bn['#post']['comment_'.$pid];
		global $user;
		$data['uid'] = $user->uid;
		$data['recommend'] = BUZZBUY_RECOMMEND;
		$data['timestamp'] = time();
		debug_base($data);
		buzzbuy_insert_comment($data);
	}
}

?>