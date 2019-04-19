# 基于 HTML5 的 WebGL 3D 智能楼宇监控系统 - xhload3d - 博客园
# 前言
智能监控的领域已经涉及到了各大领域，工控、电信、电力、轨道交通、航天航空等等，为了减少人员的消耗，监控系统必不可少。之前我写过一篇 2D 的智能地铁监控系统广受好评，突然觉得，既然 2D 的这么受欢迎，那么 3D 的需求量肯定也是非常大的，3D 毕竟比 2D 来说还是更直观一些，于是有了这个例子以及这篇文章。智能监控系统在 3D 中应用比较广泛的除了 3D 机房以外，我觉得就是楼宇的监控了，可是之前做了很多关于机房方面的 Demo，所以最终决定做 3D 楼宇监控系统。
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180422231501145-1263107851.gif)
# 代码生成
## 场景搭建
整个场景是由 3D 组件搭建而成，配合左侧的 listView 列表组件，通过点击这个 listView 列表组件中的各个项可以自由切换各个监控楼层和楼宇的场景：
```
dm = new ht.DataModel();
g3d = new ht.graph3d.Graph3dView(dm);
relativeLayout = new ht.ui.RelativeLayout();// 相对布局器 可对界面进行布局
var ht3dView = new ht.ui.HTView(g3d);// 放置 3d 组件
relativeLayout.addView(ht3dView, {// 给相对布局器添加组件显示，参数一为组件名称，参数二可设置宽高、对齐方式等属性
    width: 'match_parent',
    height: 'match_parent'
});
var listView = window.list = new ht.ui.ListView();// 列表组件
for (var i = 1; i <= 15; i++) {
    var data = new ht.Data();// 创建节点
    data.setName('楼层' + i);// 设置节点名称
    listView.dm().add(data);// 将节点添加进列表组件中
}
relativeLayout.addView(listView, {// 将 listView 组件添加进布局器中
    align: 'left',// 设置对齐方式为左对齐
    vAlign: 'top',// 设置垂直对齐方式为顶部对齐
    marginTop: 120,// 设置外边距顶部为 120 像素
    marginLeft: 60,// 设置外边距左侧为 60 像素
    width: 80,// 设置宽度为 80
    height: 480,// 设置高度为 480
    index: 100// 设置元素的堆叠顺序
});
relativeLayout.addToDOM();// 将组件添加进 body 中
```
## 加载模型
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180422231701011-1281847080.png)
进入页面显示的就是整个城市的场景，通过 ht.Default.loadObj 方法加载 obj 模型：
```
var loadCity = function(){
    ht.Default.loadObj('obj/city.obj', 'obj/city.mtl', {// 加载模型
        center: true,// 模型是否居中，默认为false，设置为true则会移动模型位置使其内容居中
        cube: true,// 是否将模型缩放到单位1的尺寸范围内，默认为false
        prefix: 'obj/',// 图片路径前缀，即在map_kd值之前增加的前缀，如果是相对路径则以加载obj的html页面的路径为参考
        shape3d: 'city',// 如果指定了shape3d名称，则HT将自动将加载解析后的所有材质模型构建成数组的方式，以该名称进行注册
        finishFunc: function(modelMap, array, rawS3){// 用于加载 obj 模型后的回调处理
            city.rawS3 = rawS3;// 设置变量 city 对象的 rawS3 属性 此函数中的 rawS3 属性为 obj 模型的原始大小
            showCity();// 创建一个节点 设置节点的 shape3d 为 city 显示 city.obj 与 city.mtl 的内容
        }
    });
}
```
工控楼层模型的加载也是类似，这里就不再赘述。
直接将组件添加进场景中是不会有相关的操作的， 必须要监听事件的触发才可进行后续的操作，这里对数据选中容器中的选中变化事件进行监听：
```
//列表点击
listView.dm().sm().ms(function(e){// 监听选中变化事件
    if (e.kind === 'set') {// 设置监听事件
        showFloor();// 显示楼层
    }
});
```
## 调用模型
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180422231750304-1596292396.png)
这里为了简单就只设置了一个工控楼层的 obj 模型，通过调用不同的 obj 模型可以显示不同的工控楼层场景，也就是说我们可以自己通过 ht.Default.loadObj 方法加载模型，设置工控楼层模型的 shape3d 属性，然后设置到节点的 shape3d 属性上，即可修改；或者直接设置节点的 shape3d 属性为 json 格式的 obj 文件，这里还是采取第一种方式：
```
var showFloor = function(){
    g3d.setCenter([210, 0, 210]);// 设置 3d 组件的“中心”位置
    dm.clear();// 清除数据容器中的所有节点
    var rawS3 = floor.rawS3,// 获取 obj 模型的原始大小
        node = new ht.Node();// 创建一个新的节点
    node.s({// 设置节点的样式属性
        'shape3d': 'floor',// 此项设置的值为 ht.Default.loadObj 中设置的 shape3d 属性的值
        'wf.visible': 'selected',// 设置选中节点时显示节点外部的线框
        '3d.selectable': false// 设置节点不可选中
    });
    node.s3(rawS3[0] / 10,rawS3[1]/ 10,rawS3[2] / 10);// 设置节点的大小为原始大小的十分之一
    node.p3(140, 0, 230);// 设置节点的位置
    dm.add(node);// 将节点添加进数据容器中
    // 添加四个“相机”的节点
    createNode([0, 20, 0]);
    createNode([110, 20, 220]);
    createNode([330, 20, 420]);
    createNode([210, 20, 120]);
    createNode([420, 20, 120]);
};
```
这里顺便说一下另一种简便的调用 obj 模型的方式，直接设置节点的 shape3d 属性为导入的 json 格式的文件：
```
var node = new ht.Node();
node.s("shape3d", "./symbols/city.json");
```
但是这个 json 的内容必须要有以下几个元素：
```
{
    "modelType": "obj",// 必须设置此属性为 obj 格式
    "obj": "./obj/city.obj",// 必须设置 obj 属性
    "mtl": "./obj/city.mtl"// 此项可写可不写，如果需要设置 obj 模型的样式（如颜色等），则必须设置此项
}
```
但是这种模式不适用于这个场景，因为我的模型有些大，需要调用到 obj 模型的原始大小 rawS3 属性除以一定比例后再显示。
## 创建面片
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180422231826803-921679427.png)
上面提到了 createNode 方法，这个方法主要是用来创建显示为“面片”类型的节点。所谓“面片”，即为只有一个面。通过这种在一个面上显示一张矢量图的方式，结果会比在一个六面体上显示一张图的性能好，3D 场景简单的时候可能看不出来效果，如果场景卡顿，立马就能看出“面片”的优势了：
```
function createNode(p3, s3){
    var node = new ht.Node();// 创建一个新的节点
    node.p3(p3);// 设置节点的位置
    // node.s3(s3);
    node.s({
        'shape3d': 'billboard',// 设置节点的 shape3d 类型为 billboard 类型，显示为“面片”
        'shape3d.image': './symbols/智能楼宇/camera.json',// 3d图形整体贴图
        'shape3d.image.cache' : true,// 如果贴图是矢量，是否缓存（缓存后性能会得到提升）
        'shape3d.autorotate': true,// 是否自动朝向相机
        'shape3d.transparent': true,// 决定3d图形是否透明
         // 'shape3d.alwaysOnTop': true,// 是否总是在最前
        'shape3d.fixSizeOnScreen': [ 38, 47 ]// 是否无论缩放远近，在屏幕内呈现固定大小，值可为true（使用图片或矢量自身大小）/false, 也可以是[100, 200]（对应宽高）
    });
    dm.add(node);// 将节点添加进数据容器中
    g3d.invalidateShape3dCachedImage(node);// cache 的代价是，这里需要更新
    return node;
}
```
## 事件交互
然后我就在想，既然到了楼层的 3D 模型显示，那么怎么返回？以哪种方式返回最好？想来想去比较没有违和感的还是点击列表组件比较好，就选中了列表组件的顶部：
```
listView.getView().addEventListener('click', function(e){// 监听点击事件
    e.preventDefault();// 阻止默认操作
    if (e.clientY - 120 < 50) {
        showCity();// 显示初始 3D 楼宇场景
        listView.dm().sm().cs();// 列表设置清除所有选中元素
    }
});
```
所有代码结束！
# 总结
这个 3D 智能楼宇监控系统非常的简单，对于技术人员来说是完全没有挑战性的，主要工作内容在美工上，这么一来，如果要添加比较复杂的需求，技术人员就可以全身心地投入到产品上，而不是一些繁琐的 3D 模型的搭建了。总而言之，我觉得这个 Demo 非常具有代表性，所以想拿出来跟大家分享一下，一起讨论一下前端的趋势所在。
[http://hightopo.com/demo/intelligent-building/index.html](http://hightopo.com/demo/intelligent-building/index.html)
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180422231913634-1798575005.gif)
