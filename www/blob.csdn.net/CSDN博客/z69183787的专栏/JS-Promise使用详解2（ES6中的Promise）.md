# JS - Promise使用详解2（ES6中的Promise） - z69183787的专栏 - CSDN博客
2018年02月12日 16:27:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：136
个人分类：[前端-Es6/7](https://blog.csdn.net/z69183787/article/category/6763766)
[http://www.hangge.com/blog/cache/detail_1638.html](http://www.hangge.com/blog/cache/detail_1638.html)
2015年6月， **ES2015**（即 **ECMAScript 6**、**ES6**） 正式发布。其中 **Promise **被列为正式规范，成为 **ES6 **中最重要的特性之一。
### 1，then()方法
简单来讲，**then **方法就是把原来的回调写法分离出来，在异步操作执行完后，用链式调用的方式执行回调函数。
而 **Promise **的优势就在于这个链式调用。我们可以在 **then **方法中继续写 **Promise **对象并返回，然后继续调用 **then **来进行回调操作。
（1）下面通过样例作为演示，我们定义做饭、吃饭、洗碗（**cook**、**eat**、**wash**）这三个方法，它们是层层依赖的关系，下一步的的操作需要使用上一部操作的结果。（这里使用 **setTimeout **模拟异步操作）
```
`//做饭`
`function``cook(){`
`    ``console.log(``'开始做饭。'``);`
`    ``var``p = ``new``Promise(``function``(resolve, reject){        ``//做一些异步操作`
`        ``setTimeout(``function``(){`
`            ``console.log(``'做饭完毕！'``);`
`            ``resolve(``'鸡蛋炒饭'``);`
`        ``}, 1000);`
`    ``});`
`    ``return``p;`
`}`
`//吃饭`
`function``eat(data){`
`    ``console.log(``'开始吃饭：'``+ data);`
`    ``var``p = ``new``Promise(``function``(resolve, reject){        ``//做一些异步操作`
`        ``setTimeout(``function``(){`
`            ``console.log(``'吃饭完毕!'``);`
`            ``resolve(``'一块碗和一双筷子'``);`
`        ``}, 2000);`
`    ``});`
`    ``return``p;`
`}`
`function``wash(data){`
`    ``console.log(``'开始洗碗：'``+ data);`
`    ``var``p = ``new``Promise(``function``(resolve, reject){        ``//做一些异步操作`
`        ``setTimeout(``function``(){`
`            ``console.log(``'洗碗完毕!'``);`
`            ``resolve(``'干净的碗筷'``);`
`        ``}, 2000);`
`    ``});`
`    ``return``p;`
`}`
```
（2）使用 **then **链式调用这三个方法：
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
当然上面代码还可以简化成如下：
```
`cook()`
`.then(eat)`
`.then(wash)`
`.then(``function``(data){`
`    ``console.log(data);`
`});`
```
（3）运行结果如下：
![原文:JS - Promise使用详解2（ES6中的Promise）](http://www.hangge.com/blog_uploads/201703/2017033109253593420.png)
### 2，reject()方法
上面样例我们通过 **resolve **方法把 **Promise **的状态置为完成态（**Resolved**），这时 **then **方法就能捕捉到变化，并执行“成功”情况的回调。
而 **reject **方法就是把 **Promise **的状态置为已失败（**Rejected**），这时 **then **方法执行“失败”情况的回调（**then **方法的第二参数）。
（1）下面同样使用一个样例做演示
```
`//做饭`
`function``cook(){`
`    ``console.log(``'开始做饭。'``);`
`    ``var``p = ``new``Promise(``function``(resolve, reject){        ``//做一些异步操作`
`        ``setTimeout(``function``(){`
`            ``console.log(``'做饭失败！'``);`
`            ``reject(``'烧焦的米饭'``);`
`        ``}, 1000);`
`    ``});`
`    ``return``p;`
`}`
`//吃饭`
`function``eat(data){`
`    ``console.log(``'开始吃饭：'``+ data);`
`    ``var``p = ``new``Promise(``function``(resolve, reject){        ``//做一些异步操作`
`        ``setTimeout(``function``(){`
`            ``console.log(``'吃饭完毕!'``);`
`            ``resolve(``'一块碗和一双筷子'``);`
`        ``}, 2000);`
`    ``});`
`    ``return``p;`
`}`
`cook()`
`.then(eat, ``function``(data){`
`  ``console.log(data + ``'没法吃!'``);`
`})`
```
运行结果如下：
![原文:JS - Promise使用详解2（ES6中的Promise）](http://www.hangge.com/blog_uploads/201703/2017033109493786299.png)
（2）如果我们只要处理失败的情况可以使用 **then(null, ...)**，或是使用接下来要讲的 **catch **方法。
```
`cook()`
`.then(``null``, ``function``(data){`
`  ``console.log(data + ``'没法吃!'``);`
`})`
```
### 3，catch()方法
（1）它可以和 **then **的第二个参数一样，用来指定 **reject **的回调
```
`cook()`
`.then(eat)`
`.``catch``(``function``(data){`
`    ``console.log(data + ``'没法吃!'``);`
`});`
```
（2）它的另一个作用是，当执行 **resolve **的回调（也就是上面 **then **中的第一个参数）时，如果抛出异常了（代码出错了），那么也不会报错卡死 **js**，而是会进到这个 **catch **方法中。
```
`//做饭`
`function``cook(){`
`    ``console.log(``'开始做饭。'``);`
`    ``var``p = ``new``Promise(``function``(resolve, reject){        ``//做一些异步操作`
`        ``setTimeout(``function``(){`
`            ``console.log(``'做饭完毕！'``);`
`            ``resolve(``'鸡蛋炒饭'``);`
`        ``}, 1000);`
`    ``});`
`    ``return``p;`
`}`
`//吃饭`
`function``eat(data){`
`    ``console.log(``'开始吃饭：'``+ data);`
`    ``var``p = ``new``Promise(``function``(resolve, reject){        ``//做一些异步操作`
`        ``setTimeout(``function``(){`
`            ``console.log(``'吃饭完毕!'``);`
`            ``resolve(``'一块碗和一双筷子'``);`
`        ``}, 2000);`
`    ``});`
`    ``return``p;`
`}`
`cook()`
`.then(``function``(data){`
`    ``throw``new``Error(``'米饭被打翻了！'``);`
`    ``eat(data);`
`})`
`.``catch``(``function``(data){`
`    ``console.log(data);`
`});`
```
运行结果如下：
![原文:JS - Promise使用详解2（ES6中的Promise）](http://www.hangge.com/blog_uploads/201703/2017033110082376335.png)
这种错误的捕获是非常有用的，因为它能够帮助我们在开发中识别代码错误。比如，在一个 **then() **方法内部的任意地方，我们做了一个 **JSON.parse() **操作，如果 **JSON **参数不合法那么它就会抛出一个同步错误。用回调的话该错误就会被吞噬掉，但是用 **promises **我们可以轻松的在 **catch() **方法里处理掉该错误。
（3）还可以添加多个 **catch**，实现更加精准的异常捕获。
```
`somePromise.then(``function``() {`
` ``return``a();`
`}).``catch``(TypeError, ``function``(e) {`
` ``//If a is defined, will end up here because`
` ``//it is a type error to reference property of undefined`
`}).``catch``(ReferenceError, ``function``(e) {`
` ``//Will end up here if a wasn't defined at all`
`}).``catch``(``function``(e) {`
` ``//Generic catch-the rest, error wasn't TypeError nor`
` ``//ReferenceError`
`});`
```
### 4，all()方法
**Promise **的 **all **方法提供了并行执行异步操作的能力，并且在所有异步操作执行完后才执行回调。
（1）比如下面代码，两个个异步操作是并行执行的，等到它们都执行完后才会进到 **then **里面。同时 **all **会把所有异步操作的结果放进一个数组中传给 **then**。
```
`//切菜`
`function``cutUp(){`
`    ``console.log(``'开始切菜。'``);`
`    ``var``p = ``new``Promise(``function``(resolve, reject){        ``//做一些异步操作`
`        ``setTimeout(``function``(){`
`            ``console.log(``'切菜完毕！'``);`
`            ``resolve(``'切好的菜'``);`
`        ``}, 1000);`
`    ``});`
`    ``return``p;`
`}`
`//烧水`
`function``boil(){`
`    ``console.log(``'开始烧水。'``);`
`    ``var``p = ``new``Promise(``function``(resolve, reject){        ``//做一些异步操作`
`        ``setTimeout(``function``(){`
`            ``console.log(``'烧水完毕！'``);`
`            ``resolve(``'烧好的水'``);`
`        ``}, 1000);`
`    ``});`
`    ``return``p;`
`}`
`Promise`
`.all([cutUp(), boil()])`
`.then(``function``(results){`
`    ``console.log(``"准备工作完毕："``);`
`    ``console.log(results);`
`});`
```
（2）运行结果如下：
![原文:JS - Promise使用详解2（ES6中的Promise）](http://www.hangge.com/blog_uploads/201703/2017033111235483513.png)
### 5，race()方法
**race **按字面解释，就是赛跑的意思。**race **的用法与 **all **一样，只不过 **all **是等所有异步操作都执行完毕后才执行 **then **回调。而 **race **的话只要有一个异步操作执行完毕，就立刻执行 **then **回调。
注意：其它没有执行完毕的异步操作仍然会继续执行，而不是停止。
（1）这里我们将上面样例的 **all **改成 **race**
```
`Promise`
`.race([cutUp(), boil()])`
`.then(``function``(results){`
`    ``console.log(``"准备工作完毕："``);`
`    ``console.log(results);`
`});`
```
![原文:JS - Promise使用详解2（ES6中的Promise）](http://www.hangge.com/blog_uploads/201703/2017033113564649491.png)
（2）**race **使用场景很多。比如我们可以用 **race **给某个异步请求设置超时时间，并且在超时后执行相应的操作。
```
`//请求某个图片资源`
`function``requestImg(){`
`    ``var``p = ``new``Promise(``function``(resolve, reject){`
`    ``var``img = ``new``Image();`
`    ``img.onload = ``function``(){`
`       ``resolve(img);`
`    ``}`
`    ``img.src = ``'xxxxxx'``;`
`    ``});`
`    ``return``p;`
`}`
`//延时函数，用于给请求计时`
`function``timeout(){`
`    ``var``p = ``new``Promise(``function``(resolve, reject){`
`        ``setTimeout(``function``(){`
`            ``reject(``'图片请求超时'``);`
`        ``}, 5000);`
`    ``});`
`    ``return``p;`
`}`
`Promise`
`.race([requestImg(), timeout()])`
`.then(``function``(results){`
`    ``console.log(results);`
`})`
`.``catch``(``function``(reason){`
`    ``console.log(reason);`
`});`
```
上面代码 **requestImg **函数异步请求一张图片，**timeout **函数是一个延时 **5** 秒的异步操作。我们将它们一起放在 **race **中赛跑。
- 如果** 5** 秒内图片请求成功那么便进入 **then **方法，执行正常的流程。
- 如果 **5** 秒钟图片还未成功返回，那么则进入 **catch**，报“图片请求超时”的信息。
![原文:JS - Promise使用详解2（ES6中的Promise）](http://www.hangge.com/blog_uploads/201703/2017033114082930723.png)
原文出自：[www.hangge.com](http://www.hangge.com/)  转载请保留原文链接：[http://www.hangge.com/blog/cache/detail_1638.html](http://www.hangge.com/blog/cache/detail_1638.html)
