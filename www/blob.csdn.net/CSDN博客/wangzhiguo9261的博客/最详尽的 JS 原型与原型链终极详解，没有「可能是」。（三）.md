
# 最详尽的 JS 原型与原型链终极详解，没有「可能是」。（三） - wangzhiguo9261的博客 - CSDN博客


2018年04月12日 15:29:06[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：42


### 七. 函数对象 （复习一下前面的知识点）
##### 所有
##### 函数对象
##### 的
##### proto
##### 都指向Function.prototype，它是一个空函数（Empty function）
```python
Number
```
```python
.__proto__ ===
```
```python
Function
```
```python
.prototype
```
```python
// true
```
```python
Number
```
```python
.constructor ==
```
```python
Function
```
```python
//true
```
```python
Boolean
```
```python
.__proto__ ===
```
```python
Function
```
```python
.prototype
```
```python
// true
```
```python
Boolean
```
```python
.constructor ==
```
```python
Function
```
```python
//true
```
```python
String
```
```python
.__proto__ ===
```
```python
Function
```
```python
.prototype
```
```python
// true
```
```python
String
```
```python
.constructor ==
```
```python
Function
```
```python
//true
```
```python
// 所有的构造器都来自于Function.prototype，甚至包括根构造器Object及Function自身
```
```python
Object
```
```python
.__proto__ ===
```
```python
Function
```
```python
.prototype
```
```python
// true
```
```python
Object
```
```python
.constructor ==
```
```python
Function
```
```python
// true
```
```python
// 所有的构造器都来自于Function.prototype，甚至包括根构造器Object及Function自身
```
```python
Function
```
```python
.__proto__ ===
```
```python
Function
```
```python
.prototype
```
```python
// true
```
```python
Function
```
```python
.constructor ==
```
```python
Function
```
```python
//true
```
```python
Array
```
```python
.__proto__ ===
```
```python
Function
```
```python
.prototype
```
```python
// true
```
```python
Array
```
```python
.constructor ==
```
```python
Function
```
```python
//true
```
```python
RegExp
```
```python
.__proto__ ===
```
```python
Function
```
```python
.prototype
```
```python
// true
```
```python
RegExp
```
```python
.constructor ==
```
```python
Function
```
```python
//true
```
```python
Error
```
```python
.__proto__ ===
```
```python
Function
```
```python
.prototype
```
```python
// true
```
```python
Error
```
```python
.constructor ==
```
```python
Function
```
```python
//true
```
```python
Date
```
```python
.__proto__ ===
```
```python
Function
```
```python
.prototype
```
```python
// true
```
```python
Date
```
```python
.constructor ==
```
```python
Function
```
```python
//true
```
JavaScript中有内置(build-in)构造器/对象共计12个（ES5中新加了JSON），这里列举了可访问的8个构造器。剩下如Global不能直接访问，Arguments仅在函数调用时由JS引擎创建，Math，JSON是以对象形式存在的，无需new。它们的**proto**是Object.prototype。如下
```python
Math
```
```python
.__proto__ ===
```
```python
Object
```
```python
.prototype
```
```python
// true
```
```python
Math
```
```python
.construrctor ==
```
```python
Object
```
```python
// true
```
```python
JSON
```
```python
.__proto__ ===
```
```python
Object
```
```python
.prototype
```
```python
// true
```
```python
JSON
```
```python
.construrctor ==
```
```python
Object
```
```python
//true
```
上面说的**函数对象**当然包括自定义的。如下
```python
// 函数声明
```
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
```
```python
// 函数表达式
```
```python
var
```
```python
Perosn =
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
{}
```
```python
console
```
```python
.log(Person.__proto__ ===
```
```python
Function
```
```python
.prototype)
```
```python
// true
```
```python
console
```
```python
.log(Man.__proto__ ===
```
```python
Function
```
```python
.prototype)
```
```python
// true
```
这说明什么呢？
** 所有的构造器都来自于`Function.prototype`，甚至包括根构造器`Object`及`Function`自身。所有构造器都继承了·Function.prototype·的属性及方法。如length、call、apply、bind**
（你应该明白第一句话，第二句话我们下一节继续说，先挖个坑：））
`Function.prototype`也是唯一一个`typeof XXX.prototype`为`function`的`prototype`。其它的构造器的`prototype`都是一个对象（原因第三节里已经解释过了）。如下（又复习了一遍）：
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
// function
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
// object
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
Number
```
```python
.prototype)
```
```python
// object
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
Boolean
```
```python
.prototype)
```
```python
// object
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
String
```
```python
.prototype)
```
```python
// object
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
Array
```
```python
.prototype)
```
```python
// object
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
RegExp
```
```python
.prototype)
```
```python
// object
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
Error
```
```python
.prototype)
```
```python
// object
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
Date
```
```python
.prototype)
```
```python
// object
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
// object
```
噢，上面还提到它是一个空的函数，`console.log(Function.prototype)`下看看（留意，下一节会再说一下这个）
知道了所有构造器（含内置及自定义）的`__proto__`都是`Function.prototype`，那`Function.prototype`的`__proto__`是谁呢？
相信都听说过JavaScript中函数也是一等公民，那从哪能体现呢？如下
`console.log(Function.prototype.__proto__ === Object.prototype) // true`
这说明所有的构造器也都是一个普通 JS 对象，可以给构造器添加/删除属性等。同时它也继承了Object.prototype上的所有方法：toString、valueOf、hasOwnProperty等。（你也应该明白第一句话，第二句话我们下一节继续说，不用挖坑了，还是刚才那个坑；））
最后Object.prototype的**proto**是谁？
`Object.prototype.__proto__ === null // true`
已经到顶了，为null。(读到现在，再回过头看第五章，能明白吗？)
### 八. Prototype
> 在 ECMAScript 核心所定义的全部属性中，最耐人寻味的就要数
> prototype
> 属性了。对于 ECMAScript 中的引用类型而言，
> prototype
> 是保存着它们所有实例方法的真正所在。换句话所说，诸如
> toString()
> 和
> valuseOf()
> 等方法实际上都保存在
> prototype
> 名下，只不过是通过各自对象的实例访问罢了。
——《JavaScript 高级程序设计》第三版 P116
我们知道 JS 内置了一些方法供我们使用，比如：
对象可以用`constructor/toString()/valueOf()`等方法;
数组可以用`map()/filter()/reducer()`等方法；
数字可用用`parseInt()/parseFloat()`等方法；
Why ？？？
![](//upload-images.jianshu.io/upload_images/1430985-3fd23e271cb70ac5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/180)
why??

#### 当我们创建一个函数时：
`var Person = new Object()`
`Person`是`Object`的实例，所以`Person`**继承**了`Object`的原型对象`Object.prototype`上所有的方法：
![](//upload-images.jianshu.io/upload_images/1430985-99ba7f98c0bd06cd.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
Object.prototype

**Object 的每个实例都具有以上的属性和方法。**
所以我可以用`Person.constructor`也可以用`Person.hasOwnProperty`。
#### 当我们创建一个数组时：
`var num = new Array()`
`num`是`Array`的实例，所以`num`**继承**了`Array`的原型对象`Array.prototype`上所有的方法：
![](//upload-images.jianshu.io/upload_images/1430985-764149fe4df24bd6.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
Array.prototype
Are you f***ing kidding me? 这尼玛怎么是一个空数组？？？
![](//upload-images.jianshu.io/upload_images/1430985-457d39b71e9c00c4.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/268)
doge

我们可以用一个 ES5 提供的新方法：`Object.getOwnPropertyNames`
获取所有（**包括不可枚举的属性**）的属性名**不包括****prototy****中的属性**，返回一个数组：
```python
var
```
```python
arrayAllKeys =
```
```python
Array
```
```python
.prototype;
```
```python
// [] 空数组
```
```python
// 只得到 arrayAllKeys 这个对象里所有的属性名(不会去找 arrayAllKeys.prototype 中的属性)
```
```python
console
```
```python
.log(
```
```python
Object
```
```python
.getOwnPropertyNames(arrayAllKeys));
```
```python
/* 输出：
["length", "constructor", "toString", "toLocaleString", "join", "pop", "push", 
"concat", "reverse", "shift", "unshift", "slice", "splice", "sort", "filter", "forEach", 
"some", "every", "map", "indexOf", "lastIndexOf", "reduce", "reduceRight", 
"entries", "keys", "copyWithin", "find", "findIndex", "fill"]
*/
```
这样你就明白了随便声明一个数组，它为啥能用那么多方法了。
细心的你肯定发现了`Object.getOwnPropertyNames(arrayAllKeys)`输出的数组里并没有`constructor/hasOwnPrototype`等**对象的**方法（你肯定没发现）。
但是随便定义的数组也能用这些方法
```python
var
```
```python
num = [
```
```python
1
```
```python
];
```
```python
console
```
```python
.log(num.hasOwnPrototype())
```
```python
// false (输出布尔值而不是报错)
```
Why ？？？
![](//upload-images.jianshu.io/upload_images/1430985-3fd23e271cb70ac5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/180)
why??
因为`Array.prototype`虽然没这些方法，但是它有原型对象（`__proto__`）：
```python
// 上面我们说了 Object.prototype 就是一个普通对象。
```
```python
Array
```
```python
.prototype.__proto__ ==
```
```python
Object
```
```python
.prototype
```
所以`Array.prototype`继承了对象的所有方法，当你用`num.hasOwnPrototype()`时，JS 会先查一下它的构造函数 （`Array`） 的原型对象`Array.prototype`有没有有`hasOwnPrototype()`方法，没查到的话继续查一下`Array.prototype`的原型对象`Array.prototype.__proto__`有没有这个方法。
#### 当我们创建一个函数时：
```python
var
```
```python
f =
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
"x"
```
```python
,
```
```python
"return x*x;"
```
```python
);
```
```python
//当然你也可以这么创建 f = function(x){ return x*x }
```
```python
console
```
```python
.log(f.arguments)
```
```python
// arguments 方法从哪里来的？
```
```python
console
```
```python
.log(f.call(
```
```python
window
```
```python
))
```
```python
// call 方法从哪里来的？
```
```python
console
```
```python
.log(
```
```python
Function
```
```python
.prototype)
```
```python
// function() {} （一个空的函数）
```
```python
console
```
```python
.log(
```
```python
Object
```
```python
.getOwnPropertyNames(
```
```python
Function
```
```python
.prototype));
```
```python
/* 输出
["length", "name", "arguments", "caller", "constructor", "bind", "toString", "call", "apply"]
*/
```
我们再复习第八小节这句话：
> 所有
> 函数对象proto
> 都指向
> Function.prototype
> ，它是一个空函数（Empty function）
嗯，我们验证了它就是空函数。不过不要忽略前半句。我们枚举出了它的所有的方法，所以所有的**函数对象**都能用，比如:
![](//upload-images.jianshu.io/upload_images/1430985-16bff45efb958d74.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
函数对象
如果你还没搞懂啥是函数对象？
![](//upload-images.jianshu.io/upload_images/1430985-9cc71526d64ea2b5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/183)
去屎 | center
还有，我建议你可以再复习下为什么：
> Function.prototype
> 是唯一一个typeof XXX.prototype为 “function”的prototype
我猜你肯定忘了。
### 九. 复习一下
第八小节我们总结了：
```python
所有函数对象的 __proto__ 都指向
```
```python
Function
```
```python
.prototype，它是一个空函数（Empty
```
```python
function
```
```python
）
```
但是你可别忘了在第三小节我们总结的：
`所有对象的 __proto__ 都指向其构造器的 prototype`咦，我找了半天怎么没找到这句话……
![](//upload-images.jianshu.io/upload_images/1430985-4d9532e2756471fd.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/440)
doge | center
我们下面再复习下这句话。
先看看 JS 内置构造器：
```python
var
```
```python
obj = {
```
```python
name
```
```python
:
```
```python
'jack'
```
```python
}
```
```python
var
```
```python
arr = [
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
]
```
```python
var
```
```python
reg =
```
```python
/hello/g
```
```python
var
```
```python
date =
```
```python
new
```
```python
Date
```
```python
var
```
```python
err =
```
```python
new
```
```python
Error
```
```python
(
```
```python
'exception'
```
```python
)
```
```python
console
```
```python
.log(obj.__proto__ ===
```
```python
Object
```
```python
.prototype)
```
```python
// true
```
```python
console
```
```python
.log(arr.__proto__ ===
```
```python
Array
```
```python
.prototype)
```
```python
// true
```
```python
console
```
```python
.log(reg.__proto__ ===
```
```python
RegExp
```
```python
.prototype)
```
```python
// true
```
```python
console
```
```python
.log(date.__proto__ ===
```
```python
Date
```
```python
.prototype)
```
```python
// true
```
```python
console
```
```python
.log(err.__proto__ ===
```
```python
Error
```
```python
.prototype)
```
```python
// true
```
再看看自定义的构造器，这里定义了一个`Person`：
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
}
```
```python
var
```
```python
p =
```
```python
new
```
```python
Person(
```
```python
'jack'
```
```python
)
```
```python
console
```
```python
.log(p.__proto__ === Person.prototype)
```
```python
// true
```
`p`是`Person`的实例对象，`p`的内部原型总是指向其构造器`Person`的原型对象`prototype`。
每个对象都有一个`constructor`属性，可以获取它的构造器，因此以下打印结果也是恒等的：
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
.name = name
}
```
```python
var
```
```python
p =
```
```python
new
```
```python
Person(
```
```python
'jack'
```
```python
)
```
```python
console
```
```python
.log(p.__proto__ === p.constructor.prototype)
```
```python
// true
```
上面的`Person`没有给其原型添加属性或方法，这里给其原型添加一个`getName`方法：
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
.name = name
}
```
```python
// 修改原型
```
```python
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
{}
```
```python
var
```
```python
p =
```
```python
new
```
```python
Person(
```
```python
'jack'
```
```python
)
```
```python
console
```
```python
.log(p.__proto__ === Person.prototype)
```
```python
// true
```
```python
console
```
```python
.log(p.__proto__ === p.constructor.prototype)
```
```python
// true
```
可以看到`p.__proto__`与`Person.prototype`，`p.constructor.prototype`都是恒等的，即都指向同一个对象。
如果换一种方式设置原型，结果就有些不同了：
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
.name = name
}
```
```python
// 重写原型
```
```python
Person.prototype = {
```
```python
getName
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
{}
}
```
```python
var
```
```python
p =
```
```python
new
```
```python
Person(
```
```python
'jack'
```
```python
)
```
```python
console
```
```python
.log(p.__proto__ === Person.prototype)
```
```python
// true
```
```python
console
```
```python
.log(p.__proto__ === p.constructor.prototype)
```
```python
// false
```
这里直接重写了`Person.prototype`（注意：上一个示例是修改原型）。输出结果可以看出`p.__proto__`仍然指向的是`Person.prototype`，而不是`p.constructor.prototype`。
这也很好理解，给`Person.prototype`赋值的是一个对象直接量`{getName: function(){}}`，使用对象直接量方式定义的对象其构造器（`constructor`）指向的是根构造器`Object`，`Object.prototype`是一个空对象`{}`，`{}`自然与`{getName: function(){}}`不等。如下：
```python
var
```
```python
p = {}
```
```python
console
```
```python
.log(
```
```python
Object
```
```python
.prototype)
```
```python
// 为一个空的对象{}
```
```python
console
```
```python
.log(p.constructor ===
```
```python
Object
```
```python
)
```
```python
// 对象直接量方式定义的对象其constructor为Object
```
```python
console
```
```python
.log(p.constructor.prototype ===
```
```python
Object
```
```python
.prototype)
```
```python
// 为true，不解释(๑ˇ3ˇ๑)
```
### 十. 原型链（再复习一下：）
下面这个例子你应该能明白了！
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
```
```python
console
```
```python
.log(person1.__proto__ === Person.prototype);
```
```python
// true
```
```python
console
```
```python
.log(Person.prototype.__proto__ ===
```
```python
Object
```
```python
.prototype)
```
```python
//true
```
```python
console
```
```python
.log(
```
```python
Object
```
```python
.prototype.__proto__)
```
```python
//null
```
```python
Person.__proto__ ==
```
```python
Function
```
```python
.prototype;
```
```python
//true
```
```python
console
```
```python
.log(
```
```python
Function
```
```python
.prototype)
```
```python
// function(){} (空函数)
```
```python
var
```
```python
num =
```
```python
new
```
```python
Array
```
```python
()
```
```python
console
```
```python
.log(num.__proto__ ==
```
```python
Array
```
```python
.prototype)
```
```python
// true
```
```python
console
```
```python
.log(
```
```python
Array
```
```python
.prototype.__proto__ ==
```
```python
Object
```
```python
.prototype)
```
```python
// true
```
```python
console
```
```python
.log(
```
```python
Array
```
```python
.prototype)
```
```python
// [] (空数组)
```
```python
console
```
```python
.log(
```
```python
Object
```
```python
.prototype.__proto__)
```
```python
//null
```
```python
console
```
```python
.log(
```
```python
Array
```
```python
.__proto__ ==
```
```python
Function
```
```python
.prototype)
```
```python
// true
```
疑点解惑：
Object.__proto__ === Function.prototype // true
Object是函数对象，是通过new Function()创建的，所以Object.__proto__指向Function.prototype。（参照第八小节：「所有函数对象的__proto__都指向Function.prototype」）
Function.__proto__ === Function.prototype // true
Function也是对象函数，也是通过new Function()创建，所以Function.__proto__指向Function.prototype。

> 自己是由自己创建的，好像不符合逻辑，但仔细想想，现实世界也有些类似，你是怎么来的，你妈生的，你妈怎么来的，你姥姥生的，……类人猿进化来的，那类人猿从哪来，一直追溯下去……，就是无，（NULL生万物）

> 正如《道德经》里所说“无，名天地之始”。
Function.prototype.__proto__ === Object.prototype //true
> 其实这一点我也有点困惑，不过也可以试着解释一下。

> Function.prototype
> 是个函数对象，理论上他的
> __proto__
> 应该指向
> Function.prototype
> ，就是他自己，自己指向自己，没有意义。

> JS一直强调万物皆对象，函数对象也是对象，给他认个祖宗，指向
> Object.prototype
> 。
> Object.prototype.__proto__ === null
> ，保证原型链能够正常结束。

### 十一 总结
原型和原型链是JS实现继承的一种模型。
原型链的形成是真正是靠__proto__而非prototype
要深入理解这句话，我们再举个例子，看看前面你真的理解了吗？
```python
var
```
```python
animal =
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
dog =
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
 animal.price =
```
```python
2000
```
```python
;
 dog.prototype = animal;
```
```python
var
```
```python
tidy =
```
```python
new
```
```python
dog();
```
```python
console
```
```python
.log(dog.price)
```
```python
//undefined
```
```python
console
```
```python
.log(tidy.price)
```
```python
// 2000
```
这里解释一下：
```python
var
```
```python
dog =
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
 dog.prototype.price =
```
```python
2000
```
```python
;
```
```python
var
```
```python
tidy =
```
```python
new
```
```python
dog();
```
```python
console
```
```python
.log(tidy.price);
```
```python
// 2000
```
```python
console
```
```python
.log(dog.price);
```
```python
//undefined
```
```python
var
```
```python
dog =
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
tidy =
```
```python
new
```
```python
dog();
 tidy.price =
```
```python
2000
```
```python
;
```
```python
console
```
```python
.log(dog.price);
```
```python
//undefined
```
这个明白吧？想一想我们上面说过这句话：
> 实例（
> tidy
> ）和 原型对象（
> dog.prototype
> ）存在一个连接。不过，要明确的真正重要的一点就是，这个连接存在于实例（
> tidy
> ）与构造函数的原型对象（
> dog.prototype
> ）之间，而不是存在于实例（
> tidy
> ）与构造函数（
> dog
> ）之间。
聪明的你肯定想通了吧 ：）
---
本文借鉴了：
[《JS原型与原型链终极详解》](https://link.jianshu.com?t=http://www.108js.com/article/article1/10201.html?id=1092)
作者：zhangjiahao8961
[JavaScript中](https://link.jianshu.com?t=http://www.cnblogs.com/snandy/archive/2012/09/01/2664134.html)proto[与prototype的关系](https://link.jianshu.com?t=http://www.cnblogs.com/snandy/archive/2012/09/01/2664134.html)
作者：snandy
《JavaScript 高级程序设计》中文译本 第三版

作者：Yi罐可乐
链接：https://www.jianshu.com/p/a4e1e7b6f4f8
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

