# logstash日志分析的配置和使用 - z69183787的专栏 - CSDN博客
2018年07月25日 10:34:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3831
[https://www.cnblogs.com/yincheng/p/logstash.html](https://www.cnblogs.com/yincheng/p/logstash.html)
      logstash是一个数据分析软件，主要目的是分析log日志。整一套软件可以当作一个MVC模型，logstash是controller层，Elasticsearch是一个model层，kibana是view层。
      首先将数据传给logstash，它将数据进行过滤和格式化（转成JSON格式），然后传给Elasticsearch进行存储、建搜索的索引，kibana提供前端的页面再进行搜索和图表可视化，它是调用Elasticsearch的接口返回的数据进行可视化。logstash和Elasticsearch是用Java写的，kibana使用node.js框架。
      这个软件官网有很详细的使用说明，[https://www.elastic.co/](https://www.elastic.co/)，除了docs之外，还有视频教程。这篇博客集合了docs和视频里面一些比较重要的设置和使用。
## 一、logstash的配置
### 1. 定义数据源
      写一个配置文件，可命名为logstash.conf，输入以下内容：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
input {
        file {
                path => "/data/web/logstash/logFile/*/*"
                start_position => "beginning" #从文件开始处读写
        }
#       stdin {}  #可以从标准输入读数据
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
      定义的数据源，支持从文件、stdin、kafka、twitter等来源，甚至可以自己写一个input plugin。如果像上面那样用通配符写file，如果有新日志文件拷进来，它会自动去扫描。
### 2. 定义数据的格式
      根据打日志的格式，用正则表达式进行匹配
![复制代码](https://common.cnblogs.com/images/copycode.gif)
filter {
  #定义数据的格式
  grok {
    match => { "message" => "%{DATA:timestamp}\|%{IP:serverIp}\|%{IP:clientIp}\|%{DATA:logSource}\|%{DATA:userId}\|%{DATA:reqUrl}\|%{DATA:reqUri}\|%{DATA:refer}\|%{DATA:device}\|%{DATA:textDuring}\|%{DATA:duringTime:int}\|\|"}
  }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
      由于打日志的格式是这样的：
> 
2015-05-07-16:03:04|10.4.29.158|120.131.74.116|WEB|11299073|http://quxue.renren.com/shareApp?isappinstalled=0&userId=11299073&from=groupmessage|/shareApp|null|Mozilla/5.0 (iPhone; CPU iPhone OS 8_2 like Mac OS X) AppleWebKit/600.1.4 (KHTML, like Gecko) Mobile/12D508 MicroMessenger/6.1.5 NetType/WIFI|duringTime|98||
      以|符号隔开，第一个是访问时间，timestamp，作为logstash的时间戳，接下来的依次为：服务端IP，客户端的IP，机器类型(WEB/APP/ADMIN)，用户的ID(没有用0表示)，请求的完整网址，请求的控制器路径，reference，设备的信息，duringTime，请求所花的时间。
     如上面代码，依次定义字段，用一个正则表达式进行匹配，DATA是[logstash定义好的正则](https://github.com/elastic/logstash/blob/v1.4.2/patterns/grok-patterns)，其实就是(.*?)，并且定义字段名。
     我们将访问时间作为logstash的时间戳，有了这个，我们就可以以时间为区分，查看分析某段时间的请求是怎样的，如果没有匹配到这个时间的话，logstash将以当前时间作为该条记录的时间戳。需要再filter里面定义时间戳的格式，即打日志用的格式：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
filter {
  #定义数据的格式
  grok {#同上... }
  #定义时间戳的格式
**  date {    match => [ "timestamp", "yyyy-MM-dd-HH:mm:ss" ]    locale => "cn"****  }**
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
     在上面的字段里面需要跟logstash指出哪个是客户端IP，logstash会自动去抓取该IP的相关位置信息：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
filter {
  #定义数据的格式
  grok {#同上}
  #定义时间戳的格式
  date {#同上}
  #定义客户端的IP是哪个字段（上面定义的数据格式）
**  geoip {    source => "clientIp"  }**}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
     同样地还有客户端的UA，由于UA的格式比较多，logstash也会自动去分析，提取操作系统等相关信息
  #定义客户端设备是哪一个字段
  useragent {
    source => "device"
    target => "userDevice"
  }
      哪些字段是整型的，也需要告诉logstash，为了后面分析时可进行排序，使用的数据里面只有一个时间
  #需要进行转换的字段，这里是将访问的时间转成int，再传给Elasticsearch
  mutate {
    convert => ["duringTime", "integer"]
  }
### 3. 输出配置
      最后就是输出的配置，将过滤扣的数据输出到elasticsearch
![复制代码](https://common.cnblogs.com/images/copycode.gif)
output {
  #将输出保存到elasticsearch，如果没有匹配到时间就不保存，因为日志里的网址参数有些带有换行
  if [timestamp] =~ /^\d{4}-\d{2}-\d{2}/ {
        elasticsearch { host => localhost }
  }
   #输出到stdout
#  stdout { codec => rubydebug }
   #定义访问数据的用户名和密码
#  user => webService
#  password => 1q2w3e4r
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
      我们将上述配置，保存到logstash.conf，然后运行logstash
      在logstash启动完成之后，输入上面的那条访问记录，logstash将输出过滤后的数据：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201131434304-1696235950.jpg)![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201131526960-219609650.jpg)
     可以看到logstash，自动去查询IP的归属地，并将请求里面的device字段进行分析。
## 二、配置Elasticsearch和kibana
### 1. Elasticsearch
      这个不需要怎么配，使用默认的配置即可。配置是: config/elasticsearch.yml
      如果需要设置数据的过期时间，可以加上这两行（目测是这样配的，没有验证过，读者可以试一下）：
#设置为30天过期
indices.cache.filter.expire: 30d
index.cache.filter: 30d
      Elasticsearch默认监听在9200端口，可对其进行查询和管理，例如看索引的健康状态：
curl 'localhost:9200/_cluster/health?level=indices&pretty'
      输出
![复制代码](https://common.cnblogs.com/images/copycode.gif)
{
  "cluster_name" : "elasticsearch",
  "status" : "yellow",
  "timed_out" : false,
  "number_of_nodes" : 2,
  "number_of_data_nodes" : 1,
  "active_primary_shards" : 161,
  "active_shards" : 161,
  "relocating_shards" : 0,
  "initializing_shards" : 0,
  "unassigned_shards" : 161,
  "number_of_pending_tasks" : 0,
  "indices" : {
    "logstash-2015.05.05" : {
      "status" : "yellow", #有三级，green, yellow和red
      "number_of_shards" : 5,
      "number_of_replicas" : 1,
      "active_primary_shards" : 5,
      "active_shards" : 5,
      "relocating_shards" : 0,
      "initializing_shards" : 0,
      "unassigned_shards" : 5
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
      可在浏览器进行访问，例如查询一下使用chrome浏览器情况：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201132712241-629023571.png)
### 2. kibana
      这个也不用配置，默认监听在5601端口。
#让它运行在后台
localhost# nohup bin/kibana &
      注意以上两个要使用Java 7以上版本，还有小版本要求，下一个最新的Java 8即可，然后在启动脚本里export JAVA_HOME;
### 三、可视化数据分析
      访问5601端口，如localhost:5601，打开kibana
### 1. 生成索引名称
      第一次访问kibana会重定向到设置索引的页面：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201133108100-388782651.png)
      在蓝色方框里输入要进行探索的索引，如果输入logstash-*就是匹配所有的索引，还可以指定日期，logstash的索引是按日期区分的，一个日期一个文件夹
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201133215100-2006008974.png)
      因此也可以输入logstash-2015.05*，那么如果用这个索引名称，在接下来的操作都是针对5月份的记录。也可以再一个六月份的，在接下来的过程中在网页左上角索引那里随时进行切换。
### 2. 按天查看上月份的访问量
      点左上角的discover
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201133409241-1586331349.png)
      再点右上角的时钟，选时间
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201133454991-397355498.png)
      有三种格式的时间可以选择，我们选择上一个月，就可以看到上月份的访问情况：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201133606350-948441114.png)
### 3. 按地域访问情况
      点击上面菜单栏的visualize，选择最后一个条形图，再选择from a new search
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201133707413-1238867299.png)
      在左边的窗口里选择X-Axis，然后，Aggregation选择Date Histogram，Interval选择Daily，以天为单位
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201133818257-1218728752.png)![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201133829272-227801615.png)
      再点击上图的绿色箭头，右边出来日访问量：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201134046850-1978692329.png)
      接着点击左边的Add Sub-buckets，选择split bars，然后Sub Aggression选择Terms，相应的Field选择city_name，如下图：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201134236194-944030802.png)
      点绿色的播放按钮，右边出来按照每天最高访问量的5个城市：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201135028835-1732502303.png)
      然后把它保存到Dashboard里，等会还要进行比较，点击右上角的save按钮，再起个名字
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201135143897-18886478.png)![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201135158679-1278971092.png)
      点击上面菜单栏的Dashboard，将刚刚保存的加进去。
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201135420444-1977675531.png)
      接下来我们看一下，这几个城市的人分别访问的哪些接口比较多，可能可以看出不同地域的人对什么感兴趣
### 4. 查看不同城市的人访问接口的异同
      点击Visulize（点两次），选择倒数第三个的饼图，接下来的操作类型于上面，按回车：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201135549397-353244934.png)
      可以看出五月份的访问量，北京最多，然后就是长沙、天津、河北。
      接着，Add sub-buckets，如下配置
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201140159116-1389092489.png)
      可以看出，北京访问的接口从高到低今次是getdataversion、getthomeinfo、getactivitys等。然后继续保存到Dashboard。
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201140321022-663308697.png)
      再看下访问比较活跃的是哪几个IP
