<?php 
require_once drupal_get_path("module", "buzzbuy") ."/buzzbuy_api.inc";

function buzzbuy_list($form, $mode) {
	drupal_set_message("buzzbuy_list ".$mode);
	buzzbuy_get_activity_list($mode);
	debug_base(buzzbuy_unfollow(1,2), 'unfollow');
	//$books = amazon_search_simple_search("drupal");
	//debug_base($books);
	//$amazon="[amazon 0596515804 publicationyear]";
	//$filter = _amazon_filter_process_text($amazon);
	//debug_base($filter);
}
?>