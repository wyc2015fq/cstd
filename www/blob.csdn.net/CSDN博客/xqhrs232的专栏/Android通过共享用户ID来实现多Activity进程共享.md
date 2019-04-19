# Android通过共享用户ID来实现多Activity进程共享 - xqhrs232的专栏 - CSDN博客
2011年01月07日 23:12:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：988标签：[android																[user																[xml																[system																[button																[application](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::
[http://www.lhzhang.org/post/2010/09/Androide9809ae8bf87e585b1e4baabe794a8e688b7IDe69da5e5ae9ee78eb0e5a49aActivitye8bf9be7a88be585b1e4baab.aspx](http://www.lhzhang.org/post/2010/09/Androide9809ae8bf87e585b1e4baabe794a8e688b7IDe69da5e5ae9ee78eb0e5a49aActivitye8bf9be7a88be585b1e4baab.aspx)
**有朋友问到如何在Android不同应用程序之间共享内存，遂引发了这个文章，转成都一位程序员，实验做的很好**
**1.同一Apk中的同一包中的Activity调用时进程状况验证**
[1]创建Project： 
   project name: FirstProject 
   package:com.demo 
   默认Activity:MainActivity 
[2]添加一个新的Activity： 
   name:SecondActivity 
[3]修改布局。在MainActivity布局中添加一个Button，当点击此Button时启动SecondActivity。在SecondActivity的布局中放置一个Textview，以证明SecondActivity已启动。
[4]运行程序，查看此Ap进程情况： 
   USER：app_36  PID：8360  NAME：com.demo 
   点击按钮，启动SecondActivity，再次查看进程情况： 
   USER：app_36  PID：8360  NAME：com.demo 
**进程列表没有变化，两个Activity运行在同一进程中。 2.同一Apk中的不同包的Activity调用时进程状况验证**
[1]将SecondActivity挪到包com.demo.second中去，相应修改AndroidManifest.xml中的name为：com.demo.second.SecondActivity
[2]运行程序，查看此时进程情况： 
   USER：app_36  PID：10593  NAME：com.demo 
[3]点击按钮启动SecondActivity，查看此时进程情况： 
   USER：app_36  PID：10593  NAME：com.demo 
**进程列表没有变化，两个Activity运行在同一进程中。即进程name只受AndroidManifest.xml中manifset结点的package属性影响。3.同一Apk中Activity process属性修改后进程状况验证**
[1]为SecondActivity添加process属性，其值为":abc"，也可以随便是其他的":"开头的字符串，常见的是":remote"：
Xml代码
- <activity android:name="com.demo.second.SecondActivity" android:process=":abc"></activity>
[2]运行程序，查看进程情况： 
   USER：app_36  PID：12137  NAME：com.demo 
[3]点击按钮，启动SecondActivity，查看进程情况： 
   USER：app_36  PID：12137  NAME：com.demo 
   USER：app_36  PID：12303  NAME：com.demo:abc 
**进程表多了一项。两个Activity各自有一个进程，SecondActivity的进程名称为 包名+后缀 4.不同Apk中不同包名的Activity进程状况验证**
[1]运行FirstProject： 
   USER：app_36  PID：12137  NAME：com.demo 
[2]创建SecondProject： 
   project name: SecondProject 
   package:com.demo2 
   默认Activity:MainActivity 
[3]运行SecondProject： 
   USER：app_37  PID：14191  NAME：com.demo2 
**进程表多了一项。两个Activity各自有一个进程，同时其进程用户名、包名也不同，互不影响。 5.不同Apk，签名相同、包名相同的Activity进程状况验证**
[1]修改SecondProject的包也为com.demo，相应要修改AndroidManifest.xml内容。 
[2]运行SecondProject，查看进程情况： 
   USER：app_36  PID：14944  NAME：com.demo 
**进程表只有一项，但是实际上FirstProject此时已经被覆盖了，系统中只存在SecondProject了，因为模拟器调试时apk使用的签名key都是一样的，系统看到key一样，包名一样认为这个包就是FirstProject所以覆盖掉了。可以通过DDMS复制/data/system/packages.xml查看一下内容。**
Xml代码
- <package name="com.demo" codePath="/data/app/com.demo.apk" system="false" ts="1279955425000" version="1" userId="10036">
   这个文件里面，package name都是唯一的，同时可以看到用户名是通过userId来决定的。 
**6.不同Apk，签名不相同，包名相同的Activity进程状况验证**
[1]在Eclipse的Package Explorer导航树中选中FirstProject，点右键。 
[2]Android tools-->Export Signed Application Package，按照向导创建一个用指定key签名的apk包。 
[3]同样导出Second Project。 
[4]切换窗口到模拟器，按Home键-->按Menu键-->设置-->应用程序-->管理应用程序-->SecondProject-->卸载。这是为了用命令行安装做准备。
[5]启动一个命令行窗口，执行adb install firstproject.apk,会提示成功安装。 
[6]执行adb install secondproject.apk，提示安装失败。 
**默认的Apk其安装时会分配新的UserId，即此时FirstProject以及SecondProject的UserId可以认为是不同的。    包名不同，则签名key是否相同无所谓，两个apk都可以安装。【第4个实验】    包名相同时，签名key相同则会覆盖【第5个实验】；签名不同则第二个apk安装会失败。【第6个实验】 7.不同Apk，Share User Id相同，包名不同时进程情况分析**
[1]修改firstproject、secondproject的AndroidManifest.xml的manifset结点，增加属性
Xml代码
- android:sharedUserId="com.demouser" 
[2]修改secondproject的包为com.demo2，不然其会覆盖firsetproject。 
[3]运行firsetproject、secondproject，查看进程列表： 
   USER：app_35  PID：19993  NAME：com.demo2 
   USER：app_35  PID：20045  NAME：com.demo2 
   仍然存在两个进程。进程的用户名一样，说明shareUserId确实有效了，但是进程名字一个不一样。 
   再次导出/data/system/packages.xml，查看其内容：
Xml代码
- <package name="com.demo" codePath="/data/app/com.demo.apk" system="false" ts="1279957484000" version="1" sharedUserId="10035">
Xml代码
- <package name="com.demo2" codePath="/data/app/com.demo2.apk" system="false" ts="1279957473000" version="1" sharedUserId="10035">
   可以看到两个项目的UserId都是10035，确实是一样的。 
**8.不同Apk，Share User Id相同，包名不同、指定Activity的process属性进程情况分析**
[1]修改SecondProject的MainActivity的process属性，指定绑定到进程名为com.demo的进程上：
Xml代码
- <activity android:name=".MainActivity" 
- android:label="@string/app_name" android:process="com.demo"> 
[2]运行firstProject、SecondProject，查看进程情况： 
    USER：app_35  PID：21387  NAME：com.demo 
    两个Activity运行于同一个进程。 
**9.不同Apk，Share User Id相同，包名不同、签名key不同**
经实验，安装第二个apk时会提示INSTALL_FAILED_UPDATE_INCOMPATIBLE错误，安装失败。 
**总结： UserId不同时：       包名不同：未设定process属性时，各自的Activity在各自的进程。                 即使process指定了包名，也不会和另一个用户的同名包共享进程。       包名相同：                 签名相同：覆盖旧的同包名apk。                 签名不同：新的apk会安装失败。【签名key一般都是不同的】 UserId相同时：       包名不同：未设定process属性时，各自的Activity在各自的进程。                 process属性指定，则可以共享进程。       包名相同：                 签名相同：覆盖旧的同包名apk。                 签名不同：新的apk会安装失败。【签名key一般都是不同的】**
