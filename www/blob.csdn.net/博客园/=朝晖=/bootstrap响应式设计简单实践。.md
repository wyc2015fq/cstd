# bootstrap响应式设计简单实践。 - =朝晖= - 博客园
# [bootstrap响应式设计简单实践。](https://www.cnblogs.com/dhcn/p/7124681.html)
      首先需要熟悉Boostrap提供的响应式设施：http://getbootstrap.com/css/#responsive-utilities，BootStrap的响应式设施主要是利用媒体查询对元素的可见性和inline or block显示做变换处理。在mobile屏下面主要做以下几个处理：
      1、原先row内行布局的div变成列布局，我一般喜欢以sm(750px)作为基准，所以这一点的实现很简单，在使用col-xx-number的xx一般使用sm即可。
      2、在mobile界面下某些影响手机页面显示的琐碎内容直接隐藏掉，如果以sm的750px作为基准，那这些元素的class直接加上hidden-xs 即可
      3、某些只需要在mobileWeb下显示的东西需要显示出来，这个需要使用visible-xs-*这个class标签
      4、以前纵向布局的需要横向布局，最明显的莫如左侧导航栏，这个东西可以借鉴bootstrap媒体查询的写法写个xs-inline的标签，也可以visible-xs-*的多个标签组合来达到这个目的。

