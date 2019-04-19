# logstash 中将时间转化为带T格式的时间 - sxf_123456的博客 - CSDN博客
2017年12月08日 12:55:42[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1321
将column_time字段时间解析为2015-01-01T01:12:23格式时间，这种格式的时间为 ISO8601标准
filter {
    date {
        match ["column_time","yyyy-MM-dd'T'HH:mm:ss"]
    }
}
gathertime字段格式化为UNIX时间戳
UNIX时间戳是从1970年01月01日 00:00:00开始计时的时间戳
date { #将某个时间字段的值写入到时间戳
match => ["gathertime", "UNIX"]
target => "gathertime"
}
