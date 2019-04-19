# tcpdump源码分析(1)——介绍 - xqhrs232的专栏 - CSDN博客
2018年09月06日 10:19:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：75
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[https://blog.csdn.net/notbaron/article/details/79735359](https://blog.csdn.net/notbaron/article/details/79735359)
相关文章
1、tcpdump源码分析(2)——抓包原理----[https://blog.csdn.net/notbaron/article/details/79735414](https://blog.csdn.net/notbaron/article/details/79735414)
本系列会分析网络诊断工具tcpdump。使得大家(其实主要是针对自己本人)对tcpdump抓包原理和机制有深入认识，并掌握如何对其进行功能扩展。同时能起到细化linux网络协议栈知识点的功效。
            第一篇从整体上来进行介绍，以及源码出处。
         Tcpdump由美国的Lawrence Berkeley National Laboratory开发。使用了libpcap，独立于系统的接口。
Tcpdump可以将网络中传送的数据包的头截获下来提供分析。支持针对网络层、协议、主机、网络或端口的过滤，使分析者过滤不需要的包，不过需要具备root权限。网卡要处在混杂模式下，当网卡进入、退出混杂模式的时候，系统会有信息提示，防止有不法之徒偷偷的进行抓包而无人知晓。
[11038930.759654] device eth0 entered promiscuous mode
[11038932.335690] device eth0 left promiscuous mode
通过tcpdump工具我们可以进一步了解网络到底是如何运行，故障出现在何处，哪种协议产生的包占主要地位，网络瓶颈位于何处等等。
所以说，该工具是系统管理员和网络管理员必须掌握的，无论怎么精通它都不为过。
1.1.1.1  关于参数
因为在网络中存在大量的数据包，而且这些数据包属于不同的协议，而不同协议数据包的格式也不同。因此对捕获的数据必须要进行解码，将包中的信息进行剖析，着对于协议分析来讲非常重要。
            因为tcpdump是开源软件，虽其功能强大，但是在解码分析这块很多还是16进制，不利用分析，可以通过-w参数将监控文件保存下来，给wireshark显示。
1.1.1.2  源码位置
tcpdump的源码并没有在内核中，因为其并不是内核的实现，所以也没必要放在内核中。
我们可以在系统中查看到源码版本：
# tcpdump  --version
tcpdump version 4.7.4
libpcap version 1.7.4
OpenSSL 1.0.2g  1 Mar 2016
            然后可以在官网http://www.tcpdump.org/中，下载源码。当前最新的就是4.9.2版本。
            其github组在[https://github.com/the-tcpdump-group/中](https://github.com/the-tcpdump-group/%D6%D0)，相关的项目均在里面。
1.1.1.3  编译
源码包下载解压后，可以直接编译。包中自带configure文件，执行./configure后就可以进行make编译了。编译之前是需要编译libpcap的。
            下篇我们开始抓包的原理分析。
