# SLAM:(编译ORB)fatal error LNK1181: 无法打开输入文件“libboost_mpi-vc110-mt-1_57.lib” - wishchinYang的专栏 - CSDN博客
2016年05月26日 16:37:56[wishchin](https://me.csdn.net/wishchin)阅读数：1758
          对于使用MD版本编译的ORB_SLAM，会用到MPI版本的Boost，需要自己编译，比较麻烦。
         因此使用MT版本进行生成，暂时无法完成。
工程配置
        发现添加库文件使用了：从父级或项目默认继承，默认包含了libboost_mpi-vc110-mt-1_57.lib
        去掉使用即可
编译成功...

