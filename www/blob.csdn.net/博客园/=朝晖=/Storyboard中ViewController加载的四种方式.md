# Storyboard中ViewController加载的四种方式 - =朝晖= - 博客园
# [Storyboard中ViewController加载的四种方式](https://www.cnblogs.com/dhcn/p/7131097.html)
       这个总结来自于《Programming [iOS](http://lib.csdn.net/base/ios) 10》一书：
1、storyboard的初始化ViewController，通过方法instantiateInitialViewController.
2、通过StoryboardID加载，通过方法：instantiateViewController(withIdentifier:).
3、通过关系：比如TabBarVC、NavigationVC这类的导航关系
3、通过segue的触发。

