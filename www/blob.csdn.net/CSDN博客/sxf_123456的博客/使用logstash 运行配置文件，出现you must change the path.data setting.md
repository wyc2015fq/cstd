# 使用logstash 运行配置文件，出现you must change the "path.data" setting - sxf_123456的博客 - CSDN博客
2017年12月21日 15:05:17[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：5491
 Logstash could not be started because there is already another instance using the configured data directory.  If you wish to run multiple instances, you must change the "path.data" setting
将原先的命令：
/bin/logstash  -f 010.conf  
改为：
/bin/logstash  -f 010.conf  --path.data=/root/
其中，--path.data是指存放数据的路径
