# MySQL与unix时间问题 - weixin_33985507的博客 - CSDN博客
2016年02月18日 16:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
1、select unix_timestamp() 　　　　　　　　　　　　　　　　　　-->可以返回以秒记的unix时间。
2、select from_unixtime('1455804595','%Y年%m月%d号');　　 -->格式化unix时间为我们常见的形式。
3、python/time.time()　　　　　　　　　　　　　　　　　　　　　-->可以返回心秒记的unix时间，与MySQL不同的事，它的精度要高一些(它还有小数部分)
