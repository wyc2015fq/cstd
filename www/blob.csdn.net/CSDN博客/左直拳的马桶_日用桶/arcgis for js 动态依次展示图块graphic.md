# arcgis for js 动态依次展示图块graphic - 左直拳的马桶_日用桶 - CSDN博客
2017年12月04日 19:58:05[左直拳](https://me.csdn.net/leftfist)阅读数：498
怎样将一个图层中的图块（graphic），逐个显示在地图上，以呈现一种动画效果？
在我目前做的这个项目中，这些图块，其实都是同一个工程的开展面积，不同时期，面积不同。客户要求按照工程的历程，逐个展示这些面积，以形成一个动态、直观的效果。 
![这里写图片描述](https://img-blog.csdn.net/20171204195607525?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171204195622041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
思路其实也简单： 
1、将图层中的图块读出来 
2、依次在地图上显示这些图块 
3、依次，可以用arcgis for js自带的控件: ersi/dijit/TimeSlider 来控制
但真做起来，并不容易。因为： 
1、图块加载有个过程。并不是图层load完毕，图块就加载完毕了。 
2、arcgis for js 3.19 版本中，并没有一个图块加载完毕的事件
因为我们要依次显示图块，就应该获得所有图块，并按工程时间进行排序。所以，如何得知图块已加载完毕？ 
看了网上的文章，知道可以用图层事件：update-end 并结合条件语句进行判断：
```
featurelayer.on("update-end", function (info) {
    if (info.target.graphics.length > 0) {//图块貌似已经加载完毕
        //可以进行timeslider控件初始化了
    }
});
```
但是这个”update-end”事件并不是为图块专设的，它代表的是地图变化事件，图块加载完成，拖动，都会触发这个事件。所以上述语句还不够，还要加一个判断，以避免反复初始化timeslider控件。
```java
var init = false;
featurelayer.on("update-end", function (info) {
    if (info.target.graphics.length > 0 && !init) {//图块貌似已经加载完毕,且未初始化timeslider控件
        init = true;
        //可以进行timeslider控件初始化了
    }
});
```
这样就可以保证初始化代码仅执行一次。
然后又发现，图块根本没显示出来。明明已经拿到图块了，里面的各种属性，数据都能读出来，怎么就不能显示呢？经过反复调试，原来图块虽然能读出来，但图块并未加载完成，由于上述代码仅执行一次，所以在这段代码里面显示图块，往往并不能成功。
最后是用两个事件来最终确定图块装载完毕时机的：
```java
fl.on("graphic-node-add", function (g) {
    g.graphic.hide();//每个图块加载的时候，先hide，成功后，图块graphic的属性visible == false
});
var init = false;
fl.on("update-end", function (info) {
    if (info.target.graphics.length > 0 && !init) {
        var tg = info.target.graphics;
        if (tg[0].visible == false) {//graphic.visible默认是true，而在事件 graphic-node-add 中，graphic已置为不可见。如果此时读到graphic.visible == true，说明该图块尚未加载完成
            //可以进行timeslider控件初始化了
        }
    }
});
```
这是我弄的时候，耗费时间最长的环节。至于TimeSlider，只是一个控件，没什么好讲的。唯一要提醒的是，并不需要将TimeSlider加到map对象。 
在acgis for js 3.19版本中吗，千万不要执行以下这条语句
`map.setTimeSlider(timeSlider);`
这样会导致图块根本出不来，去掉就好了。但在有的地图中，有无这句都没问题。不知道是做什么用的，完全多余的一条语句。
