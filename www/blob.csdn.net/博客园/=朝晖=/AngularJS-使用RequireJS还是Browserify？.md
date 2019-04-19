# AngularJS - 使用RequireJS还是Browserify？ - =朝晖= - 博客园
# [AngularJS - 使用RequireJS还是Browserify？](https://www.cnblogs.com/dhcn/p/7120764.html)
http://www.html-[js](http://lib.csdn.net/base/javascript).com/article/2126
# AngularJS - 使用RequireJS还是Browserify？
[AngularJS](http://lib.csdn.net/base/angularjs)之所以吸引了很多开发者的关注，很大一部分原因是因为它为你如何[架构](http://lib.csdn.net/base/architecture)你的应用提供了一种选择。通常来说，很多的选择并不是什么好事，因为觉得多数开发者并不希望你把一个“正确的”应用架构模式强加给他们。
就[JavaScript](http://lib.csdn.net/base/javascript)来说，存在的一种情况是有相当数量的一部分人都等待着有人 – 随便是谁都好 – 为他们提供一种关于由大型或者人员变化频繁的团队创建的企业级别应用的扩展和维护。总而言之，我们不仅仅需要一个基础概念，我们需要的是一个构建计划。
![](http://developer.telerik.com/wp-content/uploads/2014/07/blueprints.jpg)
# AngularJS应用的构建蓝图
[angularjs](http://lib.csdn.net/base/angularjs)提供的蓝图非常的基础而且简单 – [javascript](http://lib.csdn.net/base/javascript)没有一个模块系统，因此AngularJS为你提供了一个。AngularJS确保了当你的应用在运行时，所有的代码已经加载完毕，并且可用。AngularJS主要通过依赖注入完成这项工作。
现在假设我们有一个非常简单的应用。其中只有一个子视图。它有一个对应的控制器。这个控制器反过来又为这个子视图注入一个服务来提供数据接入。当应用运行起来时，AngularJS能够确保所有的“字符串”代表的都是需要注入的实际模型。
代码如下所示：
```java
// 使用Angular Kendo UI来编写UI组件和进行数据层抽象
(function () {
  var app = angular.module('app', ['ngRoute']);
  // routeProvider在这里被注入(需要require Angular.Route)
  app.config(['$routeProvider', function ($routeProvider) {
    $routeProvider.when('/home',
    {
      templateUrl: 'partials/home.html',
      controller: 'HomeController'
    })
    .otherwise(
    {
      redirectTo: '/home'
    })
  }]);
  app.controller('HomeController', ['$scope', 'productsDataSource', function($scope, $productsDataSource) {
    $scope.title = 'Home';
    $scope.productsDataSource = $productsDataSource;
    $scope.listViewTemplate = '<p>{{ ShipCity }}</p>';
  }]);
  app.factory('productsDataSource', function () {
    new kendo.data.DataSource({
      type: 'odata',
      transport: {
        read: 'http://demos.telerik.com/kendo-ui/service/Northwind.svc/Orders'
      },
      pageSize: 20,
      serverPaging: true
    });
  });
}());
```
上面的代码做了不少事情：
- 声明了一个应用模块；
- 创建了一个工厂服务用来返回一个Kendo UI DataSource；
- 针对子视图将DataSource注入HomeController；
- 定义路由将子视图和控制器匹配起来
在AngularJS中，一个非常棒的设计就是**它并不关心上面这些事情声明的顺序。**
只要第一个app模块存在，你就可以以任何顺序创建任何后续的工厂服务，控制器，路由等等东西。AngularJS在后续会帮助你检查并载入依赖项，即便你在一个依赖模块声明之前就指明了依赖项。如果你写过一段时间的JavaScript，你一定能够明白它为你解决了一个多么大的问题。
# 应用代码架构 VS 物理项目架构
目前来看的话我们的代码整洁度还可以令人接受。然后，上面这个应用的代码太罗嗦了，而且它基本上来说什么也没做。你能够想象一个真实世界中的应用代码看起来是什么样子吗？
下一个逻辑步骤将会把所有的控制器、服务、以及所有能够分开的东西分隔到不同的文件中。这是一种物理项目架构，它可以让每个文件中的代码整洁一下。摆在我们面前的选择有两个 – Browserify 和 RequireJS。
## 使用Browserify
“app”对象是Angular应用正常运行的关键。在一般的使用中，Angular会假设在应用“启动”时文档已经加载完毕。根据AngularJS的文档，Angular会在DOMContentLoaded事件发生时完成“自动初始化”。
换句话说：在document.readState被设置为complete时，angular.js脚本会被调用。在实际运行中，当DOM已经加载完成之后，Angular都会进行的步骤如下：
- 加载有ng-app指定的模块
- 创建一个应用注入器 - 它能够根据字符串的值将对象注入到相应模块中
- 编译ng-app属性所在标签下面的DOM树
以上的步骤就是一般情况下Angular会做的事情。只要所有的脚本在DOMContentLoaded事件之前加载完毕(可以将它看作document.ready)，一切都能正常运行。这使得Browserify可以很轻易的将Angular应用分成不同的物理文件。
针对上面的例子，我们可以讲文件分隔为下面的结构：
```java
app
    partials
        home.html
    controllers
        homeController.js
    services
    productsDataSource.js
app.js
```
Browserify允许在浏览器中使用CommonJS模块。这意味着每一个模块需要将它自己通过exports暴露给外界，以便其他模块能够require它。
homeController.js文件如下所示：
```java
// controllers/homeController.js
module.exports = function() {
  return function ($scope, $productsDataSource) {
    $scope.title = 'Home';
    $scope.productsDataSource = $productsDataSource;
   $scope.listViewTemplate = '<p>#: ShipCity #</p>';
  };
};
```
productsDataSource.js工厂服务的代码也类似：
// services/productsDataSource.js
module.exports = function () { // the productsDataSource 服务被注入到控制器中 return new kendo.data.DataSource({ type: 'odata', transport: { read: '[http://demos.telerik.com/kendo-ui/service/Northwind.svc/Orders](http://demos.telerik.com/kendo-ui/service/Northwind.svc/Orders)' }, pageSize: 20, serverPaging: true }); };
而app.js文件就是所有魔法发生的地方：
```java
// app.js
// require 所有的核心库
require('../vendor/jquery/jquery.min');
require('../vendor/angular/angular.min');
require('../vendor/angular-route/angular-route.min');
require('../vendor/kendo-ui-core/js/kendo.ui.core.min');
require('../vendor/angular-kendo/angular-kendo');
// 拉出我们所有我们需要的模块（控制器，服务，等等）
var homeController = require('./controllers/homeController');
var productsDataSource = require('./services/productsDataSource');
// 创建模块
var app = angular.module('app', [ 'ngRoute', 'kendo.directives' ]);
// 配置路由
app.config(['$routeProvider', function($routeProvider) {
  $routeProvider
    .when('/home',
    {
      templateUrl: 'partials/home.html',
      controller: 'HomeController'
    })
    .otherwise(
    {
      redirectTo: '/home'
    });
}]);
// 创建工厂服务
app.factory('productsDataSource', productsDataSource);
// 创建控制器
app.controller('HomeController', ['$scope', 'productsDataSource', homeController]);
```
接着，使用下面的命令行即可：
```java
$> watchify js/app/**/*.js -o build/main.js
```
> 
Watchify是一个小巧的功能插件，它能够监视文件夹并”browserify”你所有的代码。
对于Browserify来说，你可以在app.js文件中直接require所有的库文件。除此之外，Browserify还能正确处理好你require这些文件的顺序。多么不可思议的事情！
但是另一个事实是你依然需要手动的创建控制器、工厂服务以及那些不在app.js文件中的东西。我需要在某个模块中定义这些东西然后将它们在app.js中进行合并。总的来说，我所有的Angular代码事实上都位于app.js文件中，同时所有的文件都仅仅只是JavaScript。当然，这是JavaScript，所以没有什么可抱怨的。
但是总的来说，Angular和Browserify协同工作的很棒。
接下来，我们要讨论一件不那么好的事情：在AngularJS中使用RequireJS。
![](http://developer.telerik.com/wp-content/uploads/2014/07/angrycat.jpg)
## 使用RequireJS
RequireJS是一个好东西，但是如果和AngularJS一起使用，就会出现问题。核心问题在于Angular需要在DOM完全加载之后开始运行，它并不想要玩异步游戏。由于RequireJS中一切都是异步的(AMD即异步模块定义)，你很难将二者很好的结合起来。
由于脚本载入是异步的，所有的ng-app属性现在完全不可用了。你不可以使用它来指明Angular应用。
另一个讨厌的事情是app模块。为了传递它你必须使用疯狂的环形依赖。
下面我们来使用RequireJS进行文件架构：
app partials home.html controllers index.js module.js homeController.js services index.js modules.js productsDataSource.js app.js main.js routes.js
我们先从main.js这个文件开始，它在其中配置了所有的库文件信息，并且对于不遵循AMD标准的模块进行了shim设置。该文件的目的是确保所有的js文件都能通过正确的路径载入。
```java
require.config({
  paths: {
    'jquery': 'vendor/jquery/jquery',
    'angular': 'vendor/angular/angular',
    'kendo': 'vendor/kendo/kendo',
    'angular-kendo': 'vendor/angular-kendo',
    'app': 'app'
  },
  shim: {
    // 确保kendo在angular-kendo之前载入
    'angular-kendo': ['kendo'],
    // make sure that 
    'app': {
        deps: ['jquery', 'angular', 'kendo', 'angular-kendo']
    }
  }
});
define(['routes'], function () {
  // 使用bootstrap方法启动Angular应用
  angular.bootstrap(document, ['app']);
});
```
注意到在这里我们需要来手动启动Angular应用。main.js这个文件完成的事情简单来说就是：载入所有文件，然后在document上运行Angular并将ng-app属性设置为’app’。这些文件因为是由RequireJS异步载入，因此我们需要来“手动启动”Angular应用。
在angular.bootstrap方法运行之前，所有的文件已经载入完毕了。这些依赖工作由RequireJS进行解析。注意到上面的代码中define函数对route.js文件发出了请求。RequireJS接着就会在执行angular.bootstrap方法之前载入该文件。
```java
// routes.js
define([
  './app'
], function (app) {
  // app是Angular应用对象
  return app.config(['$routeProvider', function ($routeProvider) {
    $routeProvider
      .when('/home',
        {
          templateUrl: '/app/partials/home.html',
          controller: 'homeController'
        })
      .otherwise(
        {
          redirectTo: '/home'
        });
  }]);
});
```
route.js文件声明app.js为依赖项。app.js文件创建了一个Angular应用对象，并且将它暴露给外部以便于路由可以获取它。
```java
// app.js
define([
  './controllers/index',
  './services/index'
], function (controllers, index) {
  // 返回真正的Angular应用对象，在声明时指明了依赖的项目
  return angular.module('app', [
    'ngRoute',
    'kendo.directives',
    'app.controllers',
    'app.services'
  ]);
});
```
app.js文件创建了模型并且注入了所有所需要的依赖项。其中包含ngRoute服务，Angular Kendo UI 指定以及其他两个模块，这两个模块都在文件的顶部定义。我们首先来看看”controllers/index.js”文件。
```java
// controllers/index.js
define([
  './homeController'
], function () {
});
```
除了载入依赖项以外，上面的这段代码没有做别的事。到目前为止，我们只有一个控制器，但是随着应用的逐渐变大，我们会有越来越多的控制器。所有的控制器都将在这个文件中被加载。每一个控制器的代码又包含在一个单独的文件中。
```java
// controllers/homeController.js
define([
  './module'
], function (module) {
  module.controller('homeController', ['$scope', '$productsDataSource',
    function ($scope, $productsDataSource) {
      $scope.title = 'Home';
      $scope.productsDataSource = $productsDataSource;
      $scope.listViewTemplate = '<p>#: ShipCity #</p>';
    };
  );
});
```
这段代码和之前HomeController的代码很相似，但是它在运行之前还需要一个module.js文件。它的作用在于创建app.controller模块以便于我们能在任何的控制器文件中使用它。
```java
// controllers/module.js
define([
], function () {
  return angular.module('app.controllers', []);
});
```
我们现在来回顾一下从一开始到现在究竟发生了些什么：
```java
“main.js” requires “routes.js”
    “routes.js” requires “app.js”
        “app.js” requires “controllers/index.js”
            “controllers/index.js” requires 所有的控制器
                所有的控制器 require “module.js”
                    “module.js” 创建了 “app.controllers” 模块
```
这有点像一颗过于庞大的依赖树，但是它的可扩展性确实很好。如果你想添加一个新的控制器，你只需要添加”controllers/nameController.js”文件，并在”controllers/index.js”文件中添加相同的依赖项即可。
服务的运作方式和控制器类似。app.js会require services/index.js文件，它require了所有的服务。所有的服务同时会require services/module.js文件，它能够简单的创建并提供app.services模块。
现在回到app.js文件，所有的项目都在其中被加载并传递给我们创建的Angular应用模块。最后一件发生的事情是main.js文件中所发生的angular.bootstrap。简单来说，我们第一眼看到的代码其实在最后才会执行。
这实在是有点难以理解。
RequireJS会在应用运行之前加载所有的代码。这意味着我们并没有实现代码的延迟加载。
# 最终的选择？
RequireJS在绝大多数的项目中确实非常好用，但是在Angular应用中，Browserify是一个更好的选择。
当然，RequireJS和Browserify并不是仅有的选择，如果你有兴趣的话，可以去研究一下WebPack，它不经能使用AMD和CommonJS，同时也能在服务器端和浏览器端同时使用。它甚至能够处理一些预处理器例如LESS、CoffeeScript，Jade等等。
希望本文能够帮助你创建出更加性感并健壮的AngularJS应用。
本文参考自Requiring vs Browserifying Angular，原文地址[http://developer.telerik.com/featured/requiring-vs-browerifying-angular/](http://developer.telerik.com/featured/requiring-vs-browerifying-angular/)

