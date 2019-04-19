# Linux Shell实现数据库定时备份 - fanyun的博客 - CSDN博客
2017年04月04日 19:33:21[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：386标签：[shell](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[脚本语言](https://blog.csdn.net/fanyun_01/article/category/6838309)
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)
      数据备份在日常开发中是经常要做的事情，备份[数据库](http://lib.csdn.net/base/mysql)是非常必须的，但是往往手动备份的效率都比较低，遇到需要周期性备份的数据和文件，我们就可以借助Shell大显身手了，Shell定期备份mysql数据库的例子：
     首先，确定mysql的目录：/usr/local/mysql/bin/
![](https://img-blog.csdn.net/20170404165625207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  备注：主要实现每天凌晨1点开始备份2个月内的数据
