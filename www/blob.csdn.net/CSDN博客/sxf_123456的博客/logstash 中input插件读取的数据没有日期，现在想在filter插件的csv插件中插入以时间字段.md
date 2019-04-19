# logstash 中input插件读取的数据没有日期，现在想在filter插件的csv插件中插入以时间字段 - sxf_123456的博客 - CSDN博客
2017年12月08日 10:30:17[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：366
logstash 中input插件读取的数据没有日期，现在想在filter插件的csv插件中插入以时间字段
filter {
    csv {
        columns => ["column1","cloumn2","column3","colun4"]
        add_field => { 
            "field1" => "2017-12-06"
        }
        convert => {
            "field1" => "date"
        }
    }
}
            
