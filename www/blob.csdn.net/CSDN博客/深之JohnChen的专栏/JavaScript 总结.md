# JavaScript 总结 - 深之JohnChen的专栏 - CSDN博客

2017年03月02日 10:08:51[byxdaz](https://me.csdn.net/byxdaz)阅读数：2460
个人分类：[web前端](https://blog.csdn.net/byxdaz/article/category/5719017)



    JavaScript是互联网上最流行的脚本语言，这门语言可用于 HTML和
 web，更可广泛用于服务器、PC、笔记本电脑、平板电脑和智能手机等设备，JavaScript是一种轻量级的编程语言。

变量

有三个关键字可以用于声明变量：var,let,const。后面两个是ES6新加的。如果没有使用任何关键字，变量是全局变量。

var 声明的变量会被提升到当前作用域的最前面，它的作用域范围也就是当前作用域，即使它是在语句块中声明。

let、const声明的变量会绑定当前语句块（暂时性死区，temporal dead
 zone，简称TDZ），被声明之后才可以使用。只在声明所在的块级作用域内有效；不存在“变量提升“现象，只能在声明的位置后面使用；也不可重复声明。不同的是const声明之后不可变，如果声明的是对象，不能再指向另一个对象，但是对象属性可以变。使用Object.freeze方法可以使对象属性也不可变。

var a =0;
function foo(){
//if中的声明语句会被提升到这里
//var a;
if(false) {
var a = 1;
}

  a = 10;
console.log(a);//10

}
function bar(){
{

    console.log(b); //ReferenceError: can't access lexical declaration `b' before initialization
let b = 2;
}

  console.log(b); //ReferenceError: b is not defined

}
function baz(){
{

    const c = 2;
}

  console.log(c); //ReferenceError: c is not defined

}

ES6规定，var和function声明的全局变量，属于全局对象的属性；let命令、const命令、class命令声明的全局变量，不属于全局对象的属性。

声明提升

var 声明的变量，function声明的函数会被提升到当前作用域顶端。只有声明才会提升，表达式不会。

数据类型

共有七种数据类型：Undefined, Null,Boolean, String, Symbol, Number和Object.

前六种是简单（基本）数据类型，其中Boolean, String,Number有基本包装类型，每当读取一个基本类型值的时候，后台就会创建一个对应的基本包装类型的对象

ES6新加了Symbol类型，是一种特殊的、不可变的数据类型，可以作为对象属性的标识符使用。

typeof

·        “undefined”如果这个值未定义

·        “boolean”如果这个值是布尔值

·        “string”如果这个值是字符串

·        “number”如果这个值是数值

·        “object”如果这个值是对象或者null

·        “function”如果这个值是函数

·        “symbol”如果这个值是Symbol类型（ES6新增）

实际上JavaScript中函数也是对象。

大体上来说,基本类型的值应该使用 typeof来检测,而对象的值则应该使用
 instanceof来检测。

对象的类定义

JavaScript 标准文档只给出了一种获取 [[Class]]值的方法，那就是使用Object.prototype.toString。

function is(type, obj){
var clas =Object.prototype.toString.call(obj).slice(8,-1);
return obj !==undefined && obj !==null
 && clas===type;

}

is('String','test');// true
is('String', new String('test')); // true

上面例子中，Object.prototype.toString方法被调用，this被设置为了需要获取
 [[Class]]值的对象。

Object.prototype.toString 返回一种标准格式字符串，所以上例可以通过 slice截取指定位置的字符串，如下所示：

Object.prototype.toString.call([])   //"[object Array]"

Object.prototype.toString.call({})    //"[object Object]"

Object.prototype.toString.call(2)   //"[object Number]"

undefined& null

这两种类型分别都只有一个值，分别是undefined和null。

未声明的变量或声明后没有初始化的变量都是undefined的，typeof会返回undefined。全局变量中有undefined变量，其值也是undefined。函数中没有定义return或return没有显示返回任何内容时返回值会是undefined。函数参数没有显示传递值也会是undefined。

null 值表示一个空对象指针，而这也正是使用 typeof操作符检测 null值时会返回”object”的原因

实际上，undefined值是派生自null值的，null
 ==undefined将会返回true。

类型转换

在使用相等(==)和不相等(!=)操作符,或者在
 if 、 for及 while等流控制语句中使用非布尔值时,最常发生类型转换。避免隐式类型转换，可能会带来不期望的结果。尽量使用===
 ,!==替代== ,!=。

最好是使用显示类型转换，如下

//转为数值

var n1 = +'123';//123和下面等效

var n2 = Number('123');//123

var n3 = parseInt('123.3blue');//123

var n4 = parseInt('blue123');//NaN

var n5 = parseInt('0123.23',8);//83

var n6 = parseFloat('123.23.12blue');//123.23
//转为字符串

var s1 = '' +123;//"123"和下面等效

var s2 = String(123);//"123"
//转为布尔值

var b1 = !!'123';//true和下面等效

var b3 = Boolean('123');//true

技巧

'' +10 ==='10';//true

+'10' ===10;//true

!!'foo';  //true

内置类型（比如 Number和 String）的构造函数在被调用时，使用或者不使用
 new的结果完全不同。

new Number(10) ===10;    //false,对象与数字的比较

Number(10) ===10;        //true,数字与数字的比较

new Number(10) +0 ===10;//true,由于隐式的类型转换

布尔类型转换规则
|**数据类型**|**转换为true的值**|**转换为false的值**|
|----|----|----|
|Boolean|true|false|
|String|非空字符串|“”|
|Number|非零数字值（包括无穷大）|0和NaN|
|Object|任何对象|null|
|Undefined|n/a（不适用）|undefined|

## 函数

ECMAScript中最有意思的可能是函数了，函数实际上是对象。每个函数都是Function类型的实例，而且都与其他引用类型一样具有属性和方法。由于函数是对象，因此函数名实际上也是一个指向函数对象的指针，不会与某个函数绑定。

创建一个函数，在创建Funciton类型实例的同时还会创建一个原型对象，函数变量的prototype属性指向该原型对象。

### 函数声明和表达式

函数通常是使用函数声明语法定义的。

1.function xxx(){}

2.var xx = function(){}

3.var test3 = newFunction("a","b","return a+b;");//不常用，定义在方法内也是顶级作用域。

第一种和第二种方式基本一致，除了第二种解析方式是顺序解析，需要在调用的方法前定义。

### arguments

函数内部的arguments是一个类数组对象，包含着传入函数中的所有参数。虽然 arguments的主要用途是保存函数参数，但是arguments存储的参数和形式参数存储的变量空间是独立的。arguments对象为其内部属性以及函数形式参数创建
 getter和 setter方法。因此，改变形参的值会影响到 arguments对象的值，反之亦然。严格模式下不允许创建这些getter和setter，所以两者值互不影响。

functiontest(num1, num2) {
'use strict';
num1 = 11;
console.log(arguments[0]);
}

### this

函数内部的另一个特殊对象是 this ,其行为与 Java中的
 this大致类似。this引用的是执行函数的环境对象（当在网页的全局作用域中调用函数时，this对象引用的就是window）。

全局范围内使用this，它指向全局对象

this//浏览器里就是window
foo() //这种函数调用中的this也是全局对象，如果在严格模式下将是undefined
test.foo(); //this指向test对象new foo();//this指向新创建的对象

## 错误处理

ECMA-262 定义了下列 7种错误类型:Error,EvalError,RangeError,ReferenceError,SyntaxError,TypeError,URIError。可以通过继承这些类型自定义错误类型。

内置的Error对象具有两个标准属性name和message

·        **name**：错误名称

·        **message**：错误提示信息

·        **stack**：错误的堆栈（非标准属性，但是大多数平台支持）

functionfn(){
try {
var x = 1;
thrownewError('error');
} catch (e) {
console.log('x=' + x);
return x;
} finally {
x = 2;
console.log('x=' + x);
}
}

上面代码说明，即使有return语句在前，finally代码块依然会得到执行，且在其执行完毕后，并不影响return语句要返回的值。

必须要给 throw操作符指定一个值,这个值是什么类型,没有要求。

throw12345;
throw"Hello world!";
throwtrue;
throw { name:"JavaScript"};

任何没有通过 try-catch处理的错误都会触发 window对象的
 error事件。

## 垃圾回收

JavaScript 中最常用的垃圾收集方式是标记清除（mark-and-sweep）。

另一种不太常见的垃圾收集策略叫做引用计数（reference counting）。IE中的COM对象的垃圾收集机制采用的就是引用计数策略，只要在IE中涉及COM对象，就会存在循环引用的问题

## 对象

ECMA-262把对象定义为：“无序属性的集合，其属性可以包含基本值、对象或者函数。”严格来讲，这就相当于说对象是一组没有特定顺序的值。对象的每个属性或方法都有一个名字，而每个名字都映射到一个值。正因为这样（以及其他将要讨论的原因），我们可以把ECMAScript的对象想象成散列表：无非就是一组名值对，其中值可以是数据或函数

### 属性类型

ECMAScript 中有两种属性:数据属性和访问器属性。

1.     数据属性
数据属性包含一个数据值的位置。在这个位置可以读取和写入值。数据属性有4个描述其行为的特性。

o    [[Configurable]]：表示能否通过 delete删除属性从而重新定义属性，能否修改属性的特性，或者能否把属性修改为访问器属性。像前面例子中那样直接在对象上定义的属性，它们的这个特性默认值为true。

o    [[Enumerable]]：表示能否通过 for-in循环返回属性。像前面例子中那样直接在对象上定义的属性，它们的这个特性默认值为true。

o    [[Writable]]：表示能否修改属性的值。像前面例子中那样直接在对象上定义的属性，它们的这个特性默认值为true。

o    [[Value]]：包含这个属性的数据值。读取属性值的时候，从这个位置读；写入属性值的时候，把新值保存在这个位置。这个特性的默认值为undefined。

2.     访问器属性
访问器属性不包含数据值；它们包含一对儿getter和setter函数（不过，这两个函数都不是必需的）。在读取访问器属性时，会调用
 getter函数，这个函数负责返回有效的值；在写入访问器属性时，会调用setter函数并传入新值，这个函数负责决定如何处理数据。访问器属性有如下4个特性。

o    [[Configurable]]：表示能否通过 delete删除属性从而重新定义属性，能否修改属性的特性，或者能否把属性修改为数据属性。对于直接在对象上定义的属性，这个特性的默认值为true。

o    [[Enumerable]]：表示能否通过 for-in循环返回属性。对于直接在对象上定义的属性，这个特性的默认值为true。

o    [[Get]]：在读取属性时调用的函数。默认值为undefined。

o    [[Set]]：在写入属性时调用的函数。默认值为undefined。

Object.defineProperties(book, {
_year: {
value: 2004},
edition: {
value: 1},
year: {
get: function(){
returnthis._year;
},
set: function(newValue){
if (newValue > 2004) {
this._year = newValue;
this.edition += newValue - 2004;
}
}
}
});

使用 ECMAScript 5的 Object.getOwnPropertyDescriptor()方法，可以取得给定属性的描述符。这个方法接收两个参数：属性所在的对象和要读取其描述符的属性名称。返回值是一个对象，如果是访问器属性，这个对象的属性有configurable、enumerable、get和set；如果是数据属性，这个对象的属性有configurable、enumerable、writable和value

var descriptor = Object.getOwnPropertyDescriptor(book, "_year");
alert(descriptor.value); //2004

### 对象使用和属性

一种方式是使用对象字面量表示法。属性名可以是字符串也可以是Symbol类型的数据（后者必须使用方括号包含）

var person = {
name : 'Howie',
age : 26};

另一种方法是通过构造函数

functionPerson(){
this.name = 'Howie';
this.age = 26;
}
var person = new Person();

构造函数一般首字母大写，如果构造函数没有参数，使用new时后面的括号也可以省略。

ES6中可以简写

var nickname = Symbol.for('nickname');
vartype= 'cat';
var animal = { [nickname]:'mimi', type ,'comefrom':'shan xi'}

访问属性

animal[nickname]animal.typeanimal['type']animal['come from']

从上例可以看出属性名不是一个有效的变量名（比如属性名中包含空格，或者属性名是 JS的关键词）

有很多变通方法可以让数字的字面值看起来像对象。

2.toString();//出错2..toString(); //第二个点号可以正常解析2 .toString(); //注意点号前面的空格(2).toString(); //2先被计算

### 原型

JavaScript中每个函数都是Function类型的实例，每个函数都有一个 prototype（原型）属性，这个属性是一个指针，指向一个函数的原型对象，而这个对象的用途是包含可以由特定类型的**所有实例共享的属性和方法**。如果按照字面意思来理解，那么
 prototype 就是通过调用构造函数而创建的那个对象实例的原型对象。使用原型对象的好处是可以让所有对象实例共享它所包含的属性和方法。换句话说，不必在构造函数中定义对象实例的信息，而是可以将这些信息直接添加到原型对象中。

默认情况下，每个原型对象都有一个constructor属性，这个属性指向 prototype属性所在实例，也就是构造函数。创建了自定义的构造函数之后，其原型对象默认只会取得constructor属性；至于其他方法，则都是从Object继承而来的。每个对象都有[[Prototype]]属性（内部属性），虽然在脚本中没有标准的方式访问[[Prototype]]，但
 Firefox、Safari和 Chrome在每个对象上都支持一个属性`__proto__`，通过它可以访问[[Prototype]]；这个属性指向了构造函数的原型对象，JavaScript中通过递归原型链来查找对象属性，同一个构造函数创建的对象都可以访问到其原型中的属性constructor。除了通过instanceof外也可以通过person.constructor
 === Person来判断是否是Person类型，但是instanceof更加可靠，因为prototype对象完全可以被覆盖，其属性constructor也就不一定是Person了。

ECMAScript 5增加了一个新方法，叫 Object.getPrototypeOf()，在所有支持的实现中，这个方法返回[[Prototype]]的值。

每当代码读取某个对象的某个属性时，都会执行一次搜索，目标是具有给定名字的属性。搜索首先从对象实例本身开始。如果在实例中找到了具有给定名字的属性，则返回该属性的值；如果没有找到，则继续搜索指针指向的原型对象，在原型对象中查找具有给定名字的属性。如果在原型对象中找到了这个属性，则返回该属性的值。

虽然可以通过对象实例访问保存在原型中的值，但却不能通过对象实例重写原型中的值。如果我们在实例中添加了一个属性，而该属性与实例原型中的一个属性同名，那我们就在实例中创建该属性，该属性将会屏蔽原型中的那个属性。`person1.name = "Greg";`将会覆盖原型中的name属性。

使用hasOwnProperty()方法可以检测一个属性是存在于实例中，还是存在于原型中。这个方法（不要忘了它是从Object继承来的）只在给定属性存在于对象实例中时，才会返回true。

有两种方式使用in操作符：单独使用和在for-in循环中使用。在单独使用时，in操作符会在通过对象能够访问给定属性时返回true，无论该属性存在于实例中还是原型中。
```
"name"
 in person
```
为true

在使用 for-in循环时，返回的是所有能够通过对象访问的、可枚举的（enumerated）属性，其中既包括存在于实例中的属性，也包括存在于原型中的属性。屏蔽了原型中不可枚举属性（即将[[Enumerable]]标记为
 false 的属性）的实例属性也会在 for-in循环中返回，因为根据规定，所有开发人员定义的属性都是可枚举的——只有在IE8及更早版本中例外。

要取得对象上所有可枚举的实例属性，可以使用ECMAScript 5的Object.keys()方法。这个方法接收一个对象作为参数，返回一个包含所有可枚举属性的字符串数组。

var keys = Object.keys(Person.prototype); //["name","age","jbo","sayName"]
var keys = Object.getOwnPropertyNames(Person.prototype); //["constructor","name","age","jbo","sayName"]

### 创建对象

工厂模式、寄生构造函数模式、稳妥构造函数模式都是构造函数（工厂模式中就是普通函数）内部创建一个对象然后返回，通常返回的对象和构造函数没有什么联系。后两者使用new创建对象，工厂模式是直接函数调用。第三种没有公共属性，而且也不引用this的对象。

组合使用构造函数模式和原型模式结合了两者的优点，每个实例都会有自己的一份实例属性的副本，但同时又共享着对方法的引用，最大限度地节省了内存。动态原型模式则在此基础上通过检查某个应该存在的方法是否有效，来决定是否需要初始化原型。

functionPerson(name, age, job){
this.name = name;
this.age = age;
this.job = job;
//方法if (typeof this.sayName != "function"){
Person.prototype.sayName = function(){
alert(this.name);
};
}
}

需要在每个实例共享的属性应该在原型上定义，如方法，如果在this对象上定义，那么每个实例的相同函数名的函数实例实际是不同的，这样就会浪费空间。

### class

ES6提供了更接近传统语言的写法，引入了Class（类）这个概念，作为对象的模板。通过class关键字，可以定义类。基本上，ES6的class可以看作只是一个语法糖，它的绝大部分功能，ES5都可以做到，新的class写法只是让对象原型的写法更加清晰、更像面向对象编程的语法而已。

classPerson{
constructor(name, age, job){
this.name = name;
this.age = age;
this.job = job;
}
sayName(){
alert(name);
}
}
var friend = new Person("Nicholas", 29, "Software Engineer");

上面代码定义了一个“类”，可以看到里面有一个constructor方法，这就是构造方法，而this关键字则代表实例对象。注意，定义“类”的方法的时候，前面不需要加上function这个关键字，直接把函数定义放进去了就可以了。

如果对比前面的组合使用构造函数模式和原型模式，可以发现前面的Person构造函数对应现在Person类的构造方法constructor，前面Person原型上定义的sayName方法现在是类的内部方法。

一个类必须有constructor方法，如果没有显式定义，一个空的constructor方法会被默认添加。constructor方法默认返回实例对象（即this），完全可以指定返回另外一个对象。

与函数一样，Class也可以使用表达式的形式定义。

const MyClass = classMe {
getClassName() {
return Me.name;
}
};

Me只能在类内部使用

Class不存在变量提升

newFoo(); // ReferenceErrorclass Foo {}

与ES5一样，在Class内部可以使用get和set关键字，定义访问器属性。

class MyClass {
constructor() {
// ...
}
get prop() {
return 'getter';
}
set prop(value) {
console.log('setter: '+value);
}
}
inst.prop = 123;

类相当于实例的原型，所有在类中定义的方法，都会被实例继承。如果在一个方法前，加上static关键字，就表示该方法不会被实例继承，而是直接通过类来调用，这就称为“静态方法”。

class Foo {
static classMethod() {
return'hello';
}
}

Foo.classMethod() // 'hello'
var foo = new Foo();
foo.classMethod()
// TypeError: undefined is not a function```javascript父类的静态方法，可以被子类继承。ES6为new命令引入了一个`new.target`属性，（在构造函数中）返回new命令作用于的那个构造函数。如果构造函数不是通过new命令调用的，`new.target`会返回undefined，因此这个属性可以用来确定构造函数是怎么调用的。需要注意的是，子类继承父类时，`new.target`会返回子类。利用这个特点，可以写出不能独立使用、必须继承后才能使用的类。```javascriptclassShape {constructor() {
if (new.target === Shape) {
throw new Error('本类不能实例化');
}
}
}
```javascript
### 共享变量在构造函数原型上定义的属性可以被所有实例共享。
### 静态变量ES6加入了static关键字。构造函数原型链上定义的变量或属性只能通过构造函数访问，也可以当做是静态的。
### 私有变量严格来讲,JavaScript 中没有私有成员的概念;所有对象属性都是公有的。不过,倒是有一个私有变量的概念。任何在函数中定义的变量,都可以认为是私有变量,因为不能在函数的外部访问这些变量。私有变量包括函数的参数、局部变量和在函数内部定义的其他函数。我们把有权访问私有变量和私有函数的公有方法称为特权方法(privileged method)。有两种在对象上创建特权方法的方式。第一种是在构造函数中定义特权方法```javascript
function MyObject(){
//私有变量和私有函数var privateVariable = 10;
functionprivateFunction(){
returnfalse;
}
//特权方法this.publicMethod = function(){
privateVariable++;
return privateFunction();
};
}

