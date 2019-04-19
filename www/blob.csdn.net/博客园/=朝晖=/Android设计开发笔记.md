# Android设计开发笔记 - =朝晖= - 博客园
# [Android设计开发笔记](https://www.cnblogs.com/dhcn/p/7115371.html)
     1、因为[Android](http://lib.csdn.net/base/android)的开发是基于框架的开发：往对方指定的位置加代码；其运行的Message\Handler机制也决定了其单步跟踪也不方便，所以建立新代码时要多Log，这样不但便于调试，而且帮助你加深框架执行方式的认识。      
     2、充分理解每个组件，除了理解其运作机制外，特别是理解其预设用途，不要用错方向，不要滥用，也不要躲避某种方式。用其预设的用途去用它，就符合框架本身的设计意图，否则的话，又累又玩不转。再就是尽量使用框架组件，尽量少自己创建组件级代码。
     3、Activity和Fragment的Layout这类资源避免复用,尽量一个界面一个layout，复用容易出问题。
     4、有时候R只能出系统自身的内建资源，有时候能出自定义资源，后来发现原因，两种R不一样,前者是[android](http://lib.csdn.net/base/android).R，后者是com.domainname.packagename.R.一般情况下，系统import不要导入android.R，否则就会导致项目自定义资源找不见的情况。
     5、TabViewPager的预加载，看后台日志，会发现onTabSelected的position和CreateView的Fragment不一致，后来发现它是预先一个方面的位置CreateView，这种情况也说明了SQLite存储网络数据的必要性，用一般的ArrayAdpater，做预加载明显不方便，有了SQLite，CusorAdapter可以将显示的数据和下载的数据完全隔离，这样就便于下页数据的预加载。
     6、Android TextView 支持的HTML标签
```
<a href="...">
<b>
<big>
<blockquote>
<br>
<cite>
<dfn>
<div align="...">
<em>
<font size="..." color="..." face="...">
<h1>
<h2>
<h3>
<h4>
<h5>
<h6>
<i>
<img src="...">
<p>
<small>
<strike>
<strong>
<sub>
<sup>
<tt>
<u>
```

