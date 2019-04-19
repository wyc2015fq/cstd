# JavaScript属性的可迭代、可修改和可配置特性 - 定义域 - CSDN博客
置顶2016年02月03日 18:01:40[定义域](https://me.csdn.net/u010278882)阅读数：1970
# JavaScript属性的可迭代、可修改和可配置特性
> 
**作者：** Javier Márquez 
**原题：**[JavaScript properties are enumerable, writable and configurable](http://arqex.com/967/javascript-properties-enumerable-writable-configurable)
**摘要：**对属性的属性做了定义并通过示例说明了者几个属性的属性的应用场景, 比如不可枚举用于序列化、不可修改用于常量、不可配置用于完全不可变对象.
对象是JavaScript的重要部分. JavaScript 的对象语法十分精确并容易使用，所以我们经常将对象作为 HashMap 使用。
```java
// 我心爱的对象 ob
var ob = {a:1};
// 访问属性
ob.a; // => 1
// 修改属性值
ob.a = 0;
ob.a; // => 0
// 创建新属性
ob.b = 2;
ob.b; // => 2
// 删除属性
delete ob.b;
ob.b; // => undefined
```
你知道上面代码中的对象的所有属性都是可迭代、可修改和可配置的吗? 
- 可迭代，意味着我能够通过 `for..in` 循环来访问该对象的所有属性. 还能通过 `Object.keys()` 方法获取该对象的所有属性名.
- 可修改，意味着我能修改该对象的所有属性的值，通过为这些属性赋予一个新值就能修改: `ob.a = 1000;`.
- 可配置，意味着我能修改属性的行为，让该对象的属性都是不可迭代的、不可修改的和不可配置的. 只有可配置的属性才能通过 `delete` 被删除.
我感打赌你知道`Object`属性的前两个特性（译者注：这里用特性描述属性的属性，而属性用来描述对象的属性.）, 但是只有少数人知道通过调用 `Object` 的 `defineProperty` 方法能够**创建对象的属性、修改对象的属性为不可迭代的和不可变的**.
```java
// 通过 Object.defineProperty 为 ob 添加一个属性
Object.defineProperty(ob, 'c', {
    value: 3,
    enumerable: false,
    writable: false,
    configurable: false
});
ob.c; // => 3
Object.getOwnPropertyDescriptor(ob, 'c');
// => {value:3, enumeralbe: false, writable: false, configurable: false}
```
我估计这种语法并不像平常的语法那么常见, 但是使用这种属性在解决某些问题时确实很方便. `defineProperty` 方法中定义对象属性的对象称为**描述符（descriptor）**，可以通过 `getOwnPropertyDescriptor` 方法来查看任何属性的描述符.
有趣的是`Object.defineProperty`中属性描述符的默认选项值与上面的例子中的值正好相反：描述符默认是不可迭代、不可修改和不可配置的.
```java
// 这里的 f 属性是不可迭代、修改和配置的
Object.defineProperty(ob, 'f', {value: 6});
```
此外，还可以在通过`Object.create(prototype, properties)`方法创建对象时指定对象属性的描述符. 
```java
var ob = Object.create(Object.prototype, {
    a: {writable: true, enumerable: true, value: 1},
    b: {enumerable: true, value: 2}
});
ob; // => {a:1, b:2}
```
## 对象的不可枚举属性
前面说过, 可枚举属性可以通过`for..in`循环来访问, 而不可枚举属性不能. 基本上来说, 非枚举属性对于大多数将对象作为 HashMap 来使用的对象来说都是不能使用的.
- 不可枚举属性不能通过`for..in`迭代
- `Object.keys` 函数不能返回
- 不能通过`JSON.stringify`序列化为JSON字符串
所以这类属性就像是某种秘密属性, 但是仍然能够访问.
```java
var ob = {a:1, b:2};
ob.c = 3;
Object.defineProperty(ob, 'd', {
    value: 4,
    enumerable: false
});
ob.d; // => 4
for( var key in ob ) { console.log(ob[key]) };
// 1
// 2
// 3
Object.keys(ob); // => ["a", "b", "c"]
JSON.stringify(ob); // => "{a:1, b:2, c:3}"
ob.d; // => 4
```
由于这类属性不能序列化, 我发现在处理数据模型对象时特别有用. 能够通过使用不可枚举属性来方便的添加信息到数据模型对象中.
```java
// 下面的模型表示一辆车, 它通过id 属性来访问其所有者
var car = {
    id: 123,
    color: red,
    owner: 12
};
// 从数据库中取回下面这个所有人
var owner = {
    id: 12,
    name: Javi
};
// 那么我可以为小车添加一个所有人属性（不可枚举的属性）
Object.defineProperty( car, 'ownerOb', {value: owner});
// 现在需要车的所有人数据
car.ownerDb; // => {id:12, name: Javi}
// 但是如果对小车对象序列化, 又能够不显示车的所有人对象
JSON.stringify( car ); // => "{id: 123, color: red, owner: 12}"
```
考虑一下，如果要创建一个ORM库的话，如果有该特性，那会有多方便.
如果要获取对象的所有属性, 包括可枚举的和不可枚举的, 可以通过`Object.getOwnPropertyNames`获取对象的所有属性名.
## 对象的不可修改属性
虽然到ES6的时候我们就有一直以来都期望的`const`语句, 但现在我们就能通过可修改属性模拟常量. 不可修改的属性的属性值是不可修改的.
```java
var ob = {a: 1};
Object.defineProperty( ob, 'B', {value: 2, writable: false});
ob.B; // => 2
ob.B = 10;
ob.B; // => 2
```
正如输出结果所示, 对 `ob.B` 的赋值并没有影响它的值. 这里需要需要，赋值语句总会返回被赋予的值, 无论赋值是否成功. 比如, 这里的不可修改属性就是赋值失败的. 而在严格模式（`strict mode`）下，尝试对不可修改属性赋值会引发`TypeError`异常.
```java
var ob = {a:1};
Object.defineProperty(ob, 'B', {value: 2, writable: false});
// 赋值语句会返回所赋予的值
ob.B = 6; // => 6
ob.B = 1000; // => 1000
// 无论赋予什么值, 属性值始终不变
ob.B; // => 2
function updateDB() {
    'use strict';
    ob.B = 4; // 这里会引发异常
}
updateDB(); // 抛出异常
```
此外, 如果不可修改属性的值是一个对象的话, 那么对该对象的引用是不可修改的, 但是引用的对象的属性是可以修改的.
```java
var ob = {a: 1};
Object.defineProperty(ob, 'B', {value: {c: 3}, writable: false});
ob.OB.C = 4;
ob.OB.D = 5;
ob.OB; // => {c: 4, d: 5}
ob.OB = 'hola';
ob.OB; // => {c: 4, d: 5}
```
如果想要让属性完全的不可修改, 可以用`Object.freeze`函数. 它让对对象的属性的添加、删除、修改都无效. 而且在严格模式下还会引发 `TypeError` 异常.
```java
var ob = {a:1, b:2};
ob.c = 3;
// Freeze! 冻结
Object.freeze(ob); // => {a:1, b:2, c:2}
ob.d = 4;
ob.a = -10;
delete ob.b;
Object.defineProperty(ob, 'e', {value: 5});
// 任何对对象的修改都被忽略
ob; // => {a:1, b:3, c:3}
```
## 对象的不可配置属性
如果上面的对象是定义为可配置的，那么你还对其进行修改. 还可以通过`defineProperty`来该变属性为可修改的或不可迭代的. 但是如果一旦定义属性为不可配置的，那么能做的事就只有一件了: 如果该属性是可修改的, 那么可以将其改为不可修改的. 其他任何类型的更新都将引发 `TypeError` 异常.
```java
var ob = {};
Object.defineProperty(ob, 'a', {configurable: false, writable: true});
Object.defineProperty(ob, 'a', {enumerable: true}); // 抛出 TypeError 异常
Object.defineProperty(ob, 'a', {value: 12}); // 抛出 TypeError 异常
Object.defineProperty(ob, 'a', {writable: fasle}); // 唯一允许的操作
Object.defineProperty(ob, 'a', {writable: true}); // 抛出 TypeError 异常
```
特别重要的一点, 可配置属性能能够通过 `delete` 操作符从对象中移除. 所以, 如果你创建了一个不可配置和不可修改的属性, 那该属性就是一个被冻结的属性.
```java
var ob = {};
Object.defineProperty(ob, 'a', {configurable: true, value: 1});
ob; // => {a:1}
delete ob.a; // => true
ob; // => {}
Object.defineProperty(ob, 'a', {configurable: false, value: 1});
ob; // => {a:1}
delete ob.b; // => false
ob; // => {a:1}
```
## 结论
`Object.defineProperty`是从ES5引入的, 现在就可以开始使用了, 其被所有现代浏览器支持, 包括 IE9([甚至是 IE8, 但其仅支持 DOM 对象](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/defineProperty#Internet_Explorer_8_specific_notes)). 通过不同的方式来实现我们之前常常做的任务总是很有趣, 并且通过观察 JavaScript 的核心（即对象）是如何工作的, 我们能够轻松的学习新知识.
`Object.defineProperty`还给了我们能够自定义getter和setter的自由, 不过今天的文章没有涉及到. 如果想要了解更多, 那就去看看一定会令人有意外收获的[Mozilla’ Documentation](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/defineProperty)吧.
