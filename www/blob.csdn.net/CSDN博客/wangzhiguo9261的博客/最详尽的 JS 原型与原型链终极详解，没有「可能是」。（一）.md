
# 最详尽的 JS 原型与原型链终极详解，没有「可能是」。（一） - wangzhiguo9261的博客 - CSDN博客


2018年04月12日 15:26:31[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：106个人分类：[js](https://blog.csdn.net/wangzhiguo9261/article/category/7508784)


[第二篇已更新，点击进入](https://www.jianshu.com/p/652991a67186)
[第三篇已更新，点击进入](https://www.jianshu.com/p/a4e1e7b6f4f8)
三篇文章都更新完毕，完整的剖析了 JS 原型与原型链，希望通过这些教程能让你对 Javascript 这门语言理解的更透彻！
---

### 一. 普通对象与函数对象
JavaScript 中，万物皆对象！但对象也是有区别的。分为**普通对象和函数对象**，Object 、Function 是 JS 自带的函数对象。下面举例说明
```python
var
```
```python
o1 = {};
```
```python
var
```
```python
o2 =
```
```python
new
```
```python
Object
```
```python
();
```
```python
var
```
```python
o3 =
```
```python
new
```
```python
f1();
```
```python
function
```
```python
f1
```
```python
(
```
```python
)
```
```python
{};
```
```python
var
```
```python
f2 =
```
```python
function
```
```python
(
```
```python
)
```
```python
{};
```
```python
var
```
```python
f3 =
```
```python
new
```
```python
Function
```
```python
(
```
```python
'str'
```
```python
,
```
```python
'console.log(str)'
```
```python
);
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
Object
```
```python
);
```
```python
//function
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
Function
```
```python
);
```
```python
//function
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
f1);
```
```python
//function
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
f2);
```
```python
//function
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
f3);
```
```python
//function
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
o1);
```
```python
//object
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
o2);
```
```python
//object
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
o3);
```
```python
//object
```
在上面的例子中 o1 o2 o3 为普通对象，f1 f2 f3 为函数对象。怎么区分，其实很简单，**凡是通过 new Function() 创建的对象都是函数对象，其他的都是普通对象。f1,f2,归根结底都是通过 new Function()的方式进行创建的。Function Object 也都是通过 New Function()创建的**。
一定要分清楚普通对象和函数对象，下面我们会常常用到它。
### 二. 构造函数
我们先复习一下构造函数的知识：
```python
function
```
```python
Person
```
```python
(
```
```python
name, age, job
```
```python
)
```
```python
{
```
```python
this
```
```python
.name = name;
```
```python
this
```
```python
.age = age;
```
```python
this
```
```python
.job = job;
```
```python
this
```
```python
.sayName =
```
```python
function
```
```python
(
```
```python
)
```
```python
{ alert(
```
```python
this
```
```python
.name) } 
}
```
```python
var
```
```python
person1 =
```
```python
new
```
```python
Person(
```
```python
'Zaxlct'
```
```python
,
```
```python
28
```
```python
,
```
```python
'Software Engineer'
```
```python
);
```
```python
var
```
```python
person2 =
```
```python
new
```
```python
Person(
```
```python
'Mick'
```
```python
,
```
```python
23
```
```python
,
```
```python
'Doctor'
```
```python
);
```
上面的例子中 person1 和 person2 都是 Person 的**实例**。这两个**实例**都有一个`constructor`（构造函数）属性，该属性（是一个指针）指向 Person。 即：
```python
console
```
```python
.log(person1.constructor == Person);
```
```python
//true
```
```python
console
```
```python
.log(person2.constructor == Person);
```
```python
//true
```
我们要记住两个概念（构造函数，实例）：
**person1 和 person2 都是 构造函数 Person 的实例**
一个公式：
**实例的构造函数属性（constructor）指向构造函数。**
### 三. 原型对象
在 JavaScript 中，每当定义一个对象（函数也是对象）时候，对象中都会包含一些预定义的属性。其中每个**函数对象**都有一个`prototype`属性，这个属性指向函数的**原型对象**。（先用不管什么是`__proto__`第二节的课程会详细的剖析）
```python
function
```
```python
Person
```
```python
(
```
```python
)
```
```python
{}
Person.prototype.name =
```
```python
'Zaxlct'
```
```python
;
Person.prototype.age  =
```
```python
28
```
```python
;
Person.prototype.job  =
```
```python
'Software Engineer'
```
```python
;
Person.prototype.sayName =
```
```python
function
```
```python
(
```
```python
)
```
```python
{
  alert(
```
```python
this
```
```python
.name);
}
```
```python
var
```
```python
person1 =
```
```python
new
```
```python
Person();
person1.sayName();
```
```python
// 'Zaxlct'
```
```python
var
```
```python
person2 =
```
```python
new
```
```python
Person();
person2.sayName();
```
```python
// 'Zaxlct'
```
```python
console
```
```python
.log(person1.sayName == person2.sayName);
```
```python
//true
```
我们得到了本文第一个「**定律**」：
`每个对象都有 __proto__ 属性，但只有函数对象才有 prototype 属性`
---
那什么是**原型对象**呢？
我们把上面的例子改一改你就会明白了：
```python
Person.prototype = {
```
```python
name
```
```python
:
```
```python
'Zaxlct'
```
```python
,
```
```python
age
```
```python
:
```
```python
28
```
```python
,
```
```python
job
```
```python
:
```
```python
'Software Engineer'
```
```python
,
```
```python
sayName
```
```python
:
```
```python
function
```
```python
(
```
```python
)
```
```python
{
     alert(
```
```python
this
```
```python
.name);
   }
}
```
原型对象，顾名思义，它就是一个普通对象（废话 = =!）。从现在开始你要牢牢记住原型对象就是 Person.prototype ，如果你还是害怕它，那就把它想想成一个字母 A：`var A = Person.prototype`
---
在上面我们给 A 添加了 四个属性：name、age、job、sayName。其实它还有一个默认的属性：`constructor`
> 在默认情况下，所有的
> 原型对象
> 都会
> 自动获得
> 一个
> constructor
> （构造函数）属性，这个属性（是一个指针）指向
> prototype
> 属性所在的函数（Person）
上面这句话有点拗口，我们「翻译」一下：A 有一个默认的`constructor`属性，这个属性是一个指针，指向 Person。即：
`Person.prototype.constructor == Person`
---
在上面第二小节《构造函数》里，我们知道**实例的构造函数属性（constructor）指向构造函数**：`person1.constructor == Person`
这两个「公式」好像有点联系：
```python
person1.constructor == Person
Person.prototype.constructor == Person
```
person1 为什么有 constructor 属性？那是因为 person1 是 Person 的实例。
那 Person.prototype 为什么有 constructor 属性？？同理， Person.prototype （你把它想象成 A） 也是Person 的实例。
也就是在 Person 创建的时候，创建了一个它的实例对象并赋值给它的 prototype，基本过程如下：
```python
var
```
```python
A =
```
```python
new
```
```python
Person();
 Person.prototype = A;
```
**结论：原型对象（Person.prototype）是 构造函数（Person）的一个实例。**
---
原型对象其实就是普通对象（但 Function.prototype 除外，它是函数对象，但它很特殊，他没有prototype属性（前面说道函数对象都有prototype属性））。看下面的例子：
```python
function
```
```python
Person
```
```python
(
```
```python
)
```
```python
{};
```
```python
console
```
```python
.log(Person.prototype)
```
```python
//Person{}
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
Person.prototype)
```
```python
//Object
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
Function
```
```python
.prototype)
```
```python
// Function，这个特殊
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
Object
```
```python
.prototype)
```
```python
// Object
```
```python
console
```
```python
.log(
```
```python
typeof
```
```python
Function
```
```python
.prototype.prototype)
```
```python
//undefined
```
`Function.prototype`为什么是函数对象呢？
```python
var
```
```python
A =
```
```python
new
```
```python
Function
```
```python
();
```
```python
Function
```
```python
.prototype = A;
```
## 上文提到
## 凡是通过 new Function( ) 产生的对象都是函数对象
## 。因为 A 是函数对象，所以
## Function.prototype
## 是函数对象。
那原型对象是用来做什么的呢？主要作用是用于继承。举个例子：
```python
var
```
```python
Person =
```
```python
function
```
```python
(
```
```python
name
```
```python
)
```
```python
{
```
```python
this
```
```python
.name = name;
```
```python
// tip: 当函数执行时这个 this 指的是谁？
```
```python
};
  Person.prototype.getName =
```
```python
function
```
```python
(
```
```python
)
```
```python
{
```
```python
return
```
```python
this
```
```python
.name;
```
```python
// tip: 当函数执行时这个 this 指的是谁？
```
```python
}
```
```python
var
```
```python
person1 =
```
```python
new
```
```python
person(
```
```python
'Mick'
```
```python
);
  person1.getName();
```
```python
//Mick
```
从这个例子可以看出，通过给`Person.prototype`设置了一个函数对象的属性，那有 Person 的实例（person1）出来的普通对象就继承了这个属性。具体是怎么实现的继承，就要讲到下面的原型链了。
小问题，上面两个 this 都指向谁？
```python
var
```
```python
person1 =
```
```python
new
```
```python
person(
```
```python
'Mick'
```
```python
);
  person1.name =
```
```python
'Mick'
```
```python
;
```
```python
// 此时 person1 已经有 name 这个属性了
```
```python
person1.getName();
```
```python
//Mick
```
故两次 this  在函数执行时都指向 person1。
---
2017-10-27 更新：
下面的评论有喷子喷我：「null 没有 _*proto*_」，下面解释一下：
null 是一个独立的数据类型，为什么typeof(null)的值是"object"？
null不是一个空引用, 而是一个原始值, 参考[ECMAScript5.1中文版**](https://link.jianshu.com?t=https%3A%2F%2Flink.zhihu.com%2F%3Ftarget%3Dhttp%253A%2F%2Flzw.me%2Fpages%2Fecmascript%2F%252320)4.3.11节; 它只是期望此处将引用一个对象, 注意是"期望", 参考[null - JavaScript**](https://link.jianshu.com?t=https%3A%2F%2Flink.zhihu.com%2F%3Ftarget%3Dhttps%253A%2F%2Fdeveloper.mozilla.org%2Fzh-CN%2Fdocs%2FWeb%2FJavaScript%2FReference%2FGlobal_Objects%2Fnull).
typeof null结果是object, 这是个历史遗留bug, 参考[typeof - JavaScript**](https://link.jianshu.com?t=https%3A%2F%2Flink.zhihu.com%2F%3Ftarget%3Dhttps%253A%2F%2Fdeveloper.mozilla.org%2Fzh-CN%2Fdocs%2FWeb%2FJavaScript%2FReference%2FOperators%2Ftypeof)
在ECMA6中, 曾经有提案为历史平凡, 将type null的值纠正为null, 但最后提案被拒了. 理由是历史遗留代码太多, 不想得罪人, 不如继续将错就错当和事老, 参考[harmony:typeof_null [ES Wiki]](https://link.jianshu.com?t=https%3A%2F%2Flink.zhihu.com%2F%3Ftarget%3Dhttp%253A%2F%2Fwiki.ecmascript.org%2Fdoku.php%253Fid%253Dharmony%25253atypeof_null)
作者：克荷林链接：[https://www.zhihu.com/question/21691758/answer/987822](https://link.jianshu.com?t=https%3A%2F%2Fwww.zhihu.com%2Fquestion%2F21691758%2Fanswer%2F987822)
写个教程不容易，遇到喷子真心不爽。喷子不管那么多，哪怕文章是免费看的。他们只要抓住你一点点的小失误，就兴奋的噼里啪啦一顿喷。
---
别着急走，关于原型与原型链还有第二篇第三篇呢~
[第二篇已更新，点击进入](https://www.jianshu.com/p/652991a67186)
[第三篇已更新，点击进入](https://www.jianshu.com/p/a4e1e7b6f4f8)
---
本文部分参照了[JS原型与原型链终极详解](https://link.jianshu.com?t=http%3A%2F%2Fwww.108js.com%2Farticle%2Farticle1%2F10201.html%3Fid%3D1092)（感谢原作者）（侵立删）
和《JavaScript 高级程序设计》第三版
希望通过这一系列课程，能帮助你彻底搞明白 JavaScript 的原型与原型链 ：）

作者：Yi罐可乐
链接：https://www.jianshu.com/p/dee9f8b14771
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

