# date 中match插件是根据字段的日期格式，采用不同的格式进行转化 - sxf_123456的博客 - CSDN博客
2017年12月25日 10:06:56[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：137
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
                
date 中match插件是根据字段的日期格式，采用不同的格式进行转化
date{
  match => ["gathertime","UNIX"] #原始数据为UNIX时间戳，UNIX时间戳以1970.01.01-00:00:00 开始计时到当前时间
  target => "gathertime"  #数据保存
}
date{
  match => ["alarm_time","yyyyMMddHHmmss"] #原始数据为20170101101010,进行格式化
  target => "alarm_time" #数据保存
}
date{
   match => ["createdate","dd-MMM-yy"] #原始数据为01-Sep-17,格式化为dd-MMM-yy格式
   target => "createdate" #将格式化时间的字段保存
}

