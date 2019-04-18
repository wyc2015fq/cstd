# Eclipse离线安装ADT插件 - happyhorizon的算法天空 - CSDN博客
2015年04月03日 16:02:15[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：392标签：[linux																[android																[eclipse																[adt](https://so.csdn.net/so/search/s.do?q=adt&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[linux](https://blog.csdn.net/happyhorizion/article/category/3079971)
Eclipse安装ADT插件,但是由于某些不可抗拒的原因连上[https](https://dl-ssl.google.com/android/eclipse/)[://dl-ssl.google.com/android/eclipse/](https://dl-ssl.google.com/android/eclipse/) 后，始终无法更新ADT插件(卡死在Fetchingcontent.jar上)。解决办法：
1、在[http://developer.android.com/sdk/installing/installing-adt.html网站上搜索最新的](http://developer.android.com/sdk/installing/installing-adt.html%E7%BD%91%E7%AB%99%E4%B8%8A%E6%90%9C%E7%B4%A2%E6%9C%80%E6%96%B0%E7%9A%84ADT%E7%89%88%E6%9C%AC%E5%8F%B7)[ADT版本号](http://developer.android.com/sdk/installing/installing-adt.html%E7%BD%91%E7%AB%99%E4%B8%8A%E6%90%9C%E7%B4%A2%E6%9C%80%E6%96%B0%E7%9A%84ADT%E7%89%88%E6%9C%AC%E5%8F%B7)；当前是23.0.6
2、在浏览器地址栏输入：https://dl.google.com/android/ADT-23.0.6.zip，自动开始下载。
3、Eclipse---Help---InstallNew Software---Add，出现如下窗口
Name自己填，点击Archive，选择本地电脑中的ADT-23.0.6.zip文件
然后，在Workwith的列表中找到刚才添加的这一项
注意下面最后一项Contactall update sites**不要选**，否则有可能卡死，其他的跟正常一样SelectAll或者自定义需要更新的项，一路Next，就可以了。
