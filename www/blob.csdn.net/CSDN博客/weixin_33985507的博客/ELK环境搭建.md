# ELK环境搭建 - weixin_33985507的博客 - CSDN博客
2016年08月05日 14:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
说明：
1.基础环境
Centos7,jdk7
2.本次使用的版本
Elasticsearch:1.6.2
Logstash:2.3.4
Kibana:3.1.3
一、  下载elk三大组件
Elasticsearch下载地址： [https://www.elastic.co/downloads/elasticsearch](https://link.jianshu.com?t=https://www.elastic.co/downloads/elasticsearch) （目前最新版本：2.1.1）
Logstash下载地址： [https://www.elastic.co/downloads/logstash](https://link.jianshu.com?t=https://www.elastic.co/downloads/logstash) （目前最新版本：2.1.1）
Kibana下载地址： [https://www.elastic.co/downloads/kibana](https://link.jianshu.com?t=https://www.elastic.co/downloads/kibana) （目前最新版本：4.3.1）
二、  启动elasticsearch
2.1 配置及启动
打开conf/elasticsearch.yml
添加
http.cors.enabled: true
http.cors.allow-origin: "/.*/"
为kibana访问elasticsearch配置
进入elasticsearch目录\bin
./elasticsearch start &
启动成功后，在浏览器输入[http://ip:9200](https://link.jianshu.com?t=http://ip:9200)看到如下输出，说明安装正确
{
"status" : 200,
"name" : "Illyana Rasputin",
"cluster_name" : "elasticsearch",
"version" : {
"number" : "1.6.2",
"build_hash" : "622039121e53e5f520b5ff8720fdbd3d0cb5326b",
"build_timestamp" : "2015-07-29T09:24:47Z",
"build_snapshot" : false,
"lucene_version" : "4.10.4"
},
"tagline" : "You Know, for Search"
}
2.2 安装插件
在bin目录下输入
./plugin list查看当前插件列表
./plugin install pluginName
三、  logstash的启动与配置
3.1 在logstash主目录下创建conf目录，再在conf下创建logstash.conf
编辑logstash.conf
内容如下：
input{
tcp {
host => "127.0.0.1"
port => 9999
mode => "server"
tags => ["tags"]
codec => json_lines
}
stdin{}
}
output{
stdout{
codec => rubydebug
}
elasticsearch {
hosts => ["127.0.0.1:9200"]
index => "test"
}
}
3.2 启动
在logstash 的bin目录下，输入
./logstash agent -f ../conf/logstash.conf
稍等片刻，如果看到Logstash startup completed，则表示启动成功。然后另开一个终端窗口，随便找个文本编辑工具（比如：vi），向/var/opt/log/a.log里写点东西，比如：hello world之类，然后保存。观察logstash的终端运行窗口，是否有东西输出，如果有以下类似输出
{
"message" => "hello world",
"@version" => "1",
"@timestamp" => "2016-01-08T14:35:16.834Z",
"host" => "yangjunmingdeMacBook-Pro.local",
"path" => "/var/opt/log/a.log"
}
四，kibana配置及启动
4.1修改配置文件
在kibana主目录下编辑config.js
修改elasticsearch: [http://ip:9200](https://link.jianshu.com?t=http://ip:9200)
4.2启动
下载并解压tomcat
将kibana重命名
Mv kibana-3.1.3 kibana
Cp -r kibana apache-tomcat
启动tomcat
在浏览器访问[http://ip](https://link.jianshu.com?t=http://ip)：8080/kibana
看到如下界面则安装成功
![460614-f14a0de2e2fab30a.png](https://upload-images.jianshu.io/upload_images/460614-f14a0de2e2fab30a.png)
Paste_Image.png
