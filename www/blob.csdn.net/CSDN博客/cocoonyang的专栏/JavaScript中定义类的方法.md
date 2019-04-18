# JavaScript中定义类的方法 - cocoonyang的专栏 - CSDN博客





2013年01月07日 21:07:49[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：616标签：[code																[javascript](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=code&t=blog)
个人分类：[nodejs](https://blog.csdn.net/cocoonyang/article/category/1301193)








JavaScript目前根本没有class的概念（ECMAScript 6 草案中要引入 class）。作为一种灵活的编程语言，JavaScript使用基于原型的继承（prototype-based inheritance）机制模拟面向对象方法中类的一些功能。在JavaSceript语言标准[5]中，对象被注释为一些属性和一个原型对象的集合，其中的原型对象的值可能为null("An object is a collection of properties and has a single prototype
 object. The prototype may be the null value.")[5]。对象可以直接被创建，也可以通过构造函数创建对象（objects may be created in various ways including via a literal notation or via constructors）[5]。 每个构造函数都有一个名为原型（ "prototype"） 的属性。下面这段代码[6]生成一个简单对象foo，图1展示了这个简单对象的基本结构。



```java
var foo = {
  x: 10,
  y: 20
};
```



![](https://img-my.csdn.net/uploads/201301/15/1358179690_8483.png)


图1 JavaScript中的对象结构示例  [6]

至于复杂的对象，会形成一个原型链。正是依靠原型链，JavaScript实现了基于原型的继承和属性共享。例如，下面的代码[6]创建了一个复杂的对象，图2展示了这个对象的原型链。



```java
// a constructor function
function Foo(y) {
  // which may create objects
  // by specified pattern: they have after
  // creation own "y" property
  this.y = y;
}
 
// also "Foo.prototype" stores reference
// to the prototype of newly created objects,
// so we may use it to define shared/inherited
// properties or methods, so the same as in
// previous example we have:
 
// inherited property "x"
Foo.prototype.x = 10;
 
// and inherited method "calculate"
Foo.prototype.calculate = function (z) {
  return this.x + this.y + z;
};
 
// now create our "b" and "c"
// objects using "pattern" Foo
var b = new Foo(20);
var c = new Foo(30);
 
// call the inherited method
b.calculate(30); // 60
c.calculate(40); // 80
 
// let's show that we reference
// properties we expect
 
console.log(
 
  b.__proto__ === Foo.prototype, // true
  c.__proto__ === Foo.prototype, // true
 
  // also "Foo.prototype" automatically creates
  // a special property "constructor", which is a
  // reference to the constructor function itself;
  // instances "b" and "c" may found it via
  // delegation and use to check their constructor
 
  b.constructor === Foo, // true
  c.constructor === Foo, // true
  Foo.prototype.constructor === Foo // true
 
  //b.calculate === b.__proto__.calculate, // true，在Node.js中报错
  //b.__proto__.calculate === Foo.prototype.calculate // true,在Node.js中报错
 
);
```



![](https://img-my.csdn.net/uploads/201301/15/1358180859_7962.png)




图2 复杂对象的原型链[6]

JavaScript可执行代码分为三种：Global代码，Eval代码和Function代码。
- Global代码就是JavaScript脚步程序的源代码，但是JavaScript标准中特别指出Global代码并不包括那些被解析到函数体内的那部分代码[5]。
- Eval代码就是被提交到内置函数eval中的代码。 
- Function代码就是函数体中的代码，也不包括那些被解析到函数体内的那部分代码。



Execution Conetexts


ECMAScript语言标准[ECMA-262 5.1] 10.3中指出， 

  "When control is transferred to ECMAScript executable code, control is entering an execution context."  "Active execution contexts logically form a stack. The top execution context on this logical stack is the running execution context. A new execution context
 is created whenever control is transferred from the executable code associated with the currently running execution context to executable code that is not associated with that execution context. The newly created execution context is pushed onto the stack
 and becomes the running execution context."


所有的代码都是在其执行空间（execution context） 内被执行（evaluated），全局执行空间只有一个。每一次函数调用都会产生一个函数执行空间，相应的每一次调用eval函数，也会产生一个eval函数执行空间。一个执行空间可以触发另一个新执行空间，并将程序的控制流交付新执行空间。JavaScript通过执行空间堆栈来维护各个执行空间的关系。

执行空间也是一个对象。每个执行空间对象有三个必要属性：一个变量对象，this值，一个空间链。如图3所示。执行空间的变量对象与这个空间中的变量和函数声明相关联。空间链主要用于寻找这个空间代码中的标识符（变量名，函数声明，形参等）。this值是与执行空间相关联的特殊对象。任何对象都可以成为this值。它不是变量对象的属性。仅当程序的控制流进入执行空间时才确定this值，对于函数运行空间，每次调用时的this值都有可能不同。 


![](https://img-my.csdn.net/uploads/201301/15/1358197451_7693.png)


图3 执行空间对象[6]

在JavaScript中，所谓 定义类的方法其实就是创建对象的方法。

1. 最常用的方法就是定义一个构造函数，然后使用new关键字来创建实例。在构造函数内部用this值指代实例对象。

一个类包含有类的属性和类的方法。定义类的方法有两种途径：一种是在类的构造函数中用定义一般函数的方式；另一种是使用原型（prototype）方式。

根据定义类中方法的不同， 定义构造函数的方法主要分为三种：不使用原型（prototype）方式在构造函数内定义类（class）中方法（method）；使用原型（prototype）方式在构造函数内定义类（class）中方法（method）；和使用原型（prototype）方式在构造函数外定义类（class）中方法（method）。[1]比较了这 三种JavaScript中定义构造函数的方法：

    1）不使用原型（prototype）方式在构造函数内定义类（class）中方法（method）



```java
NonPrototypeClass = function() {
  this.message = function(s) {
    var msg = s + '';
  };
  this.addition = function(i, j) {
    return (i * 2 + j * 2) / 2;
  };
};
NonPrototypeClass_instance = new NonPrototypeClass();
```
使用这种方法创建出的多个实例对象不共享类的属性值。



    2）使用原型（prototype）方式在构造函数内定义类（class）中方法（method）



```java
InsidePrototypeClass = function() {
  InsidePrototypeClass.prototype.message = function(s) {
    var msg = s + '';
  };
  InsidePrototypeClass.prototype.addition = function(i, j) {
    return (i * 2 + j * 2) / 2;
  };
};
InsidePrototypeClass_instance = new InsidePrototypeClass();
```



    3）使用原型（prototype）方式在构造函数外定义类（class）中方法（method）；


```java
OutsidePrototypeClass = function() {};
OutsidePrototypeClass.prototype.message = function(s) {
  var msg = s + '';
};
OutsidePrototypeClass.prototype.addition = function(i, j) {
  return (i * 2 + j * 2) / 2;
};
OutsidePrototypeClass_instance = new OutsidePrototypeClass();
```
因为JavaScript引擎在执行原型（prototype）方法（method）之前，要检查是否有更一般的方法（method），所以调用不使用原型（prototype）方式定义出的方法（method）的速度最快。


JavaScript引擎在创建实例（Instance）对象时，构造函数越复杂，所需的时间越多。 在外部定义方法（method）的构造函数要比其余的构造函数快近20倍，它所有的实例（Instance）会使用共同的方法，不仅节省内存，也减少了垃圾收集的工作量。


综上所述，在JavaScript中，最好使用原型（prototype）方式在外部定义方法（method）的构造函数，[2]也表达了相似的观点。



2. 直接创建实例对象


```java
var laptop = {
    type: "Thinkpad",
    color: "black",
    getInfo: function () {
        return this.color + ' ' + this.type;
    }
}
laptop.color = "reddish";
```



这种方法不能实现私有属性和私有方法，实例对象之间也不能共享数据。




3.



```java
var Computer = {
    createNew: function(){
        var computer = {};
　　　　computer.type = "Thinkpad";
　　　　computer.color = "black";
　　　　computer.getInfo = function () {
           return this.color + ' ' + this.type;
        };
　　　　return computer;
　　}
};
var laptop = Computer.createNew();
```




4.



```java
var laptop = new function() {
    this.type = "Thinkpad";
    this.color = "black";
    this.getInfo = function () {
        return this.color + ' ' + this.type;
    };
}
laptop.color = "reddish";
```




5.  抽象类

使用this值的奇技淫巧，可以定义虚方法[3]。



```java
var tmp=function()
{
  {
     this.ini.apply(this,arguments); 
       //这里的arguments是构造函数的
       //this.ini通过this调用，但却是未定义的。
       //将在其prototype中给出实现
  }
}
//实现虚方法 ini,且提供给构造函数调用
tmp.prototype.ini=function()
{
  alert(arguments[0]);
}
var tt=new tmp('a');
```


```java
var Class=
 {
   create:function()
   {
     return function()  //返回一个匿名函数，也就是一个函数对象
     {      
        this.ini.apply(this,arguments);  //此时的arguments是函数对象的构造函数传递过来的参数对象
     }
   }
 }
 
 var tmp=Class.create();
 
 //此时tmp相当于
 //var tmp=function()
 //{
 //  this.ini.apply(this,arguments);
 //}
 //可以通过alert(tmp.toString());看出
 tmp.prototype.ini=function()
 {
       alert(arguments.length);  //这里的arguments是ini自身的参数
 }
 var ttt=new tmp('adf');
```



参考资料


[1] Fred'sblog  http://fred-zone.blogspot.co.uk/2012/04/javascript-class.html



[2] http://msdn.microsoft.com/en-us/library/windows/apps/hh781219.aspx#structure_objects_correctly


[3] http://www.cnblogs.com/McJeremy/archive/2009/01/14/1375779.html


[4] http://book.nodejs.tw/zh-tw/node_javascript.html

[5] http://www.ecma-international.org/publications/files/ECMA-ST/Ecma-262.pdf

[6] http://dmitrysoshnikov.com/ecmascript/javascript-the-core/


[ECMA-262 5.1] http://www.ecma-international.org/ecma-262/5.1/ 





