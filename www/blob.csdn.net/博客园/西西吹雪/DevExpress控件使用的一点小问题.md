
# DevExpress控件使用的一点小问题 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[DevExpress控件使用的一点小问题](https://www.cnblogs.com/watsonyin/archive/2005/03/22/123611.html)
第一篇随笔，就写刚解决的一个问题吧

最近采用了一组界面控件，全名叫Developer.Express.NET.Windows.Forms.Components.Suite.v1.2.0。很酷，使用也很方便。但今天操作时，不小小把主界面菜单的一个菜单项拖了一下，不见了，找来找去，就是找不回来。后来转念一想，反正控件生成也是在代码里完成的，直接到代码里找算了。在InitializeComponents中找了一下，发现果然是this.MenuBar.LinksPersistInfo.AddRange语句中少了那一项，手工加上即可。





posted on2005-03-22 16:56[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=123611)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
