# 基于 HTML5 Canvas 的 3D 碰撞检测 - xhload3d - 博客园
这是公司大神写的一个放官网上给用户学习的例子，我一开始真的不知道这是在干嘛，就只是将三个形状图元组合在一起，然后可以同时旋转、放大缩小这个三个图形，点击“Animate”就能让中间的那一个图元单独绕着某一个点旋转，表单最上方的“Axis”真的完全不知道拿来干嘛用的，觉得好累赘，而且是官网的 Demo，也没有解释。。。所以我今天得任务就是完全剖析这个例子！![奋斗](http://static.blog.csdn.net/xheditor/xheditor_emot/default/struggle.gif)
本文例子：[http://www.hightopo.cn/demo/3drotate/3d-rotate.html](http://www.hightopo.cn/demo/3drotate/3d-rotate.html)
首先让我们来看下这个案例：
![](https://images2017.cnblogs.com/blog/591709/201710/591709-20171020095805459-315445261.png)
我们来看看如何操作这个 3d 交互模型，可以直接滑动“Rotation”的滑动条，你会看到 3d 和左下角的 2d 上的图元都会旋转，接着点击“Axis”中的任意一个值，然后点击“Animate”，你会看到中间这个图元会旋转，同时滑动“Range”的滑动条，这是控制你旋转的幅度的，如果你调到“0”，那么就不会旋转，调到“30”就会旋转30度，以此类推。接着调整“Reset”你会发现，不是完全刷新这个界面，而是局部刷新两边的圆柱，根据这两个圆柱与中间节点之间的关联而重置的。
左下角的是整个 3d 场景内的俯视图，这样我们可以非常直观地看清图元的移动方向和位置。
可能你会好奇这个是怎么俯视图是怎么放上去的？如果 3d 中的图元变化，这个俯视图中的图元也会跟着变化么？如何把右上角的 form 表单和左下角的视图又是怎么放的？如何只移动 3d 二把这两个固定在这边？或者你可能还有别的问题，在这里我会尽量清楚地解答，实在找不到答案可以去官网 [HT for Web](http://www.hightopo.cn/) 查找你的问题。
好了，基础就是先布局，布 3d 场景，HT 在提供方法方面算是非常细致的了，平时我们生成网格可能就要花费一段时间，又是基础代码，新手开发人员都能很快上手呢~短短几行代码就能创建一个 3d 场景，简直太快。。。
```
dm = new ht.DataModel();
g3d = new ht.graph3d.Graph3dView(dm);
g3d.setGridVisible(true);//设置网格可视
g3d.setGridSize(m);//设置网格大小
g3d.setGridGap(w);//设置网格间距
g3d.setEye([-200, 150, 200]);//设置camera位置
g3d.getView().className = 'main';
document.body.appendChild(g3d.getView());
```
由于 HT 默认将所有组件都设置了 style “position:absolute”，所以当我们初始化这个组件之后，一定要在 style 中写上位置，并且将这个组件添加进你想要添加进的标签中，这个例子中，form 表单、2d 组件和3d 组件都互不依附，所以我们直接将这三个都添加进 body 中即可，还有一点值得注意的，所有 [HT](http://www.hightopo.cn/) 组件的最根层都是一个 div，是通过组件的 getView() 函数获得的。所以我们如果要添加进 HTML 标签中，肯定也要是 HTML 标签才行。
然后再界面的右上角放上 form 表单，放到右上角就直接设置 style 中的“top”“bottom”“left”“right”即可，2d 图同理：
```
formPane = new ht.widget.FormPane();
formPane.setLabelAlign('right');
formPane.getView().className = 'formpane';
document.body.appendChild(formPane.getView());
formPane.addRow(['Rotation:', {
    id: 'rotation',
    slider: {
        min: -180,
        max: 180,
        value: 0,
        step: 1,
        onValueChanged: function(e){
            node.setRotation(this.getValue() * Math.PI / 180);
        }
    }
}], [60, 0.1]);
```
因为这个例子的 form 表单中的行数和内容比较多，所以我这边就只取了一个 form 表单自定义行的例子。这边的“id”只是为了能快速查找到这个元素，slider 是 [HT form 表单](http://www.hightopo.cn/guide/guide/plugin/form/ht-form-guide.html)自定义的一个方法，滑动条功能，设置了该属性后 HT 将根据属性值自动构建 ht.widger.Slider 对象，具体参数可以参考 [HT for Web 表单手册](http://www.hightopo.cn/guide/guide/plugin/form/ht-form-guide.html)。
至于左下角的 2d 俯视图，这是通过跟 3d 共享同一个 [dataModel](http://www.hightopo.cn/guide/guide/core/datamodel/ht-datamodel-guide.html) 数据模型，只要我们绘制好了图形，然后添加进 dataModel 中去，不管是什么组件，只要调用了这个 dataModel 的都可以拥有dataModel 中的所有数据：
```
g2d = new ht.graph.GraphView(dm);
g2d.getView().className = 'g2d';
g2d.setEditable(true);
document.body.appendChild(g2d.getView());
ht.Default.callLater(g2d.fitContent, g2d, [true, 50, true]);
```
ht.Default.callLater(func, scope, args, delay) 获取全局下一个编号，其中 func 指的是回调函数，scope 指的是函数域，args 指的是函数参数列表，delay 则是延迟时间（毫秒）。这个函数可以在页面打开时回调 g2d.fitContent 函数，这个函数的作用域仅在 g2d 中，参数列表是 fitContent(anim, padding, notZoomIn) 函数的参数，这三个参数分别代表“是否使用动画”，“缩放后图元区域与拓扑边缘的距离”，以及“是否将最小缩放值限定为 1”。
接着将 3d 中的图元添加进去，这里我不截取全部代码，只取一个比较特别的有趣的图元，中间外层的透明图元：
```
shape = new ht.Shape();
shape.s({
    'all.reverse.cull': true,
    'all.color': 'rgba(0, 255, 0, 0.5)',
    'all.transparent': true
});
shape.setThickness(2);
dm.add(shape);
var resetShape = function() {
    var cs = node.getCorners(10, 10);
    cs.push(cs[0]);
    shape.setPoints(cs);
    g3d.setBoundaries(ht.Default.toBoundaries(cs));
};
resetShape();
```
代码中有比较有趣的几点：
1. 这边用了 node.getCorners() 这个方法，这个是获取四个点，对于 2d 来说就是左上、右上、右下、左下四个点；对于 3d 来说就是直接获取底面的四个“左上、右上、右下、左下”点，这个我反应了好一会儿才反应过来。。。并以这四个点为基础作为 shape 的 points。
2. 这边还用了 setBoundaries(boundaries) 函数,借用 ht.Default.toBoundaries 函数来将不符合 setBoundaries 函数参数的格式转换成它需要的参数格式。虽然我认为这一行在这个例子中没有什么作用，但是还是让我好好学习了一把碰撞测试。
我们在碰撞测试的时候经常要设置 g3d.setNear 函数，我实在没搞懂这个函数是拿来干嘛的，结果这个例子让我注意到，如果“我”的视线的近端截面位置也就是 setNear(1)，那么我能看到的就是比表面跟进 1 的距离，这个函数默认设置为 10，就算我们不设置这个值我们也能在 3d 中看到图元的内部去，刚刚我们介绍的 getCorners() 函数，其实它还有两个参数 xpadding 和 ypadding，分别代表“水平方向 padding”“垂直方向 padding”，也就是说，在我们获取四个角的同时，我们还能设置这四个角和边之间的 padding。只要将这个值设置得比 setNear 设置的大，我们就不会看到 3d 图元的内部中去了。
我们还注意到似乎是“废代码”的一行： cs.push(cs[0])。这个完全不是废代码啊，帮了很大忙呢！在 HT 中，用 [ht.Shape](http://www.hightopo.cn/guide/guide/core/shape/ht-shape-guide.html) 创建的图元，只要你不手动设置绘制关闭，那么就会停留在你最终绘制的位置，所以我把最后的一个点又和第一个点连起来，那么就是一个封闭的图形了，否则你会看到后面缺了一部分，像这样：
![](https://images2017.cnblogs.com/blog/591709/201710/591709-20171020100817677-1071597255.png)
我在其他文章中也提到过 HT 封装了一些很方便的方法和事件，比如 [dataModel#md](http://www.hightopo.cn/guide/guide/core/datamodel/ht-datamodel-guide.html#ref_datamodel)，监听数据的属性的变化，这边我们用了 md 方法来判断只有中间这个 node 能够绕着一个点旋转，具体参考 [HT for Web 数据模型手册](http://www.hightopo.cn/guide/guide/core/datamodel/ht-datamodel-guide.html)：
```
dm.md(function(e){
    if(e.data === node){
        if(e.property === 'rotation'){
            formPane.v('rotation', 180 / Math.PI * e.newValue);
        }
        resetShape();
    }
});
```
这边我想要说一下“绕着一个点旋转”的功能，这边没有用我们自定义的 anchor 锚点，但是功能类似，通过获取 form 表单上选择的“left、right、front、back”来设置旋转中心点，[HT 中 form 表单](http://www.hightopo.cn/guide/guide/plugin/form/ht-form-guide.html)通过 getValue(id) 简写成 v(id) 根据 id 获取对应 item 元素值：
```
formPane.addRow([
    {
         button: {
             label: 'Animate',
             onClicked: function(){
                 var dx = 0,
                 dy = 0,
                 range = formPane.v('range') * Math.PI / 180;
                 if(formPane.v('left')){
                     dx = -node.getWidth()/2;
                 }
                 if(formPane.v('right')){
                     dx = node.getWidth()/2;
                 }
                 if(formPane.v('back')){
                     dy = -node.getHeight()/2;
                 }
                 if(formPane.v('front')){
                     dy = node.getHeight()/2;
                 }
                 animate(node, range, dx, dy);
             }
         }
     },
     {
         button: {
             label: 'Reset',
             onClicked: function(){
                 node.setRotation(0);
             }
         }
     }
], [0.1, 0.1]);
```
看到这边你还是没懂？快去我们官网上从 [beginners](http://www.hightopo.cn/guide/guide/core/beginners/ht-beginners-guide.html) 重头开始学习吧！
