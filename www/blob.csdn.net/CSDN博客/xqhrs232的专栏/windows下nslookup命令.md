# windows下nslookup命令 - xqhrs232的专栏 - CSDN博客
2017年05月03日 15:02:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：482
个人分类：[PC/Windows/Office技术](https://blog.csdn.net/xqhrs232/article/category/906929)
原文地址::[http://blog.csdn.net/carolzhang8406/article/details/6218399](http://blog.csdn.net/carolzhang8406/article/details/6218399)
相关文章
1、Win7下如何使用Telnet命令----[http://jingyan.baidu.com/article/95c9d20d96ba4aec4f756154.html](http://jingyan.baidu.com/article/95c9d20d96ba4aec4f756154.html)
rcc-pok-idg-2318:/ # nslookup
> www.baidu.com
Server:         9.2.250.86
Address:        9.2.250.86#53
Non-authoritative answer:
www.baidu.com   canonical name = www.a.shifen.com.
Name:   www.a.shifen.com
Address: 220.181.6.175
nslookup，是用于检查[域名](http://www.i-so.org/tag/%E5%9F%9F%E5%90%8D) 解析，或是用来检测DNS问题的一个实用命令。只要TCP/IP协议正确安装，便可以使用。
在[Windows](http://www.i-so.org/tag/Windows) 命令行中，输入nslookup，默认查询的是本机DNS服务器名及IP地址,如
`Default Server:TDFLW`
`IP Address:10.0.0.16`
如当前DNS服务器没有命名，则显示为Unknow Server。
如查询其他域名信息，则直接输入’nslookup [域名]‘，如nslookup www.i-so.org。也可以在输入nslookup回车后，直接输入要查询的域名
windows下也有改命令
