# AngularJs 总结 - 深之JohnChen的专栏 - CSDN博客

2017年04月01日 10:14:29[byxdaz](https://me.csdn.net/byxdaz)阅读数：1077


# AngularJS是什么？

AngularJs（后面就简称ng了）是一个用于设计动态web应用的结构框架。首先，它是一个框架，不是类库，是像EXT一样提供一整套方案用于设计web应用。它不仅仅是一个javascript框架，因为它的核心其实是对HTML标签的增强。何为HTML标签增强？其实就是使你能够用标签完成一部分页面逻辑，具体方式就是通过自定义标签、自定义属性等，这些HTML原生没有的标签/属性在ng中有一个名字：指令（directive）。后面会详细介绍。那么，什么又是动态web应用呢？与传统web系统相区别，web应用能为用户提供丰富的操作，能够随用户操作不断更新视图而不进行url跳转。ng官方也声明它更适用于开发CRUD应用，即数据操作比较多的应用，而非是游戏或图像处理类应用。为了实现这些，ng引入了一些非常棒的特性，包括模板机制、数据绑定、模块、指令、依赖注入、路由。通过数据与模板的绑定，能够让我们摆脱繁琐的DOM操作，而将注意力集中在业务逻辑上。**Angular不推崇DOM操作，也就是说在NG中几乎找不到任何的DOM操作，但Angular本身内部使用dom来改变页面的值。**

# 一、AngularJS简单介绍

AngularJS 重新定义了前端应用的开发方式。面对HTML和JavaScript之间的界线，它非但不畏缩不前，反而正面出击，提出了有效的解决方案。

很多前端应用的开发框架，比如Backbone、EmberJS等，都要求开发者继承此框架特有的一些JavaScript对象。这种方式有其长处，但它不必要地污染了开发者自己代码的对象空间，还要求开发者去了解内存里那些抽象对象。尽管如此我们还是接受了这种方式，因为网络最初的设计无法提供 我们今天所需的交互性，于是我们需要框架，来帮我们填补JavaScript和HTML之间的鸿沟。而且有了它，你不用再“直接”操控DOM，只要给你的DOM注上metadata（即AngularJS里的directive们），然后让AngularJS来帮你操纵DOM。
- AngularJS 1.x 官方网站- [https://angularjs.org/](https://angularjs.org/)

- AngularJS 2.x 官方网站- [https://angular.io/](https://angular.io/)

- Google Material Design for Angular- [https://material.angularjs.org](https://material.angularjs.org/)

- Angular UI（Angular最大的第三方社区）- [http://angular-ui.github.io/](http://angular-ui.github.io/)

- AngularJS中文社区- [http://www.angularjs.cn/](http://www.angularjs.cn/)

- AngularJS中文社区提供的文档（不用FQ）- [http://docs.angularjs.cn/api](http://docs.angularjs.cn/api)
- [http://www.apjs.net/](http://www.apjs.net/)

·        AngularJS在github上的中文粗译版地址： 
- - [https://github.com/basestyle/angularjs-cn](https://github.com/basestyle/angularjs-cn)

·        AngularJS相关资料中文网站： 
- - [http://www.runoob.com/angularjs/angularjs-tutorial.html](http://www.runoob.com/angularjs/angularjs-tutorial.html)
- [http://www.cnblogs.com/xing901022/p/4931455.html](http://www.cnblogs.com/xing901022/p/4931455.html)

# 二、jQuery与AngularJS区别
- jQuery：库- 库一般都是封装了一些常用的方法。 
- 自己手动去调用这些方法，来完成我们的功能。 

                   $('#txt').val('我是小明');

                   $('div').text('我是小王');
- angular：框架- 框架都是提供一种规范或者模式 
- 我们却要按照它提供的这种规则去写代码。 
- 框架会自动帮助我们去执行相应的代码。 

# 三、AngularJS特性

AngularJS是一个新出现的强大客户端技术，提供给大家的一种开发强大应用的方式。这种方式利用并且扩展HTML，CSS和javascript，并且弥补了它们的一些非常明显的不足。本应该使用HTML来实现而现在由它开发的动态一些内容。

AngularJS有五个最重要的功能和特性：

## 3.1 特性一：双向的数据绑定

数据绑定可能是AngularJS最酷最实用的特性。它能够帮助你避免书写大量的初始代码从而节约开发时间。一个典型的web应用可能包含了80%的代码用来处理，查询和监听DOM。数据绑定是的代码更少，你可以专注于你的应用。

我们想象一下Model是你的应用中的简单事实。你的Model是你用来读取或者更新的部分。数据绑定指令提供了你的Model投射到view的方法。这些投射可以无缝的，毫不影响的应用到web应用中。

传统来说，当model变化了。开发人员需要手动处理DOM元素并且将属性反映到这些变化中。这个一个双向的过程。一方面，model变化驱动了DOM中元素变化，另一方面，DOM元素的变化也会影响到Model。这个在用户互动中更加复杂，因为开发人员需要处理和解析

这些互动，然后融合到一个model中，并且更新View。这是一个手动的复杂过程，当一个应用非常庞大的时候，将会是一件非常费劲的事情。

这里肯定有更好的解决方案！那就是AngularJS的双向数据绑定，能够同步DOM和Model等等。

这里有一个非常简单的例子，用来演示一个input输入框和<h1>元素的双向绑定(例01)：

```
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<script src="http://cdn.static.runoob.com/libs/angular.js/1.4.6/angular.min.js"></script>
</head>
<body ng-app="">
<div>
<label>Name:</label>
<input type="text" ng-model="user.name" placeholder="请输入名字">
<hr>
<h1>Hello, {{user.name}}!</h1>
</div>
</body>
</html>
```


## 3.2 特性二：模板

在AngularJS中，一个模板就是一个HTML文件。但是HTML的内容扩展了，包含了很多帮助你映射model到view的内容。

HTML模板将会被浏览器解析到DOM中。DOM然后成为AngularJS编译器的输入。AngularJS将会遍历DOM模板来生成一些指导，即，directive（指令）。所有的指令都负责针对view来设置数据绑定。

我们要理解AuguarJS并不把模板当做String来操作。输入AngularJS的是DOM而非string。数据绑定是DOM变化，不是字符串的连接或者innerHTML变化。使用DOM作为输入，而不是字符串，是AngularJS区别于其它的框架的最大原因。使用DOM允许你扩展指令词汇并且可以创建你自己的指令，甚至开发可重用的组件。

最大的好处是为设计师和开发者创建了一个紧密的工作流。设计师可以像往常一样开发标签，然后开发者拿过来添加上功能，通过数据绑定将会使得这个过程非常简单。

这里有一个例子，我们使用ng-repeat指令来循环图片数组并且加入img模板，如下：

```
function AlbumCtrl($scope) {
    scope.images = [
        {"image":"img/image_01.png", "description":"Image 01 description"},
        {"image":"img/image_02.png", "description":"Image 02 description"},
        {"image":"img/image_03.png", "description":"Image 03 description"},
        {"image":"img/image_04.png", "description":"Image 04 description"},
        {"image":"img/image_05.png", "description":"Image 05 description"}
    ];
}
<div ng-controller="AlbumCtrl">
  <ul>
    <li ng-repeat="image in images">
      <img ng-src="http://m.cnblogs.com/142260/{{image.thumbnail}}"rel="nofollow"/>
    </li>
  </ul>
</div>
```


这里还有一件事值得提一句，AngularJS并不强制你学习一个新的语法或者从你的应用中提出你的模板。

## 3.3 特性三：MVC

针对客户端应用开发AngularJS吸收了传统的MVC基本原则。MVC或者Model-View-Controll设计模式针对不同的人可能意味不同的东西。AngularJS并不执行传统意义上的MVC，更接近于MVVM（Moodel-View-ViewModel)。

Model

model是应用中的简单数据。一般是简单的javascript对象。这里没有必要继承框架的classes，使用proxy对象封装或者使用特别的setter/getter方法来访问。事实上我们处理vanilla javascript的方法就是一个非常好的特性，这种方法使得我们更少使用应用的原型。

ViewModel

viewmodel是一个用来提供特别数据和方法从而维护指定view的对象。

viewmodel是$scope的对象，只存在于AnguarJS的应用中。$scope只是一个简单的js对象，这个对象使用简单的API来侦测和广播状态变化。

Controller

controller负责设置初始状态和参数化$scope方法用以控制行为。需要指出的controller并不保存状态也不和远程服务互动。

View

view是AngularJS解析后渲染和绑定后生成的HTML 。这个部分帮助你创建web应用的[**架构**](http://lib.csdn.net/base/architecture)。$scope拥有一个针对数据的参考，controller定义行为，view处理布局和互动。

## 3.4 特性四：服务和依赖注入

AngularJS服务其作用就是对外提供某个特定的功能。

AngularJS拥有内建的依赖注入（DI）子系统，可以帮助开发人员更容易的开发，理解和[**测试**](http://lib.csdn.net/base/softwaretest)应用。

DI允许你请求你的依赖，而不是自己找寻它们。比如，我们需要一个东西，DI负责找创建并且提供给我们。

为了而得到核心的AngularJS服务，只需要添加一个简单服务作为参数，AngularJS会侦测并且提供给你：

```
function EditCtrl($scope, $location, $routeParams) {
     // Something clever here...
}
你也可以定义自己的服务并且让它们注入：
angular.module('MyServiceModule', []).
    factory('notify', ['$window', function (win) {
    return function (msg) {
        win.alert(msg);
    };
}]);
function myController(scope, notifyService) {
    scope.callNotify = function (msg) {
        notifyService(msg);
    };
}
myController.$inject = ['$scope', 'notify'];
```


## 3.5 特性五：指令（Directives）

指令是我个人最喜欢的特性。你是不是也希望浏览器可以做点儿有意思的事情？那么AngularJS可以做到。

指令可以用来创建自定义的标签。它们可以用来装饰元素或者操作DOM属性。可以作为标签、属性、注释和类名使用。

这里是一个例子，它监听一个事件并且针对的更新它的$scope ，如下：

```
myModule.directive('myComponent', function(mySharedService) {
    return {
        restrict: 'E',
        controller: function($scope, $attrs, mySharedService) {
            $scope.$on('handleBroadcast', function() {
                $scope.message = 'Directive: ' + mySharedService.message;
            });
        },
        replace: true,
        template: '<input>'
    };
});
```


然后，你可以使用这个自定义的directive来使用：

<my-component ng-model="message"></my-component>

使用一系列的组件来创建你自己的应用将会让你更方便的添加，删除和更新功能。

# 四、[使用Angularjs需要注意的一些东西](http://www.cnblogs.com/ziyeyimeng/p/3809806.html)

1）和Jquery的兼容问题，Angularjs自带jqLite，在使用过程中发现，如果项目中使用jquery-1.6.4版本，那么如果将AngularJs引用放在Jquery后面就会报错，但是Jquery-1.8之后没有这个问题。如果出现此问题尽量使用jquery高版本，也可以不使用jqu`ery``，使用angular.element()element来操作dom。`

```
<div ng-controller='myController'id='control'>
        <button ng-click='add()'>添加元素</button>
        <button ng-click='del()'>删除元素</button>
    </div>
    <script type="text/javascript">
        var app=angular.module('myapp',[]);
        app.controller('myController',['$compile','$scope',function($compile,$scope){
            $scope.hello='hello Angular!';
            $scope.log=function(){
                console.log('这是动态添加的方法！');
            };
            var html="<divng-click='log()'>{{hello}}</div>";
            //生成一个Jquery对象
            var template=angular.element(html);
            //对生成的Jquery对象进行编译
            varnewHtml=$compile(template)($scope);
            $scope.add=function(){
               angular.element(document.getElementById('control')).append(newHtml);
            }
            $scope.del=function(){
                if(newHtml){
                    newHtml.remove()
                }
            }
        }]);
    </script>
```

上述代码中，控制器中注入了`$compile`服务，目的是初始化相关的依赖，并对生成的Jquery对template进行编译，以便于调用append方法。

2）和Jquery-ui-dialog结合使用的问题，项目中会经常用到各种模态框，其实个人感觉最好的方式就是用一套采用Angularjs编写的组件/插件，ui
 bootstrap是目前我见过写的最好的。不过这个是理想的状态，奈何项目中一直习惯使用Jquery-ui-dialog，那么在使用的过程中就会遇到jquery
 uidialog 脱离了controller问题。

3）解决`setTimeout`改变属性的无效

在Angular中，大部分操作之后的效果都由$apply方法自动在页面中完成，如果直接调用非Angular中的方法或函数，例如setTimeout方法，那么系统就不会调用$apply方法，导致方法执行失败。
有两种方法可以解决：

在setTimeout方法中，将执行的函数或表达式包含在$apply方法中
setTimeout(function(){ 
*scope*. apply(function(){

$scope.tip=’欢迎来到Angular世界！’;

}) 

},1000)

直接调用与setTimeout方法对应的$timeout服务
$timeout(function(){
$scope.tip=’欢迎来到Angular世界！’;  

},1000)

4）解决双大括号绑定元素时的闪烁问题

在Angular内部，可以向元素中添加`ng-clock`属性来实现元素的隐藏效果：
`<divng-clock>{{message}}</div>`

如果是绑定纯文字的内容，建议使用`ng-bind`的方式，而非双大括号:
`<divng-bind="message"></div>`

`5``）`date:'yyyy-MM-dd HH:mm:ss'格式化 截断

我们在后台对数据进行json序列化时，如果数据中包含有日期，序列化后返回到前端的结果可能是这样的：/Date(1448864369815)/  ，这个时间是基于1970之后的秒时间，可是往往我们要在前台显示正常的日期格式，该如何处理呢？在angularjs（后面简称 ng）中处理起来还是挺方便的，首先我们来看下ng中内置的过滤器：date。ng 过滤器有两种使用方式，分别是： ng表达式  和 控制器中使用代码调用。

1、AngularJs的controller中格式：

 var dateAsString= $filter('date')(item_date, "yyyy-MM-dd hh:mm:ss"); 

注意： controller需要注入$filter

2、 AngularJs的views中格式：

{{item_date| date:'yyyy-MM-dd hh:mm:ss'}} 

[http://www.cnblogs.com/similar/p/5810304.html](http://www.cnblogs.com/similar/p/5810304.html)

6）angularJS中的缓存

一个缓存就是一个组件，它可以透明地储存数据，以便以后可以更快地服务于请求。多次重复地获取资源可能会导致数据重复，消耗时间。因此缓存适用于变化性不大的一些数据，缓存能够服务的请求越多，整体系统性能就能提升越多。

angularJS提供的内置缓存服务是一个很方便的特性，它让我们能够使用同一机制来缓存自定义的内容。

$cacheFactry简介

$cacheFactory是一个为所有angular服务生成缓存对象的服务。在内部，$cacheFactory会创建一个默认的缓存对象，即使我们并没有显示地创建。

要创建一个缓存对象，可以使用$cacheFactory通过一个ID创建一个缓存

var cache = $cacheFactory('myCache');

这里定义了一个ID为“myCache”的缓存。这个$cacheFactory方法可以接受两个参数：

1.cacheId（字符串）：这个cacheId就是创建缓存时的ID名称，即上面的“myCache”。可以通过get()方法使用缓存名称来引用它。

2.options（对象）：这个选项用于指定缓存如何表现。一般情况下，这个选项对象是一个键：capacity（数字），这个容量描述了在任何给定时间要使用缓存存储并保存的缓存键值对的最大数量。

$cacheFactory()方法返回一个缓存数组。

**缓存对象**

缓存对象自身有下列方法可以用来与缓存交互：

对象的使用方式如下（以info示例）：

cache.info()
- info()：info()方法返回缓存对象的ID，尺寸和选项。
- put()：put()方法允许我们把任意JavaScript对象值形式的键（字符串）放进缓存中。比如cache.put(“hello”,”world”)
- get()：get()方法让我们能够访问一个键对应的缓存值。如果找到了这个键，它会返回它的值，如果没哟找到，它会返回undefined。cache.get(“hello”)
- remove()：remove()函数用于在找到一个键值对的情况下从缓存中移除它。如果没有找到，返回undefined。
- removeAll()：removeAll()函数用于重置缓存，同时移除所有已缓存的值。
- destory()：destory()方法用于从$cacheFactory缓存注册表中移除指定缓存的所有引用。

**$http缓存**

angularJS的$http服务创建了一个带有ID为$http的缓存。要让$http请求使用默认的缓存对象很简单：$http方法允许我们给它传递一个cache参数。

也就是说，$http缓存默认是打开的。

当数据不会经常改变时，默认的$http缓存就特别有用了。可以像这样设置它：

$http({ 

method:"GET", 

url:'/api/user', 

cache:true 

})

或者使用辅助方法.get()

$http.get('/api/user',{

cache:true 

})

现在，通过$http到URL
 /api/user的每个请求将会存储到默认的$http缓存中。这个$http缓存中的请求键就是完整的URL路径。

通过在$http选项中传入参数true，可以告诉$http服务使用默认的缓存。如果我们不想经常干扰那些缓存，使用默认缓存时很有用的。

7）IE6，7，8兼容问题处理

[http://www.cnblogs.com/cx709452428/p/6548932.html](http://www.cnblogs.com/cx709452428/p/6548932.html)

8）解决双大括号绑定元素时的闪烁问题

在Angular内部，可以向元素中添加`ng-clock`属性来实现元素的隐藏效果：
`<divng-clock>{{message}}</div>`

如果是绑定纯文字的内容，建议使用`ng-bind`的方式，而非双大括号:
`<divng-bind="message"></div>`

`9``）`使用ng-repeat时的注意事项

9.1注意ng-repeat中的索引号

当需要删除使用ng-repeat指令遍历后生成的某一个DOM元素时，我们经常会调用index索引号来定位需要删除元素的内部元素编号。如果遍历数组的过程中，没有调用过滤器，那么，这种方法是有效的，但一旦添加了过滤器，这个索引号则无效，而必须调用实际的item对象。

例子：

```
<!DOCTYPEhtml>
<htmlng-app="a10_4">
<head>
    <meta charset="utf-8">
    <title>注意ng-repeat中的索引号</title>
    <scriptsrc="http://cdn.static.runoob.com/libs/angular.js/1.4.6/angular.min.js"></script>
    <style type="text/css">
        .frame{
            padding: 5px 8px;
            margin: 0;
            font-size: 12px;
            width: 320px;
            background-color: #eee;
        }
        .frame ul{
            margin: 0;
            padding: 0;
            list-style-type: none;
        }
        .frame ul li:first-child{
            font-weight: bold;
            font-size: 13px;
        }
        .frame ul li{
            height: 28px;
            line-height: 28px;
        }
        .frame ul li span{
            float: left;
            width: 80px;
        }
        .frame ul li span:last-child{
            cursor: pointer;
        }
    </style>
</head>
<body>
 
    <div class="frame">
        <ulng-controller="c10_4">
            <li>
                <span>序号</span>
                <span>姓名</span>
                <span>分数</span>
                <span>操作</span>
            </li>
            <li ng-repeat="item initems | filter:fscore">
               <span>{{item.id}}</span>
               <span>{{item.name}}</span>
               <span>{{item.score}}</span>
                <spanng-click="remove(item,$index)">删除</span>
            </li>
        </ul>
    </div>
 
 
    <script>
        var app = angular.module('a10_4',[])
           .controller('c10_4',function($scope){
                $scope.items = getStu();
                $scope.fscore = function(e){
                    return e.score>60;
                };
                $scope.remove =function(item,index){
                    console.log(item);
                    var item2 =$scope.items[index];
                    console.log(item2);
                };
            });
            function getStu(){
                return [
                    {id: 1010,name:'aaaa',score: 10},
                    {id: 1020,name:'bbbb',score: 50},
                    {id: 1030,name:'cccc',score: 70},
                    {id: 1040,name:'dddd',score: 90},
                    {id: 1050,name:'eeee',score: 60}
                ];
            }
    </script>
 
</body>
</html>
```

9.2使用trackby排序ng-repeat中的数据

在使用ng-repeat指令显示列表数据时，如果需要更新数据，那么页面中原有的DOM元素在更新过程中并不会被重用，而是会被删除，再重新生成与上次结构一样的元素。反复生成DOM元素对页面的加载来说，并不是一件好事，它不仅会延迟数据加载的速度，而且非常浪费页面资源。为了解决这种现象，我们在使用ng-repeat指令更新数据时，需要使用track by对数据源进行排序。

例子：

```
<!DOCTYPEhtml>
<htmlng-app="a10_5">
<head>
    <meta charset="utf-8">
    <title>使用track by排序ng-repeat中的数据</title>
    <scriptsrc="http://cdn.static.runoob.com/libs/angular.js/1.4.6/angular.min.js"></script>
    <style type="text/css">
        .frame{
            padding: 5px 8px;
            margin: 0;
            font-size: 12px;
            width: 320px;
            background-color: #eee;
        }
        .frame ul{
            margin: 0;
            padding: 0;
            list-style-type: none;
        }
        .frame ul li{
            height: 28px;
            line-height: 28px;
        }
        .frame ul li span{
            float: left;
            width: 50px;
        }
    </style>
</head>
<body>
 
    <div ng-controller="c10_5"class="frame">
        <buttonng-click="update()">更新</button>
        <ul ng-repeat="user in userstrack by user.id">
            <li>
               <span>{{user.id}}</span>
               <span>{{user.name}}</span>
            </li>
        </ul>
    </div>
 
 
    <script>
        var app = angular.module('a10_5',[])
            .controller('c10_5',function($scope){
                var users = [
                    {id: 1010,name:'aaaa',score: 10},
                    {id: 1020,name:'bbbb',score: 50},
                    {id: 1030,name:'cccc',score: 70},
                    {id: 1040,name: 'dddd',score:90},
                    {id: 1050,name:'eeee',score: 60}
                ];
 
                $scope.users = users;
                console.log($scope.users);
 
                $scope.update = function(){
                    var result = [
                        {id: 1011,name:'1111',score: 10},
                        {id: 1022,name:'2222',score: 50},
                        {id: 1033,name:'3333',score: 70},
                        {id: 1044,name:'4444',score: 90},
                        {id: 1055,name: '5555',score: 60}
                    ];
                    $scope.users = result;
                    console.log($scope.users);
                }
            });
    </script>
 
</body>
</html>
```

9.3正确理解ng-repeat指令中scope的继承关系

在调用ng-repeat指令显示数据时，ng-repeat在新建DOM元素时，也为每个新建的DOM元素创建了独立的scope作用域。虽然如此，他们的父级scope作用域是相同的，都是构建控制器时注入的$scope对象，调用angular.element(domElement).scope方法可以获取某个DOM元素所对应的作用域，通过某个元素的作用域又可以访问到它的父级作用域，从而修改绑定的数据源。

例子：

```
<!DOCTYPEhtml>
<htmlng-app="a10_6">
<head>
    <meta charset="utf-8">
    <title>正确理解ng-repeat指令中scope的继承关系</title>
    <scriptsrc="http://cdn.static.runoob.com/libs/angular.js/1.4.6/angular.min.js"></script>
    <style type="text/css">
        .frame{
            padding: 5px 8px;
            margin: 0;
            font-size: 12px;
            width: 320px;
            background-color: #eee;
        }
        .frame ul{
            margin: 0;
            padding: 0;
            list-style-type: none;
        }
        .frame ul li{
            height: 28px;
            line-height: 28px;
        }
        .frame ul li span{
            float: left;
            width: 50px;
        }
    </style>
</head>
<body>
 
    <div ng-controller="c10_6"class="frame">
        <input type="button"value="按钮1" ng-click="change1()">
        <input type="button"value="按钮2" ng-click="change2()">
        <input type="button"value="按钮3" ng-click="change3()">
        <ul ng-repeat="user in userstrack by user.id">
            <li>
                <spanid="spn{{user.id}}">{{user.id}}</span>
                <spanid="spn{{user.id}}">{{user.name}}</span>
                <spanid="spn{{user.id}}">{{user.score}}</span>
            </li>
        </ul>
    </div>
 
 
    <script>
        var app = angular.module('a10_6',[])
           .controller('c10_6',function($scope){
                $scope.users = [
                    {id: 1010,name:'aaaa',score: 10},
                    {id: 1020,name:'bbbb',score: 50},
                    {id: 1030,name:'cccc',score: 70},
                    {id: 1040,name:'dddd',score: 90},
                    {id: 1050,name:'eeee',score: 60}
                ];
 
                $scope.change1 = function(){
                    var scope1 =angular.element(document.getElementById("spn1010")).scope();
                    var scope2 =angular.element(document.getElementById("spn1020")).scope();
                    console.log(scope1 ==scope2);
                };
 
                $scope.change2 = function(){
                    var scope =angular.element(document.getElementById("spn1020")).scope();
                    console.log(scope.$parent== $scope);
                };
 
                $scope.change3 = function(){
                    var scope =angular.element(document.getElementById("spn1030")).scope();
                    scope.$parent.users = [
                        {id: 1011,name:'1111',score: 10},
                        {id: 1022,name:'2222',score: 50},
                        {id: 1033,name:'3333',score: 70},
                        {id: 1044,name:'4444',score: 90},
                        {id: 1055,name:'5555',score: 60}
                    ];
                };
            });
    </script>
 
</body>
</html>
```

10）解决单击按钮事件中的冒泡现象

与在Javascript中一样，都可以使用`stopPropagation()`方法，在Angular中：
`$event.stopPropagation()`

例子：

```
<!DOCTYPEhtml>
<htmlng-app="a10_7">
<head>
    <meta charset="utf-8">
    <title>解决单击按钮事件中的冒泡现象</title>
    <scriptsrc="http://cdn.static.runoob.com/libs/angular.js/1.4.6/angular.min.js"></script>
    <style type="text/css">
        .frame{
            padding: 5px 8px;
            margin: 0;
            font-size: 12px;
            width: 320px;
            background-color: #eee;
        }
        .frame div{
            margin: 10px 0;
        }
    </style>
</head>
<body>
 
    <div ng-controller="c10_7 aso" class="frame">
        <div ng-click="o.click('父级',$event)">
            在按钮的单击事件中，阻止冒泡现象
            <br/>
            <input type="checkbox"ng-click="o.change($event)"ng-model="o.stopPropagation">是否阻止冒泡？
            <br/><br/>
            <button type="button"ng-click="o.click('按钮',$event)">点击我</button>
        </div>
    </div>
 
 
    <script>
        var app = angular.module('a10_7',[])
            .controller('c10_7',function($scope){
                var obj = this;
                obj.click =function(name,$event){
                    console.log(name+"被触发");
                    if(obj.stopPropagation){
                       $event.stopPropagation();
                    }
                };
                obj.change = function($event){
                    $event.stopPropagation();
                }
                return obj;
            });
    </script>
 
</body>
</html>
```

11）、释放多余的$watch监测函数

我们知道，在angular中，数据的双向绑定是它非常强大的功能，也是它区别于其他前端框架的特征之一，而这个功能的实现离不开$watch函数。如果在移动端设备中，众多的数据双向数据绑定必然诞生大量的$watch函数执行，这些$watch函数的执行会导致页面数据加载缓慢、元素绑定方法执行效率过低等性能问题，因此，当不需要时，必须及时释放多余的$watch监测函数。

在angular中，当$watch函数被直接调用时，将返回一个释放$watch绑定的unbind函数。因此，根据这个特征，当需要释放某个多余的$watch监测函数时，只需要再次调用这个$watch函数就可以轻松地释放它的监测功能。

例子：

```
<!DOCTYPEhtml>
<htmlng-app="a10_8">
<head>
    <meta charset="utf-8">
    <title>释放多余的$watch监测函数</title>
    <scriptsrc="http://cdn.static.runoob.com/libs/angular.js/1.4.6/angular.min.js"></script>
    <style type="text/css">
        .frame{
            padding: 5px 8px;
            margin: 0;
            font-size: 12px;
            width: 320px;
            background-color: #eee;
        }
        .frame button,.frame div{
            margin: 5px 0;
        }
    </style>
</head>
<body>
 
    <div ng-controller="c10_8"class="frame">
        <input type="text"ng-model="content">
        <div>第 {{num}}次数据变化</div>
        <button ng-click="stopWatch()">停止监测</button>
    </div>
 
 
    <script>
        var app = angular.module('a10_8',[])
           .controller('c10_8',function($scope){
                $scope.num = 0;
                $scope.stopWatch = function(){
                    contentWatch();
                };
                var contentWatch =$scope.$watch('content',function(newVal,oldVal){
                    if(newVal === oldVal){
                        return;
                    }
                    $scope.num++;
                });
            });
    </script>
 
</body>
</html>
```

12）解决ng-if中ng-model值无效的问题

在angular中，ng-if指令的功能与ng-show指令相似，都用于控制元素的显示与隐藏，但两者又有区别，ng-if指令会移除DOM原有的元素，而ng-show指令只是将元素的”display”属性值设置为”none”。因此，在使用时必须根据实际的需要进行选择性使用。

此外，与其他指令一样，ng-if指令也会创建一个子级作用域，因此，如果在ng-if指令中添加了元素，并向元素属性增加ng-model指令，那么ng-model指令对应的作用域属于子级作用域，而并非控制器注入的$scope作用域对象，这点在进行双向数据绑定时，需要引起注意。

例子：

<!DOCTYPEhtml>

<htmlng-app="a10_9">

<head>

    <meta charset="utf-8">

    <title>解决ng-if中ng-model值无效的问题</title>

    <scriptsrc="http://cdn.static.runoob.com/libs/angular.js/1.4.6/angular.min.js"></script>

    <style type="text/css">

        .frame{

            padding: 5px 8px;

            margin: 0;

            font-size: 12px;

            width: 320px;

            background-color: #eee;

        }

        .frame button,.frame div{

            margin: 5px 0;

        }

    </style>

</head>

<body>

    <div ng-controller="c10_9"class="frame">

        <div>

            a的值：{{a}}<br/>

            b的值：{{b}}

        </div>

        <div>

            普通方式：<inputtype="checkbox" ng-model="a">

        </div>

        <div ng-if="!a">

            ngIf方式：<inputtype="checkbox" ng-model="$parent.b">

        </div>

    </div>

    <script>

        var app = angular.module('a10_9',[])

           .controller('c10_9',function($scope){

                $scope.a = false;

                $scope.b = false;

            });

    </script>

</body>

</html>

13）[AngularJS中$http.post问题](http://blog.csdn.net/u013474436/article/details/51851446)

[**AngularJS**](http://lib.csdn.net/base/angularjs)post跟[**jQuery**](http://lib.csdn.net/base/jquery)post完全两回事，会在[**Java**](http://lib.csdn.net/base/java)getparamname函数取不到值。可以发现传参方式是request payload,参数格式是json，而并非用的是form传参，所以在后台用接收form数据的方式接收参数就接收不到了。

POST表单请求提交时，使用的Content-Type是application/x-www-form-urlencoded，而使用原生AJAX的POST请求如果不指定请求头RequestHeader，默认使用的Content-Type是text/plain;charset=UTF-8，而angularjs的http.post默认的Content-Type是：application`/json,``解决方法是指定`Content-Type为表单请求样式。实例代码：

//要通过post传递的参数

vardata = {

    pageindex: 1,

    pagesize: 8,

},

//post请求的地址

url= "/admin/KeyValue/GetListByPage",

//将参数传递的方式改成form

postCfg= {

    headers: { 'Content-Type':'application/x-www-form-urlencoded;charset=utf-8' },

    transformRequest: function (data) {

        return $.param(data);

    }

};

//发送post请求，获取数据

$http.post(url,data, postCfg)

    .success(function (response) {

        alert("hello");

});

14）angular加载多个页面

一般angular单个页面只能加载一个ng-app，一个页面会自动加载第一个ng-app，其它ng-app不会被加载。如果想加载多个ng-app就需要使用angular.bootstrap去手工加载后面的ng-app。

