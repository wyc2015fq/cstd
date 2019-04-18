# 微信开放平台开发——网页微信扫码登录（OAuth2.0） - z69183787的专栏 - CSDN博客
2018年02月04日 13:45:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：274
个人分类：[微信/支付宝开发](https://blog.csdn.net/z69183787/article/category/3277967)
[https://www.cnblogs.com/0201zcr/p/5133062.html](https://www.cnblogs.com/0201zcr/p/5133062.html)
1、OAuth2.0
**　　OAuth**（开放授权）是一个开放标准，允许用户让第三方应用访问该用户在某一网站上存储的私密的资源（如照片，视频，联系人列表），而无需将用户名和密码提供给第三方应用。
　　允许用户**提供一个令牌**，**而不是用户名和密码来访问他们存放在特定服务提供者的数据**。每一个令牌授权一个特定的网站（例如，视频编辑网站)在特定的时段（例如，接下来的2小时内）内访问特定的资源（例如仅仅是某一相册中的视频）。这样，OAuth允许用户授权第三方网站访问他们存储在另外的服务提供者上的信息，而不需要分享他们的访问许可或他们数据的所有内容。
2、目标
　　我们这里主要模拟使用OAuth2.0，用户通过扫描我们网页应用的二维码并进行授权登录来获取用户的基本信息的过程。详细的接口相关信息可以在微信开放平台上查看：[https://open.weixin.qq.com/cgi-bin/showdocument?action=dir_list&t=resource/res_list&verify=1&lang=zh_CN](https://open.weixin.qq.com/cgi-bin/showdocument?action=dir_list&t=resource/res_list&verify=1&lang=zh_CN)
3、前期准备（获取微信开发者权限）
　　我们这里主要讲的是网站（Web）应用，网站应用微信登录是基于OAuth2.0协议标准构建的微信OAuth2.0授权登录系统(即上面的协议)。在微信客户端授权登录(获取用户信息）的可以查看:[http://www.cnblogs.com/0201zcr/p/5131602.html](http://www.cnblogs.com/0201zcr/p/5131602.html)
　　在进行微信OAuth2.在进行微信OAuth2.0授权登录接入之前，在微信**开放平台注册开发者帐号，并拥有一个已审核通过的网站应用，并获得相应的AppID和AppSecret，申请微信登录且通过审核后，可开始接入流程。**
3.1、注册开发者账号
　　可以在[https://open.weixin.qq.com/](https://open.weixin.qq.com/) 这里申请开发的账号。由于是腾讯的网页，这里可以直接通过 QQ号进行登录。
3.2、提交网站应用审核
　　在已经登录的界面中选择“管理中心”——》网站应用——》创建网站应用
![](https://images2015.cnblogs.com/blog/731178/201601/731178-20160115131041991-1058874094.png)
 　　将会弹出下面的界面
![](https://images2015.cnblogs.com/blog/731178/201601/731178-20160115131108803-1188358406.png)
　　填写过后，还有有一个页面需要填写，提交一份纸质版申请书扫描件（会提供模板，我们下载再来填写后，需盖章，签名），配置回调域名（扫码登录后会跳转的页面）等。
　　之后提交审核即可，等微信审核通过，我们即可获得我们需要的网页应用的**appid和AppSecret，并配置后回调的域名了**（这三样是我们开发所必须的）。
3.3、开发者资质认证
　　由于我们这里要使用微信登录的接口，所以我们还需要向微信提出认证，只有认证了才能使用微信那些高级的接口。未认证的如下图所示
![](https://images2015.cnblogs.com/blog/731178/201601/731178-20160115131632616-547932195.png)
 　　认证之后是这样子的：
![](https://images2015.cnblogs.com/blog/731178/201601/731178-20160115131729288-457434641.png)
 　  我现在暂时没有找到可以向公众账号那样子的测试账号的申请。如果有知道怎么可以申请到测试账号的高手，希望能赐教一下。
　　接下来，我们就可以开始我们的网页微信扫码登录开发了。
4、授权流程说明
　　微信OAuth2.0授权登录让微信用户使用微信身份安全登录第三方应用或网站，在微信用户授权登录已接入微信OAuth2.0的第三方应用后，第三方可以获取到用户的接口调用凭证（access_token），通过access_token可以进行微信开放平台授权关系接口调用，从而可实现获取微信用户基本开放信息和帮助用户实现基础开放功能等。
　　微信OAuth2.0授权登录目前支持authorization_code模式，适用于拥有server端的应用授权。该模式整体流程为：
```
1. 第三方发起微信授权登录请求，微信用户允许授权第三方应用后，微信会拉起应用或重定向到第三方网站，并且带上授权临时票据code参数；
2. 通过code参数加上AppID和AppSecret等，通过API换取access_token；
3. 通过access_token进行接口调用，获取用户基本数据资源或帮助用户实现基本操作。
```
 　　获取access_token时序图：
![](https://images2015.cnblogs.com/blog/731178/201601/731178-20160115132145897-758264873.png)
5、获取网页的二维码
　　当我们通过微信的认证，获取到了**appid和AppSecret，并配置了回调的域名**。我们就已经可以获取属于我们网页的二维码了，获取的方式很简单，只需打开一个微信的链接，加上我们的appid和回调域名即可在网页上面打开二维码，用户用微信客户端扫码并授权登录之后即会跳转到我们配置的回调域名下。
**注意：**
```
1、这里填写的是域名（是一个字符串），而不是URL，因此请勿加http://等协议头；
2、授权回调域名配置规范为全域名，比如需要网页授权的域名为：www.qq.com，配置以后此域名下面的页面http://www.qq.com/music.html 、 http://www.qq.com/login.html 都可以进行OAuth2.0鉴权。但http://pay.qq.com 、 http://music.qq.com 、 http://qq.com无法进行OAuth2.0鉴权
```
5.1、请求url说明
　　第三方使用网站应用授权登录前请注意已获取相应网页授权作用域（scope=snsapi_login），则可以通过在PC端打开以下链接：
```
https://open.weixin.qq.com/connect/qrconnect?appid=APPID&redirect_uri=REDIRECT_URI&response_type=code&scope=SCOPE&state=STATE#wechat_redirect
```
** 参数说明**|参数|是否必须|说明|
|----|----|----|
|appid|是|应用唯一标识（前面认证网页应用中获得）|
|redirect_uri|是|重定向地址，需要进行UrlEncode（前面认证网页应用中获得）|
|response_type|是|填code|
|scope|是|应用授权作用域，拥有多个作用域用逗号（,）分隔，网页应用目前仅填写snsapi_login即可|
|state|否|用于保持请求和回调的状态，授权请求后原样带回给第三方。该参数可用于防止csrf攻击（跨站请求伪造攻击），建议第三方带上该参数，可设置为简单的随机数加session进行校验|
** 返回说明**
 　　用户允许授权后，将会重定向到redirect_uri的网址上，并且带上code和state参数
```
redirect_uri?code=CODE&state=STATE
```
 　　若用户禁止授权，则重定向后不会带上code参数，仅会带上state参数
```
redirect_uri?state=STATE
```
5.2、事例：
　　一号店的微信二维码链接如下：
```
https://open.weixin.qq.com/connect/qrconnect?appid=wxbdc5610cc59c1631&redirect_uri=https%3A%2F%2Fpassport.yhd.com%2Fwechat%2Fcallback.do&response_type=code&scope=snsapi_login&state=3d6be0a4035d839573b04816624a415e#wechat_redirect
```
 　　将其复制到浏览器中打开即可获得一号店的二维码，二维码页面如下：
![](https://images2015.cnblogs.com/blog/731178/201601/731178-20160116103317257-2080473155.png)
　　通过使用微信客户端的扫一扫功能，扫描该二维码，即会跳转到上面填写redirect_uri所在的地址上。假如**用户同意授权**，这里就获得了微信返回的**code参数**了。
6、获取用户信息
　　假如前面已经获得code。我们可以通过code参数去获取用户openid和access_token,进而获得用户的信息。
6.1、通过code参数获取access_token
```
https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code
```
** 参数说明**
|参数|是否必须|说明|
|----|----|----|
|appid|是|应用唯一标识，在微信开放平台提交应用审核通过后获得|
|secret|是|应用密钥AppSecret，在微信开放平台提交应用审核通过后获得|
|code|是|填写第一步获取的code参数|
|grant_type|是|填authorization_code|
###### 返回说明
正确的返回：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
{ 
"access_token":"ACCESS_TOKEN", 
"expires_in":7200, 
"refresh_token":"REFRESH_TOKEN",
"openid":"OPENID", 
"scope":"SCOPE",
"unionid": "o6_bmasdasdsad6_2sgVt7hMZOPfL"
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
|参数|说明|
|----|----|
|access_token|接口调用凭证|
|expires_in|access_token接口调用凭证超时时间，单位（秒）|
|refresh_token|用户刷新access_token|
|openid|授权用户唯一标识|
|scope|用户授权的作用域，使用逗号（,）分隔|
|unionid|当且仅当该网站应用已获得该用户的userinfo授权时，才会出现该字段。|
错误返回样例：
```
{"errcode":40029,"errmsg":"invalid code"}
```
**注意**：
- **code参数的超时时间是5分钟，且每次请求的code参数的值都不一样。**
- **access_token的超时时间是32分钟。**
6.2、通过access_token获取用户的基本信息
获取的前提条件
- access_token有效且为超时；
- 微信用户已授权给第三方应用账号相应接口作用域（scope)【**在二维码生成连接那里填写**】
对于接口作用域（scope），能调用的接口有以下：
|授权作用域（scope）|接口|接口说明|
|----|----|----|
|snsapi_base|/sns/oauth2/access_token|通过code换取access_token、refresh_token和已授权scope|
|/sns/oauth2/refresh_token|刷新或续期access_token使用| |
|/sns/auth|检查access_token有效性| |
|snsapi_userinfo|/sns/userinfo|获取用户个人信息|
　　使用snsapi_base作用域的授权是扫码之后无需用户点击授权，扫码后直接跳转，用户感觉不到授权了，但这种授权方式能获取的数据量有限，这里我们要获取用户的基本信息，我们需要使用snsapi_userinfo授权。使用snsapi_userinfo授权，扫码后出现类似于下面的授权界面
![](https://images2015.cnblogs.com/blog/731178/201601/731178-20160116104851882-1088738021.jpg)
　　此接口用于获取用户个人信息。开发者可通过OpenID来获取用户基本信息。特别需要注意的是，**如果开发者拥有多个移动应用、网站应用和公众帐号，可通过获取用户基本信息中的unionid来区分用户的唯一性，因为只要是同一个微信开放平台帐号下的移动应用、网站应用和公众帐号，用户的unionid是唯一的。换句话说，同一用户，对同一个微信开放平台下的不同应用，unionid是相同的。**请注意，在用户修改微信头像后，旧的微信头像URL将会失效，因此开发者应该自己在获取用户信息后，将头像图片保存下来，避免微信头像URL失效后的异常情况。
```
https://api.weixin.qq.com/sns/userinfo?access_token=ACCESS_TOKEN&openid=OPENID
```
** 参数说明**
|参数|是否必须|说明|
|----|----|----|
|access_token|是|调用凭证**（上一个请求中获得）**|
|openid|是|普通用户的标识，对当前开发者帐号唯一**（上一个请求中获得）**|
|lang|否|国家地区语言版本，zh_CN 简体，zh_TW 繁体，en 英语，默认为zh-CN|
###### 返回说明
正确的Json返回结果：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
{ 
"openid":"OPENID",
"nickname":"NICKNAME",
"sex":1,
"province":"PROVINCE",
"city":"CITY",
"country":"COUNTRY",
"headimgurl": "http://wx.qlogo.cn/mmopen/g3MonUZtNHkdmzicIlibx6iaFqAc56vxLSUfpb6n5WKSYVY0ChQKkiaJSgQ1dZuTOgvLLrhJbERQQ4eMsv84eavHiaiceqxibJxCfHe/0",
"privilege":[
"PRIVILEGE1", 
"PRIVILEGE2"
],
"unionid": " o6_bmasdasdsad6_2sgVt7hMZOPfL"
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
|参数|说明|
|----|----|
|openid|普通用户的标识，对当前开发者帐号唯一|
|nickname|普通用户昵称|
|sex|普通用户性别，1为男性，2为女性|
|province|普通用户个人资料填写的省份|
|city|普通用户个人资料填写的城市|
|country|国家，如中国为CN|
|headimgurl|用户头像，最后一个数值代表正方形头像大小（有0、46、64、96、132数值可选，0代表640*640正方形头像），用户没有头像时该项为空|
|privilege|用户特权信息，json数组，如微信沃卡用户为（chinaunicom）|
|unionid|用户统一标识。针对一个微信开放平台帐号下的应用，同一用户的unionid是唯一的。|
错误的Json返回示例:
```
{ 
"errcode":40003,"errmsg":"invalid openid"
}
```
7、总结 
　　最近着手开发了微信网页扫码登录和公众号授权登录收获颇丰，两者的开发很类似。以下是我个人摸索过程中发现的两者的异同：
- 两者都可以通过微信客户端扫码授权的方式，让第三方页面获得微信用户的一些基本信息（昵称、性别、所在地、在微信唯一标示等……）。他们都是通过提供一个链接让用户授权的方式。但网页版需要在页面打开二维码之后授权，而公众号则需要用户先关注了我们的公众号，然后点开公众号里面的链接，确认授权即可。
- 网页扫码登录需要将授权的链接（二维码链接）在网页中打开、而公众号授权登录的链接必须要微信客户端中打开。
- 无论网页扫码登录还是在公众号中授权登录，都是通过授权的方式获得一个code参数，之后通过code参数获取access_token和openid和通过access_token和openid去获取用户的基本信息的请求链接是一样的。
- 在开发公众号授权登录的过程中，我发现了有测试账号的提供，足以满足我们的测试和开发，但在开发网页扫码时，暂时未发现哪里能获取测试账号，我是通过申请获取的。（希望知道哪里有测试账号的请求高手赐教）。
 公众账号授权登录：[http://www.cnblogs.com/0201zcr/p/5131602.html](http://www.cnblogs.com/0201zcr/p/5131602.html)
微信公众号群发消息：[http://www.cnblogs.com/0201zcr/p/5866296.html](http://www.cnblogs.com/0201zcr/p/5866296.html)
