# IPTV码流分析指标 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月25日 13:41:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：47









针对IPTV码流可以分析如下指标（参考蓝拓扑公司的IPTV码流分析产品）





**视频内容质量分析**

被动式的视频平均意见分(MOS)测量

实时对所选的IPTV 和VOD 节目进行音视频回放

MPEG PES I, P, B 帧率

存在丢包的MPEG PES I, P, B 帧

MPEG 图片组(GOP) 模式分析



**MPEG传输流（TS）质量分析**

ETSI TR-101 290 告警事件测量

带宽利用率和比特率

PCR 抖动和精度

基于PID 的监测和分析

PSI 表格重建和分析



**IP 传输质量分析**

RFC 4445 媒体传输指标（MDI） 分析

RTP/RTCP QoS 测量：丢包、抖动、乱序和延迟

RFC 3357 丢包模式分析



**IP 视频交互质量分析**

被动式IPTV 频道切换分析(支持IGMP v2/v3和IPv6 MLD)

VOD RTSP 命令响应时延分析

主动式IGMP 测试(STB 仿真)

**支持全面的IP 视频封装格式和协议栈**

> 
#188 字节和 204 字节的MPEG-2 TS 封装于 UDP 或RTP/UDP 或RTSP/TCP 或RTP/RTSP/TCP协议栈上




> 
#因特网流媒体联盟(ISMA)标准封装于RTP/UDP 和RTP/RTSP/TCP 协议栈




> 
#支持SPTS 和MPTS 传输流格式




**全面、灵活的告警门限和事件触发动作配置**

> 
#将触发事件的流存盘做后分析

#发送SNMP Traps 到OSS 系统

#启动自定义的程序




**自动检测并支持主要的2 层和3 层隧道协议**

> 
#VLAN, MPLS, PPPoE

#GTP, GRE 和其它层3 协议字节偏移


**支持长时间的原始视频流数据文件存储**

**强大、灵活的自动报表功能—可导出成PDF 文件**

**支持Wireshark pcap 数据文件的导入/导出**

**多语言支持—支持中文和英文用户界面**

**多网卡支持—单台 BVA 服务器最多可支持8个10/100/1000M 以太网接口**

**基于分布式系统体系架构设计的BVA 可用于本地或远程测试**

> 
#基于Web 的用户界面—无需安装客户端

#多用户支持—提供系统使用效率

#性能可靠的IP 视频统计引擎（7*24小时监测）








可以用于码流分析的地方：

![](https://img-blog.csdn.net/20130925133907859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






