# ajax提交时控制台出现404问题解决方案 - westbrook1998的博客 - CSDN博客





2018年02月22日 21:15:36[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：1026








解决方案就是在控制器方法前加上注解 @ResponseBody

> 
@Responsebody 注解表示该方法的返回的结果直接写入 HTTP 

  响应正文（ResponseBody）中，一般在异步获取数据时使用，通常是在使用 @RequestMapping 

  后，返回值通常解析为跳转路径，加上 @Responsebody 后返回结果不会被解析为跳转路径，而是直接写入HTTP 响应正文中。
参考博客：[浅谈@RequestMapping @ResponseBody 和 @RequestBody 注解的用法与区别](http://blog.csdn.net/ff906317011/article/details/78552426)





