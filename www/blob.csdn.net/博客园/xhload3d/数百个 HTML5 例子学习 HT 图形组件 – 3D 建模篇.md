# 数百个 HTML5 例子学习 HT 图形组件 – 3D 建模篇 - xhload3d - 博客园
[http://www.hightopo.com/demo/pipeline/index.html](http://www.hightopo.com/demo/pipeline/index.html)
![screen-shot-2016-09-30-at-1-17-33-am](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-30-at-1.17.33-AM.png)
《[数百个 HTML5 例子学习 HT 图形组件 – WebGL 3D 篇](http://www.hightopo.com/blog/504.html)》里提到 [HT](http://www.hightopo.com/) 很多情况下不需要借助 3Ds Max 和 Blender 等专业 3D 建模工具也能做出很多效果，例如  [http://www.hightopo.com/guide/guide/core/3d/examples/example_3droom.html](http://www.hightopo.com/guide/guide/core/3d/examples/example_3droom.html) 这个 3D 电信机房监控例子整个都是通过 [HT](http://www.hightopo.com/) 提供的 API 构建而成：
![screen-shot-2016-09-29-at-11-12-18-pm](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-29-at-11.12.18-PM.png)
不过这个例子中的模型都比较规矩，也就消防栓由一个球 + 圆通构成，其他图形通过 HT 提供的基本 Node 以及 Shape 对象即可搞定：
![screen-shot-2016-09-29-at-11-15-39-pm](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-29-at-11.15.39-PM.png)
但这并不意味着 API 只能做简单的模型，《[HT for Web 建模手册](http://www.hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html)》中介绍的 [HT](http://www.hightopo.com/) 建模插件可以让有想象力的同学做出各种不可思议的效果。例如这个餐座椅的例子：[http://www.hightopo.com/guide/guide/plugin/modeling/examples/example_custommodel.html](http://www.hightopo.com/guide/guide/plugin/modeling/examples/example_custommodel.html)
![Screen Shot 2015-01-07 at 11.46.14 PM](http://www.hightopo.com/blog/wp-content/uploads/2015/01/Screen-Shot-2015-01-07-at-11.46.14-PM.png)
对于这个餐座椅的例子，特别是一些不规则的花盆、酒杯、圣诞树和那颗爱心，很多人好奇我们是怎么搞出来的。其实蛮简单，就用了《[HT for Web 建模手册](http://www.hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html)》中的 createRingModel 和 createExtrusionModel 两个构建模型的函数，其中 createRingModel 顾名思义用来构建围绕一圈的环状模型，createExtrusionModel 用来构建基于某个形状的凸出效果，这两个函数生成的 3D 模型都是靠平面的 2D 图形衍生而来，都是靠 HT 系统中构建 2D 不规则多边形时采用的 Points 和 Segments 两个数组参数搞定， Points 和 Segments 的意义可参考 《[HT for Web 形状手册](http://www.hightopo.com/guide/guide/core/shape/ht-shape-guide.html)》：
可生成不规则的 3D 地板：[http://www.hightopo.com/guide/guide/core/shape/examples/example_floor.html](http://www.hightopo.com/guide/guide/core/shape/examples/example_floor.html)
![tt](http://www.hightopo.com/blog/wp-content/uploads/2016/09/tt.png)
可生成不规则的 3D 管线：[http://www.hightopo.com/guide/guide/core/shape/examples/example_polyline.html](http://www.hightopo.com/guide/guide/core/shape/examples/example_polyline.html)
![bb](http://www.hightopo.com/blog/wp-content/uploads/2016/09/bb.png)
这样大家应该理解了原理，但餐座椅的那几个不规则形状的 magic 参数是如果得来的呢，这还是得借助辅助工具：[http://www.hightopo.com/demo/3dmodel/index.html](http://www.hightopo.com/demo/3dmodel/index.html)，
![screen-shot-2016-09-30-at-12-12-36-am](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-30-at-12.12.36-AM.png)
![screen-shot-2016-09-30-at-12-15-28-am](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-30-at-12.15.28-AM.png)
![screen-shot-2016-09-30-at-12-17-49-am](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-30-at-12.17.49-AM.png)
这个工具多年前为写例子随意搞的，代码挺简单大家直接看 [http://www.hightopo.com/demo/3dmodel/index.html](http://www.hightopo.com/demo/3dmodel/index.html) 源代码即可，写的比较简陋但挺实用，如何导出？打开控制台，自己打印出 shape 对象的 sements 和 points 参数即可，或等我有空了再来写个可导入导出更完整的例子，或者 you can you up?
其实也不仅仅也用于 Node 节点类型对象的建模，对于连线其实也可以用模型来搞定，例如[http://www.hightopo.com/guide/guide/plugin/forcelayout/examples/example_forcelayout3.html](http://www.hightopo.com/guide/guide/plugin/forcelayout/examples/example_forcelayout3.html)这个 3D 弹力拓扑图例子，很多人已经觉得挺酷炫了，但我一直对这呆板规矩的管道连线很不爽，于是突发奇想搞了个像狗骨头似的两头粗中间细的连线效果，整个 3D 拓扑图例子一下子高大上了许多：
[http://www.hightopo.com/demo/pipeline/index.html](http://www.hightopo.com/demo/pipeline/index.html)
![screen-shot-2016-09-30-at-12-26-26-am](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-30-at-12.26.26-AM.png)
这个例子原理是这样的，将连线 Edge 设置成透明不可见的，然后针对每个 Edge 对应一个 Node 节点，这个节点的形状就是被拉伸并定位到连线位置替代连线来显示，而 Node 图形在还没拉伸之前长得如下：
![screen-shot-2016-09-30-at-12-54-50-am](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-30-at-12.54.50-AM.png)
这里还有个细节是通过 createMatrix 函数，为每个管线设置一个指向两节点位置的矩阵坐标变换参数到 style 的 mat 属性上，矩阵预算不理解也没关系，直接照抄例子中代码即可，为了方便大家理解我搞了个两个节点一条连线更简单的例子供参考：
![screen-shot-2016-09-30-at-1-04-12-am](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-30-at-1.04.12-AM1.png)
今天只是抛砖引玉，《[HT for Web 建模手册](http://www.hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html)》中还有众多 API 函数，只要有想象力还可以折腾出无数的花样，后续有空我再借助 [HT for Web](http://www.hightopo.com/)的 WebGL 3D 自定义建模功能多搞些实用的例子。
[http://www.hightopo.com/demo/pipeline/index.html](http://www.hightopo.com/demo/pipeline/index.html)
![screen-shot-2016-09-30-at-1-20-13-am](http://www.hightopo.com/blog/wp-content/uploads/2016/09/Screen-Shot-2016-09-30-at-1.20.13-AM.png)
```
ht.Default.setShape3dModel(
    'custom', ht.Default.createRingModel(
        [0.5, 0.5, -0.2, 0, 0.5, -0.5], [1, 3]
    )
);
var colorList = ['#FFAFA4', '#B887C5', '#B9EA9C', '#CFD9E7', '#4590B8', '#FF9C30'],
    colorLen = colorList.length;
var randomColor = function() {
    var ran = Math.random() * colorLen;
    return colorList[Math.floor(ran)];
};
var init = function() {
    var dm = new ht.DataModel(),
        g3d = window.g3d = new ht.graph3d.Graph3dView(dm);
    g3d.getBrightness = function() { return null; };
    g3d.isMovable = function(node) { return node.s('shape3d') !== 'custom'; };
    g3d.addToDOM();
    var edgeList = initDataModel(dm),
        forceLayout = new ht.layout.Force3dLayout(g3d);
    forceLayout.onRelaxed = function() {
        edgeList.forEach(updatePipeline);
    };
    forceLayout.start();
    initFormPane(g3d);
};
var initDataModel = function(dm) {
    var root = createNode(dm),
        iNode, jNode, j,
        edgeList = [];
    for (var i = 0; i < 3; i++) {
        iNode = createNode(dm);
        edgeList.push(createEdge(dm, root, iNode));
        for (j = 0; j < 3; j++) {
            jNode = createNode(dm);
            edgeList.push(createEdge(dm, iNode, jNode));
        }
    }
    return edgeList;
};
var createNode = function(dm) {
    var node = new ht.Node();
    node.s({
        'shape3d': 'sphere',
        'shape3d.color': randomColor()
    });
    node.s3(40, 40, 40);
    dm.add(node);
    return node;
};
var createEdge = function(dm, node1, node2) {
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
var updatePipeline = function(edge) {
    var pipeline = edge.a('pipeline');
    pipeline.s3(1, 1, 1);
    pipeline.p3(0, 0, 0);
    var node1 = edge.getSourceAgent(),
        node2 = edge.getTargetAgent();
    pipeline.s('mat', createMatrix(node1.p3(), node2.p3(), 20));
};
var createMatrix = function(p1, p2, width) {
    var vec = [p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]],
        dist = ht.Default.getDistance(p1, p2);
    return ht.Default.createMatrix({
        s3: [width, dist, width],
        r3: [Math.PI/2 - Math.asin(vec[1]/dist), Math.atan2(vec[0], vec[2]), 0],
        rotationMode: 'xyz',
        t3: [(p1[0]+p2[0])/2, (p1[1]+p2[1])/2, (p1[2]+p2[2])/2]
    });
};
var initFormPane = function(g3d) {
    var formPane = new ht.widget.FormPane();
    formPane.setWidth(230);
    formPane.setHeight(125);
    formPane.addToDOM();
    var view = formPane.getView();
    view.style.background = 'rgba(186, 186, 186, 0.7)';
    view.style.top = '10px';
    view.style.left = 'auto';
    view.style.right = '7px';
    formPane.addRow([{ element: 'Headlight:', font: 'bold 12px arial, sans-serif' }, {
        id: 'disable',
        checkBox: {
            label: 'disable',
            value: g3d.isHeadlightDisabled(),
            onValueChanged: function(oV, nV) {
                g3d.setHeadlightDisabled(nV);
            }
        }
    }], [70, 0.1]);
    formPane.addRow([], [0.1], 1.01, {background: '#43AFF1'});
    ['Color', 'Range', 'Intensity'].forEach(function(name) {
        var obj = { id: name },
            func = function(oV, nV) {
                g3d['setHeadlight' + name](nV);
            };
        if (name === 'Color')
            obj.colorPicker = { 
                instant: true,
                value: g3d['getHeadlight' + name](),
                onValueChanged: func
            };
        else 
            obj.slider = {
                min: 0,
                max: name === 'Range' ? 20000 : 3,
                step: 0.1,
                value: g3d['getHeadlight' + name](),
                onValueChanged: func
            };
        formPane.addRow([ name, obj ], [ 70, 0.1 ]);
    });
};
```
