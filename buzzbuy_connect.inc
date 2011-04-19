<?php
require_once drupal_get_path("module", "buzzbuy").'/buzzbuy_constants.php';
require_once drupal_get_path("module", "buzzbuy").'/buzzbuy_api.inc';

function buzzbuy_connect() {
	/* Build an image link to start the redirect process. */
	$content = '<a href="?q=buzzbuy/connect/blippy/redirect"><img src="'.BLIPPY_LIB_PATH.'/blippy_signin-solo.png" alt="Sign in with blippy"/></a>';
	$form = array();
	$form['link'] = array(
		'#type' => 'markup',
		'#value' => $content,
	);

	return $form;
}

?>