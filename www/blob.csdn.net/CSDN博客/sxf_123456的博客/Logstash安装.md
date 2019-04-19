# Logstash安装 - sxf_123456的博客 - CSDN博客
2018年01月11日 19:55:10[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：161
个人分类：[安装文档](https://blog.csdn.net/sxf_123456/article/category/7396488)
**Logstash安装过程:**
0、下载包
https://www.elastic.co/downloads/past-releases/logstash-5-2-2
1、下载es安装包
wgethttps://artifacts.elastic.co/downloads/elasticsearch/elasticsearch-5.2.2.rpm
安装jdk1.8
  参照安装jdk
**安装logstash**
1、  rpm安装
2、安装x-pack
在线安装
bin/logstash-plugin install file:///path/to/file/x-pack-5.2.2.zip
离线安装
bin/kibana-plugin installfile:///path/to/file/x-pack-5.2.2.zip
3、启动logstash
4、建立软连接
5、logstash软连接
cd /usr/share/logstash/
ln -s /etc/logstash/ ./config
6、  测试
6.1创建test.conf文件，写入下边内容
stdin {
    stdin{}
}
output {
   stdout {
    codec => rubydebug
 } 
}
6.2执行命令
/bin/logstash -f  /etc/logstash/conf.d/test.conf
