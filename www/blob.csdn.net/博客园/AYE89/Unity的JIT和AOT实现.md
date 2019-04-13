
# Unity的JIT和AOT实现 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Unity的JIT和AOT实现](https://www.cnblogs.com/eniac1946/p/7417191.html)
|Posted on|2017-08-23 11:09|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7417191)|[收藏](#)
|[https://myhloli.com/about-il2cpp.html](https://myhloli.com/about-il2cpp.html)
|JIT方式：
|Unity的跨平台技术是通过一个Mono虚拟机实现的。而这个虚拟机更新太慢，不能很好地适应众多的平台。
![](file:///C:/Users/myhloli/AppData/Local/Temp/enhtmlclip/Image.png)![b54684b854d295f6a80aaa332ae17529[1]_看图王](https://blogcdn.myhloli.com/wp-content/uploads/2015/08/b54684b854d295f6a80aaa332ae175291_KanTuWang.jpg)
|Android下会生成：
|\assets\bin\Data\Managed\           Assembly-CSharp.dll    （游戏的控制逻辑，运行时调用）
|\lib\armeabi-v7a\|libmono.so|, libmain.so, libunity.so    （libmono.so会完成mono VM的功能）
|AOT方式：
|unity公司就自行研发了IL2cpp，把本来应该再mono的虚拟机上跑的中间代码转换成cpp代码，这样再把生成的cpp代码，利用c++的跨平台特性，
|在各个平台上通过对各平台都有良好优化的native c++编译器编译，以获得更高的效率和更好的兼容性。
![](file:///C:/Users/myhloli/AppData/Local/Temp/enhtmlclip/Image(1).png)![75c24f0f60fcdaed42563e926e4ce7b8_看图王](https://blogcdn.myhloli.com/wp-content/uploads/2015/08/75c24f0f60fcdaed42563e926e4ce7b8_KanTuWang1.jpg)
|Android下会生成：
|\assets\bin\Data\Managed\           Assembly-CSharp.dll    一般不打包进apk（游戏的控制逻辑，只作为中间文件存放，运行时不会调用）
|\lib\armeabi-v7a\|libil2cpp.so|, libmain.so, libunity.so    （libil2cpp.so中包含了游戏控制的native code，和|IL2CPP VM功能|）
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170823163339105-947787528.png)
|上图，对libil2cpp.so反编译可见，其中包含il2cpp::vm功能。图中为注册一个控制脚本中的方法。
|Managed下托管代码（比如UnityEngine.dll）也会被编译进libil2cpp.so中。







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
