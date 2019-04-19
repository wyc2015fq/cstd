# Javascript中的apply、call、bind - 左直拳的马桶_日用桶 - CSDN博客
2016年04月12日 20:09:17[左直拳](https://me.csdn.net/leftfist)阅读数：6253
> 
apply 、 call 、bind 三者都是用来改变函数的this对象的指向的； 
   apply 、 call 、bind 三者第一个参数都是this要指向的对象，也就是想指定的上下文；  
   apply 、 call 、bind 三者都可以利用后续参数传参； 
  bind 是返回对应函数，便于稍后调用；apply 、call 则是立即调用 。
以前用过一段时间 node.js，经常看到有 apply，call这类的函数。干啥用的？很高大上的样子。
**一、call 和 apply**
在 javascript 中，call 和 apply 都是为了改变某个函数运行时的上下文（context）而存在的，换句话说，就是为了改变函数体内部 this 的指向。
所谓的this，javascript中，this都存在于函数内部。因此，在javascript里面，有一个总的原则，这个this，指的是调用该函数的对象。
所以，this代表什么这个问题，就转化成究竟是谁在调用该函数的问题。所谓的跟运行时上下文有关。
“*JavaScript 的一大特点是，函数存在「定义时上下文」和「运行时上下文」以及「上下文是可以改变的」这样的概念。*”
所以，
```
某函数.call(某对象，参数1，参数2,……)
=》某函数内部的this被替换成了某对象
```
for example
```java
function fruits() {}
fruits.prototype = {
    color: "red",
    say: function() {
        alert("My color is " + this.color);
    }
};
var apple = new fruits();
apple.say();    //My color is red
var banana = {
    color: "yellow"
};
apple.say.call(banana);     //My color is yellow
apple.say.apply(banana);    //My color is yellow
```
apply类似。与call唯一不同的，是this后面的参数是一个数组
```
func.call(this, arg1, arg2);
func.apply(this, [arg1, arg2])
```
**二、bind**
bind() 方法与 apply 和 call 很相似，也是可以改变函数体内 this 的指向。
但apply与call顾名思义，都是在执行某个函数，而bind，是返回一个函数定义。
*bind()方法会创建一个新函数，称为绑定函数，当调用这个绑定函数时，绑定函数会以创建它时传入 bind()方法的第一个参数作为 this，传入 bind() 方法的第二个以及以后的参数加上绑定函数运行时本身的参数按照顺序作为原函数的参数来调用原函数。*
```java
function fruits() {}
fruits.prototype = {
    color: "red",
    say: function() {
        alert("My color is " + this.color);
    }
};
var apple = new fruits();
apple.say();    //My color is red
var banana = {
    color: "yellow"
};
var bananasay = apple.say.bind(banana);//返回的是一个函数定义
bananasay();//My color is yellow
```
**三、apply、call、bind比较**
apply 、 call 、bind 三者都是用来改变函数的this对象的指向的； 
apply 、 call 、bind 三者第一个参数都是this要指向的对象，也就是想指定的上下文； 
apply 、 call 、bind 三者都可以利用后续参数传参； 
bind 是返回对应函数，便于稍后调用；apply 、call 则是立即调用 。
**四、再说说this**
如果不想this被运行时上下文所替换，有时可以将它用别的变量保存下来。看上去，javascript的变量类型都是值类型。
```java
var foo = {
    bar : 1,
    eventBind: function(){
        var _this = this;//《----- 看到没？？？？ 
        $('.someClass').on('click',function(event) {
            alert(_this.bar);     //1
        });
    }
}
```
用bind来优雅解决：
```java
var foo = {
    bar : 1,
    eventBind: function(){
        $('.someClass').on('click',function(event) {
            alert(this.bar);      //1
        }.bind(this));//《----- 看到没？？？？ 
    }
}
```
参考文章：
[【优雅代码】深入浅出 妙用Javascript中apply、call、bind](http://www.cnblogs.com/coco1s/p/4833199.html)
