# [从jQuery看JavaScript]-匿名函数与闭包（Anonymous Function and Closure） - z69183787的专栏 - CSDN博客
2014年03月07日 21:01:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：845
 jQuery片段：
**[javascript]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- (function(){  
- //这里忽略jQuery所有实现
- })();  
　　半年前初次接触jQuery的时候，我也像其他人一样很兴奋地想看看源码是什么样的。然而，在看到源码的第一眼，我就迷糊了。为什么只有一个匿名函数又没看到运行（当然是运行了……），就能有jQuery这么个函数库了？于是，我抱着疑问来到CSDN。结果相信现在很多人都很清楚了（因为在我之后也不乏来者，呵呵~）。当一个匿名函数被括起来，然后再在后面加一个括号，这个匿名函数就能立即运行起来！真神奇哦！
　　嘿嘿！胡闹到此为止。在这一节，我们碰到的jQuery片段是一组立即运行的匿名函数。而这种用法在论坛上也曾引起过激辩——这段代码究竟属不属于闭包呢？带着这个疑问，我们从基础开始，分析每个关键要素，寻找属于自己的答案。（没错，自己的答案！在我看来，所有理论只是形式，只要它有利于我们的应用实现，就是可取的——黑猫白猫，抓到老鼠的就是好猫！）
　　要说匿名函数，我们首先要由函数本身说起。函数的定义如下：
> 
**函数是将唯一的输出值赋予给每一输入的“法则”。**
　　当然，这只是数学上的定义。但是，在计算机编程语言中，函数的定义也八九不离十。因为，我们都知道，计算机中的函数，也类似数学定义中的描述，它是**将输入的若干数据，经过代码设定的逻辑操作处理后，返回唯一的输出的一组代码组合块。**——当然，特例是，输入的数据为空或输出的数据为空，或者两者都为空。
　　下面，我们先初步了解一下和匿名函数相关的概念。
- **函数声明（function 语句）**
　　要使用一个函数，我们就得首先声明它的存在。而我们最常用的方式就是使用function语句来定义一个函数，如：
**[javascript]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- function abc(){  
- // code to process
- }  
 　　当然，你的函数也可以是带参数的，甚至是带返回值的。
**[javascript]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- function abc(x,y){  
- return x+y;  
- }  
　　但是，无论你怎么去定义你的函数，JS解释器都会把它翻译成一个Function对象。例如，你在定义上面的其中一个例子的函数号，再输入如下代码：
**[javascript]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- alert(typeof abc);// "function"
　　你的浏览器就会弹出提示框，提示你abc是一个Function对象。那么Function对象究竟是什么呢？
- **Function 对象**
　　Function对象是JavaScript里面的固有对象，所有的函数实际上都是一个Function对象。关于这个方面的讨论，我们留到下一个专题节。我们先看看，Function对象能不能直接运用构造函数创建一个新的函数呢？答案是肯定的。例如：
**[javascript]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- var abc = new Function("x","y","return x*y;");  
- alert(abc(2,3)); // "6"
　　相信大家现在对如何声明一个函数应该是有所了解了。那么什么才是匿名函数呢？
- **声明匿名函数**
　　顾名思义，匿名函数就是没有实际名字的函数。例如，我们把上面的例子中，函数的名字去掉，再判断一下他是不是一个函数：
**[c-sharp]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- alert(typeof function(){});// "function"
- alert(typeof function(x,y){return x+y;});// "function"
- alert(typeofnew Function("x","y","return x*y;"))// "function"
　　我们可以很容易地看到，它们全都是Function对象，换言之，他们都是函数，但是他们都有一个特点——没有名字。所以我们把他们称作“匿名函数”。然而，正因为他们没有“名字”，我们也没有办法找到他们。这就引申了如何去调用一个匿名函数的问题了。
- **匿名函数的调用**
　　要调用一个函数，我们必须要有方法定位它，引用它。所以，我们会需要帮它找一个名字。例如：
**[javascript]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- var abc=function(x,y){  
- return x+y;  
- }  
- alert(abc(2,3)); // "5"
　　上面的操作其实就等于换个方式去定义函数，这种用法是我们比较频繁遇到的。例如我们在设定一个DOM元素事件处理函数的时候，我们通常都不会为他们定名字，而是赋予它的对应事件引用一个匿名函数。
　　对匿名函数的调用其实还有一种做法，也就是我们看到的jQuery片段——使用()将匿名函数括起来，然后后面再加一对小括号（包含参数列表）。我们再看一下以下例子：
**[c-sharp]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- alert((function(x,y){return x+y;})(2,3));// "5"
- alert((new Function("x","y","return x*y;"))(2,3));// "6"
　　很多人或许会奇怪，为什么这种方法能成功调用呢？觉得这个应用奇怪的人就看一下我以下这段解释吧。
　　大家知道小括号的作用吗？小括号能把我们的表达式组合分块，并且每一块，也就是每一对小括号，都有一个返回值。这个返回值实际上也就是小括号中表达式的返回值。所以，当我们用一对小括号把匿名函数括起来的时候，实际上小括号对返回的，就是一个匿名函数的Function对象。因此，小括号对加上匿名函数就如同有名字的函数般被我们取得它的引用位置了。所以如果在这个引用变量后面再加上参数列表，就会实现普通函数的调用形式。
　　不知道以上的文字表述大家能不能看明白，如果还是理解不了的话，再看一下以下的代码试试吧。
**[javascript]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- var abc=function(x,y){return x+y;};// 把匿名函数对象赋给abc
- // abc的constructor就和匿名函数的constructor一样了。也就是说，两个函数的实现是一样的。
- alert((abc).constructor==(function(x,y){return x+y;}).constructor);  
　　PS：constructor是指创建对象的函数。也就是函数对象所代表的函数体。
　　总之，将其（被小括号包含的匿名函数）理解为括号表达式返回的函数对象，然后就可以对这个函数对象作正常的参数列表调用了。（前面这里犯了个错误，只有函数表达式还是不能直接调用函数的，去掉匿名函数括号必须要伴随将表达式赋值。也就是(function(){alert(1)})()应该是与a=function(){alert(1)}()等价，不能连a=都去掉。）
- **闭包**
 　　闭包是什么？闭包是指某种程序语言中的代码块允许一级函数存在并且在一级函数中所定义的自由变量能不被释放，直到一级函数被释放前，一级函数外也能应用这些未释放的自由变量。
