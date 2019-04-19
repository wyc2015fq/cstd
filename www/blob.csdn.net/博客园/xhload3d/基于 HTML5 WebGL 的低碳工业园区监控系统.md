# 基于 HTML5 WebGL 的低碳工业园区监控系统 - xhload3d - 博客园
# 前言
低碳工业园区的建设与推广是我国推进工业低碳转型的重要举措，低碳工业园区能源与碳排放管控平台是低碳工业园区建设的关键环节。如何对园区内的企业的能源量进行采集、计量、碳排放核算，如何对能源消耗和碳排放进行实时动态监测等问题，涉及多个技术领域，专业性强。其数据不仅要求准确，更要求真实可靠（即可核查、可溯源）。这是低碳工业园区“管控平台”建设的核心任务，也是当前我国工业园区建设中需要迫切解决的主要问题之一。
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180514163607370-677616199.gif)
[http://www.hightopo.com/demo/HTBuilding/index.html](http://www.hightopo.com/demo/HTBuilding/index.html)
这个 gif 图中显示的是一个 2D 3D 结合而成的低碳工业园区的能源监控系统，主要对各个楼宇以及园区整体的水、电等的使用量的实时监控。
# 代码实现
## 搭建场景
要创建出一个 3D 的低碳工业园区场景并不难，但是如何在同一个界面上同时显示 2D 和 3D 的场景呢？想要做出炫酷的效果，这种方式在很多情况下是非常有用的。
整个低碳工业园区的场景是搭建在 2D 上的，我们知道，HTML 给 DOM 元素设置图片只能用传统的栅格位图，但是如果怕图片被拉伸而导致图片模糊或者变形等结果，用 json 格式的矢量图片来实现是最好的，栅格位图在拉伸放大或缩小时会出现图形模糊，线条变粗出现锯齿等问题。 而矢量图片通过点、线和多边形来描述图形，因此在无限放大和缩小图片的情况下依然能保持一致的精确度。
首先我搭建了一个 2D 的场景用来放置我们的 json 矢量图，利用 ht.Default.xhrLoad 函数将 json 矢量背景图反序列化显示在 gv 上，这个 json 矢量背景图中除了作为背景的 node 还有另外两个节点，如下图，红线框起来的比较大的这个节点是用来装 3D 场景的，而右边框起来的比较小的节点是用来放置另外一个 gv 的（暂时还用不到，后期需要添加类似 form 表单的功能，所以我需要固定位置）：
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180514165758363-1724000966.png)
```
ht.Default.xhrLoad('displays/background.json', function(text) {
    dm.deserialize(text);// 反序列化数据到数据模型
    gv.addToDOM();// 将 2D 场景添加到 body 体中
});
```
这个 2D 场景作为背景的部分就设置完毕，接下来看看如何在 2D 场景的基础下放上 3D 场景。
## 2D 中添加 3D 场景
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180515120110216-1968477199.png)
向 2D 中添加 3D 也是非常容易，问题是如何使 3D 场景根据 2D 场景缩放和平移来进行自适应变化，使 3D 场景始终保持在 2D 场景的某个固定的位置？我是通过监听 gv 的属性变化事件，监听到 zoom、translate 等属性，对 3D 场景进行自动布局的操作：
```
var g3dInfo = create3D('g3dNode');
gv.mp(function(e) {// 监听 gv 的属性变化事件
    if (e.property === 'zoom' || e.property === 'translateX' || e.property === 'translateY' ) {
        layout(g3dInfo);
    }
});
function layout(info) {
    var rect = info.node.getRect(),// 获取场景依赖的节点的 矩形区域
        zoom = gv.getZoom(),// 获取当前 gv 的缩放值
        tx = gv.tx(),// 获取当前 gv 的水平平移值
        ty = gv.ty();// 获取当前 gv 的垂直平移值
    
        // 依赖的节点的大小根据 zoom 缩放值来进行缩放
        rect.x *= zoom,
        rect.y *= zoom,
        rect.width *= zoom,
        rect.height *= zoom;
    var x = rect.x + tx,
        y = rect.y + ty;
    // 设置场景自动布局
    if (info.g3d) info.g3d.layout(x, y, rect.width, rect.height);
}
```
眼尖的同学应该已经注意到了，我没有写出 create3D 函数的声明，就展示的效果而言，这个方法只是将场景 json 图纸反序列化到 3D 场景中，并追加了一个对象 info，将 3D 场景所依赖的 node 和 3D 场景的变量传进去：
```
function create3D(tag) {
    var g3d = new ht.graph3d.Graph3dView();// 3D 组件
    var dataModel = g3d.dm();// 获取 3D 场景的数据容器
    gv.getView().appendChild(g3d.getView());// 将 3D 场景添加到 2D 场景中
    ht.Default.xhrLoad('scenes/电云维.json', function(text) {// 加载 3D 场景的 json 矢量图纸
        dataModel.deserialize(text);// 反序列化数据到数据模型
    });
    
    // 停止事件的传播，阻止它被分派到其他 Document 节点
    g3d.getView().addEventListener('mousedown', function(e) { e.stopPropagation()});
    g3d.getView().addEventListener('mousewheel', function(e) { e.stopPropagation()});
    if (isFirefox=navigator.userAgent.indexOf("Firefox") > 0) {  
        g3d.getView().addEventListener('DOMMouseScroll', function(e) { e.stopPropagation()});
    }
    var info = {
        g3d: g3d,
        node: dm.getDataByTag(tag),
    };
    return info;
}
```
2D 和 3D 在鼠标事件上有很多相同的点，但是我们并不希望在操作 3D 场景的同时 2D 场景也跟着变化，所以上面代码中禁止了鼠标按下和滚轮的事件传播。
## 楼宇信息显示
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180515120037300-1179166762.png)
低碳工业园区监控系统实现的其中一个功能：点击楼宇视线移到楼宇显示到一个比较合适的位置，并且楼宇顶部显示一个面板用来展示当前楼宇的信息。这里我直接创建了一个节点，通过设置节点的 shape3d 属性为 billboard 即可显示为一个“面片”，面板非常好用，首先它只有一个面，在 3D 场景中如果需要大量的显示数据的节点，推荐用这个 billboard 类型，非常省性能。
```
// 创建在建筑上面的显示面板
var billboard = new ht.Node();
billboard.setScaleX(2);// 将节点 X 轴上放大 2 倍
billboard.setScaleTall(2);// 将节点 Y 轴上放大 2 倍
billboard.s({
    'shape3d': 'billboard',// 此类型为一个面片
    'shape3d.image': 'symbols/nodeForm.json',// 设置面片的显示图片为矢量图片
    'shape3d.autorotate': true,// 始终面向相机
    'shape3d.vector.dynamic': true,// 设置矢量图形
    '3d.visible': false// 不可见
});
billboard.setTag('billboard');// 设置节点的 tag 唯一属性
dataModel.add(billboard);// 将节点添加到数据容器中
```
通过点击不同的楼宇则将信息面板展示在当前点击的楼宇上方， 并根据不同的选中情况对 billboard 进行显隐的控制：
```
dataModel.sm().ms(function(e) {// 监听选中变化事件
    if (e.kind === 'set' || e.kind === 'append') {// 设置选中 及 追加选中
        billboard.s('3d.visible', true);
        var data = dataModel.sm().ld();// 获取当前选中的最后一个节点
        if (!data) return;
        billboard.p3(data.getPosition().x, data.getTall() + 200, data.getPosition().y);// 设置 billboard 的位置为当前选中的节点的上方
    }
    else if (e.kind === 'remove') {// 选中移除
        var data = dataModel.sm().ld();// 获取当前最后选中的节点
        if (data) {
            billboard.setPosition(data.getPosition().x, data.getPosition().y);
            billboard.setElevation(data.getTall() + 200);
        }
        else billboard.s('3d.visible', false);
    }
    else if (e.kind === 'clear') billboard.s('3d.visible', false);// 清除所有的选中后设置 billboard 不可见
});
```
（其他例子参考）
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180515124213011-171189548.png)
[http://www.hightopo.com/demo/large-screen-photovoltaic/](http://www.hightopo.com/demo/large-screen-photovoltaic/)
至于点击楼宇，从当前视线位置推到节点位置是通过 flyTo 函数，此函数在 6.2.2  版本是有三个参数，参数一为目标节点，参数二为是否动画，参数三为眼睛跟目标节点中心距离的计算，比如下面代码设置 0.5，表示眼睛在上述方向上动态计算距离以将目标适配到屏幕 0.5 里容纳。信息面板上方显示了当前点击的楼宇的名称，我是在设计 3D 场景的图纸时给对应的楼宇设置上 displayName 属性，当前显示则根据这个 displayName 来进行显示。
```
g3d.mi(function(e) {// 增加交互事件监听器
    if(e.kind === 'clickData'){
        g3d.flyTo(e.data, true, 0.5);// 将 eye 和 center 从当前位置“飞到”目标节点的位置 第二个参数若是1 则占满全屏。 6.2.2 版本以上有此方法
        var name = e.data.getDisplayName();
        // 由于 3D 中不能将模型组合到一起，所以我用追加选中的方法来解决
        dataModel.each(function(node) {
            if(node.getDisplayName() !== name) return;// 我将同一类型的节点的 displayName 设置相同
            dataModel.sm().appendSelection(node);
        })
    }
});
```
那么，只有一个 billboard，我们如何让这个 billboard 根据不同的楼宇显示不同的信息？这个时候矢量图标的优势又多了一个，通过对矢量图标中的某个部分进行数据绑定进行数据的动态变化，这边我三言两语也讲不完整，我就简单提一下如何实现，剩下的可以去官网中的数据绑定手册中查阅相关资料和具体实现。
前面给 billboard 设置了一个 shape3d.image 属性，设置的图片为 nodeForm.json，这个 json 中有四行文本显示，顶部的文本用来显示当前点击的楼宇的名称。
根据手册我们知道数据绑定的格式分为两种，一种是绑定 function 类型，另一种是绑定 string 类型，如下：
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180515115650688-751558173.png)
也就是说如果 HT 中没有定义我们需要的属性或者说一个矢量图上有多个相同的属性需要更改为不同的值，就可以通过 attr 来自定义属性，这里我用的就是这个方法：
```
"text": {
    "func": "attr@buildingName",
    "value": "赛普健身学院学生宿舍"
}
```
 数据绑定完成后，我们只需要根据这个绑定数据对当前引用这个 json 矢量图标的节点的业务属性变化即可：
