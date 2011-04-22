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
	$form['people'] = array(
		'#type'=>'fieldset',
		'#title' => 'People',
		'#collapsible'=> false,
		'#collapsed'=> false
	);

	$form['people']['people_result'] = array(
		'#type'=>'markup',
		'#value'=> buzzbuy_people_search($text),
	);

	$form['product'] = array(
		'#type'=>'fieldset',
		'#title' => 'Products',
		'#collapsible'=> false,
		'#collapsed'=> false
	);

	$form['product']['peoduct_result'] = array(
		'#type'=>'markup',
		'#value'=> buzzbuy_product_search($text),
	);
	return $form;
}

function buzzbuy_people_search($text) {
	$sql =" SELECT DISTINCT a.uid, a.profile_image_url as img, a.username,
					COUNT(c.id) as count 
					FROM blippy_account a LEFT JOIN buzzbuy_comment c ON c.uid = a.uid
					WHERE (LOWER(username) LIKE LOWER('%%%s%') 
					OR LOWER(name) LIKE LOWER('%%%s%'))
				  GROUP BY a.uid";
	$result = db_query($sql, $text, $text);
	$rows = array();
	while($data = db_fetch_array($result)) {
		$row = array();
		$row['img'] = '<img src="'. $data['img'] .'">';
		$row['name'] = '<br>' .l($data['username'], 'buzzbuy/user/profile/'.$data['uid']);
		$row['count'] = '<br><img src="'. drupal_get_path('module', 'buzzbuy') .'/review.png">' . $data['count'] . ' purchase reviews';

		$rows[] = $row;
	}
	$header = array(
	array('data'=>'Image', 'field'=>'img'),
	array('data'=>'Name', 'field'=>'name'),
	array('data'=>'Total Reviews', 'field'=>'count'),
	);
	return theme_table(array(), $rows);
}

function buzzbuy_product_search($text) {
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
		return theme_table($header, $rows);
	}
}

function buzzbuy_search_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}
?>

