# angular单个页面加载多个ng-app - 深之JohnChen的专栏 - CSDN博客

2017年03月14日 14:17:27[byxdaz](https://me.csdn.net/byxdaz)阅读数：4783


一般angular单个页面只能加载一个ng-app，一个页面会自动加载第一个ng-app，其它ng-app不会被加载。如果想加载多个ng-app就需要使用angular.bootstrap去加载后面的ng-app

相关问题： 

单个angular页面能否有两个ng-app 开源中国社区 

http://stackoverflow.com/questions/18571301/angularjs-multiple-ng-app-within-a-page

手动初始化angular.bootstrap详细说明

angular.bootstrap(element, [modules], [config]);

其中第一个参数element:是绑定ng-app的dom元素；

modules：绑定的模块名字

config：附加的配置

代码：

```
<html>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <script src="http://apps.bdimg.com/libs/angular.js/1.2.16/angular.min.js"></script>
<body id="body">
    <div ng-controller="myCtrl">
        {{ hello }}
    </div>
    <script type="text/javascript">
        var app = angular.module("bootstrapTest",[]);
        app.controller("myCtrl",function($scope){
            $scope.hello = "hello,angular from bootstrap";
        });
        
        // angular.bootstrap(document.getElementById("body"),['bootstrapTest']);
        angular.bootstrap(document,['bootstrapTest']);
    </script>
</body>
</html>
```

值得注意的是：

1、angular.bootstrap只会绑定第一次加载的对象。

2、后面重复的绑定或者其他对象的绑定，都会在控制台输出错误提示。

3、一个ng-app可以有多个controller。

4、.$scope的范围局限于每一个controller中。

5、$rootScope 可作用于整个应用中。是各个 controller中scope 的桥梁。用rootscope定义的值，可以在各个controller 中使用。

完整实例：

```
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<script src="https://cdn.static.runoob.com/libs/angular.js/1.4.6/angular.min.js"></script>
</head>
<body>

<div ng-app="myApp" ng-controller="myCtrl">

<h1>姓氏为 {{lastname}} 家族成员:</h1>

<ul>
    <li ng-repeat="x in names">{{x}} {{lastname}}</li>
</ul>

</div>
	
<div id="myApp2" ng-app="myApp2" ng-controller="myCtrl">
<h1>myApp2 姓氏为 {{lastname}} 家族成员:</h1>
<ul>
    <li ng-repeat="x in names">{{x}}</li>
</ul>
</div>

<div id="myApp3" ng-app="myApp3" ng-controller="myCtrl">
<h1>myApp3 姓氏为 {{lastname}} 家族成员:</h1>
<input type="button" value="rootScope共享" ng-click="say()"></input>
</div>
	
<script>
var app = angular.module('myApp', []);
app.controller('myCtrl', function($scope, $rootScope) {
    $scope.names = ["Emil", "Tobias", "Linus"];
    //$rootScope.lastname = "Refsnes";
});
	
var app2 = angular.module('myApp2', []);
app2.controller('myCtrl', function($scope, $rootScope) {
		$scope.names = ["Emil2", "Tobias2", "Linus2"];
    $rootScope.lastname = "Refsnes--2";
});

var app3 = angular.module('myApp3', []);
app3.controller('myCtrl', function($scope, $rootScope) {
    $rootScope.lastname = "Refsnes--3";
    $rootScope.say = function(){
			$rootScope.lastname = "Refsnes---";
		}
});

//手动加载myApp2 ng-app
angular.bootstrap(document.getElementById("myApp2"), ['myApp2']);
angular.bootstrap(document.getElementById("myApp3"), ['myApp3']);
</script>
</body>
</html>
```

