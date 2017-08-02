<?php
/**
 * Shop System Plugins - Terms of Use
 *
 * The plugins offered are provided free of charge by Wirecard Central Eastern
 * Europe GmbH
 * (abbreviated to Wirecard CEE) and are explicitly not part of the Wirecard
 * CEE range of products and services.
 *
 * They have been tested and approved for full functionality in the standard
 * configuration
 * (status on delivery) of the corresponding shop system. They are under
 * General Public License Version 2 (GPLv2) and can be used, developed and
 * passed on to third parties under the same terms.
 *
 * However, Wirecard CEE does not provide any guarantee or accept any liability
 * for any errors occurring when used in an enhanced, customized shop system
 * configuration.
 *
 * Operation in an enhanced, customized configuration is at your own risk and
 * requires a comprehensive test phase by the user of the plugin.
 *
 * Customers use the plugins at their own risk. Wirecard CEE does not guarantee
 * their full functionality neither does Wirecard CEE assume liability for any
 * disadvantages related to the use of the plugins. Additionally, Wirecard CEE
 * does not guarantee the full functionality for customized shop systems or
 * installed plugins of other vendors of plugins within the same shop system.
 *
 * Customers are responsible for testing the plugin's functionality before
 * starting productive operation.
 *
 * By installing the plugin into the shop system the customer agrees to these
 * terms of use. Please do not use the plugin if you do not agree to these
 * terms of use!
 */
$a = '<div class="wcp_pm_additional">';
$a .= '<div id="wcp_invoice_birthday">' . t('Birthday') . ':</div>';
$a .= '<div id="wcp_invoice_too_young">' . t('You have to be 18 years or older to use this payment.') . '</div>';

$consent_message = t('I agree that the data which are necessary for the liquidation of installments and which are used to complete the identity and credit check are transmitted to payolution. My __consent__ can be revoked at any time with future effect.');

$consent_message = preg_replace_callback('/__(.*?)__/i', function ($a) {
    $mid = variable_get('uc_wirecard_checkout_page_payolution_mid');
    return (strlen($mid) > 2) ? "<a style='color:white;mix-blend-mode:difference;' href='https://payment.payolution.com/payolution-payment/infoport/dataprivacyconsent?mId=" . base64_encode($mid) . "' target='_blank'>$a[1]</a>" : $a[1];
}, $consent_message);

if (variable_get('uc_wirecard_checkout_page_invoice_provider') == 'payolution' && variable_get('uc_wirecard_checkout_page_payolution_terms') == true) {
    $a .= '<div id="wcp_invoice_payolution_terms">' . $consent_message . '</div>';
    $a .= '<div id="wcp_invoice_payolution_terms_not_checked">' . t('Consumer must accept payolution terms during the checkout process.') . '</div>';
}

$a .= '</div>';

return $a;