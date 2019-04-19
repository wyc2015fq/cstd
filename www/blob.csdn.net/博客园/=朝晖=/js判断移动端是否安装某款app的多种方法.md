# js判断移动端是否安装某款app的多种方法 - =朝晖= - 博客园
# [js判断移动端是否安装某款app的多种方法](https://www.cnblogs.com/dhcn/p/7124686.html)
本文实例讲解了[js](http://lib.csdn.net/base/javascript)判断移动端是否安装某款app的多种方法，分享给大家供大家参考，具体内容如下
**第一种方法：**
**一:判断是那种设备**
- var isAndroid = u.indexOf('Android') > -1 || u.indexOf('Linux') > -1; //android终端或者uc浏览器  
- var isiOS = !!u.match(/\(i[^;]+;( U;)? CPU.+Mac OS X/); //ios终端  
**二：[安卓](http://lib.csdn.net/base/android)设备：**原理：判断是否认识这个协议，认识则直接跳转，不认识就在这里下载app
- android();  
- 
- if(isAndroid){  
- function android(){  
-         window.location.href = "openwjtr://com.tyrbl.wjtr"; /***打开app的协议，有安卓同事提供***/  
-         window.setTimeout(function(){  
-            window.location.href = "http://www.wjtr.com/download/index.html"; /***打开app的协议，有安卓同事提供***/  
-         },2000);  
-       };  
**二：[iOS](http://lib.csdn.net/base/ios)设备：**原理：判断是否认识这个协议，认识则直接跳转，不认识就在这里下载appios();
- if(isiOS){  
- function ios(){  
- var ifr = document.createElement("iframe");  
-         ifr.src = "openwjtr://com.tyrbl.wjtr"; /***打开app的协议，有ios同事提供***/  
-         ifr.style.display = "none";   
-         document.body.appendChild(ifr);  
-         window.setTimeout(function(){  
-           document.body.removeChild(ifr);  
-            window.location.href = "http://www.wjtr.com/download/index.html"; /***下载app的地址***/  
-         },2000)  
-       };  
- }  
**第二种方法：**
虽然在Js中可以启动某个app，但是并不能判断该app是否安装；
启动app需要的时间较长，js中断时间长，如果没安装，js瞬间就执行完毕。直接上代码吧！
html代码：
- <a href="javascript:testApp('tel:1868888888')">打电话</a>   
js代码：
- function testApp(url) {   
- var timeout, t = 1000, hasApp = true;   
-   setTimeout(function () {   
- if (hasApp) {   
-       alert('安装了app');   
-     } else {   
-       alert('未安装app');   
-     }   
-     document.body.removeChild(ifr);   
-   }, 2000)   
- 
- var t1 = Date.now();   
- var ifr = document.createElement("iframe");   
-   ifr.setAttribute('src', url);   
-   ifr.setAttribute('style', 'display:none');   
-   document.body.appendChild(ifr);   
-   timeout = setTimeout(function () {   
- var t2 = Date.now();   
- if (!t1 || t2 - t1 < t + 100) {   
-        hasApp = false;   
-      }   
-   }, t);   
- }   
**第三种方法：**
最近在做项目的wap版，有个需求就是，先判断手机上是否有我们的APP应用，如果有的话打开应用，没有才跳转到wap页面。 
wap简单来说就是运行在移动端浏览器上的网站。不管应用在什么地方，总之就是浏览器呗，可以通过JS来判断本地是否有某应用，实现方式实际就是将http协议转为本地软件协议。 
还是直接贴代码吧。
 如下： 
- <script language="javascript">  
- if (navigator.userAgent.match(/(iPhone|iPod|iPad);?/i)) {  
- var loadDateTime = new Date();  
-   window.setTimeout(function() {  
- var timeOutDateTime = new Date();  
- if (timeOutDateTime - loadDateTime < 5000) {  
-     window.location = "要跳转的页面URL";  
-    } else {  
-     window.close();  
-    }  
-   },  
-   25);  
-   window.location = " apps custom url schemes ";  
-  } else if (navigator.userAgent.match(/android/i)) {  
- var state = null;  
- try {  
-    state = window.open("apps custom url schemes ", '_blank');  
-   } catch(e) {}  
- if (state) {  
-    window.close();  
-   } else {  
-    window.location = "要跳转的页面URL";  
-   }  
-  }  
- </script>  
**apps custom url schemes 是什么呢？**其实就是你与APP约定的一个协议URL，你的[ios](http://lib.csdn.net/base/ios)同事或[Android](http://lib.csdn.net/base/android)同事在写程序的时候会设置一个URL Scheme，
例如设置：
URL Scheme ：app
然后其他的程序就可以通过URLString ＝ app://  调用该应用。
还可以传参数，如：
**app://reaction/?uid=1****原理：**500ms内，本机有应用程序能解析这个协议并打开程序，调用该应用；如果本机没有应用程序能解析该协议或者500ms内没有打开这个程序，则执行setTimeout里面的function，就是跳转到你想跳转的页面。
以上就是js判断移动端是否安装某款app的多种方法，希望对大家的学习有所帮助。

