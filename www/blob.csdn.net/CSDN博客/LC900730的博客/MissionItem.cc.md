# MissionItem.cc - LC900730的博客 - CSDN博客
2017年09月18日 23:40:56[lc900730](https://me.csdn.net/LC900730)阅读数：478标签：[QGC](https://so.csdn.net/so/search/s.do?q=QGC&t=blog)
个人分类：[QGC](https://blog.csdn.net/LC900730/article/category/7111402)
```
MissionItem::MissionItem(QObject *parent):
QObject(parent),
_sequenceNumber(0)
,doJumpId(-1)
...
...
```
![这里写图片描述](https://img-blog.csdn.net/20170918235047030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170918235126259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击MAV_CMD_NAV_WAYPOINT可以发现读取的是 
/json/MavCmdInfoCommon.json文件的数据，rawName。
![这里写图片描述](https://img-blog.csdn.net/20170919001050277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170919001103461?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上述图片对应MissionItemEditor.qml文件
