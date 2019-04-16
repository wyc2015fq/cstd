# 开源网络监控管理系统：OpenNMS - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月25日 00:21:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：53
个人分类：[开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1652801)










OpenNMS是一个开源的企业级基于Java/XML的分布式网络和系统监控管理平台。OpenNMS是管理网络的绝好工具，它能够显示网络中各中终端和服务器的状态和配置，为方便地管理网络提供有效的信息。

之前研究过一段时间的网络监控系统，接触到了OpenNMS，在此简单记录一下它的使用方法。

![](https://img-blog.csdn.net/20131221140850484)

OpenNMS网站首页（[http://www.opennms.org/](http://www.opennms.org/)）如图所示。出于方便考虑，就不搭建该系统了，直接试试Demo系统。

该项目是开源的，如果需要的话，可以下载源代码进行二次开发，或者下载程序自己进行部署。

![](https://img-blog.csdn.net/20131221140856015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

点击网站首页右上方的“Live Demo”，进入OpenNMS的Demo系统的的登陆界面（[http://demo.opennms.org/](http://demo.opennms.org/)）。

默认的用户名：demo。密码：demo。

![](https://img-blog.csdn.net/20131221140859000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

登入系统后的首页。

![](https://img-blog.csdn.net/20131221140905734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击菜单栏的第一项“Node List”，列出所有节点的列表。

![](https://img-blog.csdn.net/20131221140908718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击一个节点，可以查看该节点的详细信息。包括了该节点的各种状态。

![](https://img-blog.csdn.net/20131221140913156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击菜单栏的第二项“Search”，可以查找节点信息（可以通过各种条件进行查找）。

![](https://img-blog.csdn.net/20131221140916609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击菜单栏中的“Dashboard”（仪表盘），可以查看整体的信息。

![](https://img-blog.csdn.net/20131221140919812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击菜单栏中的“Chart”，可以以图表的形式查看各种统计信息。

![](https://img-blog.csdn.net/20131221140926421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击菜单栏中的“Maps”->“Topology”，可以查看网络的拓扑结构。

![](https://img-blog.csdn.net/20131221140929687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击菜单栏中的“Maps”->“Geographical”，可以查看服务器物理地址所在。

![](https://img-blog.csdn.net/20131221140933156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





项目首页：[http://www.opennms.org/](http://www.opennms.org/)

Demo页面：[http://demo.opennms.org/](http://demo.opennms.org/)






