
# grafana-4.1.1 Install和新建日志分析的DashBoard - leofionn的博客 - CSDN博客


2018年05月01日 08:37:52[leofionn](https://me.csdn.net/qq_36142114)阅读数：115个人分类：[hadoop搭建																](https://blog.csdn.net/qq_36142114/article/category/7383244)



原地址：http://blog.itpub.net/30089851/viewspace-2133327/
1.下载
wgethttps://grafanarel.s3.amazonaws.com/builds/grafana-4.1.1-1484211277.linux-x64.tar.gz
2.解压
tar -zxvf grafana-4.1.1-1484211277.linux-x64.tar.gz3.配置文件
cd grafana-4.1.1-1484211277
cp conf/sample.ini conf/custom.ini
\#  make changes to conf/custom.ini then start grafana-server
4.后台启动
./bin/grafana-server &
5.打开web
http://172.16.101.66:3000/admin/admin
6.配置数据源influxdb
![](http://blog.itpub.net/attachment/201702/9/30089851_14866509158Xlr.jpg)
还要填写Database 为 online_log_analysis
7.IDEA本机运行OnLineLogAanlysis2.class，实时计算存储到influxdb
8.新建dashboard和 cdh_hdfs_warn曲线图
![](http://blog.itpub.net/attachment/201702/9/30089851_1486650967rZ9L.jpg)


