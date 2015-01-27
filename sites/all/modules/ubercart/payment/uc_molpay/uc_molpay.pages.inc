<?php

/**
 * @file
 * MOLPay menu items.
 */

function uc_molpay_complete($cart_id = 0) {
		
	 $errmesg = t('We are sorry. An error occurred while processing your order that prevents us from completing it at this time. Please contact us and we will resolve the issue as soon as possible.');
	 
	 $failedmsg = t('We are sorry. Your order is failed just now. Please try again to make another order.');
	 
	 $pendmsg = t('Your order is Pending just now. Please complete your cash payment.');
	

   $_POST['treq']= 1;
  // Assign posted variables to local variables
  $orderid  = check_plain($_POST['orderid']);
  $appcode  = check_plain($_POST['appcode']);
  $tran_id  = check_plain($_POST['tranID']);
  $domain   = check_plain($_POST['domain']);
  $status   = check_plain($_POST['status']);
  $amount   = check_plain($_POST['amount']);
  $currency = check_plain($_POST['currency']);
  $paydate  = check_plain($_POST['paydate']);
  $channel  = check_plain($_POST['channel']);
  $skey     = check_plain($_POST['skey']);
  $vkey     = variable_get('uc_molpay_secret_word', '');
  
	while ( list($k,$v) = each($_POST) ) {
	$postData[]= $k."=".$v;
	}
	$postdata = implode("&",$postData);
	$url = "https://www.onlinepayment.com.my/MOLPay/API/chkstat/returnipn.php";
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_POST , 1 );
	curl_setopt($ch, CURLOPT_POSTFIELDS , $postdata );
	curl_setopt($ch, CURLOPT_URL , $url );
	curl_setopt($ch, CURLOPT_HEADER , 1 );
	curl_setopt($ch, CURLINFO_HEADER_OUT , TRUE );
	curl_setopt($ch, CURLOPT_RETURNTRANSFER , 1 );
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER , FALSE);
	curl_setopt($ch, CURLOPT_SSLVERSION , CURL_SSLVERSION_TLSv1 );
	$result = curl_exec( $ch );
	curl_close( $ch );

  $key0 = md5($tran_id . $orderid . $status . $domain . $amount . $currency);
  $key1 = md5($paydate . $domain . $key0 . $appcode . $vkey);

  $details = array(
    'orderid'   => $orderid,
    'appcode'   => $appcode,
    'tranID'    => $tran_id,
    'domain'    => $domain,
    'status'    => $status,
    'amount'    => $amount,
    'currency'  => $currency,
    'paydate'   => $paydate,
    'channel'   => $channel,
    'skey'      => $skey,
  );

  // Invalid transaction
  if ($skey != $key1) {
    watchdog('MOLPay', 'Invalid MOLPay transaction attempted for order !order_id.', array('!order_id' => $orderid), WATCHDOG_ERROR);
    drupal_set_message($errmesg, 'error');
    drupal_goto('cart');
  }
	
	$order = uc_order_load($orderid);
  // Successful transaction
  if ($status == '00') {
    $order = uc_order_load($orderid);

    // Invalid order id
    if ($order === FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
      watchdog('MOLPay', 'Invalid MOLPay transaction attempted for non-existent order !order_id.', array('!order_id' => $orderid), WATCHDOG_ERROR);
      drupal_set_message($errmesg, 'error');
      drupal_goto('cart');
    }
		drupal_set_message(t('MOLPay transaction verified for order'));
		uc_order_comment_save($order->order_id, 0, t('Paid by !channel via MOLPay on !date (Transaction ID #!tran_id, Bank Approval #!appcode).',
		array('!channel' => $channel, '!date' => $paydate, '!tran_id' => $tran_id, '!appcode' => $appcode)));
		
		uc_payment_enter($order->order_id, 'MOLPay', $amount, 0 );	
	  
	  // Empty that cart...
		uc_cart_empty($cart_id);

      $output = uc_cart_complete_sale($order);
      uc_order_comment_save($order->order_id, 0, t('Payment of @amount submitted through @channel via MOLPay.', array('@channel' => $channel)), 'order', 'payment_received');

      
	}elseif($status==22){
		watchdog('MOLPay', 'Your transaction is pending.', array(), WATCHDOG_ERROR);
		
	 uc_cart_empty($cart_id);
	 
	 drupal_set_message($pendmsg, 'warning');
	 $output = uc_cart_complete_sale($order);
	 
      uc_order_comment_save($order->order_id, 0, t('MOLPay transaction pending for this order. Waiting customer to complete the payment.'), 'admin');
		
	}else {
	  watchdog('MOLPay', 'Your transaction is failed.', array(), WATCHDOG_ERROR);
	  
	 uc_cart_empty($cart_id);
	 
	 drupal_set_message($failedmsg, 'error');
	 $output = uc_cart_complete_sale($order);
	 
      uc_order_comment_save($order->order_id, 0, t('MOLPay transaction failed for this order.'), 'admin');
  }
  $page = variable_get('uc_cart_checkout_complete_page', '');
  
  if (!empty($page)) {
	drupal_goto($page);
  }
  
  return $output;
}

