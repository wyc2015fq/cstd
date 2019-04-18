# 关于微信网页调用js-sdk相关接口注意事项目（一级域名与二级域名互相干扰！！！） - z69183787的专栏 - CSDN博客
2018年02月09日 10:33:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：857
      不知道有没有网友遇到过同一个web应用用不同的域名（一级或二级域名）在两个公众号中调用JSSDK相关接口实现功能，
这种做法本来没有问题，问题在于用二级域名（同属一级域名下的二级域名）绑定另一个web应用的ip在上面两个公众号中的其中一个做测试时，
问题就出现了。
　　　我们先理一下jssdk调用的流程，
1、利用appId与appSecret得到access_token;
2、利用access_token得到jsapi_ticket临时票据；
3、通过算法获得随机字符串$nonceStr
4、利用jsapi_ticket=$jsapiTicket    &noncestr=$nonceStr    ×tamp=$timestamp    &url=$url获取$signature签名；
5、jsapi_ticket、url、$timestamp、$nonceStr、$signature、appId共同组合成微信配置参数。
6、配置成功后可调用相关接口。
重点在于配置是否成功，
access_token与jsapi_ticket都时效性两个小时，使用时必须缓存，以免触发频率限制；
问题回来上面说的同一个众号中有两个应用在调用js接口，一个是生产的，已经上线的，一个测试的，专用测试用，两个应用都是主域名下的二级域名，
这时会出现接口调用冲突。！！！！
原因是如果安全域设置了一级域名，那它下面的二级域名同样可以调用js接口，也就是说一级域名与其下面的两级以下的域名统一对应一个公众号，
它们请求生成的access_token与jsapi_ticket具有唯一性，当一个域名请求后，另一个域名下调用时所使用的缓存access_token与jsapi_ticket就会失效，
从而调用失败。
简单来说， 一个公众号只能对应一个域名（包括其下面的二级以上域名），无论多少级域名调用接口，它们access_token与jsapi_ticket 是唯一的；
