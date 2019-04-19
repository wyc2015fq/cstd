# Android 为【apk】文件签名，增加修改系统时间等权限 - xqhrs232的专栏 - CSDN博客
2011年03月07日 16:55:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1516
原文地址::[http://terryblog.blog.51cto.com/1764499/389843](http://terryblog.blog.51cto.com/1764499/389843)
昨天写应用，涉及到需要修改系统时间等操作，查了一下Android 的设置源码，里面的一个方法如下：
```
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->   public void onDateSet(DatePicker view, int year, int month, int day) {
        Calendar c = Calendar.getInstance();
        c.set(Calendar.YEAR, year);
        c.set(Calendar.MONTH, month);
        c.set(Calendar.DAY_OF_MONTH, day);
        long when = c.getTimeInMillis();
        if (when / 1000 < Integer.MAX_VALUE) {
            SystemClock.setCurrentTimeMillis(when);
        }
        updateTimeAndDateDisplay();
    }
```
从上面的 Android 应用程序源码可以看出，修改系统日期需要为Calendar 设置年月日，然后通过调用 SystemClock.setCurrentTimeMillis 方法，设置系统日期，既然源码是这样写的，那我们也依样画葫芦来学一下吧，下面是通过使用datePicker修改日期的代码：
```
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->canlendar = Calendar.getInstance();
                    year = datePicker.getYear();
                    month = datePicker.getMonth();
                    day = datePicker.getDayOfMonth();
                    canlendar.set(Calendar.YEAR, year);
                    canlendar.set(Calendar.MONTH, month);
                    canlendar.set(Calendar.DAY_OF_MONTH, day);
                    long when = canlendar.getTimeInMillis();
                    if (when / 1000 < Integer.MAX_VALUE) {
                        SystemClock.setCurrentTimeMillis(when);
                        tv_date.setText(canlendar.get(Calendar.YEAR) + "/"
                                + ((canlendar.get(Calendar.MONTH) + 1)) + "/"
                                + canlendar.get(Calendar.DAY_OF_MONTH));
                    }
```
Tip：每次需要使用Calendar 必须获得一次实例，否则会它会使用上一次获取实例的日期格式，修改的时候可能会有点小问题
可是，照着上面的代码敲了之后，运行却出现如下的警告：
![](http://images.cnblogs.com/cnblogs_com/terryblog/permission_denied.jpg)
网上查了一下，说SystemClock.setCurrentTimeMillis  这个函数需要root 权限或者运行在系统进程中才可以用，通过参考了这一篇文章我得到了解决办法：[http://dev.10086.cn/blog/?uid-49302-action-viewspace-itemid-907](http://dev.10086.cn/blog/?uid-49302-action-viewspace-itemid-907) ，不过这篇文章的第二个办法说得有点不是很清楚，这里我重新为大家详细说一下
先进入 Android 源码目录 F:/android_source/build/target/product/security ，具体机器目录自己选择。找到 【platform.pk8】和【platform.x509.pem】这两个文件，这里为了方便将它们复制到根目录，如：F:/sign 文件夹内，本来下一步就是需要打包jar 包了，不过里我们在android系统的源码内找到 out 编绎目录下
signApk.jar 这个包，可以自行打jar 包或者下载我这个jar 包都是可以的，下载地址：[signapk](http://files.cnblogs.com/TerryBlog/signapk.rar) 请自行将后缀名改为.jar 格式，把此jar 包放于F:/sign  文件内，打开dos 操作界面，定们到F:/sign，如下：![](http://images.cnblogs.com/cnblogs_com/terryblog/docs.jpg)，接着输入如下命令：java
 -jar signapk.jar platform.x509.pem platform.pk8 test.apk testnew.apk ，在dos 命令名如下图：![](http://images.cnblogs.com/cnblogs_com/terryblog/tteew.jpg)，其中set.apk 是指我们要签名的原始文件apk，后面setnew.apk 是指我们签名后输出的apk文件的名称
Tip：在签名的时候必须将set.apk事先用rar 文件打开，进入META-INF目录下，将CERT.SF和CERT.RSA这两个文件删除掉，再执行如上操作。到了这一步，程序己经可以正常安装了。
如果提示 FaiLure [INSTALL_FAILED_ALREADY_EXISTS]，请使用如下命令，将应用程序移除：![](http://images.cnblogs.com/cnblogs_com/terryblog/uninstall.jpg)，uninstall 后面字符是指应用程序所在的包名，再运行adb install [package name] 就可以安装成功。
Tip：不要忘了在你的manifest文件下注册android:sharedUserId="android.uid.system" 。
如果你照着如上的步骤一步一步写的话，保证可以成功修改系统的时间或者日期等操作。
本文出自 “[Terry_龙](http://terryblog.blog.51cto.com/)” 博客，请务必保留此出处[http://terryblog.blog.51cto.com/1764499/389843](http://terryblog.blog.51cto.com/1764499/389843)
CSDN相关文章::
Android中如何修改系统时间（应用程序获得系统权限）[http://blog.csdn.net/liujian885/archive/2010/03/22/5404834.aspx](http://blog.csdn.net/liujian885/archive/2010/03/22/5404834.aspx)

