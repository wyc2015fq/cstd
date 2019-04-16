# 服务器数据库系列 - Nginx 虚拟机设置 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月20日 17:20:45[initphp](https://me.csdn.net/initphp)阅读数：1031标签：[nginx																[server																[access																[虚拟机](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)
个人分类：[服务器数据库系列](https://blog.csdn.net/initphp/article/category/2609107)




- 配置：



```
http {

 server {

 listen          80;

 server_name     www.domain1.com;

 access_log      logs/domain1.access.log main;

 location / {

 index index.html;

 root  /var/www/domain1.com/htdocs;

 }

 }

 server {

 listen          80;

 server_name     www.domain2.com;

 access_log      logs/domain2.access.log main;

 location / {

 index index.html;

 root  /var/www/domain2.com/htdocs;

 }

 }

}
```

- 说明
- 只要在模块里面新增模块
- 具体的根据您的实际需求去配置](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)




