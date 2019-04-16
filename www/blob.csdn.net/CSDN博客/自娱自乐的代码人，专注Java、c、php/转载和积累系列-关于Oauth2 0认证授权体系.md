# 转载和积累系列 - 关于Oauth2.0认证授权体系 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月10日 16:07:44[initphp](https://me.csdn.net/initphp)阅读数：2606
个人分类：[转载和积累系列](https://blog.csdn.net/initphp/article/category/2609089)








- 关于Oauth2.0


度娘说：是OAuth协议的下一版本，但不[向后兼容](http://baike.baidu.com/view/544.htm)OAuth 1.0。 OAuth 2.0关注[客户端](http://baike.baidu.com/view/930.htm)开发者的简易性，同时为Web应用，[桌面](http://baike.baidu.com/view/79807.htm)应用和手机，和起居室设备提供专门的认证流程。规范还在IETF
 OAuth[工作组](http://baike.baidu.com/view/281640.htm)的开发中，按照Eran Hammer-Lahav的说法，OAuth将于2010年末完成。


- 一个Oauth2.0的实例：

- 首先用户从第三方网站页面，302跳转到xxx开放平台的账号中心。

这个时候，账号中心会让你输入用户名和密码（账号都是开放平台上面登录，所以是安全的。）

需要输出的参数：appkey、回跳地址、认证类型

URL：https://account.xxx.com/oauth/authorize?oauth_consumer_key=YOUR_APP_KEY&oauth_callback=YOUR_CALLBACK_URL&response_type=code


- 如果账号这些正确，就会302跳转到刚才用户所在的第三方网站的web站点

跳转过去的时候，开放平台会带上一个临时授权的code，这code是临时的，不安全的，用于换取真正的Access Token用。

URL：https://www.xxx.com/oauth_callback?code=CODE_GENERATE_BY_ACCOUNT


- 第三方如果是服务端的话，就可以获取到刚才的临时授权code，可以通过http请求，调用开放平台的API，从开放平台将code换取Access Token

需要调用开放平台接口，并且带上参数：授权临时code、appkey等

URL：https://account.xxxx.com/oauth/access_token?code=CODE_GET_BEFORE&grant_type=authorization_code&oauth_consumer_key=YOUR_APP_KEY&oauth_consumer_secret=YOUR_APP_SEC&oauth_callback=YOUR_CALLBACK_URL


- 最后就可以拿到Access Token了，拿到这个Token后，就可以安心的调用开放平台那边的API接口了。



- 一个流程图，来源网络




![](https://img-my.csdn.net/uploads/201212/10/1355129292_4363.gif)


- 说明：

- 为什么第一次要跳转到开放平台的页面上？因为用户名密码这些都需要在开放平台上登录，如果在第三方平台登录，那么完全可以获取得到用户的密码信息，会导致数据的不安全。
- 拿到临时授权的code之后，为什么不能把它当Access Token呢？而要通过再次去请求开放平台？因为临时code是不安全的，如果用户在第一步骤登录成功后，开放平台直接把Access Token传递过来，那么第三方开发者就不能验证拿到的这个AccessToken是否是合法的。





