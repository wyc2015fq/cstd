# Kibana安装与初探 - 零度的博客专栏 - CSDN博客
2018年05月18日 10:20:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：4545

原文出处：https://blog.csdn.net/qq_19598855/article/details/52420027
# 日志分析任务
## 1.    ElasticSearch
### 1.1  ElasticSearch安装
从[Elastic官网](https://www.elastic.co/products/elasticsearch)的Product选择ElasticSearch下载。 
![这里写图片描述](https://img-blog.csdn.net/20160902230028219)
 这里下载2.3.5版本，仅介绍Window下的安装。将下载好的zip包解压直接放在D盘，直接点击bin下的elasticsearch.bin启动。 
![这里写图片描述](https://img-blog.csdn.net/20160902230048914)
 打开浏览器输入如下[http://localhost:9200](http://localhost:9200/)，出现如下信息即成功。这里的”name”，是漫威中的名字，每次打开都不一样。ElasticSearch支持多节点，我们这里采用了默认配置，配置多个节点时，即会显示多个漫威人物的名字。 
![这里写图片描述](https://img-blog.csdn.net/20160902230113298)
![这里写图片描述](https://img-blog.csdn.net/20160902230130986)
### 1.2  安装Head插件
Elasticsearch可使用官方的Mavel（漫威，哈哈）进行管理，我们使用更好用的Head插件进行管理。打开cmd窗口，进入elasticsearch的bin目录，输入如下指令，安装head插件。（大多数教程为-install，es 2.0版本之后，使用install即可）。 
 plugin install mobz/elasticsearch-head 
 输入地址[http://localhost:9200/_plugin/head/](http://localhost:9200/_plugin/head/)。Head为我们提供索引信息、建立索引、数据浏览、基本查询、复合查询功能。 
![这里写图片描述](https://img-blog.csdn.net/20160902230205596)
## 2.    Kibana
### 2.1  安装Kibana
同样，从elastic官网下载Kibana的zip包，解压，运行bin下的kibana.bat，即可启动Kibana。打开浏览器，键入[http://localhost:5601](http://localhost:5601/)访问Kibana。
### 2.2 数据准备
从官方教程下载数据集[https://www.elastic.co/guide/en/kibana/current/getting-started.html](https://www.elastic.co/guide/en/kibana/current/getting-started.html)， 
 数据集1：莎士比亚全集；数据集2：银行账户数据；数据集3：日志数据，字段非常多，但是我们暂时只关注地理位置的部分。
```
{
    "line_id": INT,
    "play_name": "String",
    "speech_number": INT,
    "line_number": "String",
    "speaker": "String",
    "text_entry": "String",
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
```
{
    "account_number": INT,
    "balance": INT,
    "firstname": "String",
    "lastname": "String",
    "age": INT,
    "gender": "M or F",
    "address": "String",
    "employer": "String",
    "email": "String",
    "city": "String",
    "state": "String"
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
```
{
    "memory": INT,
    "geo.coordinates": "geo_point"
    "@timestamp": "date"
}
```
- 1
- 2
- 3
- 4
- 5
对数据进行Mapping，即定义数据各个Field的格式，是否分词，什么类型。可使用curl在命令行中对es进行mapping，也可以使用head的复杂查询功能。当然，我们在生产环境中，通过界面一个一个创建还是速度太慢了。
```
{
 "mappings" : {
  "_default_" : {
   "properties" : {
    "speaker" : {"type": "string", "index" : "not_analyzed" },// 未分词
    "play_name" : {"type": "string", "index" : "not_analyzed" },
    "line_id" : { "type" : "integer" }, // 整形
    "speech_number" : { "type" : "integer" }
   }
  }
 }
}
';
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
```
curl -XPUT http://localhost:9200/logstash-2015.05.18 -d '
{
  "mappings": {
    "log": {
      "properties": {
        "geo": {
          "properties": {
            "coordinates": {
              "type": "geo_point"
            }
          }
        }
      }
    }
  }
}
';
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
![这里写图片描述](https://img-blog.csdn.net/20160902230322988)
![这里写图片描述](https://img-blog.csdn.net/20160902230355708)
![这里写图片描述](https://img-blog.csdn.net/20160902230416067)
 官方建议使用Bulk API批量写入我们下载的json数据，我们这里先采用了Java API来写入数据。使用Java API有一点小麻烦，Object需要进行解析，所以需要写POJO，还需要把每一个Field一一对应好。使用Eclipse Mars EE，安装好Maven和M2E，新建Maven项目。
```xml
<dependency>
      <groupId>org.elasticsearch</groupId>
      <artifactId>elasticsearch</artifactId>
      <version>2.3.5</version>
    </dependency>
    <dependency>
      <groupId>com.google.guava</groupId>
      <artifactId>guava</artifactId>
      <version>19.0</version>
    </dependency>
    <dependency>
      <groupId>commons-io</groupId>
      <artifactId>commons-io</artifactId>
      <version>2.5</version>
    </dependency>
    <dependency>
      <groupId>com.alibaba</groupId>
      <artifactId>fastjson</artifactId>
      <version>1.2.16</version>
    </dependency>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
创建工程，包结构如下： 
 FileOperation封装Commons-io的FileUtils操作； 
 ShakeSpeare，POJO实体，用来接收JSON parse的对象； 
 ShakeSpeareIndex，POJO实体，用来接收JSON parse的对象； 
 BulkInsert用来进行批量的insert操作。 
![这里写图片描述](https://img-blog.csdn.net/20160902230442193)
![这里写图片描述](https://img-blog.csdn.net/20160902230502881)
![这里写图片描述](https://img-blog.csdn.net/20160902230515053)
采用官方的Bulk API操作更为简单：
`curl –XPOST "localhost：9200/bank/account/_bulk?pretty" -data-binary @C:\eespace\elk\src\main\resources\data\accounts.json`- 1
Window下是双引号，后面的json文件的路径需加@。 
 建完索引，我们可以看到如下的界面：
### 2.2  Kibana绘图
首先要让Kibana知道需要进行可视化和管理的ElasticSearch中的索引，采用正则的方式进行匹配，比如日志文件，就可以用logstash-2015.05*。 
![这里写图片描述](https://img-blog.csdn.net/20160902230539538)
 日志有时间线，对于account和shakespeare不要勾选index contains time-based events。点击Discover，开始探索应用。 
![这里写图片描述](https://img-blog.csdn.net/20160902230602463)
 可以看到各个字段和字段对应的数据。点击Visualize开始进行可视化展示。可选的类型很多，条形图、走势图、饼图、地图，甚至可以添加Markdown widget建立一个图形化的界面。 
![这里写图片描述](https://img-blog.csdn.net/20160902230629573)
 左上角选择Bank索引，键入account_number:<100 AND balance:>47500，进行数据筛选。得到筛选结果，如果只需要看其中一些数据，我们点击相应的Fileds的Add按钮即可。 
![这里写图片描述](https://img-blog.csdn.net/20160902231237200)
![这里写图片描述](https://img-blog.csdn.net/20160902230712165)
 我们建立一个饼图，来看看银行的账户里面税钱在各个范围内的占比。在可视化中点击From a new query，即新建一个Query来建立一个饼图。 
![这里写图片描述](https://img-blog.csdn.net/20160902230745244)
 选择Range，即按照范围划分，建立饼图。 
![这里写图片描述](https://img-blog.csdn.net/20160902230800948)
 我们还可以在另一个维度上继续拆分，比如，查看这些人的年龄分布。 
![这里写图片描述](https://img-blog.csdn.net/20160902230824339)
 我们在右上角的第二个按钮点击保存，保存该图为Pie Chart。下面通过条形图看看莎士比亚的戏剧中哪部剧台词最少，这部剧中哪个任务的话最多。 
![这里写图片描述](https://img-blog.csdn.net/20160902230837964)
 进行如上设置，Y轴选不重复计数，来统计speaker字段；X轴选话剧名，选词，升序排列，得到条形图。在Options中选择grouped模式，即可在同一副图中显示两项内容。 
![这里写图片描述](https://img-blog.csdn.net/20160902230900402)
![这里写图片描述](https://img-blog.csdn.net/20160902230914918)
 下面我们分析Log中的访问的IP地址，都分布在哪些国家，建立一个可视化的地图。首选选择一个时间段。 
![这里写图片描述](https://img-blog.csdn.net/20160902230936341)
 选择我们建坐标时的Geo Coordinate，即可得到地理坐标图，点击保存。 
![这里写图片描述](https://img-blog.csdn.net/20160902230958199)
 最后，我们输入一些文字性信息来展示。可视化中选择Markdown，按照Markdown的格式我们写上合适的内容。 
![这里写图片描述](https://img-blog.csdn.net/20160902231016201)
 进入DashBoard，将我们保存好的内容展示在页面上。 
![这里写图片描述](https://img-blog.csdn.net/20160902231026889)
 稍加整理，得到最终的展示界面。 
![这里写图片描述](https://img-blog.csdn.net/20160902231038624)
