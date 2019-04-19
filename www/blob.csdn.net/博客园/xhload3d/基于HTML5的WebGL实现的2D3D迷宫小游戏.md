# 基于HTML5的WebGL实现的2D3D迷宫小游戏 - xhload3d - 博客园
为了实现一个基于HTML5的场景小游戏，我采用了[HT for Web](http://hightopo.com/)来实现，短短200行代码，我就能实现用“第一人称”来操作前进后退上下左右，并且实现了碰撞检测。
先来看下实现的效果：[http://hightopo.com/guide/guide/core/3d/ht-3d-guide.html#ref_collision](http://blog.csdn.net/xhload3d/article/details/55044051)
或者[http://v.youku.com/v_show/id_XMzA5MzUzODc4NA==.html?spm=a2h3j.8428770.3416059.1](http://v.youku.com/v_show/id_XMzA5MzUzODc4NA==.html?spm=a2h3j.8428770.3416059.1)视频中出现的帧的问题是我屏幕录制器的问题，真正操作的时候不会有，建议用上面的链接自己操作
玩玩，鼠标或者触屏都可以，不过我觉得最方便的还是操作键盘wsad控制上下左右。![](https://images2017.cnblogs.com/blog/591709/201710/591709-20171023103756801-1077056428.png)
我的想法是先把场景布局好，代码如下：
```
createHT([100, -20, 100], '#E74C3C');                  
createHT([-100, -20, 100], '#1ABC9C');                    
createHT([100, -20, -100], '#3498DB');                   
createHT([-100, -20, -100], '#9B59B6');   
createCurve([0, -20, 0]);
createCircle();
```
这几个都是自定义的函数，createHT为描绘HT形状的图，场景中有四个，所以调用了四次；createCurve是描绘场景中间的黄色的曲线；createCircle是描绘最外层的圆，因为不是全包的圆，所以也是描点画的。
[HT](http://hightopo.com/)中封装了一个组件，[ht.Shape](http://hightopo.com/guide/guide/core/shape/ht-shape-guide.html)（以下简称Shape），能够根据描点来自由描绘图形，可以通过shape.setPoints(pointsArray)将所有的点添加进数组中，并且设置到shape中，然后通过setSegments()设置线段数组信息，也就是用什么样的方式来连接两点，在Shape手册中有着重描写，感兴趣的可以参考[HT for Web Shape 手册](http://hightopo.com/guide/guide/core/shape/ht-shape-guide.html)。抽其中的一个描绘点的函数来看看：
```
function createHT(p3, color){
    shape = new ht.Shape();                
    shape.s({
        'shape.background': null,
        'shape.border.width': 10,
        'shape.border.color': color,
        'all.color': color
    });
    shape.setTall(40);
    shape.setThickness(5);
    shape.setPoints([                    
        // draw H
        {x: 20, y: 0},
        {x: 20, y: 100},
        {x: 20, y: 50},
        {x: 80, y: 50},
        {x: 80, y: 0},
        {x: 80, y: 100},
        // draw T
        {x: 120, y: 0},
        {x: 180, y: 0},
        {x: 150, y: 0},
        {x: 150, y: 100}                    
    ]);                                
    shape.setSegments([
        // draw H
        1, // moveTo
        2, // lineTo
        1, // moveTo
        2, // lineTo
        1, // moveTo
        2, // lineTo
        // draw T
        1, // moveTo
        2, // lineTo
        1, // moveTo
        2 // lineTo
    ]);                
    shape.p3(p3);
    dataModel.add(shape); 
    return shape;
}
```
 因为“HT”这个字眼要描绘的点比较多，所以代码看起来有点大，如果你看到如何描绘一个不完全的圆用20行代码来完成，而且包括样式，还是会惊讶的：
```
shape = new ht.Shape();                
shape.s({
    'shape.background': null,
    'shape.border.width': 10,
    'shape.border.color': '#D26911',
    'all.color': '#D26911'
});
shape.setTall(40);
shape.p3(0, -20, 0);
shape.setThickness(10);
                
var r = 300;
for(var i=0; i<36; i++){
    var angle = Math.PI * 2 * i / 36;
    shape.addPoint({
        x: r * Math.cos(angle),
        y: r * Math.sin(angle)
    });
}
                
dataModel.add(shape); 
return shape;
```
场景设置完毕，接下来要将在3d中“我”处于的位置在2d中也显示出来。首先我得先设置“我”是“第一人称漫游模式”，直接将g3d.setFirstPersonMode(true)即可。第一人称漫游模式本质是控制eye和center，如果没有设置第一人称漫游模式，那么鼠标或者触控板拖拽会绕着center旋转。详情参考[HT for Web 3D手册](http://hightopo.com/guide/guide/core/3d/ht-3d-guide.html)。
因为[HT 3D](http://hightopo.com/guide/guide/core/3d/ht-3d-guide.html)中封装了两个方法getEye和getCenter，这两个方法分别是获取camera的位置和目标中心点的位置，前者按照想象来说就比方你头上有个摄像机，你走到哪里它的中心点就拍摄到哪里，可以很方便的记录你的位置；后者就相当于你看出去的位置，但是这里跟我们人不太一样，因为人是可以广度看到大范围的，但是这个center相当于你眼球和都不能转动，是正前方的某一点的位置就是你的视线聚焦位置。
了解了getEye和getCenter后我们就可以获取当前位置和视线位置了：
```
g2d.addTopPainter(function(g){
    var eye = g3d.getEye(),
    center = g3d.getCenter();
                        
    g.fillStyle = 'red';
    g.strokeStyle = 'black';
    g.lineWidth = 1;
    g.beginPath();                    
    g.arc(eye[0], eye[2], 12, 0, Math.PI * 2, true); //绘制圆，而且还能实时获取3d中“我”的位置
    g.fill();
    g.stroke();  
                    
    g.strokeStyle = 'black';
    g.lineWidth = 2;
    g.beginPath(); 
    g.moveTo(eye[0], eye[2]);
    g.lineTo(center[0], center[2]); //绘制线，能实时更改“我”和“我的视线位置” 之间的线段   
    g.stroke(); 
});
```
但是在代码中我们发现，这个方法只被绘制了一次，如果不一直重绘，那么2d界面的“我”的位置和移动也是不会变的，所以我们又监听了3d中属性的变化：
```
g3d.mp(function(e){//根据3d上的“我”的位置和视线来实时更新2d界面
    if(e.property === 'eye' || e.property === 'center'){ //如果e属性变化为get/setEye，get/setCenter，那么重绘2d界面
        g2d.redraw();
    } 
});
```
 在2D中，我可以编辑图元，移动它的点，变化某个图元的大小，等等功能，只要变化了图元，那么我的碰撞测试就得更新：
```
function updateBoundaries(){
    boundaries = [];
    dataModel.each(function(data){//HT curve circle              
        boundaries = boundaries.concat(ht.Default.toBoundaries(data.getPoints(), data.getSegments()));
        // ht.Default.toBoundaries将不连续曲线转化成Graph3dView#setBoundaries(bs)需要的参数格式
    }); 
    g3d.setBoundaries(boundaries);//setBoundary()可指定碰撞边界
}
```
那么我们好奇的点在于，如何在拖拽图元改变大小的时候还能保持碰撞检测呢？
[HT](http://hightopo.com/)中有一个对于属性变化的监听事件addDataPropertyChangeListener()，可简写为md()，在我们拖拽图元的时候，绘制这个图元的基础points就会被改变，所以我们只要监听points有没有被改变就行了，如何使用这个事件可以参考[HT for Web 数据模型手册](http://hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html)
```
dataModel.md(function(e){//data属性变化事件    
    if(e.property === 'points'){//如果data属性变化为getPoints/setPoints，那么更新边界
        updateBoundaries();
    }
});
```
