# Table 'xxx'is marked as crashed and last (automatic) repair failed - weixin_33985507的博客 - CSDN博客
2013年03月20日 23:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
　　维护着一个大型资讯网站，数据量比较大，最近搬到linux服务器下，在生成静态网站的时候突然发现首页调用的后台数据全都没有了。着急了，多年的心血啊。
　　第一反应是想着是不是数据被破坏了，丢了一些数据。检查后发现并不是。
　　然后通过phpmyadmin查看archives表的时候提示：Table 'xxx'is marked as crashed and last (automatic) repair failed。这意味着该表被标记为跌宕了。于是想着修复：repair table arvhives，修复却失败了。
　　只好通过mysql命令行来执行了。
　　1)service mysqld stop  停止mysql服务；
　　2)cd /var/local/mysql/tbl  切换到该表目录下(注意：不切换过来会总提示myisamchk一些错误导致失败)；
　　3)myisamchk -r xxx.MYI  （如果还是提示错误，就多加一个参数-f  强制进行。）
```
[root@localhost tjinfo]# myisamchk -r dede_archives.MYI
- recovering (with sort) MyISAM-table 'dede_archives.MYI'
Data records: 1781
- Fixing index 1
- Fixing index 2
- Fixing index 3
- Fixing index 4
```
　　最后查看，成功！
**注意：虽然这样成功了，但是当你再次添加文章的时候，又会出现这样的问题，所以长久的解决办法如下：**
        把服务器上的数据导入到本地，然后在本地用myisamchk -r -f *.MYI修复了下，然后导回去。
