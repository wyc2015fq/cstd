# 初识JavaScript闭包 - weixin_33985507的博客 - CSDN博客
2017年04月02日 12:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
### 前言：
学习JavaScript的时候老是听说**闭包**，感觉他很厉害的样子，所以就来会会他吧！
![5138806-55564e645b2e248a.jpg](https://upload-images.jianshu.io/upload_images/5138806-55564e645b2e248a.jpg)
闭包
### 闭包是什么？
**闭包**是指有权访问另外一个函数作用域里的变量和参数的函数
### 闭包的特点以及优缺点
**特点**
- 函数嵌套函数
- 函数内部可以引用外部的参数和变量
- 参数和变量不会被垃圾回收机制回收
一般函数执行结束后，局部变量会被销毁（垃圾回收机制所回收），内存只会保存全局作用域的变量。
但是闭包就不同了，闭包会使变量继续保存在内存里，当然如果使用不当会增大内存使用量。
```
function aaa() {  
  var a = 1;  
  return function(){
      alert(a++)
    };  
}         
var fun = aaa();  
fun();// 1 执行后 a++，，然后a还在~  
fun();// 2   
fun = null;//a被回收！！
```
**优点**
- 希望一个变量长期驻扎在内存中
- 避免全局变量的污染
- 私有成员的存在
**缺点**
闭包的缺点就是常驻内存，会增大内存使用量，使用不当很容易造成内存泄露。
### 循环中的闭包
一个常见错误是在循环中使用变量，我们看个例子：
```
function count() {
    var arr = [];
    for (var i=1; i<=3; i++) {
        arr.push(function () {
            return i * i;
        });
    }
    return arr;
}
var results = count();
var f1 = results[0];
var f2 = results[1];
var f3 = results[2];
```
你可能认为调用f1()，f2()和f3()结果应该是`1`，`4`，`9`，但实际结果是:
```
f1(); // 16
f2(); // 16
f3(); // 16
```
原因很简单，当你执行返回的函数时，闭包继续保持对变量`i`的访问，此时for循环已经结束并且`i = 4`,所以结果都是16。
为了得到想要的结果，需要在每次循环中创建变量 i 的拷贝。为此我们一般采用`自执行匿名函数`
```
function count() {
    var arr = [];
    for (var i=1; i<=3; i++) {
        arr.push((function (n) {
            return function () {
                return n * n;
            }
        })(i));
    }
    return arr;
}
var results = count();
var f1 = results[0];
var f2 = results[1];
var f3 = results[2];
f1(); // 1
f2(); // 4
f3(); // 9
```
**自执行匿名函数**
创建一个自执行匿名函数
```
(function (x) {
    return x * x;
})(3); // 9
//另外一种写法
(function (x) { return x * x }) (3);
```
### 总结
很多初学者学了闭包后，可能也不懂到底在哪里才有用到，甚至不懂闭包到底有啥用。其实你写的每一个js函数都是闭包，一个js函数的顶层作用域就是window对象，js的执行环境本身就是一个scope（浏览器的window/node的global），我们通常称之为全局作用域。每个函数，不论多深，都可以认为是全局scope的子作用域，可以理解为闭包。
闭包的概念其实是针对作用域而言的，闭包的当前作用域总是能够访问外部作用域中的变量，因为 函数 是 JavaScript 中唯一拥有自身作用域的结构，因此闭包的创建依赖于函数。
> 
文笔有限，才疏学浅，文中若有不对之处，还望告知。
> 
参考文章
[详解js闭包](https://link.jianshu.com?t=https://segmentfault.com/a/1190000000652891#articleHeader1)
[JavaScript 秘密花园](https://link.jianshu.com?t=http://bonsaiden.github.io/JavaScript-Garden/zh/#function.closures)
[廖雪峰JavaScript教程-闭包](https://link.jianshu.com?t=http://www.liaoxuefeng.com/wiki/001434446689867b27157e896e74d51a89c25cc8b43bdb3000/00143449934543461c9d5dfeeb848f5b72bd012e1113d15000)
