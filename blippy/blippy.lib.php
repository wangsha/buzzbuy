<?php
/*
Copyright (c) 2010 Blippy - blippy.com  
Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:
 
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

This client was based on Abraham Williams' Twitter OAuth client 
http://abrah.am - abraham@poseurte.ch - http://github.com/abraham/twitteroauth
*/

/**
 * For latest documentation please vist: 
 * @see http://developer.blippy.com/developers/docs
 * For returned data definitions: 
 * @see http://developer.blippy.com/developers/docs#data-types
**/

class Blippy
{
	private $con; 
	public function __construct(BlippyConnection $connectionObj)
	{
		$this->con = $connectionObj;
	}
	
	
	/**
	  * Administrative
		* @see http://developer.blippy.com/developers/docs#accounts
	**/
	
	/**
	*	Returns a User object for the authenticating user, or returns a 401 Unauthorized if the credentials are wrong.
	* 
	* - Method: GET
	* - Requires authentication: true
	* - Rate limited: true
	*
	* @return User $userObj for the authenticating user, or returns a 401 Unauthorized if the credentials are wrong.
	**/
	public function verifyCredentials()
	{
		return $this->con->get("account/verify_credentials"); 
	}
	
	/**
	 * Gets the rate limit data for the authenticating user, or current IP if no credentials are provided.
	 * @return Array $limit
	 **/
	public function rateLimitStatus()
	{
		return $this->con->get("account/rate_limit_status");
	}
	
	/**
	 * FEEDS 
	 * @see http://developer.blippy.com/developers/docs#feeds
	**/
	
	/**
	* A list of the most recent items from everyone with activity.
	* - Method: GET
	* - Requires authentication: false
	* - Rate limited: true
	* @param Array $params Array of valid pagination params 
	* @return Array $itemObjects
	**/
	public function feed($params = array())
	{
		return $this->con->get('feed', $params);
	}
	
	/**
	* A list of the most recent items from everyone with activity.
	* - Method: GET
	* - Requires authentication: false
	* - Rate limited: true
	* @param Array $params Array of valid pagination params 
	* @return Array $itemObjects
	**/
	public function latestFeed($params = array())
	{
		return $this->con->get('latest_feed', $params);
	}
	
	/**
	* A list of the most recent items from everyone with activity.
	* - Method: GET
	* - Requires authentication: true
	* - Rate limited: true
	* @param Array $params Array of valid pagination params
	* @return Array $itemObjects
	**/
	public function friendsFeed($params = array())
	{
		return $this->con->get('friends_feed', $params);
	}

	
	/** 
	 * ITEMS
	 * @see http://developer.blippy.com/developers/docs#items
	**/
	
	/**
	 * Fetches a single item, including objects for the items's user, comments, and reactions
	 * 		- Method: GET
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 	
	 * @param mixed $itemId The id of the item
	 * @return Item $itemObject
	 * 
	 **/
	public function fetchItem($id)
	{
		return $this->con->get("items/$id");
	}
	
	/**
	 * Has the authenticating user omgwtfs the specified item
	 * 
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 	
	 * @param mixed $itemId The id of the item
	 * @return Item $itemObject
	 * 
	 **/
	public function omgwtfItem($id)
	{
		return $this->con->post("items/$id/omgwtf");
	}
	
	/**
	 * Authenticating user awesome the specified item
	 *  	
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 
	 * @param mixed $itemId The id of the item
	 * @return Item $itemObject
	 * 
	 **/
	public function awesomeItem($id)
	{
		return $this->con->post("items/$id/awesome");
	}
	
	/**
	 * Authorizing user asks about specified item (same as "What'd you get?" and "What'd you think?" links on web UI)
	 * 
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 	
	 * @param mixed $itemId The id of the item
	 * @return Item $itemObject
	 * 
	 **/
	public function nudgeItem($id)
	{
		return $this->con->post("transactions/$id/nudges");
	}
	
