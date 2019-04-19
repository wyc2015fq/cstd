# 使用AngularJS开发中的几个问题 - =朝晖= - 博客园
# [使用AngularJS开发中的几个问题](https://www.cnblogs.com/dhcn/p/7105573.html)
1、AngularJS的模板绑定机制好像和其$http服务也有一定关系，如果用jQuery Ajax的返回值赋给$scope的作用域变量,整个绑定显示的节奏慢一个事件，神器果然麻烦啊。 
2、对hidden input做绑定好像无效：1.2以后用ng-value可以给hidden input赋值. 
3、AngularJS中对input的ng-model绑定和对Input的value赋值之间存在矛盾。如果绑定了model就无法对value做赋值。 
4、input不做ng-model绑定,那验证输入的机制就会有问题：错误提示显示不出来。观察Scope，你会发现，那段input对应的error值根本不存在。 
5、根据3和4两个条件，对表单数据做初始化最好用ng-model，对$scope的变量做初始化时，只能对一级子属性复制比如$scope.attr1，如果直接对二级属性,$scope.attr1.attr2赋值，因为第一次初始化时可能$scope.attr1这个对象实际还没有创建，会导致代码出错。 
6、关于依赖的注入办法,网上有篇文章作了总结：[http://www.cnblogs.com/lcllao/archive/2012/09/23/2699401.html](http://www.cnblogs.com/lcllao/archive/2012/09/23/2699401.html)。文章给出的依赖注入推荐方法是：
```
var MyController = function(dep1, dep2) {  
     ...  
}  
MyController.$inject = ['dep1', 'dep2'];  
MyController.prototype.aMethod = function() {  
     ...  
}
```
但是遗憾的是这种方法对非Provider的依赖注入是无效的，所以比较保险的方法还是AngularJS一书最开始14页给出的方法：
```
var myAppModule = angular.module('myApp', []);  
myAppModule.controller('TextController',  
   function($scope) {  
      var someText = {};  
      someText.message = 'You have started your journey.';  
      $scope.someText = someText;  
   });
```
这个方法最麻烦的是你得在HTML的ng-app那儿加一个“myApp”此类的属性值，而且这个地方有个问题就是个你得myapp得用JS定义，否则JS运行会出错，我个人建议
```
var myApp = angular.module('myApp',[]);
```
这么一条语句要放在站点个性化定制代码的第一行。
7、ng-repeat的那个数组,直接对数据做delete array[i]处理起不到真正的删除repeat元素的作用，需要对这个数据做一次处理，然后再对scope的数组赋值，才能真正起到更新repeat数组的作用。
8、数据绑定的异步问题，关于[AngularJS](http://lib.csdn.net/base/angularjs)的数据绑定机制简短说明见：http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/dipolar/article/details/9291117，由于这种外部监控的绑定是通过外部监控异步的实现的，如果空间取值跟在模型赋值之后是无效的：如果第一句给某input的绑定model数据做了赋值，紧接着第二句语句就用[jQuery](http://lib.csdn.net/base/jquery).val()这样的函数从DOM树中取input的value得到的往往不是第一句给起绑定的值，因为这个时候，angualrJS的外部绑定监控还没有开始起作用。
9、input的model不能绑定$scope的一级变量，最后做一个空对象，绑定其二级变量，在Select上绑定一级变量会得不到值。
10、由ng-repeat网页端动态生成的option列表，给select的ng-model赋值的界面反馈并不是很正常(1.2版本)，可以用ng-selected="ng-model_value == ng-value"来补充修正这个界面反应bug。
11、过滤器的问题：过滤器的object的表达式不是等于过滤而是字符串包含过滤，所以等于过滤必须用function来做。
12、ng-model传递texteara value值的问题，如果只是简单写在textera元素中间，不给对应ng-model赋值，页面加载完，textera里面什么都没有，因为没给对应的ng-model变量赋值，如果直接把后台参数值传给ng-model，那么texteara中的某些特殊字符会出问题，特别是换行符，会直接导致[js](http://lib.csdn.net/base/javascript)出错，如果后台给值加过滤，那么等传回texteara的时候，还得把这些值转换回来，否则textera中会显示很多过滤转换后的字符----这个问题在国内不少大网站上出现过。保证后端过滤和前端的反过滤彼此匹配也很麻烦，因此，有个多写几句代码的解决方案，还是把服务器的预先值放在texteara元素中间，通过jq的相关函数取得这个值，在控制器函数中把这个值传给ng-model，这样就省下了过滤转换的麻烦。其实input里也有此类问题，比如最典型的单双引号可能和元素属性定义或者JS代码冲突，不过我用Django，它的默认过滤似乎已经解决这个问题。在[Java](http://lib.csdn.net/base/java)领域可以引入ESAPI这个OWASP的强悍安全库，为HTML中多种位置的模板输出提供各种过滤。

