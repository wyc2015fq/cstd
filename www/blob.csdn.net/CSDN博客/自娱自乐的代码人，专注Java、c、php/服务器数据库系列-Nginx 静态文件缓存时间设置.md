# 服务器数据库系列 - Nginx 静态文件缓存时间设置 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月20日 17:17:06[initphp](https://me.csdn.net/initphp)阅读数：1537标签：[nginx																[css](https://so.csdn.net/so/search/s.do?q=css&t=blog)](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)
个人分类：[服务器数据库系列](https://blog.csdn.net/initphp/article/category/2609107)








```
location ~ .*\.(gif|jpg|jpeg|png|bmp|swf)$
    {
      expires      30d;
    }

    location ~ .*\.(js|css)?$
    {
      expires      1h;
    }
```

- 代表图片默认缓存30天
- JS和CSS默认缓存1小时
- 用户可以根据需求设置Nginx





