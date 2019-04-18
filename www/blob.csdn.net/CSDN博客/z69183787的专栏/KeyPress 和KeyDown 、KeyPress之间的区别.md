# KeyPress 和KeyDown 、KeyPress之间的区别 - z69183787的专栏 - CSDN博客
2014年05月13日 12:10:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：33326
虽然从字面理解, KeyDown是按下一个键的意思, 但实际上二者的根本区别是, 系统由KeyDown返回键盘的代码, 然后由TranslateMessage函数翻译成成字符, 由KeyPress返回字符值. 因此在KeyDown中返回的是键盘的代码, 而KeyPress返回的是ASCII字符. 所以根据你的目的, 如果只想读取字符, 用KeyPress, 如果想读各键的状态, 用KeyDown. 
## keydown：用户在键盘上按下某按键是发生。一直按着某按键则会不断触发（opera浏览器除外）。keypress：用户按下一个按键，**并产生一个字符时**发生（也就是不管类似shift、alt、ctrl之类的键，就是说用户按了一个能在屏幕上输出字符的按键keypress事件才会触发）。一直按着某按键则会不断触发。keyup：用户释放某一个按键是触发。
## 定义和用法
完整的 key press 过程分为两个部分：1. 按键被按下；2. 按键被松开。
当按钮被按下时，发生 keydown 事件。
keydown() 方法触发 keydown 事件，或规定当发生 keydown 事件时运行的函数。
**[html]**[view
 plain](http://blog.csdn.net/fox123871/article/details/8087975#)[copy](http://blog.csdn.net/fox123871/article/details/8087975#)
- <html>
- <head>
- <scripttype="text/javascript"src="/jquery/jquery.js"></script>
- <scripttype="text/javascript">
- $(document).ready(function(){  
-   $("input").keydown(function(){  
-     $("input").css("background-color","#FFFFCC");  
-   });  
-   $("input").keyup(function(){  
-     $("input").css("background-color","#D6D6FF");  
-   });  
- });  
- </script>
- </head>
- <body>
- Enter your name: <inputtype="text"/>
- <p>当发生 keydown 和 keyup 事件时，输入域会改变颜色。请试着在其中输入内容。</p>
- </body>
- </html>
中所周知，jquery封装了很多事件交互方法，这里讲到的问题在原生js中也是有的。
keyup是在用户将按键抬起的时候才会触发的，属于整个按键过程中的最后阶段，所以有其特定的用处，就是在左侧输入，右侧同步显示的过程中很有用处。典型的例子就是邮件编辑预览的应用。
**[html]**[view
 plain](http://blog.csdn.net/fox123871/article/details/8087975#)[copy](http://blog.csdn.net/fox123871/article/details/8087975#)
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <title>无标题页</title>
- 
- <scriptsrc="JS/jquery-1.4.2.js"type="text/javascript"></script>
- 
- <scripttype="text/javascript">
-         $(function() {  
-             $('#t1').live('keyup', function() {  
-                 $('#v1').text($(this).val());  
-             });  
-             $('#t2').live('keydown', function() {  
-                 $('#v2').text($(this).val());  
-             });  
-             $('#t3').live('keypress', function() {  
-                 $('#v3').text($(this).val());  
-             });  
-         });      
- </script>
- 
- </head>
- <body>
- <textareaid="t1"></textarea>
- <divid="v1">
- </div>
- <textareaid="t2"></textarea>
- <divid="v2">
- </div>
- <textareaid="t3"></textarea>
- <divid="v3">
- </div>
- </body>
- </html>
这里分别应用了三种事件，其中t1能够完整的同步到v1中，而keypress和keydown总是少最后一个字符，这样就说明了这三种事件触发的小小区别，keydown是在按下时触发，不能得到最后的输入结果，keypress也是一样。
例如：keydown绑定文本框，每次点击触发事件，在获取文本框的值，总是打印上次操作时文本框的内容，
这是因为keydown操作后，事件触发了，但值还未显示在文本框中，所以这类操作要用keyup 一个完整的按键动作后，才可以获取文本框的值。
 keydown与keypress更适用于通过键盘控制页面类功能的实现。
获取键盘点击的键位：
**[html]**[view
 plain](http://blog.csdn.net/fox123871/article/details/8087975#)[copy](http://blog.csdn.net/fox123871/article/details/8087975#)
- <html>
- <head>
- <scripttype="text/javascript"src="/jquery/jquery.js"></script>
- <scripttype="text/javascript">
- $(document).ready(function(){  
-   $("input").keydown(function(event){   
-     $("div").html("Key: " + event.which);  
-   });  
- });  
- </script>
- </head>
- <body>
- 
- 请随意键入一些字符：<inputtype="text"/>
- <p>当您在上面的框中键入文本时，下面的 div 会显示键位序号。</p>
- <div/>
- 
- </body>
- </html>
- 
``
