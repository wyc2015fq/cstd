# logstash - 输出插件之redis与es - sxf_123456的博客 - CSDN博客
2017年09月05日 21:40:13[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：339
logstash - 输出插件之redis与es
第二章 logstash - 输出插件之redis与es
最常用的两个输出插件：
redis
es
一、redis
1、用法
复制代码
 1 output {
 2     redis{
 3         batch => false
 4         batch_events => 50
 5         batch_timeout => 5
 6         codec => plain
 7         congestion_interval => 1
 8         congestion_threshold => 0
 9         data_type => list
10         db => 0
11         host => ["127.0.0.1:6379"]
12         key => xxx
13         password => xxx
14         port => 6379
15         reconnect_interval => 1
16         shuffle_hosts => true
17         timeout => 5
18         workers => 1
19     }
20 }
复制代码
2、配置项
以上所有配置项都是可选的，没有必须的。（以下4个红色配置是最重要的4个配置）
批处理类（仅用于data_type为list）
batch：设为true，通过发送一条rpush命令，存储一批的数据
默认为false：1条rpush命令，存储1条数据
设为true之后，1条rpush会发送batch_events条数据或发送batch_timeout秒（取决于哪一个先到达）
batch_events：一次rpush多少条
默认50条
batch_timeout：一次rpush最多消耗多少s
默认5s
编码类
codec：对输出数据进行codec，避免使用logstash的separate filter
拥塞保护（仅用于data_type为list）
congestion_interval：每多长时间进行一次拥塞检查
默认1s
设为0，表示对每rpush一个，都进行检测
congestion_threshold：list中最多可以存在多少个item数据
默认是0：表示禁用拥塞检测
当list中的数据量达到congestion_threshold，会阻塞直到有其他消费者消费list中的数据
作用：防止OOM
data_type
list：使用rpush
channel：使用publish
db：使用redis的数据库，默认使用0号
host：数组
eg.["127.0.0.1:6380", "127.0.0.1"]
可以指定port，会覆盖全局port
port：全局port，默认6379
key：list或channel的名字
支持动态key，例如：logstash-%{type}
password：redis密码，默认不使用密码
reconnect_interval：失败重连的间隔，默认为1s
timeout：连接超时，默认5s
二、es
1、使用方式
复制代码
 1 output {
 2     elasticsearch {
 3         hosts => ["127.0.0.1:9200"] 
 4         action => index
 5         cacert => /xxx
 6         codec => plain
 7         doc_as_upsert => false
 8         document_id => 1
 9         document_type => xxx
10         flush_size => 500
11         idle_flush_time => 1
12         index => logstash-%{+YYYY.MM.dd}
13         keystore => /xxx
14         keystore_password => xxx
15         manage_template => true
16         max_retries => 3
17         parent => nil
18         password => xxx
19         path => /
20         proxy => xxx
21         retry_max_interval => 2
22         routing => xxx
23         script => xxx
24         script_lang => xxx
25         script_type => inline
26         script_var_name => event
27         scripted_upsert => false
28         sniffing => false
29         sniffing_delay => 5
30         ssl => false
31         ssl_certificate_verification => true
32         template => /xxx
33         template_name => logstash
34         template_overwrite => false
35         timeout => 5
36         truststore => /xxx
37         truststore_password => xxx
38         upsert => xxx
39         user => xxx
40         workers => 1
41     }
42 }
复制代码
2、基本配置
以上配置全部都是可选的，没有必须的。以下列出最重要的几个。
hosts：["127.0.0.1:9200","127.0.0.2:9200"]
action：指定es的行为，index, delete, create, update
默认为index：index a document（该document就是一个来自于logstash的event）
delete：通过id删除一个document（需要指定document_id）
create：index a document（如果该document已经在index中存在，则失败）
update：通过id更新一个document
cacert：验证server合法性的.cer或.pem文件路径
codec：
document_id
document_type
index：默认值：logstash-%{+YYYY.MM.dd}
便于删除老数据
在语法解析的时候，看到+号开头的，会自动认为后面是时间格式，尝试用时间格式来解析后续字符串。所以，之前处理过程中不要给自定义的字段起一个+号开头的名字
索引名中不能有大写字母
有时也会自定义为：logstash-%{servicename}-%{+YYYY.MM.dd}
user：进入es cluster的用户
password：进入es cluster的密码
timeout：Set the timeout for network operations and requests sent Elasticsearch. If a timeout occurs, the request will be retried.
flush_size：默认500，logstash攒够500条数据再一次性向es发送
idle_flush_time：默认1s，如果1s内没攒够500条还是会一次性将攒的数据发出去给es
