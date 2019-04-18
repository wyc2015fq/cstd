# 从Function.length 与 Argument.length 区别谈到如何传递任意个数参数 - weixin_33985507的博客 - CSDN博客
2018年07月01日 13:44:51[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
之前电面有问到：“你知道一个函数的length是什么吗？”
因为没看过，也没碰到过使用场景，我没答出来。后来查了下发现是指函数的参数个数，于是也就作罢了。不过今天恰巧碰到了使用场景，且发现之前的理解也有误，于是就写一篇短文分享一下。
今天在尝试 [render props](https://juejin.im/post/5b2f99ea6fb9a00e3a5aa511) 的各种扩展玩法，写了个简单的表格数据 CRUD 操作 Demo：[CodeSandbox](https://link.juejin.im?target=https%3A%2F%2F8xwoqv0zv9.codesandbox.io%2F)
写Demo一般从所有操作都同步开始，最后再全改成异步的情况。所以需要写一个将所有同步函数（数据加载以及增删改）都转换成异步函数的工具函数。我一开始写得如下：
```
const delay = ms => new Promise(_ => setTimeout(_, ms));
// 打算从今开始尽量使用 async/await
const withRequest = func => async args => {
  await delay(1000);
  if (Math.random() > 0.3) {
    func(...args);
  } else {
    message.info("操作失败！");
  }
};
// 等一秒之后 30% 失败，70% 执行操作
复制代码
```
这是个所谓的 Curried（库里？柯里？咖喱？）函数，用于批量改造函数的函数，接受func为参数，返回改造好的func。明眼人应该已经发现错在哪里了，不过我没有，于是走了一堆弯路，却收获不少。
用此函数包裹了我的一堆测试方法：
```
add = (a,b) => a + b
  square = a => a * a
  loadData = () => this.setState({ ... })
  
  loadData = withRequest(this.loadData);
  add = withRequest(this.add);
  square = withRequest(this.square);
复制代码
```
立马报错跪了，于是我知道在没有参数的 loadData 函数那里跪了，并开始了我的求知之旅。
### 问题：
如何将任意个参数从上级函数传递给下级函数？
### 笨办法解决：分情况讨论
分情况讨论的关键是：如何知道函数有几个参数呢？毫无疑问我想到了`fn.length`, 于是写下：
```
const len = func.length
if(len === 0){
  func()
} else if(len === 1) {
  func(args)
} else {
  func(...args)
}
复制代码
```
这对了吗？答案是不对。
**`fn.length`的定义是：函数的形参个数。也就是函数定义时的参数个数，而不是函数实际接受的参数个数**。比如
```
const add = (a,b) => a + b
add(1,2,3,4,5)  // 3
add.length  // 2
复制代码
```
而问题的情况，我们需要判断的是函数接受的参数个数。这时候有一个方便的内置变量：[arguments](https://link.juejin.im?target=https%3A%2F%2Fdeveloper.mozilla.org%2Fen-US%2Fdocs%2FWeb%2FJavaScript%2FReference%2FFunctions%2Farguments)
```
function func1(a, b, c) {
  console.log(arguments[0]); // 1
  console.log(arguments[1]); // 2
  console.log(arguments[2]); // 3
}
func1(1, 2, 3);
复制代码
```
arguments 即为函数接收到的所有参数组成的（类）数组。那么用 `arguments.length` 替换所有 `func.length` 是否就对了呢？还是不对，arguments 有它的局限性：
```
const func1 = (a, b, c) => {
  console.log(arguments[0]); // 1
  console.log(arguments[1]); // 2
  console.log(arguments[2]); // 3
}
func1(1, 2, 3);
// error: arguments is not defined
复制代码
```
**箭头函数没有arguments。** 同时注意到现在前端代码的箭头函数会经过 babel 转译，产生的结果是 arguments 虽然不会 undefined，但会有各种怪异赋值。总之在箭头函数里别使用。
### 真正的解决办法
[剩余参数](https://link.juejin.im?target=https%3A%2F%2Fdeveloper.mozilla.org%2Fzh-CN%2Fdocs%2FWeb%2FJavaScript%2FReference%2FFunctions%2FRest_parameters) (Rest parameters)
```
const withRequest = func => async (...args) => {
  await delay(1000);
  if (Math.random() > 0.3) {
    func(...args);
  } else {
    message.info("操作失败！");
  }
};
复制代码
```
这段代码里出现了两个 `...args`, 前者是剩余参数，后者是数组展开。两者一个是收束，一个是展开。功能相反。
```
function fun1(...args) {
  console.log(args.length);
}
 
fun1();  // 0
fun1(5); // 1
fun1(5, 6, 7); // 2
复制代码
```
剩余参数语法将“剩余”的参数收束到一个数组中, 注意和 arguments 不一样， **剩余参数是一个真正的数组**。绕了一大圈，其实是忘记写了三个点。不过也算真正理解了：
- Function.length
- arguments
- rest & spread
最后，让我们用剩余参数挑战一个实用函数吧：
### 问题
写一个`callAll`函数，它接收任意数量的函数和任意数量的参数，如果作为参数的函数存在就用所有的参数调用那个函数。
```
const add = (a,b) => {console.log(a + b)}
const minus = (a,b) => {console.log(a - b)}
callAll(add, minus)(2,1) 
// 3
// 1
复制代码
```
答案如下：
```
// 剩余参数是一个真正的数组，可以使用任何数组方法
const callAll = (...fns) => (...args) => fns.forEach( fn => fn && fn(...args))
复制代码
```
