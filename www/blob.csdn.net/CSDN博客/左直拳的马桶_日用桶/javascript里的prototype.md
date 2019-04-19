# javascript里的prototype - 左直拳的马桶_日用桶 - CSDN博客
2014年12月10日 19:43:02[左直拳](https://me.csdn.net/leftfist)阅读数：1627
个人分类：[node.js																[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)](https://blog.csdn.net/leftfist/article/category/2626561)
在javascript中，prototype是函数的一个固有属性，其他对象，比如字符串什么的，并没有这个属性。
这个属性做什么用呢?
1、用于该函数的所有实例进行共享
比如，共同的属性，共同的方法。类似C#里的静态全局变量。所有实例，是指通过new的方法，来进行构造该函数的实例对象。比如说
```java
var Person = function(name) {
  this.name = name;
};
var z3 = new Person("z3");
var l4 = new Person("l4");
```
z3、l4都是Person的实例。
那么这个时候或者在new之前，往Person的prototype添加属性，方法，两个实例z3、l4都能访问：
```java
var Person = function(name) {
	this.name = name;
};
var z3 = new Person("z3");
var l4 = new Person("l4");
Person.prototype.sayHi = function(hi) {
	console.log(this.name + ":" + hi);
}; 
z3.sayHi("Hello World!");
l4.sayHi("Hello World!");
```
所以说，“prototype 属性使您有能力向对象添加属性和方法。”
2、public方法
也是由于这个特性，对于一个javascript的“类”(其实是函数了)来说，添加在prototype的方法是public方法，而内部的方法，则是private的。
```java
var Person = function(name) {
	this.name = name;
	Person.prototype.sayHi = function(hi) {//外部可以访问
		console.log(this.name + ":" + hi);
	}; 
	this.sayHi2 = function(hi){//外部可以访问
		console.log(this.name + ":" + hi);
	}
	var _sayHi = function(hi){//外部不能访问
		console.log(hi);
	}
};
var z3 = new Person("z3");
z3.sayHi("Hello World!");//OK
z3.sayHi2("hehehehe");//OK
z3._sayHi("hahahaha");//报错
```
3、继承
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41845367#)[copy](http://blog.csdn.net/leftfist/article/details/41845367#)
- var Father, Son, father, son;    
- 
- Father = (function() {    
- function father(name) {//相当于构造函数了。这个构造函数很重要，下面的添加成员函数，属性，都靠它。  
- this.name = name;    
-   }    
- 
-   father.prototype.sayHi = function(hi) {  
- return console.log(this.name + " : " + hi);    
-   };    
- 
- return father;  
- 
- })();    
- 
- Son = (function() {    
- function Son(name) {    
- this.name = name;    
-   }    
- 
-   Son.prototype = new Father();//继承的诀窍在于此  
-   Son.prototype.cry = function(){ console.log("噢噢噢");}//子类扩展  
- return Son;    
- 
- })();    
- 
- son = new Son("leftfist");    
- son.sayHi("Hello World!");    
- son.cry();  
有关这个prototype的来历，阮一峰的博文有详细介绍，从中可以理解为什么会有这个东西。主要是因为当初设计者不想将javascript设计得那么复杂，搞成一个面向对象那么正式的语言，但又要照顾对象的继承等属性，因此提供了这么一个东东。
[http://www.ruanyifeng.com/blog/2011/06/designing_ideas_of_inheritance_mechanism_in_javascript.html](http://www.ruanyifeng.com/blog/2011/06/designing_ideas_of_inheritance_mechanism_in_javascript.html)
