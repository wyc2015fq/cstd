# 年糕のES6笔记之Generator函数 - weixin_33985507的博客 - CSDN博客
2018年04月29日 15:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
# 前言（重点）
这一系列的文章是我搜集了很多文章和资料综合在一起整理出的一个适合我自己学习的笔记，并不是原创内容。
为什么放在最前面，因为我觉得原作者才是最辛苦哒。笔记也是持续更新的，若有错误欢迎提出。
**学习资料：**
- ECMAScript 6入门：[http://es6.ruanyifeng.com/#docs/generator](https://link.jianshu.com?t=http%3A%2F%2Fes6.ruanyifeng.com%2F%23docs%2Fgenerator)
- 你知道为什么会有 Generator 吗：[https://juejin.im/post/5adae8246fb9a07aa541e150](https://link.jianshu.com?t=https%3A%2F%2Fjuejin.im%2Fpost%2F5adae8246fb9a07aa541e150)
# 出现の理由
# Generatorの定义
## 下定义
Generator函数是一个**必须符合以下两个条件的函数**：
- 函数名前有*号。
- 函数内部使用了`yield`关键字。
```
function *name {
  yield 'hello';
}
```
**Q：为什么叫做Generator？**
A：因为Generator返回一个遍历器对象，所以它可以被认为是一个遍历器对象生成器，Generator这个名字很符合它。
**Q：定义时*的位置有影响吗?**
A：ES6没有明确规定星号位置。以下写法都对，但是最普遍的使用第二种：
```
function * foo(x, y) { ··· }
function *foo(x, y) { ··· }
function* foo(x, y) { ··· }
function*foo(x, y) { ··· }
```
**Q：yield是什么意思？**
A：yield表达式就是**暂停标志**。当Generator调用后返回了迭代器对象，我们可以对这个对象使用next()方法，它会执行了一个yield表达式后就停止下来。直到下一次再调用next()又会执行到下一个yield表达式后。
```
function* generator() {
  yield 'hello';
  yield 'world';
  return 'ending';
}
var test = generator();
test.next()  // { value: 'hello', done: false }
test.next()  // { value: 'world', done: false }
test.next()  // { value: 'ending', done: true }
test.next()  // { value: undefined, done: true }
```
## 划区别
Generator函数和普通函数的区别：
- 上面“下定义”写的必须符合的两个条件。
- 调用Generator函数后，函数并不执行，返回的是一个指向内部状态的指针对象（遍历器对象）。
```
function* test() {
  yield 1;
  yield 2;
  yield 3;
}
// 返回一个 iterator，可以继续调用next()方法
let iterator = test();
console.log(iterator.next().value); // 1
console.log(iterator.next().value); // 2
console.log(iterator.next().value); // 3
```
# 使用の方法
**注意点：**
- yield只能在Generator函数里使用，否则报错。
- yield如果用在另一个表达式之中，必须放在圆括号里面。
- 箭头函数不能做Generator函数。
