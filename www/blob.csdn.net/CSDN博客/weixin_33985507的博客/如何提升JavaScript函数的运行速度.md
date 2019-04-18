# 如何提升JavaScript函数的运行速度 - weixin_33985507的博客 - CSDN博客
2012年06月07日 21:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
Nicholas为您讲解如何提升JavaScript函数的运行速度！
这 篇是Nicholas讨论如果防止脚本失控的第二篇，主要讨论了如何重构嵌套循 环、递归，以及那些在函数内部同时执行很多子操作的函数。基本的思想和上一节trunk()那个例子一致，如果几个操作没有特定的执行顺序，而且互相不是 依赖关系，我们就可以通过异步调用的方式加以执行，不止可以减少执行的次数，还可以防止脚本失控。本文还介绍了通过memoization技术取代递归的 方法。
【原文标题】[Speed up your JavaScript, Part 2](http://www.nczonline.net/blog/2009/01/20/speed-up-your-javascript-part-2/)
 【原文作者】[Nicholas C. Zakas](http://www.nczonline.net/about/)
 上周我在《[too much happening in a loop](http://www.nczonline.net/blog/2009/01/13/speed-up-your-javascript-part-1/) 》（[译文](http://www.v-ec.com/dh20156/article.asp?id=213) ）这篇文章中介绍了JavaScript运行时间过长的第一个原因。相似的情况有时也出现在函数的定义上，函数也可能因为使用不当而过载使用。通常情况是函数内包含了过多的循环（不是在循环中执行了过多的内容），太多的递归，或者只不过是太多不相干但又要一起执行的操作。
 太 多的循环经常是以嵌套的形式出现，这种代码会一直占用JavaScript引擎直至循环结束。这方面有一个非常著名的例子，就是使用冒泡算法排序。由于 JavaScript有内置的sort()方法，我们没有必要使用这种方式进行排序，但我们可以借助这个算法理解嵌套循环占用资源的症结所在，从而避免类 似情况的发生。下面是一个在JavaScript使用冒泡排序法的典型例子：
```
function bubbleSort(items) {
 for (var i = items.length - 1; i >= 0; i--) {
    for (var j = i; j >= 0; j--) {
        if (items[j] < items[j - 1]) {
            var temp = items[j];
            items[j] = items[j - 1];
            items[j - 1] = temp;
        }
    }
 }
 }
```
回 忆一下你在学校学习的计算机知识，你可能记得冒泡排序法是效率最低的排序算法之一，原因是对于一个包含n个元素的数组，必须要进行n的平方次的循环操作。 如果数组中的元素数非常大，那么这个操作会持续很长时间。内循环的操作很简单，只是负责比较和交换数值，导致问题的最大原因在于循环执行的次数。这会导致 浏览器运行异常，潜在的直接结果就是那个脚本失控的警告对话框。
 几年前，Yahoo的研究员Julien Lecomte写了一篇题为《[Running CPU Intensive JavaScript Computations in a Web Browser](http://www.julienlecomte.net/blog/2007/10/28/) 》的文章，在这篇文章中作者阐述了如何将很大的javaScript操作分解成若干小部分。其中一个例子就是将冒泡排序法分解成多个步骤，每个步骤只遍历一次数组。我对他的代码做了改进，但方法的思路还是一样的：
```
function bubbleSort(array, onComplete) {
 var pos = 0; (function() {
    var j, value;
    for (j = array.length; j > pos; j--) {
        if (array[j] < array[j - 1]) {
            value = data[j];
            data[j] = data[j - 1];
            data[j - 1] = value;
        }
    }
    pos++;
    if (pos < array.length) {
        setTimeout(arguments.callee, 10);
    } else {
        onComplete();
    }
 })();
 }
```
这 个函数借助一个异步管理器来实现了冒泡算法，在每次遍历数组以前暂停一下。onComplete()函数会在数组排序完成后触发，提示用户数据已经准备 好。bubbleSort()函数使用了和chunk()函数一样的基本技术（参考我的上一篇帖子），将行为包装在一个匿名函数中，将 arguments.callee传递给setTimeout()以达到重复操作的目的，直至排序完成。如果你要将嵌套的循环拆解成若干个小步骤，以达到 解放浏览器的目的，这个函数提供了不错的指导意见。
 相似的问题还包括过多的递归。每个额外的递归调用都会占用更多的内存，从而减慢浏览器的运行。恼人的是，你可能在浏览器发出脚本失控警告之前，就耗尽了系统的内存，导致浏览器处于停止响应的状态。Crockford在博客上曾经对[这个问题](http://yuiblog.com/blog/2008/12/23/video-crockford-performance/) 进行过深入的讨论。他当时使用的例子，就是用递归生成一个斐波那契数列。
```
function fibonacci(n) {
 return n < 2 ? n: fibonacci(n - 1) + fibonacci(n - 2);
 };
```
按照Crockford的说法，执行fibonacci(40)这条语句将重复调用自身331160280次。避免使用递归的方案之一就是使用[memoization](http://en.wikipedia.org/wiki/Memoization) 技术，这项技术可以获取上一次调用的执行结果。Crockford介绍了下面这个函数，可以为处理数值的函数增加这项功能：
```
function memoizer(memo, fundamental) {
 var shell = function (n) {
    var result = memo[n];
    if (typeof result !== 'number') {
        result = fundamental(shell, n);
        memo[n] = result;
    }
    return result;
 };
 return shell;
 };
```
他接下来将这个函数应用在斐波那契数列生成器上：
```
var fibonacci = memoizer([0, 1],
 function(recur, n) {
   return recur(n - 1) + recur(n - 2);
 });
```
这时如果我们再次调用fibonacci(40)，只会重复调用40次，和原来相比提高得非常多。memoization的原理，概括起来就一句话，同样的结果，你没有必要计算两次。如果一个结果你可能会再次使用，把这个结果保存起来，总比重新计算一次来的快。
 最后一个可能让函数执行缓慢的原因，就是我们之前提到过的，函数里面执行了太多的内容，通常是因为使用了类似下面的开发模式：
```
function doAlot() {
   doSomething();
   doSomethingElse();
   doOneMoreThing();
 }
```
在这里要执行三个不同的函数，请注意，无论是哪个函数，在执行过程中都不依赖其他的函数，他们在本质是相对独立的，只是需要在一个特定时间逐一执行而已。同样，你可以使用类似chunk()的方法来执行一系列函数，而不会导致锁定浏览器。
```
function schedule(functions, context) {
 setTimeout(function() {
    var process = functions.shift();
    process.call(context);
    if (functions.length > 0) {
        setTimeout(arguments.callee, 100);
    }
 },
 100);
 }
```
schedule函数有两个参数，一个是包含要执行函数的数组，另外一个是标明this所属的上下文对象。函数数组以队列方式实现，Timer事件每次触发的时候，都会将队列最前面的函数取出并执行，这个函数可以通过下面的方式执行一系列函数：
`schedule([doSomething, doSomethingElse, doOneMoreThing], window);`
很希望各个JavaScript的类库都增加类似这样的进程处理函数。YUI在3.0时就已经引入了[Queue](http://developer.yahoo.com/yui/3/queue/) 对象，可以通过timer连续调用一组函数。
 无 论现有的技术可以帮助我们将复杂的进程拆分到什么程度，对于开发者来说，使用这种方法来理解并确定脚本失控的瓶颈是非常重要的。无论是太多的循环、递归还 是其他的什么，你现在应该知道如果处理类似的情况。但要记住，这里提到的技术和函数只是起到抛砖引玉的作用，在实际的应用中，你应该对它们加以改进，这样 才能发挥更大的作用。
**未知来源 欢迎告之**
