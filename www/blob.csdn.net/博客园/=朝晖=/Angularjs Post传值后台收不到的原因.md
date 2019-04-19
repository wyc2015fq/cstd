# Angularjs Post传值后台收不到的原因 - =朝晖= - 博客园
# [Angularjs Post传值后台收不到的原因](https://www.cnblogs.com/dhcn/p/7115423.html)
     如果你给[AngularJS](http://lib.csdn.net/base/angularjs)的post方法的data参数创一个key-value对象，那传给后台服务的就是JSON字符串，而正常的POST解析是需要像get?后面的那种&name=value这样的字符串，所以后端解析不到参数，简单处理方法，给app加全局http过滤器设置：
```
var app = angular.module('myApp');  
  
app.config(function ($httpProvider) {
    $httpProvider.defaults.headers.post['Content-Type'] =         'application/x-www-form-urlencoded;charset=utf-8';  
$httpProvider.defaults.transformRequest = function(data){  
    if (data === undefined) {  
        return data;  
    }  
    return $.param(data);  
}
```
 如果在ionic等移动环境下，jquery不一定可用，$.param可以由zepto.js提供。
参考：http://stackoverflow.com/questions/12190166/[angularjs](http://lib.csdn.net/base/angularjs)-any-way-for-http-post-to-send-request-parameters-instead-of-json
         http://my.oschina[.NET](http://lib.csdn.net/base/dotnet)/idiotsky/blog/219640

