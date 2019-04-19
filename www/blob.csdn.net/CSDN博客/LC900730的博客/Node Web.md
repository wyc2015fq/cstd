# Node Web - LC900730的博客 - CSDN博客
2017年08月28日 11:17:11[lc900730](https://me.csdn.net/LC900730)阅读数：129标签：[node-js](https://so.csdn.net/so/search/s.do?q=node-js&t=blog)
个人分类：[nodejs](https://blog.csdn.net/LC900730/article/category/7127126)
## Node的Http模块
```java
var http=require('http');
var server=http.createServer(function(req,res){
    //处理请求
})
```
需要http.createServer()函数。它只有1个参数，是个回调函数，服务器每次收到HTTP请求都会调用这个回调函数。这个回调会收到2个参数，请求和响应对象，req和res;
