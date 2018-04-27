
<img src="https://user-images.githubusercontent.com/38641542/39344583-da87c94e-4a15-11e8-9ad4-b2b52165cfe0.jpg">

MOLPay Drupal Ubercart Plugin

=========================================

MOLPay Plugin for Drupal Shopping Cart developed by MOLPay R&D team.

Supported Version
=========================================

Drupal 7.x

Ubercart 3.x


Notes
-----

MOLPay Sdn. Bhd. is not responsible for any problems that might arise from the use of this module. 
Use at your own risk. Please backup any critical data before proceeding. For any query or 
assistance, please email support@molpay.com

Installations for Drupal Ubercart 7.x.3.x and above
-------------------------------------------------------------

### Commerce Settings:

1. Download or clone this repository. 
2. Copy `sites` folder and paste it at your Drupal root directory.  
`<Drupal_DIR>/sites/*` 
3. Sign in to your Drupal admin
4. Under **Admin** click **Module**.
5. Under **Ubercart - payment** "tick" MOLPay Plugin.
6. Click "Save configuration".
7. Under **Show Ubercart - core (optional)** -> **Payment** click **Configure**.
8. Then "tick" at MOLPay Plugin and "setting".
10. Fill up "Payment settings"
11. Under **Merchant ID** enter your MOLPay MerchantID.
12. Under Verify Key enter your **MOLPay Merchant Verify Key** .
13. Select your checkout page language.
14. Click **Save Configuration**.
15. Log into your merchant account at https://portal.molpay.com . Click Transaction > Transaction Settings > Endpoint setting.
16. Fill in Callback URL with your shopping cart URL.
  
  ``Return URL : http://xxxxxxxxxxxxxx/cart/molpay/complete``

  ``Callback URL : http://xxxxxxxxxxxxxx/cart/molpay/callback``

  ``Notificarion URL : http://xxxxxxxxxxxxxx/cart/molpay/notification``
  
*Replace xxxxxxxxxxxxxx with your shoppingcart domain

	
Contribution
------------

You can contribute to this plugin by sending the pull request to this repository.


Issues
------------

Submit issue to this repository or email to our support@molpay.com


Support
-------
Merchant Technical Support / Customer Care : support@molpay.com <br>
Sales/Reseller Enquiry : sales@molpay.com <br>
Marketing Campaign : marketing@molpay.com <br>
Channel/Partner Enquiry : channel@molpay.com <br>
Media Contact : media@molpay.com <br>
R&D and Tech-related Suggestion : technical@molpay.com <br>
Abuse Reporting : abuse@molpay.com

