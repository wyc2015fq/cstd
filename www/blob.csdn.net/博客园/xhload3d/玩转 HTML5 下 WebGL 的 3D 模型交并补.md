# 玩转 HTML5 下 WebGL 的 3D 模型交并补 - xhload3d - 博客园
建设性的立体几何具有许多实际用途，它用于需要简单几何对象的情况下，或者数学精度很重要的地方，几乎所有的工程 CAD 软件包都使用 CSG（可以用于表示刀具切削，以及零件必须配合在一起的特征）。CSG 是 [Constructive Solid Geometry](https://en.wikipedia.org/wiki/Constructive_solid_geometry) 建模技术的简称，通过裁剪 subtract、融合 union 和相交 intersect 的运算，组合出复杂模型效果，HT 封装了 ht.CSGNode 和 ht.CSGShape 等图元类型来支持 CSG 的组合功能，常用于墙面的门窗挖空凿洞的应用场景。
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171108213736450-1893036316.png)
CSG 对象可以用二叉树表示，其中叶子表示基元，节点表示操作。在这个图中，节点被标记 ∩ 为交集，∪ 为并集，- 为差集。CSG 提供的模型或表面看起来很复杂，但实际上不过是巧妙组合或分解对象。
ht.CSGNode 继承于 ht.Node，当 style 的 shape3d 属性为空时显示为六面体效果，CSGNode 如果通过 setHost 吸附到 宿主 CSGNode 或 CSGShape 后，宿主 CSGNode 或 CSGShape 可与吸附的 CSGNode 图元进行 CSG 的组合建模。详情请参考 [HT for Web 建模手册](http://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html#ref_csgnode)[CSGNode 章节](http://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html#ref_csgnode)。这里我用 CSG 的概念写了一个例子，让大家能更好地理解这个概念。
本例 Demo 地址： [http://hightopo.com/guide/guide/plugin/modeling/examples/example_bookshelf.html](http://hightopo.com/guide/guide/plugin/modeling/examples/example_bookshelf.html)
先来看下效果图：
![](http://img.blog.csdn.net/20171107144228136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGhsb2FkM2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
从上面效果图可以看到，我们将界面分为三个部分，这三个部分先是右边部分上下分割，然后将整个界面左右分割，HT 用封装好的 ht.widget.SplitView 进行界面的分割，然后将分割组件添加进底层 div 中：
```
dm = new ht.DataModel();// 数据模型            
treeView = new ht.widget.TreeView(dm); //树组件                                                                                                 
gv1 = new ht.graph3d.Graph3dView(dm); //3D 组件  
gv2 = new ht.graph3d.Graph3dView(dm);
splitView = new ht.widget.SplitView(gv1, gv2, 'v', 0.6);//分割组件 
mainSplit = new ht.widget.SplitView(treeView, splitView, 'h', 0.27);   
                
view = mainSplit.getView();  
view.className = 'main';
document.body.appendChild(view);    
window.addEventListener('resize', function (e) {
    mainSplit.invalidate();
}, false);
```
上面代码是一种非常常见的在 HTML 中添加 HT 组件的方法，详情可参考 [HT for Web 入门手册](http://hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)组件章节。这种方法进行添加 HT 组件有一个需要注意的点，因为 HT 一般都以设置 position 为 absolute 的绝对定位方式，必须设置 left、right、top、bottom 等等基础 css 样式，像这样：
```
.main {
     margin: 0px;
     padding: 0px;
     position: absolute;
     top: 0px;
     bottom: 0px;
     left: 0px;
     right: 0px;
}
```
所以为了最外层组件加载填充满窗口的方便性，HT 的所有组件都有 addToDOM 函数，其思想逻辑如下，其中 iv 是 invalidate 的缩写：
```
addToDOM = function(){   
    var self = this,
    view = self.getView(),   
    style = view.style;
    document.body.appendChild(view);            
    style.left = '0';
    style.right = '0';
    style.top = '0';
    style.bottom = '0';      
    window.addEventListener('resize', function () { self.iv(); }, false);            
}
```
以后我们在代码中就可以直接调用 addToDOM 函数，而不用写一大堆代码了，上面代码用 addToDOM 取代之后的代码如下，而且不用描绘 css 样式： 
```
dm = new ht.DataModel();// 数据模型             
treeView = new ht.widget.TreeView(dm); //树组件                                                             
gv1 = new ht.graph3d.Graph3dView(dm); //3D 组件  
gv2 = new ht.graph3d.Graph3dView(dm);
splitView = new ht.widget.SplitView(gv1, gv2, 'v', 0.6);//分割组件 
mainSplit = new ht.widget.SplitView(treeView, splitView, 'h', 0.27);   
mainSplit.addToDOM();
```
界面分配好之后我们就要对其添加内容了，界面的左边部分是 HT 封装的树组件，我在之前的文章写到过，树组件是一个非常方便的绘制树形关系的组件，开发人员能够轻松地从数据模型 DataModel 中获取数据和节点之间的关系放到树上，只需要在树组件声明的过程中，将对应的数据模型 DataModel 放进树组件的参数即可，当然我们还扩展了很多跟树组件有关的函数，非常方便实用，这里我们只用了 expandAll 函数，将所有对象展开：
```
treeView = new ht.widget.TreeView(dm); //树组件   
treeView.expandAll();
```
右边部分上下分为两部分，都是 3D 场景，就是设置显示有点不同，其他完全相同，上面的 3D 场景重载了 getVisibleFunc 函数，如果元素的 showMe 属性为 true，则可视；如果节点为 ht.CSGNode 类型并且节点的 getHost 函数的参数为空，则不可视；其他情况均可视：
```
gv1.setVisibleFunc(function(data){
    if(data.showMe){
        return true;
    }
    if(data instanceof ht.CSGNode && data.getHost()){
        return false;
    }
    return true;
});
```
我们先向 3D 场景中添加元素对象，我们先解释中间的书架，对两边的书架有缺的再进行补充。首先我们添加了一个 ht.CSGNode 节点 shelf，作为书架的主节点，其他的节点都是依附于这个节点的，对这个节点设置了位置、大小、名称以及六个面的颜色，然后添加进数据模型 DataModel： 
```
var shelf = new ht.CSGNode();
shelf.s3(500, 400, 120);
shelf.p3(0, 200, 0);
shelf.setName('shelf1');
shelf.s({
    'all.color': '#E5BB77'
});
dm.add(shelf);
```
接着向这个 shelf 中添加 10 个节点，做书架的格子效果，并设置依附关系和父子关系添加进数据模型中：
```
for(var i=0; i<2; i++){
    for(var j=0; j<5; j++){
        var clipNode = new ht.CSGNode();
        clipNode.setHost(shelf);
        clipNode.s3(80, 100, 120);
        clipNode.p3(-200+j*100, 340-i*120, 20);
        clipNode.setName('substract-'+i+'-'+j);
        clipNode.s('batch', 'tt');
        clipNode.setParent(shelf);
        dm.add(clipNode);
    }
}
```
为了让书架变得更美观一点，我们在书架的上下左右都加上了 ht.CSGNode，最后为了更加具象化，我们还添加了一本书，实现方式也差不多，都非常简单：
```
var book = new ht.Node();
book.setName('CSS3: The Missing Manual');
book.s3(60, 80, 8);
book.p3(-100, 210, 20);
book.r3(-Math.PI/6, Math.PI/5, 0);
book.setIcon('book');
book.s({
    'front.image': 'book',
    'back.color': 'white',
    'left.color': 'white',
    'all.color': 'gray'
});
book.setHost(shelf);
book.setParent(shelf);
dm.add(book);
```
接着左边的书架也是类似的构建方法，有一点不同的是，这边有一个 ht.CSGBox 类型，继承于 ht.CSGNode，其除具备父类 CSGNode 的挖空等功能外，还可对六个面进行旋转展开关闭的操作，这里我们的节点只设置了前面的能够旋转展开，并且设置了一系列的样式：
```
clipNode = new ht.CSGBox();
clipNode.setName('CSGBox-Expand-Left');
clipNode.s3(100, 100, 120);
clipNode.p3(0, 65, 0.1);
clipNode.setHost(shelf);
clipNode.showMe = true;
clipNode.s({
    'all.visible': false,//6面均不可见
    'front.visible': true,//前面可见
    'front.toggleable': true,//允许前面双击展开                    
    'front.reverse.flip': true,//前面的反面显示正面的内容
    'front.transparent': true,//前面透明
    'front.end': Math.PI * 0.7,//前面展开状态的结束旋转弧度
    'front.color': 'rgba(0, 50, 50, 0.7)'//前面颜色
});
```
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171108221508856-1272494164.png)
可能你们还想知道下面的地球是怎么做到的？还记得之前的文章写到过 HT 中设置了 shape3d 属性，设置这个属性实际上就是在操作 setShape3dModel(name, model) 和 getShape3dModel(name)，可以通过这个属性设置为 box|sphere|cylinder|cone|torus|star|rect|roundRect|triangle|rightTriangle|parallelogram|trapezoid 等等模型，这些模型也都是 HT 封装好的，要使用时直接设置 shape3d 为其中的一个值即可，如这个例子中用到 “shape3d: sphere” 就是设置为球体。我们简单地用一张地图图片包裹在这个球体的外侧，当然，这张地图图片是先通过 ht.Default.setImage 注册过的，然后通过 shape3d.image 将图片附到这个节点上：
```
earth = new ht.Node();
earth.setName('earth');
earth.s3(70, 70, 70);
earth.p3(0, 50, 0);
earth.s({
    'shape3d': 'sphere',
    'shape3d.image': 'earth'
});
earth.setHost(shelf);  
earth.setParent(shelf);
dm.add(earth);
```
右边的书架，同样也是有一个主节点，其他节点依附于它，但是我们看到这边换了一个新的节点类型 ht.DoorWindow，ht.DoorWindow继承于 ht.CSGNode，其除具备父类 CSGNode 的挖空等功能外，还可进行整体的旋转展开关闭的操作， 常用于作为门或窗的业务对象，吸附于 CSGNode 或 CSGShape 的 host 作为墙面的图元。这个节点类型就是 ht.CSGNode 的延展，相对来说就是区分了实际应用而添加了不同的 style 参数，更多的属性请到 [HT for Web 建模手册 DoorWindow 章节](http://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html#ref_doorwindow) 查看然后添加到节点中玩玩：
```
photos = new ht.DoorWindow();
photos.setName('DoorWindow-Photos');
photos.setIcon('ben12');
photos.s3(110, 100, 130);
photos.p3(5, 180, 0);                
photos.setHost(shelf);  
photos.showMe = true;
photos.s({                    
    'bottom.uv': [1,1, 1,0, 0,0, 0,1],
    'bottom.uv.scale': [1, 1],
    'left.uv.scale': [3, 3],
    'top.uv.scale': [2, 2],
    'dw.s3': [0.8, 0.9, 0.05],
    'dw.t3': [0, -5, 0],
    'dw.axis': 'v',
    'dw.toggleable': false,
    'front.image': 'ben1',
    'back.image': 'ben2',
    'all.color': '#F8CE8B'
});
photos.setParent(shelf);
dm.add(photos);
```
最后，我们将左侧的地球 earth 和右侧的照片 photo 旋转起来：
```
var angle = 0;
setInterval(function(){
    angle += Math.PI/40;
    earth.r3(0, angle, 0);
    photos.s('dw.angle', angle);
}, 50);
```
我们看到，其实虽然 HT 封装了很多不同的 CSG 节点类型，但是实际应用都差不多，而且内容也没有差特别多，差别都是在 style 参数上，但是真的在实际开发中，这种区分就会很大程度上加快开发速度，毕竟名称一目了然，就知道要运用哪些 style 属性了。
