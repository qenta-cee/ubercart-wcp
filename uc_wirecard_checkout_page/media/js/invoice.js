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

$ = jQuery;
$(document).ready(function() {
    $("#wcp_invoice_birthday").wrap('<label>').append("<input type='text' id='wcp_invoice_datepicker' name='wcp_invoice_birthday'>");
    $("#wcp_invoice_datepicker").datepicker({
        changeYear : true,
        changeMonth : true,
        minDate: "-100y",
        maxDate: "-10y",
        onChangeMonthYear:function(y, m, i){
            var d = i.selectedDay;
            $(this).datepicker('setDate', new Date(y, m - 1, d));
        }
    }).datepicker('setDate', '-10y');
    $("#wcp_invoice_payolution_terms").wrap('<label>').prepend(' <input type="checkbox" name="wcp_invoice_payolution_terms">');

    $("#edit-continue").click(function(a){
        if($("#edit-panes-payment-payment-method-uc-wcp-invoice").is(":checked")){
            var dob = new Date($("#wcp_invoice_datepicker").datepicker('getDate')),
                underage = Math.abs(new Date(Date.now() - dob.getTime()).getUTCFullYear() - 1970) < 18,
                ptos = $("input[name=wcp_invoice_payolution_terms]");
            if(underage){
                $("#wcp_invoice_too_young").show();
                $('html, body').animate({
                    scrollTop: $("#wcp_invoice_too_young").offset().top-150
                }, 'fast');
                a.preventDefault();
                return false;
            }
            else
                $("#wcp_invoice_too_young").hide();

            if(ptos.length > 0 && !ptos.is(":checked")) {
                $("#wcp_invoice_payolution_terms_not_checked").show();
                $('html, body').animate({
                    scrollTop: $("#wcp_invoice_payolution_terms_not_checked").offset().top-150
                }, 'fast');
                a.preventDefault();
                return false;
            }
            else
                $("#wcp_invoice_payolution_terms_not_checked").hide();
        }

    });

});