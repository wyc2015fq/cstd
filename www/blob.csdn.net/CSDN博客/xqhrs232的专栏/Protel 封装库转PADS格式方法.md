# Protel 封装库转PADS格式方法 - xqhrs232的专栏 - CSDN博客
2014年09月01日 22:42:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：849
个人分类：[PCB/EDA/硬件设计与仿真](https://blog.csdn.net/xqhrs232/article/category/906926)
原文地址::[http://blog.csdn.net/yeqishi/article/details/5827230](http://blog.csdn.net/yeqishi/article/details/5827230)
最近一直在搞linux的开发，而今天画个控制板，难免在Orcad、Pads中有些操作陌生了，幸好资料保存的都齐全，有问题还是能够很快的解决。在PADS PCB设计中，会有Protel封装装成PADS的问题，我有一个办法，现总结下来供大家分享，该方法首次安装有些繁琐，但安装完毕就simple了 （ Writeby:Yeqishi 若有问题，欢迎询问）
1.首先从Protel原理图到处封装库就细说了，全选原理图，然后Make PCB library，保存就得到了*.PcbLib封装库
2.下载Protel2PCBTranslators 软件
[http://www.brsbox.com/filebox/down/fc/3ddf9160c090b65604b8914f8c67349b](http://www.brsbox.com/filebox/down/fc/3ddf9160c090b65604b8914f8c67349b)
3.解压缩包，按照软件中 说明 文档安装软件
4.解压的文件夹中alt2pads.exe即可将*.PcbLib封装库转化成.pt4封装库，此库应该就可以在PADS2005中使用了，而PADS2007还需要继续转换
5.在PADS2007安装完成后，自身有2005/2007封装库转换工具
大概位置为 “开始--所有程序--Mentor Graphics SDD--PADS2007--Library Converter ”
该软件即可将.pt4封装库转换成.pt07封装库，PADS2007就可以用了
注意：转换完成后，我遇到过丝印层线框不显示的情况，其实是转换后封装丝印层在第28层（底层丝印层），而该层颜色被设成黑色所以看不到了，改变LIne的颜色即可看到
//===================================================================================================
备注::
1》提供一个下载Protel2PCBTranslators软件可用的网址----[http://www.pc6.com/softview/SoftView_99559.html#download](http://www.pc6.com/softview/SoftView_99559.html#download)
