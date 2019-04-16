# 服务器数据库系列 - Nginx502页面解决方法之一 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月21日 16:23:45[initphp](https://me.csdn.net/initphp)阅读数：962
个人分类：[服务器数据库系列](https://blog.csdn.net/initphp/article/category/2609107)









                
部分PHP程序的执行时间超过了Nginx的等待时间，可以适当增加nginx.conf配置文件中FastCGI的timeout时间，例如：

```
......
http
{
......
fastcgi_connect_timeout 300;
fastcgi_send_timeout 300;
fastcgi_read_timeout 300;
......
}
......
```

`也有可能是PHP代码不正确，比如SVN冲突`




