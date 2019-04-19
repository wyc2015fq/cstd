# 基于 HTML5 WebGL 的 3D “弹力”布局 - xhload3d - 博客园
分子力(molecular force)，又称分子间作用力、范得瓦耳斯力，是指分子间的相互作用。当二分子相距较远时，主要表现为吸引力，这种力主要来源于一个分子被另一个分子随时间迅速变化的电偶极矩所极化而引起的相互作用；当二分子非常接近时，则排斥力成为主要的，这是由于各分子的外层电子云开始重叠而产生的排斥作用。
`HT for Web `提供了弹力布局（也称为力导向布局）的功能，即根据节点之间存在互斥力，相互连接的节点间存在引力， 弹力布局运行一段时间后，整体拓扑网络结构会逐渐达到收敛稳定的平衡状态。这个功能很有趣，今天我们就将它的魅力展现出来。
本例地址：[http://www.hightopo.com/demo/pipeline/index.html](http://www.hightopo.com/demo/pipeline/index.html)
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171122130754790-2033662965.gif)
使用弹力布局功能需要在引入 ht.js` `核心库之后，再引入一个 `ht-forcelayout.js `的弹力布局插件库，因为还用到了 form 表单，所以要引入 ht-form.js 的表单插件库：
```
<script src="../../guide/lib/core/ht.js"></script>
<script src="../../guide/lib/plugin/ht-forcelayout.js"></script>
<script src="../../guide/lib/plugin/ht-form.js"></script>
```
`ht.layout.Force3dLayout `类提供 `3D `弹力布局，构造函数可传入 `DataModel `和 `Graph3dView `两种参数。 默认仅对未选中图元进行布局，如果构造函数参数为 `Graph3dView `时，则视图组件的 `isMovable `和 `isVisible `函数将影响图元是否可布局， 图元 `style `上的 `layoutable `属性也可设为 `false `阻止图元参与布局。
介绍完 HT 封装的弹力布局的背景之后，接下来就是帮助你们也能轻松地实现这个效果。
首先我们定义一个颜色数组变量，存储各个弹力球的颜色，还定义了一个随机函数，用来生成数随机的数组中的颜色：
```
var colorList = ['#FFAFA4', '#B887C5', '#B9EA9C', '#CFD9E7', '#4590B8', '#FF9C30'], 
    colorLen = colorList.length;
var randomColor = function() {
    var ran = Math.random() * colorLen;
    return colorList[Math.floor(ran)];//随机6种颜色
};
```
接着创建弹力球，简单生成一个 3D 节点，通过设置这个节点的 style 样式属性来控制节点的显示方式，其中将“shape3d”设置为“sphere”即可将 ht.Node 六面体变成 3D 球体模型，再设置“shape3d”属性为前面定义的随机颜色，s3 是 HT 封装的设置 3D 节点大小的 setSize3d 函数的简写，最后将这个节点添加进数据模型 dataModel 中：
```
var createNode = function(dm) {//创建node节点 圆
    var node = new ht.Node();
    node.s({//设置样式为 setStyle 的简写
        'shape3d': 'sphere',
        'shape3d.color': randomColor()//设置随机颜色
    });
    node.s3(40, 40, 40);
    dm.add(node);
    return node;
};
```
现在效果图上出现的还有各个弹力球之间的连线，这个连线我们一看就觉得很不一般，也是通过构造一个一个节点，这个节点是通过 [HT for Web 建模手册](http://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html) setShape3dModel函数自定义的 ht.Default.createRingModel 根据 xy 平面的曲线，环绕一周形成的 3D 环形模型，将其命名为‘custom’：
```
ht.Default.setShape3dModel(//创建模型 根据xy平面的曲线，环绕一周形成3D模型。
    'custom', ht.Default.createRingModel( [0.5, 0.5, -0.2, 0, 0.5, -0.5], [1, 3] )
);
```
HT 将用户自定义的属性和 HT 默认的属性调用方法分为 node.a 和 node.s 这样就能将两者有效地区分开来（具体参照 [HT for Web 入门手册](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html) style 章节），我们在创建管线的时候就用了这种方法：
```
var updatePipeline = function(edge) {//重新设置edge的样式
    var pipeline = edge.a('pipeline');
    pipeline.s3(1, 1, 1);//设置大小
    pipeline.p3(0, 0, 0);//设置坐标
    var node1 = edge.getSourceAgent(),//获取图形上连接的起始节点
    node2 = edge.getTargetAgent();//获取图形上连接的目标节点
    pipeline.s('mat', createMatrix(node1.p3(), node2.p3(), 20));//3d整体图形矩阵变化
};
```
最神秘的是如何能做出让两个节点“若即若离”的效果？
我们知道，矩阵能描述任意线性变换。线性变换保留了直线和平行线，线性变换保留直线的同时，其他的几何性质如长度、角度、面积和体积可能被变换改变了。简单的说，线性变换可能“拉伸”坐标系，但不会“弯曲”或“卷折”坐标系。这个函数主要是将我们的连接线在拖动弹力球后被拖拉的连接线的进行一个“变化矩阵”的操作，变化矩阵也是 HT 封装的 ht.Default.createMatrix 函数，通过将节点的 style 属性 mat 设置为一个自定义的函数，就是将这个节点的坐标乘上在“mat”属性对应的值，也就是说如果当前这个管线的旋转角为 [Math.PI/6, 0, 0]，假设我们在 createMatrix 函数中设置 r3 为 [Math.PI/3, 0, 0]，那么这个节点会旋转 90 度。非常轻松地创建出变化矩阵：
```
var createMatrix = function(p1, p2, width) {//createMatrix(array, matrix)将一组JSON描述的缩放、移动和旋转等操作转换成对应的变化矩阵
    var vec = [p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]],
        dist = ht.Default.getDistance(p1, p2);//获取两点之间距离，或向量长度
    return ht.Default.createMatrix({
        s3: [width, dist, width],
    r3: [Math.PI/2 - Math.asin(vec[1]/dist), Math.atan2(vec[0], vec[2]), 0],
    rotationMode: 'xyz',
    t3: [(p1[0]+p2[0])/2, (p1[1]+p2[1])/2, (p1[2]+p2[2])/2]
    });
};
```
基础配件全部定义完毕，接着就是将“shape3d”属性设置为自定义的 3D 模型“custom” ，并将“layoutable”属性设置为“false”阻止图元参与布局，并将点之间的连线通过edge.a('pipeline', node)重新刷新，并添加进数据模型 dataModel 中：
```
var createEdge = function(dm, node1, node2) {//创建‘custom’模型的edge
    var node = new ht.Node();
    node.s({
        'shape3d': 'custom',
        'shape3d.color': '#ECE0D4',
        'layoutable': false
    });
    dm.add(node);
    var edge = new ht.Edge(node1, node2);
    edge.a('pipeline', node);
    edge.s('edge.color', 'rgba(0, 0, 0, 0)');
    dm.add(edge);
    return edge;
};
```
插：我们还可以在工业上用 HeatMap 热图上做文章，效果依旧很炫，具体地址 [http://hightopo.com/guide/guide/plugin/forcelayout/examples/example_heatmap3d.html](http://hightopo.com/guide/guide/plugin/forcelayout/examples/example_heatmap3d.html)
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171122141711461-1798815274.png)
界面上的图形全部绘制完毕，剩下的就只有 form 表单，首先将 form 表单添加进 HTML 页面，用的是 HT 封装的 ht.widget.FormPane 函数：
```
var formPane = new ht.widget.FormPane();
formPane.setWidth(230);
formPane.setHeight(125);
formPane.addToDOM();
```
记住，form 表单要设置宽高，不然不显示。
form 表单添加行是通过 addRow 函数，我们重点来说一下下面的几行，Color、Range 和 Intensity，这三个名字主要是用来控制“头灯”的。在 HT 中直接通过 setHeadlightColor/setHeadlightRange/setHeadlightIntensity 三个函数来控制“头灯”的颜色、范围以及灯的强度，onValueChanged 属性，顾名思义属性值改变之后触发的事件：
```
['Color', 'Range', 'Intensity'].forEach(function(name) {
    var obj = { id: name },
    func = function(oV, nV) {
        g3d['setHeadlight' + name](nV);// === g3d.setHeadlightColor(nV)/g3d.setHeadlightRange(nV)/g3d.setHeadlightIntensity(nV)
    };
    if (name === 'Color')
        obj.colorPicker = {//ht.widget.ColorPicker为颜色选择框 
        instant: true,
        value: g3d['getHeadlight' + name](),// === g3d.getHeadlightColor()
        onValueChanged: func
    };
    else 
        obj.slider = {//滑动条
            min: 0,
        max: name === 'Range' ? 20000 : 3,
        step: 0.1,
        value: g3d['getHeadlight' + name](),
        onValueChanged: func
        };
    formPane.addRow([ name, obj ], [ 70, 0.1 ]);
});
```
slider 和 colorPicker 都是 HT 自定义的滑动条和颜色选择器，详情请参考 [HT for Web 表单手册](http://hightopo.com/guide/guide/plugin/form/ht-form-guide.html)。
如果还有不懂的请咨询我，或者可以直接上 [HT for Web](http://hightopo.com/)官网查阅手册。
