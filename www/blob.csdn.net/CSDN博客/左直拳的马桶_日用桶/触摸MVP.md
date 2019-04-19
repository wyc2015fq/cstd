# 触摸MVP - 左直拳的马桶_日用桶 - CSDN博客
2007年04月30日 15:36:00[左直拳](https://me.csdn.net/leftfist)阅读数：1748标签：[textbox																[mvc																[设计模式																[string																[asp																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=asp&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)](https://so.csdn.net/so/search/s.do?q=mvc&t=blog)](https://so.csdn.net/so/search/s.do?q=textbox&t=blog)
个人分类：[系统架构																[.NET](https://blog.csdn.net/leftfist/article/category/94497)](https://blog.csdn.net/leftfist/article/category/2567173)
触摸MVP
 左直拳
我忽然关心起设计模式来了。
听说MVC是主流，现在又有个MVP。惭愧，我两样都不懂。落后啊落后，简直屎一堆。
不过这两天还是尝试着用了一下MVP。
所谓的MVP，是Model View Presenter 的缩写。Model，模型，……可能是……处理数据对象的实例……真好……；View，视图，界面，UI层吧；Presenter，表示器。
这个P，表示器，是专门为视图而设计的。
既然有了视图，为什么还要搞个表示器？
在ASP。NET里面，页面跟代码分离，真是一大进步。可是这个代码，其实与页面还是结合得相当紧密的。比方说，这个页面的TextBox该怎么赋值，ListBox的选项变了又如何，代码不是要一一控制吗？因此，这个代码文件往往要面面俱到，写一大堆，复杂无比。很可能结果是，这个代码文件既包含了视图，也包含了很多逻辑处理的细节，也就是MVC模式里的C，Controller（控制器）的一部分。
可是为什么不将这些东西都挪到控制器里面去呢？也许是页面与代码结合得太紧了，很难抽象出来。总不能，在控制器里直接操作页面的控件吧。
这样子有时候就觉得不太方便，页面代码太多东西了，一看头都大，想改也不好改。重用的话，可能拷贝、粘贴、修改是比较实惠的做法。
叉月革命一声炮响，微软给我们送来了MVP。
依我的理解，Presenter（表示器）就是为了解决上述问题而诞生的。逻辑控制，大部分都放在上面，由它推送数据，视图只单纯的负责接收数据、显示数据、提交数据。
具体做法是
1、定义一个接口，里面规定了一些属性
2、视图，即页面代码文件除了继承象WebPage/WinForm，还继承这个接口
3、视图实现这个接口，将属性落实到自己的控件上。比如，一个String型的属性，可能会对应一个TextBox的Text。
4、在Presenter里，声明一个此接口类型的私有对象。并在构造函数里接收或实例化此对象。（与此对应，视图里会创建一个Presenter对象，参数就是视图对象自己，this。）
5、现在，Presenter里控制这个私有对象的属性，也就等于是控制了视图上的控件。。。
代码分离了。视图清净了。
如果您想做一个差不多的视图，那么只须继承同一个接口就行了，或者是继承另一个差不多的接口，而表示器还是同一个，可能只须稍做扩展。
这是我对MVP一点粗浅的理解。
大江东去，浪淘尽，
千古风流人物。
故垒西边，人道是，
三国周郎赤壁。
乱石穿空，惊涛拍岸，
卷起千堆屎。
