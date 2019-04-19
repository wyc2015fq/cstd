# Mac系统上安装xgboos教程---坑 - 数据之美的博客 - CSDN博客
2018年09月04日 23:52:07[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：73
标准教程
1.下载安装包
建议直接官方网站，即github上面
git clone --recursive [https://github.com/dmlc/xgboost](https://github.com/dmlc/xgboost)
2.安装过程
a.cd xgboost；
b.cp make/config.mk .   ；   #将make下面的config.mk文件复制到xgboost的根目录
c.修改刚才复制到根目录下的config.mk文件，将下面注释去掉   #注意这里只修改根目录下的config.mk文件，不用修改make下的config.mk文件文件
# export CC = gcc
# export CXX = g++
d.cd /usr/local/bin，查看里面的gcc和g++是那个版本，例如我的是gcc-5和g++-5，那么将上面取掉注释为文件修改为  #注意这里只修改根目录下的config.mk文件，不用修改make下的config.mk文件文件
export CC = gcc-5
export CXX = g++-5
d.make -j4
e.**如果上述编译过程出现了各种解决不了的问题的话**，那么可以尝试 下面的命令
   bash build.sh；
   cd python-package；
   sudo python setup.py install；
即可安装成功！
安装过程中容易遇到的坑可见 如下链接
[https://blog.csdn.net/oppo62258801/article/details/80138065](https://blog.csdn.net/oppo62258801/article/details/80138065)
[https://blog.csdn.net/oppo62258801/article/details/80137729](https://blog.csdn.net/oppo62258801/article/details/80137729)