### 静态私有变量

(function(){
//私有变量和私有函数var privateVariable = 10;
functionprivateFunction(){
returnfalse;
}
//构造函数MyObject = function(){
};
//公有/特权方法MyObject.prototype.publicMethod = function(){
privateVariable++;
return privateFunction();
};
})();

注意,这个模式在定义构造函数时并没有使用函数声明,而是使用了函数表达式。函数声明只能创建局部函数,但那并不是我们想要的。出于同样的原因,我们也没有在声明
 MyObject 时使用 var关键字。但也要知道,在严格模式下给未经声明的变量赋值会导致错误。

### 模块模式

模块模式通过为单例添加私有变量和特权方法能够使其得到增强

var singleton = function(){
//私有变量和私有函数var privateVariable = 10;
functionprivateFunction(){
returnfalse;
}
//特权/公有方法和属性return {
publicProperty: true,
publicMethod : function(){
privateVariable++;
return privateFunction();
}
};
}();

### 增强的模块模式

有人进一步改进了模块模式,即在返回对象之前加入对其增强的代码。这种增强的模块模式适合那些单例必须是某种类型的实例,同时还必须添加某些属性和(或)方法对其加以增强的情况。

var singleton = function(){
//私有变量和私有函数var privateVariable = 10;
functionprivateFunction(){
returnfalse;
}
//创建对象var object = new CustomType();
//添加特权/公有属性和方法object.publicProperty = true;
object.publicMethod = function(){
privateVariable++;
return privateFunction();
};
//返回这个对象return object;
}();

