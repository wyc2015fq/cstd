# jquery插件开发及 jquery自定义函数 - z69183787的专栏 - CSDN博客
2013年12月04日 11:29:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3463
jQuery插件的开发包括两种：
一种是类级别的插件开发，即给jQuery添加新的全局函数，相当于给jQuery类本身添加方法。jQuery的全局函数就是属于jQuery命名空间的函数，另一种是对象级别的插件开发，即给jQuery对象添加方法。下面就两种函数的开发做详细的说明。
**1、类级别的插件开发**
类级别的插件开发最直接的理解就是给jQuery类添加类方法，可以理解为添加静态方法。典型的例子就是$.AJAX()这个函数，将函数定义于jQuery的命名空间中。关于类级别的插件开发可以采用如下几种形式进行扩展：
**1.1 添加一个新的全局函数**
添加一个全局函数，我们只需如下定义：

Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- jQuery.foo = function() {   
- alert('This is a test. This is only a test.');  
- };    
**1.2 增加多个全局函数**
添加多个全局函数，可采用如下定义：
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- jQuery.foo = function() {   
- alert('This is a test. This is only a test.');  
- };  
- jQuery.bar = function(param) {   
- alert('This function takes a parameter, which is "' + param + '".');  
- };   
- 调用时和一个函数的一样的:jQuery.foo();jQuery.bar();或者$.foo();$.bar('bar');  
**1.3 使用jQuery.extend(object);**
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- jQuery.extend({      
- foo: function() {      
- alert('This is a test. This is only a test.');      
- },      
- bar: function(param) {      
- alert('This function takes a parameter, which is "' + param +'".');      
- }     
- });  
**1.4 使用命名空间**
虽然在jQuery命名空间中，我们禁止使用了大量的javaScript函数名和变量名。但是仍然不可避免某些函数或变量名将于其他jQuery插件冲突，因此我们习惯将一些方法封装到另一个自定义的命名空间。
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- jQuery.myPlugin = {          
- foo:function() {          
- alert('This is a test. This is only a test.');          
- },          
- bar:function(param) {          
- alert('This function takes a parameter, which is "' + param + '".');    
- }         
- };  
- 采用命名空间的函数仍然是全局函数，调用时采用的方法：  
- $.myPlugin.foo();         
- $.myPlugin.bar('baz');  
通过这个技巧（使用独立的插件名），我们可以避免命名空间内函数的冲突。
**2、对象级别的插件开发**
对象级别的插件开发需要如下的两种形式：、
形式1：  
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- (function($){     
- $.fn.extend({     
- pluginName:function(opt,callback){     
- // Our plugin implementation code goes here.     
- }     
- })     
- })(jQuery);     
形式2：

Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- (function($) {       
- $.fn.pluginName = function() {     
- // Our plugin implementation code goes here.   
- };     
- })(jQuery);      
       上面定义了一个jQuery函数,形参是$，函数定义完成之后,把jQuery这个实参传递进去.立即调用执行。这样的好处是,我们在写jQuery插件时,也可以使用$这个别名,而不会与prototype引起冲突.
**2.1 在JQuery名称空间下申明一个名字**
这是一个单一插件的脚本。如果你的脚本中包含多个插件，或者互逆的插件（例如： $.fn.doSomething() 和$.fn.undoSomething()），那么你需要声明多个函数名字。但是，通常当我们编写一个插件时，力求仅使用一个名字来包含它的所有内容。我们的示例插件命名为“highlight“
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- $.fn.hilight = function() {    
- // Our plugin implementation code goes here.  
- };    
- 我们的插件通过这样被调用：  
- $('#myDiv').hilight();     
但是如果我们需要分解我们的实现代码为多个函数该怎么办？有很多原因：设计上的需要；这样做更容易或更易读的实现；而且这样更符合面向对象。这真是一个麻烦事，把功能实现分解成多个函数而不增加多余的命名空间。出于认识到和利用函数是javascript中最基本的类对象，我们可以这样做。就像其他对象一样，函数可以被指定为属性。因此我们已经声明“hilight”为jQuery的属性对象，任何其他的属性或者函数我们需要暴露出来的，都可以在"hilight" 函数中被声明属性。稍后继续。
**2.2 ****接受options参数以控制插件的行为**
让我们为我们的插件添加功能指定前景色和背景色的功能。我们也许会让选项像一个options对象传递给插件函数。例如：   
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- // plugin definition  
- $.fn.hilight = function(options) {    
-   var defaults = {    
-     foreground: 'red',    
-     background: 'yellow'
-   };    
- // Extend our default options with those provided.  
-   var opts = $.extend(defaults, options);    
- // Our plugin implementation code goes here.  
- };    
- 我们的插件可以这样被调用：  
- $('#myDiv').hilight({    
-   foreground: 'blue'
- }); 
