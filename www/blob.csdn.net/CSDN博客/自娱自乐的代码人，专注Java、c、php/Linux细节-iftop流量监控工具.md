# Linux细节 - iftop流量监控工具 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年11月10日 19:34:32[initphp](https://me.csdn.net/initphp)阅读数：1162








## 1. iftop

在类Unix系统中可以使用top查看系统资源、进程、内存占用等信息。查看网络状态可以使用netstat、nmap等工具。若要查看实时的网络流量，监控TCP/IP连接等，则可以使用iftop。官网：[http://www.ex-parrot.com/~pdw/iftop/](http://www.ex-parrot.com/~pdw/iftop/)




## 2. 安装iftop

1. 安装依赖软件


`yum install flex byacc  libpcap ncurses ncurses-devel libpcap-devel`

2. 编译安装iftop





```
wget http://www.ex-parrot.com/pdw/iftop/download/iftop-0.17.tar.gz
tar zxvf iftop-0.17.tar.gz
cd iftop-0.17
./configure
make && make install
```


3. 运行iftop




`iftop`



## 3. iftop说明

![](https://img-blog.csdn.net/20151110193058404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

TX：发送流量

RX：接受流量

TOTAL：总流量

cumm：运行一段时间的总流量

peak：峰值

rates：分别表示过去 2s 10s 40s 的平均流量




## 4. 命令参数



-i    设定监测的网卡，如：# iftop -i eth1

-B  以bytes为单位显示流量(默认是bits)，如：# iftop -B

-n  使host信息默认直接都显示IP，如：# iftop -n

-N  使端口信息默认直接都显示端口号，如: # iftop -N

-F  显示特定网段的进出流量，如# iftop -F 10.10.1.0/24或# iftop -F 10.10.1.0/255.255.255.0

-h（display this message），帮助，显示参数信息

-p  使用这个参数后，中间的列表显示的本地主机信息，出现了本机以外的IP信息;

-b  使流量图形条默认就显示;

-f  这个暂时还不太会用，过滤计算包用的;

-P  使host信息及端口信息默认就都显示;

-m  设置界面最上边的刻度的最大值，刻度分五个大段显示，例：# iftop -m 100M