## 继承

JavaScript 不包含传统的类继承模型，而是使用 prototype原型模型。

### 原型链

其基本思想是利用原型让一个引用类型继承另一个引用类型的属性和方法。简单回顾一下构造函数、原型和实例的关系：每个构造函数都有一个原型对象，原型对象都包含一个指向构造函数的指针，而实例都包含一个指向原型对象的内部指针。那么，假如我们让原型对象等于另一个类型的实例，结果会怎么样呢？显然，此时的原型对象将包含一个指向另一个原型的指针，相应地，另一个原型中也包含着一个指向另一个构造函数的指针。假如另一个原型又是另一个类型的实例，那么上述关系依然成立，如此层层递进，就构成了实例与原型的链条。这就是所谓原型链的基本概念。

### 继承方法

组合继承(combination inheritance) ,有时候也叫做伪经典继承,指的是将原型链和借用构造函数的技术组合到一块,从而发挥二者之长的一种继承模式。其背后的思路是使用原型链实现对原型属性和方法的继承,而通过借用构造函数来实现对实例属性的继承。

functionSuperType(name){
this.name = name;
this.colors = ["red", "blue", "green"];
}
SuperType.prototype.sayName = function(){
alert(this.name);
};
functionSubType(name, age){
//继承属性SuperType.call(this, name); //第二次调用 SuperType()
this.age = age;
}
//继承方法SubType.prototype = new SuperType(); ///第一次调用 SuperType()
SubType.prototype.constructor = SubType;
SubType.prototype.sayAge = function(){
alert(this.age);
};
var instance1 = new SubType("Nicholas", 29);
instance1.colors.push("black");
alert(instance1.colors); //"red,blue,green,black"instance1.sayName(); //"Nicholas";
instance1.sayAge(); //29var instance2 = new SubType("Greg", 27);
alert(instance2.colors); //"red,blue,green"instance2.sayName(); //"Greg";
instance2.sayAge(); //27

