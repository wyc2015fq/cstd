# 如何在windows下安装xgboost库-朝闻道-51CTO博客
（0）前提是，你得下载好anaconda，并且安装之，我的下载地址如下（python3 windows 64位）
[https://repo.continuum.io/archive/Anaconda3-4.4.0-Windows-x86_64.exe](https://repo.continuum.io/archive/Anaconda3-4.4.0-Windows-x86_64.exe)
（1）下载xgboost源码(这里直接用官方最新的源码，这里我们不需要用git clone --recursive,因为用的是编译好的dll，因此不需要下载那么完整，只需要python-package完整即可),你可以通过下面的连接下载源码即可。
[https://github.com/dmlc/xgboost/archive/master.zip](https://github.com/dmlc/xgboost/archive/master.zip)
（3）解压下载好的源码
解压之后，进入文件夹，找到master\python-package\xgboost目录
（4）下载windows下的编译好的xgboost库文件
选择最新的编译的dll，也就是当前源码对应的那一天的dll
大家请在页面：[http://ssl.picnet.com.au/xgboost/20180515/x64/xgboost.dll](http://ssl.picnet.com.au/xgboost/20180515/x64/xgboost.dll)
选择最新编译的dll，因为最新的dll与xgboost的源码更为适配。
下载好之后将这个库文件放入到master\python-package\xgboost目录
![如何在windows下安装xgboost库](https://s1.51cto.com/images/blog/201805/15/848876390459fb13e66badbd354cbe0b.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
注意：如果你有NVIDIA的GPU，可以下载http://ssl.picnet.com.au/xgboost/20170610/x64_gpu/libxgboost.dll，这个是GPU版本的动态链接库
（5）安装xgboost即可
进入master\python-package目录，执行以下命令行
python setup.py install  
注意这里的python必须使用 anaconda下面的那个python.exe 需要设置环境变量 否则不能使用 anaconda里面装上的numpy库
![如何在windows下安装xgboost库](https://s1.51cto.com/images/blog/201805/15/24700ab7c05c71c0eb830ae4bc7d797c.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)![如何在windows下安装xgboost库](https://s1.51cto.com/images/blog/201805/15/906ab9a0575630e93cb69e0c964d9a1e.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
即可完成安装
![如何在windows下安装xgboost库](https://s1.51cto.com/images/blog/201805/15/0e58201a36fad97206fb1d71d1edf819.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
