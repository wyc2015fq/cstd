# logstash 不同时区处理区别 - sxf_123456的博客 - CSDN博客
2018年05月12日 15:13:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：834
                使用logstash版本2.3
@timestamp少8小时处理
input {
    stdin{}
}
filter {
    date {
     match => ["message","UNIX_MS"]
     target => "@timestamp"
    }
    ruby {
     code => "event['timestamp'] = LogStash::Timestamp.new(event['@timestamp'] + 8*60*60)"
    }
    ruby {
     code => "event['@timestamp'] = event['timestamp']"
    }
    mutate{
    remove_field => ["timestamp"]
    }
}
output {
    stdout {
    codec => rebydebug
    }
}
在logstash5.2版本中处理时区
input {
    stdin {}
}
filter {
    date {
     match => ["massage","UNIX_MS"]
     target => "@timestamp"
    }
    ruby {
      code => "event.set('timestamp',event.get('@timestamp')+8*60*60)"
    }
    ruby {
      code => "event.set('@timestamp',event.get('timestamp'))"
    }
    mutate {
     remove_field => ["timestamp"]
    }
}
output {
    stdout {
     codec => rebydebug
    }
}
测试命令
echo '1504744911000'| ./logstash -f ~/test.conf