如果知识用原型链继承，那么像colors这样的引用类型会被子类的所有实例共享，一个实例更改colors，所有都会更改，这不是我们想要的，而结合借用构造函数的继承方法，在构造函数内调用父类的构造方法，通过使用
 apply() 和 call()方法也可以在(将来)新创建的对象上执行构造函数，则解决了所有子类实例共享colors的问题。

组合继承是 JavaScript最常用的继承模式;不过,它也有自己的不足。组合继承最大的问题就是无论什么情况下,都会调用两次超类型构造函数:一次是在创建子类型原型的时候,另一次是在子类型构造函数内部。没错,子类型最终会包含超类型对象的全部实例属性,但我们不得不在调用子类型构造函数时重写这些属性。

在第一次调用 SuperType构造函数时, SubType.prototype会得到两个属性:
 name和 colors ;它们都是 SuperType的实例属性,只不过现在位于
 SubType的原型中。当调用 SubType构造函数时,又会调用一次
 SuperType构造函数,这一次又在新对象上创建了实例属性name和
 colors。

寄生组合式继承,即通过借用构造函数来继承属性,通过原型链的混成形式来继承方法。其背后的基本思路是:不必为了指定子类型的原型而调用超类型的构造函数,我们所需要的无非就是超类型原型的一个副本而已。本质上,就是使用寄生式继承来继承超类型的原型,然后再将结果指定给子类型的原型。

