<?php
/*
 * Shop System Plugins - Terms of use
 *
 * This terms of use regulates warranty and liability between Wirecard Central Eastern Europe (subsequently referred to as WDCEE) and it's
 * contractual partners (subsequently referred to as customer or customers) which are related to the use of plugins provided by WDCEE.
 *
 * The Plugin is provided by WDCEE free of charge for it's customers and must be used for the purpose of WDCEE's payment platform
 * integration only. It explicitly is not part of the general contract between WDCEE and it's customer. The plugin has successfully been tested
 * under specific circumstances which are defined as the shopsystem's standard configuration (vendor's delivery state). The Customer is
 * responsible for testing the plugin's functionality before putting it into production enviroment.
 * The customer uses the plugin at own risk. WDCEE does not guarantee it's full functionality neither does WDCEE assume liability for any
 * disadvantage related to the use of this plugin. By installing the plugin into the shopsystem the customer agrees to the terms of use.
 * Please do not use this plugin if you do not agree to the terms of use!
 */

/**
 * @file
 * Page callbacks for Wirecard Checkout Page
 * return/confirm pages
 */

/* translations:
  Download and install the Translation Template Extractor (POTX).
  Use POTX to extract from the installed module a .pot template, that is, a file that includes the untranslated strings in english.
  uc_wirecard_checkout_page$ php ../potx/potx-cli.php
  msginit --locale=de --input=general.pot
  de.po
*/

/**
 * payment confirmation, server 2 server request, no session data available
 *
 * @return string
 */
function uc_wirecard_checkout_page_confirm() {

  watchdog(
    'uc_wirecard_checkout_page',
    'confirm data: @data',
    array('@data' => print_r($_POST, TRUE)),
    WATCHDOG_DEBUG
  );

  $message = '';
  if (empty($_POST)) {
    watchdog(
      'uc_wirecard_checkout_page',
      'Received an empty or incomplete response',
      array(),
      WATCHDOG_ERROR
    );

    print WirecardCEE_QPay_ReturnFactory::generateConfirmResponseString('There was a problem with your payment');
    return;
  }


  if (!isset($_POST['ubercartOrderId']) || !strlen($_POST['ubercartOrderId'])) {
    print WirecardCEE_QPay_ReturnFactory::generateConfirmResponseString('order-id missing');
    return;
  }

  $order_id = (int) $_POST['ubercartOrderId'];
  $cart_id = (int) $_POST['ubercartCartId'];
  $order = uc_order_load($order_id);

  if ($order == FALSE) {
    watchdog(
      'uc_wirecard_checkout_page',
      'Order not found with Order Id: @order_id',
      array('@order_id' => $order_id),
      WATCHDOG_ERROR
    );
    print WirecardCEE_QPay_ReturnFactory::generateConfirmResponseString('Order not found using order_id:' . $order_id);
    return;
  }

  try {
    $return = WirecardCEE_QPay_ReturnFactory::getInstance($_POST, variable_get('uc_wirecard_checkout_page_secret'));
    if (!$return->validate()) {
      $message = t('Validation error: invalid response');
      uc_order_comment_save(
        $order_id,
        0,
        t('Payment failed reason:@reason'),
        array('@reason' => $message)
      );
      print WirecardCEE_QPay_ReturnFactory::generateConfirmResponseString($message);
      return;
    }

    /** @var $return WirecardCEE_Stdlib_Return_ReturnAbstract */
    //update_post_meta($order->id, 'WCP Payment State', $return->getPaymentState());

    switch ($return->getPaymentState()) {
      case WirecardCEE_QPay_ReturnFactory::STATE_SUCCESS:
        /** @var $return WirecardCEE_QPay_Return_Success */
        $comment = t(
          'WirecardCheckoutPage @method orderNumber: @orderNumber gatewayReferenceNumber: @gatewayReferenceNumber',
          array(
               '@orderNumber' => $return->getOrderNumber(),
               '@gatewayReferenceNumber' => $return->getGatewayReferenceNumber(),
               '@method' => $return->getPaymentType(),
          )
        );
        uc_payment_enter($order_id, $return->getPaymentType(), $return->getAmount(), $order->uid, NULL, $comment);
        uc_cart_complete_sale($order);
        uc_cart_empty($cart_id);
        uc_order_comment_save(
          $order_id,
          0,
          t(
            'WirecardCheckoutPage @method a payment of @amount @currency.',
            array(
                 '@method' => $return->getPaymentType(),
                 '@amount' => uc_currency_format($return->getAmount(), FALSE),
                 '@currency' => $return->getCurrency()
            )
          )
        );
        break;

      case WirecardCEE_QPay_ReturnFactory::STATE_PENDING:
        /** @var $return WirecardCEE_QPay_Return_Pending */
        uc_order_update_status($order_id, 'pending');
        uc_order_comment_save($order_id, 0, t('Payment is pending'));
        break;

      case WirecardCEE_QPay_ReturnFactory::STATE_CANCEL:
        /** @var $return WirecardCEE_QPay_Return_Cancel */
        uc_order_comment_save($order_id, 0, t('Payment cancelled'));
        break;

      case WirecardCEE_QPay_ReturnFactory::STATE_FAILURE:
        /** @var $return WirecardCEE_QPay_Return_Failure */
        uc_order_comment_save(
          $order_id,
          0,
          t('Payment failed reason:@reason'),
          array('@reason' => $return->getErrors()->getConsumerMessage())
        );
        break;

      default:
        break;
    }
  } catch (Exception $e) {
    watchdog('uc_wirecard_checkout_page', $e->getMessage(), array(), WATCHDOG_ERROR);
    $message = $e->getMessage();
    uc_order_comment_save(
      $order_id,
      0,
      t('Payment failed reason:@reason'),
      array('@reason' => $message)
    );
  }

  print WirecardCEE_QPay_ReturnFactory::generateConfirmResponseString($message);
  return;
}


