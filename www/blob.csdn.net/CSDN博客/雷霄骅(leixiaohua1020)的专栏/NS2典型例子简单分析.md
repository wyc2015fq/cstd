# NS2典型例子简单分析 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月02日 12:04:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35
个人分类：[网络](https://blog.csdn.net/leixiaohua1020/article/category/8433201)










**“2-1-1”丢包分析**

打开Cygwin.terminal，输入 startxwin进入Cygwin的xwindow界面，输入source bashrc进行环境变量的配置；输入cd test切换目录到test文件夹，输入ns example2.tcl进行仿真。

创建四个节点(nodes)0、1、2、3，设置两个发送端0、1，节点2作为转发，节点3作为接受。实验中即有两条链路传输：0—2—3、1—2—3，为了以示区分，两条链路传输过程中设为不同的颜色。对应代码如下：

```
#创建四个节点(nodes)      
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
#节点的布局
$ns duplex-link-op $n0 $n2 orient right-down
$ns duplex-link-op $n1 $n2 orient right-up
$ns duplex-link-op $n2 $n3 orient right
#给data flows定义不同的标记颜色(根据fid)
$ns color 1 Blue
$ns color 2 Red
```


 两条链路设置不同的起始时间，便于分别观察链路的传输情况，对应代码如下：

```
#为CBR agents 设置任务计划(在第x秒，开始/结束)
$ns at 0.5 "$cbr0 start"
$ns at 1.0 "$cbr1 start"
$ns at 4.0 "$cbr1 stop"
$ns at 4.5 "$cbr0 stop"
```

因此，在0.5秒的时候可以见到0—2—3开始运行，此时流量从节点0经过节点2转发由节点3接受，如图：

![](https://img-blog.csdn.net/20131002120139093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在1.0秒的时候1—2—3开始运行，此时节点1开始传输经节点2由节点3转发，节点3的的线路开始出现排队现象，如图：

![](https://img-blog.csdn.net/20131002120126265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在1.18秒开始，此时节点2到节点3的链路出现拥塞，开始进行丢包。如图：

![](https://img-blog.csdn.net/20131002120131203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

各带宽、时延就丢包方式等信息设置如下：

```
#为节点(nodes)创建连接,并设置属性(带宽，时延，队列丢包方式)
$ns duplex-link $n0 $n2 1Mb 10ms DropTail
$ns duplex-link $n1 $n2 1Mb 10ms DropTail
$ns duplex-link $n3 $n2 1Mb 10ms SFQ
```






