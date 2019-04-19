# 使用免费的Open NFC simulator模拟器在BlackBerry模拟器上进行NFC程序调试 - xqhrs232的专栏 - CSDN博客
2013年12月19日 21:34:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1883
原文地址::[http://blog.csdn.net/berryreload/article/details/6918196](http://blog.csdn.net/berryreload/article/details/6918196)

===============================================
原创：2011.11.19
•开发工具
–JDK 1.6
–BlackBerry Java Plugin for Eclipse v1.5 (Windows)
•Open NFC 4.2.3模拟器
–4.2.3是BlackBerry JDE 1.5指定的版本
–安装.NET Framework  3.5
–下载Open NFC core 4.2.3
–安装 - 解压缩即可
•启动Open NFC模拟器
–启动Connection Center
–启动NFC Simulator
–启动Trace Server(如果没有自动启动)
•为BlackBerry模拟器配置Open NFC
–点开Connection Center, 点击Services List按钮；在Services List窗口中，NFC Controllerheader, right-click NFC Device #1 and select Copy service URI from thedrop-down menu.
–启动BlackBerry JDE，Run Configurations，选择 Simulator tab, 选择Advanced tab，选择Use costomized command options，在customized command line后面加入 /nfcc-uri=“”the URI copiedin step 4“”；点击Apply按钮
•在BlackBerry JDE中启动BlackBerry模拟器
–启动BlackBerry JDE
–JDE中启动BlackBerry模拟器
–在BlackBerry模拟器中启用NFC
–在Open NFC Simulator中选择卡片双击即可模拟NFC卡片
===============================================
转贴：original post
at the moment virtual tags can be simulated in BlackBerry simulator.
[转贴：For
 the benefit of others I thought I'd jot down how I use the Open NFC simulator. I am running version 4.2.3 by the way:](http://supportforums.blackberry.com/t5/Java-Development/NFC-API-ReaderWriterManager-addDetectionListener-Hangs/td-p/1376737)
1. Run the Connection Centre
2. Run the NFC Simulator v4.2.3
3. In connection centre click Service List.
4. Right click NFC Device #1 under NFC Controller (NFC HAL). Select Copy Service URI.
5. Start the BlackBerry device simulator. In the Simulate menu selection NFC Options. Select “Use NFC Simulator”. Paste the Service URI from
 the clip board.
需要重新启动模拟器
6. Make sure NFC is enabled in the network service list on the simulator.
7. Double click on one of the simulated tags in the OpenNFC NFC Simulator to “present” the tag
8. Use the BlackBerry Smart Tags app or an app of your own to read or write the tag.
===============================================
参考：
1)DevelopmentGuide的 [Testan NFC applications](http://docs.blackberry.com/en/developers/deliverables/32090/Test_an_NFC_app_1627607_11.jsp) 部分
[2) SDK](http://docs.blackberry.com/en/developers/deliverables/29188/Known_issues_1873711_11.jsp)[7.0](http://docs.blackberry.com/en/developers/deliverables/29188/Known_issues_1873711_11.jsp)[Knownissues](http://docs.blackberry.com/en/developers/deliverables/29188/Known_issues_1873711_11.jsp) 中
 simulateNFC 部分内容
http://docs.blackberry.com/en/developers/deliverables/29188/Known_issues_1873711_11.jsp
3)[Testing
 NFC applications on the BlackBerry Smartphone Simulator](http://docs.blackberry.com/en/developers/deliverables/34443/Testing_NFC_applications_1627604_11.jsp) site:docs.blackberry.com
You can download the package for the NFC Controller Simulator program from SourceForge at http://sourceforge.net/projects/open-nfc/files. If
 you are developing for BlackBerry® Device Software 7.0, download the Core or PC edition of the NFC Controller Simulator version 4.2.3.
===============================================
下载：
[下载Open NFC 4.2.3 (9635)](http://sourceforge.net/projects/open-nfc/files)[](http://us.blackberry.com/developers/javaappdev/index.jsp)
[下载最新的BlackBerry Java Plugin for Eclipse v1.5 (Windows)](http://us.blackberry.com/developers/javaappdev/index.jsp)
