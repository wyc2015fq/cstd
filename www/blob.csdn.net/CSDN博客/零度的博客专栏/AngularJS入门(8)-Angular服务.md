# AngularJS入门(8)-Angular服务 - 零度的博客专栏 - CSDN博客
2016年11月03日 10:31:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：296标签：[angularjs](https://so.csdn.net/so/search/s.do?q=angularjs&t=blog)
个人分类：[AngularJS](https://blog.csdn.net/zmx729618/article/category/6499854)
在 AngularJS 中，服务是一个函数或对象，可在我们的 AngularJS 应用中使用。AngularJS 内建了30 多个服务。这一篇博客将为大家介绍一些服务的使用方式以及如何创建我们自己的服务。
[AngularJS服务的API可以点击这里查看](http://docs.angularjs.cn/api/ng/service)
# $http
`$http` 是 AngularJS 应用中最常用的服务。 服务向服务器发送请求，应用响应服务器传送过来的数据。可以理解成jQuery中的ajax。
首先我们创建一个JSON数据文件，用于`$http`访问。
`[1,2,3,4,5,6,7,8,9]`[]()
接下来我们使用`$http`读取数据：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", []).controller("myCtrl", function($scope, $http) {
                $http.get("data.json").success(function(data) {
                    $scope.items = data;
                });
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <ul>
            <li ng-repeat="x in items">{{x}}</li>
        </ul>
    </body>
</html>
```
[]()
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160321201604846)
# $interval
AngularJS `$interval` 服务对应了 JS window.setInterval 函数。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", []).controller("myCtrl", function($scope, $interval) {
                $scope.now = new Date();
                $interval(function() {
                    $scope.now = new Date();
                }, 1000);
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{now|date:'yyyy:MM:dd HH:mm:ss'}}</div>
    </body>
</html>
```
![](http://static.blog.csdn.net/images/save_snippets.png)
这样就实现了一个时钟效果，运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160321202100753)
# $location
AngularJS中的`$location`服务是对`window.location`的封装。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", []).controller("myCtrl", function($scope, $location) {
               $scope.items = [];
               $scope.items.push("absUrl:" + $location.absUrl());
               $scope.items.push("protocol:" + $location.protocol());
               $scope.items.push("host:" + $location.host());
               $scope.items.push("port:" + $location.port());
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <ul>
            <li ng-repeat="item in items">{{item}}</li>
        </ul>
    </body>
</html>
```
[]()
以上介绍的为`$location`的基本用法，另外，`$location`服务还提供了获得锚点等信息的方法，下面着重介绍一下`$location`服务获得查询参数的使用，刚接触的时候也走了一些弯路。 在`$location`服务中将查询参数转换为JSON对象的方法为`search()`。
首先：假设我们的页面的访问地址为：`http://127.0.0.1:8020/location.html`，我们在其后添加一些参数，如：`http://127.0.0.1:8020/location.html?p1=123&p2=456`，我们理想中得到的JSON数据应为：`{p1:123,p2:456}`，我们来试一下是这样子的吗？
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", []).controller("myCtrl", function($scope, $location) {
               $scope.params = $location.search();
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{params}}</div>
    </body>
</html>
```
[]()
我们访问之后会发现页面显示的是：`{}`，并未没有取到数据，这是AngularJS的BUG，并不是这样子，在`$location`的API中提供了这样的实例
> 
// given url [http://example.com/#/some/path?foo=bar&baz=xoxo](http://example.com/#/some/path?foo=bar&baz=xoxo)
var searchObject = $location.search(); 
// => {foo: ‘bar’, baz: ‘xoxo’}
// set foo to ‘yipee’ 
location.search() => {foo: ‘yipee’, baz: ‘xoxo’}
我们仿照API的例子将我们的地址改造一下，改造成如下形式： `[http://127.0.0.1:8020/mui/location.html#?p1=123&p2=456](http://127.0.0.1:8020/mui/location.html#?p1=123&p2=456)`
这时候我们会发现页面上显示了我们预测的内容。其实在不对请求地址进行改造的情况下，我们也可以获得查询参数的，我们的代码应该这样写：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <!--必须添加base元素，否则报错-->
        <base href="/" />
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", []).
            config(function($locationProvider) {
                $locationProvider.html5Mode(true);
            }).controller("myCtrl", function($scope, $location) {
                $scope.params = $location.search();
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{params}}</div>
    </body>
</html>
```
[]()
# 自定义服务
上面介绍了一些AngularJS中内置的服务，下面来介绍一下如何自定义自己的服务，看过上一篇《[AngularJS 过滤器](http://blog.csdn.net/jianggujin/article/details/50930823)》的朋友应该可以猜到自定义服务的方式，和自定义过滤器很像。下面我们就通过代码来演示一下如何自定义服务。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", [])
                .service('HQString', function() {
                    this.toUpperCase = function(x) {
                        return x.toUpperCase();
                    }
                }).controller("myCtrl", function($scope, HQString) {
                    $scope.name = HQString.toUpperCase('jianggujin');
                });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{name}}</div>
    </body>
</html>
```
![](http://static.blog.csdn.net/images/save_snippets.png)
在这段代码中，我们自定义了一个服务，并向其中添加了一个方法将字符串转换为大写，运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160321205444191)
