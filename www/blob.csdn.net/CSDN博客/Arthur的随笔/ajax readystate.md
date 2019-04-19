# ajax readystate - Arthur的随笔 - CSDN博客
2011年11月01日 15:49:14[largetalk](https://me.csdn.net/largetalk)阅读数：1568标签：[ajax																[processing																[function																[javascript																[jquery																[server](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=processing&t=blog)](https://so.csdn.net/so/search/s.do?q=ajax&t=blog)
个人分类：[杂七杂八](https://blog.csdn.net/largetalk/article/category/823511)
ajax 在请求时一共有0-4五种状态，
Holds the status of the XMLHttpRequest. Changes from 0 to 4: 
0: request not initialized 
1: server connection established
2: request received 
3: processing request 
4: request finished and response is ready
用原生javascript处理ajax：
```java
xmlhttp.onreadystatechange=function()
  {
  if (xmlhttp.readyState==4 && xmlhttp.status==200)
    {
    document.getElementById("myDiv").innerHTML=xmlhttp.responseText;
    }
  }
```
jquery用suceess, error, complete, beforesend等方法对应xhr的状态，但却没有对应readystate等于3时的函数，
在stackoverflow看到一解决办法，不知可行否
```java
var xhr = $.ajax({ ... });
xhr.onreadystatechange = function() { alert(xhr.readyState); };
```
