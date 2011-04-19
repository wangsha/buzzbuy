<?php 
/**
 * Library Path
 */
define('BLIPPY_LIB_PATH', drupal_get_path("module", "buzzbuy") . '/blippylib');

/**
 * Tables
 */
define('BUZZBUY_ACTIVITY', 'buzzbuy_activity');
define('BUZZBUY_COMMENT', 'buzzbuy_comment');
define('BUZZBUY_FRIENDLIST',	'buzzbuy_friendlist');
define('BUZZBUY_FOLLOW',	'buzzbuy_follow');
define('BUZZBUY_CONNECTIONS', 'buzzbuy_connections');

/**
 * Connections
 */
define('BUZZBUY_NATIVE', 0);
define('BUZZBUY_FB_CONNECT', 1);
define('BUZZBUY_BLIPPY_CONNECT', 2);

/**
 * Permissions
 */
define('BUZZBUY_REGISTERED_PERM', 'buzzbuy_registered');
define('BUZZBUY_ANONYMOUS_PERM', 'buzzbuy_anonymous');

/**
 * Follow Mode
 */
define('BUZZBUY_FOLLOW_MODE_FOLLOWER', 0);
define('BUZZBUY_FOLLOW_MODE_FOLLOWED', 1);

/**
 * Product Rating
 */
define('BUZZBUY_RECOMMEND', 0);
define('BUZZBUY_MAYBE', 1);
define('BUZZBUY_NO', 2);

?>