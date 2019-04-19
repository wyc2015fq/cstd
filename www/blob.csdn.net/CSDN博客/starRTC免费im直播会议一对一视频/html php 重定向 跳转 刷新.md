# html php 重定向 跳转 刷新 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月10日 17:33:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1363
1秒后跳转
法一：
<!DOCTYPE HTML>
<html>
<head>    
<meta http-equiv="refresh" content="1;url=[http://wiki.elesos.com](http://wiki.elesos.com/)">   
</head>
</html>
2，js实现
<script>   
// 以下方式直接跳转 
window.location.href='[http://wiki.elesos.com](http://wiki.elesos.com/)';  
// 以下方式定时跳转  
setTimeout("javascript:location.href='[http://wiki.elesos.com](http://wiki.elesos.com/)'", 1000);   
</script> 
使用content属性表示刷新或跳转的开始时间与跳转的网址
5秒之后刷新本页面:
<meta http-equiv="refresh" content="5" />
5秒之后转到首页:
<meta http-equiv="refresh" content="5; url=[http://www.elesos.com/](http://www.elesos.com/)" />
php跳转与定时跳转
header("Location: $httpfile");
header("refresh:1;url=$httpfile");
