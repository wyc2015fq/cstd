# jQuery中 $.fn的用法 - z69183787的专栏 - CSDN博客
2014年01月26日 21:33:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3717
**jquery $.fn$.fx是什么意思**
**$.fn是指jquery的命名空间，加上fn上的方法及属性，会对jquery实例每一个有效。如扩展$.fn.abc(),即$.fn.abc()是对jquery扩展了一个abc方法,那么后面你的每一个jquery实例都可以引用这个方法了.那么你可以这样子：$("#div").abc();通常使用extend方法扩展，详细请看API.**
jQuery为开发插件提拱了两个方法，分别是：
jQuery.fn.extend(object);
jQuery.extend(object);
jQuery.extend(object);为扩展jQuery类本身.为类添加新的方法。
jQuery.fn.extend(object);给jQuery对象添加方法。
fn是什么东西呢。查看jQuery代码，就不难发现。
jQuery.fn = jQuery.prototype ={
　　　init: function( selector, context ){//....　
　　　//......
};
原来 jQuery.fn =jQuery.prototype.对prototype肯定不会陌生啦。
虽然javascript　没有明确的类的概念，但是用类来理解它，会更方便。
jQuery便是一个封装得非常好的类，比如我们用语句　$("#btn1") 会生成一个 jQuery类的实例。
jQuery.extend(object);　为jQuery类添加添加类方法，可以理解为添加静态方法。如：
$.extend({
　　add:function(a,b){returna+b;}
});
便为　jQuery　添加一个为add　的　“静态方法”，之后便可以在引入 jQuery　的地方，使用这个方法了，
$.add(3,4); //return 7
jQuery.fn.extend(object);对jQuery.prototype进得扩展，就是为jQuery类添加“成员函数”。**jQuery类的实例可以使用这个“成员函数”**。
比如我们要开发一个插件，做一个特殊的编辑框，当它被点击时，便alert当前编辑框里的内容。可以这么做：
jQuery代码 
$.fn.extend({        
  alertWhileClick:function(){        
      $(this).click(function(){        
           alert($(this).val());        
       });        
   }        
});       
源自：http://blog.sina.com.cn/s/blog_83958bc601012cl9.html
