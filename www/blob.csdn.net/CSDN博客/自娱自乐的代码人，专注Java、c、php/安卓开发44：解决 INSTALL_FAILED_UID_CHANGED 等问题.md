# 安卓开发44：解决 INSTALL_FAILED_UID_CHANGED 等问题 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年10月10日 17:50:16[initphp](https://me.csdn.net/initphp)阅读数：2414
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









apk无法卸载，一般可以下面的方法试一下：




1. 删除/data/app/(filename) 文件夹下的apk包

2. 删除/system/app/(filename) 文件夹下的apk包

3. 删除/data/data/(filename) 文件夹下相关信息


4. 将/data/system/packages.xml文件pull出来删除里面相关的包信息，然后push上去

5. 重启后生效