	/**
	 * Fills in the 'description' field for a item
	 *  	
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 	
	 * @param mixed $itemId The id of the item
	 * @param string $review The review text
	 * @return Item $itemObject
	 * 
	 **/
	public function reviewItem($id, $review = '')
	{
		return $this->con->post("items/$id/review", array('body' => $review));
	}
	
	/**
	 * Clears the item's review
	 * 	
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 
	 * @param mixed $itemId The id of the transaction
	 * @return Item $itemObject
	 * 
	 **/
	public function deleteReview($id)
	{
		return $this->con->post("items/$id/unreview");
	}
	
	
	
	/**
	 * COMMENTS 
	 * @see http://developer.blippy.com/developers/docs#comments
	**/ 
	/**
	 * Post a new comment to an item
	 * 
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: true
	 * 	
	 * @param mixed $itemId The id of the item
	 * @param String $body the text of the new comment	
	 * @return Comment $commentObj
	 * 
	 **/
	public function addComment($id, $body)
	{
		return $this->con->post("items/$id/comments", array("comment[body]" => $body));
	}
	/**
	 * Remove a comment
	 * 
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 	
	 * @param mixed $itemId The id of the item
	 * @param mixed $commentId	
	 * @return Comment $commentObj
	 **/
	public function removeComment($commentId)
	{
		return $this->con->post("comments/$commentId/remove");
	}
	
	
	/**
	 * USERS 
	 * @see http://developer.blippy.com/developers/docs#users
	**/
	
	/**
	 * Fetches the details for the user named by <screen name>, including their most recent transaction
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: false
	 * 		- Rate limited: true
	 *
	 * @param String $username the username of the user (may be protected)
	 * @return $userObject
	 **/
	public function getUsersInfo($username)
	{
		return $this->con->get("$username/info");
	}
	
	/**
	 * Gets the user's reviewed items.
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: false
	 * 		- Rate limited: true
	 *
	 * @param String $username the username of the user (may be protected)
	 * @param Array $params Array of valid pagination params 
	 * @return Array $itemObjects
	 **/
	public function getUsersReviewed($username, $params = array())
	{
		return $this->con->get("$username/reviewed", $params);
	}
	
	/**
	 * Gets the user's unreviewed items.
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: false
	 * 		- Rate limited: true
	 *
	 * @param String $username the username of the user (may be protected)
	 * @param Array $params Array of valid pagination params 
	 * @return Array $itemObjects
	 **/
	public function getUsersUnreviewed($username, $params = array())
	{
		return $this->con->get("$username/unreviewed", $params);
	}
	
	/**
	 * Gets the logged in user's unpublished items.
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: true
	 * 		- Rate limited: true
	 *
	 * @param Array $params Array of valid pagination params 
	 * @return Array $itemObjects
	 **/
	public function getUsersUnpublished($params = array())
	{
		return $this->con->get("users/unpublished", $params);
	}
	
	/**
	 * Gets the logged in user's latest items.
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: false
	 * 		- Rate limited: true
	 *
	 * @param String $username the username of the user (may be protected)
	 * @param Array $params Array of valid pagination params 
	 * @return Array $itemObjects
	 **/
	public function getUsersLatest($username, $params = array())
	{
		return $this->con->get("$username", $params);
	}
	
	
	
	/**
	 * RELATIONSHIPS
	 * @see http://developer.blippy.com/developers/docs#relationships
	**/
	/**
	 * sers followed by [screen_name]
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: false
	 * 		- Rate limited: true
	 * 	
	 * @param String $username the username of the user (may be protected)
	 * @return Array of User objects
	 **/
	public function getUserFollowing($username)
	{
		return $this->con->get("$username/following");
	}
	
	/**
	 * List of user IDs followed by [screen_name]
	 * 
	 * 		Method: GET
	 * 		Requires authentication: false
	 * 		Rate limited: true
	 * 
	 * @param String $username the username of the user (may be protected)
	 * @return Array of User objects
	 **/
	public function getUserFollowingIds($username)
	{
		return $this->con->get("$username/following_ids");
	}
	
