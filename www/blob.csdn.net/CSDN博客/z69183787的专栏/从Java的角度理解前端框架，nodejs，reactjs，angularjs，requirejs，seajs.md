# 从Java的角度理解前端框架，nodejs，reactjs，angularjs，requirejs，seajs - z69183787的专栏 - CSDN博客
2017年03月03日 14:57:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1131
转：http://blog.csdn.net/uikoo9/article/details/45999745
# 【前端神秘的面纱】
对后端开发来说，前端是神秘的，
眼花缭乱的技术，繁多的框架，
如果你还停留在前端等于只用[jQuery](http://lib.csdn.net/base/jquery)做开发，那么你out了，
本文从[Java](http://lib.csdn.net/base/javase)的角度简述下目前前端流行的一些框架。
水平有限，欢迎指正。
# 【nodejs】
#### 官网：
[https://nodejs.org/](https://nodejs.org/)
#### 简介：
对前端来说极其重要的一个“框架”，简直可以说是开天辟地
#### 类比Java中：JVM
#### 详述：
就前端来说nodejs具有划时代的意义，
做前端的没用过nodejs都不好意思说自己是前端，
做后端的没听过nodejs，
或者说不出nodejs和java的优缺点，也不是一个合格的后端。
nodejs不是一个js框架，千万不要认为是类似jquery的框架，
nodejs是js运行时，运行环境，类比java中jvm，
java的开端是什么，无疑是jvm，自从有了jvm，java才能吹牛说自己是“一次编写处处运行”，
不管你是windows还是[Linux](http://lib.csdn.net/base/linux)，只要安装了对应版本的jvm都可以运行.class文件。
同样nodejs的作用和jvm的一样一样的，也是js的运行环境，不管是你是什么[操作系统](http://lib.csdn.net/base/operatingsystem)，
只要安装对应版本的nodejs，那你就可以用js来开发后台程序。
这具有划时代的意义，意味着一直以来只能在浏览器上玩来玩去的js，可以做后端开发了，
从有了nodejs后就催生出一大批用js做后台开发的前端人员，这部分人员就是偏前端的“全栈程序员”。
### **记住，nodejs是和jvm同等地位的js运行环境，打开了前端人员走向后端的道路。**
![1.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150525/1432569343252051321.png)
# 【js mvc框架】
#### 相关框架：
框架太多，详见下面两篇文章
[私人定制，十款最佳Node.js
 MVC框架](http://www.csdn.net/article/2014-02-24/2818519-node-js-mvc-frameworks-for-javascript-developers)
[基于NodeJS的14款Web框架](http://www.csdn.net/article/2014-03-25/2818964-web-application-frameworks-for-node-js)
#### 类比Java中的：
ssh1（struts1+[spring](http://lib.csdn.net/base/javaee)+[hibernate](http://lib.csdn.net/base/javaee)），ssh2（struts2+spring+hibernate），ssm（springmvc+spring+mybatis）
#### 详述：
上面说到nodejs打开了[前端开发](http://lib.csdn.net/base/javascript)人员开发后端的大门，而且nodejs类比jvm，
那么学习java的人都知道，学习完jvm（基础）后该学什么了？
对，框架，
java中有哪些框架，正如上面所说，ssh1，ssh2，ssm等等，
这些框架都mvc框架。
既然nodej都有了，jvm出现了，那接下来就是js大神开始封装mvc框架，正如java大神开始封装mvc框架一样，
相对java流行了几种框架，nodejs对应的mvc框架就多的多了，
详见上面两篇文章，足够让你看的眼花缭乱。
其中比较有名的是expressjs。
### **记住，当你看到这些js框架的时候，微微一笑，原来就是写mvc框架，基于nodejs：**
### **Sails. js，Total.js，Partial.js，Koa. js，Locomotive. js，Express. js,Flatiron. js**
![2.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150526/1432569717927009187.png)
# 【js模块化】
#### 相关概念：
commonjs，amd，cmd，umd
#### 相关框架：
commonjs，seajs，requirejs，coolie
#### 类比Java中的：
import，对就是import。。
#### 详述：
如上所说，当有了nodejs（jvm），有了mvcjs（ssh）之后，
可想而知，每个mvcjs中会有多少js文件，这个时候js模块化就派上用处了，
当有人和你说js模块化如何如何，感觉自己很吊的时候，甩他一句，不就是java中的import吗？
对，虽然在前端看来js模块化如何牛x，如何吊，但是就java来说就是import。
看看下面两张图片就明白了，对比requirejs（amd）和java导包：
![1.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150526/1432637033653019840.png)
![2.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150526/1432637040854030389.png)
但是js中的模块化，还有很多规范，比如commonjs，amd，cmd，umd，感觉头大了吧，
其实简单的来说，就是commonjs是运行在nodejs端，amd，cmd，umd是运行在浏览器，
其作用就是import各种js文件，把js模块化管理，可以理解为java中的包管理，
详见这篇文章：[http://coolie.ydr.me/introduction/commonjs-amd-cmd-umd.html](http://coolie.ydr.me/introduction/commonjs-amd-cmd-umd.html)
同样，一个mvc，js能做出来10+种框架，可想而知模块化，js也对应很多框架，
例如commonjs，requirejs，seajs等等。
### **记住，commonjs，requirejs，seajs等js模块化框架，遵循各种规范（amd，cmd，umd，commonjs），**
### **类比java中的import**
![3.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150526/1432570019340093869.png)
### **【reactjs】**
#### 官网：
[http://facebook.github.io/react/](http://facebook.github.io/react/)
#### 简介：
facebook前不久出的一款框架，众前端膜拜之。
类比Java中的：freemarker的宏。
#### 详述：
facebook前不久出了一款js框架，reactjs，
一时间，凡是用过reactjs，或者听过reactjs的前端开发就高人一等，
那么，这个框架到底是干嘛的，我们来看看官网的一个例子：
![5.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150526/1432570273541059643.png)
好的，看不懂没关系，我来说说，
左边是一段代码，右边是这段代码在网页中的效果，
左边代码中上面一大段是定义，最后一句话是使用，
左边代码jsx是reactjs的格式，旁边有个compiled js是jsx编译后的js，原生js。
也就是说，你通过写jsx文件，编译后生成一段js文件，这段js文件运行后是右边的效果，
那么好处是什么？
是封装，一大段js定义，最后只需要一句话输出，也就是一行js代码对应右边一个ui组件。
对了reactjs最大的作用就是用来开发ui组件，例如这个：[http://material-ui.com/#/components/dropdown-menu](http://material-ui.com/#/components/dropdown-menu)
很酷的效果，material ui风格的webui组件，基于reactjs开发的。
做java的不知道用过freemarker没，用过freemarker的不知道用过宏没，看段代码：
![10.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150526/1432570782957077446.png)
----------------------------------------------
-----------------------------------------------
![8.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150526/1432570804070065074.png)
------------------------------------
------------------------------------
![7.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150526/1432570818162025983.png)
第一个图片中是freemarker用宏封装了bootstrap的button组件，
第二个图片是在代码用使用封装后的bsbutton，
第三个图片是效果
有没有发现和reactjs很相似啊，只不过reactjs是在前端实现，
而freemarker是在后端实现，两者的共同点是封装，且可以传参。
### **记住，facebook出品的reactjs是用来开发ui库的js框架，特点是可以封装大量代码。**
![11.png](http://uikoo9-ueditor.qiniudn.com/@/images/20150526/1432570974336090023.png)
**更多精彩内容：**[**http://uikoo9.com/**](http://uikoo9.com/)
