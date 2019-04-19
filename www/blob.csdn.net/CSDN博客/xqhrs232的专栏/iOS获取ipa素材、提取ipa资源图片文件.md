# iOS获取ipa素材、提取ipa资源图片文件 - xqhrs232的专栏 - CSDN博客
2019年02月28日 15:22:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：56
原文地址::[https://www.cnblogs.com/yajunLi/p/7134033.html](https://www.cnblogs.com/yajunLi/p/7134033.html)
相关文章
1、ipa文件解压方法----[https://blog.csdn.net/quanshuiwuxiang/article/details/8618349?utm_source=blogxgwz6](https://blog.csdn.net/quanshuiwuxiang/article/details/8618349?utm_source=blogxgwz6)
当我们看到一款优秀的App时，我们可能对它的一些素材比较感兴趣，或者我们也想仿写一款类似app，那么怎么能获取到它的素材资源文件呢？
下面我以ofo举例：
1、打开iTunes，搜索ofo关键字，选择商店下
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707200617940-1981578264.png)
找到后，下载下来，然后切换回到资料库，会看到已下载的列表
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707200804972-1354597563.png)
2、右击Finder显示，查看本地
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707200902534-1127019685.png)
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707200934784-701229164.png)
3、修改后缀为zip，然后双击解压，会变成下面这样
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707201112019-1403818551.png)
然后，找到最里面的主体文件，显示包内容
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707201305519-1674588050.png)
 4、然后我们会看到一些资源文件，其中包括一些appicon图标、启动图、Assets等。
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707201401097-407183107.png)![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707201508206-51472549.png)![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707201443597-454911774.png)
但这里要注意一点：
这里的Assets是不能直接获取里面的内容的，但实际情况大多数图标又都存放在这里，所以，我们需要借助一款工具来实现抓取素材。
# [iOS-Images-Extractor](https://github.com/devcxm/iOS-Images-Extractor)
这是一款Mac工具项目，我们要直接git下来项目运行。
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707202021925-1704788513.png)
我们按照Github的说明：
先cd到你想存放项目的路径，或直接根目录 cd ～
cd ~
终端运行如下命令：
git clone https://github.com/devcxm/iOS-Images-Extractor
cd iOS-Images-Extractor
git submodule update --init --recursive
open iOSImagesExtractor.xcworkspace
等待项目打开后，运行项目：
出现界面后，把Assets.car拖拽到程序里，点Start
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707202448706-297046285.png)
 等待几秒后，素材抓取完成，点击Output Dir，查看解压出来的资源图片
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707202610190-2063677399.png)
最后就能得到想要的图片啦
![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707203102847-809732523.png)![](https://images2015.cnblogs.com/blog/511196/201707/511196-20170707203211331-1795893261.png)
enjoy～ 