function uc_molpay_callback() {
	
	
	$to = "azwan@molpay.com";
	$subject = "My Callback function";
	$txt = "IT WORK!";
	$headers = "From: admin@molpay.com" . "\r\n" .
	"CC: azwan@molpay.com";
	
	mail($to,$subject,$txt,$headers);
	
	$errmesg = t('We are sorry. An error occurred while processing your order that prevents us from completing it at this time. Please contact us and we will resolve the issue as soon as possible.');
	 
	 $failedmsg = t('We are sorry. Your order is failed just now. Please try again to make another order.');
	 
	 $pendmsg = t('Your order is Pending just now. Please complete your cash payment.');
	

   $_POST['treq']= 1;
  // Assign posted variables to local variables
  $orderid  = check_plain($_POST['orderid']);
  $appcode  = check_plain($_POST['appcode']);
  $tran_id  = check_plain($_POST['tranID']);
  $domain   = check_plain($_POST['domain']);
  $status   = check_plain($_POST['status']);
  $amount   = check_plain($_POST['amount']);
  $currency = check_plain($_POST['currency']);
  $paydate  = check_plain($_POST['paydate']);
  $channel  = check_plain($_POST['channel']);
  $skey     = check_plain($_POST['skey']);
  $vkey     = variable_get('uc_molpay_secret_word', '');
  
	while ( list($k,$v) = each($_POST) ) {
	$postData[]= $k."=".$v;
	}
	$postdata = implode("&",$postData);
	$url = "https://www.onlinepayment.com.my/MOLPay/API/chkstat/returnipn.php";
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_POST , 1 );
	curl_setopt($ch, CURLOPT_POSTFIELDS , $postdata );
	curl_setopt($ch, CURLOPT_URL , $url );
	curl_setopt($ch, CURLOPT_HEADER , 1 );
	curl_setopt($ch, CURLINFO_HEADER_OUT , TRUE );
	curl_setopt($ch, CURLOPT_RETURNTRANSFER , 1 );
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER , FALSE);
	curl_setopt($ch, CURLOPT_SSLVERSION , CURL_SSLVERSION_TLSv1 );
	$result = curl_exec( $ch );
	curl_close( $ch );

  $key0 = md5($tran_id . $orderid . $status . $domain . $amount . $currency);
  $key1 = md5($paydate . $domain . $key0 . $appcode . $vkey);

  $details = array(
    'orderid'   => $orderid,
    'appcode'   => $appcode,
    'tranID'    => $tran_id,
    'domain'    => $domain,
    'status'    => $status,
    'amount'    => $amount,
    'currency'  => $currency,
    'paydate'   => $paydate,
    'channel'   => $channel,
    'skey'      => $skey,
  );

  // Invalid transaction
  if ($skey != $key1) {
    watchdog('MOLPay', 'Invalid MOLPay transaction attempted for order !order_id.', array('!order_id' => $orderid), WATCHDOG_ERROR);
    drupal_set_message($errmesg, 'error');
    drupal_goto('cart');
  }
	
	$order = uc_order_load($orderid);
  // Successful transaction
  if ($status == '00') {
    $order = uc_order_load($orderid);

    // Invalid order id
    if ($order === FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
      watchdog('MOLPay', 'Invalid MOLPay transaction attempted for non-existent order !order_id.', array('!order_id' => $orderid), WATCHDOG_ERROR);
      drupal_set_message($errmesg, 'error');
      drupal_goto('cart');
    }
		drupal_set_message(t('MOLPay transaction verified for order'));
		uc_order_comment_save($order->order_id, 0, t('Paid by !channel via MOLPay on !date (Transaction ID #!tran_id, Bank Approval #!appcode).',
		array('!channel' => $channel, '!date' => $paydate, '!tran_id' => $tran_id, '!appcode' => $appcode)));
		
		uc_payment_enter($order->order_id, 'MOLPay', $amount, 0 );	
	  
	  // Empty that cart...
		uc_cart_empty($cart_id);

      $output = uc_cart_complete_sale($order);
      uc_order_comment_save($order->order_id, 0, t('Payment of @amount submitted through @channel via MOLPay.', array('@channel' => $channel)), 'order', 'payment_received');

      
	}elseif($status==22){
		watchdog('MOLPay', 'Your transaction is pending.', array(), WATCHDOG_ERROR);
		
	 uc_cart_empty($cart_id);
	 
	 drupal_set_message($pendmsg, 'warning');
	 $output = uc_cart_complete_sale($order);
	 
      uc_order_comment_save($order->order_id, 0, t('MOLPay transaction pending for this order. Waiting customer to complete the payment.'), 'admin');
		
	}else {
	  watchdog('MOLPay', 'Your transaction is failed.', array(), WATCHDOG_ERROR);
	  
	 uc_cart_empty($cart_id);
	 
	 drupal_set_message($failedmsg, 'error');
	 $output = uc_cart_complete_sale($order);
	 
      uc_order_comment_save($order->order_id, 0, t('MOLPay transaction failed for this order.'), 'admin');
  }
  
	if ( $nbcb==1 ) {
	//callback IPN feedback to notified MOLPay
	echo "CBTOKEN:MPSTATOK"; exit;
	}
	
  $page = variable_get('uc_cart_checkout_complete_page', '');
  
  if (!empty($page)) {
	drupal_goto($page);
  }
  
  return $output;
}

