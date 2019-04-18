# Flutter/Dart中的异步 - weixin_33985507的博客 - CSDN博客
2019年01月30日 15:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
## 前言
我们所熟悉的前端开发框架大都是事件驱动的。事件驱动意味着你的程序中必然存在事件循环和事件队列。事件循环会不停的从事件队列中获取和处理各种事件。也就是说你的程序必然是支持异步的。
在Android中这样的结构是Looper/Handler；在iOS中是RunLoop；在JavaScript中是Event Loop。
同样的Flutter/Dart也是事件驱动的，也有自己的Event Loop。而且这个Event Loop和JavaScript的很像，很像。(毕竟Dart是想替换JS来着)。下面我们就来了解一下Dart中的Event Loop。
## Dart的Event Loop
Dart的事件循环如下图所示。和JavaScript的基本一样。循环中有两个队列。一个是微任务队列（MicroTask queue），一个是事件队列(Event queue)。
- 事件队列包含外部事件，例如I/O, `Timer`，绘制事件等等。
- 微任务队列则包含有Dart内部的微任务，主要是通过`scheduleMicrotask`来调度。
![1975877-7f5e775270e16dad](https://upload-images.jianshu.io/upload_images/1975877-7f5e775270e16dad)
Dart的Event Loop
Dart的事件循环的运行遵循以下规则：
- 首先处理**所有**微任务队列里的微任务。
- 处理完**所有**微任务以后。从事件队列里取**1个**事件进行处理。
- 回到微任务队列继续循环。
注意第一步里的**所有**，也就是说在处理事件队列之前，Dart要先把所有的微任务处理完。如果某一时刻微任务队列里有8个微任务，事件队列有2个事件，Dart也会先把这8个微任务全部处理完再从事件队列中取出1个事件处理，之后又会回到微任务队列去看有没有未执行的微任务。
> 
总而言之，就是对微任务队列是一次性全部处理，对于事件队列是一次只处理一个。
这个流程要清楚，清楚了才能理解Dart代码的执行顺序。
## 异步执行
那么在Dart中如何让你的代码异步执行呢？很简单，把要异步执行的代码放在微任务队列或者事件队列里就行了。
- 可以调用`scheduleMicrotask`来让代码以**微任务**的方式异步执行
```
scheduleMicrotask((){
        print('a microtask');
    });
```
- 可以调用`Timer.run`来让代码以**Event**的方式异步执行
```
Timer.run((){
       print('a event');
   });
```
好了，现在你知道怎么让你的Dart代码异步执行了。看起来并不是很复杂，但是你需要清楚的知道你的异步代码执行的顺序。这也是很多前端面试时候会问到的问题。举个简单的例子，请问下面这段代码是否会输出"executed"?
```
main() {
     Timer.run(() { print("executed"); });  
      foo() {
        scheduleMicrotask(foo);  
      }
      foo();
    }
```
答案是不会，因为在始终会有一个`foo`存在于微任务队列。导致Event Loop没有机会去处理事件队列。还有更复杂的一些例子会有大量的异步代码混合嵌套起来然后问你执行顺序是什么样的，这都需要按照上述Event Loop规则仔细去分析。
和JS一样，仅仅使用回调函数来做异步的话很容易陷入“回调地狱（Callback hell）”，为了避免这样的问题，JS引入了`Promise`。同样的, Dart引入了`Future`。
## Future
要使用`Future`的话需要引入`dart.async`
```
import 'dart:async';
```
`Future`提供了一系列构造函数供你选择。
创建一个立刻在事件队列里运行的`Future`:
```
Future(() => print('立刻在Event queue中运行的Future'));
```
创建一个延时1秒在事件队列里运行的`Future`:
```
Future.delayed(const Duration(seconds:1), () => print('1秒后在Event queue中运行的Future'));
```
创建一个在微任务队列里运行的`Future`:
```
Future.microtask(() => print('在Microtask queue里运行的Future'));
```
创建一个同步运行的Future：
```
Future.sync(() => print('同步运行的Future'));
```
对，你没看错，同步运行的。
> 
这里要注意一下，这个同步运行指的是构造`Future`的时候传入的函数是同步运行的，这个`Future`通过`then`串进来的回调函数是调度到微任务队列异步执行的。
有了`Future`之后, 通过调用`then`来把回调函数串起来，这样就解决了"回调地狱"的问题。
```
Future(()=> print('task'))
    .then((_)=> print('callback1'))
    .then((_)=> print('callback2'));
```
在task打印完毕以后，通过`then`串起来的回调函数会按照链接的顺序依次执行。
如果task执行出错怎么办？你可以通过`catchError`来链上一个错误处理函数：
```
Future(()=> throw 'we have a problem')
      .then((_)=> print('callback1'))
      .then((_)=> print('callback2'))
      .catchError((error)=>print('$error'));
```
上面这个`Future`执行时直接抛出一个异常，这个异常会被`catchError`捕捉到。类似于Java中的`try/catch`机制的`catch`代码块。运行后只会执行`catchError`里的代码。两个`then`中的代码都不会被执行。
既然有了类似Java的`try/catch`，那么Java中的`finally`也应该有吧。有的，那就是`whenComplete`:
```
Future(()=> throw 'we have a problem')
    .then((_)=> print('callback1'))
    .then((_)=> print('callback2'))
    .catchError((error)=>print('$error'))
    .whenComplete(()=> print('whenComplete'));
```
无论这个`Future`是正常执行完毕还是抛出异常，`whenComplete`都一定会被执行。
以上就是对`Future`的一些主要用法的介绍。`Future`背后的实现机制还是有一些复杂的。这里先列几个来自Dart官网的关于`Future`的烧脑说明。大家先感受一下：
> - 你通过then串起来的那些回调函数在`Future`完成的时候会被立即执 行，也就是说它们是同步执行，而不是被调度异步执行。
- 如果`Future`在调用`then`串起回调函数之前已经完成，
那么这些回调函数会被调度到微任务队列异步执行。
- 通过`Future()`和`Future.delayed()`实例化的`Future`不会同步执行，它们会被调度到事件队列异步执行。
- 通过`Future.value()`实例化的`Future`会被调度到微任务队列异步完成，类似于第2条。
- 通过`Future.sync()`实例化的`Future`会同步执行其入参函数，然后（除非这个入参函数返回一个`Future`）调度到微任务队列来完成自己，类似于第2条。
从上述说明可以得出结论，`Future`中的代码至少会有一部分被异步调度执行的，要么是其入参函数和回调被异步调度执行，要么就只有回调被异步调度执行。
不知道大家注意到没有，通过以上那些`Future`构造函数生成的`Future`对象其实控制权不在你这里。它什么时候执行完毕只能等系统调度了。你只能被动的等待`Future`执行完毕然后调用你设置的回调。如果你想手动控制某个`Future`怎么办呢？请使用`Completer`。
## Completer
这里就举个`Completer`的例子吧
```
// 实例化一个Completer
var completer = Completer();
// 这里可以拿到这个completer内部的Future
var future = completer.future;
// 需要的话串上回调函数。
future.then((value)=> print('$value'));
//做些其它事情 
...
// 设置为完成状态
completer.complete("done");
```
上述代码片段中，当你创建了一个`Completer`以后，其内部会包含一个`Future`。你可以在这个`Future`上通过`then`, `catchError`和`whenComplete`串上你需要的回调。拿着这个`Completer`实例，在你的代码里的合适位置，通过调用`complete`函数即可完成这个`Completer`对应的`Future`。控制权完全在你自己的代码手里。当然你也可以通过调用`completeError`来以异常的方式结束这个`Future`。
总结就是：
- 我创建的，完成了调我的回调就行了: 用 `Future`。
- 我创建的，得我来结束它： 用`Completer`。
`Future`相对于调度回调函数来说，缓减了回调地狱的问题。但是如果`Future`要串起来的的东西比较多的话，代码还是会可读性比较差。特别是各种`Future`嵌套起来，是比较烧脑的。
所以能不能更给力一点呢？可以的！JavaScript有 async/await，Dart也有。
## async/await
`async`和`await`是什么？它们是Dart语言的关键字，有了这两个关键字，可以让你用同步代码的形式写出异步代码。啥意思呢？看下面这个例子：
```
foo() async {
  print('foo E');
  String value = await bar();
  print('foo X $value');
}
bar() async {
  print("bar E");
  return "hello";
}
main() {
  print('main E');
  foo();
  print("main X");
}
```
函数`foo`被关键字`async`修饰，其内部的有3行代码，看起来和普通的函数没什么两样。但是在第2行等号右侧有个`await`关键字，`await`的出现让看似会同步执行的代码裂变为两部分。如下图所示：
![1975877-e8edd27d15f21f24](https://upload-images.jianshu.io/upload_images/1975877-e8edd27d15f21f24)
async await
绿框里面的代码会在`foo`函数被调用的时候同步执行，在遇到`await`的时候，会马上返回一个`Future`，剩下的红框里面的代码以`then`的方式链入这个`Future`被异步调度执行。
上述代码运行以后在终端会输出如下：
![1975877-3303418ce39bef4d](https://upload-images.jianshu.io/upload_images/1975877-3303418ce39bef4d)
output
可见`print('foo X $value')`是在`main`执行完毕以后才打印出来的。的确是异步执行的。
而以上代码中的`foo`函数可以以`Future`方式实现如下,两者是等效的
```
foo() {
  print('foo E');
  return Future(bar).then((value) => print('foo X $value'));
}
```
`await`并不像字面意义上程序运行到这里就停下来啥也不干等待`Future`完成。而是立刻结束当前函数的执行并返回一个`Future`。函数内剩余代码通过调度异步执行。
- `await`只能在`async`函数中出现。
- `async`函数中可以出现多个`await`,每遇见一个就返回一个`Future`, 实际结果类似于用`then`串起来的回调。
- `async`函数也可以没有`await`, 在函数体同步执行完毕以后返回一个`Future`。
使用`async`和`await`还有一个好处是我们可以用和同步代码相同的`try`/`catch`机制来做异常处理。
```
foo() async {
  try {
    print('foo E');
    var value = await bar();
    print('foo X $value');
  } catch (e) {
    // 同步执行代码中的异常和异步执行代码的异常都会被捕获
  } finally {
    
  }
}
```
在日常使用场景中，我们通常利用`async`，`await`来异步处理IO，网络请求，以及Flutter中的Platform channels通信等耗时操作。
## 总结
本文大致介绍了Flutter/Dart中的异步运行机制，从异步运行的基础（Event Loop）开始，首先介绍了最原始的异步运行机制，直接调度回调函数；到`Future`；再到 `async`和`await`。了解了Flutter/Dart中的异步运行机制是如何一步一步的进化而来的。对于一直从事Native开发，不太了解JavaScrip的同学来讲，这个异步机制和原生开发有很大的不同，需要多多动手练习，动脑思考才能适应。本文中介绍的相关知识点较为粗浅，并没有涉及`dart:async`中关于`Future`实现的源码分析以及`Stream`等不太常用的类。这些如果大家想了解一下的话我会另写文章来介绍一下。
