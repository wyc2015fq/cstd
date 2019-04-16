# JavaScript核心指南(JavaScript. The Core.) - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月14日 22:33:00[慢游](https://me.csdn.net/eengel)阅读数：3770







好文！不得不看。对了解JavaScript的核心原理非常有帮助。

此处转载为该文的中文翻译版本：[http://remember2015.info/blog/?p=141](http://remember2015.info/blog/?p=141)

英文原文链接：

原文
：[http://dmitrysoshnikov.com/ecmascript/javascript-the-core/](http://dmitrysoshnikov.com/ecmascript/javascript-the-core/trackback/)

原作者
：[Dmitry A. 
Soshnikov](http://dmitrysoshnikov.com/)
-------------------------------------------------------------------------------

## 零、索引
- [对象(An Object)](#an-object)
- [原型链(A Prototype Chain)](#a-prototype-chain)
- [构造函数(Constructor)](#constructor)
- [执行上下文栈(Execution Context Stack)](#execution-context-stack)
- [执行上下文(Execution Context)](#execution-context)
- [变量对象(Variable Object)](#variable-object)
- [激活对象(Activation Object)](#activation-object)
- [作用域链(Scope Chain)](#scope-chain)
- [闭包(Closures)](#closures)
- [this指针(This Value)](#this-value)
- [总结(Conclusion)](#conclusion)

本文是对“[ECMA-262-3 in 
detail](http://dmitrysoshnikov.com/tag/ecma-262-3/)
”系列学习内容的概述与总结。如果你对ES3
系列文章感兴趣，本文每一节内容均包含相应ES3
系列章节的链接，以供阅读与获取更深入的解释。

本文预期读者：有经验的程序员，专业人士

让我们首先来考虑ECMAScript的基础——* 对象(object) *
的概念。

## 一、对象(An Object)

ECMAScript是一门高度抽象的面向对象(object-oriented)语言。虽然ECMAScript包含 *基本类型(primitives) *
, 但是在需要的时候它们都会被转换为对象.

> 
对象是一个属性的集合(*collection of properties*
)，同时具有单一原型对象引用(*single prototype object*
)。原型可以指向一个对象，也可以为 *null*
。


让我们来看一个基本的对象Foo。首先要说明的是：一个对象的原型会使用内部属性[[Prototype]]表示引用。在下图中我们将使用 
“__<内部属性>__”来标记内部属性，比如以 *__proto__ 来表示原型属性*
(这是某些脚本引擎比如SpiderMonkey的对于原型概念的具体实现，尽管并非标准).

代码如下：



|`1`|`var``foo = { `|
|----|----|


|`2`|`    ````x: 10,```|
|----|----|


|`3`|`    ````y: 20```|
|----|----|


|`4`|`};`|
|----|----|




foo对象含有两个明确的私有属性（x与y）和一个隐含的 __proto__ 属性(指向foo的原型)：

![图1.一个含有原型的基本对象](http://dmitrysoshnikov.com/wp-content/uploads/basic-object.png)

图 1. 一个含有原型的基本对象

为什么需要原型呢，让我们考虑 *原型链*
 的概念来回答这个问题.

## 二、原型链(A Prototype Chain)

原型对象也只是简单的对象，并可能有自己的原型。如果一个原型有一个非空引用指向它自己的原型，它自己的原型也有非空引用指向自己的原型……诸如此类，这就是所谓的 
*原型链*
。

> 
原型链是由 *有限(finite) *
的对象链接而成，可以 实现 *继承(inheritance)*
 与 共享 
*属性*
.


考虑下面这样一种情况：假设我们有两个差别很小的对象，那么在一个设计良好的系统中，显而易见地，我们将会 *复用(reuse) *
那些相似的功能/代码而不是在每个对象中重复实现。在基于类的系统中，这个 *代码重用(*code reuse*)*
 
风格称为*基类继承(*class-based inheritance*) *
— 
你在A类中放入公共的功能(共性)，提供给继承于A类的B和C类，同时B和C类保持自身的微小差别。

ECMAScript没有类的概念。但是代码复用的理念类似 (甚至在某些方面比基于类的语言更加灵活) 并通过原型链实现。这种类型的继承被称为 
*基于原型的继承(prototype based inheritance*
)。

类似于类”A”，”B”，”C”，在ECMAScript中尼创建对象类”a”，”b”，”c”，相应地， 对象“a” 
拥有对象“b”和”c”的共同部分。同时对象“b”和”c”只包含它们自己的附加属性或方法。



|`01`|`var``a = { `|
|----|----|


|`02`|`    ````x: 10,```|
|----|----|


|`03`|`    ``calculate: ``function````(z) {```|
|----|----|


|`04`|`        ``return``this``.x + ``this````.y + z```|
|----|----|


|`05`|`    ````}```|
|----|----|


|`06`|```};```|
|----|----|


|`07`|` `|
|----|----|


|`08`|`var``b = { `|
|----|----|


|`09`|`    ````y: 20,```|
|----|----|


|`10`|`    ``__proto__: a `|
|----|----|


|`11`|```};```|
|----|----|


|`12`|` `|
|----|----|


|`13`|`var``c = { `|
|----|----|


|`14`|`    ````y: 30,```|
|----|----|


|`15`|`    ``__proto__: a `|
|----|----|


|`16`|```};```|
|----|----|


|`17`|` `|
|----|----|


|`18`|```// 调用继承的方法```|
|----|----|


|`19`|` `|
|----|----|


|`20`|`b.calculate(30); ``// 60 `|
|----|----|


|`21`|` `|
|----|----|


|`22`|`c.calculate(40); ````// 80```|
|----|----|




很容易，不是吗？ 
可以看到，对象“b”和“c”访问了定义于”a”对象中的”calculate”方法。而做到这一点正是通过原型链。规则很简单：如果在对象本身中没有找到某个属性或方法（即对象没有此私有属性），那么就尝试在原型链中查找此对象或方法。如果在原型中找不到此属性，就查找原型的原型并以此类推直到查找完整个原型链(在基于类的继承语言系统中，如果要访问一个继承的方法，实现原理完全相同，不同之处在于查找的是 
*类型链(*c*lass chain) *
)。 第一个被找到的具有同样名字的方法/属性会被访问，并被称为 
*继承属性(inherited*
 property)。如果在整个原型链中找不到此方法/属性，则返回 *undefined*
.

请注意，*this *
指针在继承的方法中指向原有对象，而非原型对象。在上面的例子中 “this.y” 取值于“b” and 
“c”，而不是“a”。但是“this.x”来自于“a”。

如果一个对象没有明确指定原型，那“__proto__”的默认值为*Object.prototype*
。对象 
“Object.prototype” 自身也有一个“__proto__”，它是整个原型链的 *最后一环(*f*inal link)*
 
并且值为 *null*
.

下图显示了对象“a”，“b”和“c”的对象继承层次结构：

![Figure 2. A prototype chain.](http://dmitrysoshnikov.com/wp-content/uploads/prototype-chain.png)

图 2. 原型链

原型链通常将会在这样的情况下使用：对象拥有 *相同或相似的状态结构(*s*ame or similar state structure)*
 （即相同的属性集合）与 *不同的状态值(different state values)*
。在这种情况下，我们可以使用 *构造函数(Constructor)*
 在 
*特定模式*
(*specified pattern) *
下创建对象。

## 三、构造函数(Constructor)

除了创建对象，*构造函数(constructor) *
还做了另一件有用的事情—自动为创建的对象设置了* 原型对象(prototype object) 。*
原型对象存放于 *ConstructorFunction.prototype*
 属性中.

例如，我们重写之前例子，使用构造函数创建对象“b”和“c”，那么对象”a”则扮演了“Foo.prototype”这个角色：



|`01`|```// 一个构造函数```|
|----|----|


|`02`|`function``Foo(y) { `|
|----|----|


|`03`|`  ````//这个构造函数将会以特定模式创建对象：被创建的对象都会有"y"属性```|
|----|----|


|`04`|`  ``this````.y = y;```|
|----|----|


|`05`|```}```|
|----|----|


|`06`|` `|
|----|----|


|`07`|```// "Foo.prototype"存放了新创建对象的原型的引用```|
|----|----|


|`08`|```// 所以我们可以将之用于定义继承属性/方法```|
|----|----|


|`09`|` `|
|----|----|


|`10`|```// 继承属性"x"```|
|----|----|


|`11`|`Foo.``prototype````.x = 10;```|
|----|----|


|`12`|` `|
|----|----|


|`13`|```// 继承方法"calculate"```|
|----|----|


|`14`|`Foo.``prototype````.calculate =````function``(z) { `|
|----|----|


|`15`|`  ``this``.x + ``this````.y + z;```|
|----|----|


|`16`|```};```|
|----|----|


|`17`|` `|
|----|----|


|`18`|```// 使用模式"Foo"创建对象"b"和“c”```|
|----|----|


|`19`|`var``b = ``new``Foo(20); `|
|----|----|


|`20`|`var``c = ``new``Foo(30); `|
|----|----|


|`21`|` `|
|----|----|


|`22`|```// 调用继承方法```|
|----|----|


|`23`|`b.calculate(30); ``// 60 `|
|----|----|


|`24`|`c.calculate(40); ``// 80 `|
|----|----|


|`25`|` `|
|----|----|


|`26`|```// 让我们看看是否使用了预期的属性```|
|----|----|


|`27`|` `|
|----|----|


|`28`|`console.``log````(```|
|----|----|


|`29`|` `|
|----|----|


|`30`|`  ``b.__proto__ === Foo.``prototype````,``````// true```|
|----|----|


|`31`|`  ``c.__proto__ === Foo.``prototype````,``````// true```|
|----|----|


|`32`|` `|
|----|----|


|`33`|`  ````// "Foo.prototype"自动创建了一个特定的属性"constructor"，指向构造函数本身```|
|----|----|


|`34`|`  ````// 实例"b"和"c"可以通过授权找到它并用以检测自己的构造函数```|
|----|----|


|`35`|` `|
|----|----|


|`36`|`  ``b.``constructor``=== Foo, ````// true```|
|----|----|


|`37`|`  ``c.``constructor``=== Foo, ````// true```|
|----|----|


|`38`|`  ``Foo.``prototype``.``constructor``=== Foo ````// true```|
|----|----|


|`39`|` `|
|----|----|


|`40`|`  ``b.calculate === b.__proto__.calculate, ``// true `|
|----|----|


|`41`|`  ``b.__proto__.calculate === Foo.``prototype````.calculate``````// true```|
|----|----|


|`42`|` `|
|----|----|


|`43`|`);`|
|----|----|




上述代码可以表示如下关系：

![Figure 3. A constructor and objects relationship.](http://dmitrysoshnikov.com/wp-content/uploads/constructor-proto-chain.png)

图 3. 构造函数与对象之间的关系

上图表明每个对象都拥有原型。构造函数 “Foo” 同样有它自己的”__proto__”—— 
“Function.prototype”，“Function.prototype”的”__proto__”属性为 
“Object.prototype”。需要重复说明的是，“Foo.prototype”只是“Foo”的隐含属性，指向对象 “b”和“c”的原型。

如果去考虑 *类化(classification)*
 
的概念(我们刚刚类化了一个分离的事物——Foo)，构造函数与原型对象组合起来或许可以称之为“类”。事实上，Python的“first-class”动态类采取了完全同样的实现。从这个角度看来，Python的类相比较于ECMAScript的基于原型的继承只是Syntactic 
Suger[译注：这里可能是“语法不同”的意思].

这个问题完整和详细的解释可以在ES3系列的第七章找到。有两个部分 [7.1章 
面向对象编程.一般理论](http://dmitrysoshnikov.com/ecmascript/chapter-7-1-oop-general-theory/)
([Chapter 
7.1. OOP. The general theory](http://dmitrysoshnikov.com/ecmascript/chapter-7-1-oop-general-theory/)
)，描述了不同的面向对象的范式与风格(OOP paradigms and 
stylistics)，以及与ECMAScript的比较, [7.2章 
面向对象编程.ECMAScript实现](http://dmitrysoshnikov.com/ecmascript/chapter-7-2-oop-ecmascript-implementation/)
([Chapter 
7.2. OOP. ECMAScript implementation](http://dmitrysoshnikov.com/ecmascript/chapter-7-2-oop-ecmascript-implementation/)
), 专门讲述了ECMAScript中的面向对象编程.

现在，我们知道了基本的对象概念，让我们看看ECMAScript中 *运行时程序执行(runtime program execution) *
是怎样实现的。这被称之为 *执行上下文堆栈(execution context stack) *
， 
其中的每个元素可以抽象地表示为对象。ECMAScript中，对象的概念几乎无处不在。

## 四、执行上下文栈(Execution Context Stack)

ECMAScript代码由三种类型: *全局(global)*
 代码, *函数(function)*
 代码和 
*求值(eval)*
 
代码[译注：这里实在是没有很好的翻译，本意为字符串求值。orz…]。每一句代码都在它的上下文中被计算与执行。每一次函数调用，会进入函数的执行上下文并求出其代码类型；每一次*eval *
调用，都会进入 *eval*
 执行上下文并执行代码。

请注意，由于每次函数调用都会产生具有不同状态的上下文，所以一个函数能可能产生无限数量的上下文集合。



|`1`|`function``foo(bar) {} `|
|----|----|


|`2`|```// 调用同一个函数生成3个上下文```|
|----|----|


|`3`|```// 每一次调用产生的上下文状态不同(比如参数"b"的值)```|
|----|----|


|`4`|```foo(10);```|
|----|----|


|`5`|```foo(20);```|
|----|----|


|`6`|`foo(30);`|
|----|----|




一个执行上下文会激活其它上下文，比如一个函数调用另一个函数(或者在全局上下文中调用一个全局函数)，然后循环往复。理论上来说，这种上下文状态会被实现为一个栈，被称为 
*执行上下文栈(execution context stack)*
 。

激活其它上下文的某个上下文被称为 *调用者(caller) 。*
被激活的上下文被称为 *被调用者(callee) 。*
被调用者同时也可能是调用者(比如一个在全局上下文中被调用的函数调用某些自身的内部方法)。

当调用一方激活(调用)被调用的一方时，调用函数会暂停(挂起)当前代码的执行并将控制流交给被调用者。此时被调用一方会被压入堆栈，称为一个 
*运行时(running (active)) *
执行上下文。当被调用一方结束其运行时，它将控制权交回调用一方，然后对调用一方的上线文的求值会继续执行(可能激活其它上下文)直到其运行结束。被调用者会简单的进行 
*返回(return) *
或以 *异常(exception) *
退出。被抛出的异常如果没有捕获，将会终止/退出(从栈中弹出)一个或多个上下文。

如下所示：所有的ECMAScript *程序运行时(program runtime)*
 可以表示为 *执行上下文栈(execution context (EC) stack) *
，其栈顶是当前被激活的上下文:

![Figure 4. An execution context stack.](http://dmitrysoshnikov.com/wp-content/uploads/ec-stack.png)

图 4. 执行上下文栈

程序开始运行时，会先进入栈底的(栈的第一个元素) *全局执行上下文(global execution context) *
，然后全局代码进行初始化，创建所需的对象与方法。在全局上下文的执行过程中，代码会激活其它(已经创建)函数并进入函数各自的执行上下文，同时会向栈中压入新的元素。初始化完成后，运行时系统会等待事件的注入(比如用户鼠标点击)，事件会激活函数并且进入新的执行上下文。

见图5，有一个函数上下文“EC1″和一个全局上下文“Global EC”，下图展现了从“Global EC”进入和退出“EC1″时栈的变化:

![Figure 5. An execution context stack changes.](http://dmitrysoshnikov.com/wp-content/uploads/ec-stack-changes.png)

图 5. 执行上下文栈的变化

ECMAScript运行时系统就是这样管理代码的执行。

关于ECMAScript执行上下文栈的内容请查阅 [第1章. 
执行上下文](http://dmitrysoshnikov.com/ecmascript/chapter-1-execution-contexts/)
([Chapter 
1. Execution context](http://dmitrysoshnikov.com/ecmascript/chapter-1-execution-contexts/)
)。

如上所述，栈中每一个执行上下文可以表示为一个对象。让我们看看上下文对象的结构以及执行其代码所需的 *状态(state) 。*

## 五、执行上下文(Execution Context)

执行上下文可以抽象为一个简单的对象。每个上下文包含一系列属性(我们称之为 *上下文状态(context’s state) *
) 
用以跟踪相关代码的执行过程。下图展示了上下文的结构：

![Figure 6. An execution context structure.](http://dmitrysoshnikov.com/wp-content/uploads/execution-context.png)

图 6. 上下文结构

除了这3个所需要的属性(*变量对象(variable object)，**this指针(this value)，**作用域链(scope chain) *
)，执行上下文根据具体实现还可以具有任意额外属性。

接着，让我们仔细说明上下文的重要属性。

## 六、变量对象(Variable Object)

> 
*变量对象(variable object)*
 是与执行上下文相关的 *数据作用域(scope of data) *
。它是与上下文关联的特殊对象，用于存储被定义在上下文中的 *变量(variables)*
 和 *函数声明(function declarations) *
。


请注意，变量对象不包含 *函数表达式(function expressions)*
 (与 *函数声明(function declarations) 比较*
)。

变量对象是一个抽象的概念。在不同的上下文中，它以不同的对象[译注:意思应为对象集合]来表示。举例来说，全局上下文中，变量对象是 
*全局对象本身(global object itself) *
(这就是我们能够通过属性名称引用全局对象的全局变量)。

让我们看看下面的例子：



|`01`|`var``foo = 10; `|
|----|----|


|`02`|` `|
|----|----|


|`03`|`function``bar() {} ````// 函数声明```|
|----|----|


|`04`|`(``function````baz() {});``````// 函数表达式```|
|----|----|


|`05`|` `|
|----|----|


|`06`|`console.``log````(```|
|----|----|


|`07`|`  ``this````.foo == foo,``````// true```|
|----|----|


|`08`|`  ``window````.bar == bar``````// true```|
|----|----|


|`09`|```);```|
|----|----|


|`10`|` `|
|----|----|


|`11`|`console.``log````(baz);``````// 引用错误，baz没有被定义```|
|----|----|




全局上下文中的变量对象(VO
)会有如下属性：

![Figure 7. The global variable object.](http://dmitrysoshnikov.com/wp-content/uploads/variable-object.png)

图 7. 全局变量对象

如上所示，函数“baz”如果作为函数表达式则不被不被包含于变量对象。这就是在函数外部尝试访问产生 
*引应用错误(ReferenceError)*
 的原因。

请注意，ECMAScript和其他语言相比(比如C/C++)，仅有函数能够创建新的作用域。在函数内部定义的变量与内部函数，在外部非直接可见并且不污染全局对象。

使用 *eval*
 
的时候，我们同样会使用一个新的(eval创建)执行上下文。eval会使用全局变量对象或调用者的变量对象(eval的调用来源)。

那函数以及自身的变量对象又是怎样的呢?在一个函数上下文中，变量对象被表示为 *激活对象(activation object)*
。

## 七、激活对象(Activation object)

当函数被调用者激活，一个特殊的对象——*激活对象(activation object)*
 将被创建。它包含 *形式参数(formal parameters)*
 与特殊 *参数(arguments)*
 
对象(具有索引属性的形参键值表(map)[译注：表现为数组])。激活对象在函数上下文中作为变量对象使用。

即：函数的变量对象保持不变，但除去存储变量与函数声明之外，还包含形参以及特殊对象 *arguments *
。

考虑下面的例子



|`1`|`function``foo(x, y) { `|
|----|----|


|`2`|`  ``var````z = 30;```|
|----|----|


|`3`|`  ``function````bar() {}``````// FD```|
|----|----|


|`4`|`  ``(``function``baz() {}); ````// FE```|
|----|----|


|`5`|```}```|
|----|----|


|`6`|` `|
|----|----|


|`7`|```foo(10, 20);```|
|----|----|




“foo”函数上下文的下一个激活对象(AO
)如下图所示：

![Figure 8. An activation object.](http://dmitrysoshnikov.com/wp-content/uploads/activation-object.png)

图 8. 激活对象

由上可见*函数表达式*
“baz”并未包含在变量/激活对象中。

对于这个话题的完整描述与所有巧妙示例(比如变量与函数声明的*“挂起(hoisting)”*
)[译注：hoisting的语义需要推敲~~]可以在[第2章. 
可变对象](http://dmitrysoshnikov.com/ecmascript/chapter-2-variable-object/)
([Chapter 
2. Variable object](http://dmitrysoshnikov.com/ecmascript/chapter-2-variable-object/)
)找到。

让我们来看下一小节的内容。众所周知，在ECMAScript中我们可以使用内部函数，而在内部函数中，我们或许会引用父函数或全局上下文中的变量。如果我们将函数的 
*变量对象*
 命名为上下文的 *作用域对象(scope object)*
 ，那么和之前讨论的原型链类似，也有一个所谓的 
*作用域链(scope chain) *
。

## 八、作用域链(Scope Chains)

> 
作用域链是一个 *对象列表(list of objects) ，*
用以检索上下文代码中出现的 *标识符(identifiers) *
。


作用域链规则简单并且与原型链相似：如果一个变量不是在自身的作用域内(自身 变量/活动 对象)，那么就会在它的外部作用域中查找。

考虑到上下文，标识符为:*变量名*
，函数声明，形式参数等等。 在函数代码中，如果标识符不是一个本地变量/函数/形参，那么就被称之为* 自由变量(free variable) *
。作用域链正是用于查找这些自由变量。

一般情况下，作用域链是 *父变量对象(parent variable objects) *
列表外加(在作用域链头部)函数的 
*自身变量/激活对象(own variable/activation object) *
。然而作用域链也会包含其他对象，举例来说某些对象会在上下文的执行过程中，动态载入作用域链——如 *with-objects*
 或 
*catch-clauses *
创建的特殊对象。[译注：with-objects指的是with语句，产生的临时作用域对象；catch-clauses指的是catch从句，如catch(e)，这会产生异常对象，导致作用域变更]

当查找标识符的时候，会从作用域链的激活对象部分开始查找，然后(如果标识符没有在激活对象中找到)查找作用域链的顶部，循环往复，就像作用域链那样。



|`01`|`var``x = 10; `|
|----|----|


|`02`|` `|
|----|----|


|`03`|`(``function````foo() {```|
|----|----|


|`04`|`  ``var````y = 20;```|
|----|----|


|`05`|`  ``(``function``bar() { `|
|----|----|


|`06`|`    ``var````z = 30;```|
|----|----|


|`07`|`    ``// "x"和"y"是自由变量 `|
|----|----|


|`08`|` `|
|----|----|


|`09`|`    ````// 会在作用域链的下一个对象中找到(函数”bar”的互动对象之后)```|
|----|----|


|`10`|`    ``console.``log````(x + y + z);```|
|----|----|


|`11`|`  ````})();```|
|----|----|


|`12`|`})();`|
|----|----|




我们可以通过隐含的*__parent__*
 属性来设定作用域链的范围，*__parent__*
 
指向链表的下一个对象。这个尝试可以在[真实的Rhino代码](http://dmitrysoshnikov.com/ecmascript/chapter-2-variable-object/#feature-of-implementations-property-__parent__)
([real 
Rhino code](http://dmitrysoshnikov.com/ecmascript/chapter-2-variable-object/#feature-of-implementations-property-__parent__)
)中测试[译注:Rhino为Mozilla的开源项目，基于Java实现的ECMAScript引擎],并且是 
*ES5词法环境(ES5 lexical environments)*
 
中实际使用的技术(被命名为outer的链接)。作用域链还可以通过数组展现。通过使用 __parent__ 
的概念，我们可以用下图展现上面的例子(父变量对象存储在函数的[[Scope]]属性内)：

![Figure 9. A scope chain.](http://dmitrysoshnikov.com/wp-content/uploads/scope-chain.png)

图 9. 作用域链

代码执行时，作用域链可能会被 *with语句(with-statement)*
 和 
*catch从句(catch-clause)*
 对象扩大(增长)。此外，由于这些对象是简单对象，他们拥有原型(和原型链)。实际上作用域链表查找分为 
*两个维度(two-dimensional) *
：(1)首先考虑自身的作用域链表指针，(2)对于每一个作用域链表指针，需要深入到指针自身的原型链当中去(如果作用域链表的节点拥有原型)。

举例：



|`01`|`Object``.``prototype````.x = 10;```|
|----|----|


|`02`|` `|
|----|----|


|`03`|`var``w = 20; `|
|----|----|


|`04`|`var``y = 30; `|
|----|----|


|`05`|` `|
|----|----|


|`06`|```// in SpiderMonkey global object```|
|----|----|


|`07`|```// i.e. variable object of the global```|
|----|----|


|`08`|```// context inherits from "Object.prototype",```|
|----|----|


|`09`|```// so we may refer "not defined global```|
|----|----|


|`10`|```// variable x", which is found in```|
|----|----|


|`11`|```// the prototype chain```|
|----|----|


|`12`|` `|
|----|----|


|`13`|`console.``log````(x);``````// 10```|
|----|----|


|`14`|` `|
|----|----|


|`15`|`(``function````foo() {```|
|----|----|


|`16`|` `|
|----|----|


|`17`|`  ````// "foo" local variables```|
|----|----|


|`18`|`  ``var````x = 100;```|
|----|----|


|`19`|`  ``var````w = 40;```|
|----|----|


|`20`|` `|
|----|----|


|`21`|`  ````// "x" is found in the```|
|----|----|


|`22`|`  ````// "Object.prototype", because```|
|----|----|


|`23`|`  ````// {z: 50} inherits from it```|
|----|----|


|`24`|` `|
|----|----|


|`25`|`  ``with````({z: 50}) {```|
|----|----|


|`26`|`    ``console.``log````(w, x, y , z);``````// 40, 10, 30, 50```|
|----|----|


|`27`|`  ````}```|
|----|----|


|`28`|` `|
|----|----|


|`29`|`  ````// after "with" object is removed```|
|----|----|


|`30`|`  ````// from the scope chain, "x" is```|
|----|----|


|`31`|`  ````// again found in the AO of "foo" context;```|
|----|----|


|`32`|`  ````// variable "w" is also local```|
|----|----|


|`33`|`  ``console.``log````(x, w);``````// 100, 40```|
|----|----|


|`34`|` `|
|----|----|


|`35`|`  ````// and that's how we may refer```|
|----|----|


|`36`|`  ````// shadowed global "w" variable in```|
|----|----|


|`37`|`  ````// the browser host environment```|
|----|----|


|`38`|`  ``console.``log``(``window````.w);``````// 20```|
|----|----|


|`39`|` `|
|----|----|


|`40`|`})();`|
|----|----|




结构如下(这表明当我们进入 __parent__ 指针之前, 第一个 __proto__ 链会先背考虑)：

![Figure 10. A /](http://dmitrysoshnikov.com/wp-content/uploads/scope-chain-with.png)

图 10. with增大的作用域链

请注意，全局对象并非在所有实现中都会继承自“Object.prototype”。上图描述的情景(从全局上下文中引用了未定义的变量”x”)可以在SpiderMonkey测试。

只要所有外部函数的变量对象都存在，那么从内部函数引用外部数据则没有特别之处——我们只要遍历作用域链表，查找所需变量。然而，如上文所提及，当一个上下文终止之后，其状态与自身将会被 
*销毁(destroyed)*
 
，同时内部函数将会从外部函数中返回。此外，这个返回的函数之后可能会在其他的上下文中被激活，那么如果一个之前被终止的含有一些自由变量的上下文又被激活将会怎样?通常来说，解决这个问题的概念在ECMAScript中与作用域链直接相关，被称为 
*(词法)闭包((lexical) closure)*
。

## 九、闭包(Closures)

在ECMAScript中，函数是“第一类”对象。这个名词意味着函数可以作为参数被传递给其他函数使用 
(在这种情况下，函数被称为*“funargs”——*
“functional 
arguments”的缩写[译注：这里不知翻译为泛函参数是否恰当])。接收“funargs”的函数被称之为 *高阶函数(higher-order functions)*
 ，或者更接近数学概念的话，被称为 *运算符(operators)*
 
。其他函数的运行时也会返回函数，这些返回的函数被称为 *function valued*
 函数 (有 *functional value *
的函数)。

“funargs”与“functional values”有两个概念上的问题，这两个子问题被称为*“Funarg problem”*
 
(“泛函参数问题”)。要准确解决泛函参数问题，需要引入 *闭包(closures)*
 
到的概念。让我们仔细描述这两个问题(我们可以见到，在ECMAScript中使用了函数的[[Scope]]属性来解决这个问题)。

“funarg problem”的一个子问题是*“upward funarg problem”*
[译注：或许可以翻译为：向上查找的函数参数问题]。当一个函数从其他函数返回到外部的时候，这个问题将会出现。要能够在外部上下文结束时，进入外部上下文的变量，内部函数 
*在创建的时候(at creation moment)*
 
需要将之存储进[[Scope]]属性的父元素的作用域中。然后当函数被激活时，上下文的作用域链表现为激活对象与[[Scope]]属性的组合(事实上，可以在上图见到)：

> 
作用域链 = 激活对象 + [[Scope]]


请注意，最主要的事情是——函数在被创建时保存外部作用域，是因为这个 *被保存的作用域链(saved scope chain)*
 
将会在未来的函数调用中用于变量查找。



[view source](#viewSource)

[print](#printSource)[?](#about)



|`01`|`function``foo() { `|
|----|----|


|`02`|`  ``var````x = 10;```|
|----|----|


|`03`|`  ``return``function````bar() {```|
|----|----|


|`04`|`    ``console.``log````(x);```|
|----|----|


|`05`|`  ````};```|
|----|----|


|`06`|```}```|
|----|----|


|`07`|` `|
|----|----|


|`08`|```// "foo" returns also a function```|
|----|----|


|`09`|```// and this returned function uses```|
|----|----|


|`10`|```// free variable "x"```|
|----|----|


|`11`|` `|
|----|----|


|`12`|`var````returnedFunction = foo();```|
|----|----|


|`13`|` `|
|----|----|


|`14`|```// global variable "x"```|
|----|----|


|`15`|`var``x = 20; `|
|----|----|


|`16`|` `|
|----|----|


|`17`|```// execution of the returned function```|
|----|----|


|`18`|`returnedFunction(); ````// 10, but not 20```|
|----|----|




这种类型的作用域被称为 *静态作用域(static (or lexical) scope) *
。可见变量“x”可以在函数”bar”返回的被保存的[[Scope]]中。通常来说，当上面代码中，变量”x”被设为20而不为10的时候还有一个 
*动态作用域(dynamic scope)*
 。 但是ECMAScript并未使用动态作用域。

“funarg problem”的第二个子问题是一个*“downward funarg problem” *
[译注：或许可以翻译为：向内查找的函数参数问题]。在这个例子中，外部上下文可能存在，但是对于解析标识符会产生二义性。问题是：哪一个作用域中的标识符的值会被使用的呢——是在函数创建时还是在函数执行时产生的作用域呢(比如调用者的作用域)?为了避免二义性与确立闭包，静态作用域会被使用：



|`01`|```// global "x"```|
|----|----|


|`02`|`var``x = 10; `|
|----|----|


|`03`|` `|
|----|----|


|`04`|```// global function```|
|----|----|


|`05`|`function``foo() { `|
|----|----|


|`06`|`  ``console.``log````(x);```|
|----|----|


|`07`|```}```|
|----|----|


|`08`|` `|
|----|----|


|`09`|`(``function````(funArg) {```|
|----|----|


|`10`|` `|
|----|----|


|`11`|`  ````// local "x"```|
|----|----|


|`12`|`  ``var````x = 20;```|
|----|----|


|`13`|` `|
|----|----|


|`14`|`  ````// there is no ambiguity,```|
|----|----|


|`15`|`  ````// because we use global "x",```|
|----|----|


|`16`|`  ````// which was statically saved in```|
|----|----|


|`17`|`  ````// [[Scope]] of the "foo" function,```|
|----|----|


|`18`|`  ````// but not the "x" of the caller's scope,```|
|----|----|


|`19`|`  ````// which activates the "funArg"```|
|----|----|


|`20`|` `|
|----|----|


|`21`|`  ``funArg(); ````// 10, but not 20```|
|----|----|


|`22`|` `|
|----|----|


|`23`|`})(foo); ````// pass "down" foo as a "funarg"```|
|----|----|




我们可以做出总结： 静态作用域是在一种语言中使用闭包的 *必备需求(obligatory requirement) *
。然而某些语言会同时提供静态作用域与动态作用域，以供程序员选择使用闭包。因ECMAScript中只有静态作用域被使用对于“funarg 
problem”的两个子问题，我们拥有解决方案)，结论如下：ECMAScript在技术上使用函数的[[Scope]]属性，从而对闭包有完整的支持。现在我们可以给闭包一个准确的定义：

> 
闭包由 代码块(ECMAScript中这是一个函数) 与 被静态/词法存储的所有外部作用域 
组合而成。因此，函数可以通过被存储的作用域简单引用自由变量。


请注意，因为每一个(一般)函数在创建时保存了[[Scope]]，理论上，ECMAScript中所有函数都是闭包。

另一件重要的事情是，一些函数可能会有同样的外部作用域(当有两个内部/全局函数式，这是很常见的情况)。在这个例子中[[Scope]]属性是由所有拥有同样外部作用域的函数共享的。在某个闭包中产生的变量变化将会在其他闭包中读取变量时 
*反映(reflected)*
 出来：



|`01`|`function``baz() { `|
|----|----|


|`02`|`  ``var````x = 1;```|
|----|----|


|`03`|`  ``return````{```|
|----|----|


|`04`|`    ````foo:````function``foo() { ``return``++x; }, `|
|----|----|


|`05`|`    ````bar:````function``bar() { ``return``--x; } `|
|----|----|


|`06`|`  ````};```|
|----|----|


|`07`|```}```|
|----|----|


|`08`|` `|
|----|----|


|`09`|`var``closures = baz(); `|
|----|----|


|`10`|` `|
|----|----|


|`11`|`console.``log````(```|
|----|----|


|`12`|`  ``closures.foo(), ````// 2```|
|----|----|


|`13`|`  ``closures.bar()  ````// 1```|
|----|----|


|`14`|`);`|
|----|----|




代码如下图所示：

![Figure 11. A shared [[Scope]].](http://dmitrysoshnikov.com/wp-content/uploads/shared-scope.png)

图 11. 共享的[[Scope]].

在某个循环中创建多个函数时，上图会引发一个困惑。如果在创建的函数中使用循环变量(如”k”)，那么所有的函数都使用同样的循环变量，导致一些程序员经常会得不到预期值。现在清楚为什么会产生如此问题了——因为所有函数共享同一个[[Scope]]，其中循环变量为最后一次复赋值。



|`01`|`var``data = []; `|
|----|----|


|`02`|` `|
|----|----|


|`03`|`for``(``var````k = 0; k < 3; k++) {```|
|----|----|


|`04`|`  ``data[k] = ``function````() {```|
|----|----|


|`05`|`    ``alert````(k);```|
|----|----|


|`06`|`  ````};```|
|----|----|


|`07`|```}```|
|----|----|


|`08`|` `|
|----|----|


|`09`|`data[0](); ``// 3, but not 0 `|
|----|----|


|`10`|`data[1](); ``// 3, but not 1 `|
|----|----|


|`11`|`data[2](); ````// 3, but not 2```|
|----|----|




有一些用以解决这类问题的技术。其中一种技巧是在作用域链中提供一个额外的对象，比如使用其他函数：



[view source](#viewSource)

[print](#printSource)[?](#about)



|`01`|`var``data = []; `|
|----|----|


|`02`|` `|
|----|----|


|`03`|`for``(``var````k = 0; k < 3; k++) {```|
|----|----|


|`04`|`  ``data[k] = (``function````(x) {```|
|----|----|


|`05`|`    ``return``function````() {```|
|----|----|


|`06`|`      ``alert````(x);```|
|----|----|


|`07`|`    ````};```|
|----|----|


|`08`|`  ````})(k);``````// pass "k" value```|
|----|----|


|`09`|```}```|
|----|----|


|`10`|` `|
|----|----|


|`11`|```// now it is correct```|
|----|----|


|`12`|`data[0](); ``// 0 `|
|----|----|


|`13`|`data[1](); ``// 1 `|
|----|----|


|`14`|`data[2](); ``// 2`|
|----|----|




闭包理论的深入研究与具体实践可以在 [第6章.闭包](http://dmitrysoshnikov.com/ecmascript/chapter-6-closures/)
([Chapter 6. 
Closures](http://dmitrysoshnikov.com/ecmascript/chapter-6-closures/)
)中找到。如果想得到关于作用域链的更多信息，可以参照 [第4章作用域链](http://dmitrysoshnikov.com/ecmascript/chapter-4-scope-chain/)
([Chapter 4. 
Scope chain](http://dmitrysoshnikov.com/ecmascript/chapter-4-scope-chain/)
)。

下一章节将会讨论一个执行上下文的最后一个属性——*this*
指针的概念。

## 十、This指针(This Value)

> 
*this *
指针是一个与执行上下文相关的特殊对象。因此，它也会被命名为 *上下文对象(context object)*
 
(激活执行上下文的上下文)。


任何对象都可以作为上下文的this值。我想再次澄清对与ECMAScript中，与执行上下文相关的一些描述——特别是this的误解。通常，*this *
被错误地，描述为变量对象的属性。最近比如在[这本书中](http://yuiblog.com/assets/High_Perf_JavaScr_Ch2.pdf)
就发现了(尽管书中提及this的那一章还不错)。 
请牢记：

*this *
是执行上下文的属性，但不是变量对象的属性。

这个特征是很重要的。因为同变量比较，*this *
从不参与变量解析的过程。即当在代码中访问*this*
，它的值是从执行上下文中直接得到而不需要任何作用域链的查找。*this*
 
的值在进入上下文时即被确立。

和ECMAScript相比，Python有自己的 *self*
 参数作为单独的变量用于解析同样问题。ECMAScript中，*this *
不能被重新赋值，因为this不是一个变量，并且不属于变量对象。

在全局上下文中，*this 就是*
全局对象本身(这意味着，this等于全局对象)：



|`1`|`var``x = 10; `|
|----|----|


|`2`|` `|
|----|----|


|`3`|`console.``log````(```|
|----|----|


|`4`|`  ````x,``````// 10```|
|----|----|


|`5`|`  ``this``.x, ``// 10 `|
|----|----|


|`6`|`  ``window``.x ``// 10 `|
|----|----|


|`7`|`);`|
|----|----|




在函数上下文中，每一次单独的函数调用，*this *
的值都不相同。这里this的值是由call函数表达式提供的调用者本身(函数被激活的一个方法)。举例来说，下面的函数“foo”被全局上下文调用，是一个被调用者。然我们看看这个例子，同样的函数代码对于不同的调用(使用不同的函数激活方式)，调用者提供的this值是怎样不同的：



|`01`|```// the code of the "foo" function```|
|----|----|


|`02`|```// never changes, but the "this" value```|
|----|----|


|`03`|```// differs in every activation```|
|----|----|


|`04`|` `|
|----|----|


|`05`|`function``foo() { `|
|----|----|


|`06`|`  ``alert``(``this````);```|
|----|----|


|`07`|```}```|
|----|----|


|`08`|` `|
|----|----|


|`09`|```// caller activates "foo" (callee) and```|
|----|----|


|`10`|```// provides "this" for the callee```|
|----|----|


|`11`|` `|
|----|----|


|`12`|`foo(); ````// global object```|
|----|----|


|`13`|`foo.``prototype``.``constructor````();``````// foo.prototype```|
|----|----|


|`14`|` `|
|----|----|


|`15`|`var``bar = { `|
|----|----|


|`16`|`  ````baz: foo```|
|----|----|


|`17`|```};```|
|----|----|


|`18`|` `|
|----|----|


|`19`|`bar.baz(); ``// bar `|
|----|----|


|`20`|` `|
|----|----|


|`21`|`(bar.baz)(); ``// also bar `|
|----|----|


|`22`|```(bar.baz = bar.baz)();``````// but here is global object```|
|----|----|


|`23`|`(bar.baz, bar.baz)(); ````// also global object```|
|----|----|


|`24`|`(``false````|| bar.baz)();``````// also global object```|
|----|----|


|`25`|` `|
|----|----|


|`26`|`var````otherFoo = bar.baz;```|
|----|----|


|`27`|`otherFoo(); ````// again global object```|
|----|----|




如果要深入思考每一次函数调用中，this值的变化(更重要的是怎样变化)，你可以阅读 [第3章. This](http://dmitrysoshnikov.com/ecmascript/chapter-3-this/)
([Chapter 3. 
This](http://dmitrysoshnikov.com/ecmascript/chapter-3-this/)
) 。上文所提及的情况都会在第三章内详细讨论。

## 十一、总结(Conclusion)

在此我们完成了一个简短的概述。尽管看来不是那么简短，但是这些话题若要完整表述完毕，则需要一整本书。.我们没有提及两个重要话题：*泛函(functions)*
 
(以及不同类型的函数之间的不同，比如函数声明与函数表达式)与ECMAScript的 *求值策略(evaluation strategy)*
 
。这两个话题可以分别查阅 [第5章. 泛函](http://dmitrysoshnikov.com/ecmascript/chapter-5-functions/)
([Chapter 5. 
Functions](http://dmitrysoshnikov.com/ecmascript/chapter-5-functions/)
) 与 [第8章. 
求值策略(](http://dmitrysoshnikov.com/ecmascript/chapter-8-evaluation-strategy/)[Chapter 
8. Evaluation strategy](http://dmitrysoshnikov.com/ecmascript/chapter-8-evaluation-strategy/)
)。

如果你有任何评论，问题或者补充，我很欢迎在文章评论中讨论。

祝大家学习ECMAScript顺利。

原文
：[http://dmitrysoshnikov.com/ecmascript/javascript-the-core/](http://dmitrysoshnikov.com/ecmascript/javascript-the-core/trackback/)

原作者
：[Dmitry A. 
Soshnikov](http://dmitrysoshnikov.com/)



