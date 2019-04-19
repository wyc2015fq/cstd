# AngularJS入门(3)-Angular表达式 - 零度的博客专栏 - CSDN博客
2016年11月02日 17:29:35[零度anngle](https://me.csdn.net/zmx729618)阅读数：1213
# AngularJS 表达式
AngularJS 使用 表达式 把数据绑定到 HTML。
- AngularJS 表达式写在双大括号内：`{{ expression }}`。
- AngularJS 表达式把数据绑定到 HTML，这与 ng-bind 指令有异曲同工之妙。
- AngularJS 将在表达式书写的位置”输出”数据。
- AngularJS 表达式 很像 JavaScript 表达式：它们可以包含文字、运算符和变量。
# AngularJS 数字
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
    </head>
    <body ng-app="" ng-init="price=1.5;count=10">
        <div>
            总价（price*count）:{{price * count}}
        </div>
    </body>
</html>
```
[]()
使用`ng-bind`实现相同效果：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
    </head>
    <body ng-app="" ng-init="price=1.5;count=10">
        <div>
            总价（price*count）:<span ng-bind="price * count"></span>
        </div>
    </body>
</html>
```
# AngularJS 字符串
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
    </head>
    <body ng-app="" ng-init="first='jiang';last='gujin'">
        <div>
            {{first + ' ' + last}}
        </div>
    </body>
</html>
```
# AngularJS 对象
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>AngularJS</title>
        <script type="text/javascript" src="js/angular.min.js"></script>
    </head>
    <body ng-app="" ng-init="persion={first:'jiang',last:'gujin'}">
        <div>
            {{persion.first + ' ' + persion.last}}
        </div>
    </body>
</html>
```
[]()
# AngularJS 数组
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
# AngularJS 表达式 与 JavaScript 表达式
- 类似于 JavaScript 表达式，AngularJS 表达式可以包含字母，操作符，变量。
- 与 JavaScript 表达式不同，AngularJS 表达式可以写在 HTML 中。
- 与 JavaScript 表达式不同，AngularJS 表达式不支持条件判断，循环及异常。
- 与 JavaScript 表达式不同，AngularJS 表达式支持过滤器。
