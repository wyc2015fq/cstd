# linux上anaconda的卸载 - 数据之美的博客 - CSDN博客
2017年06月16日 12:55:50[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：2041
声明：
本文通过参考下面三个博文而归结出[Linux](http://lib.csdn.net/base/linux)上anaconda的卸载方法。
anaconda安装及停用方法参考博文：http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/x_lock/article/details/53170321
卸载anaconda，启用spyder参考博文：http://blog.csdn[.net](http://lib.csdn.net/base/dotnet)/u010414589/article/details/51303502
anaconda的升级与卸载方法参考博文：http://blog.csdn[.Net](http://lib.csdn.net/base/dotnet)/yingyujianmo/article/details/50800568
正文：
**在ubuntu上卸载anaconda的步骤 ：**
**（一）删除整个anaconda目录：**
    由于Anaconda的安装文件都包含在一个目录中，所以直接将该目录删除即可。到包含整个anaconda目录的文件夹下，删除整个Anaconda目录：
    rm -rf anaconda文件夹名
（二）建议清理下.bashrc中的Anaconda路径：
   1.到根目录下，打开终端并输入：
      sudo gedit ~/.bashrc
   2.在.bashrc文件末尾用#号注释掉之前添加的路径(或直接删除)：
      #export PATH=/home/lq/anaconda3/bin:$PATH
      保存并关闭文件
   3.使其立即生效，在终端执行：
      source ~/.bashrc
   4.关闭终端，然后再重启一个新的终端，这一步很重要，不然在原终端上还是绑定有anaconda.