function uc_molpay_notification() {
	$errmesg = t('We are sorry. An error occurred while processing your order that prevents us from completing it at this time. Please contact us and we will resolve the issue as soon as possible.');
	 
	 $failedmsg = t('We are sorry. Your order is failed just now. Please try again to make another order.');
	 
	 $pendmsg = t('Your order is Pending just now. Please complete your cash payment.');
	

   $_POST['treq']= 1;
  // Assign posted variables to local variables
  $orderid  = check_plain($_POST['orderid']);
  $appcode  = check_plain($_POST['appcode']);
  $tran_id  = check_plain($_POST['tranID']);
  $domain   = check_plain($_POST['domain']);
  $status   = check_plain($_POST['status']);
  $amount   = check_plain($_POST['amount']);
  $currency = check_plain($_POST['currency']);
  $paydate  = check_plain($_POST['paydate']);
  $channel  = check_plain($_POST['channel']);
  $skey     = check_plain($_POST['skey']);
  $vkey     = variable_get('uc_molpay_secret_word', '');
  
	while ( list($k,$v) = each($_POST) ) {
	$postData[]= $k."=".$v;
	}
	$postdata = implode("&",$postData);
	$url = "https://www.onlinepayment.com.my/MOLPay/API/chkstat/returnipn.php";
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_POST , 1 );
	curl_setopt($ch, CURLOPT_POSTFIELDS , $postdata );
	curl_setopt($ch, CURLOPT_URL , $url );
	curl_setopt($ch, CURLOPT_HEADER , 1 );
	curl_setopt($ch, CURLINFO_HEADER_OUT , TRUE );
	curl_setopt($ch, CURLOPT_RETURNTRANSFER , 1 );
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER , FALSE);
	curl_setopt($ch, CURLOPT_SSLVERSION , CURL_SSLVERSION_TLSv1 );
	$result = curl_exec( $ch );
	curl_close( $ch );

  $key0 = md5($tran_id . $orderid . $status . $domain . $amount . $currency);
  $key1 = md5($paydate . $domain . $key0 . $appcode . $vkey);

  $details = array(
    'orderid'   => $orderid,
    'appcode'   => $appcode,
    'tranID'    => $tran_id,
    'domain'    => $domain,
    'status'    => $status,
    'amount'    => $amount,
    'currency'  => $currency,
    'paydate'   => $paydate,
    'channel'   => $channel,
    'skey'      => $skey,
  );

  // Invalid transaction
  if ($skey != $key1) {
    watchdog('MOLPay', 'Invalid MOLPay transaction attempted for order !order_id.', array('!order_id' => $orderid), WATCHDOG_ERROR);
    drupal_set_message($errmesg, 'error');
    drupal_goto('cart');
  }
	
	$order = uc_order_load($orderid);
  // Successful transaction
  if ($status == '00') {
    $order = uc_order_load($orderid);

    // Invalid order id
    if ($order === FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
      watchdog('MOLPay', 'Invalid MOLPay transaction attempted for non-existent order !order_id.', array('!order_id' => $orderid), WATCHDOG_ERROR);
      drupal_set_message($errmesg, 'error');
      drupal_goto('cart');
    }
		drupal_set_message(t('MOLPay transaction verified for order'));
		uc_order_comment_save($order->order_id, 0, t('Paid by !channel via MOLPay on !date (Transaction ID #!tran_id, Bank Approval #!appcode).',
		array('!channel' => $channel, '!date' => $paydate, '!tran_id' => $tran_id, '!appcode' => $appcode)));
		
		uc_payment_enter($order->order_id, 'MOLPay', $amount, 0 );	
	  
	  // Empty that cart...
		uc_cart_empty($cart_id);

      $output = uc_cart_complete_sale($order);
      uc_order_comment_save($order->order_id, 0, t('Payment of @amount submitted through @channel via MOLPay.', array('@channel' => $channel)), 'order', 'payment_received');

      
	}elseif($status==22){
		watchdog('MOLPay', 'Your transaction is pending.', array(), WATCHDOG_ERROR);
		
	 uc_cart_empty($cart_id);
	 
	 drupal_set_message($pendmsg, 'warning');
	 $output = uc_cart_complete_sale($order);
	 
      uc_order_comment_save($order->order_id, 0, t('MOLPay transaction pending for this order. Waiting customer to complete the payment.'), 'admin');
		
	}else {
	  watchdog('MOLPay', 'Your transaction is failed.', array(), WATCHDOG_ERROR);
	  
	 uc_cart_empty($cart_id);
	 
	 drupal_set_message($failedmsg, 'error');
	 $output = uc_cart_complete_sale($order);
	 
      uc_order_comment_save($order->order_id, 0, t('MOLPay transaction failed for this order.'), 'admin');
  }
  
  if ( $nbcb==2 ) {
	//callback IPN feedback to notified MOLPay
	echo "CBTOKEN:MPSTATOK"; exit;
	}
	
  $page = variable_get('uc_cart_checkout_complete_page', '');
  
  if (!empty($page)) {
	drupal_goto($page);
  }
  
  return $output;
}