# filebeats配合logstash采集日志 - weixin_33985507的博客 - CSDN博客
2018年07月10日 16:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：121
```
正则表达式
NAME            [^|]+
THREAD          \d+
FUNCTIME        [^|]+
LEVELNAME       [^|]+
MESSAGE         [^|]+
MDC             [^|]+
配置文件
input {
file {
path => "/opt/logs/*.log"
start_position => "beginning"
codec=> multiline {
pattern => "^\d\d\d\d"
negate => true
what => "previous"
}
}
}
filter {
grok {
patterns_dir => ["./patterns"]
match => [
"message" , "%{TIMESTAMP_ISO8601:asctime}\|\|\|\|\|%{NAME:name}\|\|%{NUMBER:thread}\|\|%{FUNCTIME:funcName}\|\|%{LEVELNAME:levelname}\|\|%{MESSAGE:ourmessage}\|\|\|\|%{MDC:mdc}",
"message" , "%{TIMESTAMP_ISO8601:asctime}\|\|\|\|\|%{NAME:name}\|\|%{NUMBER:thread}\|\|%{FUNCTIME:funcName}\|\|%{LEVELNAME:levelname}\|\|%{MESSAGE:ourmessage}"
]
}
date {
match => ["asctime","yyyy-MM-dd HH:mm:ss"]
target => "asctime"
}
mutate {
rename => ["levelname","type"]
}
}
output {
stdout { codec => rubydebug }
}
input {
   beats{
   port => 5044
}
}
filter {
     grok {
        patterns_dir => ["./patterns"]
match => [
              "message" , "%{TIMESTAMP_ISO8601:asctime}\|\|\|\|\|%{NAME:name}\|\|%{NUMBER:thread}\|\|%{FUNCTIME:funcName}\|\|%{LEVELNAME:levelname}\|\|%{MESSAGE:ourmessage}\|\|\|\|%{MDC:mdc}",
              "message" , "%{TIMESTAMP_ISO8601:asctime}\|\|\|\|\|%{NAME:name}\|\|%{NUMBER:thread}\|\|%{FUNCTIME:funcName}\|\|%{LEVELNAME:levelname}\|\|%{MESSAGE:ourmessage}"
        ]
}
date {
     match => ["asctime","yyyy-MM-dd HH:mm:ss"]
    target => "asctime"
  }
mutate {
 rename => ["levelname","type"]
}
}
output {
    stdout { codec => rubydebug }
}
```
