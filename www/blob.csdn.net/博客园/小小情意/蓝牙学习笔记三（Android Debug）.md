
# 蓝牙学习笔记三（Android Debug） - 小小情意 - 博客园






# [蓝牙学习笔记三（Android Debug）](https://www.cnblogs.com/xiaoxiaoqingyi/p/9293689.html)
android 端可以通过两种方式去Debug:
一、在手机的设置功能里，开发者模式 Enable，如下图：
![](https://img-blog.csdn.net/20180711114451505?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAyMDM3MTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[http://blog.bluetooth.com/debugging-bluetooth-with-an-android-app](http://blog.bluetooth.com/debugging-bluetooth-with-an-android-app)
然后在手机的根目录下有一个名类似叫：btsnoop_hci.log 的日志.
通过工具 Wireshark 查看日志：
![](https://img-blog.csdn.net/20180711114506942?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAyMDM3MTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
host对应的是主机， controller对应的是控制器。
Android端查看蓝牙日志：
[http://www.fte.com/webhelp/sodera/Content/Documentation/WhitePapers/BPA600/Encryption/GettingAndroidLinkKey/RetrievingHCIlog.htm](http://www.fte.com/webhelp/sodera/Content/Documentation/WhitePapers/BPA600/Encryption/GettingAndroidLinkKey/RetrievingHCIlog.htm)
[https://www.nowsecure.com/blog/2014/02/07/bluetooth-packet-capture-on-android-4-4/](https://www.nowsecure.com/blog/2014/02/07/bluetooth-packet-capture-on-android-4-4/)
日志数据的分析：
[http://www.fte.com/webhelp/FTS4BT/Content/Technical_Information/BT_Snoop_File_Format.htm](http://www.fte.com/webhelp/FTS4BT/Content/Technical_Information/BT_Snoop_File_Format.htm)
[https://blog.csdn.net/sinat_19628093/article/details/52149189](https://blog.csdn.net/sinat_19628093/article/details/52149189)

二、通过Packet Sniffer 工具抓包分析：
需要购买一个抓包神器，低功耗BLE CC2540 USB Dongle，通过它来抓包.
![](https://img-blog.csdn.net/20180711114522259?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAyMDM3MTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
抓包后的数据：
![](https://img-blog.csdn.net/201807111145349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAyMDM3MTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
对应标识的解释如下：
|Field \#
|Source
|Explanation
|Core spec chapter
|
|1
|Sniffer
|Packet number as logged by the sniffer
|
|2
|Capture device
|Time in microseconds since last packet was received and absolute time
|
|3
|Capture device
|Radio channel data was captured on
|6.B.1.4.1
|
|4
|Air
|Bluetooth spec specified address for advertising and scan response
|6.B.2.1.2
|
|5
|Air
|Type of advertisement packet
|6.B.2.3
|
|6
|Air
|Header
|6.B.2.3
|
|7
|Air
|Advertiser IEEE address
|6.B.2.3
|
|8
|Air
|Advertisement data. In this example it's capabilities and three UUIDs the device provides.
|6.B.2.3 / 3.C.11 / 3.C.18.1
|
|9
|Air
|Precalculated CRC checksum
|6.B.2.1.4
|
|10
|Capture device
|Received signal strength indicator.
|
|11
|Capture device
|Field Control Sequence. If OK, the checksum is correct
|
The advertisement fields are further explained in 6.B.2.3 on page 2202 of the BT core spec.
5 是广播报文的类型，共7种：
1、ADV_IND——通用广播指示
2、ADV_DIRECT_IND—— 定向连接指示
3、ADV_NONCONN_IND—— 不可连接指示
4、ADV_SCAN_IND——可扫描指示
5、SCAN_REQ——主动扫描请求
6、SCAN_RSP——主动扫描响应
7、COMNNECT_REQ——连接请求
广播报文，接入地址为固定值：0x8E89BED6
报文结构：
8               32                    8             8         0~296                  24                  比特
|前导
|接入地址
|报头
|长度
|数据
|循环冗余校验
|



广播报文的长度域：6~37
数据报文的长度域：0~31
这两者的区别在于：广播报文需要加上 6个字节的广播地址
发起连接的抓包情况：
![](https://img-blog.csdn.net/20180711114549214?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAyMDM3MTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
蓝牙规范 Bluetooth 4.0 Core Specification[[2]](http://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=229737)Volume 6, Part B, chapters 2.1, 2.3 and 2.4.
TI 官网有关于sniffer 抓包分析的资料
[http://processors.wiki.ti.com/index.php/BLE_sniffer_guide](http://processors.wiki.ti.com/index.php/BLE_sniffer_guide)

debug总结：
[http://www.argenox.com/bluetooth-low-energy-ble-v4-0-development/library/ultimate-guide-to-debugging-bluetooth-smart-ble-products/](http://www.argenox.com/bluetooth-low-energy-ble-v4-0-development/library/ultimate-guide-to-debugging-bluetooth-smart-ble-products/)





