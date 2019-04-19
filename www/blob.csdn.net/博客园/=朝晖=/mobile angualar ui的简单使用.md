# mobile angualar ui的简单使用 - =朝晖= - 博客园
# [mobile angualar ui的简单使用](https://www.cnblogs.com/dhcn/p/7120774.html)
最近做一个微信App形式的业务平台,之前从别人的推荐文中知道了mobile angualar ui这个东西，这次纯做mobile Web就试用了一下，之前PCWeb中用过AngularJS，HybridApp中用过ionic，这次做这个mobile web也想过ionic，不过ionic的文档明示，ionic只面向iOS和Android，在浏览器环境上支持有限，最后就用了这个Mobile Angular UI，Mobile Angular UI的基础HTML模板是：
```
<body ng-app="myApp">  
  
  <!-- Sidebars -->  
  <div class="sidebar sidebar-left"><!-- ... --></div>  
  <div class="sidebar sidebar-right"><!-- ... --></div>  
  
  <div class="app">  
    <div class="navbar navbar-app navbar-absolute-top"><!-- Top Navbar --></div>  
    <div class="navbar navbar-app navbar-absolute-bottom"><!-- Bottom Navbar --></div>  
  
    <!-- App body -->  
  
    <div class='app-body'>  
      <div class='app-content'>  
        <ng-view></ng-view>  
      </div>  
    </div>  
  </div><!-- ~ .app -->  
  
  <!-- Modals and Overlays -->  
  <div ui-yield-to="modals"></div>  
  
</body>
```
    这个模板包括了左滑动菜单、右滑动菜单、顶部和底部导航菜单，中间的的子View和下部Modal界面，基本包含MobileWeb的主要设计方式。
    Mobile Angular UI里面的SharedState及其相关的ui-*命令虽然可以和ng的控制器和$scope相互交互，但是除了SharedState包装成[AngularJS](http://lib.csdn.net/base/angularjs)服务，其本质上不是基于[angularjs](http://lib.csdn.net/base/angularjs)的那种控制器模块模式构建的，官方文档中说的很明白:
SharedState acts as a BUS between UI elements to share UI related state that is automatically disposed when all scopes requiring it are destroyed. 
Data structures retaining statuses will stay outside angular scopes thus they are not evaluated against digest cycle until its necessary.
Also although statuses are sort of global variables SharedState will take care of disposing them when no scopes are requiring them anymore. 
Any SharedState method is exposed through Ui object in $rootScope. 
      从文档可见：SharedState保管的变量是全局的，不想$scope中的变量有其控制器作用域，当然也可以通过SharedState在$scope和state 变量之间建立引用关系。不过一般通过Ui对象来对state变量做操作。
    Mobile Angular UI最大的新意也就这个SharedState及其相关的ui-*命令了，不过官网文档认真看一遍，也就got it了，Mobile Angular UI目前提供的UI组件相对来讲还是比较基本的，当然官网也推荐了Topcoat CSS框架作为补充使用，不过Mobile Angular UI目前提供的设施来构建各种交互组件还是很方便的，再就是MobileWeb目前的设计方式都是一个section一个section的简单布局和简单交互，所以Mobile Angular UI还是足够可用了。

