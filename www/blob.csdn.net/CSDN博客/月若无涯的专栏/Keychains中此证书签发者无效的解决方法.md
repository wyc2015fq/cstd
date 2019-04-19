# Keychains中此证书签发者无效的解决方法 - 月若无涯的专栏 - CSDN博客
2016年02月16日 15:36:04[月若无涯](https://me.csdn.net/u010124617)阅读数：1296
最近很多人都遇到了“此证书签发者无效”这样的问题。 
其实问题并不在于我们自己生成的证书，而是苹果的WWDRCA证书过期了。 
解决方法如下： 
1. 下载最新的苹果WWDRCA证书，下载地址：[https://developer.apple.com/certificationauthority/AppleWWDRCA.cer](https://developer.apple.com/certificationauthority/AppleWWDRCA.cer)，下完安装。 
这时候可以检查是否之前的证书已经可用了，如果仍然不可用，则继续下面的操作。 
2. 打开keychains，选择顶部工具栏的“显示” -> “显示已过期的证书” 
3. 分别点击keychains界面中的“登录”“系统”，查看是否存在一个过期了的“Apple Worldwide Developer Relations Certification Authority”证书，如果存在就删除掉过期的这个证书。 
4. 再次查看之前的证书，应该都已经恢复可用了
