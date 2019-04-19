# MySQL表不能修改、删除等操作，卡死、锁死情况的处理办法。 - Ms.Fox的专栏 - CSDN博客
2018年09月05日 17:07:10[lulu-lu](https://me.csdn.net/smbluesky)阅读数：425
MySQL如果频繁的修改一个表的数据，那么这么表会被锁死。造成假死现象。
比如用Navicat等连接工具操作，Navicat会直接未响应，只能强制关闭软件，但是重启后依然无效。
解决办法：
首先执行：
show full processlist;  //列出当前的操作process，一般会看到很多waiting的process，说明已经有卡住的proces了，我们要杀死这些process！！
再执行：
kill processid;  //processid表示process的id，比如kill 3301，就会将id为3301的process杀死。
使用 kill 将所有的 id 杀死。然后重启MySQL，一般就会解决了。如果还不行，那应该是不可能的吧。。。
重启MySQL：
net stop mysql  //停止MySQL
net start mysql  //启动MySQL
