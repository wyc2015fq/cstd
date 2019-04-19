# 解决：Invalid character found in the request target.The valid characters are defined in RFC 7230 and RF - xqhrs232的专栏 - CSDN博客
2018年11月28日 10:58:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：107
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/wsygdb/p/7661220.html](https://www.cnblogs.com/wsygdb/p/7661220.html)
相关文章
1、Tomcat 报 The valid characters are defined in RFC 7230 and RFC 3986----[https://blog.csdn.net/yhq1913/article/details/53406275](https://blog.csdn.net/yhq1913/article/details/53406275)
目录
- [背景](https://www.cnblogs.com/wsygdb/p/7661220.html#%E8%83%8C%E6%99%AF)
- [原因分析](https://www.cnblogs.com/wsygdb/p/7661220.html#%E5%8E%9F%E5%9B%A0%E5%88%86%E6%9E%90)
- [处理方法](https://www.cnblogs.com/wsygdb/p/7661220.html#%E5%A4%84%E7%90%86%E6%96%B9%E6%B3%95)
- [参考](https://www.cnblogs.com/wsygdb/p/7661220.html#%E5%8F%82%E8%80%83)
## 背景
在将tomcat升级到7.0.81版后，发现系统的有些功能不能使用了，查询日志发现是有些地址直接被tomcat认为存在不合法字符，返回HTTP 400错误响应，错入信息如下：
![](https://images2017.cnblogs.com/blog/1162504/201710/1162504-20171013144456918-321679429.jpg)
## 原因分析
经了解，这个问题是高版本tomcat中的新特性：就是严格按照 RFC 3986规范进行访问解析，而 RFC 3986规范定义了Url中只允许包含英文字母（a-zA-Z）、数字（0-9）、-_.~4个特殊字符以及所有保留字符(RFC3986中指定了以下字符为保留字符：! * ’ ( ) ; : @ & = + $ , / ? # [ ])。而我们的系统在通过地址传参时，在url中传了一段json，传入的参数中有"{"不在RFC3986中的保留字段中，所以会报这个错。
根据（https://bz.apache.org/bugzilla/show_bug.cgi?id=60594） ，从以下版本开始，有配置项能够关闭/配置这个行为：
8.5.x系列的：8.5.12 onwards
8.0.x系列的：8.0.42 onwards
7.0.x系列的：7.0.76 onwards
## 处理方法
.../conf/catalina.properties中，找到最后注释掉的一行 ＃tomcat.util.http.parser.HttpParser.requestTargetAllow=| 　，改成tomcat.util.http.parser.HttpParser.requestTargetAllow=|{}，表示把｛｝放行
------2018.01.30 新增--------
按照上面的方法处理好后，在非IE浏览器上访问，是没有问题了。但若是在IE浏览器上进行访问，这个错误还是会出现，在IE上访问出现这个错误的原因：因为url的参数json中有双引号，火狐和谷歌浏览器会自动对url进行转码，但IE不会
这种情况的处理方法：
给系统配置方向代理服务器，通过反向代理服务器进行urlrewrite，手动取出各个json的数据，手动将双引号进行转码为%22：
具体方式如下：
编辑 Apache安装目录/conf/httpd.conf， 在配置项目反向代理的前面添加如下信息：
RewriteCond %{QUERY_STRING} json
RewriteCond %{QUERY_STRING} !msKey
RewriteCond %{QUERY_STRING} msInfo
RewriteCond %{QUERY_STRING} player
RewriteCond %{QUERY_STRING} {[^a-zA-Z0-9]*([a-zA-Z]+)[^a-zA-Z0-9]*:[^a-zA-Z0-9]*([a-zA-Z0-9*]+)[^a-zA-Z0-9]*,[^a-zA-Z0-9]*([a-zA-Z]+)[^a-zA-Z0-9]*:[^a-zA-Z0-9]*([a-zA-Z0-9*]+)[^a-zA-Z0-9]*}
RewriteRule ^(.*)? $1?method=sendJson&json={%22%1%22:%22%2%22,%22%3%22:%22%4%22} [R,L,NE]
## 参考
https://tomcat.apache.org/tomcat-7.0-doc/config/systemprops.html（官网各配置项说明）
