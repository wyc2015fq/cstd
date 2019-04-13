
# unity3d工程下的data file作用 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity3d工程下的data file作用](https://www.cnblogs.com/eniac1946/p/7338776.html)
|Posted on|2017-08-10 13:22|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7338776)|[收藏](#)
|projectData文件夹中的data file：
|1. Player settings –|globalgamemanagers|and|globalgamemanagers.assets|files.
|2. Scenes and Assets –|level*|and|sharedassets*.asset|files.
|3. Resources –|resources.assets|files.
|4. GI data.
|5. Built-in resources –|unity_builtin_extra|file
|注意：
|所有开发脚本 ==>|Assembly-CSharp.dll
|Managed文件夹下其他dll ==> 和|unity的版本|相关，不会随工程改变
|asset文件夹 ==>|sharedassets*.asset
|hierarchy（下的某层） ==>|level*
|unity在build完后|gameobject存放是不带name的：
|unity是按ID顺序封包的，
|大概就是文件顺序对应一个id0，文件类型对应一个id1，索引表地址是相对的，要根据头部的版本号后地址计算相对偏移，4.X的索引表在头部，2.x的索引表在尾部，地址跳转后得到文件名，不过大部分文件都是没有文件名的.....修改文件后要fix一些东西，比如数据对齐和子文件的size之类的。
|工具：
|用UnityAssetsExplorer可以打开.asset文件
|UnityStudio可以打开folder和工程文件
|[3种反编译工具及教程](https://my.oschina.net/jjyuangu/blog/368184)
|[decompile最全教程](http://www.cnblogs.com/zhaoqingqing/p/3751351.html)
|[unity studio工具地址](https://github.com/Perfare/UnityStudio/releases)|GitHub







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
