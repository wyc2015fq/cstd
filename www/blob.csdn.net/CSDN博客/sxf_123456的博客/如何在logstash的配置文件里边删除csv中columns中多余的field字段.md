# 如何在logstash的配置文件里边删除csv中columns中多余的field字段 - sxf_123456的博客 - CSDN博客
2017年07月21日 18:01:43[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：2956
当采集数据时生成的csv的文件，但是发现采集表数据有些在es中不需要，这时，可以在filter插件中的csv中，使用remove_field => [ "filed1","filed2" ]来删除多余字段
input {
    file {
        path => [
                        "/test/111.csv"
                ]
        start_position => "beginning" #从什么位置读取，beginnig时导入原有数据
        sincedb_path => "/test/111"
                type => "111"
                tags => ["optical", "gather"]
    }
}
filter {
        if [type] == "111" { #多个配置文件同时执行的区分
        csv {
                columns =>["gathertime","device_id","oltip","devicename","vendor_id","oltportinfo","oltsendpower","oltrecvpower","onuid","onuip","onusn","onumac","loid","onusendpower","onurecvpower","oltsenddecay","onusenddecay","oltcurrent","olttemperature","oltvoltage","onucurrent","onutemperature","onuvoltage","onuadminstate","onuoperstat","oltadminstate","oltoperstat","distance","oltopvarname","oltopvarpn","oltopvarsn","opticaltype","down_access","up_access","all_access","stat_num","only_id","city_id","branch_id","branch_name","area_type"]
                separator => "^"
        quote_char => "‰"
        remove_field => ["device_id","branch_id","area_type"]
       }
   }
output{
}
2、使用命令执行logstash
logstash  -f  111.conf
3、使用下边命令从es中查询已删除的字段是否存在
curl -XGET 'localhost:9200/gather-111/_search?pretty' -H 'Content-Type:application/json' -d'
{
 "query":{
  "match":{
  "only_id":"19011111"
   }
  }
}'
 "device_id":"190574"
"branch_id":"1759"
"city_id":"118214"
"area_typ":"2"
4、删除字段
filter {
 if [type] == "034test" { #多个配置文件同时执行的区分
        csv {
                columns =>["OUI","IptvAccount","UploadTime"]
                separator => ","
                quote_char => "‰"
        }
       date {
        match => ["UploadTime","yyyyMMddHHmmss"]
        target => "UploadTime"
    }
     mutate {
        remove_field => "message"
        remove_field => "@version"
        remove_field => "host"
    }
}
