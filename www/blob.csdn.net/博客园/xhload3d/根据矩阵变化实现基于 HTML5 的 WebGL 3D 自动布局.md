# 根据矩阵变化实现基于 HTML5 的 WebGL 3D 自动布局 - xhload3d - 博客园
在数学中，矩阵是以行和列排列的数字，符号或表达式的矩形阵列，任何矩阵都可以通过相关字段的标量乘以元素。矩阵的主要应用是表示线性变换，即f（x）= 4 x等线性函数的推广。例如，旋转的载体在三维空间是一个线性变换，这可以通过一个表示旋转矩阵 [R ：如果v是一个列向量描述（只有一列的矩阵）的位置在空间中的点，该产品器Rv是列矢量描述旋转后该点的位置。两个变换矩阵的乘积是表示两个变换组成的矩阵。矩阵的另一个应用是线性方程组的解。如果矩阵是方形的，可以通过计算其行列式来推断它的一些性质。例如，当且仅当其行列式不为零时，方阵具有相反的 。从矩阵的特征值和特征向量中可以看到线性变换的几何结构（以及其他信息）。
矩阵的应用可以在大多数科学领域找到。在物理学的每一个分支，包括经典力学，光学，电磁学，量子力学和量子电动力学，都被用来研究物理现象，比如刚体的运动。在计算机图形学中，它们被用来操作三维模型并将其投影到二维屏幕上。在概率论和统计学中，随机矩阵被用来描述概率集; 例如，它们在PageRank算法中用于对Google搜索中的页面进行排名。[5] 矩阵演算概括经典分析概念，如衍生物和指数更高的尺寸。矩阵在经济学中被用来描述经济关系系统。
![](http://img.blog.csdn.net/20171112222342788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGhsb2FkM2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
首先，这个实现真的是非常的酷，我从来不知道分子中的引力和斥力结合矩阵能做出这么炫酷的效果，而且还是代码量非常少的情况下，这个例子在医疗界还有生物界应该是应用非常广泛的，但是如果好好利用，在工业方面也一定有不小的成就。
[http://www.hightopo.com/demo/pipeline/index.html](http://www.hightopo.com/demo/pipeline/index.html)
![](http://img.blog.csdn.net/20171112201931254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGhsb2FkM2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来我的任务就是帮助你们也能轻松地实现这个效果，[HT](http://hightopo.com/) 将弹力布局的 js 文件放到了一个独立的文件中，在引用这个 js 文件需要先引入 ht.js，因为我们还做了一个 form 表单，所以也要引入 form 表单的 js 文件， 并不是所有的 HT 封装的的功能都需要引入一个特别的 js 文件，需要引入额外的 js 文件的手册中顶部都会有介绍，这里 forcelayout 弹力布局的 js 和 form 表单的 js 的排放顺序没有关系：
```
<script src="../../guide/lib/core/ht.js"></script>
<script src="../../guide/lib/plugin/ht-forcelayout.js"></script>
<script src="../../guide/lib/plugin/ht-form.js"></script>
```
首先我们定义一个颜色数组变量，存储各个弹力球的颜色，还定义了一个随机函数，用来生成数随机的数组中的颜色：
```
var colorList = ['#FFAFA4', '#B887C5', '#B9EA9C', '#CFD9E7', '#4590B8', '#FF9C30'],
colorLen = colorList.length;
var randomColor = function() {
    var ran = Math.random() * colorLen;
    return colorList[Math.floor(ran)];//随机6种颜色
};
```
接着创建弹力球，简单生成一个 3D 节点，通过设置这个节点的 style 样式属性来控制节点的显示方式，其中将“shape3d”设置为“sphere”即可将 ht.Node 六面体变成 3D 球体模型，再设置“shape3d”属性为前面定义的随机颜色，s3 也是 HT 封装的设置 3D 节点大小的函数，最后将这个节点添加进数据模型 dataModel 中：
```
var createNode = function(dm) {//创建node节点 圆
    var node = new ht.Node();
    node.s({
        'shape3d': 'sphere',
    'shape3d.color': randomColor()
    });
    node.s3(40, 40, 40);
    dm.add(node);
    return node;
};
```
现在效果图上出现的还有各个弹力球之间的连线，这个连线我们一看就觉得很不一般，也是通过构造一个一个节点，这个节点是通过 [HT for Web 建模手册](http://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html) setShape3dModel函数自定义的一个 3D 模型，将其命名为‘custom’：
```
ht.Default.setShape3dModel(//创建模型 根据xy平面的曲线，环绕一周形成3D模型。
    'custom', ht.Default.createRingModel( [0.5, 0.5, -0.2, 0, 0.5, -0.5], [1, 3] )
);
```
HT 将用户自定义的属性和 HT 默认的属性调用方法分为 node.a 和 node.s 这样就能将两者有效地区分开来，我们在创建管线的时候就调用了这种方法：
```
var updatePipeline = function(edge) {//重新设置edge的样式
    var pipeline = edge.a('pipeline');
    pipeline.s3(1, 1, 1);
    pipeline.p3(0, 0, 0);
    var node1 = edge.getSourceAgent(),//获取图形上连接的起始节点
    node2 = edge.getTargetAgent();
    pipeline.s('mat', createMatrix(node1.p3(), node2.p3(), 20));//3d整体图形矩阵变化
};
```
我们知道，矩阵能描述任意线性变换。线性变换保留了直线和平行线，线性变换保留直线的同时，其他的几何性质如长度、角度、面积和体积可能被变换改变了。简单的说，线性变换可能“拉伸”坐标系，但不会“弯曲”或“卷折”坐标系。这个函数主要是将我们的连接线在拖动弹力球后被拖拉的连接线的进行一个“变化矩阵”的操作，变化矩阵也是 HT 封装的 ht.Default.createMatrix 函数，能够非常轻松地创建出变化矩阵：
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
我们还可以在工业上用 HeatMap 热图上做文章，效果依旧很炫，具体地址[http://hightopo.com/guide/guide/plugin/forcelayout/examples/example_heatmap3d.html](http://hightopo.com/guide/guide/plugin/forcelayout/examples/example_heatmap3d.html)
![](http://img.blog.csdn.net/20171112201817615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGhsb2FkM2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里我们还是专注地继续完成这个例子。
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
slider 和 colorPicker 都是 HT 自定义的滑动条和颜色选择器，详情请参考 [HT for Web 表单手册](http://hightopo.com/guide/guide/plugin/form/ht-form-guide.html)。
如果还有不懂的请咨询我，或者直接上 [HT for Web](http://hightopo.com/) 官网查阅手册。
