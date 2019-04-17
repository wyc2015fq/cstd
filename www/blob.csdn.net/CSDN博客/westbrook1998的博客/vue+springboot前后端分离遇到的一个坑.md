# vue+springboot前后端分离遇到的一个坑 - westbrook1998的博客 - CSDN博客





2018年06月05日 22:38:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：1115








今天重新拿起vue和springboot 简单写了个页面和后台测试一下 

结果登录前端用axios的get方法就能获取到数据，用post就死活不行，一直是400，找了一下午一晚上，终于找到一个有用的博客 
[大佬的博客](https://blog.csdn.net/transformer_WSZ/article/details/80273815)

果然是这个请求头的 `Content-Type`的问题，前端有试着改过但是没效果，最后解决方案就是后端控制器不要用`@RequestParam`注解而是用`@RequestMap`，然后再从map中取出参数，ok，感动



