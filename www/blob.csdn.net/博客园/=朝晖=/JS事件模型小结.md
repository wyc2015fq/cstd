# JS事件模型小结 - =朝晖= - 博客园
# [JS事件模型小结](https://www.cnblogs.com/dhcn/p/7116194.html)
08年年初的一篇blog，转到这个博客上,想想当年跨浏览器真是一座山：
   前几天看dojo的文档，其中多次提到DOM2事件模型，以前学习这部分的内容，主要愤慨于IE的不兼容性了，现在对于DOM2事件模型的概念又有些模糊，所以翻出了[js](http://lib.csdn.net/base/javascript)权威指南(V4)又复习了一下，简单作一小结。
       JS权威指南中将JS事件模型分为四种
            1、原始事件模型:属性事件处理模式
            2、标准事件模型:DOM2对其作了标准化
            3、IE事件模型(IE5.5/IE6)
            4、 Netscape事件模型
 原始事件模型
         也就是基本事件处理，其实我们大多数人使用的JS事件处理模式都是这种代码方式。
         其事件类型:分为"输入事件(如onclicki)"和"语义事件(如onsubmit)"
        事件程序的注册可以以下几种方式:
         1、JS代码作为HTML性质值
```
<input type="button" value="Press me" onclick="alert('thanks');"
```
          2、事件处理程序作为JS属性
         附注：文档中的每个HTML元素在文档树中都有一个相应的JS对象，这个JS对象的属性对应于那个HTML元素的性质，无论作为HTML性质的JS代码还是作为JS属性的时间处理程序，其本身的属性都是函数"function".
         作为JS属性的例子：
html背景：
```
<form name="f1">  
<input name="b1" type="button" value="Press Me"/>  
</form>
```
第一种赋值方式:
```
document.f1.b1.onclick=function()  
    {  
       alert('thanks');  
     };
```
第二种赋值方式:
```
function plead()  
{  
   window.status="Please Press Me!";  
}  
document.f1.bi.onmouseover=plead;
```
作为JS属性的事件处理程序可以用JS属性显式调用
```
document.myfrom.onsubmit();
```
事件处理程序可以返回fale来阻止浏览器执行事件的默认动作，常用的如对onsubmit的操作。例外是
对超链接mouseover的window.status显示事件的阻挡，是返回true.例如:
```
<a href="help.htm" onmouseover="window.status='help';return true;">help</a>
```
        JS函数运行在词法作用域中，也就是定义他们的作用域中，因此作为HTML性质的JS事件处理程序，作用域链比较特殊，其头是调用对象(既这段JS代码),下一个对象是触发事件的对象既event.currentTarget,其后就是其包容层元素。整个作用域链的精确构成依赖于具体实现。
DOM2事件模型
         DOM2的模型提高的复杂度:
         1、首先它的事件模型中引入了传播过程：1）先由document向目标对象传播称之为：捕捉阶段；2）目标对象的事件处理程序运行，3）从目标对象向document起泡。Event.stopPropagation()可以停止传播，preventDefault可以阻止事件的默认动作（想想以前的return false）。
        2、事件处理程序注册：EventTarget.addEventListensr(String type,EcentListener listener,blooean useCapure).其中第三个参数决定注册程序在传播的那个过程被调用，true:在捕捉阶段调用，false:在后两个阶段被调用。
        3、DOM2还支持JAVA式的对象式事件处理程序注册,用的少，略。
        4、事件模块和事件类型：根据事件语义和特性做分类
        5、合成事件，即由JS代码模拟事件发生(适用于Test代码):它分为以下三个步骤：
              1）创建合适的事件对象:
```
var e=document.createEvent("HTMLEvents");
```
             2)初始化事件对象域(即:属性)
```
e.initEcent("click","true","true");
```
             3)分派事件对象：
```
eventTarget.dispatchEvent();
```
IE事件模型
         特点：
           1、传播过程只起泡，不捕捉。起泡中断方法：
window.ecent.cancelBubble=true;
           2、Event对象不是事件处理程序的函数参数，而是window的全局变量。
           3、事件注册函数:attachEvent()和反注册：detachEvent().
Netscape4事件模型
          由于Netscape好像已经完全停止开发，所以就不详述了，简单的说：就是只捕捉不起泡。
          后话：DOM的实现模范本来应该是FireFox。不过就事件注册的标准度上，Opera实现的好象更严格一点。参见《关于DOM事件模型的两件事》http://www.marchbox.com/blog/2-things-about-dom-events-model/      
