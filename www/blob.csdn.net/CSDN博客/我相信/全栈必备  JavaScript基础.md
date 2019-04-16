# 全栈必备  JavaScript基础 - 我相信...... - CSDN博客





2017年04月05日 08:29:37[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：10123
所属专栏：[全栈之路](https://blog.csdn.net/column/details/12818.html)











## JavaScript 来了

[喔家ArchiSelf](https://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658971386&idx=1&sn=d5b529569c01a1f43b9629bea10542af&chksm=80d333dcb7a4baca756bba381a6bbf47f34eb83326cdd4ebe36573984f67e5f53ff49801f3fc#%23)





1995年，诞生了JavaScript语言，那一年，我刚刚从大学毕业。在今年RedMonk 推出的2017 年第一季度编程语言排行榜中，JavaScript 排第一，Java 第二，Python 反超 PHP 排第三，PHP 第四，C# 和 C++ 并列第五。RedMonk 排名的主要依旧是各种编程语言在 Stack Overflow 和 GitHub 上的表现，比如编程语言在 Stack Overflow 上的讨论数量，在 GitHub 上的代码量等。尽管有一定的片面性，还是说明了JavaScript 应用的广泛性。从全栈的角度看，Javascript 是必备的一种编程语言。 

![](https://img-blog.csdn.net/20170404085720601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## ECMAScript 和 JavaScript 的关系

JavaScript 诞生于Netscape，但是在1996年，微软发布了与JavaScript 兼容的JScript，面对兼容和发展的需要，网景公司的先贤们努力加入了 ECMA International 标准化组织，致力于JavaScript 的标准化，命名为ECMAScript。后来，由于历史的原因， JavaScript标准的开发主体变成了Mozila基金会。关于ECMAScript 的最新版本可以参阅 https://tc39.github.io/ecma262/。

简单地，ECMAScript 是JavaScript语言的标准规范，就像C++的标准相对于C++语言那样。

## JavaScript 是怎样的语言

在mozilla 开发者网站上是这样描述JavaScript的：

JavaScript (JS) is a lightweight interpreted or JIT-compiled programming language with first-class functions. 

意思是说JavaScript 是一个轻量级解释或即时编译的函数式语言，里面有很多的概念，轻量、解释、编译、即时编译、函数式。在老码农看来，简单起见，理解为扩展语言较为方便。

一般的编程语言都有着自己相对独立的执行环境，但是JavaScript的执行环境依赖在宿主环境中，宿主环境尤其是客户端的宿主环境提供了更多统一的环境变量，比如浏览器中的window，document等。实际上，JavaScript 和DOM 是可分的，对于不同的运行环境，有着不同的内置宿主对象。JavaScript作为扩展语言在内置的宿主环境中运行，全局对象在程序启动前就已经存在了。

## JavaScript的时空基础

从空间观的角度看，JavaScript包括数据结构，操作符，语句与表达式，函数；从时间的角度看，包括作用域，处理方式，模块与库。关于技术系统的时空观，可以参见《面向全栈的技术管理》一文。

#### 数据结构

JavaScript 中包含的六种基本类型：
- 
Boolean

- 
Null

- 
Undefined

- 
Number

- 
String

- 
Symbol (ECMAScript 6)


其它全是对象。值是有类型的，变量是没有类型的，类型定义了值的行为特征，变量在没有持有值的时候是undefined。 JavaScript对值和引用的赋值／传递在语法上没有区别，完全根据值的类型来判定。

对于对象的属性和方法而言，全局变量和全局函数是全局对象的属性，全局对象相当于宿主对象的根对象。需要注意是属性的属性中那些不可变对象的实现方式： 
- 
对象常量： 结合writable和configurable:false 可以创建一个真正的常量属性

- 
禁止扩张：Object.preventExtensions(..)来禁止一个对象添加新属性并保留已有属性

- 
密封： 在 Object.seal(..) 后不能增，删，改 该属性

- 
冻结： Object.freeze(..) 会禁止对于对象本身及任意直接属性的修改


数据类型的判定可以通过 contructor，instanceof， isPrototypeOf等方法实现，对于鸭子类型的判定还可以使用 in 的相关操作。符号并非对象，而是一种简单标量基本类型。

 JavaScript 中的强制类型转换总是返回基本类型值，将对象强制转换为String 是通过ToPrimitive抽象操作完成的，而toJSON（）是返回一个能够被字符串化的安全的JSON值。

#### 操作符

操作符是空间元素连接的纽带之一，JavaScript操作符包括算术，连接，相等，比较，逻辑，位，类型判断，条件，new，delete， void，"，", ".", "[]"等。

在JavaScript中以操作符进行操作往往都附带着类型转换。

一元运算符+ 是显式强制类型转换，而～是先转换为32位数字，然后按位反转。|| 和&& 更应该算是选择器运算符，其返回值不一定是布尔值，而是两个操作数其中的一个值。一般先对第一个操作数进行toBoolean强制类型转换，然后再执行条件判断。例如：a||b 理解成a？a：b 更通俗。对&& 而言，如果第一个是真值，则把第二个作为返回值，a&&b 理解成a?b:a 。

== 和=== 都会对操作数进行类型检查，并执行隐性类型转换，需要注意的是： 
- 
如果两边的值中有true或false，千万不要使用==

- 
如果两边有[],””或者0，尽量不要使用==


这里是Github上关于各种相等性的矩阵： 

![](https://img-blog.csdn.net/20170404085755951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### 语句与表达式

操作符与变量／常量等连接形成了语句和表达式，例如表达式a+1中的null 被强制转换为0。 语句包括声明与块，控制语句有判断，循环，break，continue，return，异常等。每个语句都有一个结果值，哪怕是undefined。

正则表达式是非常重要的一类表达式，主要使用RegExp类，执行方法test效率高，exec 会得到一个结果对象的数组。

逗号运算符可以把多个独立的表达式串联成一个语句，{ }在不同情况下的意思不尽相同，作为语句块，{ ..} 和for／while循环以及if条件语句中代码块的作用基本相同。{a,b} 实际上是{a:a,b:b}的简化版本。

try..catch..finally 中，如果finally中抛出异常，函数会在此处终止。需要注意的是，如果此前try中已经有return设置了返回值，则该值会被丢弃。finally中的return也会覆盖try和catch中的return的返回值。

#### 函数与作用域

函数就是具有运算逻辑的对象，匿名函数不利于调试，回调函数是一种控制反转。所有的函数（对象）都具有名为prototype的属性，prototype属性引用的对象是prototype对象；所有的对象都含有一个隐式链接，用以指向在对象生成过程中所使用的构造函数的prototype对象。

匿名函数没有name 标识符，具有如下缺陷： 
- 
代码更难理解

- 
调试栈更难追踪

- 
自我引用（递归，事件（解除）绑定，等）更难


如果function是声明的第一个词，那就是函数声明，否则就是函数表达式。立即执行函数表达式形如：`（function …)( )`

时空密不可分，作用域是时空连接的纽带之一。作用域包括全局，函数，块级作用域。作用域是根据名称查找变量的一套规则，遍历嵌套作用域链的规则简单：引擎从当前执行作用域逐级向上查找。闭包可以理解为具有状态的函数。

函数作用域指属于这个函数的全部变量都可以在整个函数的范围内使用或复用。块作用域形如 with， try／catch， ES6 引入了let，const等。

动态作用域并不关心函数和作用域是如何声明以及在何处声明的，只关心它们从何处调用的。词法作用域是定义在词法分析阶段的作用域，词法作用域查找会在第一个匹配的标识符时停止。作用域链是基于调用栈的，而不是代码中的作用域嵌套。ReferenceError 是与作用域判别失败相关，而TypeError则是作用域判别成功，但是对结果的操作非法或不合理。

this 提供了一种优雅方式来隐式“传递”一个对象引用。 this 即没有指向函数的自身，也没有指向函数的作用域，是在函数被调用时发生的绑定，它指向什么完全取决于函数在哪里被调用。如果分析this绑定的话，可以使用调试工具得到调用栈，然后找到栈中的第二个元素，就是真正的调用位置。

this 的绑定规则有：
- 
默认绑定：独立的函数调用，严格模式不能将全局对象用于默认绑定

- 
隐式绑定：把函数调用中的this 绑定到函数引用中的上下文对象

- 
显式绑定：通过call（）和apply（）方法可以直接指定this的绑定对象。其中，硬绑定是一种显式的强制绑定，ES5中提供了内置方法Function.prototype.bind, API中调用的上下文和bind的作用一样。

- 
new 绑定，构造函数只是一些使用new操作符调用的函， 使用new 来调用函数的操作过程大致如下:

- 
创建一个全新的对象

- 
这个新对象会被执行[[Prototype]]链接

- 
这个新对象会绑定到函数调用的this

- 
如果函数没有返回其他对象，那么new表达式中的函数调用会自动返回这个新对象


如果同时存在多种绑定，那么绑定的优先级大致如下：
- 
由new调用绑定到新创建的对象

- 
由call 或者apply（或bind）调用绑定到指定的对象

- 
由上下文对象调用绑定到那个上下文对象

- 
默认在在严格模式下绑定到undefined，否则绑定到全局对象


更安全地使用this 绑定的做法是传入一个特殊的对象，把this 绑定到这个对象。需要注意的是，箭头函数不使用this的4种规则，而是根据外层（函数或全局）作用域来决定this。

还要注意一点，eval 和 with 会导致作用域变化而引起性能下降，尽量不要使用。eval（） 函数中的字符串是代码，用来执行动态创建的代码，严格模式有自己的作用域，还存在安全隐患；with 是重复引用一个对象中的多个属性的快捷方式，通过将一个对象的引用当作作用域来处理，会改变作用域范围。

#### 处理和执行方式

JavaScript引擎本身没有时间概念，只是一个按需执行任意代码片段的环境，事件调度总是由包含它的宿主环境来执行。一旦有事件需要运行，事件循环队列就会运行，直到队列清空，用户交互、IO和定时器等事件源会向事件队列加入事件。

由于JavaScript的单线程特性，很多函数的代码具有原子性。

回调函数封装了程序的延续性，常见设计是分离回调（一个用于成功通知，一个用于出错通知）。另一种回调模式是“error-first”，可能受到防御式编程的影响，NodeJS API 采用了此类的风格，如果成功的话，这个参数就会被清空。需要注意的是，回调函数的嵌套往往称为回调地狱。

Deferred是一种将异步处理串联书写并执行的机制，Deferred对象是一种具有unresolved,resolved,rejected 中某一种状态的对象。Deferred内部机制是先注册回调函数，Deferred对象状态发生变化时执行该函数，是一种提高代码可读性的机制。

Deferred对象的状态迁移只能发生一次，以then(),done(),fail(),always(),pipe()指定后续函数的方法，通过when（）来并行处理，将Deferred 对象中的一部分方法删除后得到是Promise对象，对状态的管理由最初创建该Deferred对象的所有者来执行。

Promise 封装了依赖于时间的状态，从而使得本身与时间无关，Promise 可以按照可预测的方式进行，而不用关心时序或底层的结果。一旦Promise决议完成，就成为了不变值，可以安全地吧这个值传递给第三方，并确保不会改变。

Promise 是一种在异步任务中作为两个或更多步骤的流程控制机制，时序上的this-then-that。 不仅表达了多步异步序列的流程控制，还是一个从一个步骤到下一个步骤传递消息的消息通道。事件监听对象可以当成是对promise 的一种模拟，对控制反转的恢复实现了更好的关注点分离。

判断是否是Promise 值的示例代码如下：

```
if（
    p !==null &&
    ( typeof p ===“object” || typeof p ===“function”) && typeof p.then===“function”)
    {
        console.log(“thenable”);
    }
else{
    console.log(“not thenable”);
}
```

生成器是一类特殊的函数，可以一次或多次启动和停止，并不非的一定要完成，生成器把while true 带回了Javascript的世界。其中，yield 委托的主要目的是代码组织，以达到与普通函数调用的对称。从生成器yield出一个Promise， 并且让这个Promise 通过一个辅助函数恢复这个生成器，这是通过生成器管理异步的好方法之一。 

需要注意的是，如果在Promise.all([..]) 中传入空数组，会立即完成， 而Promise.race([..]) 则会挂住。 在各种Promise库中，finally （ .. ) 还是会创建并返回一个新Promise的。

#### 模块与库

模块和库是JavaScript 时空中的另一纽带，提高了代码的复用性和开发效率。

模块充分利用了闭包的强大能力，从模块中返回一个实际的对象并不是必须的，也可以直接返回一个内部函数，例如：jQauery 和 $标识符就是jQuery 模块的公共API。

模块有两个必要条件： 
- 
必须有外部的封闭函数，该函数必须至少被调用一次 

- 
封闭函数必须返回至少一个内部函数，这样内部函数才能在私有作用域中形成闭包，并且可以访问或修改私有的状态



import 可以将一个模块的一个或多个API导入到当前作用域中，并分别绑定在一个变量上；module 则将整个模块的API 导入并绑定到一个变量上， export 将当前模块的一个标识符导出为公共API。

大多数模块所依赖的加载器／管理器本质上是将这种模块定义封装进一个API。基于函数的模块并不是一个能被静态识别的模式（编译器），API定义只有在运行时考虑进来。但是ES6 模块的API 是静态的，必须被定义在独立的文件中。

JavaScript 中的库浩如烟海，这里仅对JQuery做简要说明。JQuery压缩后大约31k，轻巧灵活，通过链式语法实现逻辑功能，通过CSS3选择器及自定义选择器获取元素，支持插件，可扩展性高。

JQuery中 的特色函数——$ ，可以抽取与选择器匹配的元素，或者创建新的DOM元素，将已有的DOM元素转换为jQuery对象，对DOM构造完成后的事件监听器进行设定等等。JQuery 对DOM，样式，AJAX 均可有效处理。

通过扩展JQuery.fn 就可以创建JQuery的插件，code.google.com/apis/libraries 给出了很多JQuery 的插件信息。

利用JavaScript 的时空观，可以对这一语言有一些基本的梳理。就语言本身而言，关键字是不能回避的，对JavaScript 关键字，在StackOverFlow中有人给出了如下诗一样的总结：

```
Let this long package float，
Goto private class if short。
While protected with debug case，
Continue volatile interface。
Instanceof super synchronized throw，
Extends final export throws.

Try import double enum?
-False, boolean, abstract function.
Implements typeof transient break!
Void static,default do,
Switch int native new,
else, delete null public var,
In return for const, true, char,
…… finally catch byte.
```

## 客户端应用

一门语言所被使用的广泛程度取决于使用的场景，正如PHP被广泛采用那样，互联网应用不仅是JavaScript 的家乡，而且是它大展身手的最重要场所，没有JavaScript 的Web应用几乎绝迹了。

web应用中使用JavaScript有拖拽操作，异步读取，键盘访问 和动画效果等基本功能。对于清晰地使用JavaScript实现Web应用而言，理解浏览器网页处理过程是必要的。一般地，浏览器先分析HTML，然后构造DOM树，再载入外部Javascript 文件以及CSS文件，接下来载入图像文件等外部资源，最后在分析Javascript后开始执行至全部完成。

在HTML中加载JavaScript的方式有多种：
- 
<script> 标签，在body 结束标签前写

- 
读取外部JavaScript 文件，读取完就开始执行，浏览器可以缓存

- 
onload 事件加载

- 
DOMContentLoaded是在完成HTML解析后发生的事件，也可以用于加载JavaScript

- 
动态载入，这样JS在载入时不会阻断其他操作,如

var script = document.createElement(‘script’);

script.src = ‘my-javascript.js’；

document.getElementsByTagName(‘head’)[0].appendChild(script)


window对象是JavaScript所能操作的最高层对象，其中的属性包括navigator，location，history，screen，frames，document，parent，top，self 等。

DOM 是一种API，完成对HTML／XML 的树形结构访问，如标签，元素，节点等。节点可以通过ID，标签名，名称和类名进行检索，例如：

```
var element = document.getElementById(“abel”)
var allelements = document.getElementByTagName(‘*’)
```

由于返回的是NodeList对象，性能较差，可以通过 `var array = Array.prototye.slice.call(allelements)`转换为array 后处理。节点的访问可以通过XPath 进行灵活的访问，当然，Selector API 比XPath更简单且同样灵活，例如：

```
var a_label = document.querySelector(‘#abel’)
var b_all = document.querySelectorAll(‘div’)
```

如果先修改DocumentFragment，再对实际的document对象操作，DOM 的操作性能会较高一些。

事件侦听器的设定可以制定HTML元素的属性，也可以指定DOM元素的属性，还可以通过EventTarget.addEventListenser()进行指定。事件的处理包括捕获，目标处理和事件冒泡三个阶段，捕获的过程是：
` window -> document -> html -> body -> div -> button  `
然后处理器执行,冒泡向上传播的过程是遍历DOM树，需要注意的是 focus 不会冒泡。

DOM2中的标准事件有HTMLEvent，MouseEvent，UIEvent和MutationEvent。DOM3 中的事件更多：UIEvent，FocusEvent，MouseEvent， WheelEvent， TextEvent，KeyboardEvent 和compositionEvent等，还可以通document.createEvent来自定义事件。

通过JavaScript 对CSS样式变更的方法有通过className 属性变更class名，通过classList属性更改class名（其中classList 是H5对DOM TokenList接口的实现），还可以更改Style 属性或者直接更改样式表。通过JavaScript可以对屏幕位置（screenX，screenY），窗口位置（clientX，clientY），文档坐标（pageX，pageY，由浏览器自行实现的），特定元素内的相对位置（layerX，layerY 或offsetX offsetY）进行修改。通过JavaScript可以对表单中的元素，控件和内容进行验证，可用于验证的事件有submit，focus，blur，change，keydown/up/press，input。使用表单而不产生页面跳转的方式可以是指向到一个 （0,0 ）的空iframe。

对于动画而言，css的动画性能一般要更好一些。

AJAX 在Web应用中是不可或缺的，简单地说，是一种不发生页面跳转就能异步载入内容并改写页面内容的技术，主要通过 XMLHttpRequest 对象的创建，实现通／异步通信，处理超时和响应。AJAX有着跨源限制，实现跨源通信的方式有：JSONP，iframe hack，window.postMessage() 以及 XMLHttpRequest Level 2。

HTML5+CSS3+JavaScript的综合使用才可能成就一个Web应用。H5中的 History API 使用了window属性的history对象监听popstate事件，用于恢复页面状态的处理。ApplicationCache 在html标签的manifest 属性中指定了缓存清单文件的路径，必须通过text/cache-manifest 这一MIME type 来发布缓存清单文件，注意清单中的CACHE，NETWORK，和FALLBACK 的区分。

通过navigator.onLine 可以获知网络状态，还可以通过online/offline事件来侦听连接状态的切换时机。online/offline事件是document.body 触发的，并传给document对象和window对象。

```
<p> network is : <span id = “indicator”> (state unknown) </span> </p>
<script>
{
    function updateIndicator = document.getElementById(‘indicator’);
    indicator.textContext = navigator.online?’online’:’offline’;
}
document.body.onload = updateIndicator;
document.body.ononline= updateIndicator;
document.body.onoffline = updateIndicator;
</script>
```

DataTransfer 是Drag Drop API 的核心，在所有拖拽事件的事件对象中，都有该属性，主要是接收数据。拖拽文件从浏览器保存到桌面：`event.dataTransfer.setData(‘DownloadURL’,’MIMETYPE: 文件url’）`例如：

```
<a href=“http://a.b.c/abel.pdf” 
    data-downloadurl = “application/pdf:abel.pdf:http://a.b.c/abel.pdf”
    class=“dragout” draggable = “true”>download </a>
    <script>
    var files = document.querySelectorAll(‘.dragout’);
    for (var i = 0,file; file =files[i];i++) {
        file.addEventListener(‘dragstart’,function(event){
    event.dataTransfer.setData(“DownloadURL”,this.dataset.downloadurl);
    },false);
    }
    </script>
```

FileAPI 通过FileReader 读取文件，也可以读取dataURL，FileReaderSync 用于同步读取文件内容，可以在Web Worker 中使用。

Web Storage 为所有源共享5M空间，localStorage 和sessionStorage 的区别在于数据的生命周期。cookie 最大4k，发请求时一起发送，保存会话等重要信息。indexedDB 可以归为文档型数据库, 作为客户端存储又一选择。

`var indexdb = window.indexDB||window.webkitIndexedDB||window.mozIndexedDB;`

Web worker 是H5 的新特性，是宿主环境（浏览器）的功能，JavaScript 本身是不支持多线程的。专用的worker 与创建它的程序之间是一对一的关系。

Web worker 能在另外的线程中创建新的Javascript 运行环境，使JavaScripts可以在后台处理。主线程和工作线程分离，无法使用对方环境的变量。工作线程无法引用document对象，需要通过消息收发完成数据传递。 在主线程创建工作线程，大约向`var worker = new Worker(‘work.js’)这样` 在主线程中停止worker的方式是`worker.terminate();` worker 自身停止的方式是 `self.close();`worker 中 可以通个 importScripts 方法，在工作线程内读取外部的文件。

了解了这些基础方式和方法，仅仅是Web应用中JavaScript开发的第一步吧。

## 服务端应用

技术系统总是又着向超系统进化的趋势，JavaScript 也不例外。

JavaScript 应用于服务端的开发源于2009年初出现的CommonJS，后来成为为了服务器端javaScript的规范。基于JavaScript没有模块系统、标准库较少、缺乏包管理工具等现状，CommonJS规范希望JavaScript可以在任何地方运行，以达到Java、C#、PHP这些后台语言具备开发大型应用的能力。CommonJS是一种思想，它的终极目标是使应用程序开发者根据CommonJS API编写的JavaScript应用可以在不同的JavaScript解析器和HOST环境上运行，例如编写服务端应用，命令行工具，基于GUI的桌面应用和混合应用编程等，详情参加 www.commonjs.org 。

NodeJS可以理解成CommonJS规范的一种实现，而且是部分实现。NodeJS以V8作为JavaScript的实现引擎，通用的异步处理事件循环，提供了一系列非阻塞函数库来支持实践循环特性。同时，NodeJS提供了高度优化的应用库，来提高服务器效率，例如其http 模块是为快速非阻塞式http服务而用C语言重写的。另外，NodeJS还有shell的命令行工具，通过包系统实现扩展，扩展列表可以详情参见： GitHub.com/node/wiki/modules。

JavaScript 中的主要实现引擎包括：IE采用的JScript，Firefox采用的SpiderMoneky，Chrome 采用的V8，Safari采用的webkit中的 javacriptcore灯。如果要对引擎有进一步的了解，可以研读一下javascriptcore等相关的源代码。

V8 是NodeJS 中的核心引擎，NodeJS的系统架构大致如下：

![](https://img-blog.csdn.net/20170404090606752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


与浏览器相对应，Node 中的全局变量可以通过 `Object.keys(global);` 获得， 看一看NodeJS中的 “hello world” 程序：

```
var http = require('http');
http.createServer(function (req,res){
    res.writeHead(200,{'Content-type':'text/plain'});
    res.end('Hello Node.js \n');
}).listen(1234,"127.0.0.1");
console.log('Server running on http://127.0.0.1:1234/');
```

几行代码就实现一个简单web server， 使Pythoner 们联想到了 Tornado， 它们都走在单线程异步IO的路上。

NodeJS 提供了对https 的支持，可以通过openssl 生成证书的方式大致是：
`openssl req  -new -x509 -keyout key.pen -out cert.perm`
使用证书的示例如下：

```
var fs  =require(‘fs’);
var options = {
    key: fs.readFileSync(‘key.perm’);
    cert:fs.readFileSync(‘cert.perm’);
}
```

NodeJS支持socket 和文件处理，配合系统扩展可以使用各种模版语言。基于NodeJS的实际在业界非常广泛，比如面向websocket的IM系统，各种web应用网站等等。

鉴于微服务架构的兴起，也诞生了基于Node的微服务架构——Seneca，它使用完备的模式匹配接口来连接各个服务，从代码中将数据传输抽象出来，使编写具有高扩展性的软件变得相当容易。Seneca 没有使用依赖注入，但是在处理控制反转上相当灵活，没有关键字和强制的字段，只需一组键值对，用于模式匹配的引擎中。具体参考实现，可以参考《node.js 微服务》一书。

## 基于JavaScript的全栈

如果在整个应用系统中主要使用JavaScript编程语言作为技术栈，那么也可以成为基于JavaScript 的全栈，关于全栈的论述可以参加《全栈的技术栈设想》和《再谈< 全栈架构师>》两篇文字。例如MEAN架构，即MongoDB + Express + Angular + Node，MEAN 技术栈代表着一种完全现代的 Web 开发方法：一种语言运行在应用程序的所有层次上，从客户端到服务器，再到持久层。借助JavaScript的测试框架，比如MochaJS、JasmineJS 和 KarmaJS，可以为自己的 MEAN 应用程序编写深入而又全面的测试套件，据说MEAN有取代LAMP／LNMP的的趋势，但还需保持谨慎。

#### 引擎的差异

正像Java 那样，尽管又着虚拟机规范，但各个JVM的实现还是有着些许的不同，JavaScript 也是如此。JavaScript各引擎中同样存在着少量的限制，例如：
- 
字符串常量中允许的最大字符数

- 
作为参数传递到函数中的数据大小（栈大小）

- 
函数声明中的参数个数

- 
函数调用链的最大长度

- 
以阻塞方式在浏览器中运行的最大时间

- 
变量名的最大长度


尽管如此，JavaScript 在浏览器中的表现还是基本上可信的。

#### 从软件到硬件

实际上，JavaScript已经嵌入到了从机器人到各种家电等各种各样的设备中。这里隆重推荐我非常敬佩的好友——周爱民老师，他在Ruff（南潮信息科技）做的事情就是JavaScript 在物联网上的进一步应用。

Ruff 是一个可以让开发者实现敏捷开发智能硬件的系统平台。它包含了Ruff SDK、Ruff OS，Rap Registry等。从技术上讲，Ruff 是一个 JavaScript 运行时，专为硬件开发而设计。Ruff 对硬件进行了抽象，使用了基于事件驱动、异步 I/O 的模型，使硬件开发变得轻量而且高效。硬件抽象层，使得操作硬件犹如普通程序库，降低了硬件领域进入门槛。

Ruff 为开发者提供了完善的开发服务。从项目生产、软件包管理、应用管理、外设管理到固件管理等一系列现代软件开发方式，PC 端完成开发，无需烧板子，提升开发者的开发效率。Ruff 还提供了完善的测试框架，支持 assert、test、mock 等模块，在开发机上测试逻辑，硬件测试也能 TDD。

官网（ruff.io) 上给出的示例如下：

```
$.ready(function() {
   $('#led-0').turnOn();
});
```

打开电路板上的一个LED 灯，就是如此的简单。目前，一个 Ruff 硬件同时只能运行一款 Ruff 应用，它将拥有自己独立的进程，着可能也受到JavaScript自身的限制吧。

#### 关注性能

性能是全栈关注的一个重要维度，那句“过早优化是万恶之源”实际上是我们对高德纳先生的断章取义，原文是这样的：

```
我们应该在例如97%的时间里，忘掉小处的效率；
过早优化是万恶之源。
但我们不应该错过关键的3%中的机会。
```

实际上是非关键路径上的优化是万恶之源，问题在于如何确定我们的代码是否在关键路径上。不论节省的时间多么少，花费在关键路径上的性能优化都是值得的。

对于性能优化工具，用于JavaScript源代码压缩有 google Closure complier，packer，YUI compressor，JSmin等。页面的性能优化工具有YSlow 和Page Speed等。实际上，任何有意义且可靠的性能测试都是基于统计学上的合理实践。 就JavaScript 代码本身的性能而言，benchmarkjs 是一个很好的工具，而jsperf.com 提供了对JavaScript 执行环境的性能测试。

总之，JavaScript 是一个具有强大生命力的语言，前端框架更是日新月异，从Angular，Vue，到React， 乃至React Native，给人以目不暇接的感觉，但是，老码农觉得基础认识还是非常必要的，勿在浮沙筑高塔。

### 参考阅读

https://developer.mozilla.org/en-US/docs/Web/JavaScript 

https://tc39.github.io/ecma262/#sec-global-object













![](https://mp.weixin.qq.com/mp/qrcode?scene=10000004&size=102&__biz=MzAwOTcyNzA0OQ==&mid=2658971386&idx=1&sn=d5b529569c01a1f43b9629bea10542af&send_time=)
微信扫一扫
关注该公众号






