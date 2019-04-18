# JS 创建对象 - z69183787的专栏 - CSDN博客
2013年11月07日 14:54:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：823
个人分类：[Javascript-概述](https://blog.csdn.net/z69183787/article/category/2175033)
```java
http://www.cnblogs.com/tiwlin/archive/2009/08/06/1540161.html
http://www.blogjava.net/CopyHoo/articles/244547.html
/* 创建对象的几种方式: */
// 1. 工厂方法:能创建并返回特定类型对象的工厂函数(factory function).
function createCar(sColor){
var car = new Object(); // 或者 var car = new Object ;
// 对象属性
car.color = sColor ; 
// 对象方法
car.showColor = function (){ 
alert(123);
} ; // 记住，这里一定要用 ; 表示结束
return car; // 这里是 return car ; 而不是 return this.car ; 因为 this.car 为 undefined
}
/* 
调用此函数时，将创建对象，并赋予它所有必要的属性，使用此方法创建 car 对象的两个版本，
( oCar1 和 oCar2 ) ,他们的属性完全一样。
使用此方法存在的问题:
1. 语义上看起来不像使用带有构造函数的 new 运算那么正规.
2. 使用这种方式必须创建对象的方法。每次调用 createCar()，都要创建 showColor(),意味着每一个对象
都有自己的 showColor 版本，事实上，每一个对象都共享了是同一个函数.
有些开发者在工厂函数外定义对象的方法，然后通过属性指向该方法。从而避免这个问题:
*/
function createCar2(sColor){
var car = new Object();
car.color = sColor ;
car.showColor = showColor ;
return car ;
}
function showColor(){
alert(this.color);
}
var oCar1 = createCar('red');
var oCar2 = createCar('yellow');
var oCar3 = createCar2('blue');
var oCar4 = createCar2('black');
/*
注意这两个对象(oCar3 和 oCar4 ) 调用showColor 属性的方式，虽然美其名曰是“属性”，其实还是方法!!!
所以是 oCar3.showColor(); 而不是 oCar3.showColor ;
*/
oCar3.showColor();
oCar4.showColor();
/*
在这段重写的代码中，在函数 createCar2() 前定义了函数showColor(), 在 createCar2() 内部，赋予对象一个已经
指向已经存在的 showColor() 函数的指针，从功能上讲，这样解决了重复创建对象的问题，但该函数看起来不像对象
的方法。
所有这些问题引起了开发者的定义构造函数的出现
*/
// 2. 构造函数方式
function Car(sColor){
this.color = sColor;
this.showColor = function(){
alert(this.color);
};
}
var car1 = new Car('red');
car1.showColor();
/*
你可能已经注意到第一个的差别了，在构造函数内部无创建对象，而是使用 this 关键字，使用 new 
运算符调用构造函数，在执行第一行代码前先创建一个对象，只有用 this 才能访问该对象。然后可以
直接赋予 this 属性，默认情况下是构造函数的返回值，（不必明确使用 return 运算符）。
这种方式在管理函数方面与工厂方法一样都存在相同的问题.
*/
// 3. 原型方式
function PCar(){
}
PCar.prototype.color = "blue";
var pcar1 = new PCar();
/*
调用 new Car()时，原型的所有属性都被立即赋予要创建的对象，意味着所有的 PCar 实例存放的是指向
showColor() 函数的指针，从语义看起来都属于一个对象，因此解决了前面两种方式存在的问题。此外使用
该方法，还能使用 instanceof 运算符检查给定变量指向的对象类型。因此下面的代码将输出 true:
*/
alert(pcar1 instanceof PCar); // output "true"
/*
这个方法看起来不错，遗憾的是，它并不尽人意。
1. 首先这个构造函数没有参数。使用原型方式时，不能给构造函数传递参数初始化属性值，因为 pcar1 和
pcar2 的属性都等于 "blue"
2. 真正的问题出现在属性指向的对象,而不是函数时，函数共享不会造成任何问题，但是对象却是很少被多个
实例共享的。
*/
// 4. 混合的构造函数/原型方式(推荐)
/*
联合使用构造函数和原型方式，就可像使用其他程序设计语言一样创建对象，这种概念非常简单，即用构造函数
定义对象的所有非函数属性，用原型方式定义对象的函数属性(方法)。
*/
function hCar(sColor){
this.color = sColor; 
this.drivers = new Array('Mike','Sue');
}
hCar.prototype.showColor = function(){
alert(this.color);
}
var hcar1 = new hCar('y color');
var hcar2 = new hCar('r color');
hcar1.drivers.push('Matt');
alert(hcar1.drivers); // output "Mike,Sue,Matt"
alert(hcar2.drivers); // output "Mike,Sue"
// 5. 动态原型方式 (推荐)
/*
对于习惯使用其他开发语言的开发者来说，使用混合构造函数/原型方式感觉不那么和谐。批评构造函数/原型方式的人
认为，在构造函数内找属性，在外部找方法的做法不合理。所以他们设计了动态原型方式，以供更友好的编码风格。
动态原型方式的基本想法与混合构造函数/原型方式 相同，即在构造函数内定义非函数的属性，而函数的属性则利用
原型属性定义。唯一的区别是赋予对象方法的位置。下面是使用动态原型方法重写的 Car 类:
*/
function DCar(sColor){
this.color = sColor;
this.drivers = new Array('Mike','Sue');
if(typeof DCar._initialized == 'undefined'){
DCar.prototype.showColor = function(){
alert(this.color);
}
}
DCar._initialized = true;
}
var dcar1 = new DCar('y dcar');
var dcar2 = new DCar('b dcar');
dcar1.showColor();
dcar2.showColor();
alert(DCar._initialized); // output "true"
alert(dcar1._initialized); // output "undefined"
// 6、对象直接量、new创建
创建对象最简单的方法是你的javascript代码中包含对象直接量，也可以通过运算符new创建。
var empty = {}; // An object with no properties
var point = { x:0, y:0 };
var circle = { x:point.x, y:point.y+1, radius:2 };
var homer = {
"name": "Homer Simpson",
"age": 34,
"married": true,
"occupation": "plant operator",
'email': homer@example.com
};
var a = new Array(); // Create an empty array
var d = new Date(); // Create an object representing the current date and time
var r = new RegExp("javascript", "i"); // Create a pattern-matching object
　　创建对象后，我们可以通过"."运算符，在对象中创建新属性、引用已有属性、设置属性值等。
　　　　var book = new Object(); 　　//创建对象
　　　　book.title="JavaScript: The Definitive Guide";
　　　　book.chapter1=new Object(); //作为对象属性的，嵌套对象
　　　　book.chapter1.title = "Introduction to JavaScript";
　　　　book.chapter1.pages = 11;
　　　　book.chapter2 = { title: "Lexical Structure", pages: 6 };
alert("Outline: " + book.title + "\n\t" +
　　　　　　"Chapter 1 " + book.chapter1.title + "\n\t" +
　　　　　　"Chapter 2 " + book.chapter2.title); // 从对象中读取一些属性.
在上例中，需注意，可以通过把一个值赋给对象的一个新属性来创建它．
　　在JavaScript语句中提到过用for/in语句可以遍历对象的属性和方法。
删除属性：delete book.chapter2;
```
