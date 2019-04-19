# QGC的PlanMasterController - LC900730的博客 - CSDN博客
2017年09月20日 10:03:02[lc900730](https://me.csdn.net/LC900730)阅读数：264标签：[QGC](https://so.csdn.net/so/search/s.do?q=QGC&t=blog)
个人分类：[QGC](https://blog.csdn.net/LC900730/article/category/7111402)
在程序主要界面的 
MainWindowInner.qml中，默认的visible是打开FlightDisplayView。 
![这里写图片描述](https://img-blog.csdn.net/20170920100246977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在FlightDisplayView.qml中，定义了很多属性。
```
QGCView{
    id:root
    property alias  guidedController   
            :guidedActionsController
    property var _planMasterController：
        masterController
}
```
