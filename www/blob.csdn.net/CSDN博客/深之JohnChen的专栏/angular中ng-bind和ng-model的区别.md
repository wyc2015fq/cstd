# angular中ng-bind和ng-model的区别 - 深之JohnChen的专栏 - CSDN博客

2017年03月18日 11:23:42[byxdaz](https://me.csdn.net/byxdaz)阅读数：6085


ng-bind和ng-model的区别

AngularJS的数据绑定有ng-bind和ng-model，一般用于如下：

<input ng-model="object.xxx">

<span ng-bind="object.xxx"></span>

ng-bind是单向绑定，由作用于$scope到view层，且在HTML控件(HTML控件有：input、select、button和textarea)中不可显示。

ng-model是双向绑定，$scope<--->view层。

{{}}用于显示数据。

完整代码

```
<!DOCTYPE html>
<html>
<meta charset="utf-8">
<head>
<style>
div textarea{
  background-color:yellow;
}
</style>
<script src="https://cdn.static.runoob.com/libs/angular.js/1.4.6/angular.min.js"></script>
</head>
<body ng-app="myNoteApp">
<div class="aa" ng-controller="myNoteCtrl">
<h2>我的笔记</h2>
<p>
<textarea ng-model="message" rows="10" cols="20" maxlength="100">
</textarea>
</p>
<p>
<button ng-click="save()">保存</button>
<button ng-click="clear()">清除</button>
</p>
<p>Number of characters left: <span ng-bind="left()"></span></p>
<p>Number of characters left(method2): <span>{{leftvar}}</span></p>
</div>
<script>
var app = angular.module("myNoteApp",[]);
app.controller("myNoteCtrl",function($scope){
	$scope.message = "";
	$scope.leftvar = 0;
	$scope.left = function() {$scope.leftvar = 100 - $scope.message.length;return $scope.leftvar;};
	$scope.clear = function() {$scope.message = "";};
	$scope.save = function() {alert("Note Saved");};
});
</script>
</body>
</html>
```


