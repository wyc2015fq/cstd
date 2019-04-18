# 微信公众平台开发--OAuth2.0网页授权 - z69183787的专栏 - CSDN博客
2015年05月29日 10:13:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1215
转自：http://www.cnblogs.com/txw1958/p/weixin71-oauth20.html
## 一、什么是OAuth2.0
官方网站：http://oauth.net/   http://oauth.net/2/
权威定义：OAuth is An open protocol to allow secure authorization in a simple and standard method from web, mobile and desktop applications. 
OAuth是一个开放协议，允许用户让第三方应用以安全且标准的方式获取该用户在某一网站、移动或桌面应用上存储的私密的资源（如用户个人信息、照片、视频、联系人列表），而无需将用户名和密码提供给第三方应用。
OAuth 2.0是OAuth协议的下一版本，但不向后兼容OAuth 1.0。 OAuth 2.0关注客户端开发者的简易性，同时为Web应用，桌面应用和手机，和起居室设备提供专门的认证流程。
OAuth允许用户提供一个令牌，而不是用户名和密码来访问他们存放在特定服务提供者的数据。每一个令牌授权一个特定的网站（例如，视频编辑网站)在特定的时段（例如，接下来的2小时内）内访问特定的资源（例如仅仅是某一相册中的视频）。这样，OAuth允许用户授权第三方网站访问他们存储在另外的服务提供者上的信息，而不需要分享他们的访问许可或他们数据的所有内容。
新浪微博API目前也使用OAuth 2.0。
原文：http://www.cnblogs.com/txw1958/p/weixin71-oauth20.html
二、微信公众平台OAuth2.0授权
微信公众平台OAuth2.0授权详细步骤如下：
1. 用户关注微信公众账号。
2. 微信公众账号提供用户请求授权页面URL。
3. 用户点击授权页面URL，将向服务器发起请求
4. 服务器询问用户是否同意授权给微信公众账号(scope为snsapi_base时无此步骤)
5. 用户同意(scope为snsapi_base时无此步骤)
6. 服务器将CODE通过回调传给微信公众账号
7. 微信公众账号获得CODE
8. 微信公众账号通过CODE向服务器请求Access Token
9. 服务器返回Access Token和OpenID给微信公众账号
10. 微信公众账号通过Access Token向服务器请求用户信息(scope为snsapi_base时无此步骤)
11. 服务器将用户信息回送给微信公众账号(scope为snsapi_base时无此步骤)
![](http://images.cnitblog.com/blog/340216/201401/06193653-6c44fea30bc946238e6940d46ba0f1fc.jpg)
使用的AppId和AppSecret在开发者中心-开发者ID中，可以找到。
![](http://images.cnitblog.com/blog/340216/201411/292051020278792.jpg)
1. 配置授权回调页面域名
进入微信公众平台后台后，依次进入开发者中心-权限表，找到网页授权获取用户基本信息，
![](http://images.cnitblog.com/blog/340216/201411/292048235741068.jpg)
点击右侧的修改。原文：http://www.cnblogs.com/txw1958/p/weixin71-oauth20.html
![](http://images.cnitblog.com/blog/340216/201311/09103045-fbd48b2b755646e29cd3b4bf3ea5cd06.jpg)
授权回调域名配置规范为全域名并且不带http，比如需要网页授权的域名为：www.qq.com，配置以后此域名下面的页面http://www.qq.com/music.html 、 http://www.qq.com/login.html 都可以进行OAuth2.0鉴权。但http://pay.qq.com 、 http://music.qq.com
 、 http://qq.com无法进行OAuth2.0鉴权。
这里我们填写方倍工作室的一个百度应用二级域名为 mascot.duapp.com
![](http://images.cnitblog.com/blog/340216/201311/09104700-65c3e92a2eb84f5f8cc51d2f40d613d4.jpg)原文：http://www.cnblogs.com/txw1958/p/weixin71-oauth20.html
如果你的网址没有被列入过黑名单，就会在顶部出现
![](http://images.cnitblog.com/blog/340216/201311/09104959-fa946a92f6a54e0bb9a8c81854ab523e.jpg)
![](http://images.cnitblog.com/blog/340216/201311/09104836-bfae53f6217c4ccba70076c3c1ca6b8d.jpg)
然后，域名配置就成功了。
2. 用户授权并获取code
在域名根目录下，新建一个文件，命名为oauth2.php，其内容为
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?php
if (isset($_GET['code'])){
    echo $_GET['code'];
}else{
    echo "NO CODE";
}
?>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
先了解下请求授权页面的构造方式：
```
https://open.weixin.qq.com/connect/oauth2/authorize?appid=APPID&redirect_uri=REDIRECT_URI&response_type=code&scope=SCOPE&state=STATE#wechat_redirect
```
参数说明
|参数|必须|说明|
|----|----|----|
|appid|是|公众号的唯一标识|
|redirect_uri|是|授权后重定向的回调链接地址|
|response_type|是|返回类型，请填写code|
|scope|是|应用授权作用域，snsapi_base （不弹出授权页面，直接跳转，只能获取用户openid），snsapi_userinfo （弹出授权页面，可通过openid拿到昵称、性别、所在地。并且，即使在未关注的情况下，只要用户授权，也能获取其信息）|
|state|否|重定向后会带上state参数，开发者可以填写任意参数值|
|#wechat_redirect|否|直接在微信打开链接，可以不填此参数。做页面302重定向时候，必须带此参数|
应用授权作用域：由于snsapi_base只能获取到openid，意义不大，所以我们使用snsapi_userinfo。
回调地址：填写为刚才上传后的oauth2.php的文件地址，
state参数：随便一个数字，这里填1
构造请求url如下：
```
https://open.weixin.qq.com/connect/oauth2/authorize?appid=wx8888888888888888&redirect_uri=http://mascot.duapp.com/oauth2.php&response_type=code&scope=snsapi_userinfo&state=1#wechat_redirect
```
把这个链接发送到微信中，以便在微信浏览器中打开，这里使用A链接封装如下：
```
OAuth2.0网页授权演示 
<a href="https://open.weixin.qq.com/connect/oauth2/authorize?appid=wx8888888888888888&redirect_uri=http://mascot.duapp.com/oauth2.php&response_type=code&scope=snsapi_userinfo&state=1#wechat_redirect">点击这里体验</a>
技术支持 方倍工作室
```
在微信中显示如下
![](http://images.cnitblog.com/blog2015/340216/201505/051946364077837.png)
点击绑定后，弹出应用授权界面
![](http://images.cnitblog.com/blog2015/340216/201505/051940302041382.png)
选择允许，点击
![](http://images.cnitblog.com/blog/340216/201311/09143609-357950addb7f4a07bafc186b75c4cf17.png)
跳转到auth2.php页面，执行
```
echo $_GET['code']
```
界面上显示的就是code，这时候通过右上角按钮中的复制链接，得到链接如下：
```
http://mascot.duapp.com/oauth2.php?code=00b788e3b42043c8459a57a8d8ab5d9f&state=1
```
我们成功得到了code了。
```
注意：如果在绑定的时候出现这样的界面，就说明参数不对，需要回头检查一下参数
![](http://images.cnitblog.com/blog/340216/201311/09144149-03e710fa93854bf790ef097eb0557d01.png)
```
3. 使用code换取access_token
换取网页授权access_token页面的构造方式：
```
https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code
```
参数说明
|参数|是否必须|说明|
|----|----|----|
|appid|是|公众号的唯一标识|
|secret|是|公众号的appsecret|
|code|是|填写第一步获取的code参数|
|grant_type|是|填写为authorization_code|
code：在这里填写为上一步获得的值
构造请求url如下：
```
https://api.weixin.qq.com/sns/oauth2/access_token?appid=wx8888888888888888&secret=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa&code=00b788e3b42043c8459a57a8d8ab5d9f&grant_type=authorization_code
```
可以在浏览器中直接执行这条语句：
得到如下json数据:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
{
    "access_token": "OezXcEiiBSKSxW0eoylIeAsR0GmYd1awCffdHgb4fhS_KKf2CotGj2cBNUKQQvj-G0ZWEE5-uBjBz941EOPqDQy5sS_GCs2z40dnvU99Y5AI1bw2uqN--2jXoBLIM5d6L9RImvm8Vg8cBAiLpWA8Vw",
    "expires_in": 7200,
    "refresh_token": "OezXcEiiBSKSxW0eoylIeAsR0GmYd1awCffdHgb4fhS_KKf2CotGj2cBNUKQQvj-G0ZWEE5-uBjBz941EOPqDQy5sS_GCs2z40dnvU99Y5CZPAwZksiuz_6x_TfkLoXLU7kdKM2232WDXB3Msuzq1A",
    "openid": "oLVPpjqs9BhvzwPj5A-vTYAX3GLc",
    "scope": "snsapi_userinfo,"
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
数据格式解读如下：
|参数|描述|
|----|----|
|access_token|网页授权接口调用凭证,注意：此access_token与基础支持的access_token不同|
|expires_in|access_token接口调用凭证超时时间，单位（秒）|
|refresh_token|用户刷新access_token|
|openid|用户唯一标识，请注意，在未关注公众号时，用户访问公众号的网页，也会产生一个用户和公众号唯一的OpenID|
|scope|用户授权的作用域，使用逗号（,）分隔|
于是，我们成功的通过code换取到了access_token，以及refresh_token。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
刷新access_token
官方文档中提到了刷新access_token的功能，但这不是必须要做的，初次使用可以先忽略。
url请求方法如下：
```
https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=APPID&grant_type=refresh_token&refresh_token=REFRESH_TOKEN
```
参数说明|参数|是否必须|说明|
|----|----|----|
|appid|是|公众号的唯一标识|
|grant_type|是|填写为refresh_token|
|refresh_token|是|填写通过access_token获取到的refresh_token参数|
构造如下：
```
https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=wx8888888888888888&grant_type=refresh_token&refresh_token=OezXcEiiBSKSxW0eoylIeAsR0GmYd1awCffdHgb4fhS_KKf2CotGj2cBNUKQQvj-G0ZWEE5-uBjBz941EOPqDQy5sS_GCs2z40dnvU99Y5CZPAwZksiuz_6x_TfkLoXLU7kdKM2232WDXB3Msuzq1A
```
在浏览器中执行得到前面同样格式的json数据
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
4. 使用access_token获取用户信息
请求方法：
```
https://api.weixin.qq.com/sns/userinfo?access_token=ACCESS_TOKEN&openid=OPENID
```
参数说明
|参数|描述|
|----|----|
|access_token|网页授权接口调用凭证,注意：此access_token与基础支持的access_token不同|
|openid|用户的唯一标识|
构造url如下：
```
https://api.weixin.qq.com/sns/userinfo?access_token=OezXcEiiBSKSxW0eoylIeAsR0GmYd1awCffdHgb4fhS_KKf2CotGj2cBNUKQQvj-G0ZWEE5-uBjBz941EOPqDQy5sS_GCs2z40dnvU99Y5AI1bw2uqN--2jXoBLIM5d6L9RImvm8Vg8cBAiLpWA8Vw&openid=oLVPpjqs9BhvzwPj5A-vTYAX3GLc
```
可以在浏览器中直接执行这条语句：
得到如下json数据:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
{
    "openid": "oLVPpjqs9BhvzwPj5A-vTYAX3GLc",
    "nickname": "刺猬宝宝",
    "sex": 1,
    "language": "简体中文",
    "city": "深圳",
    "province": "广东",
    "country": "中国",
    "headimgurl": "http://wx.qlogo.cn/mmopen/utpKYf69VAbCRDRlbUsPsdQN38DoibCkrU6SAMCSNx558eTaLVM8PyM6jlEGzOrH67hyZibIZPXu4BK1XNWzSXB3Cs4qpBBg18/0",
    "privilege": []
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
参数解读：
|参数|描述|
|----|----|
|openid|用户的唯一标识|
|nickname|用户昵称|
|sex|用户的性别，值为1时是男性，值为2时是女性，值为0时是未知|
|province|用户个人资料填写的省份|
|city|普通用户个人资料填写的城市|
|country|国家，如中国为CN|
|headimgurl|用户头像，最后一个数值代表正方形头像大小（有0、46、64、96、132数值可选，0代表640*640正方形头像），用户没有头像时该项为空|
|privilege|用户特权信息，json 数组，如微信沃卡用户为（chinaunicom）|
这与我个人的微信信息是一致的
![](http://images.cnitblog.com/blog2015/340216/201505/051941286262567.png)
至此，在不输入我的账号及密码的情况下，微信公众账号近宝获得了我的个人信息，这些信息包括昵称、性别、国家、省份、城市、个人头像以及特权列表。
一个完整的OAuth2认证就完成了。
三、详细演示
扫描关注下列微信公众账号“方倍工作室”的二维码，回复“授权”。
![](http://images.cnitblog.com/blog2015/340216/201505/051942323604862.jpg)![](http://images.cnitblog.com/blog2015/340216/201505/051940123297209.png)
弹出获取到的结果
![](http://images.cnitblog.com/blog2015/340216/201505/051947109386654.png)![](http://images.cnitblog.com/blog2015/340216/201505/051947235321474.png)
