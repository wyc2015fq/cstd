# 关于Winsock：Winsock编程注意事项：Windows Sockets 2体系结构 - 一世豁然的专栏 - CSDN博客





2018年10月10日 16:20:12[一世豁然](https://me.csdn.net/Explorer_day)阅读数：29标签：[Windows Sockets2](https://so.csdn.net/so/search/s.do?q=Windows Sockets2&t=blog)
个人分类：[Windows](https://blog.csdn.net/Explorer_day/article/category/6772518)









Windows Sockets 2体系结构符合Windows开放系统体系结构（WOSA），如下所示：

![](https://img-blog.csdn.net/20181010161904516?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Winsock在应用程序编程接口（API）之间定义了一个标准服务提供者接口（SPI），其功能是从WS2_32.dll和协议栈导出的。 因此，Winsock支持不仅限于TCP / IP协议栈，就像Windows Sockets 1.1一样。

对于Windows Sockets 2体系结构，堆栈供应商不必或不希望提供自己的WS2_32.dll实现，因为单个WS2_32.dll必须适用于所有堆栈。 应该以与操作系统组件相同的方式查看WS2_32.dll和兼容性填充程序。



