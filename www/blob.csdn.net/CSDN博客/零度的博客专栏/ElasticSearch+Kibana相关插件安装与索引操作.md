# ElasticSearch+Kibana相关插件安装与索引操作 - 零度的博客专栏 - CSDN博客
2018年06月15日 10:49:58[零度anngle](https://me.csdn.net/zmx729618)阅读数：201
### 一 前言
    ElasticiSearch 简介 ElasticSearch是一个基于Lucene的搜索服务器。它提供了一个分布式多用户能力的全文搜索引擎，基于RESTful web接口。Elasticsearch是用Java开发的，并作为Apache许可条款下的开放源码发布，是当前流行的企业级搜索引擎。设计用于[云计算](http://baike.baidu.com/view/1316082.htm)中，能够达到实时搜索，稳定，可靠，快速，安装使用方便。
### 二  安装步骤
# 2.1 Elasticsearch2.2安装（Yum安装步骤）
##   下载安装签名
`rpm --import https://packages.elastic.co/GPG-KEY-elasticsearch`
##   配置更新源
在 /etc/yum.repos.d/ 目录下创建.repo 后缀的文件,如elasticsearch.repo，编辑内容如下：
```
[elasticsearch-2.x]
name=Elasticsearch repository for2.x packages
baseurl=https://packages.elastic.co/elasticsearch/2.x/centos
gpgcheck=1
gpgkey=https://packages.elastic.co/GPG-KEY-elasticsearch
enabled=1
```
## 使用yum命令安装：
`yum install elasticsearch`
## 加入启动项：
`chkconfig --add elasticsearch`
## 基本配置
编辑文件/etc/elasticsearch/elasticsearch.yml，根据实际情况配置如下节点(也可使用默认)
- [cluster.name](http://cluster.name/)
- [node.name](http://node.name/)
- index.number_of_shards
- index.number_of_replicas
- network.host
## 启动Es
运行命令：
`Service ElasticSearch start`
参考地址：
- https://www.elastic.co/guide/en/elasticsearch/reference/current/setup-repositories.html
# 2.2 Kibana安装
## Kibana 可使用它对数据进行高效的搜索、可视化、分析等各种操作
## 下载解压安装包
下载地址：
- [https://www.elastic.co/downloads/kibana](https://www.elastic.co/downloads/kibana)
## 配置
编辑文件config/kibana.yml ，配置属性：
- server.host (站点地址)
- elasticsearch.url (指向Elasticsearch 实例)
## 启动
`.bin/kibana`
参考地址：
- [https://www.elastic.co/downloads/kibana](https://www.elastic.co/downloads/kibana)
# 2.3 Marvel安装
Marvel是Elasticsearch的管理和监控工具
## 安装Marvel代理插件到各个ES节点
在ES目录下运行 bin/plugin 安装插件许可
`bin/plugin install license`
运行bin/plugin install 安装Marvel代理插件
`bin/plugin install marvel-agent`
## 将Marvel插件安装到Kiabana
运行命令：
`bin/kibana plugin --install elasticsearch/marvel/latest`
#### 重启 Kibana
参考地址：
- [https://www.elastic.co/downloads/marvel](https://www.elastic.co/downloads/marvel)
- [https://www.elastic.co/guide/en/marvel/current/getting-started.html](https://www.elastic.co/guide/en/marvel/current/getting-started.html)
# 2.4 Sense安装
一个flask写的elasticsearch查询工具:
支持es查询语言自动提示，es结构自动提示，支持两种主题，支持查询历史记录，支持快捷键。
到Kibana目录运行命令安装 Sense插件
`./bin/kibana plugin --install elastic/sense`
重新启动 Kibana
参考地址：
- [https://www.elastic.co/guide/en/sense/master/installing.html](https://www.elastic.co/guide/en/sense/master/installing.html)
# 2.5 安装Ik分词插件
## 获取源码
`git clone https://github.com/medcl/elasticsearch-analysis-ik.git`
## 编译
去源码根目录下编译
```
cd elasticsearch-analysis-ik
mvn clean
mvn compile
mvn package
```
将文件 #{project_path}/elasticsearch-analysis-ik/target/releases/elasticsearch-analysis-ik-*.zip 复制解压到 elasticsearch的目录: plugins/ik
这几个插件安装完成后效果如下
![](https://images2015.cnblogs.com/blog/366784/201604/366784-20160412111445473-846328426.png)
### 三 ElasticSearch客户端操作索引
       官方文档语法有相应的官网文档解释很全，也很简单,最常用的对索引的一些基本操作 新建， 删除， 添加数据， 局部更新，给索 引设别名。官方文档地址：https://www.elastic.co/guide/en/elasticsearch/client/index.html
### 四 总结  
　　以上只是对ElasticSearch 最简单基本的一些操作与概念，大家如果有兴趣可以继续深入了解。
