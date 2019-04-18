# OperationalError：(1054 - "Unknown column 'game.lable1' in 'field list' ")解决办法 - weixin_33985507的博客 - CSDN博客
2016年05月08日 00:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：24
今天白天遇到一个错误，第一次遇到这样的问题，数据库的问题，百度了很多答案也找了很多博客文章看
问题：OperationalError：(1054 - "Unknown column 'game.lable1' in 'field list' ")
问题截图如下：
![](https://images2015.cnblogs.com/blog/922662/201605/922662-20160508002944250-1017481220.png)
![](https://images2015.cnblogs.com/blog/922662/201605/922662-20160508002952203-719673721.png)
经过我不断的查找和尝试，最后终于解决了问题；
答案：OperationalError：(1054 - "Unknown column 'game.lable1' in 'field list' ")
是我的表里没有字段lable1和lable2和lable3    这三个字段      
其实python错误提示蛮详细的，只要大家细心点耐心点不断的去尝试       
