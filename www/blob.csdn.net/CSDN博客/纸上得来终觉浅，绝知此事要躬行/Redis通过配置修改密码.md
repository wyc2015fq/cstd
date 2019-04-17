# Redis通过配置修改密码 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月20日 10:39:26[boonya](https://me.csdn.net/boonya)阅读数：1849








Redis 设置了密码之后的访问是比较安全的，裸奔还是至少要穿点遮住隐私。



## 修改redis.conf文件配置 



**使用vi命令编辑redis.conf :**

找到# requirepass foobared 然后去掉注释，这个foobared改为自己的密码。然后使用:wq保存。

重启redis，这个时候尝试登录redis，发现可以登上，但是执行具体命令是提示操作不允许：



```
redis-cli -h 127.0.0.1 -p 6379  
redis 127.0.0.1:6379>  
redis 127.0.0.1:6379> keys *  
(error) ERR operation not permitted
```

尝试用密码登录并执行具体的命令看到可以成功执行

```
redis-cli -h 127.0.0.1 -p 6379 -a password
redis 127.0.0.1:6379> keys *
1) "myset"  
2) "mysortset"  
redis 127.0.0.1:6379> select 1  
OK
```


## 局域网访问注意事项

需要注释掉redis.config的IP绑定，否则外部访问不能成功：


`#bind 127.0.0.1`
这样局域网内其他用户就可以访问了。
















