# 关于如何使用websocket 接收并显示图片或者操作二进制 - z69183787的专栏 - CSDN博客
2016年09月13日 11:41:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8348
今天想实现一个在firefox中使用websocket接收二进制图片并显示的功能。百度了半天..一堆人都说用`MozBlobBuilder`，可尼玛我的firefox游览器中就没这个对象！！版本(25.0.1)
回想一年前想实现用websocket实现模仿C++接收二进制并操作的时候，百度google上一堆人还说“js是不能实现二进制操作的”，我就开始火大了。都什么鸟玩意儿？？百度上什么时候出现的那么多坑货来坑人的？ 搞JS的牛人都去哪了？
实在受不了，发篇博客希望后者百度再想实现类似功能的时候不要再那么累了。
贴上代码 供参考：
JS操作websocket接收的二进制，安全性能有保障，已经过一年实践考验：
**[javascript]**[view
 plain](http://blog.csdn.net/kkkkk4400/article/details/16980647#)[copy](http://blog.csdn.net/kkkkk4400/article/details/16980647#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83967/fork)
- ws.onmessage = function(evt) {  
- if(typeof(evt.data)=="string"){  
-         textHandler(JSON.parse(evt.data));  
-     }else{  
- var reader = new FileReader();  
-         reader.onload = function(evt){  
- if(evt.target.readyState == FileReader.DONE){  
- var data = new Uint8Array(evt.target.result);  
-                 handler(data);  
-             }  
-         }  
-         reader.readAsArrayBuffer(evt.data);  
-     }  
- };    
**[html]**[view
 plain](http://blog.csdn.net/kkkkk4400/article/details/16980647#)[copy](http://blog.csdn.net/kkkkk4400/article/details/16980647#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83967/fork)
- function handler(data){  
-     switch(data[0]){  
-     case 1:  
-         getCard(data[1]);  
-         break;  
- 
- ...  
JS操作websocket接收的图片，今天刚写的，也是用filereader实现。
**[html]**[view
 plain](http://blog.csdn.net/kkkkk4400/article/details/16980647#)[copy](http://blog.csdn.net/kkkkk4400/article/details/16980647#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/83967/fork)
- ws.onmessage = function(evt) {  
-     if(typeof(evt.data)=="string"){  
-         //textHandler(JSON.parse(evt.data));  
-     }else{  
-   var reader = new FileReader();  
- reader.onload = function(evt){  
-             if(evt.target.readyState == FileReader.DONE){  
-                 var url = evt.target.result;  
-     alert(url);  
-                 var img = document.getElementById("imgDiv");  
- img.innerHTML = "<img src = "+url+" />";  
-             }  
-         }  
-         reader.readAsDataURL(evt.data);  
-     }  
- };    
![](http://static.blog.csdn.net/images/save_snippets.png)
最后想说.看了那么多JS达人的博客，那么多转摘的文章，请问转摘之前自己实践过么？知道自己转的文章是正确的么？？
我平时搞ERLANG 搞C 搞JAVA，偶尔才写一点JS小代码还被坑成这样，真心难受。