functionobject(o){
functionF(){}
F.prototype = o;
returnnew F();
}
functioninheritPrototype(subType, superType){
var prototype = object(superType.prototype); //创建对象prototype.constructor = subType; //增强对象subType.prototype = prototype; //指定对象}

在函数内部,第一步是创建超类型原型的一个副本。第二步是为创建的副本添加constructor属性,从而弥补因重写原型而失去的默认的
 constructor属性。最后一步,将新创建的对象(即副本)赋值给子类型的原型。

ECMAScript 5 新增 Object.create()方法。这个方法接收两个参数:一个用作新对象原型的对象和(可选的)一个为新对象定义额外属性的对象。在传入一个参数的情况下,Object.create()与
 object()方法的行为相同

functionSuperType(name){
this.name = name;
this.colors = ["red", "blue", "green"];
}
SuperType.prototype.sayName = function(){
alert(this.name);
};
functionSubType(name, age){
SuperType.call(this, name);
this.age = age;
}
inheritPrototype(SubType, SuperType);
SubType.prototype.sayAge = function(){
alert(this.age);
};

YUI 的 YAHOO.lang.extend()方法采用了寄生组合继承

### extends

Class之间可以通过extends关键字实现继承，这比ES5的通过修改原型链实现继承，要清晰和方便很多。

classSuperType{
constructor(name){
this.name = name;
}
sayName(){
alert(this.name);
}
}
classSubTypeextendsSuperType{
constructor(name, age){
super(name);
this.age = age;
}
sayAge(){
alert(this.age);
super.sayName();
}
}

子类必须在constructor方法中调用super方法，否则新建实例时会报错。这是因为子类没有自己的this对象，而是继承父类的this对象，然后对其进行加工。如果不调用super方法，子类就得不到this对象。

对比借用构造函数继承，实质是先创建子类的实例对象this，然后再将父类的属性添加到this上面（`Parent.apply(this)`）。ES6的继承机制则不同，实质是先创建父类的实例对象this（所以必须先调用super方法），然后再用子类的属性修改this。

如果子类没有定义constructor方法，这个方法会被默认添加，代码如下。也就是说，不管有没有显式定义，任何一个子类都有constructor方法。

另一个需要注意的地方是，在子类的构造函数中，只有调用super之后，才可以使用this关键字，否则会报错。这是因为子类实例的构建，是基于对父类实例加工，只有super方法才能返回父类实例。

大部分浏览器实现中，每一个对象都有`__proto__`属性，指向对应的构造函数的prototype属性。Class作为构造函数的语法糖，同时有prototype属性和`__proto__`属性，因此同时存在两条继承链。

1.     子类的`__proto__`属性，表示构造函数的继承，总是指向父类。

2.     子类prototype属性的`__proto__`属性，表示方法的继承，总是指向父类的prototype属性。

SubType.__proto__ === SuperType // true
SubType.prototype.__proto__ === SuperType.prototype // true

下面，讨论三种特殊情况。

第一种特殊情况，子类继承Object类。

classAextendsObject{
}

A.__proto__ === Object// trueA.prototype.__proto__ === Object.prototype // true

第二种特殊情况，不存在任何继承。

classA{
}

A.__proto__ === Function.prototype // trueA.prototype.__proto__ === Object.prototype // true

第三种特殊情况，子类继承null。

classAextendsnull{
}

A.__proto__ === Function.prototype // trueA.prototype.__proto__ === undefined // true

Object.getPrototypeOf方法可以用来从子类上获取父类。

Object.getPrototypeOf(SubType) === SuperType // true

因此，可以使用这个方法判断，一个类是否继承了另一个类。

原生构造函数是指语言内置的构造函数，通常用来生成数据结构，比如`Array()`。以前，这些原生构造函数是无法继承的，即不能自己定义一个Array的子类。

functionMyArray() {
Array.apply(this, arguments);
}

MyArray.prototype = Object.create(Array.prototype, {
constructor: {
value: MyArray,
writable: true,
configurable: true,
enumerable: true}
});

上面代码定义了一个继承Array的MyArray类。但是，这个类的行为与Array完全不一致。

var colors = new MyArray();
colors[0] = "red";
colors.length// 0colors.length = 0;colors[0]//"red"

之所以会发生这种情况，是因为原生构造函数无法外部获取，通过`Array.apply()`或者分配给原型对象都不行。ES5是先新建子类的实例对象this，再将父类的属性添加到子类上，由于父类的属性无法获取，导致无法继承原生的构造函数。

ES6允许继承原生构造函数定义子类，因为ES6是先新建父类的实例对象this，然后再用子类的构造函数修饰this，使得父类的所有行为都可以继承。下面是一个继承Array的例子。

classMyArrayextendsArray{
constructor(...args) {
super(...args);
}
}

var arr = newMyArray();
arr[0] = 12;
arr.length // 1arr.length = 0;arr[0] // undefined

## 原生类型

### Object

JavaScript语言的所有对象都是由Object衍生的对象；所有对象都继承了Object.prototype的方法和属性，尽管它们可能被覆盖。

构建方法的参数为空，null或undefined将返回一个空对象，参数为String，Number,Boolean类型相当于使用其对应包装类型创建对象。对于其它类型的对象会返回原对象。
|**函数**|**描述**|
|----|----|
|Object.assign(target, …sources)|把任意多个的源对象所拥有的自身可枚举属性拷贝给目标对象，然后返回目标对象。|
|Object.create(proto, [ propertiesObject ])|创建具有指定原型并可选择包含指定属性的对象。|
|Object.defineProperties(obj, props)|将一个或多个属性添加到对象，和/或修改现有属性的特性，并返回该对象。|
|Object.defineProperty(obj, prop, descriptor)|将属性添加到对象，或修改现有属性的特性，并返回该对象。|
|Object.freeze(obj)|冻结对象是指那些不能添加新的属性，不能修改已有属性的值，不能删除已有属性，以及不能修改已有属性的可枚举性、可配置性、可写性的对象。也就是说，这个对象永远是不可变的。该方法返回被冻结的对象。|
|Object.getOwnPropertyDescriptor(obj, prop)|返回指定对象上一个自有属性对应的属性描述符。（自有属性指的是直接赋予该对象的属性，不需要从原型链上进行查找的属性）|
|Object.getOwnPropertyNames(obj)|返回一个由指定对象的所有自身属性的属性名（包括不可枚举属性）组成的数组。|
|Object.getOwnPropertySymbols(obj)|返回一个数组，该数组包含了指定对象自身的（非继承的）所有 symbol属性键。|
|Object.getPrototypeOf(object)|返回指定对象的原型（也就是该对象内部属性[[Prototype]]的值）。|
|Object.is(value1, value2)|返回一个值，该值指示两个值是否相同。它与严格比较运算符（===）的行为基本一致，不同之处只有两个：一是+0不等于-0，二是NaN等于自身|
|Object.isExtensible(obj)|返回指示是否可将新属性添加到对象的值。|
|Object.isFrozen(obj)|判断一个对象是否被冻结（frozen）|
|Object.isSealed(obj)|判断一个对象是否是密封的（sealed）|
|Object.keys(obj)|返回一个由给定对象的所有可枚举自身属性的属性名组成的数组，数组中属性名的排列顺序和使用for-in循环遍历该对象时返回的顺序一致（两者的主要区别是 for-in 还会遍历出一个对象从其原型链上继承到的可枚举属性）。|
|Object.preventExtensions(obj)|让一个对象变的不可扩展，也就是永远不能再添加新的属性|
|Object.seal(obj)|可以让一个对象密封，并返回被密封后的对象。密封对象是指那些不能添加新的属性，不能删除已有属性，以及不能修改已有属性的可枚举性、可配置性、可写性，但可能可以修改已有属性的值的对象。|
|Object.setPrototypeOf(object, prototype)|设置一个对象的原型(既对象的[[Prototype]]内部属性)。|

Object原型属性

·        Object.prototype.constructor：返回一个指向创建了该对象原型的函数引用

·        Object.prototype.**proto**：非标准，一个对象的**proto**属性和自己的内部属性[[Prototype]]指向一个相同的值
 (通常称这个值为原型),原型的值可以是一个对象值也可以是null(比如说Object.prototype.**proto**的值就是null)

Object实例方法
|**方法**|**描述**|
|----|----|
|obj.hasOwnProperty(prop)|用来判断某个对象是否含有指定的自身属性|
|prototype.isPrototypeOf(obj)|测试一个对象是否存在于另一个对象的原型链上|
|obj.propertyIsEnumerable(prop)|返回一个布尔值，表明指定的属性名是否是当前对象可枚举的自身属性|
|obj.toLocaleString();|返回一个该对象的字符串表示。该方法主要用于被本地化相关对象覆盖。|
|obj.toString()|返回一个代表该对象的字符串。|
|obj.valueOf()|返回一个对象的值，默认情况下返回对象本身。|

## 匿名函数

（function(){//代码 })();//可立即执行的匿名函数

解释：，包围函数（function(){})的第一对括号向脚本返回未命名的函数，随后一对空括号立即执行返回的未命名函数，括号内为匿名函数的参数。

例子：(function(arg){ alert(arg+100); })(20);

这个例子返回120。

1.匿名函数，就是没有引用直接加上()进行执行的函数。

var addFunc = function(a,b){

cosole.log(a+b);

};

--调用方式

addFunc(1,2);

--匿名函数

(function(a,b){console.log(a+b)})(1,2);

上面两个函数的产生的结果是一样的.只不过addFunc为函数对象的引用,而匿名函数不存在引用.

--来看个复杂的利用匿名函数的例子

var add = (function(){

varsum = 0 ;

returnfunction(){

for(vari=0;l=arguments.length,i<l;i++){

sum+=arguments[i];

}

returnsum;

};

})(); --加上括号为匿名函数的写法,可以理解为直接执行了此函数,函数的执行结果返回

add(1,2,3,4);

add(2);

--等价于

var func1 = function(){

varsum = 0 ;

returnfunction(){

for(vari=0;l=arguments.length,i<l;i++){

sum+=arguments[i];

}

returnsum;

};

};

var add = func1();

add(1,2,3,4);

add(2);

//现在有一个场景，需要设计一个函数，前面每次调用，不会真正去计算，而是把参数保存下来，等到需要计算的再一起计算

var currying = function(fn){

varargs = [] ;

returnfunction(){

if(arguments.length=== 0){

returnfn.apply(this,args);

}else{

Array.prototype.push.apply(args,arguments);

returnarguments.callee;

}

};

}

var add = (function(){

varsum = 0 ;

returnfunction(){

for(vari=0;l=arguments.length,i<l;i++){

sum+=arguments[i];

}

returnsum;

};

})();

var add= currying(add);

add(1);

add(2);

add(3);

add();

## 闭包

可以理解成一个函数内的内部函数，它被创建在一个函数内部，引用这其外部函数的变量值。

var a = function(){

varx = 1;

return function(y){

console.log(x+=y);//外部函数的x值就被包裹在引用对象中

}

}

var b = a();//实际上是返回function(y){...}的引用

b();//执行上面的引用，此时的x值变成了private的属性

闭包的两个作用：

1.可以读取函数体所在外部函数的局部变量

2.就是让这些变量的值始终保持在内存中

## Constructor构造函数

constructor 属性返回对创建此对象的数组函数的引用。

var test=new Array();

if (test.constructor==Array)//判断test的构造函数是否是Array

{

document.write("This is anArray");

}

var createPerson = function(name,sex,age){

this.name = name;

this.sex = sex;

this.age = age;

};

//undefined

var p1 = new createPerson('z1',19,'男');

//undefined

p1.constructor

//输出

function (name,sex,age){

this.name = name;

this.sex = sex;

this.age = age;

}

p1.constructor == createPerson

//输出：true

createPerson.constructor

//Function() { [native code] }//createPerson构造函数为Function,从一个侧面说明function也是对象

## 利用JS特性实现AOP

AOP就是面向切面编程，主要作用是把一些核心业务逻辑模块无关的功能抽离出来，这些跟业务逻辑无关的功能

通常包括日志统计，安全控制，异常处理等。把这些功能抽离出来后，又通过动态织入的方式渗入到业务逻辑模块中

在JAVA中，可以通过反射和动态代理机制来实现AOP。但对于javascript这种动态语言，AOP的实现更加简单

在javascript中实现AOP，都是指把一个函数动态织入到另外一个函数中。我们可以通过扩展Function.prototype来做到这一点

*/

Function.prototype.before =function(beforefn){

var that = this ;

return function(){

beforefn.apply(this,arguments);

return that.apply(this,arguments);

};

};

Function.prototype.after =function(afterfn){

var that = this ;

return function(){

var ret =that.apply(this,arguments);

afterfn.apply(this,arguments);

return ret ;

};

};

var func = function(){

console.log(2);

};

func = func.before(function(){

console.log(1);

}).after(function(){

console.log(3);

});

func();

var func1 = document.getElementById;

undefined

func1('nt-contents');

//报错信息如下,是由于document.getElementById函数中的this指向为windows对象,而windows对象不存在这些变量。

VM245:1 Uncaught TypeError: Illegalinvocation(…)

//写那么复杂主要是将this的指向还是定位到document上面

var getElementById1 = (function(func){

returnfunction(){

returnfunc.apply(document,arguments);

}

})(document.getElementById);

var getId = document.getElementById1; //返回这个对象function(){return func.apply(document,arguments);}

var div = getId('ntp-contents');//返回func.apply(document,arguments)的对象，arguments参数值div1,func执行闭包中的document.getElementById函数

alert(div.id);

//怎么在一个内部函数中保持外层的this上下使用了一个变量that来保存外,还可以使用以下方法

Function.prototype.bind =function(context){

varself = this ;

returnfunction(){

alert(self);

alert(context);

returnself.apply(context,arguments);

}

};

## SetInterval与setTimeout的区别

setTimeout和setInterval的语法相同。它们都有两个参数，一个是将要执行的代码字符串，还有一个是以毫秒为单位的时间间隔，当过了那个时间段之后就将执行那段代码。

不过这两个函数还是有区别的，setInterval在执行完一次代码之后，经过了那个固定的时间间隔，它还会自动重复执行代码，而setTimeout只执行一次那段代码。

区别：

window.setTimeout("function",time)；//设置一个超时对象，只执行一次,无周期 。

window.setInterval("function",time)；//设置一个超时对象，周期＝'交互时间'。

停止定时： 

window.clearTimeout(对象) 清除已设置的setTimeout对象。

window.clearInterval(对象) 清除已设置的setInterval对象。

如果针对的是不断运行的代码，**不应该使用setTimeout，而应该是用setInterval**，因为setTimeout每一次都会初始化一个定时器，而setInterval只会在开始的时候初始化一个定时器。

代码如下：

PerRefresh();

function PerRefresh() {

     var today = new Date();

     alert("The time is: " + today.toString());

     setTimeout("showTime()", 5000);

}

一旦调用了这个函数PerReflesh，那么就会每隔5秒钟就显示一次时间。

代码如下：

setInterval("PerRefresh()",5000);

function PerRefresh() {

     var today = new Date();

     alert("The time is: " + today.toString());

}

## javascript取值的时候什么时候用value什么时候用innerHTML？

对于表单控件,都可以用value属性,对于非表单控件,可以用innerHTML

INPUT标签都有value属性,但都没有innerHTML属性,所以只能用value

SELECT标签和OPTION标签,即有value属性也有innerHTML属性,但是一个是取回值,一个是取回文本,这两个可能相同也可能不同,具体要看你想要哪个值.

TEXTAREA标签也没有innerHTML属性,有value属性和innerText属性.

## 事件冒泡或事件捕获？

事件传递有两种方式：冒泡与捕获。

事件传递定义了元素事件触发的顺序。如果你将<p>元素插入到 <div>元素中，用户点击
 <p>元素,哪个元素的 "click"事件先被触发呢？

在*冒泡*中，内部元素的事件会先被触发，然后再触发外部元素，即： <p>元素的点击事件先触发，然后会触发
 <div>元素的点击事件。

在*捕获*中，外部元素的事件会先被触发，然后才会触发内部元素的事件，即： <div>元素的点击事件先触发，然后再触发
 <p>元素的点击事件。

addEventListener()方法可以指定"useCapture"参数来设置传递类型：

addEventListener(*event*,*function*,*useCapture*);

默认值为false,即冒泡传递，当值为 true时,事件使用捕获传递。

实例：

[http://www.runoob.com/try/try.php?filename=tryjs_addeventlistener_usecapture](http://www.runoob.com/try/try.php?filename=tryjs_addeventlistener_usecapture)

## 事件类型
|load|页面加载后|||
|----|----|----|----|
|resize|窗口或框架变化时|||
|scroll|滚动条改变时|||
|焦点事件|blur|失去焦点||
|focus|获得焦点|| |
|鼠标与滚轮事件|click|单击||
|dblclick|双击|| |
|mousedown|按下鼠标|| |
|mouseenter|鼠标移动到元素内部触发|| |
|mouseleave|鼠标移动到元素外部触发|| |
|mousemove|鼠标在元素内部重复触发|| |
|mouseout|元素内部到外部触发，只触发一次,有bug|| |
|mouseover|元素外部到内部触发，只触发一次，有bug|| |
|mouseup|释放鼠标|| |
|mousewheel|IE和chrome的滚轮事件|event.wheelDelta<0 滑轮向下滚| |
|DOMMouseScroll|火狐的滚轮事件|evnet.detail>0 滑轮向下滚| |
|键盘与文本事件|keydown|按下键盘任意键，如果不放，会一直触发|如果按下键盘的字符键，keydown首先执行，然后再执行keypress|
|keypress|按下键盘字符键，如果不放，会一直触发| | |
|keyup|释放键盘时触发|| |
|触摸事件|touchstart|手指触摸屏幕时触发||
|touchmove|手指在屏幕滑动时连续触发|| |
|touchend|手指移开屏幕|| |
|touchcancel|手指停留在屏幕不动时|| |
|touches|触摸了多少个手指|| |
|手势事件|gesturestart|一个手指按在屏幕而另一个手机又触摸屏幕时||
|gesturechange|当触摸屏幕的任何一个手指的位置发生变化时|| |
|gestureend|任何一个手指从屏幕上移开时|| |
|拖放事件|dragstart|拖拽对象---按下触发|拖拽对象要加上（draggable="true"）|
|drag|拖拽对象---相当于mousemove| | |
|dragend|拖拽对象---拖拽停止| | |
|dragenter|投放的地方---移动到投放区| | |
|dragover|投放的地方---相当于mousemove| | |
|dragleave|投放的地方---离开投放区| | |
|drop|投放的地方---投进去了| | |
|其他事件|contextmenu|右击出现菜单栏||
|DOMContentLoaded|就是非IE的ready事件|| |
|设备事件|deviceorientation|检测设备方向的变化---重力感应|event.alpha=z轴 event.beta=x轴 event.gamma=y轴|

## JavaScript for...in 语句循环遍历对象的属性。

**实例**

varperson={fname:"John",lname:"Doe",age:25}; 

for (x in person) 

{ 

    txt=txt + person[x]; 

} 

## JS对象与JSON格式数据相互转换

JS对象转换成为JSON 

var  jsonData = {};
jsonData.MainCmdID = 1;
jsonData.SubCmdID = 1;
var toStr =JSON.stringify(jsonData);
alert(toStr);

JSON转换成为JS

var result2 = $.parseJSON( '{ "name": "CodePlayer","age": 1 }' ); //
一个Object对象

alert( result2.name ); // CodePlayer

## JS文件加密解密

对于JAVASCRIPT函数escape()和unescape()想必是比较了解啦（很多网页加密在用它们），分别是编码和解码字符串，比如例子代码
用escape()函数加密后变为如下格式：

alert%28%22%u9ED1%u5BA2%u9632%u7EBF%22%29%3B 

如何？还看的懂吗？当然其中的ASCII字符"alert"并没有被加密，如果愿意我们可以写点JAVASCRIPT代码重新把它加密如下：

%61%6C%65%72%74%28%22%u9ED1%u5BA2%u9632%u7EBF%22%29%3B 

当然，这样加密后的代码是不能直接运行的，幸好还有eval(codeString)可用，这个函数的作用就是检查JavaScript代码并执行，必选项
codeString 参数是包含有效 JavaScript
代码的字符串值，加上上面的解码unescape()，加密后的结果如下：

<SCRIPT LANGUAGE="JavaScript"> 

var code=unescape("%61%6C%65%72%74%28%22%u9ED1%u5BA2%u9632%u7EBF%22%29%3B");

eval(code) 

</SCRIPT> 

对于此加密的解密，也就同样的简单，（unescape()）

Js混淆压缩工具：[http://tool.chinaz.com/js.aspx](http://tool.chinaz.com/js.aspx)

## JavaScript 语言基础知识点

（来自：[http://t.cn/zjbXMmi](http://t.cn/zjbXMmi)[@刘巍峰](http://weibo.com/n/%E5%88%98%E5%B7%8D%E5%B3%B0) 分享
 ）

![](http://img3.tbcdn.cn/tfscom/T1qtogXc8fXXartXjX.gif)

![](http://img2.tbcdn.cn/tfscom/T10d.hXa0eXXartXjX.gif)

![](http://img3.tbcdn.cn/tfscom/T1ltAjXmXbXXartXjX.gif)

![](http://img2.tbcdn.cn/tfscom/T1wgEiXfVfXXartXjX.gif)

![](http://img3.tbcdn.cn/tfscom/T1GukhXgXcXXartXjX.gif)

![](http://img2.tbcdn.cn/tfscom/T1A1kiXaFaXXartXjX.gif)

![](http://img1.tbcdn.cn/tfscom/T1WoIgXchdXXartXjX.gif)

![](http://img3.tbcdn.cn/tfscom/T1jsshXgRdXXartXjX.gif)

![](http://img3.tbcdn.cn/tfscom/T14Nb_XjNhXXartXjX.gif)

![](http://img3.tbcdn.cn/tfscom/T1AGMiXjRaXXartXjX.gif)

