# 三种mac os系统镜像文件在虚拟机上如何使用 - xqhrs232的专栏 - CSDN博客
2018年12月12日 15:39:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：37
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
相关文章
1、U盘安装Mac High Sierra（制作U盘启动必须是在mac系统中）----[https://blog.csdn.net/sunxiaoju/article/details/80783269](https://blog.csdn.net/sunxiaoju/article/details/80783269)
1>cdr格式的-----直接在虚拟机下设置引用他就好，一般下载懒人版，可以直接去安装了，但也不是所有的cdr文件能直接用
 2>dmg格式的----用于制作mac os系统的引导u盘----通过u盘安装系统-----必须在mac os环境下去制作，通过windows下的工具制作试了好像是不行的，dmg格式文件本来也是mac os系统的文件格式
 3>iso格式的----直接在虚拟机下设置引用他就好,iso可以通过dmg转换而来，但我转换过还是是不行的，dmg要不不能被UltraISO.exe转换工具识别，要不转出来的ISO文件也用不了
