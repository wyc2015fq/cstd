# Logstash处理json格式日志文件的三种方法 - 零度的博客专栏 - CSDN博客
2018年07月02日 15:36:30[零度anngle](https://me.csdn.net/zmx729618)阅读数：2390
假设日志文件中的每一行记录格式为json的，如：
`{"Method":"JSAPI.JSTicket","Message":"JSTicket:kgt8ON7yVITDhtdwci0qeZg4L-Dj1O5WF42Nog47n_0aGF4WPJDIF2UA9MeS8GzLe6MPjyp2WlzvsL0nlvkohw","CreateTime":"2015/10/13 9:39:59","AppGUID":"cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d","_PartitionKey":"cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d","_RowKey":"1444700398710_ad4d33ce-a9d9-4d11-932e-e2ccebdb726c","_UnixTS":1444700398710}`
默认配置下，logstash处理插入进elasticsearch后，查到的结果是这样的：
```
1 {
 2     "_index": "logstash-2015.10.16",
 3     "_type": "voip_feedback",
 4     "_id": "sheE9eXiQASMDVtRJ0EYcg",
 5     "_version": 1,
 6     "found": true,
 7     "_source": {
 8         "message": "{\"Method\":\"JSAPI.JSTicket\",\"Message\":\"JSTicket:kgt8ON7yVITDhtdwci0qeZg4L-Dj1O5WF42Nog47n_0aGF4WPJDIF2UA9MeS8GzLe6MPjyp2WlzvsL0nlvkohw\",\"CreateTime\":\"2015/10/13 9:39:59\",\"AppGUID\":\"cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d\",\"_PartitionKey\":\"cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d\",\"_RowKey\":\"1444700398710_ad4d33ce-a9d9-4d11-932e-e2ccebdb726c\",\"_UnixTS\":1444700398710}",
 9         "@version": "1",
10         "@timestamp": "2015-10-16T00:39:51.252Z",
11         "type": "voip_feedback",
12         "host": "ipphone",
13         "path": "/usr1/data/voip_feedback.txt"
14     }
15 }
```
即会将json记录做为一个字符串放到”message”下，但是我是想让logstash自动解析json记录，将各字段放入elasticsearch中。有三种配置方式可以实现。
### 第一种，直接设置format => json
```
1     file {
2         type => "voip_feedback"
3         path => ["/usr1/data/voip_feedback.txt"]  
4         format => json
5         sincedb_path => "/home/jfy/soft/logstash-1.4.2/voip_feedback.access"     
6     }
```
这种方式查询出的结果是：
```
1 {
 2     "_index": "logstash-2015.10.16",
 3     "_type": "voip_feedback",
 4     "_id": "NrNX8HrxSzCvLl4ilKeyCQ",
 5     "_version": 1,
 6     "found": true,
 7     "_source": {
 8         "Method": "JSAPI.JSTicket",
 9         "Message": "JSTicket:kgt8ON7yVITDhtdwci0qeZg4L-Dj1O5WF42Nog47n_0aGF4WPJDIF2UA9MeS8GzLe6MPjyp2WlzvsL0nlvkohw",
10         "CreateTime": "2015/10/13 9:39:59",
11         "AppGUID": "cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d",
12         "_PartitionKey": "cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d",
13         "_RowKey": "1444700398710_ad4d33ce-a9d9-4d11-932e-e2ccebdb726c",
14         "_UnixTS": 1444700398710,
15         "@version": "1",
16         "@timestamp": "2015-10-16T00:16:11.455Z",
17         "type": "voip_feedback",
18         "host": "ipphone",
19         "path": "/usr1/data/voip_feedback.txt"
20     }
21 }
```
可以看到，json记录已经被直接解析成各字段放入到了_source中，但是原始记录内容没有被保存
### 第二种，使用codec => json
```
1 file {
2         type => "voip_feedback"
3         path => ["/usr1/data/voip_feedback.txt"]  
4         sincedb_path => "/home/jfy/soft/logstash-1.4.2/voip_feedback.access"
5         codec => json {
6             charset => "UTF-8"
7         }       
8     }
```
这种方式查询出的结果与第一种一样，字段被解析，原始记录内容也没有保存
### 第三种，使用filter json
```
1 filter {
2     if [type] == "voip_feedback" {
3         json {
4             source => "message"
5             #target => "doc"
6             #remove_field => ["message"]
7         }        
8     }
9 }
```
这种方式查询出的结果是这样的：
```
1 {
 2     "_index": "logstash-2015.10.16",
 3     "_type": "voip_feedback",
 4     "_id": "CUtesLCETAqhX73NKXZfug",
 5     "_version": 1,
 6     "found": true,
 7     "_source": {
 8         "message": "{\"Method222\":\"JSAPI.JSTicket\",\"Message\":\"JSTicket:kgt8ON7yVITDhtdwci0qeZg4L-Dj1O5WF42Nog47n_0aGF4WPJDIF2UA9MeS8GzLe6MPjyp2WlzvsL0nlvkohw\",\"CreateTime\":\"2015/10/13 9:39:59\",\"AppGUID\":\"cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d\",\"_PartitionKey\":\"cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d\",\"_RowKey\":\"1444700398710_ad4d33ce-a9d9-4d11-932e-e2ccebdb726c\",\"_UnixTS\":1444700398710}",
 9         "@version": "1",
10         "@timestamp": "2015-10-16T00:28:20.018Z",
11         "type": "voip_feedback",
12         "host": "ipphone",
13         "path": "/usr1/data/voip_feedback.txt",
14         "Method222": "JSAPI.JSTicket",
15         "Message": "JSTicket:kgt8ON7yVITDhtdwci0qeZg4L-Dj1O5WF42Nog47n_0aGF4WPJDIF2UA9MeS8GzLe6MPjyp2WlzvsL0nlvkohw",
16         "CreateTime": "2015/10/13 9:39:59",
17         "AppGUID": "cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d",
18         "_PartitionKey": "cb54ba2d-1d38-45f2-9ed1-abff0bf7dd3d",
19         "_RowKey": "1444700398710_ad4d33ce-a9d9-4d11-932e-e2ccebdb726c",
20         "_UnixTS": 1444700398710,
21         "tags": [
22             "111",
23             "222"
24         ]
25     }
26 }
```
可以看到，原始记录被保存，同时字段也被解析保存。如果确认不需要保存原始记录内容，可以加设置：remove_field => [“message”]
**比较以上三种方法，最方便直接的就是在file中设置format => json**
另外需要注意的是，logstash会在向es插入数据时默认会在_source下增加type,host,path三个字段，如果json内容中本身也含有type,host,path字段，那么解析后将覆盖掉logstash默认的这三个字段，尤其是type字段，这个同时也是做为index/type用的，覆盖掉后，插入进es中的index/type就是json数据记录中的内容，将不再是logstash config中配置的type值。
这时需要设置filter.json.target，设置该字段后json原始内容将不会放在_source下，而是放到设置的”doc”下：
```
1 {
 2     "_index": "logstash-2015.10.20",
 3     "_type": "3alogic_log",
 4     "_id": "xfj3ngd5S3iH2YABjyU6EA",
 5     "_version": 1,
 6     "found": true,
 7     "_source": {
 8         "@version": "1",
 9         "@timestamp": "2015-10-20T11:36:24.503Z",
10         "type": "3alogic_log",
11         "host": "server114",
12         "path": "/usr1/app/log/mysql_3alogic_log.log",
13         "doc": {
14             "id": 633796,
15             "identity": "13413602120",
16             "type": "EAP_TYPE_PEAP",
17             "apmac": "88-25-93-4E-1F-96",
18             "usermac": "00-65-E0-31-62-5D",
19             "time": "20151020-193624",
20             "apmaccompany": "TP-LINK TECHNOLOGIES CO.,LTD",
21             "usermaccompany": ""
22         }
23     }
24 }
```
这样就不会覆盖掉_source下的type,host,path值 
而且在kibana中显示时字段名称为doc.type,doc.id…
```bash
补充: 无法解析的json不记录到elasticsearch中
```
```
output {
      stdout{
    codec => rubydebug
    }
#无法解析的json不记录到elasticsearch中
if "_jsonparsefailure" not in [tags] {
  elasticsearch {
    host => "localhost"
  }
}
```
转载自：[http://blog.csdn.net//jiao_fuyou/article/details/49174269](http://blog.csdn.net//jiao_fuyou/article/details/49174269)
由于自己的项目只处理JSON字符串的日志,网上搜集资料过程中，还找到了一些对于系统日志类型以及普通打印类型字符串的日志格式处理，留下连接以后有需要参考。
[logstash使用grok正则解析日志和kibana遇到的问题](http://xiaorui.cc/2015/01/27/logstash%E4%BD%BF%E7%94%A8grok%E6%AD%A3%E5%88%99%E8%A7%A3%E6%9E%90%E6%97%A5%E5%BF%97%E9%81%87%E5%88%B0%E7%9A%84%E9%97%AE%E9%A2%98/)
[logstash快速入门](http://www.2cto.com/os/201411/352015.html)
[logstash使用grok正则解析日志](http://blog.csdn.net/earbao/article/details/49306465)
[http://udn.yyuap.com/doc/logstash-best-practice-cn/filter/grok.html](http://udn.yyuap.com/doc/logstash-best-practice-cn/filter/grok.html)
[https://github.com/elastic/logstash/blob/v1.1.9/patterns/grok-patterns](https://github.com/elastic/logstash/blob/v1.1.9/patterns/grok-patterns)[https://www.elastic.co/guide/en/logstash/current/plugins-filters-grok.html](https://www.elastic.co/guide/en/logstash/current/plugins-filters-grok.html)
[logstash配置语言基础](http://hxw168.blog.51cto.com/8718136/1589540)[使用mutate处理数据](http://hxw168.blog.51cto.com/8718136/1589498)[使用grok提取信息](http://hxw168.blog.51cto.com/8718136/1588477)
[使用logstash内置patterns或自定义patterns同时收集linux系统日志、web日志及项目自定义格式日志](http://www.run-debug.com/?p=633)
[logstash+grok+json+elasticsearch解析复杂日志数据(一)](http://blog.csdn.net/u014297722/article/details/52823964)
还有另外一种思路：使用索引模板
[Elasticsearch之索引模板index template与索引别名index alias](http://www.cnblogs.com/zlslch/p/6478168.html)
[如何为logstash+elasticsearch配置索引模板?](https://sanwen8.cn/p/1a3FWXm.html)
[logstash 向elasticsearch写入数据，如何指定多个数据template](http://blog.csdn.net/zhifeiyu2008/article/details/47720847)
[Elasticsearch——Templates 模板](http://blog.csdn.net/july_2/article/details/27551739)
