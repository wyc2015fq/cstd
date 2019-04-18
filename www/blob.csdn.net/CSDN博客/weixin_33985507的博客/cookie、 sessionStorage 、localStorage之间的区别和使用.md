# cookie、 sessionStorage 、localStorage之间的区别和使用 - weixin_33985507的博客 - CSDN博客
2016年06月27日 10:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
1.cookie:存储在用户本地终端上的数据。有时也用cookies，指某些网站为了辨别用户身份，进行session跟踪而存储在本地终端上的数据，通常经过加密。一般应用最典型的案列就是判断注册用户是否已经登过该网站。
2.HTML5 提供了两种在客户端存储数据的新方法：(http://www.w3school.com.cn/html5/html_5_webstorage.asp)...两者都是仅在客户端（即浏览器）中保存，不参与和服务器的通信；
- localStorage - 没有时间限制的数据存储,第二天、第二周或下一年之后，数据依然可用。
- 如何创建和访问 localStorage：
- 
```
<script type="text/javascript">
localStorage.lastname="Smith";
document.write(localStorage.lastname);
</script>
```
下面的例子对用户访问页面的次数进行计数：
- 
```
<script type="text/javascript">
if (localStorage.pagecount){
  localStorage.pagecount=Number(localStorage.pagecount) +1;
  }
else{
  localStorage.pagecount=1;
  }
document.write("Visits "+ localStorage.pagecount + " time(s).");
</script>
```
- sessionStorage - 针对一个 session 的数据存储,当用户关闭浏览器窗口后，数据会被删除。
- 创建并访问一个 sessionStorage：
- 
```
<script type="text/javascript">
  sessionStorage.lastname="Smith";
  document.write(sessionStorage.lastname);
</script>
```
下面的例子对用户在当前 session 中访问页面的次数进行计数：
- 
```
<script type="text/javascript">
if (sessionStorage.pagecount){
  sessionStorage.pagecount=Number(sessionStorage.pagecount) +1;
  }
else{
  sessionStorage.pagecount=1;
  }
document.write("Visits "+sessionStorage.pagecount+" time(s) this session.");
</script>
```
- 
**sessionStorage 、localStorage 和 cookie 之间的区别**
共同点：都是保存在浏览器端，且同源的。
- 
区别：cookie数据始终在同源的http请求中携带（即使不需要），即cookie在浏览器和服务器间来回传递；cookie数据还有路径（path）的概念，可以限制cookie只属于某个路径下。存储大小限制也不同，cookie数据不能超过4k，同时因为每次http请求都会携带cookie，所以cookie只适合保存很小的数据，如会话标识。
- 
而sessionStorage和localStorage不会自动把数据发给服务器，仅在本地保存。sessionStorage和localStorage 虽然也有存储大小的限制，但比cookie大得多，可以达到5M或更大。
- 
数据有效期不同，sessionStorage：仅在当前浏览器窗口关闭前有效，自然也就不可能持久保持；localStorage：始终有效，窗口或浏览器关闭也一直保存，因此用作持久数据；cookie只在设置的cookie过期时间之前一直有效，即使窗口或浏览器关闭。
- 
作用域不同，sessionStorage**不在**不同的浏览器窗口中共享，即使是同一个页面；localStorage 在所有同源窗口中都是共享的；cookie也是在所有同源窗口中都是共享的。Web Storage 支持事件通知机制，可以将数据更新的通知发送给监听者。Web Storage 的 api 接口使用更方便。
附上自认为还可以的资料：（https://segmentfault.com/a/1190000002723469），不太懂的时候也可以再看看；
