# iOS开发 提交应用出现this bundle is invalid. armv7s are required to include armv7 architecture - 月若无涯的专栏 - CSDN博客
2013年11月15日 14:35:59[月若无涯](https://me.csdn.net/u010124617)阅读数：4983标签：[iOS																[上传应用																[错误](https://so.csdn.net/so/search/s.do?q=错误&t=blog)](https://so.csdn.net/so/search/s.do?q=上传应用&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/u010124617/article/category/1393991)
前两天提交应用，出现了
ERROR ITMS-9000: "this bundle is invalid. armv7s are required to include armv7 architecture." at SoftwareAssets/SoftwareAsset
 (MZItmspSoftwareAssetPackage).
这样的错误，通过搜索和检查，在stackoverflow找到了解决方案。
原因在于 arch
 (arm7, arm7s, arm64) + Build Active Arch Only - YES.
解决方法：把工程配置的中的Build
 Active Arch Only这个参数的值，release情况下的置为NO就可以了。
原英文见：http://stackoverflow.com/questions/19211654/ios-application-loader-shows-bundle-error
