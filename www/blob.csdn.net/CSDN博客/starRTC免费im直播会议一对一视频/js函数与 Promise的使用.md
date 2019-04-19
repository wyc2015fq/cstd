# js函数与 Promise的使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年02月02日 09:40:25[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：250
JavaScript的函数不但是“头等公民”，而且可以像变量一样使用，具有非常强大的抽象能力。
定义函数的方式如下：
**function**abs(x) {
**if** (x >=
0) {
**return** x;
 } **else** {
**return** -x;
 }}
如果没有return语句，函数执行完毕后也会返回结果，只是结果为undefined。
由于JavaScript的函数也是一个对象，上述定义的abs()函数实际上是一个函数对象，而函数名abs可以视为指向该函数的变量。
因此，第二种定义函数的方式如下：
**var**
 abs = **function**
 (x) { **if**
 (x >= 0) {
**return** x;
 } **else** {
**return** -x;
 }};
在这种方式下，function
 (x) { ... }是一个匿名函数，它没有函数名。但是，这个匿名函数赋值给了变量abs，所以，通过变量abs就可以调用该函数。
述两种定义完全等价，注意第二种方式按照完整语法需要在函数体末尾加一个;，表示赋值语句结束。
abs();
// 返回NaN
此时abs(x)函数的参数x将收到undefined，计算结果为NaN。
要避免收到undefined，可以对参数进行检查：
**function**abs(x) {
**if** (**typeof**
 x !== 'number') {**throw**'Not a number'; }
**if** (x >=
0) {
**return** x;
 } **else** {
**return** -x;
 }}
请严格遵守“在函数内部首先申明所有变量”这一规则。最常见的做法是用一个var申明函数内部用到的所有变量：
**function**foo() {
**var** x =
1,
*// x初始化为1* y = x
 + 1,
*// y初始化为2* z, i;
*// z和i为undefined**// 其他语句:***for** (i=0;
 i<100; i++) { ... }}
全局作用域
不在任何函数内定义的变量就具有全局作用域。实际上，JavaScript默认有一个全局对象window，全局作用域的变量实际上被绑定到window的一个属性：
**var**
 course = 'Learn JavaScript';alert(course);
*// 'Learn JavaScript'*alert(window.course);
*// 'Learn JavaScript'*
以变量方式var
 foo = function () {}定义的函数实际上也是一个全局变量，因此，顶层函数的定义也被视为一个全局变量，并绑定到window对象：
**function**foo() { alert('foo');}foo();
*// 直接调用foo()*window.foo();
*// 通过window.foo()调用*
不同的JavaScript文件如果使用了相同的全局变量，或者定义了相同名字的顶层函数，都会造成命名冲突，并且很难被发现。
减少冲突的一个方法是把自己的所有变量和函数全部绑定到一个全局变量中。例如：
*// 唯一的全局变量MYAPP:***var**
 MYAPP = {};*// 其他变量:*MYAPP.name
 = 'myapp';MYAPP.version =
1.0;*// 其他函数:*MYAPP.foo =
**function** ()
 { **return**'foo';};