	/**
	 * Users following [screen_name]
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: false
	 * 		- Rate limited: true
	 * 	
	 * @param String $username the username of the user (may be protected)
	 * @return Integer list
	 **/
	public function getUserFollowers($username)
	{
		return $this->con->get("$username/followers");
	}
	
	/**
	 * List of IDs for the users following [screen_name]
	 * 
	 * 		Method: GET
	 * 		Requires authentication: false
	 * 		Rate limited: true
	 * 
	 * @param String $username the username of the user (may be protected)
	 * @return Array $integers
	 **/
	public function getUserFollowersIds($username)
	{
		return $this->con->get("$username/followers_ids");
	}
	

	/**
	 * Users blocked by <screen name>
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: true
	 * 		- Rate limited: true
	 * 
	 * @param String $username the username of the user (may be protected)
	 * @return Array of User objects
	 **/
	public function getUserBlocking($username)
	{
		return $this->con->get("$username/blocking");
	}
	
	/**
	 * IDs of users blocked by [screen_name]
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: true
	 * 		- Rate limited: true
	 * 
	 * @param String $username the username of the user (may be protected)
	 * @return Array $integers
	 **/
	public function getUserBlockingIds($username)
	{
		return $this->con->get("$username/blocking_ids");
	}
	
	/**
	 * Authenticating user follows <screen name>, or requests to, if that user is protected
	 * 
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: true
	 * 	
	 * @param String $username the username of the user (may be protected)
	 * @return Friendship $friendshipObj
	 **/
	public function followUser($username)
	{
		return $this->con->post("$username/follow");
	}
	/**
	 * Authenticating user unfollows <screen name>
	 * 	
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 	
	 * @param String $username the username of the user (may be protected)
	 * @return Friendship $friendshipObj
	 **/
	public function unfollowUser($username)
	{
		return $this->con->post("$username/unfollow");
	}
	
	/**
	 * Have the authenticating user approve <screen name>'s friendship request
	 * 
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 	
	 * @param String $username the username of the user (may be protected)
	 * @return Friendship $friendshipObj
	 **/
	public function approveFriendRequest($username)
	{
		return $this->con->post("$username/approve");
	}
	
	/**
	 * Have the authenticating user deny <screen name>'s friend request
	 * 
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 	
	 * @param String $username the username of the user (may be protected)
	 * @return Friendship $friendshipObj
	 **/
	public function denyFriendRequest($username)
	{
		return $this->con->post("$username/deny");
	}
	
	/**
	 * Authenticating user blocks <screen name>.
	 * 
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * @param String $username the username of the user (may be protected)
	 * @return Block $blockObj
	 **/
	public function blockUser($username)
	{
		return $this->con->post("$username/block");
	}
	
	/**
	 * Authenticating user unblocks <screen name>
	 * 
	 * 		- Method: POST
	 * 		- Requires authentication: true
	 * 		- Rate limited: false
	 * 	
	 * @param String $username the username of the user (may be protected)
	 * @return Array $response decoded json response
	 **/
	public function unblockUser($username)
	{
		return $this->con->post("$username/unblock");
	}
	
	
	/* PRODUCTS */
	/**
	 * Search products by name
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: false
	 * 		- Rate limited: true
	 * 	
	 * @param String $q Search string 
	 * @param Mixed $page which page of results
	 * @param Mixed $perPage how many results per page 
	 * @return Array of Business objects
	 **/
	public function productSearch($q, $page = 1, $perPage = 30)
	{
		return $this->con->get("products/search", array("q" => $q, "page" => $page, "per_page" => $perPage)); 
	}
	
