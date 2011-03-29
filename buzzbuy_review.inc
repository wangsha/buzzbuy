<?php
require_once drupal_get_path("module", "buzzbuy") ."/buzzbuy_api.inc";

function buzzbuy_review(&$form_state, $asin) {
	drupal_add_js(drupal_get_path("module", "buzzbuy")."/buzzbuy.js");
	
	$form = array();


	list($module, $review, $mode) = explode('/', $_GET['q']);
	$form['page_mode'] = array(
    '#type'=>'hidden',
    '#id'=>'page_mode',
    '#value'=>$mode,
	);
	$item = amazon_item_lookup_from_web(array($asin));

	$item = $item[$asin];
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
		'#title' => 'Write a comment',
		'#value' => '',
		'#prefix' => '<div style= "padding-top:20px">',
		'#suffix' => '</div>',
	);

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

	$form['publish'] = array(
		'#type' => 'submit',
		'#value' => 'Publish',
		'#prefix' => '<br><div style="float:left; padding-top:10px">',
		'#suffix' => '</div>',
	);
	$form['share'] = array(
		'#type' => 'checkbox',
		'#title' => 'Share on Facebook',
		'#value' => 1,
		'#prefix' => '<div style="padding-left:80px; padding-top:2px">',
		'#suffix' => '</div>',
	);
	return $form;
}

function buzzbuy_review_submit(&$form, &$form_state) {
	debug_base($_POST, 'post');
	$form_state['rebuild'] = TRUE;
	
	debug_base($form_state);
	$values = $form_state['values'];
	$page_mode = $values['page_mode'];
	
	$review = $_POST['review'];
	$recommend = $_POST['recommend'];
	
	
}
?>