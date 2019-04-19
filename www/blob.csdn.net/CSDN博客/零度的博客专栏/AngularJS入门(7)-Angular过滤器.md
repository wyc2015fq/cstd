# AngularJS入门(7)-Angular过滤器 - 零度的博客专栏 - CSDN博客
2016年11月03日 10:00:07[零度anngle](https://me.csdn.net/zmx729618)阅读数：578
过滤器可以使用一个管道字符(|)添加到表达式和指令中。 
AngularJS中提供了如下几种过滤器：
|过滤器|描述|
|----|----|
|filter|从数组项中选择一个子集|
|currency|格式化数字为货币格式|
|number|格式化数字为文本|
|date|格式化日期为指定格式字符串|
|json|允许将js对象转换为JSON字符串|
|lowercase|格式化字符串为小写|
|uppercase|格式化字符串为大写|
|limitTo|从数组或字符串项中选择制定长度的一个子集|
|orderBy|根据某个表达式排列数组|
# filter
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
                $scope.items = [];
                for (var i = 0; i < 20; i++) {
                    $scope.items.push("item" + i);
                }
                $scope.welcom = function() {
                    return "Hello " + $scope.name;
                }
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <input type="text" ng-model="item" />
        <ul>
            <li ng-repeat="it in items|filter:item">{{it}}</li>
        </ul>
    </body>
</html>
```
[]()
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160319133334400)
![这里写图片描述](https://img-blog.csdn.net/20160319133346604)
# currency
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
                $scope.amount = 100.25;
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{amount|currency}}</div>
        <div>{{amount|currency:"￥"}}</div>
        <div>{{amount|currency:"￥":1}}</div>
    </body>
</html>
```
[]()
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160319134015343)
# number
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
                $scope.amount = 100.25;
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{amount|number:1}}</div>
    </body>
</html>
```
![](http://static.blog.csdn.net/images/save_snippets.png)
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160319134338160)
# date
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
                $scope.now = new Date();
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{now|date:'yyyy-MM-dd HH:mm:ss'}}</div>
    </body>
</html>
```
[]()
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160319135107466)
# json
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
                $scope.jsObj = {
                    first: 'jiang',
                    last: 'gujin'
                };
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{jsObj|json}}</div>
    </body>
</html>
```
[]()
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160319135411111)
# lowercase和uppercase
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
                $scope.name = "jIanG guJiN";
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{name|lowercase}}</div>
        <div>{{name|uppercase}}</div>
    </body>
</html>
```
[]()
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160319135650753)
# limitTo
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
                $scope.name = "jiang gujin";
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{name|limitTo:8}}</div>
    </body>
</html>
```
[]()
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160319135827425)
# orderBy
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
                $scope.friends = [{
                    name: 'John',
                    phone: '555-1212',
                    age: 10
                }, {
                    name: 'Mary',
                    phone: '555-9876',
                    age: 19
                }, {
                    name: 'Mike',
                    phone: '555-4321',
                    age: 21
                }, {
                    name: 'Adam',
                    phone: '555-5678',
                    age: 35
                }, {
                    name: 'Julie',
                    phone: '555-8765',
                    age: 29
                }];
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <table class="friend">
          <tr>
            <th><a href="" ng-click="predicate='name'; reverse=false">Name</a> 
               (<a href="" ng-click="predicate='-name'; reverse=false">^</a>)</th>
            <th><a href="" ng-click="predicate='phone'; reverse=!reverse">Phone Number</a></th>
            <th><a href="" ng-click="predicate='age'; reverse=!reverse">Age</a></th>
          </tr>
          <tr ng-repeat="friend in friends | orderBy:predicate:reverse">
            <td>{{friend.name}}</td>
            <td>{{friend.phone}}</td>
            <td>{{friend.age}}</td>
          </tr>
        </table>
    </body>
</html>
```
[]()
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160319140142599)
# 自定义过滤器
下面我们来实现一个字符串反转的过滤器。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
        <script type="text/javascript">
            var app = angular.module("myApp", []);
            app.filter("reverse",
                function() {
                    return function(input, uppercase) {
                        var out = "";
                        for (var i = 0; i < input.length; i++) {
                            out = input.charAt(i) + out;
                        }
                        if (uppercase) {
                            out = out.toUpperCase();
                        }
                        return out;
                    }
                });
            app.controller("myCtrl", function($scope) {
                $scope.name = "jianggujin";
            });
        </script>
    </head>
    <body ng-app="myApp" ng-controller="myCtrl">
        <div>{{name|reverse}}</div>
        <div>{{name|reverse:true}}</div>
    </body>
</html>
```
[]()
运行效果：
![这里写图片描述](https://img-blog.csdn.net/20160319140712643)
