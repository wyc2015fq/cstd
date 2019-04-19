# JQuery when(deferreds)方法 的使用 - 零度的博客专栏 - CSDN博客
2016年04月01日 10:43:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：3467
参数deferreds，一个或多个延时对象或JS对象，我们初略的认为它就是一个或多个异步请求。
关于deferred对象 请见：[http://blog.csdn.net/z69183787/article/details/24025505](http://blog.csdn.net/z69183787/article/details/24025505)
例如：
**[javascript]**[view
 plain](http://blog.csdn.net/starrexstar/article/details/7940972#)[copy](http://blog.csdn.net/starrexstar/article/details/7940972#)
- $.when($.ajax("page1.php"), $.ajax("page2.php"))  
when()函数常常和done()函数、fail()函数、then()函数联合使用：
**done(Function func)** - 当deferreds中的处理都完成的时候执行Function回调函数
**fail(Function func)** - 当deferreds中有一个处理失败的时候执行Function回调函数
**then(Function func1,Function func2)**- 结合了done和fail函数，当都成功执行func1，当有一个失败执行func2
Example：
**[javascript]**[view
 plain](http://blog.csdn.net/starrexstar/article/details/7940972#)[copy](http://blog.csdn.net/starrexstar/article/details/7940972#)
- var whenResult = $.when($.ajax("page1.php"), $.ajax("page2.php"));  
- whenResult.done(function(a1,a2){  
- //函数内容略
- //a1和a2俩参数是when函数中两个ajax请求的相关jqXHR对象
- });  
- whenResult.fail(function(){  
- //函数内容略
- })  
- whenResult.then(successFunc,failureFunc);  
