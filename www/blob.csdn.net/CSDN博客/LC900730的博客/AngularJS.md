# AngularJS - LC900730的博客 - CSDN博客
2017年06月15日 13:52:11[lc900730](https://me.csdn.net/LC900730)阅读数：170标签：[angularjs](https://so.csdn.net/so/search/s.do?q=angularjs&t=blog)
个人分类：[前端学习](https://blog.csdn.net/LC900730/article/category/6974024)
## 基本概念
### Controller
- 不用去视图复用controller，一个控制器只负责一小块视图
- 不要在Controller里面操作DOM
- 不要在Controller里面做数据格式化，ng有很好的表单控件
- 不要在Controller里面做数据过滤，ng有$filter服务
- 一般来说，Controlle是不会相互调用的，控制器之间的交互会通过事件进行
$scope:  是一个POJO  提供了一些工具方法；  是表达式的执行环环境；  是一个树型结构，与DOM标签平行  子$scope对象会继承父$scope上属性和方法  每一个Angular应用只有一个根$scope对象(一般位于ng-app上) 
$scope可以传播事件，类似DOM事件，可以向上也可以向下
## ngRoute
一切都是模块开始的； 
依赖的模块注入是：启动检测； 
ng开头是angular提供的 
directive也是定义在模块上的实例 
ng-bind
