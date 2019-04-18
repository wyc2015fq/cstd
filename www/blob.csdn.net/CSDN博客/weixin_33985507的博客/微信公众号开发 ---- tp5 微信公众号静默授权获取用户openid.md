# 微信公众号开发 ---- tp5 微信公众号静默授权获取用户openid - weixin_33985507的博客 - CSDN博客
2018年08月11日 10:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：22
###### 关于网页授权回调域名的说明
1、在微信公众号请求用户网页授权之前，开发者需要先到公众平台官网中的“开发 - 接口权限 - 网页服务 - 网页帐号 - 网页授权获取用户基本信息”的配置选项中，修改授权回调域名。请注意，这里填写的是域名（是一个字符串），而不是URL，因此请勿加 http:// 等协议头；
2、授权回调域名配置规范为全域名，比如需要网页授权的域名为：[www.qq.com](http://www.qq.com)，配置以后此域名下面的页面[http://www.qq.com/music.html](http://www.qq.com/music.html) 、 [http://www.qq.com/login.html](http://www.qq.com/login.html) 都可以进行OAuth2.0鉴权。但[http://pay.qq.com](http://pay.qq.com/) 、 [http://music.qq.com](http://music.qq.com/) 、 [http://qq.com无法进行OAuth2.0鉴权](http://qq.xn--comoauth2-735sh62dwk9eysua.xn--0-k76bu98j/)
###### 关于网页授权的两种scope的区别说明
1、以snsapi_base为scope发起的网页授权，是用来获取进入页面的用户的openid的，并且是静默授权并自动跳转到回调页的。用户感知的就是直接进入了回调页（往往是业务页面）
2、以snsapi_userinfo为scope发起的网页授权，是用来获取用户的基本信息的。但这种授权需要用户手动同意，并且由于用户同意过，所以无须关注，就可在授权后获取该用户的基本信息。
3、用户管理类接口中的“获取用户基本信息接口”，是在用户和公众号产生消息交互或关注后事件推送后，才能根据用户OpenID来获取用户基本信息。这个接口，包括其他微信接口，都是需要该用户（即openid）关注了公众号后，才能调用成功的。
###### 关于网页授权access_token和普通access_token的区别
1、微信网页授权是通过OAuth2.0机制实现的，在用户授权给公众号后，公众号可以获取到一个网页授权特有的接口调用凭证（网页授权access_token），通过网页授权access_token可以进行授权后接口调用，如获取用户基本信息；
2、其他微信接口，需要通过基础支持中的“获取access_token”接口来获取到的普通access_token调用。
关于特殊场景下的静默授权
###### 关于特殊场景下的静默授权
1、上面已经提到，对于以snsapi_base为scope的网页授权，就静默授权的，用户无感知；
2、对于已关注公众号的用户，如果用户从公众号的会话或者自定义菜单进入本公众号的网页授权页，即使是scope为snsapi_userinfo，也是静默授权，用户无感知。
具体而言，网页授权流程分为四步：
1、引导用户进入授权页面同意授权，获取code
2、通过code换取网页授权access_token（与基础支持中的access_token不同）
3、如果需要，开发者可以刷新网页授权access_token，避免过期
4、通过网页授权access_token和openid获取用户基本信息（支持UnionID机制）
###### 第一步：用户同意授权，获取code
链接：[https://open.weixin.qq.com/connect/oauth2/authorize?appid=APPID&redirect_uri=REDIRECT_URI&response_type=code&scope=SCOPE&state=STATE#wechat_redirect](https://open.weixin.qq.com/connect/oauth2/authorize?appid=APPID&redirect_uri=REDIRECT_URI&response_type=code&scope=SCOPE&state=STATE#wechat_redirect)
参数说明:
|参数|是否必须|说明|
|----|----|----|
|appid|是|公众号的唯一标识|
|redirect_uri|是|授权后重定向的回调链接地址， 请使用 urlEncode 对链接进行处理|
|response_type|是|返回类型，请填写code|
|scope|是|应用授权作用域，snsapi_base（不弹出授权页面，直接跳转，只能获取用户openid），snsapi_userinfo（弹出授权页面，可通过openid拿到昵称、性别、所在地。并且， 即使在未关注的情况下，只要用户授权，也能获取其信息 ）|
|state|否|重定向后会带上state参数，开发者可以填写a-zA-Z0-9的参数值，最多128字节|
|#wechat_redirect|是|无论直接打开还是做页面302重定向时候，必须带此参数|
###### 第二步：通过code换取网页授权access_token
首先请注意，这里通过code换取的是一个特殊的网页授权access_token,与基础支持中的access_token（该access_token用于调用其他接口）不同。
如果网页授权的作用域为snsapi_base，则本步骤中获取到网页授权access_token的同时，也获取到了openid，snsapi_base式的网页授权流程即到此为止。
链接： [https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code](https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code)
参数说明：
|参数|是否必须|说明|
|----|----|----|
|appid|是|公众号的唯一标识|
|secret|是|公众号的appsecret|
|code|是|填写第一步获取的code参数|
|grant_type|是|填写为authorization_code|
返回说明：
正确时返回的JSON数据包如下：
{
"access_token":"ACCESS_TOKEN",
"expires_in":7200,
"refresh_token":"REFRESH_TOKEN",
"openid":"OPENID",
"scope":"SCOPE"
}
|参数|描述|
|----|----|
|access_token|网页授权接口调用凭证,注意：此access_token与基础支持的access_token不同|
|expires_in|access_token接口调用凭证超时时间，单位（秒）|
|refresh_token|用户刷新access_token|
|openid|用户唯一标识，请注意，在未关注公众号时，用户访问公众号的网页，也会产生一个用户和公众号唯一的OpenID|
|scope|用户授权的作用域，使用逗号（,）分隔|
###### 示例代码：
```
<?php
/**
 * Created by PhpStorm.
 * User: Administrator
 * Date: 2018/8/6
 * Time: 22:41
 */
namespace app\index\controller;
use think\Controller;
class Wechat extends Controller
{
    protected $accessTokenUrl = 'https://api.weixin.qq.com/cgi-bin/token';
    protected $wechatAuthCodeUrl = 'https://open.weixin.qq.com/connect/oauth2/authorize?';
    protected $userOpenIdUrl = 'https://api.weixin.qq.com/sns/oauth2/access_token?';
    protected $appId;
    protected $secret;
    protected $code;
    protected $openId;
    /**
     * 加载微信配置
     */
    protected function _initialize(){
        $this->appId = config('wechat.appId');
        $this->secret = config('wechat.secret');
    }
    
    /**
     * 作用：格式化参数，签名过程需要使用
     * @param $paraMap
     * @param $urlencode
     * @return bool|string
     */
    protected function formatBizQueryParaMap($paraMap, $urlencode)
    {
        $buff = "";
        ksort($paraMap);
        foreach ($paraMap as $k => $v)
        {
            if($urlencode)
            {
                $v = urlencode($v);
            }
            $buff .= $k . "=" . $v . "&";
        }
        $reqPar = '';
        if (strlen($buff) > 0)
        {
            $reqPar = substr($buff, 0, strlen($buff)-1);
        }
        return $reqPar;
    }
    /**
     * 网页授权获取用户openId -- 1.获取授权code url
     */
    public function getWechatAuthCode(){
        // 获取来源地址
        $url = get_url();
        // 获取code
        $urlObj["appid"] = $this->appId;
        $urlObj["redirect_uri"] = "$url";
        $urlObj["response_type"] = "code";
        $urlObj["scope"] = "snsapi_base";
        $urlObj["state"] = "STATE"."#wechat_redirect";
        $bizString = $this->formatBizQueryParaMap($urlObj, false);
        $codeUrl =  $this->wechatAuthCodeUrl.$bizString;
        return $codeUrl;
    }
    /**
     * 网页授权获取用户openId -- 2.获取openid
     * @return mixed
     */
    public function getUserOpenId(){
        if (!isset($_GET['code']))
        {
            $codeUrl = $this->getWechatAuthCode();
            Header("Location: $codeUrl");
            die;
        }else{
            $code = $_GET['code'];
            $this->code = $code;
            // 请求openid
            $param = [
                'appid'     =>  $this->appId,
                'secret'    =>  $this->secret,
                'code'      =>  $this->code,
                'grant_type'=>  "authorization_code",
            ];
            $data = httpGuzzle('get',$this->userOpenIdUrl,$param);
            //取出openid
            $this->openId = $data['openid'];
            return $this->openId;
        }
    }
}
```
```
/**
 * 获取来源地址
 * @return string
 */
function get_url() {
    //获取来源地址
    $url = "http://".$_SERVER["HTTP_HOST"].$_SERVER["REQUEST_URI"];
    return $url;
}
```
###### 注意：记得配置接口回调域名哦~
![12218344-92a3a39a1f246777.png](https://upload-images.jianshu.io/upload_images/12218344-92a3a39a1f246777.png)
image.png
![12218344-2bf87852bfd00564.png](https://upload-images.jianshu.io/upload_images/12218344-2bf87852bfd00564.png)
image.png
