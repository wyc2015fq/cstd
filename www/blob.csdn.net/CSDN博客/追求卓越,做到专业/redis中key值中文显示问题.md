
# redis中key值中文显示问题 - 追求卓越,做到专业 - CSDN博客


2019年01月24日 09:54:02[Waldenz](https://me.csdn.net/enter89)阅读数：490


key中包含中文，在客户端中显示为\xe4\xbd\xa0等的16进制字符串。
解决方法：
连接时加上--raw 参数。意思为在终端上强制原始输出。
```python
/home/redis/redis-3.0.7/bin/redis-cli -a thepassword --raw
```


