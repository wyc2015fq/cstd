# "Uncaught object angular.js:36"诡异错误 - =朝晖= - 博客园
# ["Uncaught object angular.js:36"诡异错误](https://www.cnblogs.com/dhcn/p/7115361.html)
        这个错误的调用顶级是[jQuery](http://lib.csdn.net/base/jquery).ready()函数，这个错误的原因是如果你在html元素里面定义ng-app,则在[JavaScript](http://lib.csdn.net/base/javascript)里面必须初始化这个ngapp，初始化语句是：
```
var AppName = angular.module('AppName',[]);
```

