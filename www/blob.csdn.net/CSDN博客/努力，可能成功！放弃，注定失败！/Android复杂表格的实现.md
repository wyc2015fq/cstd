# Android复杂表格的实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年08月22日 13:34:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4035

[本文来自简书，原文地址:http://www.jianshu.com/p/6d133f572ed2](http://www.jianshu.com/p/6d133f572ed2)
最近在做一个房贷计算器的项目，项目中需要实现一个较为复杂的表格。如下图所示。
![](https://img-blog.csdn.net/20170822133134137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
12.png
需求是当滑动中间区域的内容时，表头可以跟着滑动。即左右滑动时，第一列固定，第一行和第二行的内容可以跟着内容区域一起滑动；上下滑动时，第一行第二行固定，第一列可以跟着内容区域一起滑动。
刚开始有点懵，内容区域怎么可以既能左右滑动又能上下滑动呢，listview只能实现上下滑动，无法实现；recycleView虽然可以实现左右滑动，但是也只能一次实现一个方向。
一个复杂的问题都是由一个个小问题所组成，可以把这个问题分解成以下几个小问题。
- 实现两个view的联动
- 实现view样式的自定义
- 实现表格数据的绑定
#### 实现两个view的联动
先解决第一个问题，在Android里实现两个view的联动，我能想到的有两个方法。一个是设置滑动监听，当一个view滚动时，另一个view也滚动相同的距离。另一个是利用事件的分发机制，在两个view的外面再套一个父view，让父view拦截两个子view某一个方向的滑动事件即可。根据这个思路可以想到如下的实现方案。
![](https://img-blog.csdn.net/20170822133243820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Paste_Image.png
垂直方向是两个recycleView,外面套了一个垂直方向的ScrollView,这样便可以实现上下方向的联动。水平方向上是两个水平方向的HorizontalScrollView,互相设置滚动监听，这样便可以实现左右方向的联动。
联动实现了，不过这里有一个问题，水平方向的拓展性不是很好，无法跟垂直方向的recycleView一样动态的拓展。
#### 开源库ScrollablePanel
我们再来看一个比较优秀的开源库是怎么实现的。
![](https://img-blog.csdn.net/20170822133249496?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Paste_Image.png
它的布局很简单，垂直方向上使用了一个recycleView,故可以实现上下的联动。这个垂直recycleView的每一个item是由一个FrameLayout和一个水平方向的recycleView组成。每一个水平方向的recycleView都与顶部的recycleView互相设置滚动监听，由此实现左右方向的联动。这样的布局便解决了上文中提到的水平方向拓展性差的问题。
ScrollablePanel在github上有一千多颗star,除了它较好的布局外，它的封装也很值得学习。它只向开发者暴露了一个抽象的PanelAdapter 类。用户只要实现这个类确定每个View的布局和数据便可实现想要的功能。
```
public abstract class PanelAdapter {
    //获得table的行数
    public abstract int getRowCount();
    //获得table的列数
    public abstract int getColumnCount();
    //获得view类型
    public int getItemViewType(int row, int column) {
        return 0;
    }
    //绑定数据
    public abstract void onBindViewHolder(RecyclerView.ViewHolder holder, int row, int column);
    //创建view
    public abstract RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType);
}
```
这个类里面包含了用户最关心的问题，这个表格有多少行多少列，每一个view长什么样，需要填充什么数据，而把一些用户不关心的底层实现封装在内部。内部实现时会把PanelAdapter里面的view和数据传递给具体的adapter。
#### ScrollablePanel项目地址
[https://github.com/Kelin-Hong/ScrollablePanel](https://github.com/Kelin-Hong/ScrollablePanel)
作者：学日益_道日损
链接：http://www.jianshu.com/p/6d133f572ed2
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
            
