
# Maven基础（3）：代理设定方式 - 知行合一 止于至善 - CSDN博客

2018年10月30日 22:10:24[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2583所属专栏：[DevOps系列之：持续构建与集成](https://blog.csdn.net/column/details/27057.html)



![在这里插入图片描述](https://img-blog.csdn.net/20161205164217366)
在很多场景下，maven不能直接访问到外网时，使用代理是其中常见的一种方式。这篇文章整理一下常见的maven中设置代理的方法。
[
](https://img-blog.csdn.net/20161205164217366)
# 代理服务器
[
](https://img-blog.csdn.net/20161205164217366)代理服务器的搭建，可以使用nginx，或者更为专业一点的使用squid，轻松实现http和https方式。具体方式可以参看如下文章：
[
](https://img-blog.csdn.net/20161205164217366)|代理服务器设定|详细设定方法|
|---|---|
|nginx|[https://blog.csdn.net/liumiaocn/article/details/80502352](https://blog.csdn.net/liumiaocn/article/details/80502352)|
|suqid|[https://blog.csdn.net/liumiaocn/article/details/80586879](https://blog.csdn.net/liumiaocn/article/details/80586879)|
# maven代理服务器设定
[
](https://img-blog.csdn.net/20161205164217366)
## 方法1: 设定settings.xml
[
](https://img-blog.csdn.net/20161205164217366)通过设定全局或者局部的proxy段即可设定代理服务器，一般为注视掉。使用例：
[
](https://img-blog.csdn.net/20161205164217366)`<proxies>
      <proxy>
      <id>optional</id>
      <active>true</active>
      <protocol>http</protocol>
      <username>用户名</username>
      <password>用户密码</password>
      <host>ip或者代理服务器的url</host>
      <port>端口</port>
      <nonProxyHosts>无需使用代理访问的地址</nonProxyHosts>
    </proxy>
  </proxies>`[
](https://img-blog.csdn.net/20161205164217366)设定说明
[
](https://img-blog.csdn.net/20161205164217366)|设定项|说明|
|---|---|
|id|proxy的id，proxy可以有多个，根据id唯一确认proxy|
|active|多个proxy，active的只能有一个，根据此项进行设定|
|protocol|协议，可设定为https或者http以及socks5等|
|username|proxy的用户名，没有的无需设定|
|password|proxy的用户密码，没有的无需设定|
|host|proxy的IP或者访问地址|
|port|proxy的port端口|
|nonPorxyHosts|无需使用代理访问的地址|
## 方法2: 在mvn命令中通过-D传入运行时设定
[
](https://img-blog.csdn.net/20161205164217366)方法1适合与长期保存的内容，如果需要临时的变更性的设定或者不太方便修改pom文件的情况下，择适合使用这种方法进行设定，其实从根本上来说，这是JVM的设定，自然mvn也是这样使用的。
[
](https://img-blog.csdn.net/20161205164217366)|设定选项|说明|
|---|---|
|https.proxyHost|https方式的代理服务器的hostname或者IP|
|https.proxyPort|https方式的代理服务器端口|
|http.proxyHost|http方式的代理服务器的hostname或者IP|
|http.proxyPort|http方式的代理服务器端口|
|socksProxyHost|socks5方式的代理服务器的hostname或者IP|
|socksProxyPort|socks5方式的代理服务器端口|
> 使用例（https方式）：mvn compile -Dhttps.proxyHost=192.168.163.118 -Dhttps.proxyPort=3128
[
](https://img-blog.csdn.net/20161205164217366)
# 总结
[
](https://img-blog.csdn.net/20161205164217366)这篇文章总结了maven下如何设定代理的方式，主要是使用pom文件设定或者运行时传入代理服务器信息，都是常用的maven代理的设定方式。
[
            ](https://img-blog.csdn.net/20161205164217366)

