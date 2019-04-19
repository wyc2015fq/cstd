# asp.net mvc的代码重用 - 左直拳的马桶_日用桶 - CSDN博客
2018年10月20日 13:18:02[左直拳](https://me.csdn.net/leftfist)阅读数：296
软件复用是一个巨大的主题。软件架构设计，很大的一个目的，其实也是为了复用。
软件复用，有三方面可以进行：
> 
需求复用
架构复用
代码复用
前面两个太宏大了，现在只来总结下第三个，[asp.net](http://asp.net) mvc的代码复用。
[asp.net](http://asp.net) mvc的代码复用，我认为有以下几个方面可以进行：
**1、框架**
包括数据库ORM框架，前端JS框架，整套项目框架，等等，不赘述
**2、公用类库、公共函数**
不赘述
**3、部件**
[Html.Partial](https://blog.csdn.net/leftfist/article/details/60589658)
**4、子视图**
[Html.RenderAction](https://blog.csdn.net/leftfist/article/details/77097789)
一张页面，可以划分为若干块子视图，每块子视图都有独立的action，输出的浏览器的时候，还是一张页面，区别于html的iframe。这样做的好处是：
1）主页面代码量大为减少，脉络清晰明了
2）每块子视图都有独立的action，逻辑清晰，利于维护
3）利于多人同时开发
4）利于重用
值得一提的是，不仅仅是一小块页面区域值得制成子视图，一个下拉框等单独元素，也可以做成子视图。非常利于重用。
与此相关，多人开发的情况下，可以将类声明为分部类，比如控制器、业务逻辑操作类等等。分部类的涵义为还是同一个类，但物理文件可以分开：
```
public partial class TaskController : Controller
```
虽然现在有各种代码管理工具，可以合并代码，但物理文件分开，能够将冲突降到最低，并且因为每个文件的代码量少了，而且由于一般都是相关的action才放在一起，代码的相关性也非常强，逻辑清晰、易于阅读和维护：
![在这里插入图片描述](https://img-blog.csdn.net/20181020131714733?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**5、复用控制器**
控制器也是可以复用的。控制器的实质，其实也是类，可以编译成DLL，然后各个应用系统引用这个DLL，只保留视图即可。一则视图没法编译成DLL，二则这里面有个思想：视图负责展示，而控制器负责提供数据和一些简单的逻辑。数据一样，但展示方式却可以多种多样，每个应用都可以根据自己的情况来修改视图，比如应用不同的样式，不同的布局，等等，既有重用，又有个性化，两全其美。
比如说，我们的框架，就将后台的用户管理，角色管理之类加以复用：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181029091017980.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_27,color_FFFFFF,t_70)
控制器在哪？控制器编译成了一个DLL。
