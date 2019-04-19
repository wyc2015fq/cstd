# AngularJS入门(5)-Angular作用域($Scope) - 零度的博客专栏 - CSDN博客
2016年11月03日 09:14:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：413标签：[angularjs](https://so.csdn.net/so/search/s.do?q=angularjs&t=blog)
个人分类：[AngularJS](https://blog.csdn.net/zmx729618/article/category/6499854)
Scope(作用域) 是应用在 HTML (视图) 和 JavaScript (控制器)之间的纽带。Scope 是一个对象，有可用的方法和属性， 可应用在视图和控制器上。
当我们在 AngularJS 创建控制器时，我们可以将 `$scope` 对象当作一个参数传递，当在控制器中添加 `$scope` 对象时，视图 (HTML) 可以获取了这些属性。视图中，我们不需要添加
`$scope` 前缀, 只需要添加属性名即可，如： {{carname}}。 
在一个复杂的页面上，我们可能会使用多个scope，所以要注意scope对应的作用域对应的是那个。 
如果我们需要scope 的作用域在整个应用内都有效，那么我们可以使用根作用域，即`$rootScope`，`$rootScope` 可作用于整个应用中。是各个controller中scope的桥梁。用$rootscope 定义的值，可以在各个 controller 中使用。
下面我们通过一段代码来演示scope 的使用：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
        <script type="text/javascript">
            var app = angular.module("myApp", []);
            app.run(function($rootScope){
                $rootScope.first = "jiang";
            });
            app.controller("ctrl1", function($scope){
                $scope.last = "gujin";
            });
            app.controller("ctrl2", function($scope){
                $scope.last = "haiqing";
            });
        </script>
    </head>
    <body ng-app="myApp">
        <div ng-controller="ctrl1">
            {{first}} {{last}}
        </div>
        <div ng-controller="ctrl2">
            {{first}} {{last}}
        </div>
    </body>
</html>
```
[]()
页面显示效果如下：
![这里写图片描述](https://img-blog.csdn.net/20160319112515948)
看到实际运行的效果，我们很容易的就可以知道`$scope`是相互独立的，而`$rootScope`是全局的。