　　怎样？看得一头冒汗吧……没事，我也是（虽然是我是了解的，只是表达能力的问题）。让我们换个更加简单的方法说明：闭包，其实是一种语言特性，它是指的是程序设计语言中，允许将函数看作对象，然后能像在对象中的操作搬在函数中定义实例（局部）变量，而这些变量能在函数中保存到函数的实例对象销毁为止，其它代码块能通过某种方式获取这些实例（局部）变量的值并进行应用扩展。
　　不知道这么再解释后会否更加清晰，如果还是不明白，那么我们再简化一下：闭包，其实就是指程序语言中能让代码调用已运行的函数中所定义的局部变量。
　　现在我们看一个例子：
**[javascript]**[view
 plain](http://blog.csdn.net/natineprince/article/details/4759533#)[copy](http://blog.csdn.net/natineprince/article/details/4759533#)
- var abc=function(y){  
- var x=y;// 这个是局部变量
- returnfunction(){  
-   alert(x++);// 就是这里调用了闭包特性中的一级函数局部变量的x，并对它进行操作
-   alert(y--);// 引用的参数变量也是自由变量
- }}(5);// 初始化
- abc();// "5" "5"
- abc();// "6" "4"
- abc();// "7" "3"
- alert(x);// 报错！“x”未定义！
　　看到这里，你能判断究竟jQuery的那个代码片段是否闭包了吗？
　　以我的理解来说吧。是否应用了闭包特性，必须确定该段代码有没有最重要的要素：未销毁的局部变量。那么很显然，没有任何实现的匿名函数不可能应用了闭包特性。但如果匿名函数里面有实现呢？那也还得确定它的实现中有没有用到那些未销毁的局部变量。所以如果问你那个开篇中的jQuery代码片段是应用了JS里的什么特性？那么它只是匿名函数与匿名函数的调用而已。但是，它隐含了闭包的特性，并且随时可以实现闭包应用。因为JS天生就是有这个特性的！（这只是我的理解，我也想知道你的理解，欢迎交流！关于闭包，有机会还是独立再开一个专题吧！）
　　在这里向以前理解错误时，在回答CSDN各位问题的时候所作的错误回答道歉——Sorry...
　　由于本笔记是走读jQuery时引申出的JS知识点整理，所以可能和真正的JS学习不搭旮，建议初学JS的人还是先看看基础的书籍吧。不过接下来的几章应该都是对JS基础的整理了，对此篇阅读有困难的同学可以先查看后面的章节。谢谢支持！
