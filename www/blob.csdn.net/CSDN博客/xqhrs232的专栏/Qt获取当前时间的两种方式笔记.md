# Qt获取当前时间的两种方式笔记 - xqhrs232的专栏 - CSDN博客
2017年09月26日 18:08:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：369
原文地址::[http://blog.csdn.net/philosophyatmath/article/details/41803111](http://blog.csdn.net/philosophyatmath/article/details/41803111)
相关文章
1、[Qt 获取当前时间](http://blog.csdn.net/lusirking/article/details/51460716)----[http://blog.csdn.net/lusirking/article/details/51460716](http://blog.csdn.net/lusirking/article/details/51460716)
- QDateTime dt;  
- QTime time;  
- QDate date;  
- dt.setTime(time.currentTime());  
- dt.setDate(date.currentDate());  
- QString currentDate = dt.toString("yyyy:MM:dd:hh:mm:ss");  
- qDebug()<<currentDate<<"-=-=-=-=-=-=-=-";  
- 
- QDateTime current_date_time = QDateTime::currentDateTime();  
- QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");  
- 
- qDebug()<<current_date<<"-=-=-=-=-=-=-=-";  
![](http://static.blog.csdn.net/images/save_snippets.png)

