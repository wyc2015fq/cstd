# 看几道JQuery试题后总结（下篇） - z69183787的专栏 - CSDN博客
2014年03月07日 16:04:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：653
### 感谢圆友的提醒
昨天下午完成了9道试题中的前4道，之后好多园友存在些疑惑和建议，在这里我一并说一下吧。首先对于昨天第一题可能存在误导，在JQuery中并没有innerHTML这个属性，不过我们可以将JQuery对象转换成DOM对象再使用innerHTML，还有对于那个innerTEXT也是操作DOM对象的，不过貌似用的比较少。其次是第三题，我们选择表单元素，尽量使用表单元素选择器。总之，感谢圆友们的批评指正、、、
接下来我们继续完成昨天没有完成的试题......
### 第五题
- 题目：JQuery对象与dom对象的区别，以及两者互相转换的方法
```
var jquery = $("#x1");      //jquery对象
var dom = document.getElementById("#x1");       //dom对象
```
　　两者区分：
　　jQuery对象就是通过jQuery包装DOM对象后产生的对象，它是jQuery独有的，使用JQuery对象可以使用JQuery对象中的方法。例如在JQuery中我们可以使用"对　　　　象.html()"来获取元素内的内容，它等同于在DOM中使用"对象.innerHTML"。还有我们使用这两种对象的时候必须分清情况，不能再JQuery对象中使用DOM对象的方法，也不能在BOM对象中使用JQuery对象的方法，小弟我昨天就犯错了。
　　两者互相转换：
```
/*JQuery转化成DOM*/
var x=$("#x1"); //jquery对象
var y = x[0]; //dom对象 也可写成 var y=x.get(0);
y.innerHTML = "断桥残雪";
```
```
/*DOM转化成JQuery*/
var m=document.getElementById("x1");     //dom对象
var n = $(m);     //转换成jquery对象
n.html();
```
通过以上演示的方法，我们就可以任意的相互转换jquery对象和dom对象。
### 第六题
- 题目：如何动态的为某个div添加class属性，如何移除class属性，以及如何获取div中某个属性的值，比如说Id的名称或者其他属性的值
　　首先看代码：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View
 Code
```
点击添加Class与点击移除Class
![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_23.jpg)![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_456.jpg)
同时我们可以看到，我们通过JQuery对象的attr()方法可以获取对象中属性存在的数据，如果没有该属性则返回undefined。
### 第七题
- 题目：JQuery里面的动画函数一般用什么？时间设置函数都有哪些,setTimeout()与setInterval()有何区别
在JQuery中使用动画(从上到下分组依次是基本，滑动，淡入淡出，自定义)
- show()
- hide()
- toggle()　　
- slideDown()
- slideUp()
- slideToggle()
- fadeIn()
- fadeOut()
- fadeToggle()
- animate()
- stop()
- delay()
对于setTimeout()与setInterval()的区别，setInterval()是按照指定的周期(毫秒)来调用函数或计算表达式，而setTimeout()是在指定的毫秒数后调用函数或表达式
### 第八题
- 题目：写一个ajax例子出来，要标清楚各个参数的含义。以及ajax数据传输的时候有几种数据格式
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View
 Code
```
向后台传值：
![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_aa.jpg)![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_bb.jpg)
![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_cc.jpg)
### 第九题
- 题目：要做出一个demo具体功能如下：页面中有一个按钮，点击按钮后弹出一个遮罩层，在遮罩层上面有一个div,在点击按钮后5秒后div内部出现内容。 在div下面有一个隐藏按钮，点击后，遮罩层消失，div缓慢消失。回到页面打开状态。（友情提示：遮罩要用到层级z-index,以及透明效果，5秒要用到时间函数，缓慢消失要用到动画效果，自己查询API文档。）
　　类似这种题目曾经做过一次，都是基础知识的拼凑，下次贴上代码吧！
待续ing、、、、、、
### 结语
　　感觉这套题目一般般，不过覆盖的面也挺全的，大家细细琢磨吧！这篇比上篇写得水多了，话说会被移除首页的，呵呵。今天遇到了些不爽的事儿，写这博客期间中断了好多次，抱歉！
　　------如果你觉得此文对你有所帮助，别忘了点击下右下角的推荐咯，谢谢！------
