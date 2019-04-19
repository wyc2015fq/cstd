# AngularJS入门(9)-与DOM有关的指令 - 零度的博客专栏 - CSDN博客
2016年11月03日 10:54:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：216标签：[angularjs](https://so.csdn.net/so/search/s.do?q=angularjs&t=blog)
个人分类：[AngularJS](https://blog.csdn.net/zmx729618/article/category/6499854)

AngularJS 为 HTML DOM 元素的属性提供了绑定应用数据的指令。今天就为大家介绍一下AngularJS中一些与HTML DOM操作有关的指令。
# ng-options
在 AngularJS 中我们可以使用 ng-option 指令来创建一个下拉列表，列表项通过对象和数组循环输出。示例代码如下：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", [])
                .controller("myCtrl", function($scope) {
                    $scope.items = ["red", "blue", "green"];
                });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <select ng-model="selectColor" ng-options="item for item in items"></select>
    </body>
</html>
```
[]()
这样就会显示一个下拉选择列表，看过之前博客的朋友应该知道，在AngularJS中有一个`ng-repeat`指令用于重复创建元素，那么我们用`ng-repeat`来实现同样的效果，代码如下：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", [])
                .controller("myCtrl", function($scope) {
                    $scope.items = ["red", "blue", "green"];
                });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <select ng-model="selectColor">
            <option ng-repeat="item in items">{{item}}</option>
        </select>
    </body>
</html>
```
[]()
这段代码也同样可以实现显示一个下拉选择框，那么在这两者当中，使用哪个更好呢？ 
`ng-repeat` 指令是通过数组来循环 HTML 代码来创建下拉列表，但 `ng-options` 指令更适合创建下拉列表，使用`ng-options` 的数据可以是对象， 而`ng-repeat` 是一个字符串。，当我们用于创建下拉选择框的数据是一个对象的时候，`ng-options`的优势就特别明显了。下面我们通过代码来演示一下：
``
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">angular.module("myApp", [])
       .controller("myCtrl", function($scope) {
        $scope.items = {
            red: {
                r: 255,
                g: 0,
                b: 0
            },
            green: {
                r: 0,
                g: 255,
                b: 0
            },
            blue: {
                r: 0,
                g: 0,
                b: 255
            }
```
```
}
       });
    </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <select ng-model="selectColor" ng-options="x for (x, y) in items"></select>
        <div>{{selectColor}}</div>
    </body>
</html>
```
[]()
# ng-disabled
`ng-disabled` 指令直接绑定应用程序数据到 HTML 的 disabled 属性。
示例代码：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", [])
                .controller("myCtrl", function($scope) {});
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <input type="checkbox" ng-model="checked" />
        <input ng-disabled="checked" type="button" value="按钮"/>
    </body>
</html>
```
[]()
# ng-show和ng-hide
`ng-show` 指令隐藏或显示一个 HTML 元素。`ng-hide` 指令也是用于隐藏或显示 HTML 元素。与`ng-show`相反。
示例代码：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="js/angular.min.js"></script>
        <script type="text/javascript">
            angular.module("myApp", [])
                .controller("myCtrl", function($scope) {});
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <input type="checkbox" ng-model="showed" />
        <input ng-show="showed" type="button" value="按钮1"/>
        <input type="checkbox" ng-model="hided" />
        <input ng-hide="hided" type="button" value="按钮2"/>
    </body>
</html>
```
[]()
