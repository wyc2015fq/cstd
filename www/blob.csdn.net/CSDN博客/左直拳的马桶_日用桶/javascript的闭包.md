# javascript的闭包 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月11日 23:58:11[左直拳](https://me.csdn.net/leftfist)阅读数：1618
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
闭包闭包闭包，屁包屁包屁包
一、什么是闭包
**javascript里面，所有的函数都是闭包！**
这句断言真是惊天地，泣鬼神。不过，通常意义上的闭包是指：某个函数内部的子函数，这个子函数被外界所调用，用于访问父函数（就是那个该死的“某个函数”啦）内的变量（包括参数）。
从中可以看出，我们常常说的闭包是：
1）首先，闭包是一个函数
2）其次，闭包是一个子函数，就是说，它是一个嵌套在函数里的子函数，函数中的函数
3）这个子函数有什么用呢？它会被外界访问，外界通过访问它来访问和操控它所在的父函数中的变量
不过也有一种说法，说闭包是一种特殊的对象，这个对象包括函数和创建该函数的环境。这个环境是指什么呢？我觉得是指父函数中的那些变量。
一个典型的闭包如下：
```java
function a() { 
 var i = 0; 
 function b() { alert(++i); } 
 return b;
}
var c = a();
c();
```
其中，b就是那个传说中的闭包。
当 函数a 的 内部函数b 被 函数a外的一个变量 引用的时候，就创建了一个 闭包。
二、闭包有什么用
1、外界访问函数内部变量的桥梁
如上所述
好处就是封装，提高私密性
2、保护父函数的内部变量，避免被JS的垃圾回收机制回收
看回开头那个例子：
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41869741#)[copy](http://blog.csdn.net/leftfist/article/details/41869741#)
- function a() {   
- var i = 0;   
- function b() { alert(++i); }   
- return b;  
- }  
- var c = a();  
- c();  
var i 是函数a的内部变量，在语句 var c = a(); 运行之后，函数a已经运行结束，按照一般的理解，i 就应该被回收了（或者说被打上回收标志了），不可再用了。但这段代码正常运行，一点问题没有，原因就在于闭包：b 被 c 引用，而 b 依赖于 i，因此，i 免遭垃圾回收。
3、闭包被调用的时候，会如同面向对象中创建实例一样，将父函数的内部变量（包括父函数传入的参数）克隆一份。被调用多少次，就创建多少份，每份都各自独立，互不影响。
看看这个例子：
页面中有如下HTML代码：
**[html]**[view plain](http://blog.csdn.net/leftfist/article/details/41869741#)[copy](http://blog.csdn.net/leftfist/article/details/41869741#)
- <pid="help">Helpful notes will appear here</p>
- <p>E-mail: <inputtype="text"id="email"name="email"></p>
- <p>Name: <inputtype="text"id="name"name="name"></p>
- <p>Age: <inputtype="text"id="age"name="age"></p>
然后我们试图通过JS代码实现这样的效果：光标停在不同的文本框，就显示不同的提示信息
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41869741#)[copy](http://blog.csdn.net/leftfist/article/details/41869741#)
- function showHelp(help) {  
-   document.getElementById('help').innerHTML = help;  
- }  
- 
- function setupHelp() {  
- var helpText = [  
-       {'id': 'email', 'help': 'Your e-mail address'},  
-       {'id': 'name', 'help': 'Your full name'},  
-       {'id': 'age', 'help': 'Your age (you must be over 16)'}  
-     ];  
- 
- for (var i = 0; i < helpText.length; i++) {  
- var item = helpText[i];  
-     document.getElementById(item.id).onfocus = function() {  
-       showHelp(item.help);  
-     }  
-   }  
- }  
- 
- setupHelp();  
结果没有达到预期的结果，文本框获得焦点的时候，全部提示最后一个提示信息：'Your age (you must be over 16)'
为什么会这样呢？
赋给onfocus事件的，是一个个匿名函数。这些匿名函数，使用了父函数setupHelp里的变量 item。因此，从这个意义上来说，这些匿名函数是闭包（别看item是在循环体内定义的，但在父函数setupHelp里，处处都能访问， javascript就是这么怪。可能在javascript里，变量的作用域都是对函数而言的）。当onfocus事件被激发的时候，循环早已结束，此时item因为匿名函数是闭包的缘故，它还存活着，但值是指向helpText最后一项的。而因为那几个匿名函数引用的都是item，因此出现所有提示都相同的情况。
怎么办呢？应该在循环中，将每一次的item都保留下来。
完成这项任务的，还是闭包。因为闭包对变量的保护，无微不至。闭包就是为了操控内部变量而存在。如果世界上有一种爱是真爱，那就是闭包对变量的爱。闭包闭包闭包，屁包屁包屁包。
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41870231#)[copy](http://blog.csdn.net/leftfist/article/details/41870231#)
- //看，这里有一个闭包，提供了对变量help(参数也算内部变量)的保护
- function showHelp(help) {  
- returnfunction(){document.getElementById('help').innerHTML = help;};  
- }  
- 
- function setupHelp() {  
- var helpText = [  
-       {'id': 'email', 'help': 'Your e-mail address'},  
-       {'id': 'name', 'help': 'Your full name'},  
-       {'id': 'age', 'help': 'Your age (you must be over 16)'}  
-     ];  
- 
- for (var i = 0; i < helpText.length; i++) {  
- var item = helpText[i];  
-     document.getElementById(item.id).onfocus = showHelp(item.help);//这里也稍有改动
-   }  
- }  
- 
- setupHelp();  
如此，目的可达到矣。每一次循环的item，都通过闭包机制，好好地保存下来了。
---------------------------------------------------
function showHelp(help) 构成了一个闭包，将每轮循环中的help保存下来了。此类闭包保存变量的机制，常用在循环中；每一轮循环的值，要作为参数传递给这个闭包。
三、闭包虽好，可别滥用
1、由于闭包会使得函数中的变量都被保存在内存中，内存消耗很大，所以不能滥用闭包，否则会造成网页的性能问题，在IE中可能导致内存泄露。解决方法是，在退出函数之前，将不使用的局部变量全部删除。
2、闭包会在父函数外部，改变父函数内部变量的值。所以一定要小心，不要随便改变父函数内部变量的值。
参考文章：
[http://www.ruanyifeng.com/blog/2009/08/learning_javascript_closures.html](http://www.ruanyifeng.com/blog/2009/08/learning_javascript_closures.html)
[https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Guide/Closures](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Guide/Closures)
[http://www.jb51.net/article/24101.htm](http://www.jb51.net/article/24101.htm)
有一个网站很好，可以在里面写JS、HTML等进行测试
[http://jsfiddle.net/](http://jsfiddle.net/)
