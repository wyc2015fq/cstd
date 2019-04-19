# Hive 的collect_set使用详解 - 数据之美的博客 - CSDN博客
2017年07月03日 17:03:06[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：5153
有这么一需求，在[Hive](http://lib.csdn.net/base/hive)中求出一个数据表中在某天内首次登陆的人；可以借助collect_set来处理sql:
**[html]**[view
 plain](http://blog.csdn.net/liyantianmin/article/details/48262109#)[copy](http://blog.csdn.net/liyantianmin/article/details/48262109#)
[print](http://blog.csdn.net/liyantianmin/article/details/48262109#)[?](http://blog.csdn.net/liyantianmin/article/details/48262109#)
- select count(a.id)   
- from (select id,collect_set(time) as t from t_action_login where time<='20150906' group by id) as a where size(a.t)=1 and a.t[0]='20150906';  
上面中的
**[html]**[view
 plain](http://blog.csdn.net/liyantianmin/article/details/48262109#)[copy](http://blog.csdn.net/liyantianmin/article/details/48262109#)
[print](http://blog.csdn.net/liyantianmin/article/details/48262109#)[?](http://blog.csdn.net/liyantianmin/article/details/48262109#)
- select id,collect_set(time) as t from t_action_login where time<='20150906' group by id  
会按照id分组，因为一个id可能对应一天也可能对应多天，对应多天表示有多天都有登陆，所以一个id会对应多个日期time,通过collect_set会把每个id所对应的日期构建成一个以逗号分隔的数组返回。上述SQL返回：
**[html]**[view
 plain](http://blog.csdn.net/liyantianmin/article/details/48262109#)[copy](http://blog.csdn.net/liyantianmin/article/details/48262109#)
[print](http://blog.csdn.net/liyantianmin/article/details/48262109#)[?](http://blog.csdn.net/liyantianmin/article/details/48262109#)
-   123@163.com                                                                                      | ["20150620","20150619"]                                                                                    |  
- | abc@163.com                                                                                      | ["20150816"]                                                                                               |  
- | cde@qq.com                                                                                       | ["20150606","20150608","20150607","20150609","20150613","20150610","20150616","20150615"]                  |  
- | 789@sohu.com                                                                                     | ["20150827","20150623","20150627","20150820","20150823","20150612","20150717"]                             |  
- | 987@163.com                                                                                      | ["20150701","20150829","20150626","20150625","20150726","20150722","20150629","20150824","20150716","20150 |  
- | ddsf@163.com                                                                                     | ["20150804","20150803","20150801","20150809","20150807","20150806","20150905","20150904","20150730","20150 |  
- | 182@163.com                                                                                      |["20150803","20150801","20150809","20150808","20150805","20150806","20150906","20150904","20150730","20150 |  
- | 22225@163.com                                                                                    | ["20150604","20150609","20150622","20150827","20150625","20150620","20150613","20150610","20150614","20150 |  
- | 18697@qq.com                                                                                     | ["20150902"]                                                                                               |  
- | 1905@qq.com                                                                                      | ["20150709"]                                         
所以我们就可以按照这个返回的数组做文章，即为
**[html]**[view
 plain](http://blog.csdn.net/liyantianmin/article/details/48262109#)[copy](http://blog.csdn.net/liyantianmin/article/details/48262109#)
[print](http://blog.csdn.net/liyantianmin/article/details/48262109#)[?](http://blog.csdn.net/liyantianmin/article/details/48262109#)
- where size(a.t)=1 and a.t[0]='20150906';  
表示某id所对应的数组长度为1 并且第一个时间为20150906的id表示为该天首次登陆。
总结：
- Hive不允许直接访问非group by字段；
- 对于非group by字段，可以用Hive的collect_set函数收集这些字段，返回一个数组；
- 使用数字下标，可以直接访问数组中的元素；
