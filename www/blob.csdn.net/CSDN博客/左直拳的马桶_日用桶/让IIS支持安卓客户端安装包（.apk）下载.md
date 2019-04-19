# 让IIS支持安卓客户端安装包（.apk）下载 - 左直拳的马桶_日用桶 - CSDN博客
2013年02月05日 11:29:50[左直拳](https://me.csdn.net/leftfist)阅读数：3020
把安卓客户端安装包（.apk）放到网站上，供用户下载，鼠标点击这个链接，结果浏览器出现那种查找不到页面的错误。
解决方法：
打开IIS，在网站里添加一个添加一个MIME类型：
扩展名中填写“.apk”,MIME类型中填写apk的MIME类型“ application/vnd.android.package-archive ”
另外，手机浏览器有很顽固的缓存，所以下载安装包的链接应该加上个参数，避免安装包升级了，然后手机还是下载到旧的。如：
<a href="/tool/test.apk?ver=20130205">立即下载</a>
其实，ver这个就是没有什么意义的参数。
参考文章：
http://www.juyo.org/juyo/server/iis-mime-apk/
