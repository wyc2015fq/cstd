# 解决mysql导入数据库出现数据丢失 - weixin_33985507的博客 - CSDN博客
2018年08月24日 13:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：29
问题：有一次导入一个数据库 , 数据表内的content字段内容特别大 , 导入数据库失败
## 1、打开my.ini文件 , 在文件末尾填写如下内容
```
wait_timeout=2880000
interactive_time=2880000
max_allowed_packet=16M
```
wait_timeout：代表mysql在误操作2880000秒之后链接会关闭
max_allowed_packet：代表控制其缓存区的最大长度
wait_timeout：代表无操作链接等待时间
## 2、重启mysql , 此设置可根据自己的需求修改
## 如何查看mysql版本
登陆进入mysql数据库
```
mysql -uroot -p
select version();
```
