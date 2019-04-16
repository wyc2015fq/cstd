# Android开发中遇到的问题(二)——新建android工程的时候eclipse没有生成MainActivity和layout布局 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Android开发中遇到的问题(二)——新建android工程的时候eclipse没有生成MainActivity和layout布局](https://www.cnblogs.com/xdp-gacl/p/4320736.html)



## 一、新建android工程的时候eclipse没有生成MainActivity和layout布局

　　最近由于工作上的原因，开始学习Android开发，在入门的时候就遇到了不少的坑，遇到的第一个坑就是"**新建android工程的时候eclipse没有自动生成MainActivity和layout布局**”，项目的创建过程如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/071817157588881.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/071818228679215.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/071819411499431.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/071820560552316.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/071822020086665.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/071823137894762.png)

　　展开HelloAndroid项目，可以看到创建好的项目的目录结构，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/071632318671750.png)

　　我的项目是采用的是官方集成了ADT的Eclipse(adt-bundle-windows-x86_64-20140702)创建的，创建项目时选择的是【Blank Activity】，结果生成的项目的目录结构就成了如上图所示的样子，正常情况下，Eclipse会在src目录下创建一个名字为me.gacl.helloandroid的包，然后里面会有一个MainActivity.java的文件，然后layout文件夹中会有一个activity_main.xml文件，但是现在这两个都没有生成。由于HelloAndroid项目中没有可运行的Activity，那么直接运行HelloAndroid项目是无法运行的，如下图所示：

　　在Eclipse中运行Android项目，操作步骤如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/071835312746266.png)

　　运行会在Console打印出 "No Launcher activity found!"，并且模拟器也不运行，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/071839331333890.png)

　　下面说一下"No Launcher activity found！"错误，叫Android的项目中都会有一个AndroidManifest.xml配置文件，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/071848406173682.png)

　　我们看一下AndroidManifest.xml里面的代码，如下所示：

```
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="me.gacl.helloandroid"
    android:versionCode="1"
    android:versionName="1.0" >

    <uses-sdk
        android:minSdkVersion="8"
        android:targetSdkVersion="21" />

    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
    </application>

</manifest>
```

　　可以看到，里面没有配置启动项，原因是因为Eclipse在创建项目时没有帮我们自动生成MainActivity.java和activity_main.xml文件，这应该是Eclipse在创建【Blank Activity】的bug，在<application></application>中少了这段代码

```
<activity android:name=".MainActivity" android:label="@string/app_name">

            <intent-filter>

                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />

            </intent-filter>

</activity>
```

　　其中.MainActivity就是你的项目的主Activity，所以凡是遇到"No Launcher activity found！"错误，那么绝对是AndroidManifest.xml文件中没有配置启动的主Activity

## 二、新建android工程的时候eclipse没有生成MainActivity和layout布局的解决办法

　　解决办法：**在创建项目时把【Blank Activity】换成【Empty Activity】就可以解决上述所说的问题！**

　　删除HelloAndroid项目，重新创建，前面的创建步骤都一样，就是到【Create Activity】时选择【Empty Activity】，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/071901153839601.png)

　　很多视频教程在【Create Activity】这一步时选择的都是【Blank Activity】，但是经过我实践证明，【Create Activity】时这一步时如果选择的是【Blank Activity】，那么创建出来的Android项目就会出现"**新建android工程的时候eclipse没有自动生成MainActivity和layout布局**”**的问题**，**也就是没有生成MainActivity.java和activity_main.xml文件，缺少了MainActivity的Android项目是无法运行的，运行时就会出现上述所说的"No Launcher activity found！"错误。**所以到了这一步千万要注意，【Create Activity】这一步时选择的是【Empty Activity】，而不是【Blank Activity】。这或许和我自身使用的Eclipse版本有关系，我使用的采用的是官方集成了ADT的Eclipse(adt-bundle-windows-x86_64-20140702)，而有些版本的Eclipse在【Create Activity】这一步时选择【Blank Activity】是可以正常生成MainActivity和layout布局的，反正创建项目到【Create Activity】这一步时，如果自身使用的Eclipse版本在【Create Activity】这一步时选择【Blank Activity】可以正常生成MainActivity和layout布局的话，那么【Create Activity】就可以选择【Blank Activity】作为创建模板，如果不行，那么【Create Activity】就选择【Create Activity】作为创建模板，反正我们的目的就是希望Eclipse自动帮我们生成MainActivity和layout布局，哪一种模板可以做到就选择哪一种。

![](https://images0.cnblogs.com/blog2015/289233/201503/071922556491739.png)

　　点击【Finish】按钮完成创建，这样创建的android项目就会自动生成MainActivity.java和activity_main.xml文件，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/071926588527106.png)

　　我们看看AndroidManifest.xml中的代码：

```
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="me.gacl.helloandroid"
    android:versionCode="1"
    android:versionName="1.0" >

    <uses-sdk
        android:minSdkVersion="8"
        android:targetSdkVersion="21" />

    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        <activity
            android:name=".MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>

</manifest>
```

　　在<application></application>中多了这段代码

```
<activity
            android:name=".MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
</activity>
```

　　我们直接运行创建的Android项目，如下图所示：
![](https://images0.cnblogs.com/blog2015/289233/201503/071835312746266.png)

　　运行Android应用时，是需要在模拟器上面运行的，到这一步时首先是要创建一个模拟器的，不过在Eclipse中创建模拟器时启动要花很长时间，刚好我手头上有一台台电的平板电脑，可以用于测试，因此就直接使用真机测试了，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/071937534614294.png)

　　在平板电脑中的运行结果如下：

![](https://images0.cnblogs.com/blog2015/289233/201503/071956177743700.png)

　　平板电脑的桌面上也显示出了我们安装部署上面的HelloAndriod应用的图标，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/071949198998003.png)









