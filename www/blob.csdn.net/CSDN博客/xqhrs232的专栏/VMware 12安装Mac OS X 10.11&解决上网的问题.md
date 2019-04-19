# VMware 12安装Mac OS X 10.11&解决上网的问题 - xqhrs232的专栏 - CSDN博客
2018年12月04日 22:12:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：167
原文地址::[https://jingyan.baidu.com/article/f006222819d1e1fbd3f0c892.html](https://jingyan.baidu.com/article/f006222819d1e1fbd3f0c892.html)
相关文章
1、[如何解决VMware上MAC虚拟机不能上网问题]()----[https://zhidao.baidu.com/question/177718351562492124.html](https://zhidao.baidu.com/question/177718351562492124.html)
//========================================================================
[VMware](https://www.baidu.com/s?wd=VMware&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao) Workstation 11在安装 Mac OS X 10.9之后出现不能上网的问题，本文将一步步教你如何解决这个问题。
PC
从本机中选择打开连接网络，选择本地连接。如果是无线网可以选择无线网。
选择属性，点击共享按钮。
将internet连接共享下面两个选项都选中，然后在家庭网络连接选择VMware Network Adapter VMnet1。
在安装的虚拟机中选择虚拟机->设置选项。
点击网络适配器，将网络连接改成仅主机模式（Host-only），然后在右侧选择主机模式，点击确定。
进入Mac系统，选择设置，进入网路设置
7
配置Ipv4选择设置[DHCP](https://www.baidu.com/s?wd=DHCP&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)，点击应用即可开始上网。
