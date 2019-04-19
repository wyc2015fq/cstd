# 如何解决Error - cannot open input file C /postproc/nlscfg.inf fmerge for Windows CE (Release) - xqhrs232的专栏 - CSDN博客
2014年04月12日 14:39:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1133
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/wxswx1987/article/details/5825560](http://blog.csdn.net/wxswx1987/article/details/5825560)
上篇博文介绍，修改完时间后，编译产生如下错误：
Error - cannot open input file C<...> /postproc/nlscfg.inf  fmerge for Windows CE (Release)
Google了下解决方法，这里直接引用博文：
按照向导建立一个新的平台，编译，然后出现如下错误。
makeimg: Creating <...> nlscfg.out because <... > nlscfg.inf doesn't exist. 
makeimg: run command: fmerge -nls <....> nlscfg.out <... > nlscfg.inf 
Error - cannot open input file C<...> /postproc/nlscfg.inf  fmerge for Windows CE (Release)
makeimg: FATAL ERROR: Command returned non-zero exit code 1 (dec). 
makeimg: FATAL ERROR: Command returned non-zero exit code 1 (dec).
在PB的Menu/ Build OS/ Copy files to release directory手动复制一次，即解决了找不到nlscfg.inf 文件的错误。
然而又出现了：
Error: Could not find file '/WINCE420/PUBLIC/videonimage/RelDir/CIRRUS_EP931X_ARMV4IRelease/kernkitl.exe' on disk
nk.exe /WINCE420/PUBLIC/videonimage/RelDir/CIRRUS_EP931X_ARMV4IRelease/kernkitl.exe NK SH 
Error: failed setting line
makeimg: FATAL ERROR: Command returned non-zero exit code 1 (dec).
makeimg: FATAL ERROR: Command returned non-zero exit code 1 (dec).
Error PB2505: Error executing #(/WINCE420/PUBLIC/videonimage/RelDir/CIRRUS_EP931X_ARMV4IRelease/)makeimg.exe.
又找不到kernkitl.exe了。
然后我在wince500/目录下搜索“kernkitl.exe”,找到一个原有的，复制到缺少的目录中，再次编译，即通过。
kernkitl.exe是调试用的工具对应的程序。
070329_2440test - 0 error(s), 0 warning(s)
//==================================================================================================
备注::
1>觉得应该是OS那边没有成功编译完成，所以没有产生这些配置文件

