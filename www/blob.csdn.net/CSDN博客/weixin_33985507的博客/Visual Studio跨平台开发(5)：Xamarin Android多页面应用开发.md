# Visual Studio跨平台开发(5)：Xamarin Android多页面应用开发 - weixin_33985507的博客 - CSDN博客
2015年06月27日 20:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
**前言**
大部份的Android 都具有实体或虚拟的Back键. 因此在处理多页面应用程序时, 与先前所介绍的iOS Navigation controller 比较起来会简单许多.
1. 开启Visual Studio 并新增Android Application 专案并命名为Lab4-MultiScreen
![](https://img-blog.csdn.net/20140303113653546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGFtYXJpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2. 在Layout文件夹中新增Second.axml
![](https://img-blog.csdn.net/20140303113726406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGFtYXJpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在Second.axml 中拖放1个TextView并标示此为第2个Activity
![](https://img-blog.csdn.net/20140303113748640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGFtYXJpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2. 在专案底下新增一个SecondActivity.cs. 在OnCreate事件中选写以下代码：
[view source](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#viewSource)
[print](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#printSource)[?](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#about)
|1|SetContentView(Resource.Layout.Second);|
3. 开启Activity1.cs, 在class name的地方按滑鼠右键=>重构=>重新命名. 将类别名称改为FirstActivity. 记得在方案总览中的档名也一并改为FirstActivity.cs
![](https://img-blog.csdn.net/20140303113819890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGFtYXJpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4. 开启Main.axml, 在画面中放置1个Button并指定Text属性值为”Load Second Activity”并将id 的属性值变更为[”@+id/ShowSecond](mailto:%E2%80%9D@+id/ShowSecond)”
5. 开启FirstActivity.cs, 在OnCreate事件中输入以下代码:
[view source](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#viewSource)
[print](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#printSource)[?](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#about)
|01|//载入页面|
|02||
|03|SetContentView(Resource.Layout.Main);|
|04||
|05|//声明并取得按钮对象, 并在按钮的click事件处理中载入SecondActivity|
|06||
|07|Button button = FindViewById< Button >(Resource.Id.showSecond);|
|08||
|09|button.Click += delegate|
|10||
|11|{…….按钮处理函数}|
Button的click处理函数中, 我们将使用3种方法来载入SecondActivity.
- 方法一: 使用内建的StartActivity方法, 代码如下：
[view source](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#viewSource)
[print](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#printSource)[?](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#about)
|1|//呼叫其他Activity的第一种方法(隐含的建立Intent)|
|2||
|3|StartActivity(typeof(SecondActivity));|
- 方法二: 建立Intent, 然后使用StartActivity载入其他SecondActivity. 代码如下：
[view source](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#viewSource)
[print](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#printSource)[?](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#about)
|1|//呼叫其他Activity的第二种法, 建立Intent, 然后使用StartActivity载入其他Activity|
|2||
|3|var second = newIntent(this,typeof(SecondActivity));|
|4||
|5|StartActivity(second);|
- 方法三: 建立Intent, 并通过Intent.PutExtra载入Activity并传入参数. 代码如下：
[view source](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#viewSource)
[print](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#printSource)[?](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#about)
|1|//使用Intent.PutExtra载入Activity并传入参数|
|2||
|3|var second = newIntent(this,typeof(SecondActivity));|
|4||
|5|second.PutExtra("FirstData","Data from FirstActivity");|
|6||
|7|StartActivity(second);|
上述的3种方式, 第1个跟第2个是一样的, 使用第1种方式, 会隐含简历一个Intent对象。
6. 执行专案并检视结果.
7. 通过上述的第3个方法, 可以像QueryString般传递参数到下一个Activity. 现在我们开启SecondActivity.cs. 通过Intent的GetStringExtra方法来取得参数的值. 在Oncreate方法中输入以下代码:
[view source](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#viewSource)
[print](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#printSource)[?](http://www.dotblogs.com.tw/taihon64/archive/2013/08/27/115616.aspx#about)
|01|//载入页面|
|02||
|03|SetContentView(Resource.Layout.Second);|
|04||
|05|//声明并取得TextView对象|
|06||
|07|var label = FindViewById(Resource.Id.screen2Label);|
|08||
|09|//通过Intent.GetStringExtra取得从前一个Activity所传来的讯息|
|10||
|11|label.Text = Intent.GetStringExtra("FirstData") ??"Data not available";|
在上述代码中, 我们通过Intent的GetStringExtra(“参数名称”)来取得字串型別的参数. 事实上还可以通过类似的方法取得不同型別的参数值. 如下图所示：
![](https://img-blog.csdn.net/20140303113912125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGFtYXJpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而??陈述式则是用来判断是否为Null的方便写法. 若取出的值为Null则显示后面的字串.
8. 执行专案并检视结果, 如下图所示
![](https://img-blog.csdn.net/20140303113931250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGFtYXJpbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### **结语**
在本篇文章中, 我们介绍Android 应用程序在多页面中的切换, 相较于iOS, Android 对于多页面的处理较为方便. 另外在Android中也提供Tab控制项在多页面之间进行切换. 
