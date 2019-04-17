# C# 开发DirectX.DirectSound录音在Win10兼容解决方法 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月17日 15:58:00[boonya](https://me.csdn.net/boonya)阅读数：1193








最近做的[声音录制项目](https://github.com/BoonyaCSharp-ASP/DirectSoundTest)中用到了Microsoft.DirectX.DirectSound组件，win10默认的DirectX版本是DX12，如果不是请不要升级DirectX12。如果升级到DirectX12会出现：Microsoft.DirectX.DirectSound.PriorityLevelNeededException异常。DirectX是不提供卸载功能的，所以需要借助DirectX卸载和安装工具（DirectX随意卸）。我是在没有卸载DirectX12的情况下重新安装的DirectX11，使用命令行命令:dxdiag查看系统DX版本，但是安装DirectX11并不能覆盖调命令检查的结果，实际上还是DirectX12，所以这个不用太纠结。实际上DirectX随意卸界面基本信息里面就是检查当前系统的DX版本。

## 系统差异

官网上明确指出DirectX只支持Win7及以下的版本，如果要在win10下面用如果DirectX不升级是没有问题的。

win10:

![](https://img-blog.csdn.net/20180117160810710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


win7：

![](https://img-blog.csdn.net/20180117160841720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 下载DirectX11

百度下载地址：


[http://www.baidu.com/link?url=RyrgnZkhzNGfh7iSpNd44uKmmjO_PyBJVZ3uOKFWkcymju2qI1-0Vsh-5FTyAx_uPxPL2nkzIOEQ2rdAFnbugK&wd=&eqid=b359642800008779000000045a61a602](http://www.baidu.com/link?url=RyrgnZkhzNGfh7iSpNd44uKmmjO_PyBJVZ3uOKFWkcymju2qI1-0Vsh-5FTyAx_uPxPL2nkzIOEQ2rdAFnbugK&wd=&eqid=b359642800008779000000045a61a602)


## DirectX工具

1、基本信息




![](https://img-blog.csdn.net/20180117154937524?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


2、备份DirectX11

![](https://img-blog.csdn.net/20180117155003360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


3、卸载DirectX11

![](https://img-blog.csdn.net/20180117155116627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


4、还原DirectX11

![](https://img-blog.csdn.net/20180117155025919?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


注意：还原后系统需要重启，重启后dxdiag命令检查仍然是DirectX12，但是Microsoft.DirectX.DirectSound组件可以重新使用了。

推测：也许安装完DX11不用卸载、重启就可以使用了，有兴趣的可以试一下，理由是DX12和11可以共存。

**特别提醒：此方式对于老机器的显卡未必奏效。**

参考资料：[How to Deal with DirectX Problems](http://cn.bing.com/search?q=how+to+deal+with+Microsoft.DirectX.DirectSound.PriorityLevelNeededException&qs=n&form=QBLHCN&sp=-1&pq=how+to+deal+with+microsoft.directx.directsound.prioritylevelneededexception&sc=0-75&sk=&cvid=6B1AB4426B3048A0B392AC3EB2DD7F83)

DX卸载工具下载：[https://pan.baidu.com/s/1pNmOjtp](https://pan.baidu.com/s/1pNmOjtp)



