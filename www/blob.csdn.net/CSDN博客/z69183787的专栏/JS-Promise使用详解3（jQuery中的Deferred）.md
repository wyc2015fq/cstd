# JS - Promise使用详解3（jQuery中的Deferred） - z69183787的专栏 - CSDN博客
2018年02月12日 16:44:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：174
[http://www.hangge.com/blog/cache/detail_1639.html](http://www.hangge.com/blog/cache/detail_1639.html)
上文我介绍了 **ES6 **中的 **Promise**，它完全遵循 **Promises/A** 规范。而我们熟悉的 **jQuery **又有自己的 **Promise **实现：**Deferred**（但其并不是遵循** Promises/A **规范）。本文就讲讲 **jQuery** 中 **Promise **的实现。
## 一、Deferred对象及其方法
### 1，$.Deferred
- **jQuery **用 **$.Deferred **实现了 **Promise **规范。
- **$.Deferred() **返回一个对象，我们可以称之为 **Deferred **对象，上面挂着一些熟悉的方法如：**done**、**fail**、**then **等。
- **jQuery **就是用这个 **Deferred **对象来注册异步操作的回调函数，修改并传递异步操作的状态。
下面我们定义做饭、吃饭、洗碗（**cook**、**eat**、**wash**）这三个方法（这里使用 **setTimeout **模拟异步操作）
```
`//做饭`
`function``cook(){`
`    ``console.log(``'开始做饭。'``);`
`    ``var``def = $.Deferred();`
`    ``//执行异步操作`
`    ``setTimeout(``function``(){`
`        ``console.log(``'做饭完毕！'``);`
`        ``def.resolve(``'鸡蛋炒饭'``);`
`    ``}, 1000);`
`    ``return``def.promise();`
`}`
`//吃饭`
`function``eat(data){`
`    ``console.log(``'开始吃饭：'``+ data);`
`    ``var``def = $.Deferred();`
`    ``//执行异步操作`
`    ``setTimeout(``function``(){`
`        ``console.log(``'吃饭完毕！'``);`
`        ``def.resolve(``'一块碗和一双筷子'``);`
`    ``}, 1000);`
`    ``return``def.promise();`
`}`
`//洗碗`
`function``wash(data){`
`    ``console.log(``'开始洗碗：'``+ data);`
`    ``var``def = $.Deferred();`
`    ``//执行异步操作`
`    ``setTimeout(``function``(){`
`        ``console.log(``'洗碗完毕！'``);`
`        ``def.resolve(``'干净的碗筷'``);`
`    ``}, 1000);`
`    ``return``def.promise();`
`}`
```
### 2，then()方法
通过 **Deferred** 对象的 **then **方法我们可以实现链式调用。
（1）比如上面样例的三个方法是层层依赖的关系，且下一步的的操作需要使用上一部操作的结果。我们可以这么写：
```
`cook()`
`.then(``function``(data){`
`    ``return``eat(data);`
`})`
`.then(``function``(data){`
`    ``return``wash(data);`
`})`
`.then(``function``(data){`
`    ``console.log(data);`
`});`
```
当然也可以简写成如下：
```
`cook()`
`.then(eat)`
`.then(wash)`
`.then(``function``(data){`
`    ``console.log(data);`
`});`
```
（2）运行结果如下：
![原文:JS - Promise使用详解3（jQuery中的Deferred）](http://www.hangge.com/blog_uploads/201703/2017033115040880465.png)
### 3，reject()方法
上面样例我们通过 **resolve **方法把 **Deferred **对象的状态置为完成态（**Resolved**），这时 **then **方法就能捕捉到变化，并执行“成功”情况的回调。
而 **reject **方法就是把 **Deferred **对象的状态置为已失败（**Rejected**），这时 **then **方法执行“失败”情况的回调（**then **方法的第二参数）。
（1）下面同样使用一个样例做演示
```
`//做饭`
`function``cook(){`
`    ``console.log(``'开始做饭。'``);`
`    ``var``def = $.Deferred();`
`    ``//执行异步操作`
`    ``setTimeout(``function``(){`
`        ``console.log(``'做饭完毕！'``);`
`        ``def.reject(``'烧焦的米饭'``);`
`    ``}, 1000);`
`    ``return``def.promise();`
`}`
`//吃饭`
`function``eat(data){`
`    ``console.log(``'开始吃饭：'``+ data);`
`    ``var``def = $.Deferred();`
`    ``//执行异步操作`
`    ``setTimeout(``function``(){`
`        ``console.log(``'吃饭完毕！'``);`
`        ``def.resolve(``'一块碗和一双筷子'``);`
`    ``}, 1000);`
`    ``return``def.promise();`
`}`
`cook()`
`.then(eat, ``function``(data){`
`  ``console.log(data + ``'没法吃!'``);`
`})`
```
运行结果如下：
![原文:JS - Promise使用详解3（jQuery中的Deferred）](http://www.hangge.com/blog_uploads/201703/201703311657181456.png)
（2）**Promise **规范中，**then **方法接受两个参数，分别是执行完成和执行失败的回调。而 **jQuery **中进行了增强，还可以接受第三个参数，就是在 **pending**（进行中）状态时的回调。
```
`deferred.then( doneFilter [, failFilter ] [, progressFilter ] )`
```
### 4，done()与fail()方法
**done **和 **fail **是 **jQuery **增加的两个语法糖方法。分别用来指定执行完成和执行失败的回调。
比如下面两段代码是等价的：
```
`//then方法`
`d.then(``function``(){`
`    ``console.log(``'执行完成'``);`
`}, ``function``(){`
`    ``console.log(``'执行失败'``);`
`});`
`//done方法、fail方法`
`d.done(``function``(){`
`    ``console.log(``'执行完成'``);`
`})`
`.fail(``function``(){`
`    ``console.log(``'执行失败'``);`
`});`
```
### 5，always()方法
**jQuery **的 **Deferred **对象上还有一个 **always **方法，不论执行完成还是执行失败，**always **都会执行，有点类似 **ajax **中的 **complete**。
```
`cook()`
`.then(eat)`
`.then(wash)`
`.always(``function``(){`
`  ``console.log(``'上班去!'``);`
`})`
```
## 二、与Promises/A规范的差异
在开头讲到，目前 **Promise **事实上的标准是社区提出的** Promises/A **规范，**jQuery **的实现并不完全符合 **Promises/A**，主要表现在对错误的处理不同。
### 1，ES6中对错误的处理
下面代码我们在回调函数中抛出一个错误，**Promises/A **规定此时 **Promise **实例的状态变为 **reject**，同时该错误会被下一个 **catch **方法指定的回调函数捕获。
```
`cook()`
`.then(``function``(data){`
`    ``throw``new``Error(``'米饭被打翻了！'``);`
`    ``eat(data);`
`})`
`.``catch``(``function``(data){`
`    ``console.log(data);`
`});`
```
### 2，jQuery中对错误的处理
同样我们在回调函数中抛出一个错误，**jQuery **的 **Deferred **对象此时不会改变状态，亦不会触发回调函数，该错误一般情况下会被** window.onerror **捕获。换句话说，在 **Deferred **对象中，总是必须使用 **reject **方法来改变状态。
```
`cook()`
`.then(``function``(data){`
`    ``throw``new``Error(``'米饭被打翻了！'``);`
`    ``eat(data);`
`})`
`window.onerror = ``function``(msg, url, line) {`
`    ``console.log(``"发生错误了："``+ msg);`
`    ``return``true``; ``//如果注释掉该语句，浏览器中还是会有错误提示，反之则没有。`
`}`
```
![原文:JS - Promise使用详解3（jQuery中的Deferred）](http://www.hangge.com/blog_uploads/201703/2017033120184319677.png)
## 三、$.when方法
**jQuery **中，还有一个 **$.when **方法。它与 **ES6 **中的 **all **方法功能一样，并行执行异步操作，在所有的异步操作执行完后才执行回调函数。当有两个地方要注意：
- **$.when **并没有定义在 **$.Deferred**中，看名字就知道，**$.when** 它是一个单独的方法。
- **$.when **与 **ES6 **的 **all **的参数稍有区别，它接受的并不是数组，而是多个 **Deferred **对象。
（1）比如下面代码，两个个异步操作是并行执行的，等到它们都执行完后才会进到 **then **里面。同时 **all **会把所有异步操作的结果传给 **then**。
```
`//切菜`
`function``cutUp(){`
`    ``console.log(``'开始切菜。'``);`
`    ``var``def = $.Deferred();`
`    ``//执行异步操作`
`    ``setTimeout(``function``(){`
`        ``console.log(``'切菜完毕！'``);`
`        ``def.resolve(``'切好的菜'``);`
`    ``}, 1000);`
`    ``return``def.promise();`
`}`
`//烧水`
`function``boil(){`
`    ``console.log(``'开始烧水。'``);`
`    ``var``def = $.Deferred();`
`    ``//执行异步操作`
`    ``setTimeout(``function``(){`
`        ``console.log(``'烧水完毕！'``);`
`        ``def.resolve(``'烧好的水'``);`
`    ``}, 1000);`
`    ``return``def.promise();`
`}`
`$.when(cutUp(), boil())`
`.then(``function``(data1, data2){`
`    ``console.log(``"准备工作完毕："``);`
`    ``console.log(data1, data2);`
`});`
```
## 四、Ajax函数与Deferred的关系
**jQuery **中我们常常会用到的 **ajax**, **get**, **post **等 **Ajax **函数，其实它们内部都已经实现了 **Deferred**。这些方法调用后会返回一个受限的 **Deferred **对象。既然是 **Deferred **对象，那么自然也有上面提到的所有特性。
### 1，then方法
比如我们通过链式调用，连续发送多个请求。
```
`req1 = ``function``(){`
`    ``return``$.ajax(``/*...*/``);`
`}`
`req2 = ``function``(){`
`    ``return``$.ajax(``/*...*/``);`
`}`
`req3 = ``function``(){`
`    ``return``$.ajax(``/*...*/``);`
`}`
`req1().then(req2).then(req3).done(``function``(){`
`    ``console.log(``'请求发送完毕'``);`
`});`
```
### 2，success、error与complete方法
**success**、**error**、**complete**是 **ajax **提供的语法糖，功能与 **Deferred **对象的 **done**、**fail**、**always **一致。比如下面两段代码功能是一致的：
```
`//使用success、error、complete`
`$.ajax(``/*...*/``)`
`.success(``function``(){``/*...*/``})`
`.error(``function``(){``/*...*/``})`
`.complete(``function``(){``/*...*/``})`
`//使用done、fail、always`
`$.ajax(``/*...*/``)`
`.done(``function``(){``/*...*/``})`
`.fai(``function``(){``/*...*/``})`
`.always(``function``(){``/*...*/``})`
```
原文出自：[www.hangge.com](http://www.hangge.com/)  转载请保留原文链接：[http://www.hangge.com/blog/cache/detail_1639.html](http://www.hangge.com/blog/cache/detail_1639.html)
