
# pycharm之no python interpreter configured for project的解决办法 - Magician的博客 - CSDN博客


2017年09月17日 20:19:27[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：43988


今天由于重装了系统，所以必须得重新配置一些软件，在打开pycharm运行程序时显示“no python interpreter configured for project”提示。根据字面意思是“python没有解释器”。
解决办法：
找到你之前下载python环境支持库如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20170917201330460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在pycharm中点击File->setting进行如下图操作，选中支持库中pythonw.exe文件作为解释器即可解决问题。
![这里写图片描述](https://img-blog.csdn.net/20170917201606093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当然如果你需要安装python的其他包如numpy，scipy，sklearn等，安装过程请参考：[http://blog.csdn.net/qq_16633405/article/details/56682025](http://blog.csdn.net/qq_16633405/article/details/56682025)

