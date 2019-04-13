
# ArcGIS Engine开发－取得ArcMap文档缩略图 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcGIS Engine开发－取得ArcMap文档缩略图](https://www.cnblogs.com/watsonyin/archive/2006/06/09/421891.html)
ArcObjects提供了一个IMapDocument接口，可以实现对ArcMap文档的读写操作。该接品其中有一个属性是Thumbnail。当我们保存文档，使用了保存缩略选项时，会在文档中保存一个缩略图。我们可以通过Thumbnail来读取这个缩略图。
Thumbnail的类型是stdole.IPicture，所以要使用它，首先要引用stdole。
private void System.Drawings.Image GetThumbnail(IMapDocument pMapDoc)
{
return System.Drawing.Image.FromHbitmap((IntPtr)(pMapDoc.Thumbnail.Handle));
}
一行代码搞定，哈哈。本来就很简单嘛。
好久没写了。先写这么一点吧。




posted on2006-06-09 17:56[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=421891)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
