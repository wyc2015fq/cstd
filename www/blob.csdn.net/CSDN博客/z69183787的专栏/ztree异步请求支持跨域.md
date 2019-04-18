# ztree异步请求支持跨域 - z69183787的专栏 - CSDN博客
2014年05月05日 11:17:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2062
1、请尝试一下 setting.async.dataType="jsonp" 同时让你的接口可以支持 callback 方式封装即可。。 你可以搜索一下 jQuery 的 jsonp 使用。
2、若需要自定义的jsonp 与 jsoncallback函数
你好，在async里面可以传jsonp和jsoncallback么？ 我们系统可能现在需要自己设置callback的名字才能解析返回的json字符串，我设置了之后还是默认的callback名字，请问你这个怎么修改？
不好意思，目前没有这个扩展。。不过你可以利用
 setting.async.dataFilter 将数据 传到你指定的 function 中进行预处理一下。。。这个方案可行么？