	/**
	 * Get the object for a product
	 * 
	 * 		- Method: GET
	 * 		- Requires authentication: false, but some items may be protected
	 * 		- Rate limited: true
	 * 	
	 * @param mixed $id the porduct id 
	 * @return Product $productObj
	 * 
	 **/
	public function productInfo($id)
	{
		$this->con->get("products/$id/info");
	}
	/**
	 * Gets the most recent transactions for a product
	 * 	
	 * 		- Method: GET
	 * 		- Requires authentication: false, but some items may be protected
	 * 		- Rate limited: true
	 * 	
	 * @param mixed $id the product id
	 * @param Mixed $page which page of results
	 * @return Array of Item objects
	 **/
	public function getProductItems($id, $page = 1)
	{
		return $this->con->get("products/$id", array("page" => $page));
	}
	

}

class BlippyConnection
{
	/* Contains the last HTTP status code returned. */
	public $http_code;
	/* Contains the last API call. */
	public $url;

	/* Set timeout default. */
	public $timeout = 30;
	/* Set connect timeout. */
	public $connecttimeout = 30;
	/* Verify SSL Cert. */
	public $ssl_verifypeer = FALSE;
	/* Respons format. */
	public $format = 'json';
	/* Decode returned json data. */
	public $decode_json = TRUE;
	/* Contains the last HTTP headers returned. */
	public $http_info;
	/* Set the useragnet. */
	public $useragent = 'BlippyPHP v1';

	/* Set up the API root URL. */
	public $host = "http://api.blippy.com/api/alpha/";
}

class BlippyOAuth extends BlippyConnection
{
	
	public static $REQUEST_TOKEN_URL = "http://api.blippy.com/oauth/request_token";
	public static $ACCESS_TOKEN_URL = "http://api.blippy.com/oauth/access_token";
	public static $AUTHORIZE_URL = "http://api.blippy.com/oauth/authorize";
	
	function lastStatusCode ()
	{
		return $this->http_status;
	}

	function lastAPICall ()
	{
		return $this->last_api_call;
	}

	function __construct ($consumer_key, $consumer_secret, $oauth_token = NULL, $oauth_token_secret = NULL)
	{
		$this->sha1_method = new OAuthSignatureMethod_HMAC_SHA1();
		$this->consumer = new OAuthConsumer($consumer_key, $consumer_secret);
		if (! empty($oauth_token) && ! empty($oauth_token_secret))
		{
			$this->token = new OAuthConsumer($oauth_token, $oauth_token_secret);
		}
		else
		{
			$this->token = NULL;
		}
	}
	/**
	 * Get a request_token from Blippy
	 *
	 * @return a key/value array containing oauth_token and oauth_token_secret
	**/
	function getRequestToken ($oauth_callback = NULL)
	{
		$parameters = array();
		if (! empty($oauth_callback))
		{
			$parameters['oauth_callback'] = $oauth_callback;
		}
	
		$request = $this->oAuthRequest(self::$REQUEST_TOKEN_URL, 'GET', $parameters);
		$token = OAuthUtil::parse_parameters($request);
		$this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
		return $token;
	}

	/**
	* Get the authorize URL
	*
	* @returns a string
	*/
	function getAuthorizeURL ($token, $sign_in_with_blippy = TRUE)
	{ 
		if (is_array($token))
		{
			$token = $token['oauth_token'];
		}
		return self::$AUTHORIZE_URL . "?oauth_token={$token}";
	}

	/**
	* Exchange request token and secret for an access token and
	* secret, to sign API calls.
	*
	* @returns array("oauth_token" => "the-access-token",
	*                "oauth_token_secret" => "the-access-secret",
	*                "user_id" => "9436992",
	*                "screen_name" => "johndoe")
	*/
	function getAccessToken ($oauth_verifier = FALSE)
	{
		$parameters = array();
		if (! empty($oauth_verifier))
		{
			$parameters['oauth_verifier'] = $oauth_verifier;
		}
	
		$request = $this->oAuthRequest(self::$ACCESS_TOKEN_URL, 'GET', $parameters);
		$token = OAuthUtil::parse_parameters($request);
		$this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
		return $token;
	}

