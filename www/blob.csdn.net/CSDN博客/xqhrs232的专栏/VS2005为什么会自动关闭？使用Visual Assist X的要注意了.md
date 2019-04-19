# VS2005为什么会自动关闭？使用Visual Assist X的要注意了 - xqhrs232的专栏 - CSDN博客
2010年09月11日 10:20:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1799
原文地址::[http://www.cnblogs.com/yanrongpi/archive/2007/05/27/479310.html](http://www.cnblogs.com/yanrongpi/archive/2007/05/27/479310.html)
【现象描述】
        打开一个Solution，加载了所有的工程，当从Solution Explorer上双击打开一个代码文件的时候，VS2005自动退出了。当我再次打开该Solution的时候，VS2005每次都是自动退出。
【前因】
        因为网络故障，VSS提示网络不存在，然后每次打开Solution的时候都会出现这个问题。
【烦恼的解决过程】
        首先，我怀疑是不是VSS有问题，卸载了VSS重新安装了VSS，但是故障依旧。没办法只好卸载VS005，重新安装，并安装了我常用的VS005的一些插件，如Visaul Assist X，但是故障依然涛声依旧。我以为是VS2005版本的问题，卸载了VS005安装了不同的语言版本，故障依旧。此时我并没有怀疑是因为插件的原因，一狠心重新安装了操作系统，并重新安装了所有的开发工具，包括Visual Assist X等插件，开始的时候工作很好，但是后来又因为一次网络故障，让我火冒三丈的事情发生了，所有的故障又重新发生了。我现在怀疑是不是Visual
 Assist X的问题，卸载了它，卸载了VS2005，重新安装，我故意断开网络，OK，故障排除了。该死的Visual Assist X。注意，我使用的是Visual assist X 10.2.0.2445版本。
【结论】
Visual assist X是个好东西，我很喜欢。但是所有的VS2005的插件并不是都是稳定的，使用的时候要特别小心，现在很多公司都喜欢在VS上搞个插件集成，别的不说，插件多了，启动VS的时候慢的一塌糊涂。该割爱的时候还是要割爱啊。真搞不明白，为什么一定要集成到VS中，Standalone不是很好吗？
要是有一个插件集中了Resharper的Refator功能，Visual assist X的代码提示功能，CodeRush的Outline功能就完美了。 
