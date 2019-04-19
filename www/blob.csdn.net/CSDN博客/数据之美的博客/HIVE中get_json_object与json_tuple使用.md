# HIVE中get_json_object与json_tuple使用 - 数据之美的博客 - CSDN博客
2019年01月16日 10:38:18[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：65
个人分类：[hive/sql																[大数据](https://blog.csdn.net/oppo62258801/article/category/6768103)](https://blog.csdn.net/oppo62258801/article/category/6961227)
hive中提供了两种针对json数据格式解析的函数，即get_json_object（…）与json_tuple(…)，理论不多说，直接上效果示意图：
假设存在如下json数据对象，若使用hive环境可这么设置：
set hivevar:msg={
"message":"2015/12/08 09:14:4", 
"client": "10.108.24.253", 
"server": "passport.suning.com", 
"request": "POST /ids/needVerifyCode HTTP/1.1",
"server": "passport.sing.co",
"version":"1",
"timestamp":"2015-12-08T01:14:43.273Z",
"type":"B2C","center":"JSZC",
"system":"WAF","clientip":"192.168.61.4",
"host":"wafprdweb03",
"path":"/usr/local/logs/waf.error.log",
"redis":"192.168.24.46"}
1
2
3
4
5
6
7
8
9
10
11
12
13
get_json_object函数
当使用get_json_object函数时，则可以这么用：
select get_json_object(‘${hivevar:msg}’,’$.server’) from test;
返回：
passport.sing.com
其中，get_json_object函数第一个参数填写json对象变量，第二个参数使用$表示json变量标识，然后用 . 或 [] 读取对象或数组；
json_tuple函数 
当使用json_tuple对象时，可以显著提高效率，一次获取多个对象并且可以被组合使用，写法如下：
select a.* from test lateral view json_tuple(‘${hivevar:msg}’,’server’,’host’) a as f1,f2;
返回：
passport.sing.com wafprdweb03
其中，需要使用lateral view 视图方法来写，不需要加$标示符读取对象，获取的f1,f2对象可用于array()或map()等函数使用
另附：parse_url函数使用案例
parse_url(‘http://facebook.com/path/p1.php?query=1‘, ‘HOST’)返回’facebook.com’ ， 
parse_url(‘http://facebook.com/path/p1.php?query=1‘, ‘PATH’)返回’/path/p1.php’ ， 
parse_url(‘http://facebook.com/path/p1.php?query=1‘, ‘QUERY’)返回’query=1’， 
或，可以指定key来返回特定参数，key的格式是QUERY:， 
例如：QUERY:k1 
parse_url(‘http://facebook.com/path/p1.php?query=1#Ref‘, ‘REF’)返回’Ref’ 
parse_url(‘http://facebook.com/path/p1.php?query=1#Ref‘, ‘PROTOCOL’)返回’http’
--------------------- 
作者：Running_you 
来源：CSDN 
原文：https://blog.csdn.net/sinat_29508201/article/details/50215351 
版权声明：本文为博主原创文章，转载请附上博文链接！
