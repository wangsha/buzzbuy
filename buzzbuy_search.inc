<?php
require_once drupal_get_path("module", "buzzbuy") ."/buzzbuy_api.inc";
drupal_add_js(drupal_get_path("module", "buzzbuy")."/buzzbuy.js");

function buzzbuy_search(&$form_state) {
	$form = array();

	$text = $_POST['search'];

	$form['search'] = array(
	  '#type' => 'textfield',
		'#default_value' => $text,
	  '#prefix' => '<table width="100%" border=0 rules="none" cellspacing=0 cellpadding=0><tr><td width="128px">',
    '#suffix' => '</td>',
	);
	$form['search_submit'] = array(
   	'#type' => 'submit',
    '#name' => 'normal_search',
	  '#id' => 'normal_search',
    '#value' => t('Search'),
    '#prefix' => '<td style="padding-top:20px">',
    '#suffix' => '</td></tr></table>',  
	);

	$form['result'] = array(
		'#type'=>'fieldset',
		'#title' => 'Search Result',
		'#collapsible'=> false,
		'#collapsed'=> false
		);

		$form['result']['search_result'] = array(
		'#type'=>'markup',
		'#value'=> buzzbuy_get_search_result($text),
		);
		return $form;
}

function buzzbuy_get_search_result($text) {
	if (strlen($text)>0) {
		$result = amazon_search_simple_search($text);
		$header = array(
		array('data'=>'Image', 'field'=>'img'),
		array('data'=>'Title', 'field'=>'title'),
		array('data'=>'Price', 'field'=>'price'),
		array('data'=>'Product Group', 'field'=>'productgrouop'),
		array('data'=>'Options', 'field'=>'options'),
		);
		
		$rows = array();
		foreach($result as $asin => $item) {
			$row = array();
			$row['img'] = _amazon_filter_process_text("[amazon ".$asin." thumbnail]");
			$row['title'] = l($item['title'], $item['detailpageurl']);
			$row['price'] = $item['listpriceformattedprice'];
			$row['productgroup'] = $item['productgroup'];
			$row['options'] = l('Review', 'buzzbuy/review/add/'.$asin);
			$rows[] = array('data'=> $row);
		}
		
		//debug_base(theme_table($header, $rows), 'table');
		return theme_table($header, $rows);
	}
}

function buzzbuy_search_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}
?>

