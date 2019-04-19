# jquery源码学习笔记三：jQuery工厂剖析 - 左直拳的马桶_日用桶 - CSDN博客
2017年01月15日 23:45:52[左直拳](https://me.csdn.net/leftfist)阅读数：361
[jquery源码学习笔记二：jQuery工厂](http://blog.csdn.net/leftfist/article/details/51668430)
[jquery源码学习笔记一：总体结构](http://blog.csdn.net/leftfist/article/details/51503904)
上两篇说过，query的核心是一个jQuery工厂。其代码如下
```
function( window, noGlobal ) {
    var jQuery = function( selector, context ) {//首先定义一个内部jQuery。注意，此jQuery只是一个工厂内部的变量，并非我们在外面引用的那个jQuery或$
        return new jQuery.fn.init( selector, context );
    };
    /*然后，对这个内部jQuery对象进行各种增强、扩展*/
    jQuery.fn = jQuery.prototype = {
        //...
    };
    jQuery.extend = jQuery.fn.extend = function() {
        //...
    };
    var init = jQuery.fn.init = function( selector, context, root ) {
        //...
    };
    init.prototype = jQuery.fn;
    jQuery.extend({
        //...
    });
    jQuery.fn.extend({
        //...
    });
    if ( !noGlobal ) {//最后，将内部jQuery赋给了外部，即我们所熟知的那个“$”
        window.jQuery = window.$ = jQuery;
    }
    return jQuery;
}
```
其实目前来讲，最难理解的就是这个：
```java
var jQuery = function( selector, context ) {
        return new jQuery.fn.init( selector, context );
    };
```
感觉怎么将孙子赋给了爷爷？究竟是先有鸡还是先有蛋的问题？
其实呢，应该是这么看：
```
//只是先定义了一个函数而已，还没有执行，无所谓先有鸡还是先有蛋
var jQuery = function( selector, context ) {
        ……
    };
```
好吧，第二点疑惑，为啥要new jQuery.fn.init( selector, context );这么个实例赋给jQuery?
是因为，new给jQuery，这样调用jQuery的时候，就不用再new了，简洁一点。
```java
function( window, noGlobal ) {
    var jQuery = function( selector, context ) {
        return new jQuery.fn.init( selector, context );
    };
    ……
    if ( !noGlobal ) {
        window.jQuery = window.$ = jQuery;
    }
    return jQuery;
}
```
由上代码可知，jQuery 已经是一个 new 出来的对象。如此，我们在使用jQuery的时候，就没有new这一步了，直接就是$.**或者$(“**“)这样来使用。
往下看，有：
```
jQuery.fn = jQuery.prototype = {
        //...
    };
```
为啥这样写呢？有什么玄妙？或者纯粹就是为了装逼？还真是，将原型prototype赋给fn，就是一种简写，不用写成”prototype”这么长。
第四个疑惑：
```
jQuery.fn = jQuery.prototype = {
        //...
    };
    jQuery.extend = jQuery.fn.extend = function() {
        //...
    };
    jQuery.extend({
        //...
    });
    jQuery.fn.extend({
        //...
    });
```
jQuery.extend和jQuery.fn.extend有啥不一样？
当然不一样了。jQuery.extend扩展（顾名思义）的是实例，而jQuery.fn.extend扩展的是原型。扩展实例，作用仅限于实例；扩展原型，却可以惠及所有实例。扩展原型，可以节省内存。慢着，jQuery不是只有一个实例吗？
不是的，
```java
var jQuery = function( selector, context ) {
        return new jQuery.fn.init( selector, context );
    };
```
jQuery随同参数不同，是有许多个实例的。比如，$(“div”) 和 $(“#div1”)是两个不同的实例。
疑惑五：
```
var init = jQuery.fn.init = function( selector, context, root ) {
        //...
    };
    init.prototype = jQuery.fn;
```
为什么要把jQuery的原型(jQuery.fn)覆盖它孙子(jQuery.fn.init)的原型？ 
答曰，因为不这么写的话，jQuery将用不上jQuery.fn，一系列的原型扩展：jQuery.fn.extend()都白费了。为啥呢，因为
```java
var jQuery = function( selector, context ) {
        return new jQuery.fn.init( selector, context );
    };
```
意味着当这个函数真正被执行以后，jQuery将被它孙子的实例所替换，所以必须在定义阶段，就要将扩展应用到孙子的原型中。那么，为啥不直接扩展孙子的原型呢，非要扩展爷爷的原型，然后覆盖孙子的原型，再然后构造孙子实例赋给爷爷？真他妈绕啊。搞来搞去，可能就是为了简洁，和显得有点可读性吧？你看，孙子的名字叫做init。
