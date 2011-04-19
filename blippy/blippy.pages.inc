<?php
// $Id: blippy_signin.pages.inc,v 1.1.2.2 2010/01/23 23:27:21 walkah Exp $

function blippy_admin_settings() {
	$form = array();

	$img_path = drupal_get_path('module', 'blippy_signin') . '/images';
	$results = file_scan_directory($img_path, '.png');
	$form['oauth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Callback URL'),
    '#value' => url('blippy/oauth', array('absolute' => TRUE)),
	);

	$form['oauth']['blippy_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer key'),
    '#default_value' => variable_get('blippy_consumer_key', NULL),
	);

	$form['oauth']['blippy_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer secret'),
    '#default_value' => variable_get('blippy_consumer_secret', NULL),
	);
	$form['blippy_signin_register'] = array(
    '#title' => t('Automatically register new users'),
    '#type' => 'radios',
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => variable_get('blippy_signin_register', 0),
	);

	return system_settings_form($form);
}


function blippy_oauth_callback(&$form_state) {
	$oauth_token = $_SESSION['blippy_oauth']['token']['oauth_token'];
  $form['#post']['oauth_token'] = $oauth_token;
  $form['oauth_token'] = array(
    '#type' => 'hidden',
    '#default_value' => $oauth_token,
  );
  
  
  return $form;
}

function blippy_oauth_callback_validate($form, &$form_state) {
  $key = variable_get('blippy_consumer_key', '');
  $secret = variable_get('blippy_consumer_secret', '');

  $form_state['blippy_oauth'] = $_SESSION['blippy_oauth'];
  unset($_SESSION['blippy_oauth']);

  $token = $form_state['blippy_oauth']['token'];
 
  if (!is_array($token) || !$key || !$secret) {
    form_set_error('oauth_token', t('Invalid Blippy OAuth request'));
  }

  if ($token['oauth_token'] != $form_state['values']['oauth_token']) {
    form_set_error('oauth_token', t('Invalid OAuth token.'));
  }

  module_load_include('lib.php', 'oauth');
  module_load_include('lib.php', 'blippy');
  $blippy = new BlippyOAuth($key, $secret, $token['oauth_token'], $token['oauth_token_secret']);  
	if($_GET['oauth_verifier'])
  $response = $blippy->getAccessToken($_GET['oauth_verifier']);
  $form_state['blippy_oauth']['response'] = $response;
  $form_state['blippy_oauth']['connection'] = $blippy;
}

function blippy_oauth_callback_submit(&$form, &$form_state) {
  $form_state['redirect'] = $form_state['blippy_oauth']['destination'];
  $form['#programmed'] = FALSE;
}