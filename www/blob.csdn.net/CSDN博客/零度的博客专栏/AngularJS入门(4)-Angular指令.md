# AngularJS入门(4)-Angular指令 - 零度的博客专栏 - CSDN博客
2016年11月02日 17:40:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：432标签：[angularjs](https://so.csdn.net/so/search/s.do?q=angularjs&t=blog)
个人分类：[AngularJS](https://blog.csdn.net/zmx729618/article/category/6499854)
AngularJS 通过被称为 指令 的新属性来扩展 HTML。并且通过内置的指令来为应用添加功能。另外，AngularJS 允许我们自定义指令。 
AngularJS为我们提供了很多指令，在前面我们也见到了一些，如：`ng-app`、’ng-bind’等。AngularJS提供的指令我们可以查阅官网的文档，这里就不一一列举了，下面介绍几个比较常用的指令的用法，其他指令的使用也大同小异。
AngularJS 指令是扩展的 HTML 属性，带有前缀 ng-。
# ng-app 指令
在之前的代码中，我们可以发现每一个AngularJS的代码里都含有`ng-app`指令。`ng-app` 指令定义了 AngularJS 应用程序的 根元素。在网页加载完毕时会自动引导（自动初始化）应用程序。
# ng-init 指令
`ng-init` 指令为 AngularJS 应用程序定义了 初始值。通常情况下，我们不使用 `ng-init`。我们将使用一个控制器或模块来代替它。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
    </head>
    <body ng-app="" ng-init="color=['red', 'blue', 'green']">
        <div>
            {{color[1]}}
        </div>
    </body>
</html>
```
[]()
这是之前的一个示例，使用`ng-init`指令初始化了一个名为color的数组。
# ng-model 指令
`ng-model` 指令 绑定 HTML 元素 到应用程序数据。另外`ng-model` 指令也可以：
- 为应用程序数据提供类型验证（number、email、required）。
- 为应用程序数据提供状态（invalid、dirty、touched、error）。
- 为 HTML 元素提供 CSS 类。
- 绑定 HTML 元素到 HTML 表单。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
    </head>
    <body ng-app="">
        <div>
            <input type="text" ng-model="msg" />
        </div>
        <div>
            {{msg}}
        </div>
    </body>
</html>
```
[]()
# ng-repeat 指令
`ng-repeat` 指令对于集合中（数组中）的每个项会 克隆一次 HTML 元素。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
    </head>
    <body ng-app="" ng-init="color=['red', 'blue', 'green']">
        <div>
            <ul>
                <li ng-repeat="x in color">{{x}}</li>
            </ul>
        </div>
    </body>
</html>
```
[]()
# 自定义的指令
除了 AngularJS 内置的指令外，我们还可以创建自定义指令。 
我们可以使用 .directive 函数来添加自定义的指令。要调用自定义指令，HTMl 元素上需要添加自定义指令名。我们需要使用驼峰法来命名一个指令， 例如：`myDirective`, 但在使用它时需要以
`-` 分割, `my-directive`。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
    </head>
    <body ng-app="myApp">
        <my-directive></my-directive>
        <script>
            var app = angular.module("myApp", []);
            app.directive("myDirective", function() {
                return {
                    template: "<h1>自定义指令!</h1>"
                };
            });
        </script>
    </body>
</html>
```
[]()
我们可以通过元素名、属性、类名、注释来调用指令。
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
    </head>
    <body ng-app="myApp">
        <my-directive></my-directive>
        <div my-directive></div>
        <div class="my-directive"></div>
        <!-- directive: my-directive -->
        <script>
            var app = angular.module("myApp", []);
            app.directive("myDirective", function() {
                return {
                    restrict : "EACM",
                    replace : true,//
                    template: "<h1>自定义指令!</h1>"
                };
            });
        </script>
    </body>
</html>
```
[]()
## 限制使用
我们可以限制我们的指令只能通过特定的方式来调用。通过添加 `restrict` 属性来限制,例如：设置`restrict`值为 “A”, 则指令只能通过属性的方式来调用。
`restrict` 值可以是以下几种:
- E 只限元素名使用
- A 只限属性使用
- C 只限类名使用
- M 只限注释使用
`restrict` 默认值为 EA, 即可以通过元素名和属性名来调用指令。
