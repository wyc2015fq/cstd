# AngularJS中Route例子 - weixin_33985507的博客 - CSDN博客
2017年08月24日 19:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
代码：[https://files.cnblogs.com/files/xiandedanteng/angularJSRouteSample.rar](https://files.cnblogs.com/files/xiandedanteng/angularJSRouteSample.rar)
点击‘首页’后：
![](https://images2017.cnblogs.com/blog/162475/201708/162475-20170824192222793-1538201187.png)
点击‘电脑’后：
![](https://images2017.cnblogs.com/blog/162475/201708/162475-20170824192240308-616088733.png)
```
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
    <head>
        <meta charset="utf-8">
        <title>AngularJS 路由实例 - 菜鸟教程</title>
    </head>
    <body ng-app='routingDemoApp'>
     
        <h2>AngularJS 路由应用</h2>
        <ul>
            <li><a href="#/">首页</a></li>
            <li><a href="#/computers">电脑</a></li>
            <li><a href="#/printers">打印机</a></li>
            <li><a href="#/blabla">其他</a></li>
        </ul>
         
        <div ng-view></div>
    </body>
</html>
<script src="angular1.4.6.min.js"></script>
<script src="angular-route_1.3.13.js"></script>
<script>
    angular.module('routingDemoApp',['ngRoute'])
    .config(['$routeProvider', function($routeProvider){
        $routeProvider
        .when('/',{template:'这是首页页面'})
        .when('/computers',{template:'这是电脑页面'})
        .when('/printers',{template:'这是打印机页面'})
        .otherwise({redirectTo:'/'});
    }]);
</script>
```
