# JQuery选择器$()的工作原理浅析 - z69183787的专栏 - CSDN博客
2014年02月25日 10:48:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1278
每次申明一个jQuery对象的时候，返回的是jQuery.prototype.init对象，很多人就会不明白，init明明是jQuery.fn的方法啊，实际上这里不是方法，而是init的构造函数，因为js的prototype对象可以实现继承，加上js的对象只是引用不会是拷贝，new jQuery,new jQuery.fn和new jQuery.fn.init的子对象是一样的，只是有没有执行到init的不同。
当我们使用选择器的时候$(selector,content)，就会执行init(selectot,content)，我们看看inti中是怎样执行的：
```java
if ( typeof selector == "string" )
{
    //正则匹配，看是不是HTML代码或者是#id
    var match = quickExpr.exec( selector );
    //没有作为待查找的 DOM 元素集、文档或 jQuery 对象。
    //selector是#id的形式
    if ( match && (match[1] || !context) )
    {
        // HANDLE: $(html) -> $(array)
        //HTML代码,调用clean补全HTML代码
        if ( match[1] ){
            selector = jQuery.clean( [ match[1] ], context );
        }
        // 是: $("#id")
        else {
            //判断id的Dom是不是加载完成
            var elem = document.getElementById( match[3] );
            if ( elem ){
                if ( elem.id != match[3] )
                return jQuery().find( selector );
                return jQuery( elem );//执行完毕return
            }
            selector = [];
        }
        //非id的形式.在context中或者是全文查找
    }
    else{
        return jQuery( context ).find( selector );
    }
}
```
这里就说明只有选择器写成$('#id')的时候最快，相当于执行了一次getElementById，后边的程序就不用再执行了。当然往往我们需要的选择器并不是这么简单，比如我们需要id下的CSS为className， 有这样的写法$('#id.className')和$('#id').find('.className');这两种写法的执行结果都是一样的，比如<div id=”id”><span class=”className”></span></div>，返回的肯定都是<span class=”className”></span>，但是执行的效率是完全不一样的。
在分析一下上边的代码，如果不是$('#id')这样的简单选择器的话，都会执行find函数，那我们再看看find到底是做用的：
```java
find: function( selector ) {
    //在当前的对象中查找
    var elems = jQuery.map(this, function(elem){
        return jQuery.find( selector, elem );
    });
    //下边的代码可以忽略，只是做一些处理
    //这里应用了js的正则对象的静态方法test
    //indexOf("..")需要了解一下xpath的语法,就是判断selector中包含父节点的写法
    //本意就是过滤数组的重复元素
    return this.pushStack( /[^+>] [^+>]/.test( selector ) || selector.indexOf("..") > -1 ?
        jQuery.unique( elems ) :
        elems );
}
```
如果这样写$('#id .className')，就会执行到扩展的find('#id .className',document)，因为当前的this是document的jQuery数组，那我们在看看扩展的find他的实现，代码比较 多，就不列出来，总之就是从第二个参数传递进行的dom第一个子节点开始找，遇见#比对id，遇见.比对ClassName，还有：<+-等处理。 那我们要优化，是不是就要想办法让第二个参数context的范围最小，那样遍历是不是就很少了？
如果我们这样写$('#id').find('.className')，那程序只这样执行 的，第一次init的时候执行一步getElementById，就return了，接着执行 find('.className',divDocument)，divDocument就是我们第一次选择的是div标签，如果document下有很 多dom对象的时候，这次只遍历divDocument是不是少了很多次，而且在第一次选择id的速度也要比遍历快的多。
现在大家应该是明白了吧。就是说第一层选择最好是ID，而是简单选择器，目的就是定义范围，提高速度。
