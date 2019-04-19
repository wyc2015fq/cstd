# mac系统Anaconda安装 - 数据之美的博客 - CSDN博客
2018年08月26日 18:01:36[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：913
**目录**
[下载安装包](#%E5%AE%89%E8%A3%85%E5%8C%85)
[安装过程](#%E5%AE%89%E8%A3%85%E8%BF%87%E7%A8%8B)
[验证安装是否成功](#%E9%AA%8C%E8%AF%81%E5%AE%89%E8%A3%85%E6%98%AF%E5%90%A6%E6%88%90%E5%8A%9F)
步骤比较简单，如果整个流程中任何一步出现了问题导致无法安装成功，请联系我
# 下载安装包
官网下载安装包 [https://www.anaconda.com/download/#macos](https://www.anaconda.com/download/#macos)
可以选择对应python2、python3的安装文件有下面图形化界面安装包和命令行安装包
![](https://img-blog.csdn.net/20180826180701667?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 安装过程
我下载完的安装包是bash Anaconda2-5.2.0-MacOSX-x86_64.sh，可以直接输入命令行
bash Anaconda2-5.2.0-MacOSX-x86_64.sh，安装即可！
回车下一步
![](https://img-blog.csdn.net/20180826180757587?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
输入yes，同意license遵守相关规则，回车下一步
![](https://img-blog.csdn.net/20180826180939712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
回车下一步
![](https://img-blog.csdn.net/2018082618455031?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
installation finished，指定依赖保存位置，默认输入yes，下一步
![](https://img-blog.csdn.net/20180826184631277?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
不用管，下一步
![](https://img-blog.csdn.net/20180826184750663?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
结束安装过程！
# 验证安装是否成功
需要验证是否安装成功
vim ~/.bash_profile，查看是否添加anaconda2的依赖库到source文件
![](https://img-blog.csdn.net/20180826184903736?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
或者直接conda list，查看是否有相应的安装包
恭喜，Anaconda2安装过程到这里结束！
可以愉快的使用了。
附
打开python输入窗口，可以看到如下，说明现在python是从anaconda中打开的，正常的使用就ok
![](https://img-blog.csdn.net/20180826185104951?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29wcG82MjI1ODgwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
