# jquery.cookie.js 使用小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月14日 13:45:54[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：102
个人分类：[前端js](https://blog.csdn.net/elesos/article/category/7108612)
先引入jquery，再引入：jquery.cookie.js
**添加一个"会话cookie"**
$.cookie('the_cookie', 'the_value');
这里没有指明 cookie有效时间，所创建的cookie有效期默认到用户关闭浏览器为止，所以被称为 “会话cookie（session cookie）”。
**创建一个cookie并设置有效时间为 7天**
$.cookie('the_cookie', 'the_value',
 { expires: 7 });
**读取cookie**
$.cookie('the_cookie');
**删除cookie**
$.cookie('the_cookie', null);   //通过传递null作为cookie的值即可
[https://www.cnblogs.com/webcome/p/5470975.html](https://www.cnblogs.com/webcome/p/5470975.html)
