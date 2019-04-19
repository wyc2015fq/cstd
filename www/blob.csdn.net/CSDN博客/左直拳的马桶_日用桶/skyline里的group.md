# skyline里的group - 左直拳的马桶_日用桶 - CSDN博客
2017年10月30日 19:43:50[左直拳](https://me.csdn.net/leftfist)阅读数：336标签：[skyline																[group](https://so.csdn.net/so/search/s.do?q=group&t=blog)](https://so.csdn.net/so/search/s.do?q=skyline&t=blog)
个人分类：[gis](https://blog.csdn.net/leftfist/article/category/6685585)
Skyline里面，有分组（group）这么个概念。目前我对于这个group还不甚了了，先将一丁点儿理解写下来。写作的过程，就是整理思路的过程。
arcgis里面，有图层的概念，当然skyline也有，但好像arcgis没有group这个概念。或者有，但我并不知道，我对于arcgis目前同样是个门外汉。
这个group是个逻辑概念，并且可以跨图层。分组，你可以将各个图层，画出来的多边形，图标之类，都分到同一个组里面，达到批量控制的目的。
比如：
```
var _groupname = "test";
var _groupId = sgworld.ProjectTree.CreateGroup(_groupname, "");//CreateGroup(GroupName,ParentGroupID)
//父队列为空，则放于根目录下
```
之后无论画各种多边形，叠各种图标，都用放在这个group下，现在要全部去掉，怎么办？
```
//清除旧有对象
var item = sgworld.ProjectTree.FindItem("\\" + _groupname);
if (item != null && item.length > 0) {
    sgworld.ProjectTree.DeleteItem(item);
}
```
好嘛，一下子全干掉了。
