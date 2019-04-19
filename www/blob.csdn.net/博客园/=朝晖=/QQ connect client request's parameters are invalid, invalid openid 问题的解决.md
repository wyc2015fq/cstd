# QQ connect client request's parameters are invalid, invalid openid 问题的解决 - =朝晖= - 博客园
# [QQ connect client request's parameters are invalid, invalid openid 问题的解决](https://www.cnblogs.com/dhcn/p/7106471.html)
   很多人的这个问题是POST的时候发生，我的也恰好在POST的时候发生。后来我发现可能是因为QQ的这个后端是采用类[PHP](http://lib.csdn.net/base/php)的语言开发，在动态语言的获取参数时POST参数和GET参数是可以分开读取的，也就是说如果你把openid、access_token这些数据放在URL GET参数里面，采用$_POST这样的接口就无法读取到openid参数，所以导致POST数据时报错说Invalid openid,当然我的采用的开发语言是官方不支持的，所以SDK是个山寨版，所以我对SDK做了自己的修改：把权限验证数据全部添加到了POST Data部分。

