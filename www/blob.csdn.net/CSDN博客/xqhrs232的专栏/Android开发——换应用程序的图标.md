# Android开发——换应用程序的图标 - xqhrs232的专栏 - CSDN博客
2012年01月09日 10:41:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4384
原文地址::[http://hi.baidu.com/yongfeng_liang/blog/item/67f5ba19a3d6b4ccac6e754d.html](http://hi.baidu.com/yongfeng_liang/blog/item/67f5ba19a3d6b4ccac6e754d.html)
Android默认的应用程序图标太单一，而且我们也不想每个应用程序都是一个图标，这就需要我们在开发时，更改为符合程序要求并突出我们的个性的图标，方便找到，而且能吸引眼球！！
==============方法一================
步骤如下：
1。打开Android工程，找到AndroidManifest.xml。
<application android:icon="@drawable/icon" android:label="@string/app_name">
....
</application>
这里的icon就是应用程序的图标了
2. 在res文件夹下面新建一个drawable（不要拼错哦），把自己喜欢的图标扔进去，本例中，命名为icon.png。
3. 把res下的drawable-hdpi、drawable-ldpi、drawable-mdpi文件夹下面的icon.png图标删掉。
好了，大功告成，很简单。
==============方法二==============
直接把res下的drawable-hdpi、drawable-ldpi、drawable-mdpi文件夹下面的icon.png图标替换成你的图标。
更直接！
