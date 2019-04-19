# SSH远程链接:SCP远程拷贝文件与文件夹 - Koma Hub - CSDN博客
2017年12月16日 22:30:56[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1266
**scp是ssh的一部分，可以进行远程文件拷贝。**
下面以本地A和远程机器B为例介绍：
首先本地A:
![](https://img-blog.csdn.net/20171216223320370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
用户名为rongtao，ip地址为:180.201.178.4
远程机器B的信息如下：
![](https://img-blog.csdn.net/20171216223526605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
远程用户名Toa，ip地址:172.18.174.84
在本地机器A上进行如下操作，可以讲文件拷贝至远程机器B上:
![](https://img-blog.csdn.net/20171216223820970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在远程机器B上进行如下操作，可以将文件拷贝至本地机器上:
![](https://img-blog.csdn.net/20171216223949794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**远程拷贝文件夹的方法为:**
![](https://img-blog.csdn.net/20171216224513299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
拷贝到本地A如下所示:
![](https://img-blog.csdn.net/20171216224750064?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
