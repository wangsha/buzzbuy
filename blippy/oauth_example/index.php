<?php
/**
 * @file
 * User has successfully authenticated with Twitter. Access tokens saved to session and DB.
 */

/* Load required lib files. */
session_start();
require_once('../lib/blippy.php');
require_once('config.php');

/* If access tokens are not available redirect to connect page. */
if (empty($_SESSION['access_token']) || empty($_SESSION['access_token']['oauth_token']) || empty($_SESSION['access_token']['oauth_token_secret'])) {
    header('Location: ./clearsessions.php');
}
/* Get user access tokens out of the session. */
$access_token = $_SESSION['access_token'];

/* Create a TwitterOauth object with consumer/user tokens. */
$connection = new BlippyOAuth(CONSUMER_KEY, CONSUMER_SECRET, $access_token['oauth_token'], $access_token['oauth_token_secret']);
$b = new Blippy($connection);

/* Some example calls */
$out = array();
$pagination = array('count' => 5); 

$out["credentials"] = $b->verifyCredentials();
#$out["userfeed"] = $b->getUserFeed();
$out["rate_limit"] = $b->rateLimitStatus(); 

# Feeds 
$out['feed'] = $b->feed($pagination); 
$out['friend_feed'] = $b->friendsFeed($pagination);
$out['latest_feed'] = $b->latestFeed($pagination);

# Items

# Users 
$out['user_latest']       = $b->getUsersLatest('ryan', $pagination);
$out['user_reviwed']      = $b->getUsersReviewed('ryan', $pagination);
$out['user_unreviewed']   = $b->getUsersUnreviewed('ryan', $pagination);
$out['user_unpublished']  = $b->getUsersUnpublished($pagination);
$out['user_ifo']          = $b->getUsersInfo('cte');

# Relationships 
$out['follow'] = $b->followUser('ryan');
$out['block']  = $b->blockUser('ryan_test');


/* Include HTML to display on the page */
include('html.inc');
