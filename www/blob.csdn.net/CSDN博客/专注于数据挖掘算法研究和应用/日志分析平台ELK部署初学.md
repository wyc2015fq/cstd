# 日志分析平台ELK部署初学 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年04月11日 16:15:47[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2970








1）ELK介绍

ELK官方网站：https://www.elastic.co/products

elk-docker 官网：http://elk-docker.readthedocs.org/#installation

日志分析平台ELK由 ElasticSearch 、 Logstash 和 Kiabana 三个开源工具组成。Elasticsearch 是个开源分布式搜索引擎，它的特点有：分布式，零配置，自动发现，索引自动分片，索引副本机制， restful风格接口，多数据源，自动搜索负载等。Logstash 是一个完全开源的工具，他可以对你的日志进行收集、分析，并将其存储供以后使用（如搜索）。kibana 也是一个开源和免费的工具，Kibana 可以为 Logstash 和 ElasticSearch 提供的日志分析友好的
 Web 界面，可以帮助您汇总、分析和搜索重要数据日志。

在需要收集日志的所有服务上部署 logstash ，作为 logstash agent （ logstash shipper ）用于监控并过滤收集日志，将过滤后的内容发送到logstash indexer ， logstash indexer 将日志收集在一起交给全文搜索服务 ElasticSearch ，可以用 ElasticSearch 进行自定义搜索通过 Kibana 来结合自定义搜索进行页面展示。基本原理如下图：

![](https://img-blog.csdn.net/20160411161533909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




2）JDK部署

Logstash 依赖JDK，部署Ubuntu+jdk+tomcat基本环境。

3）安装Logstash

下载logstash-2.2.2.zip并解压，进入logstash-2.2.2目录。

执行启动：

#bin/logstash-e 'input { stdin { } } output { stdout {} }'

运行后显示如下：

Settings:Default pipeline workers: 8

Logstashstartup completed

logstash按照某种格式输出所输入的内容，其中-e参数允许Logstash直接通过命令行接受设置。

使用CTRL-C命令可以退出之前运行的Logstash。

使用-e参数设置外，还可以创建配置文件并制定logstash使用这个配置文件运行。

在logstash-2.2.2目录下创建test.conf，内容如下：

# vi test.conf

input {stdin { } }

output {

   stdout { codec=> rubydebug }

}

Logstash 使用input 和 output 定义收集日志时的输入和输出的相关配置。

使用logstash的-f参数来读取配置文件，并启动。

# bin/logstashagent -f test.conf

运行后显示如下：

Settings:Default pipeline workers: 8

Logstashstartup completed

Helloworld   //输入

{

       "message" => " Helloworld ",

      "@version" => "1",

    "@timestamp" =>"2016-04-11T01:27:22.765Z",

          "host" =>"ubuntu.202.96.128.86"

}

4）安装Elasticsearch

下载elasticsearch-2.2.1.tar.gz并解压，进入elasticsearch-2.2.1目录。

执行下面命令启动：

#bin/elasticsearch

提示：Exceptionin thread "main" java.lang.RuntimeException: don't run elasticsearchas root.

# adduser elastic   //新增用户

# chown –Relastic:elastic /home/elasticsearch-2.2.1 //授权安装目录访问权限

#suelastic  //重新运行bin/elasticsearch

远程连接并后台运行命令：

#bin/elasticsearch>nohup &

查看elasticsearch 的 9200 端口是否已监听：

#netstat-anp |grep  9200

在logstash-2.2.2目录下创建test2.conf，内容如下：

input {stdin { } }

output {

 elasticsearch { host => "IP"}

 stdout { codec=> rubydebug }

}

# bin/logstashagent -f test2.conf

使用curl 命令发送请求来查看 ES 是否接收到了数据：

# curl'http://ip:9200/_search?pretty'

5）安装elasticsearch 插件

Elasticsearch-kopf插件可以查询 Elasticsearch 中的数据，elasticsearch-2.2.1目录执行：

#./plugin-install lmenezes/elasticsearch-kopf

安装完成后在plugins 目录下可以看到 kopf

# lsplugins/

浏览保存在Elasticsearch 中的数据http://IP:9200/_plugin/kopf

6）安装Kibana

下载kibana-4.4.2-linux-x64.tar.gz并解压，进入kibana-4.4.2-linux-x64目录。

启动kibana执行：

#bin/kibana

使用http:// IP:5601 访问 Kibana ，登录后，首先，配置一个索引，默认， Kibana 的数据被指向 Elasticsearch ，使用默认的 logstash-* 的索引名称，并且是基于时间的，点击“ Create ”即可。

索引创建成功后，点击“ Discover ”，可以搜索和浏览 Elasticsearch 中的数据，默认搜索的是最近 15 分钟的数据，可以自定义选择时间。

7）平台测试

将logstash 配置为索引器，并将 logstash 的日志数据存储到 Elasticsearch ，本范例主要是索引本地系统日志。在在logstash-2.2.2目录下创建test3.conf，内容如下：

input {

  file {

    type =>"syslog"

     path => ["/var/log/messages","/var/log/syslog" ]

  }

  syslog {

    type =>"syslog"

    port =>"5544"

  }

}

output {

  stdout { codec=> rubydebug }

  elasticsearch {host => "IP" }

}

启动:

# bin/logstash–f test3.conf

使用echo 命令模拟写入日志：

# echo"`date` test3" >>/var/log/messages

刷新 kibana ，最新的测试数据显示到浏览器中。
            


