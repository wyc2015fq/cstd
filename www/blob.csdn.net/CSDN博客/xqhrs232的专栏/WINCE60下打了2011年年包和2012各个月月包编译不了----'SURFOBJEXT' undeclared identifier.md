# WINCE60下打了2011年年包和2012各个月月包编译不了----'SURFOBJEXT'  undeclared  identifier - xqhrs232的专栏 - CSDN博客
2013年02月03日 13:35:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：912
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
需要去下载2012年的年包更新----[http://www.microsoft.com/en-us/download/details.aspx?id=36505](http://www.microsoft.com/en-us/download/details.aspx?id=36505)
其随带的文档里面提到了这个错误的更新！！！
仔细看了一下，其实2011年的年补丁包就修订了这个问题------问题编号为[111208_KB2571456](http://support.microsoft.com/kb/2571456) - This update adds suppport for running CT/AA fonts
 on all surfaces when driver is enabled for this capability, adds a new graphics flag GCAPS_AA_ALLSURFACES for backward compatibility, and adds SURFOBJEXT to winddi.h for CT/AA rendering.
