# skyline里显示图层，点击弹出气泡 - 左直拳的马桶_日用桶 - CSDN博客
2017年11月07日 20:20:26[左直拳](https://me.csdn.net/leftfist)阅读数：1495
skyline里加载图层，点击图层对象，弹出相关气泡，其实挺简单的。
步骤：
> 
显示图层 
  绑定弹出框
**1、显示图层**
skyline与arcgis不同，arcgis是要用脚本从地图服务里加载，才能拥有并显示图层；而skyline里面，图层需要一早做好放在三维文件（*.fly）里，然后我们可以通过文件内部的所谓路径（类似XML的xpath）来获得某图层，进行操作。 
![这里写图片描述](https://img-blog.csdn.net/20171107200314902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
另外，我们飞行文件时，默认里面所有的图层都会显示出来（这个可能可以在fly制作过程中设定），所以我们加载之初，通常要将一些图层隐藏：
隐藏图层
```java
function hideLayer(path) {//path就是某个图层的路径，比如"业务数据\\"
    var item = sgworld.ProjectTree.FindItem(path);
    if (item) {
        sgworld.ProjectTree.SetVisibility(item, false);
    }
}
```
相反，显示图层
```java
function showLayer(path) {
    var item = sgworld.ProjectTree.FindItem(path);
    if (item) {
        sgworld.ProjectTree.SetVisibility(item, true);
    }
}
```
**2、绑定气泡**
创建一个message，然后将message的ID赋给相关图层，进行绑定即可。
```java
function showLayer(path,node) {
    var item = sgworld.ProjectTree.FindItem(path);
    if (item) {
        sgworld.ProjectTree.SetVisibility(item, true);
        var layer = sgworld.ProjectTree.GetLayer(item);
        layer.FeatureGroups(0).SetProperty("Message", bindMessage(node));
    }
}
function bindMessage(node) {
    var content = node.template;
    var msgp = [400, 200, content];//分别为width,height,内容
    return getMessageBox(msgp);
}
function getMessageBox(msgp) {
    var caption = ' ';
    var src = "";
    var left = 0;
    var top = 0;
    var timeout = -1;
    var mess = sgworld.Creator.CreatePopupMessage(caption, caption, left, top, msgp[0], msgp[1], timeout);
    mess.InnerHtml = "<div style=\"position:absolute;left:0px;top:0px; z-index: 100;margin: 0px;width:100%;height:100%;\">" + msgp[2] + "</div>";
    mess.ShowCaption = true;
    return mess.ID;
}
```
其中，气泡的内容如何确定呢？
当然是动态的。动态内容为“[字段名]”（在acgis里，是”${字段名}”）。主要赋给message的InnerHtml里，包含图层相应的“[字段名]”，即可动态反映出图层的相关信息矣。系统会自动匹配，我们不必刻意去赋值，好神奇哟。
例如，假设有如下模板：
```xml
mess.InnerHtml ="<b>[功能区名称]</b><hr>类型:<b>[功能区类型]</b><br />面积:<b>[面积]</b>（公顷）<br />岸段:<b>[岸段长度] 米</b><br />所在市县：<b>[所在市] - [所在县]</b><br />";
```
![这里写图片描述](https://img-blog.csdn.net/20171107201915775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
