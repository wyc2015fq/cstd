# JS中对象 - LC900730的博客 - CSDN博客
2017年08月28日 14:18:11[lc900730](https://me.csdn.net/LC900730)阅读数：113
个人分类：[nodejs](https://blog.csdn.net/LC900730/article/category/7127126)
## ECMAScript中对象
ECMAScript中没有类的概念。ECMA-262将对象定义为：无序属性的 集合，其属性可以包含基本值、对象或者函数。严格来讲对象就是一组没有特定顺序的值。对象的每个属性或方法都有一个名字，而每个名字都映射到一个值。我们可以将ECMAScript想象成一个散列表：无非就是一组名值对，其中值可以是数据或者函数。
### 理解对象
创建自定义对象最简单方式就是创建一个Object实例，然后为其添加属性和方法。
```java
var person=new Object();
person.name="Nicholas";
person.age=29;
person.sayName=function(){
    alert(this.name);
}
```
对象字面量语法可以如下：
```java
var person={
        name:'Nicholas',
        age:29,
        job:'Software Engine',
        sayName:function(){
            alert(this.name);
        }
    }
```
### 工厂模式
虽然Object构造函数或者对象字面量都可以用来创建单个对象，但是这个方式缺点：使用同一个接口创建很多对象，会产生大量重复代码。
```java
function createPerson(name,age,job){
    var o=new Object();
    o.name=name;
    o.age=age;
    o.sayName=function(){
        alert(this.name);
    }
    return o;
}
var person1=createPerson('Nial',29,'Software Engineer');
var person1=createPerson('Greg',27,'Doctor');
```
工厂模式虽然解决了创建多个相似对象的问题，但是没有解决对象识别的问题(即怎样知道一个对象的类型)。
### 构造函数模式
```java
function Person(name,age,job){
        this.name=name;
        this.age=age;
        this.job=job;
        this.sayName=function(){
            alert(this.name);
        }
    }
var person1=new Person('Nicl',29,'Software Engine');
var person2=new Person('Greg',27,'Doctor');
person1.constructor==Person ;//true
person2.constructor==Person ;//true
```
对象的constructor属性最初是用来标识对象类型的，但是提到检测对象类型，还是instanceof操作符更要可靠一些。
### 原型模式
我们创建的每一个函数都有prototype(原型)属性。这个属性是一个指针，指向一个对象，而这个对象的 用途是包含可以由特定类型的所有实例共享的属性和方法。那么prototype就是通过调用构造函数而创建的那个对象实例的原型对象。使用原型对象的好处是可以让所有对象实例共享它所含有的属性和方法。
//将属性和方法直接添加到了Person的prototype属性中，构造函数变成了空函数。
```java
function Person(){
    }
    Person.prototype.name="Nicholas";
    Person.prototype.age=29;
    Person.prototype.job="Software Engineer";
    Person.prototype.sayName=function(){
        alert(this.name);
    }
    var person1=new Person();
    person1.sayName();             //"Nicholas"
    var person2=new Person();
    person2.sayName();            //"Nicholas"
```
### 理解原型对象
无论什么时候，只要创建一个新函数，就会根据一组特定规则为该函数创建一个prototype属性，这个属性指向函数的原型对象。在默认情况下，所有原型对象都会自动获得一个constructor(构造函数)属性，这个属性包含一个指向prototype属性所在的函数指针。 
Person.prototype.constructor指向Person。而通过这个构造函数还可以继续为原型对象添加其他属性和方法。
创建了自定义构造函数，其原型对象默认只会取得constructor属性，至于其他方法，则都是从Object继承而来的。当调用构造函数创建一个新实例后，该实例内部包含一指针(内部属性),指向构造函数的原型对象。
### 原型与in操作符
单独使用：in操作符会在通过对象能够访问给定属性时返回true，无论该属性存在实例中还是原型中。
```java
function PErson(){
    }
    Person.prototype.name="Nicholas";
    Person.prototype.age=29;
    Person.prototype.job="Software Engineer";
    Person.prototype.sayName=function(){
        alert(this.name);
    };
var person1=new Person();
var person2=new Person();
alert(person1.hasOwnProperty('name'));   //false
alert("name" in person1);   //true
person1.name="Greg";
alert(person1.name);
alert(person1.hasOwnProperty('name'));   //   自己实例 true
alert("name" in person1);   //true
alert(person2.hasOwnProperty('name'));   //false
person2.name="Greg2";
alert(person2.hasOwnProperty('name'));   //true，来自实例
```
由于in操作符只要通过对象能够访问到属性就返回true，hasOwnProperty()只在属性存在于实例中才返回true。因此只要in操作符返回true而hasOwnProperty()返回false，就可以确定属性是原型中的属性。
hasPrototypeProperty();
