# 微信图片上传 invalid credential, access_token is invalid or not latest - 三少GG - CSDN博客
2016年09月06日 07:25:08[三少GG](https://me.csdn.net/scut1135)阅读数：9831

这个问题可能是因为你部署的时候，起了多个进程，每个进程都去微信的服务器获取一次access_token，只有最后一个获取到的access_token才有效，比如，如果你用gunicorn去启django，并设置4个进程，那么你会发现，每上传4个图片可能只有一个成功，其它都失败。我就遇到这个问题了。
`access_token是公众号的全局唯一接口调用凭据，公众号调用各接口时都需使用access_token。开发者需要进行妥善保存。access_token的存储至少要保留512个字符空间。access_token的有效期目前为2个小时，需定时刷新，重复获取将导致上次获取的access_token失效。`
公司的应用调了一个微信上传下载图片的接口，本来在线上跑的好好的，什么问题没有，但是这两天总是不定时的出现下载下来的图片损坏，拿着Token和serverid去接口测试网页验证，返回的是Token失效了，返回信息："errcode": 40001, "errmsg": "invalid
 credential, access_token is invalid or not latest hint: [17ZG50664ent3]。我是把Token和ticket都缓存下来了，过两个小时再去取，只有一个入口去取Token，结果跟了两天，就是不知道Token什么时候失效的。今天打算继续跟，突然想起来，我开发时测试用的域名和线上用的域名是同一个，结果导致我开发时取了一次Token，导致线上的失效了，这个常识性的失误，让我受了两天折磨，真是郁闷啊。
http://www.hutuseng.com/article/weixin-api-invalid-credential
查看了一下报错信息如下：
invalid credential, access_token is invalid or not latest hint
大意是access_token无效或者不是最新的， 于是想到access_token每次从微信服务器取一次就新生成一次，同时老的token就失效了。
而我们服务器上不止这一个业务调用了微信接口，我们的网页也加了微信转发分享的功能，也是要取access_token的，而这两个业务的access_token是分开缓存的，没有打通，因此出现了上面的问题。
解决方法，就是**整个系统使用统一的获取access_token的方法**，保证access_token的有效性。
