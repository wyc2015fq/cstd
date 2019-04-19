# Storyboard的几点缺憾 - =朝晖= - 博客园
# [Storyboard的几点缺憾](https://www.cnblogs.com/dhcn/p/7131118.html)
        Storyboard作为[iOS](http://lib.csdn.net/base/ios)主推的UI开发方式，不管接受也好，不接受也好，在未来几年，都会逐渐在产业界流行，之前bignerd在其[ios](http://lib.csdn.net/base/ios)开发第四版中，作者曾经说过一节的Storyboard优缺点分析，并被国内很多人所免费借用，时至今日，到第六版的时候，storyboard已经是bignerd已经国外资料的UI开发默认方式。我在iOS开发的初期也曾犹豫过，到底采用哪种开发方式，但是在看过一些老外资料后，只好选择今朝趋势storyboard。在开发过程中，感觉加快了界面开发过程，当然也有一些缺憾，列举如下：
       1、UI设计不易模块化复用，storyboard的基本思路是以storyboard组织整个App的界面交互，程序的组织以Storyboard为主，以代码为辅。于是乎，每一个界面交互位置在InterfaceBuilder中，最好有一个ViewController。不过所幸多个界面相同或类似的VC可以共用一个ViewController类代码，我觉得这种设计体现了Apple的基因，就是产品经理主导，很多人说Storyboard比较易于做原型，做原型这事一般是产品经理干的事。

