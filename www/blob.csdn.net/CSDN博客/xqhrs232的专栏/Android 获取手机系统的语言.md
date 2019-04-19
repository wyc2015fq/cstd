# Android 获取手机系统的语言 - xqhrs232的专栏 - CSDN博客
2012年04月03日 11:23:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：560标签：[android																[手机																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/jj_zerogra/article/details/5969961](http://blog.csdn.net/jj_zerogra/article/details/5969961)
相关网帖
1. Android获取当前系统语言及地区----[http://quanminchaoren.iteye.com/blog/969079](http://quanminchaoren.iteye.com/blog/969079)
如果想获取手机的当前系统语言，可以通过Locale类获取，主要方法：Locale.getDefault().getLanguage()，
通过Locale.getDefault().getCountry()获取当前国家或地区；如果当前手机设置为中文-中国，则使用此方法
返回zh-CN，同理可得到其他语言与地区的信息。
PS：开发者可根据系统语言的不同显示不同的内容。


