# Redis (error) MISCONF Redis is configured to save RDB snapshots, but is currently not able to persis - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月18日 17:21:08[boonya](https://me.csdn.net/boonya)阅读数：501








异常：(error) MISCONF Redis is configured to save RDB snapshots, but is currently not able to persist on disk. Commands that may modify the data set are disabled. Please check Redis logs for details about the error.


![](https://img-blog.csdn.net/20170318171928285)




原因：强制关闭Redis快照导致不能持久化。



解决方案：运行config set stop-writes-on-bgsave-error no　命令后，关闭配置项stop-writes-on-bgsave-error解决该问题。



```
boonya@ubuntu:/usr/local/redis/redis-3.0.3$ 
boonya@ubuntu:/usr/local/redis/redis-3.0.3$ ./src/redis-cli
127.0.0.1:6379> auth 123456
OK
127.0.0.1:6379> config set stop-writes-on-bgsave-error no
OK
127.0.0.1:6379> set foo tar
OK
127.0.0.1:6379> get foo
"tar"
127.0.0.1:6379>
```






