# jquery.fn.extend与jquery.extend - z69183787的专栏 - CSDN博客
2013年11月23日 18:49:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：868
个人分类：[Javascript-Jquery插件																[Javascript-Jquery](https://blog.csdn.net/z69183787/article/category/2175111)](https://blog.csdn.net/z69183787/article/category/2175469)
jQuery为开发插件提拱了两个方法，分别是：
JavaScript代码
- jQuery.fn.extend(object);   
- jQuery.extend(object);   
jQuery.extend(object); 为扩展jQuery类本身.为类添加新的方法。
jQuery.fn.extend(object);给jQuery对象添加方法。
fn 是什么东西呢。查看jQuery代码，就不难发现。
 JavaScript代码
- jQuery.fn = jQuery.prototype = {      
- 　　　init: function( selector, context ) {//….　   
- 
- 　　　//……   
- 
- };   
原来 jQuery.fn = jQuery.prototype.对prototype肯定不会陌生啦。
虽然 javascript　没有明确的类的概念，但是用类来理解它，会更方便。
jQuery便是一个封装得非常好的类，比如我们用 语句　$(“#btn1″) 会生成一个 jQuery类的实例。
jQuery.extend(object);　为jQuery类添加添加类方法，可以理解为添加静态方法。如：
 XML/HTML代码
- $.extend({   
- 
- 　　add:function(a,b){return a+b;}   
- 
- });   
便为　jQuery　添加一个为 add　的　“静态方法”，之后便可以在引入 jQuery　的地方，使用这个方法了，
JavaScript代码
- $.add(3,4); //return 7  
jQuery.fn.extend(object); 对jQuery.prototype进得扩展，就是为jQuery类添加“成员函数”。jQuery类的实例可以使用这个“成员函数”。
比如我们要开发一个插件，做一个特殊的编辑框，当它被点击时，便alert 当前编辑框里的内容。可以这么做：
JavaScript代码
- $.fn.extend({        
- 
-      alertWhileClick:function(){        
- 
-          $(this).click(function(){        
- 
-               alert($(this).val());        
-           });        
- 
-       }        
- 
- });        
- 
- $(“#input1″).alertWhileClick(); //页面上为：<input id=”input1″ type=”text”/>    
$(“#input1″)　为一个jQuery实例，当它调用成员方法 alertWhileClick后，便实现了扩展，每次被点击时它会先弹出目前编辑里的内容。
 真实的开发过程中，当然不会做这么小白的插件，事实上jQuery提拱了丰富的操作文档，事件，CSS ,Ajax、效果的方法，结合这些方法，便可以开发出更加 Niubility 的插件。
注意：
在这里还有一个特殊的地方，就是在函数开头的地方有jQuery.extend = jQuery.fn.extend，而在程序的前面已经将jQuery.prototype赋值给jQuery.fn了，所以在后面的调用中会出现 jQuery.extend()和jQuery.fn.extend()的不同调用，这两个方法调用产生的结果也不一样，jQuery.extend() 的调用并不会把方法扩展到对象的实例上，引用它的方法也需要通过jQuery类来实现，如jQuery.init()，而 jQuery.fn.extend()的调用把方法扩展到了对象的prototype上，所以实例化一个jQuery对象的时候，它就具有了这些方法，这
 是很重要的，在jQuery.js中到处体现这一点
