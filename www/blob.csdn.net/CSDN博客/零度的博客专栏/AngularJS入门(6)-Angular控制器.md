# AngularJS入门(6)-Angular控制器 - 零度的博客专栏 - CSDN博客
2016年11月03日 09:24:56[零度anngle](https://me.csdn.net/zmx729618)阅读数：265
在前面的博客中，我们能看到类似着这样的代码`<div ng-controller="ctrl1">`，这就是AngularJS的控制器指令。
AngularJS 应用程序被控制器控制，`ng-controller` 指令定义了应用程序控制器，控制器是 JavaScript 对象，由标准的 JavaScript 对象的构造函数创建。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
        <script type="text/javascript">
            var app = angular.module("myApp", []);
            app.controller("myCtrl", function($scope) {
                $scope.name = "jianggujin";
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <input type="text" ng-model="name" />
        <div>Hello {{name}}</div>
    </body>
</html>
```
[]()
AngularJS 应用程序由 `ng-app` 定义，应用程序在`<body>` 内运行。`ng-controller="myCtrl"` 属性是一个 AngularJS 指令，用于定义一个控制器，AngularJS 使用`$scope` 对象来调用控制器。
在 AngularJS 中， `$scope` 是一个应用对象(属于应用变量和函数)。控制器的 `$scope` （相当于作用域、控制范围）用来保存AngularJS Model(模型)的对象。
控制器在作用域中创建了一个属性 name。`ng-model` 指令绑定输入域到控制器的属性name。
在上面的示例演示了一个带有name属性的控制器对象。控制器也可以有方法（变量和函数）的，我们现在对上面的示例进行改造：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
        <script type="text/javascript">
            var app = angular.module("myApp", []);
            app.controller("myCtrl", function($scope) {
                $scope.name = "jianggujin";
                $scope.welcom = function() {
                    return "Hello " + $scope.name;
                }
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <input type="text" ng-model="name" />
        <div>{{welcom()}}</div>
    </body>
</html>
```
