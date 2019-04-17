# 六、总结php框架的功能 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:16:39[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：387
个人分类：[做一个全栈工程师](https://blog.csdn.net/jiangjingxuan/article/category/6694850)












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_104_default_big.png)



可以成为一个产品的网站的业务逻辑都有一定的复杂性，哪怕一个小小的博客网站也会有用户管理、新闻发布、分类管理和多媒体资源管理等，但是有没有办法像堆积木一样实现想要的功能呢？有！开源框架及其海量组件可以帮你实现梦想，本节主要是php框架的介绍。

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 什么是php框架？



php框架就是把php开发过程中通用而繁琐的部分抽象出来，约定出一些固定的套路来供你配置，这样协作开发的人也更容易相互理解。



所有php框架一般都包含以下内容：model、视图（view）、控制器（controller）、路由（router）、工具集、扩展组件。



**model：**就是数据库表的类表达，这个class对应着数据库表结构，比如数据库表里有一列叫做name，那么这个类就有一个成员变量name，当然model可以附加getters和setters以及其他自己实现的方便操作的成员函数。框架抽象出model的目的是让我们像对待内存中的变量一样对待数据库表，这样我们就不用关注数据库了



**视图：**就是展现给用户的部分，通常是网页（html），比如要把某个model的某个成员值展现出来，那么就传给视图来通过html显示出来。当然样式也是在视图里面实现的。为了开发的方便性，视图一般结合着模板使用



**模板：**模板包含三部分内容，一个是视图的继承，一个是传递变量，一个执行逻辑。视图继承就是我们可以写一个base模板，定义好通用的页面布局大框架，并把每个可定制的部分预留出接口来给子类视图实现。传递变量的功能实现了从php到html的数据传递，比如在html里用<h1>{{ name }}</h1>来展示php里的name这个变量的值。执行逻辑就是定义了一些分支、循环等逻辑来动态输出html



**控制器：**控制器是用来实现业务逻辑的部分，控制器可以理解为事件触发时的执行逻辑，比如当你访问某个url（如: [http://www.shareditor.com/bloglist/2](http://www.shareditor.com/blogshow/9)）可以触发bloglist对应的控制器执行“列出博客列表”的逻辑，那么这个控制会怎么做呢？可能是这样的：它获取blog这个model里的所有数据，并把数据全部传给视图，而视图通过循环逻辑取出每个blog的name展示出来。



**路由：**就是定义了什么pattern的url对应执行哪个控制器逻辑，其实就是一个映射关系。除此之外还有变量传递、路由通配等功能。



**工具集：**除了上面有固定套路的部分之外就是一堆工具集了，这部分不同框架也不太一样，所以用着再说



**扩展组件：**一般框架都会配有一些扩展组件，当然强大的网友会帮你开发各种各样的组件，git上只有你想不到的，没有你找不到的。



## 都有哪些流行的php框架？



国外比较流行的php框架有laravel、symfony2、yii2等，国内比较流行的有phalcon、thinkphp、zend framework等。其实所有的php框架不外乎上面讲的几大部分，真正的业务逻辑都是得我们自己来实现。我个人用symfony2有一段时间了，相对比较熟悉，所以后续章节都以symfony2来带大家领略php的美妙。当然这里我也给symfony2打个广告，我当前发文章的博客网站就是用symfony2框架搭建的，功能包括：用户管理、新闻发布、分类管理和多媒体资源管理，自己开发的代码行数不到800行，其余全是用symfony2各种组件修改配置搭起来的，是不是amazing？

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 展示一下我的800行代码的网站效果

### 首页效果

![](http://www.shareditor.com/uploads/media/my-context/0001/01/2bc8c3b5c4795bb758e0e9e55e727335133da097.png)

### 文章列表页

![](http://www.shareditor.com/uploads/media/my-context/0001/01/3b4b7cc5e35ac16a84e21879e60f4be666aeeeb6.png)

### 文章详情页

![](http://www.shareditor.com/uploads/media/my-context/0001/01/2da74136850bfced04e2d19001ac957a860a5e72.png)

### 底部评论（多说）

![](http://www.shareditor.com/uploads/media/my-context/0001/01/da8280d87639690d9b42f31c259e924d57f122c8.png)

### 后台管理首页

![](http://www.shareditor.com/uploads/media/my-context/0001/01/314505917a440fc45e63bf107f7248a633a6036a.png)

### 后台管理类别管理

![](http://www.shareditor.com/uploads/media/my-context/0001/01/e7ee1b717cab698dab47afe050b05b777331b42c.png)

### 文章编辑

![](http://www.shareditor.com/uploads/media/my-context/0001/01/ab09122c7403d40506029726d389fdc6aa6f3942.png)

### 多媒体资源管理

![](http://www.shareditor.com/uploads/media/my-context/0001/01/08148325c9a4c5619f31024a26ac82cc5c1d8c10.png)

### 用户管理

![](http://www.shareditor.com/uploads/media/my-context/0001/01/531efb29c426f73888a85c8042620c3ce574cb6f.png)

### 图片详情

![](http://www.shareditor.com/uploads/media/my-context/0001/01/8925c078449910929a96f4e3f6d1a1a99d6cc898.png)



