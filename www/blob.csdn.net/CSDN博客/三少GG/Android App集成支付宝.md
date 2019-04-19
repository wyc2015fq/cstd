# Android App集成支付宝 - 三少GG - CSDN博客
2012年06月15日 21:27:25[三少GG](https://me.csdn.net/scut1135)阅读数：4454
## [Android学习系列(28)--App集成支付宝](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html)
手机的在线支付，被认为是2012年最看好的功能，我个人认为这也是移动互联网较传统互联网将会大放光彩的一个功能。
人人有手机，人人携带手机，花钱买东西，不再需要取钱付现，不再需要回家上网银，想买什么，扫描一下，或者搜索一下，然后下单，不找零，直接送到你家，这将是手机支付给我们带来的全新交易体验。
谷歌刚推出了谷歌钱包，这必是我们后面要使用的主要手段，但是鉴于当前国情，我觉得有必要介绍一下android手机集成支付宝功能。 
**1.下载官方架包和说明文档**
其实官方已经提供了安装指南,下载地址：
[https://mobiless.alipay.com/product/product_down_load.htm?code=SECURITY_PAY](https://mobiless.alipay.com/product/product_down_load.htm?code=SECURITY_PAY)
里面有有个pdf，详细说明了说用指南，写的比较详细，可以重点参考。
![](http://pic002.cnblogs.com/images/2012/31770/2012040421054311.png)
下载下来，我们主要是用到Android(20120104)目录下的alipay_plugin.jar和AppDemo/assets下的alipay_plugin223_0309.apk，这两个文件是我们不能修改的支付宝api和安装包。
**2. 商户签约**
现在的安全机制，都是这样，客户端需要先和服务端请求验证后才能进行进一步操作，oauth也是如此。
打开https://ms.alipay.com/，登陆支付宝，点击签约入口，选择"应用类产品"，填写并等待审核，获取商户ID和账户ID。
签约的时候还要向需要提供实名认证和上传应用，所以我建议先把应用做好了，最后再集成支付宝。
![](http://pic002.cnblogs.com/images/2012/31770/2012040421302878.png)
我大概等了1-2天审核，审核是失败的，回复是应用类型啥的应该是"虚拟货币"，我改成那个马上自动就审核通过了。
**3.密钥配置**
解压openssl-0.9.8k_WIN32(RSA密钥生成工具).zip，打开cmd，命令行进入openssl-0.9.8k_WIN32(RSA密钥生成工具)\bin目录下，
(1).执行
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```
`openssl genrsa  -out rsa_private_key.pem 1024`
```
生成rsa_private_key.pem文件。
(2).再执行
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```
`openssl rsa  -in rsa_private_key.pem  -pubout -out rsa_public_key.pem`
```
生成rsa_public_key.pem 文件。
(3).在执行
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```
`openssl pkcs8  -topk8  -inform PEM  -in rsa_private_key.pem  -outform PEM  -nocrypt`
```
将RSA私钥转换成 PKCS8 格式，去掉begin和end那两行，把里面的内容拷贝出来，保存到某个txt中，如rsa_private_pkcs8_key.txt中（我好像没用到这个）。
打开rsa_public_key.pem,即商户的公钥，复制到一个新的TXT中，删除文件头”-----BEGIN PUBLIC KEY-----“与文件尾”-----END PUBLIC KEY-----“还有空格、换行，变成一行字符串并保存该 TXT 文件，然后在网站的“我的商家服务”切换卡下的右边点击“密钥管理”，然后有个"上传商户公钥(RSA)"项，选择上传刚才的TXT文件.
好了，服务器配置OK，因为这一段之前没有截图，现在弄好了又不好截图，如果有不明白的地方请大家参考官方文档。 
**4.引用jar和包含安装包**
    (1).新建android工程；
    (2).copy上面说的alipay_plugin.jar到工程的libs目录下，并在java build path中通过Add External JARs找到并引用该jar；
    (3).copy上面说的alipay_plugin223_0309.apk安装包到assets目录下，后面配置路径用到。
![](http://pic002.cnblogs.com/images/2012/31770/2012040421142141.png)
如果libs和assets目录没有，手动建立者两个目录。
**5.调用代码整理**
这里我们要严重的参考文档中AppDemo，我们建一个包com.tianxia.lib.baseworld.alipay，把AppDemo的com.alipay.android.appDemo4包下的源码全部copy到刚才我们自己的包下，还有res目录下的资源文件也合并到我们工程res下。
其中AlixDemo.java,ProductListAdapter.java,Products.java是示例类，我们借鉴完后可以删除。
PartnerConfig.java是配置类，配置商户的一些配置参数。
其他的类是严重参考类，直接留下使用。
PartnerConfig.java代码如下:
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```
```java
public
```
```java
class
```
```java
PartnerConfig {
```
```java
```
```java
//合作商户ID。用签约支付宝账号登录ms.alipay.com后，在账户信息页面获取。
```
```java
```
```java
public
```
```java
static
```
```java
final
```
```java
String PARTNER =
```
```java
"xxx"
```
```java
;
```
```java
```
```java
//账户ID。用签约支付宝账号登录ms.alipay.com后，在账户信息页面获取。
```
```java
```
```java
public
```
```java
static
```
```java
final
```
```java
String SELLER =
```
```java
"xxx"
```
```java
;
```
```java
```
```java
//商户（RSA）私钥 ,即rsa_private_key.pem中去掉首行，最后一行，空格和换行最后拼成一行的字符串
```
```java
```
```java
public
```
```java
static
```
```java
final
```
```java
String RSA_PRIVATE =
```
```java
"xxx"
```
```java
;
```
```java
```
```java
//支付宝（RSA）公钥  用签约支付宝账号登录ms.alipay.com后，在密钥管理页面获取。
```
```java
```
```java
public
```
```java
static
```
```java
final
```
```java
String RSA_ALIPAY_PUBLIC =
```
```java
"xxx"
```
```java
;
```
```java
```
```java
//下面的配置告诉应用去assets目录下找安装包
```
```java
```
```java
public
```
```java
static
```
```java
final
```
```java
String ALIPAY_PLUGIN_NAME =
```
```java
"alipay_plugin223_0309.apk"
```
```java
;
```
```java
}
```
```
AlixDemo中代码是最终的调用代码在onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {}中，下面我们提取其中的核心代码。
**6.提取核心调用代码**
在AlixDemo.java同目录下新建AlixPay.java,来提取AlixDemo.java的核心代码：
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```
```java
package
```
```java
com.tianxia.lib.baseworld.alipay;
```
```java
import
```
```java
java.net.URLEncoder;
```
```java
import
```
```java
java.text.SimpleDateFormat;
```
```java
import
```
```java
java.util.Date;
```
```java
import
```
```java
com.tianxia.lib.baseworld.R;
```
```java
import
```
```java
android.app.Activity;
```
```java
import
```
```java
android.app.ProgressDialog;
```
```java
import
```
```java
android.content.DialogInterface;
```
```java
import
```
```java
android.os.Handler;
```
```java
import
```
```java
android.os.Message;
```
```java
import
```
```java
android.view.KeyEvent;
```
```java
import
```
```java
android.widget.Toast;
```
```java
public
```
```java
class
```
```java
AlixPay {
```
```java
```
```java
static
```
```java
String TAG =
```
```java
"AlixPay"
```
```java
;
```
```java
```
```java
private
```
```java
Activity mActivity;
```
```java
```
```java
public
```
```java
AlixPay(Activity activity) {
```
```java
```
```java
mActivity = activity;
```
```java
```
```java
}
```
```java
```
```java
private
```
```java
ProgressDialog mProgress =
```
```java
null
```
```java
;
```
```java
```
```java
// the handler use to receive the pay result.
```
```java
```
```java
private
```
```java
Handler mHandler =
```
```java
new
```
```java
Handler() {
```
```java
```
```java
public
```
```java
void
```
```java
handleMessage(Message msg) {
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
String strRet = (String) msg.obj;
```
```java
```
```java
switch
```
```java
(msg.what) {
```
```java
```
```java
case
```
```java
AlixId.RQF_PAY: {
```
```java
```
```java
closeProgress();
```
```java
```
```java
BaseHelper.log(TAG, strRet);
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
String memo =
```
```java
"memo="
```
```java
;
```
```java
```
```java
int
```
```java
imemoStart = strRet.indexOf(
```
```java
"memo="
```
```java
);
```
```java
```
```java
imemoStart += memo.length();
```
```java
```
```java
int
```
```java
imemoEnd = strRet.indexOf(
```
```java
";result="
```
```java
);
```
```java
```
```java
memo = strRet.substring(imemoStart, imemoEnd);
```
```java
```
```java
ResultChecker resultChecker =
```
```java
new
```
```java
ResultChecker(strRet);
```
```java
```
```java
int
```
```java
retVal = resultChecker.checkSign();
```
```java
```
```java
if
```
```java
(retVal == ResultChecker.RESULT_CHECK_SIGN_FAILED) {
```
```java
```
```java
BaseHelper.showDialog(
```
```java
```
```java
mActivity,
```
```java
```
```java
"提示"
```
```java
,
```
```java
```
```java
mActivity.getResources().getString(
```
```java
```
```java
R.string.check_sign_failed),
```
```java
```
```java
android.R.drawable.ic_dialog_alert);
```
```java
```
```java
}
```
```java
else
```
```java
{
```
```java
```
```java
BaseHelper.showDialog(mActivity,
```
```java
"提示"
```
```java
, memo,
```
```java
```
```java
R.drawable.infoicon);
```
```java
```
```java
}
```
```java
```
```java
```
```java
}
```
```java
catch
```
```java
(Exception e) {
```
```java
```
```java
e.printStackTrace();
```
```java
```
```java
BaseHelper.showDialog(mActivity,
```
```java
"提示"
```
```java
, strRet,
```
```java
```
```java
R.drawable.infoicon);
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
break
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
super
```
```java
.handleMessage(msg);
```
```java
```
```java
}
```
```java
catch
```
```java
(Exception e) {
```
```java
```
```java
e.printStackTrace();
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
};
```
```java
```
```java
// close the progress bar
```
```java
```
```java
void
```
```java
closeProgress() {
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
if
```
```java
(mProgress !=
```
```java
null
```
```java
) {
```
```java
```
```java
mProgress.dismiss();
```
```java
```
```java
mProgress =
```
```java
null
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
catch
```
```java
(Exception e) {
```
```java
```
```java
e.printStackTrace();
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
public
```
```java
void
```
```java
pay() {
```
```java
```
```java
MobileSecurePayHelper mspHelper =
```
```java
new
```
```java
MobileSecurePayHelper(mActivity);
```
```java
```
```java
boolean
```
```java
isMobile_spExist = mspHelper.detectMobile_sp();
```
```java
```
```java
if
```
```java
(!isMobile_spExist)
```
```java
```
```java
return
```
```java
;
```
```java
```
```java
if
```
```java
(!checkInfo()) {
```
```java
```
```java
BaseHelper.showDialog(mActivity,
```
```java
"提示"
```
```java
,
```
```java
```
```java
"缺少partner或者seller，"
```
```java
, R.drawable.infoicon);
```
```java
```
```java
return
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
// prepare the order info.
```
```java
```
```java
String orderInfo = getOrderInfo();
```
```java
```
```java
String signType = getSignType();
```
```java
```
```java
String strsign = sign(signType, orderInfo);
```
```java
```
```java
strsign = URLEncoder.encode(strsign);
```
```java
```
```java
String info = orderInfo +
```
```java
"&sign="
```
```java
+
```
```java
"\""
```
```java
+ strsign +
```
```java
"\""
```
```java
+
```
```java
"&"
```
```java
```
```java
+ getSignType();
```
```java
```
```java
```
```java
// start the pay.
```
```java
```
```java
MobileSecurePayer msp =
```
```java
new
```
```java
MobileSecurePayer();
```
```java
```
```java
boolean
```
```java
bRet = msp.pay(info, mHandler, AlixId.RQF_PAY, mActivity);
```
```java
```
```java
```
```java
if
```
```java
(bRet) {
```
```java
```
```java
// show the progress bar to indicate that we have started paying.
```
```java
```
```java
closeProgress();
```
```java
```
```java
mProgress = BaseHelper.showProgress(mActivity,
```
```java
null
```
```java
,
```
```java
"正在支付"
```
```java
,
```
```java
false
```
```java
,
```
```java
```
```java
true
```
```java
);
```
```java
```
```java
}
```
```java
else
```
```java
```
```java
;
```
```java
```
```java
}
```
```java
catch
```
```java
(Exception ex) {
```
```java
```
```java
Toast.makeText(mActivity, R.string.remote_call_failed,
```
```java
```
```java
Toast.LENGTH_SHORT).show();
```
```java
```
```java
}
```
```java
```
```java
```
```java
}
```
```java
```
```java
private
```
```java
boolean
```
```java
checkInfo() {
```
```java
```
```java
String partner = PartnerConfig.PARTNER;
```
```java
```
```java
String seller = PartnerConfig.SELLER;
```
```java
```
```java
if
```
```java
(partner ==
```
```java
null
```
```java
|| partner.length() <=
```
```java
0
```
```java
|| seller ==
```
```java
null
```
```java
```
```java
|| seller.length() <=
```
```java
0
```
```java
)
```
```java
```
```java
return
```
```java
false
```
```java
;
```
```java
```
```java
return
```
```java
true
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
// get the selected order info for pay.
```
```java
```
```java
String getOrderInfo() {
```
```java
```
```java
String strOrderInfo =
```
```java
"partner="
```
```java
+
```
```java
"\""
```
```java
+ PartnerConfig.PARTNER +
```
```java
"\""
```
```java
;
```
```java
```
```java
strOrderInfo +=
```
```java
"&"
```
```java
;
```
```java
```
```java
strOrderInfo +=
```
```java
"seller="
```
```java
+
```
```java
"\""
```
```java
+ PartnerConfig.SELLER +
```
```java
"\""
```
```java
;
```
```java
```
```java
strOrderInfo +=
```
```java
"&"
```
```java
;
```
```java
```
```java
strOrderInfo +=
```
```java
"out_trade_no="
```
```java
+
```
```java
"\""
```
```java
+ getOutTradeNo() +
```
```java
"\""
```
```java
;
```
```java
```
```java
strOrderInfo +=
```
```java
"&"
```
```java
;
```
```java
```
```java
//这笔交易价钱
```
```java
```
```java
strOrderInfo +=
```
```java
"subject="
```
```java
+
```
```java
"\""
```
```java
+ mActivity.getString(R.string.donate_subject) +
```
```java
"\""
```
```java
;
```
```java
```
```java
strOrderInfo +=
```
```java
"&"
```
```java
;
```
```java
```
```java
//这笔交易内容
```
```java
```
```java
strOrderInfo +=
```
```java
"body="
```
```java
+
```
```java
"\""
```
```java
+ mActivity.getString(R.string.donate_body) +
```
```java
"\""
```
```java
;
```
```java
```
```java
strOrderInfo +=
```
```java
"&"
```
```java
;
```
```java
```
```java
//这笔交易价钱
```
```java
```
```java
strOrderInfo +=
```
```java
"total_fee="
```
```java
+
```
```java
"\""
```
```java
+
```
```java
"10.00"
```
```java
+
```
```java
"\""
```
```java
;
```
```java
```
```java
strOrderInfo +=
```
```java
"&"
```
```java
;
```
```java
```
```java
strOrderInfo +=
```
```java
"notify_url="
```
```java
+
```
```java
"\""
```
```java
```
```java
+
```
```java
"[http://notify.java.jpxx.org/index.jsp](http://notify.java.jpxx.org/index.jsp)"
```
```java
+
```
```java
"\""
```
```java
;
```
```java
```
```java
return
```
```java
strOrderInfo;
```
```java
```
```java
}
```
```java
```
```java
// get the out_trade_no for an order.
```
```java
```
```java
String getOutTradeNo() {
```
```java
```
```java
SimpleDateFormat format =
```
```java
new
```
```java
SimpleDateFormat(
```
```java
"MMddHHmmss"
```
```java
);
```
```java
```
```java
Date date =
```
```java
new
```
```java
Date();
```
```java
```
```java
String strKey = format.format(date);
```
```java
```
```java
java.util.Random r =
```
```java
new
```
```java
java.util.Random();
```
```java
```
```java
strKey = strKey + r.nextInt();
```
```java
```
```java
strKey = strKey.substring(
```
```java
0
```
```java
,
```
```java
15
```
```java
);
```
```java
```
```java
return
```
```java
strKey;
```
```java
```
```java
}
```
```java
```
```java
// get the sign type we use.
```
```java
```
```java
String getSignType() {
```
```java
```
```java
String getSignType =
```
```java
"sign_type="
```
```java
+
```
```java
"\""
```
```java
+
```
```java
"RSA"
```
```java
+
```
```java
"\""
```
```java
;
```
```java
```
```java
return
```
```java
getSignType;
```
```java
```
```java
}
```
```java
```
```java
// sign the order info.
```
```java
```
```java
String sign(String signType, String content) {
```
```java
```
```java
return
```
```java
Rsa.sign(content, PartnerConfig.RSA_PRIVATE);
```
```java
```
```java
}
```
```java
```
```java
// the OnCancelListener for lephone platform.
```
```java
```
```java
static
```
```java
class
```
```java
AlixOnCancelListener
```
```java
implements
```
```java
```
```java
DialogInterface.OnCancelListener {
```
```java
```
```java
Activity mcontext;
```
```java
```
```java
AlixOnCancelListener(Activity context) {
```
```java
```
```java
mcontext = context;
```
```java
```
```java
}
```
```java
```
```java
public
```
```java
void
```
```java
onCancel(DialogInterface dialog) {
```
```java
```
```java
mcontext.onKeyDown(KeyEvent.KEYCODE_BACK,
```
```java
null
```
```java
);
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
}
```
```
这个类的pay方法就是支付的方法，最简单的不设置的话，调用方法如下：
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```
```java
AlixPay alixPay =
```
```java
new
```
```java
AlixPay(SettingTabActivity.
```
```java
this
```
```java
);
```
```java
alixPay.pay();
```
```
如果没有安装支付宝，它会提示你安装，如果已经安装，它直接让你选择付款：
![](http://pic002.cnblogs.com/images/2012/31770/2012040422291258.png)
这说明已经配置成功了。
然后可以删掉那些示例java文件了： AlixDemo.java,ProductListAdapter.java,Products.java。 
你也可以通过调整参数来修改订单信息，如主题，价格等。
另外在BaseHelper的94行：
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```
```java
dialog.setOnCancelListener(
```
```java
new
```
```java
AlixDemo.AlixOnCancelListener( (Activity)context ) );
```
```
需要修改为：
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```
```java
dialog.setOnCancelListener(
```
```java
new
```
```java
AlixPay.AlixOnCancelListener( (Activity)context ) );
```
```
**7.注意**
我在测试的时候，调用的activity是框在一个ActivityGroup里的（与tabhost类似，据说tabhost也有这个问题），导致MobileSecurePayer.java的pay方法中调用服务的两行代码：
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```java
mActivity.bindService(
```
```java
new
```
```java
Intent(IAlixPay.
```
```java
class
```
```java
.getName()), mAlixPayConnection, Context.BIND_AUTO_CREATE);
```
```java
mActivity.unbindService(mAlixPayConnection);
```
需要修改为：
[?](http://www.cnblogs.com/qianxudetianxia/archive/2012/04/04/2432406.html#)
```
```java
mActivity.getApplicationContext().bindService(
```
```java
new
```
```java
Intent(IAlixPay.
```
```java
class
```
```java
.getName()), mAlixPayConnection, Context.BIND_AUTO_CREATE);
```
```java
mActivity.getApplicationContext().unbindService(mAlixPayConnection);
```
```
不然会报错java.lang.ClassCastException: android.os.BinderProxy cannot be cast to com.android.server.am.ActivityRecord$Token...
**8.小结**
支付宝的集成比我想象的要复杂一些，比较麻烦，首先需要审核，然后代码需要提取，所以写出来与大家分享。 
在做集成配置的时候，一定要仔细认真，一个地方出错，可能要导致后面查错查很长时间。
因为本人是先集成成功后才写的这篇文章，难免会漏掉一些重要的细节或者步骤，如有不对，请留言指正。 
分类: [Android学习系列](http://www.cnblogs.com/qianxudetianxia/category/293007.html)
标签: [Android](http://www.cnblogs.com/qianxudetianxia/tag/Android/), [Android学习系列](http://www.cnblogs.com/qianxudetianxia/tag/Android%E5%AD%A6%E4%B9%A0%E7%B3%BB%E5%88%97/)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [android开发之支付宝集成教程](http://www.eoeandroid.com/thread-166772-1-1.html)
大家好，今天这讲，我们来学习下如何在Andrdoid平台上集成支付宝的支付接口。
现在跑步进入了移动互联网时代，移动支付必然是一个趋势，虽然目前还没普及，但是，时间问题，我相信，到了年末，甚至就会越来越多的人用手机支付。我同学有一个网店，我也打算用他的网店，利用淘宝无线的开源API，加上支付宝的无线支付，去开发一个小软件，当然只在计划中，最后会不会做成，技术不是问题，问题是因为校园招聘已经开始了，我马上要准备找工作了，压力比较大，做什么都没有激情了，定下工作的事是目前最大的事情。
好了，闲话少说，大家跟着我走，我只给大家列一个大体的框架，具体怎么做，其实文档说的很明白了，不过作为新手，想要尽快集成，还是挺麻烦的一个事。
第一步：你首先要有淘宝网和支付宝的账号，并且在支付宝是实名认证的用户，这个条件是必须的，为了安全。没完成的朋友，先去完成这步。
第二步：https://mobiless.alipay.com/index.htm
这个网址是支付宝无线平台的官网，不好找，所以贴出来给大家。
登陆之后，在我的产品那可以签约，因为我已经签约完毕，并且好像每个账号只能签约一种商品，所以我没法给大家截图，一点一点看。大家根据页面的提示和文字，很容易就可以明白的，我不多说。
这是我签约后的页面：
![](http://www.apkbus.com/data/attachment/forum/201108/30/1056131ssa53u3dk4vk6v6.jpg) 2011-8-30 10:56:13 上传
**下载附件** (16.47 KB)
签约时，要填写必要的信息，还有一个地方要上传你应用的APK文件，这里你就选一个能运行的DEMO就可以，在审核的时候，支付宝的人会给你打电话审核的，问你集成支付宝干什么，有什么想法等等。审核可能要2、3天吧。大家先有个心理准备。审核通过以后就是下面这个页面。
![](http://www.apkbus.com/data/attachment/forum/201108/30/105615rkklsr77eelel2q8.jpg) 2011-8-30 10:56:15 上传
**下载附件** (49.17 KB)
技术支持那有群号，大家不懂的可以去问，有人回答的，而且是官方人员，这点做的比较好。
账户信息那里非常重要，在集成时会用到。为了安全，我就把我的给涂鸦了，大家不要介意。还有一个地方，点击密钥管理，在这里要生成支付宝公钥和商户公钥。具体操作步骤见文档这个地方：安全支付服务Android平台开发指南(20110217).pdf  4.2 RSA算法签名。都在我附件中。按照说明去生成各种密钥，还有上传什么的，成功后就是下面这样的了。
![](http://www.apkbus.com/data/attachment/forum/201108/30/105617cretrbwebejbygug.jpg) 2011-8-30 10:56:17 上传
**下载附件** (82.3 KB)
做好了前期准备后，我们要开始写代码了。
他们给了两个例子代码，都在我附件中。
AppDemo4_0413
SampleMsp
然后我自己写了一个，很简单，没做过多的界面美化，AliTest。
大家具体看这篇文档：支付宝安全支付简单教程.docx，非常简单容易。按这篇文档去操作就一切都OK了。
大家要把示例文档中PartnerConfig.java中的一些参数啊、密钥啊换成自己的。为了让大家测试，我把我的放进去了，大家可以直接运行程序，但是千万别做不好的事，我还想去淘宝或者支付宝工作呢，千万别做不好的事。测试的时候，大家在支付的时候要用自己的支付宝账号测试，每次会支付一分钱，安全性大家可以放心。
[测试](http://www.eoeandroid.com/misc.php?mod=tag&id=10),
[签名](http://www.eoeandroid.com/misc.php?mod=tag&id=115), [框架](http://www.eoeandroid.com/misc.php?mod=tag&id=4), [APK](http://www.eoeandroid.com/misc.php?mod=tag&id=15), [算法](http://www.eoeandroid.com/misc.php?mod=tag&id=91)
[本帖地址:   http://www.eoeandroid.com/thread-166772-1-1.html](http://www.eoeandroid.com/thread-166772-1-1.html)[[复制链接]](http://www.eoeandroid.com/thread-166772-1-1.html)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### Android笔记30: 应用内支付宝支付集成
2011-05-27 23:37:59|  分类：[android笔记](http://snmoney.blog.163.com/blog/#m=0&t=1&c=fks_084067083082086070082094084095080082081067081094)|  标签：[android](http://snmoney.blog.163.com/blog/#m=0&t=3&c=android)[支付宝](http://snmoney.blog.163.com/blog/#m=0&t=3&c=%E6%94%AF%E4%BB%98%E5%AE%9D)[集成](http://snmoney.blog.163.com/blog/#m=0&t=3&c=%E9%9B%86%E6%88%90)|字号订阅
嗯，其实官方的文档和DEMO够用了，只是我悟性太不堪。
为自己把过程要点给备忘一下。
## 准备工作
1. 要有一个支付宝账号
2. 去[http://ms.alipay.com](http://ms.alipay.com/) 登录和自助签约 “[无线部-自助签约-手机安全支付](https://mobiless.alipay.com/product/product_center.htm?product_id=1)”；
3. 完成后登录 ms.alipay.com 在“我的产品页” 会见到 账户信息像以下内容
账户名：[abc@xxx.com](mailto:abc@xxx.com)
账户ID：208800*******993
合作商户ID：2088*******993
4. 这个时候先不管密钥管理，先去下载SDK包和文档，
    下载地址 [https://mobiless.alipay.com/product/product_down_load.htm?product_id=1](https://mobiless.alipay.com/product/product_down_load.htm?product_id=1)
     解压出以下东东备用 
- DEMO项目  //里面有好多现成的类，好东西~
- mobile_sp.jar //支付的类库
- mobile_sp.apk //支付宝安全支付控件，手机支付的前提，可捆绑在应用安装包内支付前检测和提示用户安装
- RSA密钥的W32生成工具 //不解释... 交易流程sign用
5. 按文档说明生成商家私钥、公钥对，再将私钥转换为 PKCS8 格式，备用~
6. 回到 ms.alipay.com 进入账号信息下的密钥管理，上传刚生成的商家公钥，获得 AliPay的对应公钥，备用~
（注意不要混淆 商家pub_key 和Ali_pub_key）
## 准备工作 part.2
呃，还得准备准备，刚才说了DEMO里面有很多好东西，可以直接拿来就用，省下不少爬文档和代码的时间省力又省心..有用的是
- AlixId.java
- Base64.java
- BaseHelper.java
- MobileSecurePayer.java
- MobileSecurePayHelper.java
- NetworkManager.java
- ResultChecker.java
- Rsa.java
全部拿出来扔到我们自己的项目包内，然后逐一来看看
**AlixId.java** ——这个不需要动，只是一些静态的全局变量；
**Base64.java** ——编码算法，不动；
**Rsa.java** ——同上，不动；
**BaseHelper.java** ——一些调用 ProgressDialog AlertDialog 还有格式转换的现成的方法，不动；
**MobileSecurePayer.java** ——不用改动，里面的 pay((String, Handler, int, Activity) ，支付用它；
**MobileSecurePayHelper.java **——这里要注意，捆绑安装支付控件的话，要把mobile_sp.apk 放到项目 assets 路径下，文件名可以改别的随便，但要注意与源码中52行 cachePath 文件名一致，还有 retrieveApkFromAssets()中的第二个文件名参数一致。若还打算提供在线下载安装控件的话还要更改 apkUrl ，url根据实际情况修改。其余的不用动，是一些检验、安装安全控件的方法；
**NetworkManager.java** ——不用动，上面如果启用了在线下载安装控件，就要用到这个包实现下载。
**ResultChecker.java** ——支付结果的解释和校验用，留意 Rsa.doCheck() 的第三个参数，这里要用的是 Ali_pub_key；
## 开工集成
public class Pay extends Activity { 
 Spinner spPrice;   //下拉菜单，有几个不同金额的支付项目选择
 Button btAlipayl; //支付按钮
 ProgressDialog pd; //提示Loading信息用
 /** Called when the activity is first created. */
 @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.pay);
        //绑定实例
        spPrice = (Spinner) findViewById(R.id.sp_pay);
        btAlipay = (Button) findViewById(R.id.bt_pay_alipay);
        pd = new ProgressDialog(this);
        pd.setCancelable(false);
        //点击支付按钮
        btAlipay.setOnClickListener(new OnClickListener(){
   @Override
   public void onClick(View v) {
    // TODO Auto-generated method stub
    // check to see if the MobileSecurePay is already installed. 没有支付安全控件的话提示安装
    MobileSecurePayHelper mspHelper =  new MobileSecurePayHelper(Pay.this);
    boolean isMobile_spExist = mspHelper.detectMobile_sp();
    if( !isMobile_spExist )
     return;
    // start pay for this order.
    try
    {   
     // prepare the order info.
     String orderInfo  = getOrderInfo(spPrice.getSelectedItemPosition()); //组织要发去支付接口的数据，这里传递了一个下拉菜单的选中序号，我在Array.xml仲分别定义了下拉菜单各项对应的商品名称、详情、价格数组
     String strsign   = Rsa.sign(orderInfo,getString(R.string.privateKey_Rsa)); //rsa算法，文档说支持md5，我用出错不清楚原因，换回来了
     strsign    = URLEncoder.encode(strsign);
     String info   =  orderInfo
           + "&sign=" +  "\"" + strsign  + "\""
           + "&" + getSignType();
     // start the pay. 支付
     MobileSecurePayer msp = new MobileSecurePayer();
     boolean bRet = msp.pay(info, mHandler, AlixId.RQF_PAY, Pay.this);
     if( bRet )
     {// 尝试收起ProgressDialog
      try
         { if( pd != null ) pd.dismiss(); }
         catch(Exception e)
         {e.printStackTrace(); }
      //然后投放支付中的提示
         pd.setMessage("正在支付");
         pd.show();         
     }
     else ;
    }
    catch (Exception ex)
    {showMsg(getString(R.string.remote_call_failed), false); }    
  } });
}
 //Alipay 的函数族
 // get the selected order info for pay. 组织交易数据
 String getOrderInfo(int arg2)
 {
  //以下内容在string.xml 或Array.xml中定义，参考SDK文档
  String strOrderInfo = "partner=" + "\"" + getString(R.string.partner) + "\"";
  strOrderInfo   += "&";
  strOrderInfo   += "seller=" + "\"" + getString(R.string.seller) + "\"";
  strOrderInfo   += "&";
  strOrderInfo  += "out_trade_no=" + "\"" + getOutTradeNo() + "\"";
  strOrderInfo   += "&";
  strOrderInfo  += "subject=" + "\"" + getString(R.string.subject) + "\"";
  strOrderInfo   += "&";
  strOrderInfo  += "body=" + "\"" + getResources().getStringArray(R.array.pay_name)[arg2] + "\"";
  strOrderInfo   += "&";
  strOrderInfo  += "total_fee=" + "\"" + getResources().getStringArray(R.array.pay_ap)[arg2] + "\"";
  strOrderInfo   += "&";
  strOrderInfo  += "notify_url="  + "\"" + getString(R.string.notify_url) + "\"";
  return strOrderInfo;
 }
 // get the out_trade_no for an order.根据文档生成一个不重复的商家交易号ID
 String getOutTradeNo()
 {
  SimpleDateFormat format = new SimpleDateFormat("MMddHHmmss");
  Date date = new Date();
  String strKey = format.format(date);
  java.util.Random r = new java.util.Random(); 
  strKey = strKey + r.nextInt();
  strKey = strKey.substring(0, 15);
  return strKey;
 }
 // get the sign type we use. 定义签名的算法，这里用了RSA
 String getSignType()
 {
  String getSignType = "sign_type=" + "\"" + "RSA" + "\"";
  return getSignType;
 }
  // the handler use to receive the pay result. 接收响应支付结果的 Handler
 private Handler mHandler = new Handler()
 {
  public void handleMessage(Message msg)
  { 
   try
   {
    String strRet = (String)msg.obj;  //这个是交易结果返回的信息由几部分组成，如: resultStatus={};memo={};result={}
    switch (msg.what)
    {
     case AlixId.RQF_PAY:
     {
      //如果支付中提示仍在，收起
      try
         {if( pd != null )  pd.dismiss();}
          catch(Exception e)
         {e.printStackTrace(); }      
      //把返回的结果拆包..解析成各部分信息，包括交易结果的状态码、错误信息等
      try
      {
       String memo  = "memo=";
       int imemoStart  = strRet.indexOf("memo=");
       imemoStart  +=  memo.length();
       int imemoEnd  = strRet.indexOf(";result=");
       memo = strRet.substring(imemoStart, imemoEnd);
       BaseHelper.showDialog(PayForm.this, "提示", memo, R.drawable.icon);
       // handle result
       ResultChecker resultChecker = new ResultChecker(strRet);
       boolean isPayOk = resultChecker.isPayOk(); //这种是严谨的验证支付结果是否成功的方法，不严谨的可以只检测状态码是否9000详情参考文档说明
       if(isPayOk){//支付成功了，以下该干啥干啥...
           ... ...
           }else ;  //支付不成功的处理...
      }
      catch(Exception e)
      { e.printStackTrace();      }
     }
     break; 
    }
    super.handleMessage(msg);
   }
   catch(Exception e)
   {e.printStackTrace();}
 } }; 
}
