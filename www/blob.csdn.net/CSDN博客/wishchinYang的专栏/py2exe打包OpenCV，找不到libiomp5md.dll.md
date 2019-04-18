# py2exe打包OpenCV，找不到libiomp5md.dll - wishchinYang的专栏 - CSDN博客
2014年07月17日 16:56:01[wishchin](https://me.csdn.net/wishchin)阅读数：3706
问题：py2exe打包OpenCV，找不到libiomp5md.dll 
        解决方法：把 libiomp5md.dll 从numpy/core/ 里面复制到 python27/DLLS/文件夹！！！
