# AngularJS入门(1)-双向数据绑定 - 零度的博客专栏 - CSDN博客
2016年11月02日 15:16:38[零度anngle](https://me.csdn.net/zmx729618)阅读数：361
       AngularJS (Angular.JS) 是一组用来开发Web页面的框架、模板以及数据绑定和丰富UI组件。它支持整个开发进程，提供web应用的架构，无需进行手工DOM操作。 AngularJS很小，只有60K，兼容主流浏览器，与 jQuery 配合良好。双向数据绑定可能是AngularJS最酷最实用的特性,将MVC的原理展现地淋漓尽致.
   AngularJS的工作原理是:HTML模板将会被浏览器解析到DOM中, DOM结构成为AngularJS编译器的输入。AngularJS将会遍历DOM模板, 来生成相应的NG指令,所有的指令都负责针对view(即HTML中的ng-model)来设置数据绑定。因此, NG框架是在DOM加载完成之后, 才开始起作用的. 
    在html中:
```java
<body ng-app="ngApp">
  <div ng-controller="ngCtl">
     <label ng-model="myLabel"></label>
     <input type="text" ng-model="myInput" />
     <button ng-model="myButton" ng-click="btnClicked"></button>
  </div>
</body>
```
    在js中:
```java
// angular app
var app = angular.module("ngApp", [], function(){
  console.log("ng-app : ngApp");
});
// angular controller
app.controller("ngCtl", [ '$scope', function($scope){
  console.log("ng-controller : ngCtl");
  $scope.myLabel = "text for label";
  $scope.myInput = "text for input";
  $scope.btnClicked = function() {
     console.log("Label is " + $scope.myLabel);
  }
}]);
```
    如上,我们在html中先定义一个angular的app,指定一个angular的controller,则该controller会对应于一个作用域(可以用$scope前缀来指定作用域中的属性和方法等). 则在该ngCtl的作用域内的HTML标签, 其值或者操作都可以通过$scope的方式跟js中的属性和方法进行绑定. 
    这样, 就实现了NG的双向数据绑定: 即HTML中呈现的view与AngularJS中的数据是一致的. 修改其一, 则对应的另一端也会相应地发生变化.
    这样的方式,使用起来真的非常方便. 我们仅关心HTML标签的样式, 及其对应在js中angular controller作用域下绑定的属性和方法. 仅此而已, 将众多复杂的DOM操作全都省略掉了.
   这样的思想,其实跟jQuery的DOM查询和操作是完全不一样的, 因此也有很多人建议用AngularJS的时候,不要混合使用jQuery. 当然, 二者各有优劣, 使用哪个就要看自己的选择了.
    NG中的app相当于一个模块module, 在每个app中可以定义多个controller, 每个controller都会有各自的作用域空间,不会相互干扰.
    看下边这段html:
```java
<div ng-app="dataApp">
    单价： <input type="number" min=0 ng-model="price" ng-init="price = 299"><br>
    数量： <input type="number" min=0 ng-model="quantity" ng-init="quantity = 1"><br>
    总价： {{ quantity * price }}
</div>
```
    你会惊喜地发现, 甚至不用写一行的JS代码, 即可完成计算并在界面展示结果. 
    即: 在前端html中使用{{ }}括起来的变量, 是跟AngularJS中对应的controller作用域内的属性绑定在一起的. 实际上,{{}}等同于ng-bind指令, 即ng-bind="myData"就能将NG中的myData数据跟前端对应元素绑定在一起.这样的话, 可以非常方便地做到从NG中获取任意数据并实时展示在页面上了.
   另外, $scope对象还提供了一个$apply方法, 用于进行html页面上的更新, 使用方式为:
```java
$scope.$apply(function(){
  $scope.myValue = "NewValue";
});
```
    $scope对象, 我们可以理解为NG框架中的一个作用域对象, 在该作用域内可以做到数据和视图的相互绑定, 同时又能与其他$scope对象的作用域隔离开来.
    当然, $scope也可以实现继承, 这部分内容在以后接触NG框架中其他对象的时候再分别做记录.
