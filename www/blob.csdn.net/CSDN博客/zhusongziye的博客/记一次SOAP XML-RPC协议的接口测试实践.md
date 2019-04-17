# 记一次SOAP/XML-RPC协议的接口测试实践 - zhusongziye的博客 - CSDN博客





2017年09月21日 21:29:51[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1318








大家好，之前写的脚本一直都是Http协议的接口，第一次写WebService脚本，用到了SOAP/XML-RPC协议，很高兴自己又get到一项新技能，同时也想把如何实现SOAP/XML-RPC协议的接口过程做个记录！




步骤：


1、新建一个线程组

![](https://img-blog.csdn.net/20170921213139325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



2、在线程组中添加一个SOAP/XML-Request请求

![](https://img-blog.csdn.net/20170921214813004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


3、通过WSDL描述调用方法，填写消息体

![](https://img-blog.csdn.net/20170921213207629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


   这一步可能很多同学都会问如何通过WSDL描述调用方法以及要用到什么工具？别急，下面会告诉你方法！


1）官网下载AltovaXMLSpy工具


2）安装完成之后打开工具创建一个新的SOAP请求

![](https://img-blog.csdn.net/20170921213221282?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3）在文件位置中填写WebService接口的地址，点击确定

![](https://img-blog.csdn.net/20170921213239888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



4）选择要调用的方法，点击确定

![](https://img-blog.csdn.net/20170921213253407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





5）生成方法的消息体

![](https://img-blog.csdn.net/20170921213307708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






4、创建察看结果树和聚合报告方便对请求结果进行查看



![](https://img-blog.csdn.net/20170921213321794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






此篇来源于网络，http://mp.weixin.qq.com/s/WevWBE1E8mY6YuHgXY_XSA，感谢作者的分享！






