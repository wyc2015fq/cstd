# 基于 HTML5 WebGL 的 3D 网络拓扑图 - xhload3d - 博客园
　　在数据量很大的2D 场景下，要找到具体的模型比较困难，并且只能显示出模型的的某一部分，显示也不够直观，这种时候能快速搭建出 3D 场景就有很大需求了。但是搭建 3D 应用场景又依赖于通过 3ds Max 或 Maya 的专业 3D 设计师来建模，Unity 3D 引擎做图形渲染等，这对用户来说都是挑战！不过，[HT](http://www.hightopo.com/index.html) 一站式的提供了从建模到渲染，包括和 2D 组件呈现和数据融合的一站式解决方案。[HT](http://www.hightopo.com/index.html) 基于 WebGL 的 3D 技术的图形组件 ht.graph3dView 组件通过对 WebGL 底层技术的封装，与 [HT](http://www.hightopo.com/index.html) 其他组件一样， 基于 [HT](http://www.hightopo.com/index.html) 统一的 DataModel 数据模型来驱动图形显示，极大降低了 3D 图形技术开发的门槛，在熟悉[HT](http://www.hightopo.com/index.html) 数据模型基础上，一般程序员只需要 1 小时的学习即可上手 3D 图形开发。
　　好了，废话不多说，先附上 [Demo](http://www.hightopo.com/demo/blog_3dedge_20170630/index.html)：[http://www.hightopo.com/demo/blog_3dedge_20170630/index.html](http://www.hightopo.com/demo/blog_3dedge_20170630/index.html)
![](https://images2015.cnblogs.com/blog/591709/201706/591709-20170630123856664-219217034.png)
　　当然，这里的我只是用简单的图形来表示设备，脑洞大开的你当然可以将其换成更有意思的模型。
　　接下来看看我们是怎么做到的：
1、准备工作：
3D 和 2D 的 API 的设计上保持了很多一致性，3D 视图组件是 ht.graph3d.Graph3dView，  2D 视图组件是 ht.graph.GraphView，两者可共享同一数据模型 DataModel。在 [HT](http://www.hightopo.com/index.html) 中，为了让了获得接近真实三维物体的视觉效果，我们通过透视投影使得远的对象变小，近的对象变大，平行线会出现先交等更接近人眼观察的视觉效果：
![](https://images2015.cnblogs.com/blog/591709/201706/591709-20170630123945227-1145149101.png)
　　如上图所示，透视投影最终显示到屏幕上的内容只有截头椎体部分的内容，因此 GraphView 提供了 eye，center，up，far，near， fovy 和 aspect 参数来控制截头椎体的具体范围，我们在实际运用中用到更多的是 eye 和 center：
    getEye() | setEye([x, y, z])，决定眼睛（或 Camera）所在位置，默认值为 [0, 300, 1000]；
    getCenter() | setCenter([x, y, z])，决定目标中心点（或 Target）所在位置，默认值为 [0, 0, 0]；
详情看 [HT for Web 3D 手册](http://www.hightopo.com/guide/guide/core/3d/ht-3d-guide.html) 手册 （[http://www.hightopo.com/guide/guide/core/3d/ht-3d-guide.html](http://www.hightopo.com/guide/guide/core/3d/ht-3d-guide.html)）。
```
dataModel = new ht.DataModel();
g3d = new ht.graph3d.Graph3dView(dataModel);    
g3d.setEye(1800, 800, 1000);
g3d.setCenter(0, 100, 0);
g3d.setDashDisabled(false);
g3d.getView().style.background = 'rgb(10, 20, 36)';
g3d.addToDOM();
```
2、创建设备：
服务器，Demo 中的服务器其实是通过 addStyleIcon 方式在服务器的位置添加图片，详情可看 [HT for Web 入门手册](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)（[http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)）：
```
//注册图片
ht.Default.setImage('server', 'server.png');
var server = new ht.Node();
server.s3(0, 0, 0);
server.p3(0, 60, 0);
server.addStyleIcon('icon', {
　　position: 0,
　　width: 200, 
　　autorotate: true,
　　transparent: true,               
　　height: 200,
　　names: ['server']
});
dataModel.add(server);
```
　　工作台，这里的工作台实际上是立体圆柱来表示的，[HT](http://www.hightopo.com/index.html) 在 GraphView 的 2D 图形上，呈现各种图形是通过 style 的shape 属性决定，类似的 [HT](http://www.hightopo.com/index.html) 在 3D 上提供了 shape3d属性，预定义了多种 3D 的形体，详情见[HT for Web 3D 手册](http://www.hightopo.com/guide/guide/core/3d/ht-3d-guide.html)。不过在这里我并没有用预定义的图形，而是通过 ht.Default.createRingModel 的方式创建圆柱，该方法可以根据 xy 平面的曲线，环绕一周形成 3D 模型，所以可以用来定义多种圆形 3D 模型。
```
var desktop = new ht.Node();
desktop.s({
　　'3d.selectable': false,
　　'shape3d': ht.Default.createRingModel([
　　　　　　　　　　0, 40,
　　　　　　　　　　450, 40,
　　　　　　　　　　450, 0,
　　　　　　　　　　0, 40
　　], null, 20, false, false, 50),
　　'shape3d.color': '#003333'
});
desktop.s3(1, 1, 1);
dataModel.add(desktop);
```
　　平台上的设备，我们一共创建了 32 个设备：
```
var count = 32,
　　radius = 400,
　　index = count/2;
for (var i =  1; i <= count/2; i++) {
　　var device1_angle1 = Math.PI * 2 * (index - i) / count,
　　　　device1_angle2 = Math.PI * 2 * (index + i) / count,
　　　　device1_angle3 = Math.PI * 2 * index / count;
　　var device1_1 = createDevice(device1_angle1, radius, 60),
　　　　device1_2 = createDevice(device1_angle2, radius, 60),
　　　　device1_3 = createDevice(device1_angle3, radius, 60);
　　layoutDevice1(device1_1, device1_angle1);
　　var device1_edge1 = createEdge(device1_1, server, 'line1');
　　device1_edge1.s({'shape3d.color': 'rgb(205, 211, 34)'});
　　dataModel.add(device1_1);
　　dataModel.add(device1_edge1);
　　layoutDevice1(device1_2, device1_angle2);
　　var device1_edge2 = createEdge(device1_2, server, 'line1');
　　device1_edge2.s({'shape3d.color': 'rgb(205, 211, 34)'});
　　dataModel.add(device1_2);
　　dataModel.add(device1_edge2);
　　layoutDevice1(device1_3, device1_angle3);
　　var device1_edge3 = createEdge(device1_3, server, 'line1');
　　device1_edge3.s({'shape3d.color': 'rgb(205, 211, 34)'});
　　dataModel.add(device1_3);
　　dataModel.add(device1_edge3);
}
```
　　为了让创建的设备在平台上的布局更加合理，根据 index 计算出设备摆放角度，并且根据圆柱中心，圆盘半径和角度计算出每个设备摆放的位置：
```
function createDevice (angle, x, y) {
　　var node = new ht.Node();
　　cos = Math.cos(angle);
　　sin = Math.sin(angle);
　　node.p3(x*sin, y, x*cos);
　　return node;
}
```
　　其他设备，
```
var num = 18;
var h = [800, 900, 1000, 1100, 1200];
var v = [40, 60, 80, 100];
var colors = ['#fcfc63', '#00E1E4'];
for (var j = 0; j < num; j++) {
　　var device2_angle = Math.PI * j / num;
　　var device2 = createDevice(device2_angle, h[Math.floor(Math.random()*5)], v[Math.floor(Math.random()*4)]);
　　device2.s3(100, 20, 100); 
　　device2.s({
　　　　'shape3d': 'cylinder',
　　　　'shape3d.color': colors[Math.floor(Math.random()*2)]
   });
　　var device2_edge = createEdge(device2, desktop , 'line2');
　　device2_edge.s({'shape3d.color': 'rgb(0, 203, 94)'});
　　dataModel.add(device2);
　　dataModel.add(device2_edge);         
}
```
3、连线
[HT for Web](http://www.hightopo.com/index.html) 提供了默认的直线和多点的连线类型能满足大部分基本拓扑图形应用，但在这里我们需要根据实际需求绘制曲线，所以，需要用到自定义连线类型，详情看[HT for Web 连线类型手册](http://www.hightopo.com/guide/guide/plugin/edgetype/ht-edgetype-guide.html)：
用 ht.Default.setEdgeType(type, func, mutual) 函数可用于自定义新连线类型：
    type：字符串类型的连线类型，对应 style 的 edge.type 属性；
    func：函数类型，根据传入参数（edge，gap，graphView，sameSourceWithFirstEdge）返回连线走向信息
               edge：当前连线对象；
               gap：多条连线成捆时，本连线对象对应中心连线的间距；
               graphView：当前对应拓扑组件对象；
               sameSourceWithFirstEdge：boolean 类型，改连线是否与同组的第一条同源；
               返回值为 {points： new ht.List(...)，segments：new ht.List(...)} 结构的连线走向信息，segments 可取值如下：
                      1、moveTo，占用 1 个点信息；
                       2、lineTo，占用 1 个点信息；
                       3、quadraticCurveTo，占用 2 个点信息；
                       4、bezierCurveTo，占用 3 个点信息；
                       5、closePath，不占用点信息；
    mutual：该参数决定连线是否影响起始或结束节点上的所有连线，默认为 false 代表只影响同 source 和 target 的 EdgeGroup 中的连线，[HT](http://www.hightopo.com/index.html) 预定义的连线类型中，后缀为 2 的类型都是 mutural 为 true 的复杂连线类型。
在 Demo 中定义了两种类型的连线，分别为 line1 和 line：
```
ht.Default.setEdgeType('line1', function(edge){
　　var sourcePoint1 = edge.getSourceAgent().getPosition(),
　　　　targetPoint1 = edge.getTargetAgent().getPosition(),
　　　　points1 = new ht.List();       
　　points1.add(sourcePoint1);
　　points1.add({
　　　　x: (sourcePoint1.x + targetPoint1.x)/2 + 200,
　　　　e: sourcePoint1.e,
　　　　y: (sourcePoint1.y + targetPoint1.y)/2
　　});
　　points1.add(targetPoint1);                          
　　return {
　　　　points: points1,
　　　　segments: new ht.List([1, 3])
　　};
});
ht.Default.setEdgeType('line2', function(edge){
　　var sourcePoint = edge.getSourceAgent().getPosition(),
　　　　targetPoint = edge.getTargetAgent().getPosition(),
　　　　points = new ht.List();       
　　points.add(sourcePoint);
　　points.add({
　　　　x: (sourcePoint.x + targetPoint.x)/2,
　　　　e: ((sourcePoint.e + targetPoint.e)/2 || 0) - 300,
　　　　y: (sourcePoint.y + targetPoint.y)/2
　　});
　　points.add({
　　　　x: targetPoint.x,
　　　　e: targetPoint.e -80, 
　　　　y: targetPoint.y                            
　　});
　　return {
　　　　points: points,
　　　　segments: new ht.List([1, 3])
　　};
});
```
　　连线类型定义好，接下来就是创建连线，但是连线上还有流动效果，这个又怎么实现呢？我们 [HT](http://www.hightopo.com/index.html) 有扩展流动线插件，可以在 ht.Shape 和 ht.Edge 上增加流动效果，支持内部流动元素或用户自定义的流动元素沿着路径步进，要使用也非常方便，只需要引入 ht-flow.js 文件，详情可见 [HT for Web流动线手册](http://www.hightopo.com/guide/guide/plugin/flow/ht-flow-guide.html)（[http://www.hightopo.com/guide/guide/plugin/flow/ht-flow-guide.html](http://www.hightopo.com/guide/guide/plugin/flow/ht-flow-guide.html)），但是插件并不适用于 3D 模型中，那在 3D 模型中该怎么办呢？即使不能使用现成的插件，我们也可以实现流动效果，可以看[HT for Web 入门手册](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html) 中连线部分，我们可以将连线样式通过 edge.dash 设置为虚线后，动态改变 edge.dash.offset 虚线偏移，即可实现流动效果，所以，我们创建连线时：
```
function createEdge (source, target , type) {
　　var edge = new ht.Edge(source, target);
　　edge.s({
　　　　'edge.color': 'yellow',
　　　　'edge.dash': true,
　　　　'edge.dash.3d': true,
　　　　'edge.dash.width': 4,
　　　　'edge.type': type,                    
　　　　'edge.dash.color': 'rgb(10, 20, 36)',                    
　　　　'edge.dash.pattern': [20, 25]
　　});
　　edge.a({
　　　　'flow.enabled': true,
　　　　'flow.direction': -1,
　　　　'flow.step': 4
　　});
　　return edge;
}
```
　　最后，要让虚线流动起来，可以使用 [HT](http://www.hightopo.com/index.html) 中的调度，详情可看[HT for Web 调度手册](http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-guide.html)（[http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-　　guide.html](http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-guide.html)）：
```
flowTask = {
　　interval: 50,
　　action: function(data){
　　　　if(data.a('flow.enabled')){
　　　　　　var offset = data.s('edge.dash.offset') + data.a('flow.step') * data.a('flow.direction');
　　　　　　data.s('edge.dash.offset', offset);                        
　　　　}
　　}
};
dataModel.addScheduleTask(flowTask);
```
　　到这里，Demo 中的主要技术点都已经介绍了一遍，可以看出我们 [HT](http://www.hightopo.com/index.html) 的强大之处，当然我们官网上还有很多很有意思的效果，大家也可以看一看，也可以玩一玩我们的 [HT](http://www.hightopo.com/index.html) 感受它的强大之处，再次附上 Demo 地址： [http://www.hightopo.com/demo/blog_3dedge_20170630/index.html](http://www.hightopo.com/demo/blog_3dedge_20170630/index.html)。
