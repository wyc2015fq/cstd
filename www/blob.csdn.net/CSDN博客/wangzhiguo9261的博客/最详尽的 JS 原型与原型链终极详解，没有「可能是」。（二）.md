
# 最详尽的 JS 原型与原型链终极详解，没有「可能是」。（二） - wangzhiguo9261的博客 - CSDN博客


2018年04月12日 15:27:43[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：228


[第三篇已更新，点击进入](https://www.jianshu.com/p/a4e1e7b6f4f8)
### 四. __proto__
JS 在创建对象（不论是普通对象还是函数对象）的时候，都有一个叫做`__proto__`的内置属性，用于指向创建它的构造函数的原型对象。
对象 person1 有一个`__proto__`属性，创建它的构造函数是 Person，构造函数的原型对象是 Person.prototype ，所以：
`person1.__proto__ == Person.prototype`
请看下图：
![](//upload-images.jianshu.io/upload_images/1430985-b650bc438f236877.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
《JavaScript 高级程序设计》的图 6-1
根据上面这个连接图，我们能得到：
```python
Person.prototype.constructor == Person;
person1.__proto__ == Person.prototype;
person1.constructor == Person;
```
**不过，要明确的真正重要的一点就是，这个连接存在于实例（****person1****）与构造函数（****Person****）的原型对象（****Person.prototype****）之间，而不是存在于实例（****person1****）与构造函数（****Person****）之间。**
注意：因为绝大部分浏览器都支持__proto__属性，所以它才被加入了 ES6 里（ES5 部分浏览器也支持，但还不是标准）。
### 五. 构造器
熟悉 Javascript 的童鞋都知道，我们可以这样创建一个对象：
`var obj = {}`
它等同于下面这样：
`var obj = new Object()`
obj 是构造函数（Object）的一个实例。所以：
`obj.constructor === Object`
`obj.__proto__ === Object.prototype`
新对象 obj 是使用 new 操作符后跟一个**构造函数**来创建的。构造函数（Object）本身就是一个函数（就是上面说的函数对象），它和上面的构造函数 Person 差不多。只不过该函数是出于创建新对象的目的而定义的。所以不要被 Object 吓倒。
---
同理，可以创建对象的构造器不仅仅有 Object，也可以是 Array，Date，Function等。
所以我们也可以构造函数来创建 Array、 Date、Function
```python
var
```
```python
b =
```
```python
new
```
```python
Array
```
```python
();
b.constructor ===
```
```python
Array
```
```python
;
b.__proto__ ===
```
```python
Array
```
```python
.prototype;
```
```python
var
```
```python
c =
```
```python
new
```
```python
Date
```
```python
(); 
c.constructor ===
```
```python
Date
```
```python
;
c.__proto__ ===
```
```python
Date
```
```python
.prototype;
```
```python
var
```
```python
d =
```
```python
new
```
```python
Function
```
```python
();
d.constructor ===
```
```python
Function
```
```python
;
d.__proto__ ===
```
```python
Function
```
```python
.prototype;
```
这些构造器都是函数对象：
![](//upload-images.jianshu.io/upload_images/1430985-b8373019f5f3bab3.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
函数对象

### 六.  原型链
小测试来检验一下你理解的怎么样：
person1.__proto__是什么？
Person.__proto__是什么？
Person.prototype.__proto__是什么？
Object.__proto__是什么？
Object.prototype__proto__是什么？
答案：
第一题：
因为`person1.__proto__ === person1 的构造函数.prototype`
因为`person1的构造函数 === Person`
所以`person1.__proto__ === Person.prototype`
第二题：
因为`Person.__proto__ === Person的构造函数.prototype`
因为`Person的构造函数 === Function`
所以`Person.__proto__ === Function.prototype`
第三题：
`Person.prototype`是一个普通对象，我们无需关注它有哪些属性，只要记住它是一个普通对象。
因为一个普通对象的构造函数 === Object
所以`Person.prototype.__proto__ === Object.prototype`
第四题，参照第二题，因为 Person 和 Object 一样都是构造函数
第五题：
`Object.prototype`对象也有**proto**属性，但它比较特殊，为 null 。因为 null 处于原型链的顶端，这个只能记住。
`Object.prototype.__proto__ === null`
好了，如果以上你都能明白，那就可以继续深入学习第三篇教程了：
[第三篇，点击进入](https://www.jianshu.com/p/a4e1e7b6f4f8)

作者：Yi罐可乐
链接：https://www.jianshu.com/p/652991a67186
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

