# 自己对JS中constructor的误解 - weixin_33985507的博客 - CSDN博客
2017年07月27日 20:29:44[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
在JS中，构造器(constructor)是用来用new关键词来创建对象的一个普普通通的函数，为了在形式上区别一般的函数，第一个字母通常大写。
```
function Person(){};  // constructor
var wenzhe = new Person();
```
这就是基本的格式，我们能通过wenzhe.constructor或者Person.prototype.constructor来查看我们的构造函数，但是查看的这个构造函数只是表象。它不参与对象的构造，也就是说这个属性只是存储了构造函数的映像而已，没啥用。真正构造对象的那个构造函数永远是new后面那个。即它只是为了方便人查看构造函数而存在的
```
function Number() {
    this.name = 'Number';
 }
 function Two() {}
 function Three() {
    this.name = 'Three'
 }
Two.prototype = new Number(); 
Two.prototype.constructor === Number.prototype.constructor; // true
Two.prototype.constructor = Three;
var a = new Two();
a.name // shape;
a.constructor // ..three...
```
所以说，当我们 Two.prototype = new Number()之后，最好再添加 Two.prototype.constructor = Two,这样避免混淆。
我们可以用 console.log(wenzhe instanceof Person) 判断对象和构造函数是否对应
,虽然上面那个a.constructor是指向Three,但是console.log(a instanceof Three) 依然为false。
还有一点就是 object literal 或者 Object constructor创建的对象的constructor指向Object
