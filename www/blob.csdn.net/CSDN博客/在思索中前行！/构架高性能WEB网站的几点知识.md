# 构架高性能WEB网站的几点知识 - 在思索中前行！ - CSDN博客





2014年10月22日 09:49:00[_Tham](https://me.csdn.net/txl16211)阅读数：687标签：[高性能的Web站点																[Html静态页面](https://so.csdn.net/so/search/s.do?q=Html静态页面&t=blog)](https://so.csdn.net/so/search/s.do?q=高性能的Web站点&t=blog)
个人分类：[php开发																[Web前端](https://blog.csdn.net/txl16211/article/category/2377621)](https://blog.csdn.net/txl16211/article/category/2231073)







# HTML静态化

## 什么是html静态化？

> 
说得简单点，就是把所有不是.htm或者.html的页面改为.htm或者.html

1.纯静态页面

当用户访问是，不需要经过服务器解析，直接就可以传送到客户端，此类型的页面，由于不需要解析就能直接访问，一般情况下，比动态页面的执行速度快。

#### 2.静态化

页面静态化就是用动静结合的方式将动态网站生成静态网站来保存。这是实实在在的html文件，也就是静态页面。

#### 3. 伪静态方法（Url静态化）

伪静态不是真正意义上的静态化，它只是利用某种方法将访问的路径也就是url地址转换成为.html为后缀的文件，但是实际上，文件本身还是以原来的后缀保存在服务器上。


## HTML静态化的优点

一、减轻服务器负担。 
二、有利于搜索引擎优化[seo](http://www.kmwzjs.com/site/q-view50.html)，Baidu、Google都会优先收录[静态页面](http://www.kmwzjs.com/site/p-view2.html)，不仅被收录的快还收录的全； 
三、加快页面打开速度，[静态页面](http://www.kmwzjs.com/site/p-view2.html)无需连接数据库打开速度较动态页面有明显提高； 
四、HTML页面不会受程序相关漏洞的影响

## PHP中的html静态化的模版

著名的smarty模版就是一个实现很好的html静态化的模版，smarty的优点就是速度快，高效缓存和html静态化，模版分离

# MVC开发模式




MVC是三个单词的缩写,分别为： 模型(Model),[视图](http://baike.baidu.com/view/71981.htm)(View)和控制Controller)


MVC是一个设计模式，它强制性的使[应用程序](http://baike.baidu.com/view/330120.htm)的输入、处理和输出分开。使用MVC应用程序被分成三个核心部件：模型、视图、控制器。它们各自处理自己的任务。

##### 视图


　　视图是用户看到并与之交互的界面。对老式的Web应用程序来说，视图就是由HTML元素组成的界面，在新式的Web应用程序中，HTML依旧在视图中扮演着重要的角色，但一些新的技术已层出不穷，它们包括Macromedia Flash和象XHTML，XML/XSL，WML等一些标识语言和Web services.


　　如何处理应用程序的界面变得越来越有挑战性。MVC一个大的好处是它能为你的应用程序处理很多不同的视图。在视图中其实没有真正的处理发生，不管这些数据是联机存储的还是一个雇员列表，作为视图来讲，它只是作为一种输出数据并允许用户操纵的方式。

##### 模型


　　模型表示企业数据和业务规则。在MVC的三个部件中，模型拥有最多的处理任务。例如它可能用象EJBs和ColdFusion Components这样的构件[对象](http://baike.baidu.com/view/2387.htm)来处理数据库。被模型返回的数据是中立的，就是说模型与数据格式无关，这样一个模型能为多个视图提供数据。由于应用于模型的代码只需写一次就可以被多个视图重用，所以减少了代码的重复性。

##### 控制器


　　控制器接受用户的输入并调用模型和视图去完成用户的需求。所以当单击Web页面中的超链接和发送[HTML表单](http://baike.baidu.com/view/3488030.htm)时，控制器本身不输出任何东西和做任何处理。它只是接收请求并决定调用哪个模型构件去处理请求，然后再确定用哪个视图来显示返回的数据。

## MVC优点：


低耦合


高度重用性


高效部署，降低开发周期


可维护性


有理软件工程化管理

## PHP中比较好的MVC框架：


Zend framework PHP官方框架


ThinkPHP 国产框架


yii 高效的次轻量级框架





