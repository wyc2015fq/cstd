# ELK 之 Logstash - sxf_123456的博客 - CSDN博客
2017年12月20日 16:27:30[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：141
## 简介:
ELK 之 LogstashLogstash 是一个接收,处理,转发日志的工具。支持系统日志,webserver 日志,错误日志,应用日志,总之包括所有可以抛出来的日志类型。在一个典型的使用场景下(ELK):用 Elasticsearch 作为后台数据的存储,kibana用来前端的报表展示。Logstash 在其过程中担任搬运工的角色,它为数据存储,报表查询和日志解析创建了一个功能强大的管道链。Logstash 提供了多种多样的 input,filters,codecs 和 output 组件,让使用者轻松实现强大的功能。
## 安装:
(需要 jdk 环境,安装过程这里不再阐述,笔者此处使用 jdk 1.8) 
这里使用 2.4.1 版本,是为了和公司 elasticsearch2.x 配合,版本自行控制。 
注意: ELK 技术栈有 version check,软件大版本号需要一致
`yum -y install https://download.elastic.co/logstash/logstash/packages/centos/logstash-2.4.1.noarch.rpm`- 1
安装完成后会生成两个主要目录和一个配置文件 
程序主体目录:/opt/logstash
log 分析配置文件目录:/etc/logstash/conf.d
程序运行配置文件:/etc/sysconfig/logstash
先测试是否安装成功
```php
[root@~]#/opt/logstash/bin/logstash -e 'input{stdin{}}output{stdout{codec=>rubydebug}}'
Settings: Default pipeline workers: 4
Pipeline main started
hello world! # 输入测试字符串
{
"message" => "hello world!",  # 成功输出
"@version" => "1",
"@timestamp" => "2017-08-07T07:47:35.938Z",
"host" => "iZbp13lsytivlvvks4ulatZ"
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
如何执行按指定配置文件执行
`/opt/logstash/bin/logstash –w 2 -f /etc/logstash/conf.d/test.conf`- 1
### 参数
-w # 指定线程,默认是 cpu 核数 
-f # 指定配置文件 
-t # 测试配置文件是否正常 
-b # 执行 filter 模块之前最大能积累的日志,数值越大性能越好,同时越占内 
存
### 配置文件写法:
```
# 日志导入
input {
}
# 日志筛选匹配处理
filter {
}
# 日志匹配输出
output {
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
日志解析配置文件的框架共分为三个模块,input,output,filter。后面会一一讲解, 每个模块里面存在不同的插件。
## input 模块
#### 列子1
```php
input {
    # file为常用文件插件，插件内选项很多，可根据需求自行判断
    file {
        path => "/var/lib/mysql/slow.log"
        # 要导入的文件的位置，可以使用*，例如/var/log/nginx/*.log
        Excude =>”*.gz”
        # 要排除的文件
        start_position => "beginning"
        # 从文件开始的位置开始读,end表示从结尾开始读
        ignore_older => 0  
        # 多久之内没修改过的文件不读取，0为无限制，单位为秒
        sincedb_path => "/dev/null"
        # 记录文件上次读取位置，输出到null表示每次都从文件首行开始解析
        type => "mysql-slow"
        # type字段，可表明导入的日志类型
    }   
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
- 14
- 15
- 16
- 17
#### 例子2
```php
input {
    # redis插件为常用插件，插件内选项很多，可根据需求自行判断
    redis {
        batch_count => 1 
        # EVAL命令返回的事件数目，设置为5表示一次请求返回5条日志信息
        data_type => "list" 
        # logstash redis插件工作方式
        key => "logstash-test-list" 
        # 监听的键值
        host => "127.0.0.1" 
        # redis地址
        port => 6379 
        # redis端口号
        password => "123qwe" 
        # 如果有安全认证，此项为认证密码
        db => 0 
        # 如果应用使用了不同的数据库，此为redis数据库的编号，默认为0。
        threads => 1 
        # 启用线程数量
      }
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
常用的 input 插件其实有很多,这里只举例了两种。其他还有 kafka,tcp 等等
## filter 模块
#### 例子
```php
filter {  # 插件很多，这里选取我使用过的插件做讲述
    if ([message] =~ "正则表达式")  {  drop {}  }
    # 正则匹配=~,!~,包含判断in,not in ，字符串匹配==,!=,等等，匹配之后可以做任何操作，这里过滤掉匹配行，除了做过滤操作，if后面可以作任意操作，甚至可以为匹配到的任意行做单独的正则分割操作
    multiline {
        pattern => "正则表达式"
        negate => true
        what => "previous"
        # 多行合并，由于一些日志存在一条多行的情况，这个模块可以进行指定多行合并，通过正则匹配，匹配到的内容上面的多行合并为一条日志。
    }   
    grok {
        match => { "message" => "正则表达式"
         # 正则匹配日志，可以筛选分割出需要记录的字段和值
        }   
        remove_field => ["message"]
        # 删除不需要记录的字段
   }   
    date {
        match => ["timestamp","dd/MMM/yyyy:HH:mm:ss Z"] 
       # 记录@timestamp时间，可以设置日志中自定的时间字段，如果日志中没有时间字段，也可以自己生成
        target=>“@timestamp”
        # 将匹配的timestamp字段放在指定的字段 默认是@timestamp
    }
    ruby {
        code => "event.timestamp.time.localtime"
        # timestamp时区锁定
    }   
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
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
## output 模块
#### 例子1
```php
output {
    # tdout { codec => "rubydebug" }
    # 筛选过滤后的内容输出到终端显示
    elasticsearch {  # 导出到es，最常用的插件
        codec => "json"
        # 导出格式为json
        hosts => ["127.0.0.1:9200"]
        # ES地址+端口
        index => "logstash-slow-%{+YYYY.MM.dd}"
        # 导出到index内，可以使用时间变量
        user => "admin"
        password => "xxxxxx"
        # ES如果有安全认证就使用账号密码验证，无安全认证就不需要
        flush_size => 500
        # 默认500，logstash一次性攒够500条的数据在向es发送
        idle_flush_time => 1
        # 默认1s，如果1s内没攒够500，还是会一次性把数据发给ES
    }   
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
#### 例子2
```php
output {
     redis{  # 输出到redis的插件，下面选项根据需求使用
         batch => true
         # 设为false，一次rpush，发一条数据，true为发送一批
         batch_events => 50
         # 一次rpush发送多少数据
         batch_timeout => 5
         # 一次rpush消耗多少时间
         codec => plain
         # 对输出数据进行codec，避免使用logstash的separate filter
         congestion_interval => 1
         # 多长时间进项一次拥塞检查
         congestion_threshold => 5
         # 限制一个list中可以存在多少个item，当数量足够时，就会阻塞直到有其他消费者消费list中的数据
         data_type => list
         # 使用list还是publish
         db => 0
         # 使用redis的那个数据库，默认为0号
         host => ["127.0.0.1:6379"]
         # redis 的地址和端口，会覆盖全局端口
         key => xxx
         # list或channel的名字
         password => xxx
         # redis的密码，默认不使用
         port => 6379
         # 全局端口，默认6379，如果host已指定，本条失效
         reconnect_interval => 1
         # 失败重连的间隔，默认为1s
         timeout => 5
         # 连接超时的时间
         workers => 1
         # 工作进程
     }
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
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
常用插件还有很多,更多的插件使用可以查看官方文档 
通过上面的介绍,我们大体知道了 logstash 的处理流程: 
input => filter => output
接下来就看一完整的应用例子
## 完整的应用:
#### Elasticsearch slow-log
```
input {
    file {
        path => ["/var/log/elasticsearch/private_test_index_search_slowlog.log"]
        start_position => "beginning"
        ignore_older => 0
        # sincedb_path => "/dev/null"
        type => "elasticsearch_slow"
        }   
}
filter {
    grok {
        match =>  { "message" => "^\[(\d\d){1,2}-(?:0[1-9]|1[0-2])-(?:(?:0[1-9])|(?:[12][0-9])|(?:3[01])|[1-9])\s+(?:2[0123]|[01]?[0-9]):(?:[0-5][0-9]):(?:(?:[0-5]?[0-9]|60)(?:[:.,][0-9]+)?)\]\[(TRACE|DEBUG|WARN\s|INFO\s)\]\[(?<io_type>[a-z\.]+)\]\s\[(?<node>[a-z0-9\-\.]+)\]\s\[(?<index>[A-Za-z0-9\.\_\-]+)\]\[\d+\]\s+took\[(?<took_time>[\.\d]+(ms|s|m))\]\,\s+took_millis\[(\d)+\]\,\s+types\[(?<types>([A-Za-z\_]+|[A-Za-z\_]*))\]\,\s+stats\[\]\,\s+search_type\[(?<search_type>[A-Z\_]+)\]\,\s+total_shards\[\d+\]\,\s+source\[(?<source>[\s\S]+)\]\,\s+extra_source\[[\s\S]*\]\,\s*$" }
        remove_field => ["message"]
        }   
    date {
        match => ["timestamp","dd/MMM/yyyy:HH:mm:ss Z"] 
        }   
    ruby {
        code => "event.timestamp.time.localtime"
        }   
    }
output {
     elasticsearch {
         codec => "json"
         hosts => ["127.0.0.1:9200"]
         index => "logstash-elasticsearch-slow-%{+YYYY.MM.dd}"
         user => "admin"
         password => "xxxx"
    }   
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
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
#### Mysql-slow log
```php
input {
    file {
        path => "/var/lib/mysql/slow.log"
        start_position => "beginning"
        ignore_older => 0
        # sincedb_path => "/dev/null"
        type => "mysql-slow"
    }   
}
filter {
    if ([message] =~ "^(\/usr\/local|Tcp|Time)[\s\S]*") { drop {} }
    multiline {
        pattern => "^\#\s+Time\:\s+\d+\s+(0[1-9]|[12][0-9]|3[01]|[1-9])"
        negate => true
        what => "previous"
    }   
    grok {
        match => { "message" => "^\#\sTime\:\s+\d+\s+(?<datetime>%{TIME})\n+\#\s+User@Host\:\s+[A-Za-z0-9\_]+\[(?<mysql_user>[A-Za-z0-9\_]+)\]\s+@\s+(?<mysql_host>[A-Za-z0-9\_]+)\s+\[\]\n+\#\s+Query\_time\:\s+(?<query_time>[0-9\.]+)\s+Lock\_time\:\s+(?<lock_time>[0-9\.]+)\s+Rows\_sent\:\s+(?<rows_sent>\d+)\s+Rows\_examined\:\s+(?<rows_examined>\d+)(\n+|\n+use\s+(?<dbname>[A-Za-z0-9\_]+)\;\n+)SET\s+timestamp\=\d+\;\n+(?<slow_message>[\s\S]+)$"
   }   
        remove_field => ["message"]
   }   
    date {
        match => ["timestamp","dd/MMM/yyyy:HH:mm:ss Z"] 
    }   
    ruby {
        code => "event.timestamp.time.localtime"
    }   
}
output { 
    elasticsearch {
        codec => "json"
        hosts => ["127.0.0.1:9200"]
        index => "logstash-mysql-slow-%{+YYYY.MM.dd}"
        user => "admin"
        password => "xxxxx"
    }   
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
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
#### Nginx access.log
logstash 中内置 nginx 的正则,我们只要稍作修改就能使用 
将下面的内容写入到/opt/logstash/vendor/bundle/jruby/1.9/gems/logstash- 
patterns-core-2.0.5/patterns/grok-patterns 文件中
```
X_FOR (%{IPV4}|-)
NGINXACCESS %{COMBINEDAPACHELOG} \"%{X_FOR:http_x_forwarded_for}\"
ERRORDATE %{YEAR}/%{MONTHNUM}/%{MONTHDAY} %{TIME}
NGINXERROR_ERROR %{ERRORDATE:timestamp}\s{1,}\[%{DATA:err_severity}\]\s{1,}(%{NUMBER:pid:int}#%{NUMBER}:\s{1,}\*%{NUMBER}|\*%{NUMBER}) %{DATA:err_message}(?:,\s{1,}client:\s{1,}(?<client_ip>%{IP}|%{HOSTNAME}))(?:,\s{1,}server:\s{1,}%{IPORHOST:server})(?:, request: %{QS:request})?(?:, host: %{QS:server_ip})?(?:, referrer:\"%{URI:referrer})?
NGINXERROR_OTHER %{ERRORDATE:timestamp}\s{1,}\[%{DATA:err_severity}\]\s{1,}%{GREEDYDATA:err_message}
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
之后的 log 配置文件如下
```php
input {
    file {
    path => [ "/var/log/nginx/www-access.log" ]
    start_position => "beginning"
    # sincedb_path => "/dev/null"
    type => "nginx_access"
    }   
}
filter {
    grok {
         match => { "message" => "%{NGINXACCESS}"}
    }
    mutate {
        convert => [ "response","integer" ]
        convert => [ "bytes","integer" ]
    }
    date {
        match => [ "timestamp","dd/MMM/yyyy:HH:mm:ss Z"]
    }   
    ruby {
        code => "event.timestamp.time.localtime"
    }   
}
output {
    elasticsearch {
        codec => "json"
        hosts => ["127.0.0.1:9200"]
        index => "logstash-nginx-access-%{+YYYY.MM.dd}"
        user => "admin"
        password => "xxxx"
    }
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
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
#### Nginx error.log
```php
input {
    file {
    path => [ "/var/log/nginx/www-error.log" ]
    start_position => "beginning"
    # sincedb_path => "/dev/null"
    type => "nginx_error"
    }
}
filter {
    grok {
        match => [
                   "message","%{NGINXERROR_ERROR}",
                   "message","%{NGINXERROR_OTHER}"
                 ]
    }   
    ruby {
        code => "event.timestamp.time.localtime"
    }   
     date {
         match => [ "timestamp","dd/MMM/yyyy:HH:mm:ss"]
     } 
}
output {
    elasticsearch {
        codec => "json"
        hosts => ["127.0.0.1:9200"]
        index => "logstash-nginx-error-%{+YYYY.MM.dd}"
        user => "admin"
        password => "xxxx"
    }   
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
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
#### PHP error.log
```php
input {
    file {
        path => ["/var/log/php/error.log"]
        start_position => "beginning"
        # sincedb_path => "/dev/null"
        type => "php-fpm_error"
    }   
}
filter {
    multiline {
        pattern => "^\[(0[1-9]|[12][0-9]|3[01]|[1-9])\-%{MONTH}-%{YEAR}[\s\S]+"
        negate => true
        what => "previous"
    }   
    grok {
        match => { "message" => "^\[(?<timestamp>(0[1-9]|[12][0-9]|3[01]|[1-9])\-%{MONTH}-%{YEAR}\s+%{TIME}?)\s+[A-Za-z]+\/[A-Za-z]+\]\s+(?<category>(?:[A-Z]{3}\s+[A-Z]{1}[a-z]{5,7}|[A-Z]{3}\s+[A-Z]{1}[a-z\s]{9,11}))\:\s+(?<error_message>[\s\S]+$)" }
        remove_field => ["message"]
    }   
    date {
        match => ["timestamp","dd/MMM/yyyy:HH:mm:ss Z"] 
    }   
    ruby {
        code => "event.timestamp.time.localtime"
    }   
}
output {
    elasticsearch {
        codec => "json"
        hosts => ["127.0.0.1:9200"]
        index => "logstash-php-error-%{+YYYY.MM.dd}"
        user => "admin"
        password => "xxxxx"
    }   
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
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
#### Php-fpm slow-log
```php
input {
    file {
        path => ["/var/log/php-fpm/www.slow.log"]
        start_position => "beginning"
        # sincedb_path => "/dev/null"
        type => "php-fpm_slow"
    }   
}
filter {
    multiline {
        pattern => "^$"
        negate => true
        what => "previous"
    }   
    grok {
        match => { "message" => "^\[(?<timestamp>(0[1-9]|[12][0-9]|3[01]|[1-9])\-%{MONTH}-%{YEAR}\s+%{TIME})\]\s+\[[a-z]{4}\s+(?<pool>[A-Za-z0-9]{1,8})\]\s+[a-z]{3}\s+(?<pid>\d{1,7})\n(?<slow_message>[\s\S]+$)" }
        remove_field => ["message"]
    }   
    date {
        match => ["timestamp","dd-MMM-yyyy:HH:mm:ss Z"] 
    }   
    ruby {
        code => "event.timestamp.time.localtime"
    }   
}
output {
    elasticsearch {
        codec => "json"
        hosts => ["127.0.0.1:9200"]
        index => "logstash-php-fpm-slow-%{+YYYY.MM.dd}"
        user => "admin"
        password => "xxxx"
    }   
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
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
log 解析配置文件统一放在/etc/logstash/conf.d 目录下,不过也可以任意放 
置,统一起来最好。 
在多个配置文件的时候,不能使用如下命令运行logstash:
```bash
/opt/logstash/bin/logstash -f /etc/logstash/conf.d/(或者有个*)
```
- 1
这个命令会拼接配置文件,不会单个使用,会报错。 
如果有多个配置文件,就一个一个启动:
`/opt/logstash/bin/logstash -f /etc/logstash/conf.d/nginx_error.conf`- 1
但是这样也很麻烦,如果配置文件很多的情况下需要一个个来,并且启动 
速度还很慢,所以我写了一个测试脚本用来方便使用，仅供参考：
```bash
#!/bin/bash
conf_path=/etc/logstash/conf.d  
# /配置文件存放目录根据需求自己更改
conf_name=$( ls ${conf_path} ) 
case $1 in
start)
    echo "-----------please wait.----------"
    echo "The start-up process is too slow."
    for cf in ${conf_name}
    do  
        /opt/logstash/bin/logstash  -f $conf_path/$cf > /dev/null 2>&1 &
            if [ $? -ne 0 ];then
                echo 'The '${cf}' start-up failed.'
            fi
        sleep 20
    done
    echo "start-up success."
;;    
stop)
    ps -ef |grep logstash |grep -v grep > /dev/null 2>&1 
    if [ $? -eq 0 ];then
        ps -ef|grep logstash |grep -v grep |awk '{print $2}'|xargs  kill -9 > /dev/null 2>&1
        sleep 2
        echo "Stop success."
    fi  
;;
restart)
    ps -ef |grep logstash |grep -v grep 2>&1
    if [ $? -eq 0 ];then
        ps -ef|grep logstash |grep -v grep |awk '{print $2}'|xargs  kill -9 > /dev/null 2>&1
        sleep 3
        echo "Stop success."
    fi  
    echo "-----------please wait.----------"
    echo "The start-up process is too slow."
    for cf in ${conf_name}
    do  
        /opt/logstash/bin/logstash  -f $conf_path/$cf > /dev/null 2>&1 &
            if [ $? -ne 0 ];then
                echo 'The '${cf}' start-up failed.'
            fi
        sleep 10
    done 
    echo "start-up success."
;;
*)
    echo "Usage: "$0" {start|stop|restart|}"
    exit 1
esac
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
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
脚本的名字中不要包含 logstash,这里保存为 log_stash.sh 
使用./log_stash.sh (start|stop|restart) 来执行脚本
