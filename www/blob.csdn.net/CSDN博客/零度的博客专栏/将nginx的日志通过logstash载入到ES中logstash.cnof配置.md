# 将nginx的日志通过logstash载入到ES中logstash.cnof配置 - 零度的博客专栏 - CSDN博客
2018年07月03日 10:49:49[零度anngle](https://me.csdn.net/zmx729618)阅读数：631
1、nginx日志格式化为json格式的配置（nginx日志json格式化请自行google）：
input {  # 定义日志源
# file {   #从文件中拉取
   #    type => "log"
   #    path => "D:/log/academy_*.log"
   #    start_position => "beginning" 
   # }
   beats {  #从fileBeat中获取
          port => "5044"
   }
# kafka {  #从kafka中获取
#       bootstrap_servers => "127.0.0.1:9092"
#       topics => ["ecplogs"]
#   }
}
filter {
  json { #日志为json格式
       source => "message"
       #remove_field => ["message"]
  }
  date {  #日志中包括timestamp字段
       match => [ "timestamp", "UNIX", "ISO8601", "yyyy-MM-dd HH:mm:ss" ]
  }
}
output {  # 定义日志输出
    elasticsearch {
        hosts => ["localhost:9200"]       # 定义es服务器的ip
        index => "logstash-system-syslog-%{+YYYY.MM.dd}"   # 定义索引名称
    }
#    stdout {
#     codec => rubydebug { }
#    }
}
2、nginx使用默认格式化形式:
input {  # 定义日志源
  # file {
  #     type => "log"
  #     path => "D:/nginx-1.14.0/access.log"
  #     #start_position => "beginning" 
  #  }
   beats {
       port => "5044"
   }
#   stdin { }
#   kafka {
#       bootstrap_servers => "127.0.0.1:9092"
#       topics => ["ecplogs"]
#   }
}
filter {
  grok {
      match => { "message" => "%{IP:clientIp} - - \[%{HTTPDATE:logdate}\] \"%{WORD:verb} %{URIPATHPARAM:request} HTTP/%{NUMBER:httpversion}\" %{NUMBER:http_status_code} %{NUMBER:bytes} \"%{NOTSPACE:ref}\" \"%{DATA:user_agent}\"" }
      }
  date {
         match => [ "logdate", "UNIX", "ISO8601", "dd/MMM/yyyy:HH:mm:ss Z","yyyy-MM-dd HH:mm:ss" ]
  target => "@timestamp"
       }
   kv  {
          source => "request"
          field_split => "&?"
          value_split => "="
        }
   urldecode {
        all_fields => true
    }
   geoip { source => "clientIp" }
}
output {  # 定义日志输出
    elasticsearch {
        hosts => ["localhost:9200"]       # 定义es服务器的ip
        index => "logstash-nginx-%{+YYYY.MM.dd}"   # 定义索引名称
    }
    stdout {
        codec => json_lines
    }
}
