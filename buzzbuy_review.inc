<?php
require_once drupal_get_path("module", "buzzbuy") ."/buzzbuy_api.inc";

function buzzbuy_review(&$form_state, $asin) {
	drupal_add_js(drupal_get_path("module", "buzzbuy")."/buzzbuy.js");

	$form = array();

	global $user;
	debug_base($user);
	debug_base($_COOKIE);
	list($module, $review, $mode) = explode('/', $_GET['q']);
	$form['page_mode'] = array(
    '#type'=>'hidden',
    '#id'=>'page_mode',
    '#value'=>$mode,
	);

	$item = amazon_item_lookup_from_web(array($asin));
	$item = $item[$asin];
	$form_state['product'] = $item;

	$title = $item['title'];
	$detailurl = $item['detailpageurl'];
	$customer_review = $item['customerreviews_iframe'];
	$img = _amazon_filter_process_text("[amazon ".$asin." mediumimage]");
	$price = $item['listpriceformattedprice'];
	$productgroup = $item['productgroup'];

	$markup = '<div style="float:left; width:160px">' . $img .'</div>';
	$markup .= '<div style="display:block; padding-left:170px; height:160px"><h1>' . $title .'</h1>';
	$markup .= '<span style="color:#777; font-size:11px; padding-top:q0px">' .$price. ' - ' . $productgroup . '</span>';
	$markup .= '</div>';
	$form['product'] = array(
		'#type' => 'markup',
		'#value' => $markup,
	);

	$form['review'] = array(
		'#type' => 'textarea',
		'#title' => 'Write a review',
		'#value' => '',
		'#prefix' => '<div style= "padding-top:20px">',
		'#suffix' => '</div>',
	);

	/*
	 $recommend = '<b>Recommend? </b><span style="border:1px solid #CCC; padding:0px 4px 0px 4px"><a href="#" id="r_0" onClick="setRecommend(this); return false;">YES</a></span>';
	 $recommend .= '<span style="border:1px solid #CCC; padding:0px 4px 0px 4px"><a href="#" id="r_1" onclick="setRecommend(this); return false;">MAYBE</a></span>';
	 $recommend .= '<span style="border:1px solid #CCC; padding:0px 4px 0px 4px"><a href="#" id="r_2" onclick="setRecommend(this); return false;">NO</a></span>';
	 $form['recommend'] = array(
		'#type' => 'hidden',
		'#id' => 'recommend',
		'#value' => '0',
		);

		$form['markup1'] = array(
		'#type' => 'markup',
		'#value' => $recommend,
		);
		*/
	$form['publish'] = array(
		'#type' => 'submit',
		'#value' => 'Publish',
		'#prefix' => '<br><div style="float:left; padding-top:10px">',
		'#suffix' => '</div>',
	);

	if(_is_user_facebook_connected()) {
		$fb_favicon = theme('stream_publish_favicon');
		$form['facebook'] = array(
			'#type' => 'checkbox',
			'#title' => $fb_favicon . ' Share on Facebook',
			'#name' => 'facebook',
			'#id' => 'facebook',
 			'#value' => 1,
			'#prefix' => '<div style="float:left; color:#999; padding-top:5px; padding-left:5px">',
			'#suffix' => '</div>',
		);
	}

	//share on twitter
	if(_is_user_twitter_connected()) {
		$tt_favicon = '<img src="'. drupal_get_path("module", 'buzzbuy') .'/twitter.ico" />';
		$form['twitter'] = array(
			'#type' => 'checkbox',
			'#title' => $tt_favicon . ' Share on Twitter',
			'#name' => 'twitter',
			'#id' => 'twitter',
			'#value' => 1,
		  '#prefix' => '<div style="float:left; color:#999; padding-top:4px; padding-left:5px">',
			'#suffix' => '</div>',
		);
	}
	return $form;
}

function buzzbuy_review_submit(&$form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$values = $form_state['values'];
	$page_mode = $values['page_mode'];
	$product = $form_state['product'];
	$product['img'] = $product['imagesets']['mediumimage']['url'];
	buzzbuy_insert_product($product);
	
	global $user;
	$data['uid'] = $user->uid;
	$data['recommend'] = BUZZBUY_RECOMMEND;
	$data['timestamp'] = time();
	$data['comment'] = $_POST['review'];
	$data['pid'] = $product['asin'];

	buzzbuy_insert_comment($data);
	$data['name'] = $product['title'];
	$data['message'] = $data['comment'];
	$data['link'] = buzzbuy_get_base_url() . 'buzzbuy/review/view/'.$data['pid'];
	$data['picture'] = $product['img'];
	
	if($_POST['facebook']) {
		buzzbuy_post_to_facebook($data);
		
	}

	if($_POST['twitter']) {
		buzzbuy_post_to_twitter($data);
	}

}
?>