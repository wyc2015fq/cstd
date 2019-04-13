
# ArcGIS Engine开发－自定义图层类型 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcGIS Engine开发－自定义图层类型](https://www.cnblogs.com/watsonyin/archive/2006/09/14/504020.html)
用ArcGIS Engine开发的一个特别就是面向接口编程，每组接口代表了对象在某个方面的特性，表现为一个方法、属性或事件。要定义自己的图层类型，实际上只需要实现ILayer接口就可以了。该接口的主要方法或属生包括：
publicIEnvelopeAreaOfInterest {get;}
public[void](javascript:LinkKwd('frlrfSystemVoidClassTopic'))Draw (
[esriDrawPhase](javascript:LinkKwd('eaglrfesriSystemesriDrawPhase'))DrawPhase,
[IDisplay](javascript:LinkKwd('eaglrfDisplayIDisplay'))Display,
[ITrackCancel](javascript:LinkKwd('eaglrfesriSystemITrackCancel'))trackCancel
)
publicvoidSpatialReference {set;}
publicstringget_TipText (
doublex,
doubley,
doubleTolerance
);
等等，请直接查阅帮助。
但实现了该接口，图层只是能加入以地图中，并显示出来而已。如果你要自定义的图层类型是矢量要素的话，最好直接实现IFeatureLayer。由于IFeatureLayer从ILayer继承而来，所以也包含了上述方法或属性。同时还可以支持选择，查询等操作了。
实现了IFeatureLayer，还不能支持图层渲染功能。要想支持图层渲染，得实现IGeoFeatureLayer接口，该接口从IFeatureLayer继承而来，所以也包含IFeatureLayer所支持的所有方法属性。
如果要支持属性表功能的话，得实现ITable接口。
如果希望图层信息能保存到MXD文档中，得实现IPersistStream接口。
由此可见，在ArcGIS Engine中，要实现自定义图层类型，并不是一件不可能的事。虽然会比较烦琐，但思路却非常清晰。本人就通过这种思路设计了一种组合图层，可以同时包含点线面多种图形，但在TOCControl中只表现为一个图层。当然，这个工作要简单轻松一些，但还是富有乐趣的～





posted on2006-09-14 11:23[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=504020)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
