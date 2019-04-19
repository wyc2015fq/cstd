# JS中引用类型 - LC900730的博客 - CSDN博客
2017年08月28日 21:32:11[lc900730](https://me.csdn.net/LC900730)阅读数：152标签：[js](https://so.csdn.net/so/search/s.do?q=js&t=blog)
个人分类：[js](https://blog.csdn.net/LC900730/article/category/7133535)
## 引用类型
引用类型的值是引用类型的一个实例。在ECMAScript中，引用类型是一种数据结构，用于将数据和功能组织在一起，它也被称为类。 
对象是某个引用类型的实例。 新对象是使用new操作符跟一个构造函数来创建的。构造函数本身就是一个函数，只不过该函数是出于创建新对象的目的而定义的。
### Object类型
var perosn=new Object(); 
这行代码创建了一个Object引用类型的一个新实例，然后把该实例保存到变量person中。使用的构造函数是Object。ECMAScript提供了很多原生引用类型，如Object。
### Array类型
ECMAScript数组的每一项都可以保存任何类型的数据。 
创建方式：
```
1.var name=new Array();
    var colors=Array();
    2.var color=['red','blue','green'];
```
数组的length属性不是只读的。
```
var colors=['red','blue','green'];
    colors.length=2;
    alert(colors[2]);     //undefined
```
### 原型对象
创建新函数，就会根据一组特定规则为该函数创建一个prototype属性。所有原型对象会自动获得一个constructor(构造函数)属性，这个属性包含一个指向prototype属性所在函数的执行。
```
function Person{}
Person.prototype.name='Nicholas';
Person.prototype.age=29;
console.log(Person.prototype); //Person{name:'Nicholas',age:29}
console.log(Person.prototype.constructor);   //[Function:Person]
```
![这里写图片描述](https://img-blog.csdn.net/20170828165420562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
虽然在所有实现中都无法访问到[[Prototype]],但是可以通过isPrototypeOf()方法来确定对象之间是否存在这种关系。
```
Person.prototype.isPrototypeOf(person1);  //true
Person.prototype.isPrototypeOf(person2);  //true
因为它们内部都有一个指向Person.prototype的指针。
```
还可以通过Object.getPrototypeOf().返回的对象实际上就是这个对象的原型。 
如alert(Object.getPrototypeOf(person1)==Person.prototype) ;   //true
### 原型的动态性
```java
var friend=new Person();
Person.prototype.sayHi()=function(){
    console.log('say hi');
}
friend.sayHi();
//即使person实例friend是在添加新方法之前创建的，但它仍然可以访问这个新方法。原因是实例与原型之间的松散连接关系。
当我们调用person.sayHi()时，首先会在实例中搜索名为sayHi的属性，没找到的情况下，会继续搜索原型。因为实例与原型的连接只不过是一个指针，而不是副本，因此可以在原型中找到新的sayHi属性并返回保存在那里的函数。
```
### 重写原型对象的情况
```java
function Person(){
    }
    var friend=new Person();
```