```
// 不同的楼宇上显示的内容不同
billboard.a('buildingName', name);
billboard.a('electricUsage', (Math.random()*300).toFixed(2));
billboard.a('waterUsage', (Math.random()*300).toFixed(2));
billboard.a('gasUsage', (Math.random()*300).toFixed(2));
```
##  右侧数据显示
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180515124054572-1568036557.png)
3D 场景创建完毕，接下来如何在 3D 上面再加右边的两个数据显示面板？这里我是在前面 2D json 场景中已排布好位置的节点上添加了另外一个 2D 场景，用来显示整体场景数据。因为这个 gv 上有两个信息面板，所以我直接在 graphView 上添加了两个节点，并将节点添加到这个 graphView 的 dataModel 数据容器上，其他部分我就不再做解释了，都是基础的代码：
```
function createGV(tag) {
    var g2d = new ht.graph.GraphView();// 2D 拓扑场景
    var dataModel = g2d.dm();// 获取当前拓扑场景的数据容器
    gv.getView().appendChild(g2d.getView());// 将此拓扑场景添加到底层背景图上
    g2d.setInteractors([]);// 清除此组件上的交互
    // 添加两个节点到拓扑场景上
    var node = new ht.Node();
    node.setImage('symbols/form.json');
    node.setPosition(0, 0);
    dataModel.add(node);
    var node1 = new ht.Node();
    node1.setImage('symbols/form1.json');
    node1.setPosition(0, dm.getDataByTag(tag).getHeight()/3);
    dataModel.add(node1);
    g2d.fitContent();
    setInterval(function() {// form表单数据动态变化
        node.a('electricUse', (Math.random()*300).toFixed(2));
        node.a('waterUse', (Math.random()*300).toFixed(2));
        node.a('gasUse', (Math.random()*300).toFixed(2));
        node.a('tempUse', (RandomNumBoth(10, 40))+'');
        node.a('wetUse', (Math.floor((Math.random()*100)))+'');
    }, 3000);
    var info = {
        g2d: g2d,
        node: dm.getDataByTag(tag)
    }
    return info;
}
```
 以上，整个低碳工业园区监控系统的实现全部结束，有问题的或者建议都可以给我留言，或者直接访问官网（[http://hightopo.com/](http://hightopo.com/)）查阅对应的资料。