	/**
	 * GET wrapper for oAuthRequest.
	**/
	public function get ($url, $parameters = array())
	{
		$response = $this->oAuthRequest($url, 'GET', $parameters);
		if ($this->format === 'json' && $this->decode_json)
		{
			return json_decode($response, true);
		}
		return $response;
	}

	/**
	* POST wrapper for oAuthRequest.
	**/
	public function post ($url, $parameters = array())
	{
		$response = $this->oAuthRequest($url, 'POST', $parameters);
		if ($this->format === 'json' && $this->decode_json)
		{
			return json_decode($response, true);
		}
		return $response;
	}

	/**
	* DELETE wrapper for oAuthReqeust.
	**/
	public function delete ($url, $parameters = array())
	{
		$response = $this->oAuthRequest($url, 'DELETE', $parameters);
		if ($this->format === 'json' && $this->decode_json)
		{
			return json_decode($response, true);
		}
		return $response;
	}
	/**
	* PUT wrapper for oAuthReqeust.
	**/
	public function put($url, $parameters = array())
	{
		$response = $this->oAuthRequest($url, 'PUT', $parameters);
		if ($this->format === 'json' && $this->decode_json)
		{
			return json_decode($response, true);
		}
		return $response;
	}


	/**
	 * Format and sign an OAuth / API request
	**/
	public function oAuthRequest ($url, $method, $parameters)
	{
		if (strrpos($url, 'https://') !== 0 && strrpos($url, 'http://') !== 0)
		{
			$url = "{$this->host}{$url}.{$this->format}";
		}

		$request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, $method, $url, $parameters);
		$request->sign_request($this->sha1_method, $this->consumer, $this->token);

		switch ($method)
		{
			case 'GET':
				return $this->http($request->to_url(), 'GET');
			default:
				return $this->http($request->get_normalized_http_url(), $method, $request->to_postdata());
		}
	}

	/**
	 * Make an HTTP request
	 * @return API results
	**/
	function http ($url, $method, $postfields = NULL)
	{
		$this->http_info = array();
		$ci = curl_init();
		/* Curl settings */
		curl_setopt($ci, CURLOPT_USERAGENT, $this->useragent);
		curl_setopt($ci, CURLOPT_CONNECTTIMEOUT, $this->connecttimeout);
		curl_setopt($ci, CURLOPT_TIMEOUT, $this->timeout);
		curl_setopt($ci, CURLOPT_RETURNTRANSFER, TRUE);
		curl_setopt($ci, CURLOPT_HTTPHEADER, array('Expect:'));
		curl_setopt($ci, CURLOPT_SSL_VERIFYPEER, $this->ssl_verifypeer);
		curl_setopt($ci, CURLOPT_HEADERFUNCTION, array($this , 'getHeader'));
		curl_setopt($ci, CURLOPT_HEADER, FALSE);
		switch ($method)
		{
			case 'POST':
				curl_setopt($ci, CURLOPT_POST, TRUE);
				if (! empty($postfields))
				{
					curl_setopt($ci, CURLOPT_POSTFIELDS, $postfields);
				}
				break;
			case 'DELETE':
				curl_setopt($ci, CURLOPT_CUSTOMREQUEST, 'DELETE');
				if (! empty($postfields))
				{
					$url = "{$url}?{$postfields}";
				}
				break; 
			case 'PUT':
				curl_setopt($ci, CURLOPT_CUSTOMREQUEST, 'PUT');
				if (! empty($postfields))
				{
					$url = "{$url}?{$postfields}";
				}
			}
		
		curl_setopt($ci, CURLOPT_URL, $url);
		$response = curl_exec($ci);
		$this->http_code = curl_getinfo($ci, CURLINFO_HTTP_CODE);
		switch($this->http_code)
		{
			case BlippyAPIErrorCodes::API_SUCCESS:
				break;
			case BlippyAPIErrorCodes::API_UNAUTHORIZED_REQUEST: 
				$description = BlippyAPIErrorCodes::$descriptions[$this->http_code];
				throw new BlippyException($description, $this->http_code);
				break;
			case BlippyAPIErrorCodes::API_EXCEEDED_RATE_LIMIT:
				$description = BlippyAPIErrorCodes::$descriptions[$this->http_code];
				throw new BlippyException($description, $this->http_code);
				break;
			case BlippyAPIErrorCodes::API_NOT_IMPLEMENTED:
				$description = BlippyAPIErrorCodes::$descriptions[$this->http_code];
				throw new BlippyException($description, $this->http_code);
				break; 
			case BlippyAPIErrorCodes::API_SERVER_ERROR:
				$description = BlippyAPIErrorCodes::$descriptions[$this->http_code];
				throw new BlippyException($description, $this->http_code);
				break;
			
		}
		$this->http_info = array_merge($this->http_info, curl_getinfo($ci));
		$this->url = $url;
		curl_close($ci);
		return $response;
	}

	/**
	 * Get the header info to store.
	**/
	function getHeader ($ch, $header)
	{
		$i = strpos($header, ':');
		if (! empty($i))
		{
			$key = str_replace('-', '_', strtolower(substr($header, 0, $i)));
			$value = trim(substr($header, $i + 2));
			$this->http_header[$key] = $value;
		}
		return strlen($header);
	}
}

