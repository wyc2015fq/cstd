# logstash input插件，加载数据时，将编码设定为GBK - sxf_123456的博客 - CSDN博客
2017年12月25日 18:22:51[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：636
logstash input插件，加载数据时，将编码设定为GBK
input{
file {
path => ["/data/sftp/ftpaaa/AAA/USER_*.txt"]
start_position => "beginning"
sincedb_path => "/data/elk/sincedb/025"
type => "025"
tags => ["customer","gather"]
codec => plain{
charset=>"GBK"
}
}
}
            
