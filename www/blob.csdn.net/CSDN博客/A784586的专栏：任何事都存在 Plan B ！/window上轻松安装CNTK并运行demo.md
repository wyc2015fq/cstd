# window上轻松安装CNTK并运行demo - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月21日 16:09:59[QuJack](https://me.csdn.net/A784586)阅读数：2235








**window上安装CNTK并运行demo**

CNTK安装有其中方式：

windows上：Python-only installation；Script-driven installation；Manual installation

Linux上：Python-only installation；Script-driven installation；Manual installation；Docker installation；


本文Python-only installation，在windows上PC 64 bit上安装CNTK，only-CPU版本；

1.首先必须安装Anaconda：一个开源的 Python发行版本，包含了
 conda、Python 等 180 多个科学包及其依赖项。

下载地址：https://github.com/Microsoft/CNTK/wiki/Setup-Windows-Python
 点击超链接：[Anaconda3 4.1.1 Python for Windows (64-bit)](https://repo.continuum.io/archive/Anaconda3-4.1.1-Windows-x86_64.exe).

由于文件较大，如果下载慢，从**百度云**地址：http://pan.baidu.com/s/1dFIfoYX

下载成功后傻瓜式双击安装。

2.
 打开cmd: > pip install https://cntk.ai/PythonWheel/CPU-Only/cntk-2.0rc1-cp35-cp35m-win_amd64.whl

3.测试是否安装成功： C:\> python -c "import cntk;
 print(cntk.__version__)"

![](https://img-blog.csdn.net/20170421140517896?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20170421140525286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


4.安装CNTK的例子：python -m cntk.sample_installer

可能我网速不好，这个一直不结束，手动浏览器输入地址下载：https://cntk.ai/Samples/CNTK-Samples-2-0rcl.zip

然后解压后把文件复制到某个文件夹下即可。

由于文件较大，如果你的网速很慢，扛不住，百度云：http://pan.baidu.com/s/1hrLgHBu

5.进入例子目录运行例子：

cd Tutorials

cd NumpyInterop

python FeedForwardNet.py


运行效果图：我把手动下载的两个文件Examples，Tutorials放在C:\Users\Administrator\mycntkdemo下![](https://img-blog.csdn.net/20170421155604938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

备注：

官方安装说明：https://github.com/Microsoft/CNTK/wiki/Setup-CNTK-on-your-machine

可以尝试其他的安装方法：

https://github.com/Microsoft/CNTK/wiki/Setup-Windows-Binary-Script
