class BlippyException extends Exception 
{
	
}

class BlippyAPIErrorCodes
{
	const API_SUCCESS								= 200; 
	const API_UNAUTHORIZED_REQUEST 	= 401;
	const API_EXCEEDED_RATE_LIMIT 	= 400;
	const API_NOT_IMPLEMENTED 			= 501;
	const API_SERVER_ERROR 					= 500;
	
	public static $descriptions = array(self::API_UNAUTHORIZED_REQUEST => "This method requires authorization: either you didn't supply any credentials, or they were wrong",
																			self::API_EXCEEDED_RATE_LIMIT => "Missing required parameters, or rate limit exceeded", 
																			self::API_NOT_IMPLEMENTED => "The Blippy API doesn't support this method", 
																			self::API_SERVER_ERROR => "Something broke with our code. Please tell us about it"); 
}


class BlippyUser {

  public $id;

  public $username;

  public $name;

  public $location;

  public $about;

  public $followers_count;

  public $following_count;

  public $reviews_count;

  public $purchases_count;

  public $profile_url;

  public $protected;

  public $profile_image_url;

  public $profile_background_color;

  public $verified;

  public $created_at;

  public $created_time;

  public $utc_offset;

  protected $password;

  protected $oauth_token;

  protected $oauth_token_secret;

  public function __construct($values = array()) {
    $this->id = $values['id'];
    $this->username = $values['username'];
    $this->name = $values['name'];
    $this->location = $values['location'];
    $this->about = $values['about'];
    $this->profile_url = $values['profile_url'];
    $this->profile_image_url = $values['profile_image_url'];
    $this->followers_count = $values['followers_count'];
    $this->following_count = $values['following_count'];
    $this->statuses_count = $values['statuses_count'];
    $this->purchases_count = $values['purchases_count'];
    $this->protected = $values['protected'];
    $this->reviews_count = $values['reviews_count'];
    $this->verified = $values['verified'];
    $this->created_at = $values['created_at'];
    if ($values['created_at'] && $created_time = date('d-M-Y', $values['created_at'])) {
      $this->created_time = $created_time;
    }
  }

  public function get_auth() {
    return array('oauth_token' => $this->oauth_token, 'oauth_token_secret' => $this->oauth_token_secret);
  }

  public function set_auth($values) {
    $this->oauth_token = $values['oauth_token'];
    $this->oauth_token_secret = $values['oauth_token_secret'];
  }
}
