# 深度解析ES6 迭代器与可迭代对象的实现 - weixin_33985507的博客 - CSDN博客
2019年02月21日 08:35:45[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
ES6 新的数组方法、集合、for-of 循环、展开运算符（...）甚至异步编程都依赖于迭代器（Iterator ）实现。本文会详解 ES6 的迭代器与生成器，并进一步挖掘可迭代对象的内部原理与使用方法
![](https://upload-images.jianshu.io/upload_images/12577968-44311878ffc173cb.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**一、迭代器的原理**
在编程语言中处理数组或集合时，使用循环语句必须要初始化一个变量记录迭代位置，而程序化地使用迭代器可以简化这种数据操作
**如何设计一个迭代器呢？**
迭代器的本身是一个对象，这个对象有 next( ) 方法返回结果对象，这个结果对象有下一个返回值 value、迭代完成布尔值 done，模拟创建一个简单迭代器如下：
```
function createIterator(iterms) {
 let i = 0
 return {
  next() {
   let done = (i >= iterms.length)
   let value = !done ? iterms[i++] : undefined
   return {
    done,
    value
   }
  }
 }
}
 //在此我向大家推荐一个前端全栈开发交流圈：619586920  突破技术瓶颈，提升思维能力
let arrayIterator = createIterator([1, 2, 3])
 
console.log(arrayIterator.next()) // { done: false, value: 1 }
console.log(arrayIterator.next()) // { done: false, value: 2 }
console.log(arrayIterator.next()) // { done: false, value: 3 }
console.log(arrayIterator.next()) // { done: true, value: undefined }
```
每次调用迭代器的 next( ) 都会返回下一个对象，直到数据集被用尽。
ES6 中迭代器的编写规则类似，但引入了生成器对象，更简单的创建迭代器对象
**二、创建迭代器**
ES6 封装了一个生成器用来创建迭代器。显然生成器是返回迭代器的函数，这个函数通过 function 后的星号（*）表示，并使用新的内部专用关键字yield指定迭代器 next( ) 方法的返回值。
**如何使用 ES6 生成器创建一个迭代器呢？**一个简单的例子如下：
```
function *createIterator() {
 yield 864305860;
 yield 'someValue'
}
 
let someIterator = createIterator()
 //在此我向大家推荐一个前端全栈开发交流圈：619586920  突破技术瓶颈，提升思维能力
console.log(someIterator.next()) // { value: 864305860, done: false }
console.log(someIterator.next()) // { value: 'someValue', done: false }
console.log(someIterator.next()) // { value: undefined, done: true }
```
使用yield关键字可以返回任意值或表达式，可以给迭代器批量添加元素：
```
// let createIterator = function *(items) { // 生成器函数表达式
function *createIterator(items) {
 for (let i = 0; i < items.length; i++) {
  yield items[i]
 }
}
 
let someIterator = createIterator([864305860, 'someValue'])
 
console.log(someIterator.next()) // { value: 864305860, done: false }
console.log(someIterator.next()) // { value: 'someValue', done: false }
console.log(someIterator.next()) // { value: undefined, done: true }
```
由于生成器本身是函数，所以可添加到对象中，使用方式如下：
```
let obj = {
 // createIterator: function *(items) { // ES5
 *createIterator(items) { // ES6
  for (let i = 0; i < items.length; i++) {
   yield items[i]
  }
 }
}
let someIterator = obj.createIterator([864305860, 'someValue'])
```
生成器函数的一个特点是，当执行完一句 yield 语句后函数会自动停止执行，再次调用迭代器的 next( ) 方法才会继续执行下一个 yield 语句。
这种自动中止函数执行的能力衍生出很多高级用法。
**三、可迭代对象**
在 ES6 中常用的集合对象（数组、Set/Map集合）和字符串都是可迭代对象，这些对象都有默认的迭代器和`Symbol.iterator`属性。
通过生成器创建的迭代器也是可迭代对象，因为生成器默认会为Symbol.iterator属性赋值。
**3.1 Symbol.iterator
可迭代对象具有  `Symbol.iterator`属性，即具有  `Symbol.iterator`  属性的对象都有默认迭代器。
我们可以用  `Symbol.iterator`  来访问对象的默认迭代器，例如对于一个数组：
```
let list = [11, 22, 33]
let iterator = list[Symbol.iterator]()
console.log(iterator.next()) // { value: 11, done: false }
```
`Symbol.iterator`  获得了数组这个可迭代对象的默认迭代器，并操作它遍历了数组中的元素。
反之，我们可以用  `Symbol.iterator`  来检测对象是否为可迭代对象：
```
function isIterator(obj) {
 return typeof obj[Symbol.iterator] === 'function'
}
 
console.log(isIterator([11, 22, 33])) // true
console.log(isIterator('sometring')) // true
console.log(isIterator(new Map())) // true
console.log(isIterator(new Set())) // true
console.log(isIterator(new WeakMap())) // false
console.log(isIterator(new WeakSet())) // false
```
显然数组、Set/Map 集合、字符串都是可迭代对象，而 WeakSet/WeakMap 集合（弱引用集合）是不可迭代的。
**3.2 创建可迭代对象**
默认情况下，自定义的对象都是不可迭代的。
刚才讲过，通过生成器创建的迭代器也是一种可迭代对象，生成器默认会为Symbol.iterator属性赋值。
那如何将自定义对象变为可迭代对象呢？通过给Symbol.iterator属性添加一个生成器：
```
let collection = {
 items: [11,22,33],
 *[Symbol.iterator]() {
  for (let item of this.items){
   yield item
  }
 }
}
 
console.log(isIterator(collection)) // true
//在此我向大家推荐一个前端全栈开发交流圈：619586920  突破技术瓶颈，提升思维能力 
for (let item of collection){
 console.log(item) // 11 22 33
}
```
数组 items 是可迭代对象，collection 对象通过给Symbol.iterator属性赋值也成为可迭代对象。
**3.3 for-of**
注意到上个栗子使用了for-of代替索引循环，for-of是 ES6 为可迭代对象新加入的特性。
思考一下for-of循环的实现原理。
对于使用for-of的可迭代对象，for-of每执行一次就会调用这个可迭代对象的 next( )，并将返回结果存储在一个变量中，持续执行直到可迭代对象 done 属性值为 false。
```
// 迭代一个字符串
let str = 'somestring'
 
for (let item of str){
 console.log(item) // s o m e s t r i n g
}
```
本质上来说，for-of调用 str 字符串的Symbol.iterator属性方法获取迭代器（这个过程由 JS 引擎完成），然后多次调用 next( ) 方法将对象 value 值存储在 item 变量。
将for-of用于不可迭代对象、null 或 undefined 会报错！
**3.4 展开运算符（...）**
ES6 语法糖展开运算符（...）也是服务于可迭代对象，即只可以“展开”数组、集合、字符串、自定义可迭代对象。
以下栗子输出不同可迭代对象展开运算符计算的结果：
```
let str = 'somestring'
console.log(...str) // s o m e s t r i n g
  
let set = new Set([1, 2, 2, 5, 8, 8, 8, 9])
console.log(set) // Set { 1, 2, 5, 8, 9 }
console.log(...set) // 1 2 5 8 9
  
let map = new Map([['name', '前端全栈开发交流圈'], ['id', 864305860]])
console.log(map) // Map { 'name' => '前端全栈开发交流圈', 'id' => 864305860 }
console.log(...map) // [ 'name', '前端全栈开发交流圈' ] [ 'id', 864305860 ]
 
let num1 = [1, 2, 3], num2 = [7, 8, 9]
console.log([...num1, ...num2]) // [ 1, 2, 3, 7, 8, 9 ]
//在此我向大家推荐一个前端全栈开发交流圈：619586920  突破技术瓶颈，提升思维能力  
let udf
console.log(...udf) // TypeError: undefined is not iterable
```
由以上代码可以看出，展开运算符（...）可以便捷地将可迭代对象转换为数组。同for-of一样，展开运算符（...）用于不可迭代对象、null 或 undefined 会报错！
**四. 默认迭代器**
ES6 为很多内置对象提供了默认的迭代器，只有当内建的迭代器不能满足需求时才自己创建迭代器。
ES6 的 三个集合对象：Set、Map、Array 都有默认的迭代器，常用的如values()方法、entries()方法都返回一个迭代器，其值区别如下：
- entries()：多个键值对
- values()：集合的值
- keys()：集合的键
调用以上方法都可以得到集合的迭代器，并使用for-of循环，示例如下：
```
/******** Map ***********/
let map = new Map([['name', '前端全栈开发交流圈'], ['id', 864305860]])
 
for(let item of map.entries()){
 console.log(item) // [ 'name', '前端全栈开发交流圈' ] [ 'id', 864305860 ]
}
for(let item of map.keys()){
 console.log(item) // name id
}
for (let item of map.values()) {
 console.log(item) // 前端全栈开发交流圈 864305860
}
 //在此我向大家推荐一个前端全栈开发交流圈：619586920  突破技术瓶颈，提升思维能力
/******** Set ***********/
let set = new Set([1, 4, 4, 5, 5, 5, 6, 6,])
 
for(let item of set.entries()){
 console.log(item) // [ 1, 1 ] [ 4, 4 ] [ 5, 5 ] [ 6, 6 ]
}
 
/********* Array **********/
let array = [11, 22, 33]
 
for(let item of array.entries()){
 console.log(item) // [ 0, 11 ] [ 1, 22 ] [ 2, 33 ]
}
```
此外 String 和 NodeList 类型都有默认的迭代器，虽然没有提供其它的方法，但可以用for-of循环
**结语**
感谢您的观看，如有不足之处，欢迎批评指正。
