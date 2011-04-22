<?php
require_once drupal_get_path("module", "buzzbuy") ."/buzzbuy_api.inc";

function buzzbuy_list(&$form_state, $mode = null) {
	$form = array();
	$form['markup'] = array(
		'#type' => 'markup',
		'#value' => buzzbuy_get_activity_list($form_state, $form, $mode),
	);
	return $form;
}



function buzzbuy_list_submit($form, $form_state) {

	$bn = $form_state['clicked_button'];
	$products = $form_state['products'];
	if($bn['#value'] == 'Write a Review') {
		$pid = substr($bn['#id'], 7);
		$data['pid'] = $pid;
		$product = $products[$pid];
		$fb = $bn['#post']['fb_'.$pid];
		$bp = $bn['#post']['bp_'.$pid];
		$data['comment'] = $bn['#post']['comment_'.$pid];
		global $user;
		$data['uid'] = $user->uid;
		$data['recommend'] = BUZZBUY_RECOMMEND;
		$data['timestamp'] = time();
		buzzbuy_insert_comment($data);
		$data['name'] = $product['title'];
		$data['message'] = $data['comment'];
		$data['link'] = buzzbuy_get_base_url() . 'buzzbuy/review/view/'.$pid;
		$data['picture'] = $product['url'];
		$post = $form_state['clicked_button']['#post'];
		if($post['fb_'.$pid]) {
			buzzbuy_post_to_facebook($data);
		}
		if($post['tt_'.$pid]) {
			buzzbuy_post_to_twitter($data);
		}
	}
}

?>