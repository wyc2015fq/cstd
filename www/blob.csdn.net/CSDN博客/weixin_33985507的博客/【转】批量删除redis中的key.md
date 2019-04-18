# 【转】批量删除redis中的key - weixin_33985507的博客 - CSDN博客
2017年07月07日 10:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
**1. DEL 直接加键名称**
DEL key1 key2 key3
127.0.0.1:6379>  DEL site_msg_99973  false site_msg_99974   false site_msg_99979false
**2. 批量删除key**
可用keys ‘str*’ 列出要删除的key，接linux管道删除（linux命令行执行）
根据通配符查看待删除的key
redis-cli KEYS "site_msg_999*”
接linux管道删除之
redis-cli KEYS "site_msg_999*"|xargs redis-cli DEL
但是如果key中有各种转义字符，例如如下的key需要批量删除，第二种方法就无法实现删除，需要用方法3
```
"newmobile:type:list:1{\"lastid\":0,\"limit\":20,\"offset\":60,\"sortName\":\"\",\"sortOrder\":\"asc\"}"
```
**3.更好的方式**
keys * 命令在数据量很大的情况下，直接在redis cli中执行会严重影响服务器性能，更好的方式是在lua脚本中执行
eval方式执行redis lua
lua方式通配符查找
redis-cli eval "return redis.call('keys','newmobile:type:list*')" 0
lua方式通配符删除
redis-cli -hxxx -pxxx -a 'xxx' eval "return redis.call('del',unpack(redis.call('keys',ARGV[1])))" 0 'newmobile:type:list*'
未完待续。。。
【转自】：http://gccmx.blog.51cto.com/479381/1611419
