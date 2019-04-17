# JavaScript学习小结 - 长歌行 - CSDN博客





2013年04月17日 13:27:57[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：2167








### 学习目的：

1.Web相关开发越来越流行，学习JS十分有必要

2.多学习一种语言，想多了解一种语言的文化内涵

3.认识一下脚本语言，之前一直学习C，C++，换换口味




### 学习途径：

1.之前实习期间的项目积累

2.互联网的各种零碎的资料

3.codecademy的在线Js教学课程（冗长细致的课程，打字打到手抽筋）

4.各种书本，如《headfirst Js》等




### 零星的感受：（一直在补充）



1.js中类的属性，可以使用.也可以使用["xx"]来标识


2.JS也有封装，在类的构造函数中使用 var来定义属性或者方法而不是this


3.Js的函数定义之后没有分好，但是变量定义之后有分号。


4.函数和类中的this不能省略


5.Js的实例化是通过 new 构造函数实现的。

function Person(name,age) {


```java
this.name = name;
  this.age = age;
}
// Let's make bob and susan again, using our constructor
var bob = new Person("Bob Smith", 30);
```


6.使用prototype使得每个实例都有这个属性，也实现了继承


```java
// the original Animal class and sayName method
function Animal(name, numLegs) {
    this.name = name;
    this.numLegs = numLegs;
}
Animal.prototype.sayName = function() {
    console.log("Hi my name is "+this.name);
};

// define a Penguin class
function Penguin(name, numLegs) {
    this.name = name;
    this.numLegs = 2;
}

// set its prototype to be a new instance of Animal
Penguin.prototype = new Animal();

var penguin = new Penguin("Gigi");
penguin.sayName();
```







