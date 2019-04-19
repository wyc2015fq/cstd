# logstash将不规则的分隔符文本导入到es中 - sxf_123456的博客 - CSDN博客
2017年08月02日 21:46:56[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1058
                
input{
file {
path => ["/test/csv/csv/data.txt"]
start_position => "beginning"
sincedb_path => "/test/test2"
type => "test2"
tags => ["test2","gather"]
}
}
filter{
if [type] == "test2" {
grok{
 match => {
    "message" => "(?<BAND_ID>.*),SCU(?<UP_WIDTH>\d*).*D(?<DOWN_WIDTH>\d*)"
 }
overwrite => ["message"]
               remove_field => ["message"]
}
}
}
output {
if [type] == "test2" {
   elasticsearch{
hosts => ["0.0.0.0:9200"]
index => "gather-%{type}"
document_type => "%{type}"
flush_size => 20000
idle_flush_time => 10
sniffing => true
template_overwrite => true
codec => "json"
   }
}
}
关键部分是过滤部分用到grok匹配正则表达式
    "message" => "(?<BAND_ID>.*),SCU(?<UP_WIDTH>\d*).*D(?<DOWN_WIDTH>\d*)"
文本数据格式
1222222,SCU2122;SCD22111
1222221,SCU1334D1211212
获取的数据是
1222222    2122   22111
22111        1334    1211212
使用到的正则表达式是
^(\d*),SCU(\d*).*D(\d*)$
grok中使用"(?<BAND_ID>.*),SCU(?<UP_WIDTH>\d*).*D(?<DOWN_WIDTH>\d*)" 实现统计字段BAND_ID,UP_WIDTH,DOWN_WIDTH的值
