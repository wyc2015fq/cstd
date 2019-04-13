
# ArcSDE中直接取得图层几何类型  - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcSDE中直接取得图层几何类型 ](https://www.cnblogs.com/watsonyin/archive/2006/04/28/387377.html)
要取得SDE图层要素类型，一个方法就是打开该图层，然后通过取得DataSetType属性和FeatureType属性来判断，但这样做效率比较低。能不能通过ＳＤＥ的系统表来获得呢，答案是可以的。
要有到两张系统表：GDB_OBJECTCLASSES和GDB_FEATURECLASSES。
其中GDB_OBJECTCLASSES表中相关字段有：ID、Name，GDB_FEATURECLASSES表的相关字段是：ObjectClassID、FeatureType、GeometryType，两个表的关联关系是：GDB_OBJECTCLASSES.ID=GDB_FEATURECLASSES.ObjectClassID。通过这两个表的关联，就可以取得指定图层要素类型了。
我们可以先通过SQL语句根据图层名称来获取GDB_FEATURECLASSES表中字段FeatureType、GeometryType的值，然后将其转化为对应的要素类型。转换关系请参照下面的代码：
![](/Images/OutliningIndicators/ExpandedBlockStart.gif)![](/Images/OutliningIndicators/ContractedBlock.gif)publicenumGeoType![](https://www.cnblogs.com/Images/dot.gif){Point,Line,Polygon,Anno,Raster,AttrTable,Unknown};
![](/Images/OutliningIndicators/None.gif)
![](/Images/OutliningIndicators/None.gif)publicstaticGeoType GetGeoType(objectGeometryType,objectFeatureType)
![](/Images/OutliningIndicators/ExpandedBlockStart.gif)![](/Images/OutliningIndicators/ContractedBlock.gif)[        ](/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[            ](/Images/OutliningIndicators/InBlock.gif)if(GeometryType==DBNull.Value)
![](/Images/OutliningIndicators/InBlock.gif)[                ](/Images/OutliningIndicators/InBlock.gif)returnGeoType.AttrTable;
![](/Images/OutliningIndicators/InBlock.gif)[            ](/Images/OutliningIndicators/InBlock.gif)elseif(GeometryType.ToString()=="2")
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[                ](/Images/OutliningIndicators/InBlock.gif)returnGeoType.Point;
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/InBlock.gif)[            ](/Images/OutliningIndicators/InBlock.gif)elseif(GeometryType.ToString()=="3")
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[                ](/Images/OutliningIndicators/InBlock.gif)returnGeoType.Line;
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/InBlock.gif)[            ](/Images/OutliningIndicators/InBlock.gif)elseif(GeometryType.ToString()=="4")
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[                ](/Images/OutliningIndicators/InBlock.gif)if(FeatureType.ToString()=="1")
![](/Images/OutliningIndicators/InBlock.gif)[                    ](/Images/OutliningIndicators/InBlock.gif)returnGeoType.Polygon;
![](/Images/OutliningIndicators/InBlock.gif)[                ](/Images/OutliningIndicators/InBlock.gif)elseif(FeatureType.ToString()=="11")
![](/Images/OutliningIndicators/InBlock.gif)[                    ](/Images/OutliningIndicators/InBlock.gif)returnGeoType.Anno;
![](/Images/OutliningIndicators/InBlock.gif)[                ](/Images/OutliningIndicators/InBlock.gif)elseif(FeatureType.ToString()=="14")
![](/Images/OutliningIndicators/InBlock.gif)[                    ](/Images/OutliningIndicators/InBlock.gif)returnGeoType.Raster;
![](/Images/OutliningIndicators/InBlock.gif)[                ](/Images/OutliningIndicators/InBlock.gif)else
![](/Images/OutliningIndicators/InBlock.gif)[                    ](/Images/OutliningIndicators/InBlock.gif)returnGeoType.Unknown;
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/InBlock.gif)[            ](/Images/OutliningIndicators/InBlock.gif)else
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[            ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[                ](/Images/OutliningIndicators/InBlock.gif)returnGeoType.Unknown;
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[            }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/ExpandedBlockEnd.gif)[        }](/Images/OutliningIndicators/ExpandedBlockEnd.gif)
通过这种方式，速度很快，而且可以一次读出所有图层的图层基本信息和要素类型，速度极快。如果这些信息要经常使用的话，可以保存到一个哈希表中，这样只用连接一次数据库就ＯＫ了。





posted on2006-04-28 11:59[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=387377)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