/**
 * payment return to shop
 *
 * @return string
 */
function uc_wirecard_checkout_page_return() {

  watchdog(
    'uc_wirecard_checkout_page',
    'return data: @data',
    array('@data' => print_r($_POST, TRUE)),
    WATCHDOG_DEBUG
  );

  $message = '';
  if (empty($_POST)) {
    return t('Received an empty or incomplete response');
  }

  if (array_key_exists('xIframeUsed', $_POST) && !array_key_exists('redirected', $_POST)) {
    $str = sprintf('<h3>%s</h3>', t('You will be redirected shortly'));
    // workaround, drupal doesent allow html code inside translations
    $click = t('If not, please click @click1 here @click2');
    $click = strtr($click, array('@click1' => '<a href="#" onclick="iframeBreakout()">', '@click2' => '</a>'));
    $str .= sprintf('<h3>%s</h3>', $click);
    $str .= sprintf(
      '<form method="POST" name="redirectForm" action="%s" target="_parent">',
      url('cart/wirecard_checkout_page/return', array('absolute' => TRUE))
    );

    foreach ($_POST as $k => $v) {
      $str .= sprintf('<input type="hidden" name="%s" value="%s" />', htmlspecialchars($k), htmlspecialchars($v));
    }

    $str .= '<input type="hidden" name="redirected" value="1" />';

    $str .= "</form>
      <script type=\"text/javascript\">
      // <![CDATA[
      function iframeBreakout()
      {
        document.redirectForm.submit();
      }
      iframeBreakout();
      //]]>
      </script>";

    print $str;
    return;
  }

  if (!isset($_POST['ubercartOrderId']) || !strlen($_POST['ubercartOrderId'])) {
    return t('Order Id is missing');
  }

  $order_id = (int) $_POST['ubercartOrderId'];
  $order = uc_order_load($order_id);

  if ($order === FALSE) {
    return t('Order not found with Order Id:@order_id', array('@order_id' => $order_id));
  }

  try {
    $return = WirecardCEE_QPay_ReturnFactory::getInstance($_POST, variable_get('uc_wirecard_checkout_page_secret'));
    if (!$return->validate()) {
      $message = t('Validation error: invalid response');
      return $message;
    }

    $redirect = 'cart/checkout/review';

    /** @var $return WirecardCEE_Stdlib_Return_ReturnAbstract */

    switch ($return->getPaymentState()) {
      case WirecardCEE_QPay_ReturnFactory::STATE_SUCCESS:
        /** @var $return WirecardCEE_QPay_Return_Success */
        $redirect = 'cart/checkout/complete';
        break;

      case WirecardCEE_QPay_ReturnFactory::STATE_PENDING:
        /** @var $return WirecardCEE_QPay_Return_Pending */
        drupal_set_message(t('Your order will be shipped, as soon as we get the payment notification from your bank.'));
        $redirect = 'cart/checkout/complete';
        break;

      case WirecardCEE_QPay_ReturnFactory::STATE_CANCEL:
        /** @var $return WirecardCEE_QPay_Return_Cancel */
        drupal_set_message(t('Payment cancelled'), 'warning');
        break;

      case WirecardCEE_QPay_ReturnFactory::STATE_FAILURE:
        /** @var $return WirecardCEE_QPay_Return_Failure */
        drupal_set_message(t($return->getErrors()->getConsumerMessage()), 'error');
        break;

      default:
        break;
    }
  } catch (Exception $e) {
    watchdog('uc_wirecard_checkout_page', $e->getMessage(), array(), WATCHDOG_ERROR);
    drupal_set_message($e->getMessage(), 'error');
  }

  // order completion is allowed
  if ($redirect == 'cart/checkout/complete') {
      if (isset($_SESSION['cart_order'])) {
          $_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = true;
      }
  }

  drupal_goto($redirect);
}

/**
 * payment page opened inside an iframe
 *
 * @return string
 */
function uc_wirecard_checkout_page_iframe() {
  if (($order = uc_order_load($_SESSION['cart_order'])) == FALSE) {
    drupal_set_message(t('An error has occurred in your payment. Please review your cart and try again.'));
    drupal_goto('cart');
  }

  try {
    $client = uc_wirecard_checkout_page_initiate_payment($order);
    $client->xIframeUsed = TRUE;
    $response = $client->initiate();

    if ($response->hasFailed()) {
      watchdog(
        'Wirecard Checkout Page',
        "Response failed! Error: @message",
        array('@message' => $response->getError()->getMessage()),
        WATCHDOG_ERROR
      );
      return $response->getError()->getMessage();
    }

    return sprintf(
      '<iframe src="%s" name="%%s" width="100%%" height="700px" border="0" frameborder="0"><p>Your browser does not support iframes.</p></iframe>',
      $response->getRedirectUrl(),
      UC_WIRECARD_CHECKOUT_PAGE_WINDOWNAME
    );

  } catch (Exception $e) {
    return $e->getMessage();
  }


}


/**
 * store additional data during checkout
 */
function uc_wirecard_checkout_page_store_additional_data(){
    $_SESSION['wcp_additional'] = $_POST;
}