# javascript 和 CoffeeScript 里的类 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月10日 00:21:17[左直拳](https://me.csdn.net/leftfist)阅读数：1688
javascript不是面向对象的语言，它用函数来模拟类和继承。
javascript里，提供一个类并不难：
```java
var Person，l4, z3;
Person = function(name) {
  return this.name = name;
};
z3 = new Person("z3");
l4 = new Person("l4");
z3.sayHi = function(hi) {
  return alert(hi);
};
z3.sayHi("hehehe");//可以
l4.sayHi("hahaha");//报错
```
在这里，Person是一个类（其实就是一个函数）；z3、l4分别new了一个Person。new，就是构造实例。为什么要构造，而不是将Person直接赋给它们？是因为构造实例的话，各个实例之间不会互相影响。在上面的例子中，z3定义了一个函数sayHi()，但l4并没有这个方法，并不受z3的影响。
然后加上如下语句，则l4也有sayHi()方法矣。不过，z3使用的，仍然是自己定义的那个sayHi()方法。这算是override吗？
```java
Person.prototype.sayHi = function(hi) {
  return alert("OK!" + hi);
};
z3.sayHi("hehehe");
l4.sayHi("hahaha");
```
在上面的例子中，Person只是一个简单的函数，虽然它同样可以供其他变量new。通常一个javascript类应该是这样子的：
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41831875#)[copy](http://blog.csdn.net/leftfist/article/details/41831875#)
- var Father, Son, father, son;  
- 
- Father = (function() {  
- function Father(name) {//相当于构造函数了。这个构造函数很重要，下面的添加成员函数，属性，都靠它。
- this.name = name;  
-   }  
- 
-   Father.prototype.sayHi = function(hi) {//这个Father,其实是指上面这个“构造函数”。prototype，是javascript函数的固有属性。
- return alert("" + this.name + " : " + hi);  
-   };  
- 
- return Father;//同样，这个Father，<span style="font-family: Arial, Helvetica, sans-serif;">是指上面的“构造函数”</span>
- 
- })();  
- 
- Son = (function() {  
- function Son(name) {  
- this.name = name;  
-   }  
- 
-   Son.prototype = new Father();//继承的诀窍在于此
-   Son.prototype.cry = function(){ alert("噢噢噢");}//子类扩展
- return Son;  
- 
- })();  
- 
- son = new Son("leftfist");  
- 
- son.sayHi("Hello World!");  
CoffeeScript主要是为了生产javascript，规避javascript的一些弊端，同时也是为了提高可读性。因此在CoffeeScript里，写类的代码类似：
**[ruby]**[view plain](http://blog.csdn.net/leftfist/article/details/41831875#)[copy](http://blog.csdn.net/leftfist/article/details/41831875#)
- class Father  
- #构造函数
-   constructor: (name) ->   
- @name = name  
- 
-   sayHi:(hi) ->  
-     alert("#{@name} : #{hi}")  
- 
- class Son  
-   constructor: (name) ->  
- @name = name  
- 
-   @.prototype = new Father()  
- 
- son = new Son("leftfist")  
- son.sayHi("Hello Human!")  
