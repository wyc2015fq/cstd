# Logstash之时区问题的建议和修改 - BlueSky - CSDN博客
2017年02月16日 08:50:40[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：7722
大家在用Logstash收集日志时，发现@timestamp的时间比本地早8个小时，这是正常的，因为这个时间是UTC时间，日志应统一采用这个时间。
原则上建议大家不要修改这个时间戳：
**[plain]**[view
 plain](http://blog.csdn.net/shan1369678/article/details/51375537#)[copy](http://blog.csdn.net/shan1369678/article/details/51375537#)
- 时区问题的解释  
- 
- 很多中国用户经常提一个问题：为什么 @timestamp 比我们早了 8 个小时？怎么修改成北京时间？+  
- 其实，Elasticsearch 内部，对时间类型字段，是统一采用 UTC 时间，存成 long 长整形数据的！对日志统一采用 UTC 时间存储，是国际安全/运维界的一个通识——欧美公司的服务器普遍广泛分布在多个时区里——不像中国，地域横跨五个时区却只用北京时间。  
- 对于页面查看，ELK 的解决方案是在 Kibana 上，读取浏览器的当前时区，然后在页面上转换时间内容的显示。  
- 所以，建议大家接受这种设定。否则，即便你用 .getLocalTime 修改，也还要面临在 Kibana 上反过去修改，以及 Elasticsearch 原有的 ["now-1h" TO "now"] 这种方便的搜索语句无法正常使用的尴尬。  
- 以上，请读者自行斟酌。  
如果你非要修改，可以这么配置：
![](https://img-blog.csdn.net/20160511164105958?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果上述报错，就直接修改配置文件吧：
**[plain]**[view
 plain](http://blog.csdn.net/shan1369678/article/details/51375537#)[copy](http://blog.csdn.net/shan1369678/article/details/51375537#)
- 老版本（1.5之前的版本）：  
- WithZone (org. Joda. Time. DateTimeZone: : UTC)  
- 修改为：  
- WithZone (org. Joda. Time. DateTimeZone.getDefault ())  
- 
- 1.5以后的版本：  
- 路径：/vendor/bundle/jruby/1.9/gems/logstash-core-(version)-java/lib/logstash/event.rb  
- 
- 我的例子（2.1版本的Logstash）：  
- /logstash/vendor/bundle/jruby/1.9/gems/logstash-core-2.1.0-java/lib/logstash/timestamp.rb  
- 57行  
- UTC = org.joda.time.DateTimeZone.forID(“UTC”)  
- 修改为：  
- UTC = org.joda.time.DateTimeZone.getDefault(:)  
![](http://static.blog.csdn.net/images/save_snippets.png)
