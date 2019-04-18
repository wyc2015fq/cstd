# 你真的会JavaScript吗 - weixin_33985507的博客 - CSDN博客
2007年05月18日 15:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

很久没有看到这样让人唇齿留香的好文了。上次看到的是一篇是 Douglas Crockford 的[JavaScript, We Hardly new Ya](http://yuiblog.com/blog/2006/11/13/javascript-we-hardly-new-ya/) (我简单翻译了一下，译文在后)。
同其他教你如何用面向对象的思想编写JavaScript的其他文章一样，该文也是着重在这么几个要素：
- JavaScript的对象就是一个关联数组。
- JavaScript 函数也是一个对象。
- 原型(Prototype)
- 闭包(Closures)
- 继承/私有属性/静态方法
- 命名空间
作者文笔很好，英文很容易看懂，没有生僻的用词(顺便提一下，《PPK on JavaScript》作者的英文不敢恭维)。用来举例的代码也很贴切。
特别是文章的开头很有意思，作者写到他和一个据说已经写了快4年JavaScript的女程序员聊天，女程序员认为她的JS水平very good，后来作者发现她确实会写，但仅仅是会写，其实对JavaScript的内涵所知甚少。
作者想用这个例子说明，有很多具备Java/C++/C#开发经验的开发人员在编写JavaScript或者转行到FED(比如我)的时候，想当然的把那些标准面向对象语言的思想套用在JavaScript上，反而走入迷途。
对此我深有体会，我正是在真正参与了一次Ajax的项目并真正读懂了Prototype框架的源码之后，对JavaScript有了完全全新的认识。
总之，推荐阅读。附上[JavaScript, We Hardly new Ya](http://yuiblog.com/blog/2006/11/13/javascript-we-hardly-new-ya/)的译文，译得匆忙，定有行文不通之处，请客官见谅！
## JavaScript 的 new， 好久不见啊
*原文： *[*JavaScript, We Hardly new Ya*](http://yuiblog.com/blog/2006/11/13/javascript-we-hardly-new-ya/)*  －－**Douglas Crockford。 *
JavaScript是一门基于原型的语言，但它却拥有一个 new 操作符使得其看起来象一门经典的面对对象语言。那样也迷惑了程序员们，导致一些有问题的编程模式。
其实你永远不需要在JavaScript使用 new Object()。用字面量的形式{}去取代吧。
同理，不要使用 new Array() ，而代之以字面量[]。JavaScript中的数组并不象Java中的数组那样工作的，使用类似Java的语法只会让你糊涂。
同理不用使用 new Number, new String, 或者 new Boolean。这些的用法只会产生无用的类型封装对象。就直接使用简单的字面量吧。
不要使用 new Function 去创建函数对象。用函数表达式更好。比如：
frames[0].onfocus = new Function(”document.bgColor=’antiquewhite’”)
更好的写法是：
frames[0].onfocus = function () {document.bgColor = ‘antiquewhite’;};
第二种形式让脚本编译器更快的看到函数主体，于是其中的语法错误也会更快被检测出来。有时候程序员使用 new Function 是因为他们没有理解内部函数是如何工作的。
selObj.onchange = new Function(”dynamicOptionListObjects[”+
        dol.index+”].change(this)”);
如果我们让用字符串做函数体，编译器不能看到它们。如果我们用字符串表达式做函数体，我们同样也看不到它们。更好的方式就是不要盲目编程。通过制造一个返回值为函数的函数调用，我们可以明确的按值传递我们想要绑定的值。这允许我们在循环中初始化一系列 selObj 对象。
selObj.onchange = function (i) {
    return function () {
        dynamicOptionListObjects[i].change(this);
    };
}(dol.index);
直接对一个函数使用new永远不是一个好主意。比如， new function 对构造新对象没有提供什么优势。
myObj = new function () {
    this.type = ‘core’;
};
更好的方式是使用对象字面量，它更轻巧，更快捷。
myObj = {
    type: ‘core’
};
假如我们需要创建的对象包含的方法需要访问私有变量或者函数，更好的方式仍然是避免使用new.var foo = new function() {
    function processMessages(message) {
        alert(”Message: ” + message.content);
    }
    this.init = function() {
        subscribe(”/mytopic”, this, processMessages);
    }
}通过使用 new 去调用函数，对象会持有一个无意义的原型对象。这只会浪费内存而不会带来任何好处。如果我们不使用new，我们就不用在对象链维护一个无用的prototype对象。所以我们可以用（）来正确的调用工厂函数。var foo = function () {
    function processMessages(message) {
        alert(”Message: ” + message.content);
    }
    return {
        init: function () {
            subscribe(”/mytopic”, this, processMessages);
        }
    };
}();所以原则很简单： 唯一应该要用到new操作符的地方就是调用一个古老的构造器函数的时候。当调用一个构造器函数的时候，是强制要求使用new的。有时候可以来new一下, 有的时候还是不要了吧。 
#### 2 Responses to “你真的会写JavaScript吗？”
[Feed for this Entry](http://www.uiplanet.com/taobao/2007/05/15/%E4%BD%A0%E7%9C%9F%E7%9A%84%E4%BC%9A%E5%86%99javascript%E5%90%97%EF%BC%9F/feed/)
- [1](http://www.uiplanet.com/taobao/2007/05/15/%E4%BD%A0%E7%9C%9F%E7%9A%84%E4%BC%9A%E5%86%99javascript%E5%90%97%EF%BC%9F/#comment-2387)[coocboy](http://coocboy.myrice.com/)[May 15th, 2007 at 11:16](http://www.uiplanet.com/taobao/2007/05/15/%E4%BD%A0%E7%9C%9F%E7%9A%84%E4%BC%9A%E5%86%99javascript%E5%90%97%EF%BC%9F/#comment-2387)
so fast 
- [2](http://www.uiplanet.com/taobao/2007/05/15/%E4%BD%A0%E7%9C%9F%E7%9A%84%E4%BC%9A%E5%86%99javascript%E5%90%97%EF%BC%9F/#comment-2388)lenel[May 15th, 2007 at 12:02](http://www.uiplanet.com/taobao/2007/05/15/%E4%BD%A0%E7%9C%9F%E7%9A%84%E4%BC%9A%E5%86%99javascript%E5%90%97%EF%BC%9F/#comment-2388)
对于javaScript的Prototype我理解是一颗描述继承树的链子
使用javaScript进行OO开发是完全可以的
和java，C 等对比，有一点做不到：
对于父类的成员方法（非构造方法）要么完全重写，要么原封不动，也就是无法在子类的成员方法中调用父类的同名方法
其他的重写、重载、构造、接口、继承、多态都是没问题的
当然脚本语言没有编译器就无法进行比如做接口实现是否完整等检查
靠程序员自己控制好吧 