### 5. 查看访问量最高的几个IP
      继续点visualize，选择data table，相应的配置类似于上面，返回一个前20个最活跃的IP的表格：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201140532507-414502605.png)
      保存到面板
      然后再看下5月份，总的访问量
### 6. 总点击量
      在visualize那里选择metric，总的访问量为：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201140641038-147449323.png)
      还可以看下在地图上的分布
### 7. 地图分布
      在visualize选择Title map
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201140744163-1415034453.png)
      可以看出访问量主要集中在北京一带。
      同样保存到面板，现在在面板保存了5张图：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201140947147-1715747494.png)
### 8. Dashboard的相关联操作
      Dashboard的一个强大之处是可以进行关联查看。例如说要看一下最活跃的IP，是什么时候访问的，访问了哪些接口。
      只需要在上图左下角的clientIP点一下相应的IP即可，面板里的其它各图都会相应地变化。
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201141249038-1138476817.png)9. 搜索
      kibana每个页面都有一个搜索的输入框，进行的任何可视化都可以通过搜索动态地改变.
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201141454569-1739177503.png)
      kibana的搜索使用[Luence语法](https://lucene.apache.org/core/2_9_4/queryparsersyntax.html)，常用的可能就以下几个
（1）字段名:值
      点击上面菜单的Discover，在搜索那里输入: clientIp:123.125.40.1，就可以调出这个IP，该段时间内所进行的所有访问，然后再进行后续其它的各种操作。
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201141918538-2145971581.png)
     例如再看下他使用什么设备访问，在左边侧栏点击userDevice.os_name，再点Visualize     
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201142028710-563855974.png)
可以看出他使用的是ios系统比较多：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201142123335-1087616318.png)
（2）To 范围
      字段是Number型的支持范围搜索，例如看下响应时间在1000ms以上的。搜索duringTime:[1000 TO 1000000000]
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201142240085-1097920904.png)
      看下是哪些接口，设置y轴为平均响应时间，就可以看出哪些接口的调用比较耗时：
![](https://images2015.cnblogs.com/blog/818663/201602/818663-20160201142644022-1342581688.png)
（3）支持and or ()的组合，这里不再说明。
      这里只是根据自已的日志进行一个分析，读者可根据自己的数据内容做相关的挖掘
      个人博客：[http://yincheng.site/logstash](http://yincheng.site/logstash)
参考：
1. [https://www.elastic.co/](https://www.elastic.co/) logstash官网
