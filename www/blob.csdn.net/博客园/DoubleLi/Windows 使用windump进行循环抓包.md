# Windows 使用windump进行循环抓包 - DoubleLi - 博客园






# 准备工作

1、下载tcpdump 
[http://www.winpcap.org/windump/](http://www.winpcap.org/windump/)
![2017_03_18_at_1_42_PM](https://yqfile.alicdn.com/1de38ee357ec156fe6921b4f7960578424b6ac38.png)

2、下载WinPcap
[http://www.winpcap.org/install/bin/WinPcap_4_1_3.exe](http://www.winpcap.org/install/bin/WinPcap_4_1_3.exe)

# 安装

1、安装Winkpcap 双击运行->下一步->完成
2、移动WinDump.exe到C盘根目录下

# 使用

cd到C盘根目录
1、查看帮助信息

```
C:\>WinDump.exe -h
WinDump.exe version 3.9.5, based on tcpdump version 3.9.5
WinPcap version 4.1.3 (packet.dll version 4.1.0.2980), based on libpcap version 1.0 branch 1_0_rel0b (20091008)
Usage: WinDump.exe [-aAdDeflLnNOpqRStuUvxX] [ -B size ] [-c count] [ -C file_size ]
                [ -E algo:secret ] [ -F file ] [ -i interface ] [ -M secret ]
                [ -r file ] [ -s snaplen ] [ -T type ] [ -w file ]
                [ -W filecount ] [ -y datalinktype ] [ -Z user ]
                [ expression ]
```

2、查看网卡信息
通过systeminfo确认网卡顺序（只提取了网卡部分，网卡信息一般在命令结果的最下方）：
![2017_03_18_at_3_54_PM](https://yqfile.alicdn.com/df3ac7917564392f8a55c74af74fa897a65dfb95.png)

进行对比得知，序号1位内网网卡，序号2位公网网卡。

![2017_03_18_at_3_54_PM1](https://yqfile.alicdn.com/c2f6a8b9a6bc55df384f6e6819ed09647a451901.png)

3、抓包显示

```
C:\>WinDump.exe -i 2
WinDump.exe: listening on \Device\NPF_{F08820B5-CE29-4149-8F61-AAD4AA472523}
14:01:10.382760 IP iZddso65qb9n1sZ.3389 > xxx.xx.xx.xx.6800: P 1424906657:1424906822(165) ack 1778482190 win 8419 <nop,nop,timestamp 952070
 935655340>
5 packets captured
19 packets received by filter
0 packets dropped by kernel
```

4、循环抓包

最大抓取50个包，每个包1M
相关参数可以调整。

```
C:\>WinDump.exe -i 2 -C 1 -W 50 -w c:\3389.pcap tcp and port 3389
WinDump.exe: listening on \Device\NPF_{F08820B5-CE29-4149-8F61-AAD4AA472523}

91381 packets captured
100856 packets received by filter
9405 packets dropped by kernel
```

![2017_03_18_at_2_20_PM](https://yqfile.alicdn.com/047b213c344436d7f8bd9f96b27de680118e0988.png)

# 注

抓包时CMD窗口不可关闭，如果关闭，抓包将停止。



from:https://yq.aliyun.com/articles/72447









