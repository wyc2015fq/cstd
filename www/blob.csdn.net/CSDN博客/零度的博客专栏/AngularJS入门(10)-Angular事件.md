# AngularJS入门(10)-Angular事件 - 零度的博客专栏 - CSDN博客
2016年11月03日 11:08:43[零度anngle](https://me.csdn.net/zmx729618)阅读数：479标签：[angularjs](https://so.csdn.net/so/search/s.do?q=angularjs&t=blog)
个人分类：[AngularJS](https://blog.csdn.net/zmx729618/article/category/6499854)

这一篇博客为大家介绍一下AngularJS为我们提供的事件指令。
# ngBlur
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.blur=function(msg){
                    alert(msg);
                };
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <input ng-blur="blur('button1 blur')" type="button" value="button1" />
    </body>
</html>
```
[]()
# ngChange
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.counter = 0;
                $scope.change = function() {
                    $scope.counter++;
                };
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <input type="checkbox" ng-model="confirmed" ng-change="change()" id="ng-change-example1" />
        <label for="ng-change-example1">Confirmed</label><br />
        <tt>counter = {{counter}}</tt><br/>
    </body>
</html>
```
[]()
# ngClick
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <button ng-click="count = count + 1">Increment</button>
        <span>count: {{count}}</span>
    </body>
</html>
```
[]()
# ngCopy
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.copied = false;
                $scope.value = 'copy me';
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <input ng-copy="copied=true" ng-model="value"> copied: {{copied}}
    </body>
</html>
```
[]()
# ngCut
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.cut = false;
                $scope.value = 'cut me';
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <input ng-cut="cut=true" ng-model="value"> cut: {{cut}}
    </body>
</html>
```
[]()
# ngDblclick
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <button ng-dblclick="count = count + 1">
            Increment (on double click)
        </button> count: {{count}}
    </body>
</html>
```
[]()
# ngKeydown
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <input ng-keydown="count = count + 1">
        key down count: {{count}}
    </body>
</html>
```
[]()
# ngKeypress
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <input ng-keypress="count = count + 1">
        key press count: {{count}}
    </body>
</html>
```
[]()
# ngKeyup
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <p>Typing in the input box below updates the key count</p>
        <input ng-keyup="count = count + 1"> key up count: {{count}}
        <p>Typing in the input box below updates the keycode</p>
        <input ng-keyup="event=$event">
        <p>event keyCode: {{ event.keyCode }}</p>
        <p>event altKey: {{ event.altKey }}</p>
    </body>
</html>
```
[]()
# ngMousedown
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <button ng-mousedown="count = count + 1">
            Increment (on mouse down)
        </button>
        count: {{count}}
    </body>
</html>
```
[]()
# ngMouseenter
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <button ng-mouseenter="count = count + 1">
            Increment (when mouse enters)
        </button>
        count: {{count}}
    </body>
</html>
```
[]()
# ngMouseleave
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <button ng-mouseleave="count = count + 1">
            Increment (when mouse leaves)
        </button>
        count: {{count}}
    </body>
</html>
```
![](http://static.blog.csdn.net/images/save_snippets.png)
# ngMousemove
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <button ng-mousemove="count = count + 1">
            Increment (when mouse moves)
        </button>
        count: {{count}}
    </body>
</html>
```
[]()
# ngMouseover
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <button ng-mouseover="count = count + 1">
            Increment (when mouse is over)
        </button>
        count: {{count}}
    </body>
</html>
```
![](http://static.blog.csdn.net/images/save_snippets.png)
# ngMouseup
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.count = 0;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <button ng-mouseup="count = count + 1">
            Increment (on mouse up)
        </button>
        count: {{count}}
    </body>
</html>
```
[]()
# ngPaste
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title></title>
        <script src="angular.min.js" type="text/javascript"></script>
        <script type="text/javascript">
            angular.module("MyApp", []).controller("MyCtrl", function($scope) {
                $scope.paste = false;
            });
        </script>
    </head>
    <body ng-app="MyApp" ng-controller="MyCtrl">
        <input ng-paste="paste=true" placeholder='paste here'>
        pasted: {{paste}}
    </body>
</html>
```
[]()
[](http://blog.csdn.net/jianggujin/rss/list)
