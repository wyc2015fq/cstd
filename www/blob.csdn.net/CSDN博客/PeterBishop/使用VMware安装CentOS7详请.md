# 使用VMware安装CentOS7详请 - PeterBishop - CSDN博客





2018年10月09日 13:30:56[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：37
个人分类：[必备操作](https://blog.csdn.net/qq_40061421/article/category/7965045)









**准备资料： **

**CentOS-7-x86_64-Everything-1611 **

**[点击下载CentOS](http://isoredirect.centos.org/centos/7/isos/x86_64/CentOS-7-x86_64-Everything-1804.iso)**

**![](https://img-blog.csdn.net/20170401103118424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

对，资料就这些

**第一步、  点击文件  再点击新建虚拟机**

![](https://img-blog.csdn.net/20170331180123158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**第二步 、点击完新建虚拟机之后会跳出一个窗口  我一般都选择典型的因为比较方便  然后下一步**

![](https://img-blog.csdn.net/20170331180812989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**第三步、**

**![](https://img-blog.csdn.net/20170331181716728?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**第四步、版本这里需要注意 因为我安装的是CentOS是64位的所以我选择了CentOS 64位**

**![](https://img-blog.csdn.net/20170331182026155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**第五步、**

**![](https://img-blog.csdn.net/20170331182858027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**第六步、**

**![](https://img-blog.csdn.net/20170331184003590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**第七步、**

**![](https://img-blog.csdn.net/20170331184141427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**第八步、**

**![](https://img-blog.csdn.net/20170331184943087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**![](https://img-blog.csdn.net/20170331184956415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**第九步、**

**![](https://img-blog.csdn.net/20170331185215199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**第十步、刚才忘记在弄镜像时设置这个了，网络连接设置为桥接模式是因为设置之后这个虚拟主机的IP网段就可以自动的与你主机的IP段就一致了**

**![](https://img-blog.csdn.net/20170331185839007?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**第十一步、选择第一个然后回车 接着等待安装**

**![](https://img-blog.csdn.net/20170401091453969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**第十二步、选择语言 我选择了中文  你懂得，如果你觉得你的英文不错可以选择英文**

![](https://img-blog.csdn.net/20170401091717942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第十三步、其他的都会默认帮我们选择，现在我们选择软件选择 ，进入选择黑窗口还是图形化界面模式

![](https://img-blog.csdn.net/20170401092616146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401092746034?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击完成之后会安装源会自动的帮你选择，你就不用操心了

![](https://img-blog.csdn.net/20170401093213852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401093227274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击安装位置之后

![](https://img-blog.csdn.net/20170401093644307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401093836097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170401095009094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170401095203017?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401095415325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401095947489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401100001896?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401100011850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

安装过程比较久可以泡杯咖啡先![偷笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/titter.gif)........

![](https://img-blog.csdn.net/20170401101029136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401101037927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170401101058558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401101136667?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401101318543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170401101928080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20170401101948985?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401102002469?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401102022065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401102030501?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170401102037987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**一个漂亮的图形化界面就出来了安装就到这里结束了**

![](https://img-blog.csdn.net/20170401102137626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVpXzIwMTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**文章若有不对之处还请众大神指出,以免误人子弟**

--------------------- 本文来自 hui_2016 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/hui_2016/article/details/68927487?utm_source=copy



