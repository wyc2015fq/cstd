# logstash 中filter中的date插件 - sxf_123456的博客 - CSDN博客
2017年12月20日 14:27:57[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：4585
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
"RECEIVE_TIME": "2017-11-26T22:11:47.000Z",
时间转化为中间带T格式时间
filter{
date{
- match => ["logdate", "dd/MMM/yyyy:HH:mm:ss Z"]  
- add_field =>["RECEIVE_TIME","%{logdate}"]
  
}
}
等价于
filter{
date{
     match => ["RECEIVE_TIME","dd/MMM/yyy:HH:mm:ss Z"]
     target =>  "RECEIVE_TIME"
}
}
logstash 中filter中date多个字段需要格式时间，只能一个date里边只能一个match和一个target
 date {
            match => ["RECEIVE_TIME", "yyyyMMddHHmmss"]
            target => "RECEIVE_TIME"
    }
    date{
            match => ["START_DATE", "yyyyMMddHHmmss"]
            target => "START_DATE"
    }
不能为这种格式
 date {
            match => ["RECEIVE_TIME", "yyyyMMddHHmmss"]
            target => "RECEIVE_TIME"
            match => ["START_DATE", "yyyyMMddHHmmss"]
            target => "START_DATE"
    }
