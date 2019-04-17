# Android百度地图HelloWorld运行异常处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年08月25日 16:47:20[boonya](https://me.csdn.net/boonya)阅读数：2784








     Android新手百度地图HelloWorld运行异常，所有的步骤都是按照百度官方操作来的但是启动的时候报错，而且这些错误很离谱.后来拿来官方的示例程序也是有相关的错误的，最后发现主要的问题在于AVD版本没有和SDK的版本对应起来，另外地图的ak签名需要用系统cmd方式获取而不是Eclipse来获取（这两种方式获取到的SHA1签名不一致）。


#### **1.AVD版本不对应Android sdk的版本导致程序启动不起来**

源码见此处：[http://developer.baidu.com/map/wiki/index.php?title=androidsdk/guide/hellobaidumap](http://developer.baidu.com/map/wiki/index.php?title=androidsdk/guide/hellobaidumap)，错误如下：

![](http://static.oschina.net/uploads/space/2014/0825/120143_LMnM_592236.png)

找了很久其实不是API的问题，也不是程序的问题而是AVD 模拟器的问题，我用的是Android的4.4.w所以虚拟机也要创建相同的版本，不然就有如上意想不到的事情发生。

#### **2.签名验证失败调用地图API地图无法显示**

总是出现如下图所示的界面：

![](http://static.oschina.net/uploads/space/2014/0825/162931_wcjV_592236.png)

这是由于SHA1签名不匹配造成的。将百度APP创建的项目及ak删掉重新创建，得到新的ak。ak生成方式见：[http://developer.baidu.com/map/sdkandev-14.htm](http://developer.baidu.com/map/sdkandev-14.htm)，用cmd方式获取到SHA1的ak才行。

#### **3.测试重新注册后的ak**

在AndroidManifest.xml中加入如下代码的：
   <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >

        <!-- 配置百度地图key -->
        <meta-data
            android:name="com.baidu.lbsapi.API_KEY"
            android:value="GaCKiZmOn99Vb7uDEWfrlV3b" />
            ......................

启动AVD测试，如下测试成功：

![](http://static.oschina.net/uploads/space/2014/0825/163638_FVV7_592236.png)​

注：oschina(开源中国)上有人说百度官网上的API更新不及时，所以导致程序运行不起来，这个可能未必是这样，不要把所有的问题归结为百度地图本身的问题，否则就会阻止自己成长和提高了；多从自己的角度出发，利用工具去发现问题寻找问题的突破口才是最佳实践之道。






