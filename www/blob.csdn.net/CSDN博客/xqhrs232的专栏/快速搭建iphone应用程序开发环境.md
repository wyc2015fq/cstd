# 快速搭建iphone应用程序开发环境 - xqhrs232的专栏 - CSDN博客
2012年10月08日 16:52:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：671标签：[程序开发																[iphone](https://so.csdn.net/so/search/s.do?q=iphone&t=blog)](https://so.csdn.net/so/search/s.do?q=程序开发&t=blog)
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[http://blog.csdn.net/hipeterfeng/article/details/5889319](http://blog.csdn.net/hipeterfeng/article/details/5889319)
这几天在windows xp上安装mac os x和iphone sdk、搭建iphone应用程序的开发环境，在这里把搭建环境的过程和各位朋友分享下。
一、下载必须的软件
- mac os x系统：[Mac_OS_10.6_Snow_Leopard_10.6_Retail_maiyadi.dmg](ftp://snowleoparddown.maiyadi.com/Mac_OS_10.6_Snow_Leopard_10.6_Retail_maiyadi.dmg)
- iphone sdk：[iphone_sdk_3.1.2_with_xcode_3.2.1__snow_leopard__10m2003.dmg](http://ipsw.info/iphone_sdk_3.1.2_with_xcode_3.2.1__snow_leopard__10m2003.dmg)
- vmware 7.0：[VMware_Workstation_7.0_CN_Lite.exe]()
- darwin：[darwin.iso]()或者Rebel EFI：[Rebel
 EFI.iso](http://public.blu.livefilestore.com/y1pSxoCrI_0_Ro2GAWpV-sii1qJ-v5R13ToF1UfIFj0ZopHulonMmlMsPP3iVckEsM5JtDnNveplwLubfYio147eA/Rebel%20EFI.iso?download)
二、检查机器的配置
- 内存在2G以上
- CPU支持虚拟技术。是否支持虚拟技术可以使用工具[securable.exe](http://bbs.51cto.com/attachment.php?aid=174118&k=ff00f18ba089fd21377b8e749857daed&t=1281420302)检测。
三、安装软件
- 安装vmware7.0
- 安装mac os x：- 在vmware新建虚拟机，在虚拟机上安装mac os x。
- 配置虚拟机：比如内存分配1024M，硬盘分配15G，CD/DVD装载引导工具darwin.iso或Rebel EFI.iso，删除不用的软驱等。
- 选择当前新建的虚拟机，点击“Power On”。
- 把mac os x的dmg文件装载到CD/DVD，开始安装mac os x。
- mac os x安装完成，把darwin.iso装载到CD/DVD上。
- 选择安装mac os x之后的虚拟机，点击“Power On”。
- 安装iphone sdk：- 把iphone sdk的dmg文件放到windows的网络共享目录下。
- 编辑虚拟机网络配置。
- 在mac os x网络中找到共享的windows目录。
- 双击打开dmg文件。
- 安装iphone sdk。
四、搭建iphone开发环境完成。
