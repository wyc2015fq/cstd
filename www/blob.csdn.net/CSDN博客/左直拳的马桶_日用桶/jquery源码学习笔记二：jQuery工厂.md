# jquery源码学习笔记二：jQuery工厂 - 左直拳的马桶_日用桶 - CSDN博客
2016年06月14日 07:34:21[左直拳](https://me.csdn.net/leftfist)阅读数：4698
[笔记一](http://blog.csdn.net/leftfist/article/details/51503904)里记录，jQuery的总体结构如下：
```java
(function( global, factory ) {
    //调用factory(工厂)生成jQuery实例
    factory( global );
}(typeof window !== "undefined" ? window : this, function( window, noGlobal ) {
    //factory实现，jquery源码主体部分
}));
```
那么这个生成jQuery的工厂是咋样的？
酱紫的：
```java
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
从工厂的代码来看，$ 本质上是一个函数，因此，我们才可以这样引用：
`$(".class")`
