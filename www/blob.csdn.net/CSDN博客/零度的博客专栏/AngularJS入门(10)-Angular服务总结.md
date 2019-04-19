# AngularJS入门(10)-Angular服务总结 - 零度的博客专栏 - CSDN博客
2016年11月03日 11:44:05[零度anngle](https://me.csdn.net/zmx729618)阅读数：276标签：[angularjs](https://so.csdn.net/so/search/s.do?q=angularjs&t=blog)
个人分类：[AngularJS](https://blog.csdn.net/zmx729618/article/category/6499854)

在之前介绍AngularJS的博客中，简单的介绍了AngularJS提供的常用服务，以及如何自定义服务，具体可参见《[AngularJS 服务(Service)](http://blog.csdn.net/jianggujin/article/details/50950142)》。在这一篇博客中，我将为大家详细介绍如何自定义服务，以及各种方式之间的区别。
# value(name, object)
`value(name, object)`方法允许我们直接将一个普通值或对象作为服务。我们通过一段代码来看看如何使用：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", [])
                .value("HQValue", 10)
                .controller("myCtrl", function($scope, HQValue) {
                    $scope.value = HQValue;
                });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        {{value}}
    </body>
</html>
```
[]()
在这段代码中，我们是用`value`方法定义了一个服务，服务名称为`HQValue`，值为：10，这是一个很简单的例子。
# constant(name, value)
`constant(name, value)`也可以注册一个服务，值可以是一个字符串、数字、数组、对象或函数，和`value(name, object)`很像有木有。对上面的示例进行简单的修改：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", [])
                .constant("HQConstant", 10)
                .config(function(HQConstant) {
                    console.info(HQConstant);
                })
                .controller("myCtrl", function($scope, HQConstant) {
                    $scope.value = HQConstant;
                });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        {{value}}
    </body>
</html>
```
[]()
细心的小伙伴应该可以发现`constant(name, value)`和`value(name, object)`的用法虽然很想，但是二者也是有区别的，不然也就没必要搞两个出来了，二者最大的区别在于使用`constant(name, value)`注册的服务可以在`config(configFn)`方法中注入，而`value(name, object)`不可以。通常用
```
value(name,
 object)
```
来注册服务对象或者函数，而用`constant(name, value)`来配置数据。
# service(name, constructor)
使用`service(name, constructor)`注册服务，服务对象是使用`new`进行实例化的，所以我们应该给`this`添加属性。
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
[]()
在这段代码中，我们使用了`service(name, constructor)`注册了一个服务，服务名为`HQString`，我们为该服务添加了一个`toUpperCase`用于将字符串转换为大写。
# factory(name, providerFunction)
使用`factory(name, providerFunction)`注册服务其实就是创建一个对象作为`providerFunction`的返回值，注入到`controller`中的其实就是创建的对象。我们使用`factory(name, providerFunction)`来实现`service(name, constructor)`同样的功能，代码如下：
```
<head>
    <meta charset="UTF-8">
    <title></title>
    <script src="js/angular.min.js"></script>
    <script type="text/javascript">
        angular.module("myApp", [])
            .factory('HQString', function() {
                return {
                    toUpperCase: function(x) {
                        return x.toUpperCase();
                    }
                };
            }).controller("myCtrl", function($scope, HQString) {
                $scope.name = HQString.toUpperCase('jianggujin');
            });
    </script>
</head>
<body ng-app="myApp" ng-controller="myCtrl">
    <div>{{name}}</div>
</body>
```
[]()
# provider(name, provider)
`provider(name, provider)`是可以传入`config(configFn)`的服务，如果我们想要在 service对象启用之前，先进行模块范围的配置，那就应该用 provider。使用`provider(name, provider)`注册服务，我们需要为`this`添加`$get`函数，函数返回用于注入到`controller`中对象，我们也可以为`this`添加其他属性以便于我们在`config(configFn)`方法中使用。需要注意的是，如果我们的服务名称为：`HQString`，那么我们在`config(configFn)`方法中注入的名称应该为：`HQStringProvider`。下面我们通过代码来看一下如何使用：
```
<head>
    <meta charset="UTF-8">
    <title></title>
    <script src="js/angular.min.js"></script>
    <script type="text/javascript">
        angular.module("myApp", [])
            .provider('HQString', function() {
                this.defaults = "defaults";
                this.$get = function() {
                    return {
                        toUpperCase: function(x) {
                            return x.toUpperCase();
                        }
                    };
                }
            }).config(function(HQStringProvider) {
                console.info(HQStringProvider.defaults)
            }).controller("myCtrl", function($scope, HQString) {
                $scope.name = HQString.toUpperCase('jianggujin');
            });
    </script>
</head>
<body ng-app="myApp" ng-controller="myCtrl">
    <div>{{name}}</div>
</body>
```
[]()