局部作用域
由于JavaScript的变量作用域实际上是函数内部，我们在for循环等语句块中是无法定义具有局部作用域的变量的：
'use strict';**function**foo() {
**for** (**var**
 i=0; i<100;
 i++) { *//* } i
 += 100;
*// 仍然可以引用变量i*}
为了解决块级作用域，ES6引入了新的关键字let，用let替代var可以申明一个块级作用域的变量：
'use strict';**function**foo() {
**var** sum =
0;
**for** (**let**
 i=0; i<100;
 i++) { sum += i; } *// SyntaxError:*
 i += 1;}
关键字const来定义常量
解构赋值，可以同时对一组变量进行赋值。
var [x, y, z] = ['hello', 'JavaScript', 'ES6'];
注意，对数组元素进行解构赋值时，多个变量要用[...]括起来。
如果数组本身还有嵌套，也可以通过下面的形式进行解构赋值，注意嵌套层次和位置要保持一致：
**let**
 [x, [y, z]] = ['hello', ['JavaScript',
'ES6']];x;
*// 'hello'*y;
*// 'JavaScript'*z;
*// 'ES6'*
如果需要从一个对象中取出若干属性，也可以使用解构赋值，便于快速获取对象的指定属性：
'use strict';var person = { name: '小明', age: 20, gender: 'male', passport: 'G-12345678', school: 'No.4
 middle school'};var {name, age, passport} = person;
同样可以直接对嵌套的对象属性进行赋值，只要保证对应的层次是一致的：
**var**
 person = { name: '小明', age:
20, gender:
'male', passport:
'G-12345678', school:
'No.4 middle school', address:
 { city: 'Beijing', street:
'No.1 Road', zipcode:
'100001' }};**var**
 {name, address: {city, zip}} = person;name; *// '小明'*city;
*// 'Beijing'*zip;
*//**undefined**, 因为属性名是zipcode而不是zip**// 注意: address不是变量**，而是为了让city和zip获得嵌套的address对象的属性:*address;
*// Uncaught ReferenceError: address is not defined*
解构赋值还可以使用默认值，这样就避免了不存在的属性返回undefined的问题：
**var**
 person = { name: '小明', age:
20, gender:
'male', passport:
'G-12345678'};*// 如果person对象没有single属性，默认赋值为true:***var**
 {name, single=true} = person;name; *// '小明'*single;
*// true*

有些时候，如果变量已经被声明了，再次赋值的时候，正确的写法也会报语法错误：
*// 声明变量:***var**
 x, y;*// 解构赋值:*{x,
 y} = { name: '小明', x:
100, y:
200};*// 语法错误: Uncaught SyntaxError: Unexpected token =*
这是因为JavaScript引擎把{开头的语句当作了块处理，于是=不再合法。解决方法是用小括号括起来：
({x, y} = { name: '小明', x: 100, y: 200});
在一个对象中绑定函数，称为这个对象的方法。
**var**
 xiaoming = { name: '小明',
 birth: 1990, age:
**function** ()
 { **var** y =
**new** Date().getFullYear();
**return** y -
**this**.birth;
 }};xiaoming.age; *// function xiaoming.age()*xiaoming.age();
*// 今年调用是25,明年调用就变成26了*
让我们拆开写：
**function**getAge() {
**var** y =
**new** Date().getFullYear();
**return** y -
**this**.birth;}**var**
 xiaoming = { name: '小明',
 birth: 1990, age: getAge};xiaoming.age();
*// 25, 正常结果*getAge();
*// NaN*
单独调用函数getAge()怎么返回了NaN？请注意，我们已经进入到了JavaScript的一个大坑里。
JavaScript的函数内部如果调用了this，那么这个this到底指向谁？
答案是，视情况而定！
如果以对象的方法形式调用，比如xiaoming.age()，该函数的this指向被调用的对象，也就是xiaoming，这是符合我们预期的。
如果单独调用函数，比如getAge()，此时，该函数的this指向全局对象，也就是window。
“创建一个匿名函数并立刻执行”的语法：
(**function**
 (x) { **return**
 x * x;})(3);
*// 9*
需要用括号把整个函数定义括起来
箭头函数
x => x * x
上面的箭头函数相当于：
**function**
 (x) { **return**
 x * x;}
相当于匿名函数，并且简化了函数定义。箭头函数有两种格式，一种像上面的，只包含一个表达式，连{
 ... }和return都省略掉了。还有一种可以包含多条语句，这时候就不能省略{
 ... }和return：
x => {
**if** (x >
0) {
**return** x *
 x; } **else**
 { **return**
 - x * x; }}
如果参数不是一个，就需要用括号()括起来：
*// 两个参数:*(x,
 y) => x * x + y * y
*// 无参数:*()
 => 3.14
如果要返回一个对象，需要加小括号
x => ({ foo: x })
Promise
异步执行可以用回调函数实现
AJAX就是典型的异步操作。
看一个最简单的Promise例子：生成一个0-2之间的随机数，如果小于1，则等待一段时间后返回成功，否则返回失败：
**function**test(resolve, reject) {
**var** timeOut
 = Math.random() * 2; log('set
 timeout to: ' + timeOut +
' seconds.'); setTimeout(**function**
 () { **if** (timeOut
 < 1) { log('call
 resolve()...'); resolve('200
 OK'); }
**else** { log('call
 reject()...'); reject('timeout
 in ' + timeOut +
' seconds.'); } }, timeOut
 * 1000);}
这个test()函数有两个参数，这两个参数都是函数，如果执行成功，我们将调用resolve('200
 OK')，如果执行失败，我们将调用reject('timeout in ' + timeOut
 + ' seconds.')。可以看出，test()函数只关心自身的逻辑，并不关心具体的resolve和reject将如何处理结果。
**var**
 p1 = **new**
 Promise(test);**var**
 p2 = p1.then(**function**
 (result) { console.log('成功：'
 + result);});**var**
 p3 = p2.**catch**(**function**
 (reason) { console.log('失败：'
 + reason);});
变量p1是一个Promise对象，它负责执行test函数。由于test函数在内部是异步执行的，当test函数执行成功时，我们告诉Promise对象：
*// 如果成功，执行这个函数：*p1.then(**function**
 (result) { console.log('成功：'
 + result);});
当test函数执行失败时，我们告诉Promise对象：
p2.**catch**(**function**
 (reason) { console.log('失败：'
 + reason);});
Promise对象可以串联起来，所以上述代码可以简化为：
**new**
 Promise(test).then(**function**
 (result) { console.log('成功：'
 + result);}).**catch**(**function**
 (reason) { console.log('失败：'
 + reason);});
Promise还可以做更多的事情，比如，有若干个异步任务，需要先做任务1，如果成功后再做任务2，任何任务失败则不再继续并执行错误处理函数。
job1.**then**(job2).**then**(job3).catch(handleError);
其中，job1、job2和job3都是Promise对象。
// 0.5秒后返回input*input的计算结果:
function multiply(input) {
return new Promise(function (resolve, reject) {
log('calculating ' + input + ' x ' + input + '...');
setTimeout(resolve, 500, input * input);
});
}
// 0.5秒后返回input+input的计算结果:
function add(input) {
return new Promise(function (resolve, reject) {
log('calculating ' + input + ' + ' + input + '...');
setTimeout(resolve, 500, input + input);
});
}
var p = new Promise(function (resolve, reject) {
log('start new Promise...');
resolve(123);
});
p.then(multiply)
.then(add)
.then(multiply)
.then(add)
.then(function (result) {
log('Got value: ' + result);
});
把上一节的AJAX异步执行函数转换为Promise对象，看看用Promise如何简化异步处理：
'use strict';// ajax函数将返回Promise对象:function ajax(method, url, data) { var request = new XMLHttpRequest();
 return new Promise(function (resolve, reject) { request.onreadystatechange = function () { if (request.readyState === 4) { if (request.status === 200) { resolve(request.responseText); } else { reject(request.status); } } }; request.open(method, url); request.send(data);
 });}
var log = document.getElementById('test-promise-ajax-result');
var p = ajax('GET', '/api/categories');
p.then(function (text) { // 如果AJAX成功，获得响应内容
log.innerText = text;
}).catch(function (status) { // 如果AJAX失败，获得响应代码
log.innerText = 'ERROR: ' + status;
});
[http://es6.ruanyifeng.com/#docs/promise](http://es6.ruanyifeng.com/#docs/promise)
Promise，简单说就是一个容器，里面保存着某个未来才会结束的事件（通常是一个异步操作）的结果。
下面代码创造了一个Promise实例。
const promise = new Promise(function(resolve, reject) { // ... some code if (/* 异步操作成功 */){ resolve(value); } else { reject(error); }});
Promise构造函数接受一个函数作为参数，该函数的两个参数分别是resolve和reject。它们是两个函数，由
 JavaScript 引擎提供，不用自己部署。
resolve函数的作用:在异步操作成功时调用，并将异步操作的结果，作为参数传递出去
reject函数的作用:在异步操作失败时调用，并将异步操作报出的错误，作为参数传递出去。
promise.then(function(value) { // success}, function(error) { // failure});
then方法可以接受两个回调函数作为参数。
其中，第二个函数是可选的，不一定要提供。
这两个函数都接受Promise对象传出的值作为参数。
function timeout(ms) { return new Promise((resolve, reject) => { setTimeout(resolve, ms, 'done'); });}timeout(100).then((value) => { console.log(value);});
一般来说，最好在它们前面加上return语句，这样就不会有意外。
return
 resolve(1);
可以采用链式写法，即then方法后面再调用另一个then方法。
getJSON("/posts.json").then(function(json){return json.post;}).then(function(post){// ...});
上面的代码使用then方法，依次指定了两个回调函数。第一个回调函数完成以后，会将返回结果作为参数，传入第二个回调函数。
getJSON("/post/1.json").then(function(post){return getJSON(post.commentURL);}).then(function
 funcA(comments){ console.log("resolved:
 ", comments);},function funcB(err){
 console.log("rejected:
 ", err);});
上面代码中，第一个then方法指定的回调函数，返回的是另一个Promise对象。这时，第二个then方法指定的回调函数，就会等待这个新的Promise对象状态发生变化。如果变为resolved，就调用funcA，如果状态变为rejected，就调用funcB。
如果采用箭头函数，上面的代码可以写得更简洁。
getJSON("/post/1.json").then(
 post => getJSON(post.commentURL)).then(
 comments => console.log("resolved:
 ", comments),
 err => console.log("rejected:
 ", err));
有时需要将现有对象转为 Promise 对象，Promise.resolve方法就起到这个作用。
