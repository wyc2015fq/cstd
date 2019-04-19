# Failed to create provisioning profile----本人备注 - xqhrs232的专栏 - CSDN博客
2019年01月04日 11:47:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：444
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://stackoverflow.com/questions/39603667/failed-to-create-provisioning-profile](https://stackoverflow.com/questions/39603667/failed-to-create-provisioning-profile)
相关文章
1、Signing for "xxx" requires a development team. Select a development team----[https://blog.csdn.net/wujakf/article/details/70171240](https://blog.csdn.net/wujakf/article/details/70171240)
2、Signing for "xxx" requires a development team. Select a development team in the project editor. Code signing is required for product type 'Application' in SDK 'iOS 10.0'----[https://www.jianshu.com/p/218fe8d02c75](https://www.jianshu.com/p/218fe8d02c75)

//============================================================================================
本人备注::
1>In my case this error disappeared when I changed the "Bundle Identifier" to something less generic (e.g. adding some random numbers at the end)-------project/targets/general/Bundle Identifier
2>xcode关闭，重新打开一下
