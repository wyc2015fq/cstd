# 关于Linux访问web遇到403 Forbidden的问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月06日 11:40:57[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6439








                
问题：

大数据平台的集群监控地址是通过一个代理（ip:port）访问，在windows环境下设置代理是可以正常访问；

但在linux环境下设置了代理却不能访问；telnet和ping代理是可以，但curl和wget却出现问题。



1)wget返回错误：



```
[root@SJJH-SERVER2 ~]# wget -e "http_proxy=http://ip:port" http://c9m1.ecld.com:8088/cluster
--2017-06-06 11:00:01--  http://c9m1.ecld.com:8088/cluster
正在连接 ip:port... 已连接。
已发出 Proxy 请求，正在等待回应... 403 Forbidden
2017-06-06 11:00:01 错误 403：Forbidden。
```


2)curl执行情况：



```
[root@SJJH-SERVER2 ~]#curl -x ip:port http://c9m1.ecld.com:8088/cluster

<div id="content">
<p>The following error was encountered while trying to retrieve the URL: <a href="http://c9m1.ecld.com:8088/cluster">http://c9m1.ecld.com:8088/cluster</a></p>

<blockquote id="error">
<p><b>Access Denied.</b></p>
</blockquote>

<p>Access control configuration prevents your request from being allowed at this time. Please contact your service provider if you feel this is incorrect.</p>

<p>Your cache administrator is <a href="mailto:webmaster?subject=CacheErrorInfo%20-%20ERR_ACCESS_DENIED&body=CacheHost%3A%2032ae9d8e1ec1%0D%0AErrPage%3A%20ERR_ACCESS_DENIED%0D%0AErr%3A%20%5Bnone%5D%0D%0ATimeStamp%3A%20Tue,%2006%20Jun%202017%2003%3A00%3A33%20GMT%0D%0A%0D%0AClientIP%3A%20132.121.82.139%0D%0A%0D%0AHTTP%20Request%3A%0D%0AGET%20%2Fcluster%20HTTP%2F1.1%0AUser-Agent%3A%20curl%2F7.19.7%20(x86_64-unknown-linux-gnu)%20libcurl%2F7.19.7%20NSS%2F3.12.7.0%20zlib%2F1.2.3%20libidn%2F1.18%20libssh2%2F1.2.2%0D%0AAccept%3A%20*%2F*%0D%0AProxy-Connection%3A%20Keep-Alive%0D%0AHost%3A%20c9m1.ecld.com%3A8088%0D%0A%0D%0A%0D%0A">webmaster</a>.</p>
<br>
</div>
```


对于该问题暂时还没找到解决办法，有遇到类似的可以一起沟通，后续找到找到解决方案更新文章。

网上查询了下，问题解决的方向，一个是和web部署的目录权限有关系；一个是和linux环境下不能执行web动态脚本有关系。







