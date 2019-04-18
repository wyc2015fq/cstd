# Js作用域与作用域链详解 - z69183787的专栏 - CSDN博客
2017年07月04日 17:03:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：617
    一直对[js](http://lib.csdn.net/base/javascript)的作用域有点迷糊，今天偶然读到[JavaScript](http://lib.csdn.net/base/javascript)权威指南，立马被吸引住了，写的真不错。我看的是第六版本，相当的厚，大概1000多页，Js博大精深，要熟悉精通需要大毅力大功夫。
一：函数作用域
   先看一小段代码：
**[javascript]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)
- var scope="global";  
- function t(){  
-     console.log(scope);  
- var scope="local"
-     console.log(scope);  
- }  
- t();  
(PS: console.log()是firebug提供的调试工具，很好用，有兴趣的童鞋可以用下，比浏览器+alert好用多了）
第一句输出的是： "undefined"，而不是 "global"
第二讲输出的是："local"
  你可能会认为第一句会输出："global",因为代码还没执行var scope="local",所以肯定会输出“global"。
  我说这想法完全没错，只不过用错了对象。我们首先要区分[javascript](http://lib.csdn.net/base/javascript)的函数作用域与我们熟知的C/C++等的块级作用域。
  在C/C++中，花括号内中的每一段代码都具有各自的作用域，而且变量在声明它们的代码段之外是不可见的。而Javascript压根没有块级作用域，而是函数作用域.
所谓函数作用域就是说：-》变量在声明它们的函数体以及这个函数体嵌套的任意函数体内都是有定义的。
所以根据函数作用域的意思，可以将上述代码重写如下：
**[javascript]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)
- var scope="global";  
- function t(){  
- var scope;  
-     console.log(scope);  
-     scope="local"
-     console.log(scope);  
- }  
- t();  
    我们可以看到，由于函数作用域的特性，局部变量在整个函数体始终是由定义的，我们可以将变量声明”提前“到函数体顶部，同时变量初始化还在原来位置。
为什么说Js没有块级作用域呢，有以下代码为证：
**[javascript]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)
- var name="global";  
- if(true){  
- var name="local";  
-     console.log(name)  
- }  
- console.log(name);  
都输出是“local",如果有块级作用域，明显if语句将创建局部变量name,并不会修改全局name,可是没有这样，所以Js没有块级作用域。
现在很好理解为什么会得出那样的结果了。scope声明覆盖了全局的scope,但是还没有赋值，所以输出：”undefined“。
所以下面的代码也就很好理解了。
**[javascript]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)
- function t(flag){  
- if(flag){  
- var s="ifscope";  
- for(var i=0;i<2;i++)   
-             ;  
-     }  
-     console.log(i);  
-     console.log(s);  
- }  
- t(true);  
输出：2  ”ifscope"
二：变量作用域
还是首先看一段代码：
**[javascript]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)
- function t(flag){  
- if(flag){  
-         s="ifscope";  
- for(var i=0;i<2;i++)   
-             ;  
-     }  
-     console.log(i);  
- }  
- t(true);  
- console.log(s);  
就是上面的翻版，知识将声明s中的var去掉。
程序会报错还是输出“ifscope"呢？
让我揭开谜底吧，会输出：”ifscope"
这主要是Js中没有用var声明的变量都是全局变量，而且是顶层对象的属性。
所以你用console.log(window.s)也是会输出“ifconfig"
当使用var声明一个变量时，创建的这个属性是不可配置的，也就是说无法通过delete运算符删除
var name=1    ->不可删除
sex=”girl“         ->可删除
this.age=22    ->可删除
三：作用域链
先来看一段代码：
**[javascript]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)
- name="lwy";  
- function t(){  
- var name="tlwy";  
- function s(){  
- var name="slwy";  
-         console.log(name);  
-     }  
- function ss(){  
-         console.log(name);  
-     }  
-     s();  
-     ss();  
- }  
- t();  
当执行s时，将创建函数s的执行环境(调用对象),并将该对象置于链表开头，然后将函数t的调用对象链接在之后，最后是全局对象。然后从链表开头寻找变量name,很明显
name是"slwy"。
但执行ss()时，作用域链是： ss()->t()->window,所以name是”tlwy"
下面看一个很容易犯错的例子：
**[html]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)
- <html>
- <head>
- <scripttype="text/javascript">
- function buttonInit(){  
-     for(var i=1;i<4;i++){  
-         var b=document.getElementById("button"+i);  
-         b.addEventListener("click",function(){ alert("Button"+i);},false);  
-     }  
- }  
- window.onload=buttonInit;  
- </script>
- </head>
- <body>
- <buttonid="button1">Button1</button>
- <buttonid="button2">Button2</button>
- <buttonid="button3">Button3</button>
- </body>
- </html>
当文档加载完毕，给几个按钮注册点击事件，当我们点击按钮时，会弹出什么提示框呢？
很容易犯错，对是的，三个按钮都是弹出："Button4",你答对了吗？
当注册事件结束后，i的值为4，当点击按钮时，事件函数即function(){ alert("Button"+i);}这个匿名函数中没有i,根据作用域链，所以到buttonInit函数中找，此时i的值为4，
所以弹出”button4“。
弹出Button 1 2 3 的 实现方法：
```
<!doctype html>
<html>
<head>
<script type="text/javascript">
function buttonInit(){
for(var i=1;i<4;i++){
var b = document.getElementById("button" + i);
(function (a)
{
b.addEventListener("click", function () { alert("Button" + a); }, false);
})(i);
}
}
window.onload=buttonInit;
</script>
</head>
<body>
<button id="button1">Button1</button>
<button id="button2">Button2</button>
<button id="button3">Button3</button>
</body>
</html>
```
四：with语句
说到作用域链，不得不说with语句。with语句主要用来临时扩展作用域链，将语句中的对象添加到作用域的头部。
看下面代码
**[javascript]**[view
 plain](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)[copy](http://blog.csdn.net/yueguanghaidao/article/details/9568071#)
- person={name:"yhb",age:22,height:175,wife:{name:"lwy",age:21}};  
- with(person.wife){  
-     console.log(name);  
- }  
with语句将person.wife添加到当前作用域链的头部，所以输出的就是：“lwy".
with语句结束后，作用域链恢复正常。
