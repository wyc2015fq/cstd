# 解决Android SDK Manager更新失败问题 - =朝晖= - 博客园
# [解决Android SDK Manager更新失败问题](https://www.cnblogs.com/dhcn/p/7105630.html)
from:[http://www.ztyhome.com/android-sdk-update/](http://www.ztyhome.com/android-sdk-update/)
**问题描述:**
使用SDK Manager更新时无法完成
更新ADT时无法解析https://dl-ssl.google.com/[Android](http://lib.csdn.net/base/android)/eclipse
          一般在这个连接界面一直停留，无法连接，只能点击Close结束。
![](http://ztyhome.gotoip2.com/wp-content/uploads/2012/04/SDK-update.jpg)
  或者是直接如上般显示
> 
Failed to fetch URL https://dl-ssl.google.com/[android](http://lib.csdn.net/base/android)/repository/repository-6.xml, reason: Connection to https://dl-ssl.google.com refused
Failed to fetch URL http://dl-ssl.google.com/android/repository/addons_list-1.xml, reason: Connection to http://dl-ssl.google.com refused
Failed to fetch URL https://dl-ssl.google.com/android/repository/addons_list-1.xml, reason: hostname in certificate didn’t match: <dl-ssl.google.com> != <www.google.com>
问题分析：
        由于某些众所周知又无法理解的原因，我们大陆使用Google的服务会出现种种问题，譬如[Android](http://www.ztyhome.com/tag/android-2/)开发也会出现阻碍。不过首先要说明的是一般情况下使用SDK Manager更新或者更新Eclipse的ADT插件是没有问题的，我以前也能正常更新，但是昨天不知道节点抽什么风，压根无法连接服务器，出现了上边的种种问题，下面说一下如果网络抽风的话应该如何解决问题。
### **解决办法：**
**第一种方法：**
一劳永逸，直接配置VPN（可以直接去淘宝买），但是现在想找个速度快又稳定还免费的VPN实在不易，尤其是更新SDK，以几kb/s的速度一个文件需要400多分钟，所以也就放弃了VPN。
**第二种方法：**
使用http协议而不是https协议
在SDK Manager的操作界面下，Tools->Options打开了SDK Manager的Settings，选中“Force https://… sources to be fetched using http://…”，强制使用http协议。重启SDK Manager.exe
而在更新ADT插件的时候则使用网址http://dl-ssl.google.com/android/eclipse，而不是https://dl-ssl.google.com/android/eclipse，这个在官方开发文档里也有介绍。
补充说明：个别用户这样设置可能没效果。
**第三种方法：**
        比较麻烦的方法，就是直接打开
https://dl-ssl.google.com/android/repository/addons_list.xml
https://dl-ssl.google.com/android/repository/repository.xml
https://dl-ssl.google.com/android/repository/addon.xml
这几个文件，找到你要下载的文件名，直接用迅雷下载，ADT可以直接在官网下载ADT包进行安装。具体方法自己搜索。
**最好的方法：**
那就是修改hosts文件的方法，更新速度较快。
        Windows在C:\WINDOWS\system32\drivers\etc目录下，[Linux](http://lib.csdn.net/base/linux)用户打开/etc/hosts文件。打开文件后添加以下内容，直接放到最后就可以了。
> 
#Google主页
203.208.46.146 www.google.com
#这行是为了方便打开Android开发官网 现在好像不FQ也可以打开
74.125.113.121 developer.android.com
#更新的内容从以下地址下载
203.208.46.146 dl.google.com
203.208.46.146 dl-ssl.google.com
      添加以上代码后，再用第二种方法中提到的选用http协议，就可以正常的进行更新了。

