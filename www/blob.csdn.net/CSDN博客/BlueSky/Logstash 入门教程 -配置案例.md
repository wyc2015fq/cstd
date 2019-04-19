# Logstash 入门教程 -配置案例 - BlueSky - CSDN博客
2016年04月06日 19:03:08[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：3973
转自：http://corejava2008.iteye.com/blog/2215591
介绍一个详细案例，讲述如何配置读取Apache日志，Syslog并根据自定义条件进行过滤和输出。
**手工输入并解析数据**
LS中可以通过Filter针对数据进行切片切块等操作，解析，装换，组装等等。。
Java代码  ![收藏代码](http://corejava2008.iteye.com/images/icon_star.png)
- input { stdin { } } #控制台输入  
- 
- filter {  
-   grok { #通过GROK来自动解析APACHE日志格式  
-     match => { "message" => "%{COMBINEDAPACHELOG}" }  
-   }  
-   date {#通过Date过滤器来自动识别日期格式。  
-     match => [ "timestamp" , "dd/MMM/yyyy:HH:mm:ss Z" ]  
-   }  
- }  
- 
- output {  
-   elasticsearch { host => localhost } #输出到ES中。  
-   stdout { codec => rubydebug } #输出到控制台  
- }  
启动LS：bin/logstash -f logstash-filter.conf
并在控制台输入：
Java代码  ![收藏代码](http://corejava2008.iteye.com/images/icon_star.png)
- 127.0.0.1 - - [11/Dec/2013:00:01:45 -0800] "GET /xampp/status.php HTTP/1.1"2003891"http://cadenza/xampp/navi.php""Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:25.0) Gecko/20100101 Firefox/25.0"
 可以看到输出结果为：
Java代码  ![收藏代码](http://corejava2008.iteye.com/images/icon_star.png)
- {  
- "message" => "127.0.0.1 - - [11/Dec/2013:00:01:45 -0800] \"GET /xampp/status.php HTTP/1.1\" 200 3891 \"http://cadenza/xampp/navi.php\" \"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:25.0) Gecko/20100101 Firefox/25.0\"",  
- "@timestamp" => "2013-12-11T08:01:45.000Z",  
- "@version" => "1",  
- "host" => "cadenza",  
- "clientip" => "127.0.0.1",  
- "ident" => "-",  
- "auth" => "-",  
- "timestamp" => "11/Dec/2013:00:01:45 -0800",  
- "verb" => "GET",  
- "request" => "/xampp/status.php",  
- "httpversion" => "1.1",  
- "response" => "200",  
- "bytes" => "3891",  
- "referrer" => "\"http://cadenza/xampp/navi.php\"",  
- "agent" => "\"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:25.0) Gecko/20100101 Firefox/25.0\""
- }  
 看到结果了吧，完成了两个操作
1.LS使用grok自动解析了日志格式，
2.针对timestamp进行Date格式识别，并复制给了@timestamp字段。
再进一步，从文件中输入并解析数据
Java代码  ![收藏代码](http://corejava2008.iteye.com/images/icon_star.png)
- input {  
-   file { #通过Input配置，从文件中读取数据。  
-     path => "/tmp/access_log"  #日志文件位置  
-     start_position => "beginning"
-     #是否从头部开始读取。  
-     #Logstash启动后，会在系统中记录一个隐藏文件，记录处理过的行号，  
-     #当进行挂掉，重新启动后，根据该行号记录续读。  
-     #所以start_position只会生效一次。  
-   }  
- }  
- 
- filter {  
- if [path] =~ "access" {#当路径包含access时，才会执行以下处理逻辑  
-     mutate { replace => { "type" => "apache_access" } }  
-     grok {  
-       match => { "message" => "%{COMBINEDAPACHELOG}" }  
-     }  
-   }elseif [path] =~ "error" { #IF-ElSE 配置方式。  
-     mutate { replace => { type => "apache_error" } }  
-     #使用Mutate 替换type的值为“apache_error”  
-   } else {  
-     mutate { replace => { type => "random_logs" } }  
-   }  
-   date {  
-     match => [ "timestamp" , "dd/MMM/yyyy:HH:mm:ss Z" ]  
-   }  
- }  
- 
- output {  
-   elasticsearch {  
-     host => localhost  
-   }  
-   stdout { codec => rubydebug }  
- }  
再来看个案例：
Java代码  ![收藏代码](http://corejava2008.iteye.com/images/icon_star.png)
- output {  
- if [type] == "apache" {  
- if [status] =~ /^5\d\d/ { #如果status状态为5xx，发送给nagios。  
-       nagios { ...  }  
-     } elseif [status] =~ /^4\d\d/ {#如果是4xx，发送到elasticSearch  
-       elasticsearch { ... }  
-     }  
-     statsd { increment => "apache.%{status}" } #所有数据给statsd  
-   }  
- }  
 处理SysLog案例：
Java代码  ![收藏代码](http://corejava2008.iteye.com/images/icon_star.png)
- 处理SysLog案例：  
- input {  
-   tcp {  
-     port => 5000   #读取TCP端口  
-     type => syslog #数据类型  
-   }  
-   udp {  
-     port => 5000   #读取UDP端口  
-     type => syslog #数据类型  
-   }  
- }  
- 
- filter {  
- if [type] == "syslog" {  
-     grok { #解析日志格式。  
-       match => { "message" => "%{SYSLOGTIMESTAMP:syslog_timestamp} %{SYSLOGHOST:syslog_hostname} %{DATA:syslog_program}(?:\[%{POSINT:syslog_pid}\])?: %{GREEDYDATA:syslog_message}" }  
-       add_field => [ "received_at", "%{@timestamp}" ] #添加字段   
-       add_field => [ "received_from", "%{host}" ]     #添加字段  
-     }  
-     syslog_pri { }  
-     date {  #日期格式处理，这里匹配了两种不同的日期格式。  
-       match => [ "syslog_timestamp", "MMM  d HH:mm:ss", "MMM dd HH:mm:ss" ]  
-     }  
-   }  
- }  
- 
- output { #数据输出。  
-   elasticsearch { host => localhost }  
-   stdout { codec => rubydebug }  
- }  
启动服务：
bin/logstash -f logstash-syslog.conf
发送测试数据：
Java代码  ![收藏代码](http://corejava2008.iteye.com/images/icon_star.png)
- telnet localhost 5000
- Dec 2312:11:43 louis postfix/smtpd[31499]: connect from unknown[95.75.93.154]  
- Dec 2314:42:56 louis named[16000]: client 199.48.164.7#64817: query (cache) 'amsterdamboothuren.com/MX/IN' denied  
- Dec 2314:30:01 louis CRON[619]: (www-data) CMD (php /usr/share/cacti/site/poller.php >/dev/null2>/var/log/cacti/poller-error.log)  
- Dec 2218:28:06 louis rsyslogd: [origin software="rsyslogd" swVersion="4.2.0" x-pid="2253" x-info="http://www.rsyslog.com"] rsyslogd was HUPed, type 'lightweight'.  
 查看结果：
Java代码  ![收藏代码](http://corejava2008.iteye.com/images/icon_star.png)
- {  
- "message" => "Dec 23 14:30:01 louis CRON[619]: (www-data) CMD (php /usr/share/cacti/site/poller.php >/dev/null 2>/var/log/cacti/poller-error.log)",  
- "@timestamp" => "2013-12-23T22:30:01.000Z",  
- "@version" => "1",  
- "type" => "syslog",  
- "host" => "0:0:0:0:0:0:0:1:52617",  
- "syslog_timestamp" => "Dec 23 14:30:01",  
- "syslog_hostname" => "louis",  
- "syslog_program" => "CRON",  
- "syslog_pid" => "619",  
- "syslog_message" => "(www-data) CMD (php /usr/share/cacti/site/poller.php >/dev/null 2>/var/log/cacti/poller-error.log)",  
- "received_at" => "2013-12-23 22:49:22 UTC",  
- "received_from" => "0:0:0:0:0:0:0:1:52617",  
- "syslog_severity_code" => 5,  
- "syslog_facility_code" => 1,  
- "syslog_facility" => "user-level",  
- "syslog_severity" => "notice"
- }  
