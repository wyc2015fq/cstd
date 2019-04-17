# sudo pip install -r requirements.txt出现***has requirement **，but you'll have ** which is incompatible - Snoopy_Dream - CSDN博客





2018年05月04日 15:29:38[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：724








配置caffe前期，出现sudo pip install -r requirements.txt出现***has requirement **，but you'll have ** which is incompatible。比如：


matplotlib 2.2.2 has requirement python-dateutil>=2.1, but you'll have python-dateutil 1.5 which is incompatible.

![](https://img-blog.csdn.net/20180504152720682?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




根据提示输入：

 sudo pip install --upgrade python-dateutil






