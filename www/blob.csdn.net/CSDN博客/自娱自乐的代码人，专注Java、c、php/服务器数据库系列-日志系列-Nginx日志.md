# 服务器数据库系列 - 日志系列 - Nginx日志 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月21日 15:15:21[initphp](https://me.csdn.net/initphp)阅读数：1000








Nginx日志可以分为两类：**用户访问日志**和**Nginx错误日志**。



**Nginx的错误日志**

一般会记录PHP的运行错误的信息。
`error_log  /usr/local/nginx.1.0.5/nginx_error.log  crit;`


![](https://img-my.csdn.net/uploads/201207/21/1342855430_3622.jpg)


**Nginx的访问日志：**

一般记录访问信息，包含来源IP等信息。

```
log_format  Payaccess  '$remote_addr - $remote_user [$time_local] "$request" '
              '$status $body_bytes_sent "$http_referer" '
              '"$http_user_agent" $http_x_forwarded_for';
    access_log  /usr/local/nginx.1.0.5/logs/pay.log  Payaccess;
```


![](https://img-my.csdn.net/uploads/201207/21/1342855124_3613.jpg)



