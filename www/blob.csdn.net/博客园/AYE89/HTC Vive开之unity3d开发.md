
# HTC Vive开之unity3d开发 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[HTC Vive开之unity3d开发](https://www.cnblogs.com/eniac1946/p/7248415.html)
|Posted on|2017-07-28 09:19|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7248415)|[收藏](#)
|[常用的几款插件](http://www.52vr.com/article-652-1.html)
|Steam VR,  SteamVR Unity Toolkit
|配置要求：|显卡不低于GTX960性能的主机
|一、引入手柄交互
|1.通过Asset Store导入SteamVR Plugin
![](https://images2017.cnblogs.com/blog/1181483/201707/1181483-20170728092000180-362138989.png)
|2.拖入相关prefab
|先删除所有默认GameObject
![](http://img.manew.com/data/attachment/forum/201602/16/104150djnelzxlsftsnxt7.png.thumb.jpg)

|然后将SteamVR/Prefabs中的所有prefab拖入Hierarchy窗口
![](http://img.manew.com/data/attachment/forum/201602/16/104208palz9d81m8b2lxd5.jpg.thumb.jpg)
|3. 手柄按键
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811100927558-537685371.png)
|二、游戏控制
|手柄有一套自己的脚本，以SteamVR_XXX形式存在；|[教程入口](http://www.manew.com/thread-89036-1-1.html)
|手柄动作的触发通过“事件”机制与其他对象交互，其他对象若要用到手柄控制需编写相应的Handler
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811101510011-674460085.png)
|上图，SteamVR_XXX最后会被编译进Assembly-CSharp.dll中







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
