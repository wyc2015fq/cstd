
# 中国再现大规模Android手机病毒 逾50万手机受感染 - 雷锋网 - CSDN博客


2012年08月20日 09:53:52[leiphone](https://me.csdn.net/leiphone)阅读数：2780


![](http://www.leiphone.com/wp-content/uploads/2012/08/virus.jpg)因为GooglePlay的种种限制，随着中国市场Android设备的飞速增长，中国独特的应用商店生态系统已经催生出了数百家独立的Android应用商店。然而众多的第三方市场也导致病毒滋生，让Android这一开源系统更加脆弱，据[TNW报道](http://thenextweb.com/asia/2012/08/19/stealth-sms-payment-malware-identified-chinese-app-stores-500000-android-devices-infected/)近期在中国Android市场中又发现一种新的病毒，该病毒可以通过短信擅自进行手机支付，据称受感染的用户已经超过50万，你中招了么？
早在六周前，MMarketPlay应用商店曾发现账单入侵病毒，近日反病毒专家TrustGo又发现了一例比较复杂的新型Android病毒，它能够在用户的Android手机上进行手机支付、读取用户银行卡、信用卡以及账单支付历史信息。
该病毒被命名为“Trojan!SMSZombie”（“特洛伊！SMS僵尸”），由TrustGo反病毒公司于7月25日首次发现。据称TrustGo是目前唯一一家能够识别此类病毒并提供病毒清除方案的公司。
![](http://www.leiphone.com/wp-content/uploads/2012/08/virus-520x245.jpg)
据[TNW](http://thenextweb.com/asia/2012/08/19/stealth-sms-payment-malware-identified-chinese-app-stores-500000-android-devices-infected/)称目前中国有多家主流应用商店都遭到该病毒的侵袭，截止到目前为止已有超过50万的用户设备遭感染。虽然50万相对于中国6.83亿的手机用户来说只是沧海一粟，但是这例病毒能私下进行未经授权的支付交易，有可能会给用户带来巨大的经济损失，因而该病毒的潜在威胁非常大。
而且，为了避免用户起疑，制造该病毒的黑客采取了比较谨慎的做法，该病毒每次进行的支付都是小额的，基本上很少产生高额账单，这样受感染用户也不会轻易发现。据TrustGo称，到目前为止，该病毒只是通过重新支付网络游戏和其他服务的方式从受感染用户账户中获取“相对较低”的金额而已，但是积少成多，50万份较低金额加起来恐怕也是一笔不小的数目吧。
![](http://www.leiphone.com/wp-content/uploads/2012/08/adin-180x300.png)
TrustGo在其官方博客上解释说：该病毒通常是隐藏在墙纸应用和热门下载应用当中，然后入侵用户的SMS功能，利用中国移动（看来联通、电信的用户是没事了）的一个漏洞——短信支付网关，进行交易和访问数据（这一切都是在后台未经用户授权的情况下进行）。
> ““Trojan!SMSZombie”（“特洛伊！SMS僵尸”）病毒往往是隐藏在各种墙纸应用中，以充满吸引力的标题和图像来诱惑用户下载安装。当用户将该受感染的应用作为手机壁纸之后，该应用就会要求用户安装与病毒相关的其他文件。如果用户同意并进入下一步，那么该病毒就会通过一个名为“Android System Service”（Android系统服务）的文件来进行交易数据的传输。

> 一旦用户安装了这类受感染应用，该病毒就会尝试不断获取用户设备的管理员权限，并且该操作用户是无法取消的，因为按“取消”按钮只会重新加载确认对话框，用户最终不得不按“激活”来停止这个对话框，然后就这样被强制激活了该病毒。而且当该病毒获取管理员权限之后，用户就无法卸载这个受感染的应用，用户点击“卸载”的时候，手机会自动跳回到主屏幕。”
由于中国许多手机用户都通过手机短信来进行手机支付，而且很多银行都会将账单信息发送到用户的手机当中，所以当用户的手机受该病毒感染之后，银行卡和账户信息很容易就会被该病毒截取并进行小额支付。
TrustGo表示，受该病毒感染的用户或者疑似并担心受该病毒感染的用户可以下载TrustGo的移动安全应用进行查杀，或者直接参考TrustGo网站（trustgo.com/en/smszombie-eliminate ）上的方法清除病毒。
目前TrustGo正在着手升级其手机安全应用来实现对该病毒的自动清除，预计于本月底之前通过更新就可以实现。
> “这种病毒感染后会自动复制并传递恶意代码，所以很难检测。像这种比较复杂的新型病毒的出现也突显Android这一开放性平台的弊端，有时候开源也是一把双刃剑。手机用户在接触众多应用程序的同时，还得小心防范手机病毒的感染，也许你就是黑客的下一个目标！”
Via[ TNW](http://thenextweb.com/asia/2012/08/19/stealth-sms-payment-malware-identified-chinese-app-stores-500000-android-devices-infected/)

**（****[流云](http://www.leiphone.com/author/%E6%B5%81%E4%BA%91)****供****雷锋网****专稿，转载请注明来自雷锋网及作者，并链回本页)**

