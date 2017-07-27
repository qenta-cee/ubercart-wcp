<?php
/**
 * Shop System Plugins - Terms of Use
 *
 * The plugins offered are provided free of charge by Wirecard Central Eastern Europe GmbH
 * (abbreviated to Wirecard CEE) and are explicitly not part of the Wirecard CEE range of
 * products and services.
 *
 * They have been tested and approved for full functionality in the standard configuration
 * (status on delivery) of the corresponding shop system. They are under General Public
 * License Version 2 (GPLv2) and can be used, developed and passed on to third parties under
 * the same terms.
 *
 * However, Wirecard CEE does not provide any guarantee or accept any liability for any errors
 * occurring when used in an enhanced, customized shop system configuration.
 *
 * Operation in an enhanced, customized configuration is at your own risk and requires a
 * comprehensive test phase by the user of the plugin.
 *
 * Customers use the plugins at their own risk. Wirecard CEE does not guarantee their full
 * functionality neither does Wirecard CEE assume liability for any disadvantages related to
 * the use of the plugins. Additionally, Wirecard CEE does not guarantee the full functionality
 * for customized shop systems or installed plugins of other vendors of plugins within the same
 * shop system.
 *
 * Customers are responsible for testing the plugin's functionality before starting productive
 * operation.
 *
 * By installing the plugin into the shop system the customer agrees to these terms of use.
 * Please do not use the plugin if you do not agree to these terms of use!
 */

/**
 * Test class for WirecardCEE_Client_QMore_Return_Cancel.
 * Generated by PHPUnit on 2011-06-24 at 13:25:58.
 */
class WirecardCEE_QMore_Return_SuccessTest extends PHPUnit_Framework_TestCase
{

    /**
     * @var WirecardCEE_QMore_Return_Success
     */
    protected $object;

    /**
     *
     * @var array
     */
    protected $_returnData = Array(
        'amount'                   => '1',
        'currency'                 => 'EUR',
        'paymentType'              => 'QUICK',
        'financialInstitution'     => 'QUICK',
        'language'                 => 'en',
        'orderNumber'              => '16280512',
        'paymentState'             => 'SUCCESS',
        'gatewayReferenceNumber'   => 'DGW_16280512_RN',
        'gatewayContractNumber'    => 'DemoContractNumber123',
        'avsResponseCode'          => 'X',
        'avsResponseMessage'       => 'Demo AVS ResultMessage',
        'responseFingerprintOrder' => 'amount,currency,paymentType,financialInstitution,language,orderNumber,paymentState,gatewayReferenceNumber,gatewayContractNumber,avsResponseCode,avsResponseMessage,secret,responseFingerprintOrder',
        'responseFingerprint'      => 'aef780d2d0af569e3ed0da3195827981e607c693fb9f4294e6b03dd32938a477615c0edbfe43b3cea2e27f5d81434fcf9c54571fda13ee16e70d3a2864b10e43'
    );

    protected $_secret = 'B8AKTPWBRMNBV455FG6M2DANE99WU2';

    /**
     * Sets up the fixture, for example, opens a network connection.
     * This method is called before a test is executed.
     */
    protected function setUp()
    {
        ini_set('magic_quotes_gpc', 0);
        WirecardCEE_Stdlib_Fingerprint::setHashAlgorithm(WirecardCEE_Stdlib_Fingerprint::HASH_ALGORITHM_MD5);
        $this->object = new WirecardCEE_QMore_Return_Success($this->_returnData, $this->_secret);
    }

    /**
     * Tears down the fixture, for example, closes a network connection.
     * This method is called after a test is executed.
     */
    protected function tearDown()
    {

    }

    public function testGetAmount()
    {
        $this->assertEquals('1', $this->object->getAmount());
    }

    public function testGetCurrency()
    {
        $this->assertEquals('EUR', $this->object->getCurrency());
    }

    public function testGetPaymentType()
    {
        $this->assertEquals('QUICK', $this->object->getPaymentType());
    }

    public function testGetFinancialInstitution()
    {
        $this->assertEquals('QUICK', $this->object->getFinancialInstitution());
    }

    public function testGetLanguage()
    {
        $this->assertEquals('en', $this->object->getLanguage());
    }

    public function testGetOrderNumber()
    {
        $this->assertEquals('16280512', $this->object->getOrderNumber());
    }

    public function testGetGatewayReferenceNumber()
    {
        $this->assertEquals('DGW_16280512_RN', $this->object->getGatewayReferenceNumber());
    }

    public function testGetGatewayContractNumber()
    {
        $this->assertEquals('DemoContractNumber123', $this->object->getGatewayContractNumber());
    }

    public function testGetAvsResponseCode()
    {
        $this->assertEquals('X', $this->object->getAvsResponseCode());
    }

    public function testGetAvsResponseMessage()
    {
        $this->assertEquals('Demo AVS ResultMessage', $this->object->getAVSResponseMessage());
    }

    public function testValidate()
    {
        $this->assertTrue($this->object->validate());
    }

    public function testValidateFalse()
    {
        $object = new WirecardCEE_QMore_Return_Success($this->_returnData, '');

        try {
            $object->validate();
        } catch (WirecardCEE_Stdlib_Exception_InvalidArgumentException $e) {
            $this->assertContains('Secret is empty', $e->getMessage());
        }
    }

    public function testValidateNoFingerprintOrder()
    {
        $returnData = $this->_returnData;
        unset( $returnData['responseFingerprintOrder'] );
        $object = new WirecardCEE_QMore_Return_Success($returnData, $this->_secret);
        try {
            $object->validate();
        } catch (WirecardCEE_Stdlib_Exception_InvalidArgumentException $e) {
            $this->assertContains('Parameter responseFingerprintOrder has not been returned', $e->getMessage());
        }
    }

    public function testGetPaymentState()
    {
        $this->assertEquals('SUCCESS', $this->object->getPaymentState());
    }

    public function testGetReturned()
    {
        $returned = $this->object->getReturned();
        $this->assertArrayHasKey('amount', $returned);
        $this->assertArrayHasKey('currency', $returned);
        $this->assertArrayHasKey('paymentType', $returned);
        $this->assertArrayHasKey('financialInstitution', $returned);
        $this->assertArrayHasKey('language', $returned);
        $this->assertArrayHasKey('orderNumber', $returned);
        $this->assertArrayHasKey('paymentState', $returned);
        $this->assertArrayHasKey('gatewayReferenceNumber', $returned);
        $this->assertArrayHasKey('gatewayContractNumber', $returned);
        $this->assertArrayHasKey('avsResponseCode', $returned);
        $this->assertArrayHasKey('avsResponseMessage', $returned);
        $this->assertArrayNotHasKey('responseFingerprintOrder', $returned);
        $this->assertArrayNotHasKey('responseFingerprint', $returned);
    }
}

?>
