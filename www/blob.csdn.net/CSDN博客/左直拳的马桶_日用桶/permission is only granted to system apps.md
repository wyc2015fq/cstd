# permission is only granted to system apps - 左直拳的马桶_日用桶 - CSDN博客
2012年12月26日 16:19:56[左直拳](https://me.csdn.net/leftfist)阅读数：38534
我打算为我们网站的安卓客户端加上在线升级功能，不然的话，这个客户端的意义将大打折扣。
根据大拿们的经典，我在AndroidManifest.xml中加上以下三句：
```
<uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.INSTALL_PACKAGES"/>
	<uses-permission android:name="android.permission.MOUNT_UNMOUNT_FILESYSTEMS"/>
```
结果eclipse马上来个红叉叉，说是 permission is only granted to system apps 。
what is it?
疯狂搜索，一堆英文，一堆提问，一堆不知所谓，好混乱。几次看到有人提示说，将project clean一下，就可以鸟。
果然。方法是点eclipse的菜单project->clean...，然后叉叉就不见了。
啊！多么神奇的clean啊，I like it! 原来，这里的clean，就相当于VS.NET 里的rebuild。
不过，只要稍微修改一下AndroidManifest.xml，立刻会惊动警惕的编译器，它马上又打出一个红叉叉。
怎么办？只好再次 clean 咯。
