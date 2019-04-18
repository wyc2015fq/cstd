# 到底什么是URL - 星爷2401 - CSDN博客
2016年12月08日 12:21:30[星爷2401](https://me.csdn.net/leifukes)阅读数：4317
对于URL大家并不陌生，第一印象就是我们访问网页的网址 如：www.blog.csdn.net，只是算是其中的一种url，下面详细介绍一下url的内容。
scheme://host:port/path?query#fragment:
a) .scheme 通信协议，比如http，https，ftp，stmp，weixin：//等等。
b) .host:主机，IP或者是服务器的域名（DNS）。
c) .port:端口号，比如HTTP默认使用的是80端口。端口号的范围是1-65535.
d) .path:路径，由零或多个"/"符号隔开的字符串，一般用来表示主机上的一个目录或文件地址。
e) .query:查询，URL传递的参数。以“？”开头，多个参数用“&”链接，key=value的格式。如：？age=18&name=xingye
f) .fragment:信息片断，字符串，用于指定网络资源中的片断。例如一个网页中有多个名词解释，可使用fragment直接定位到某一名词解释。(也称为锚点)，不会作为内容传给服务器。
