# oAuth - 左直拳的马桶_日用桶 - CSDN博客
2014年10月14日 22:38:17[左直拳](https://me.csdn.net/leftfist)阅读数：1679
个人分类：[oAuth](https://blog.csdn.net/leftfist/article/category/2634359)
这里说的oAuth是指oAuth2。
oAuth是一种第三方的认证方式。
比如说，我想访问A网，需要认证：
**第一步：授权**
1、于是A网将我转到提供认证服务的B网（其实，常见的是我选择了一家可认证的网站，比如新浪微博，腾讯QQ，人人，等等）；
2、登陆B网后，B网会询问我，允许A网得到我哪些授权？比如说，可以访问我的头像信息，邮箱信息，手机号码。我选择或确认之后，B网就会生成一个唯一的授权码Auth Code等通知A网；
**第二步：获取访问令牌和身份ID**
3、A网获得授权码Auth Code后，接着向B网请求访问令牌（Access Token）
4、B向A发送访问令牌（Access Token）
5、A收到后，又向B请求OpenID，也就是询问我是谁，即获取我的身份ID；
6、B又将我的OpenID（身份ID）发给A。
**第三步：凭访问令牌和身份ID，A调用B提供的API,，获得了我的授权和身份信息**
如此完成了一次验证过程，我最终得以进入A网。
![](https://img-blog.csdn.net/20141015002243535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在使用过程中，只要我保持活动，A网会在超时前及时刷新，从而维持我的登录状态。
这个就是oAuth2。
在此之前，有oAuth1.0，跟oAuth2的区别是，在发现需要认证的时候，是A主动先去询问B，然后再引导用户去B授权；再有就是2.0有多种获取访问令牌的方式，等等。总的来说，2.0比1.0简单，但两者不兼容。
参考资料：
[http://www.coin163.com/doc/oauth.html](http://www.coin163.com/doc/oauth.html)
[http://www.cnblogs.com/artech/p/oauth-03.html](http://www.cnblogs.com/artech/p/oauth-03.html)
