# [转]NopCommerce How to code my own payment method - weixin_33985507的博客 - CSDN博客
2016年12月06日 10:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
本文转自：[http://docs.nopcommerce.com/display/nc/How+to+code+my+own+payment+method](http://docs.nopcommerce.com/display/nc/How+to+code+my+own+payment+method)
Payment methods are implemented as plugins in nopCommerce. We recommend you read [How to write a nopCommerce plugin](http://docs.nopcommerce.com/display/nc/How+to+write+a+nopCommerce+plugin) before you start coding a new payment method. It will explain to you what the required steps are for creating a plugin.
So actually a payment method is an ordinary plugin which implements an IPaymentMethod interface (Nop.Services.Payments namespace). As you already guessed IPaymentMethod interface is used for creating payment method plugins. It contains some methods which are specific only for payment methods such as ProcessPayment() or GetAdditionalHandlingFee(). So add a new payment plugin project (class library) to solution and let's get started.
### Controllers, views, models.
First thing you need to do is create a controller. This controller is responsible for responding to requests made against an ASP.NET MVC website.
- 
When implementing a new payment method, this controller should be derived from a special BaseNopPaymentController abstract class. Some of the payment methods ask customer for some data, such as credit card information. That's why a BasePaymentController class requires you to implement two action methods used to parse and validate customer input during checkout.
- **ValidatePaymentForm** is used in the public store to validate customer input. It returns a list of warnings (for example, a customer did not enter his credit card name). If your payment method does not ask the customer to enter additional information, then the ValidatePaymentForm should return an empty list.
        [NonAction]
        public override IList ValidatePaymentForm(FormCollection form)        {             var warnings = new List();             return warnings;        } 
- **GetPaymentInfo** method is used in the public store to parse customer input, such as credit card information. This method returns a ProcessPaymentRequest object with parsed customer input (for example, credit card information). If your payment method does not ask the customer to enter additional information, then GetPaymentInfo will return an empty ProcessPaymentRequest object.
        [NonAction]
        public override ProcessPaymentRequest GetPaymentInfo(FormCollection form)
        {
            var paymentInfo = new ProcessPaymentRequest();            return paymentInfo;
        }
- Then implement **Configure** action methods used for plugin configuration (by a store owner in admin area). This method and an appropriate view will define how a store owner sees configuration options in admin panel (System > Configuration > Payment methods).
- And the last step is to create **PaymentInfo** action method. This method and an appropriate view will define how your customers will see the payment information page during checkout.
### Payment processing.
Now you need to create a class which implements **IPaymentMethod** interface. This is the class that will be doing all the actual work of communicating with your payment gateway. When someone creates an order either the **ProcessPayment** or **PostProcessPayment** methods of your class will be called. Here is how CashOnDeliveryPaymentProcessor class is defined ("Cash on delivery" payment method):
    ///          /// CashOnDelivery payment processor         /// 
    public class CashOnDeliveryPaymentProcessor : BasePlugin, IPaymentMethod
    {...
**IPaymentMethod** interface has several methods and properties which are required to implement.
- **ProcessPayment**. This method is always invoked right before a customer places an order. Use it when you need to process a payment before an order is stored into database. For example, capture or authorize credit card. Usually this method is used when a customer is not redirected to third-party site for completing a payment and all payments are handled on your site (for example, PayPal Direct).
- **PostProcessPayment**. This method is invoked right after a customer places an order. Usually this method is used when you need to redirect a customer to a third-party site for completing a payment (for example, PayPal Standard).
- ****HidePaymentMethod. ****You can put any logic here. For example, hide this payment method if all products in the cart are downloadable. Or hide this payment method if current customer is from certain country
- **GetAdditionalHandlingFee**. You can return any additional handling fees which will be added to an order total.
- **Capture**. Some payment gateways allow you to authorize payments before they're captured. It allows store owners to review order details before the payment is actually done. In this case you just authorize a payment in **ProcessPayment** or **PostProcessPayment** method (described above), and then just capture it. In this case a **Capture** button will be visible on the order details page in admin area. Note that an order should be already authorized and **SupportCapture** property should return**true**.
- **Refund**. This method allows you make a refund. In this case a **Refund** button will be visible on the order details page in admin area. Note that an order should be paid, and **SupportRefund** or **SupportPartiallyRefund** property should return **true**.
- **Void**. This method allows you void an authorized but not captured payment. In this case a **Void** button will be visible on the order details page in admin area. Note that an order should be authorized and **SupportVoid** property should return **true**.
- **ProcessRecurringPayment**. Use this method to process recurring payments.
- **CancelRecurringPayment**. Use this method to cancel recurring payments.
- **CanRePostProcessPayment**. Usually this method is used when it redirects a customer to a third-party site for completing a payment. If the third party payment fails this option will allow customers to attempt the order again later without placing a new order. **CanRePostProcessPayment** should return **true** to enable this feature.
- **GetConfigurationRoute**. As you remember we created a controller in the previous step. This method should return a route information of its Configure method. For example,
        public void GetConfigurationRoute(out string actionName, 
            out string controllerName, 
            out RouteValueDictionary routeValues)
        {
            actionName = "Configure";
            controllerName = "PaymentCashOnDelivery";
            routeValues = new RouteValueDictionary()
            {
                { "Namespaces", "Nop.Plugin.Payments.CashOnDelivery.Controllers" }, 
                { "area", null }
            };
        }
- **GetPaymentInfoRoute**. This method should return a route information of appropriate PublicInfo method of the previously created controller. For example,
        public void GetPaymentInfoRoute(out string actionName, 
            out string controllerName, 
            out RouteValueDictionary routeValues)
        {
            actionName = "PaymentInfo";
            controllerName = "PaymentCashOnDelivery";
            routeValues = new RouteValueDictionary()
            {
                { "Namespaces", "Nop.Plugin.Payments.CashOnDelivery.Controllers" }, 
                { "area", null }
            };
        }
- **GetControllerType**. This method should return a type of the previously created controller.
- **SupportCapture, SupportPartiallyRefund, SupportRefund, SupportVoid**. These properties indicate whether appropriate methods of your payment method are supported.
- **RecurringPaymentType**. This property indicates whether recurring payments are supported.
- **PaymentMethodType**. This property indicates payment method type. Currently there are three types. **Standard** used by payment methods when a customer is not redirected to a third-party site. **Redirection** is used when a customer is redirected to a third-party site. And **Button** is similar to **Redirection** payment methods. The only difference is used that it's displayed as a button on shopping cart page (for example, Google Checkout).
### Conclusion
Hopefully this will get you started with adding a new payment method.
