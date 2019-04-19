# 基于 WebGL 3D 的 HTML5 档案馆可视化管理系统 - xhload3d - 博客园
## 前言
档案管理系统是通过建立统一的标准以规范整个文件管理，包括规范各业务系统的文件管理的完整的档案资源信息共享服务平台，主要实现档案流水化采集功能。为企事业单位的档案现代化管理，提供完整的解决方案，档案管理系统既可以自成系统，为用户提供完整的档案管理和网络查询功能，也可以与本单位的OA办公自动化和DPM设计过程管理，或者与MIS信息管理系统相结合，形成更加完善的现代化信息管理网络。传统档案馆随着社会的快速发展与变化，其内在形式上也发生了巨大变化，逐渐演变为现代智慧档案馆。智慧档案馆以现代科技为依托，充分结合现代物联网技术与云计算技术构建完善的城市智慧档案，实现了现代社会全面管理的目标。本文以当前流行的 H5 技术为主，为现代智慧档案馆提供一套 WEB 解决方案。
![](https://images2018.cnblogs.com/blog/591709/201809/591709-20180902233827618-275027955.gif)
## 代码实现
### 场景搭建
在本例中，将使用 HT UI 组件对页面实现布局；使用 RelativeLayout 相对布局器将页面分为三个部分：left, top, center，使用 VBoxLayout 纵向布局器将 LEFT 部分分为 logo，editor，chart 三个部分
![](https://images2018.cnblogs.com/blog/591709/201809/591709-20180902232943891-846950229.png)
### Graph3dView 加载 3D 场景
Graph3dView 是 HT（[www.hightopo.com](http://www.hightopo.com/)） 组件中加载 3D 模型的拓扑组件，RelativeLayout 则是 HT 提供的 UI 解决方案，UI 组件中提供 HTView 组件来实现拓扑与 UI 的融合。
```
// 初始化相对布局器
    var relativeLayout = new ht.ui.RelativeLayout();
    // 初始化 3D 拓扑
    var g3dView = new ht.graph3d.Graph3dView();
    // 初始化 HTVIEW 组件， 并将 3D 拓扑放入其中
    var htView = new ht.ui.HTView(g3dView);
    // 布局器加载 HTVIEW 组件
    relativeLayout.addView(htView, {
        width: 'match_parent',  // 填满
        height: 'match_parent', // 填满
        marginTop: 64,            // 为 TOP 留下空间
        marginLeft: 250            // 为 LEFT 留下空间
　　});
```
### *创建 LEFT 中的档案袋模型*
*左侧的 EDITOR 部分使用 HT 的调色板组件（ht.widget.Palette）, 将档案袋添加到调色板上，并设置为可以拖拽：*
```
var palette = new ht.widget.Palette();
    // palette 面板是将图元都分在“组”里面，然后向“组”中添加图元即可
    var group = new ht.Group();
    // 设置分组为打开的状态
    group.setExpanded(true);
    // 设置组的说明
    group.setName('基础图元');
    palette.dm().add(group);
    
    // 添加子图元
    var childNode = new ht.Node();
    childNode.setParent(group);
    childNode.setName(name);
    childNode.setImage(image);
    childNode.s({
        'draggable': true,                     // true 为可拖拽 
        'image.stretch': 'centerUniform'    // 图片申展方式
    });
    palette.dm().add(childNode);
```
### *实现从调色板中将图元拖拽至 3D 场景*
*在上一步中我们对调色板中的图元属性设置了可拖拽，此时可以实现拖拽图元的动画。但是并不能直接将图元拖拽到 3D 场景中，实现思路是：1.  在拖拽时获取拖拽的图元信息2.  拖拽到对应位置时显示可摆放位置3.  结束拖拽后在对应位置创建对应的 3D 模型 对应代码实现如下：拖拽时获取图元信息*
```
g3dView.getView().addEventListener('dragover', function(e) {
        e.preventDefault();
        var paletteNode = palette.dm().sm().ld();// 获取 palette 上最后选中的节点 
        if (!paletteNode || !g3d.getDataAt(e)) return;
        
        // 获取鼠标下的节点
        var data = g3d.getDataAt(e);
        if (data.s('shape3d') === '档案柜.json') {
            // 记录文件袋拖拽到的是哪个档案柜
            g3dView._focusData = data;
        }
    });
```
*拖拽到对应位置时创建 3D 模型，在实际实现过程中由于很难准确地获取到档案柜中每一个可以摆放档案袋的坐标位置，所以在本例中采用了预置的方法。具体原理就是在先创建一个正常不可见的档案柜模型，并在其中将档案袋都摆放完整，在拖拽时，将此不可见的模型与将要摆放的模型重合，此时只需判断鼠标所在的点下是否存在预置的模型存在就可以知道该处是否可以创建 3D 模型，实现效果如下：*
*![](https://images2018.cnblogs.com/blog/591709/201809/591709-20180902233428332-121165454.gif)*
```
g3dView.getView().addEventListener('dragover', function(e) { 
        ... // 旧逻辑省略
        
        // 拖拽下来的时候设置 所有的 displayName 为 box 的节点 为可见 （这样拖拽才能获取到预置模型）
        array.forEach(function(data) {
            data.s('3d.visible', true);
        });
        var data = g3d.getDataAt(e);
        if (data.s('shape3d') === '档案柜.json') {
            // 记录文件袋拖拽到的是哪个档案柜
            g3dView._focusData = data;
            // 将预置模型移动到拖拽的柜子坐标
            shelf.p3(data.p3());
        }
        if(data.getDisplayName() === 'box') {
            // 将对应坐标下预置的档案袋模型进行显示
            // 该属性可修改模型的透明度，更多属性可参考 HT 风格手册
            data.s('shape3d.opacity',  0.8);
        }
        ...
    })
    
    g3dView.getView().addEventListener('drop', function(e) {
        // 获取鼠标位置模型    
        var data = g3dView.getDataAt(e);
        if(!data) return;
        // 鼠标位置不是预置模型，直接跳过
        if(data.getDisplayName() !== 'box') return;
        data.s('shape3d.opacity', 0);
        // 放手时候设置 所有的 displayName 为 box 的节点 不可见
        array.forEach(function(data) {
            data.s('3d.visible', false);
        });
        var node = new ht.Node();
        node.s('shape3d', url); // 档案袋 3D 模型地址
        node.r3([Math.PI/2, -Math.PI/2, 0]); // 旋转档案袋模型， 使其平放
        node.p3(data.p3());
        node.setParent(g3dView._focusData);
        node.setHost(g3dView._focusData);
    });
```
### *档案柜虚化效果实现*
*上面我们已经实现了档案袋拖拽至 3D 场景的效果，但是我们可以发现档案袋模型远小于柜子，要将档案袋摆放到正确的位置并不是那么容易。所以此时我们可以将需要操作的档案柜放大到正中间，其它模型进行虚化处理。![](https://images2018.cnblogs.com/blog/591709/201809/591709-20180902233522496-557558538.gif)*
```
// 3D 拓扑交互监听
    g3dView.mi(function(e){
        if(e.kind === 'doubleClickData') {
            // 双击事件
            var shape3d = e.data.s('shape3d'),
                parentShape3d = e.data.getParent() && e.data.getParent().s('shape3d');
            if (shape3d && shape3d.indexOf('档案柜') > -1) {
                // 重点突出档案柜
                showDetail(e.data);
            }
            else if (parentShape3d && parentShape3d.indexOf('档案柜') > -1) {
                showDetail(e.data.getParent());
            }
        }
    });
    showDetail = function(data) {
        // 保存进入虚化状态前 视角 与 中心点
        eyeBack = ht.Default.clone(graph3dView.getEye());
        centerBack = ht.Default.clone(graph3dView.getCenter());
    
        // 设置相机指向配置
        var opt = {};
        opt.animation = true;
        opt.ratio = 1;
        opt.direction = [1, 0.5, 0];
        opt.center = [data.getX(), 100, data.getY()];
        graph3dView.flyTo(data, opt);
        focusData = data;
        data.s('select.brightness', 1);
        dataModel.each(function (d) {
            if (d === focusData || (!d.s('3d.selectable') && d.getTag() !== 'wall')
                || d.getParent() === focusData || d.getDisplayName() === 'box') return;
            // 将拓扑中除了要操作的柜子 与柜子中档案袋 以及墙外 透明度都设置为 opacity (0~1)
            
            // 保存设置前配置， 还原用
            if (!opacityMap[d.getId()]) {
                opacityMap[d.getId()] = {
                    'shape3d.opacity': d.s('shape3d.opacity'),
                    'shape3d.transparent': d.s('shape3d.transparent'),
                    'all.opacity': d.s('all.opacity'),
                    'all.transparent': d.s('all.transparent'),
                    'left.opacity': d.s('left.opacity'),
                    'left.transparent': d.s('left.transparent'),
                    'right.opacity': d.s('right.opacity'),
                    'right.transparent': d.s('right.transparent'),
                    'front.opacity': d.s('front.opacity'),
                    'front.transparent': d.s('front.transparent'),
                    'back.opacity': d.s('back.opacity'),
                    'back.transparent': d.s('back.transparent'),
                    'top.opacity': d.s('top.opacity'),
                    'top.transparent': d.s('top.transparent'),
                    'bottom.opacity': d.s('bottom.opacity'),
                    'bottom.transparent': d.s('bottom.transparent'),
                    '3d.selectable': d.s('3d.selectable')
                }
            }
            
            // 透明度设置为 opacity
            d.s({
                'shape3d.opacity': opacity,
                'shape3d.transparent': true,
                'all.opacity': opacity,
                'all.transparent': true,
                'left.opacity': opacity,
                'left.transparent': true,
                'right.opacity': opacity,
                'right.transparent': true,
                'front.opacity': opacity,
                'front.transparent': true,
                'back.opacity': opacity,
                'back.transparent': true,
                'top.opacity': opacity,
                'top.transparent': true,
                'bottom.opacity': opacity,
                'bottom.transparent': true,
                '3d.selectable': false
            });
        });
    }
```
*退出虚化模式，以监控 3D 拓扑的选中变化来实现*
```
g3dView.dm().ms(function(e) {
        var lastData = g3dView.sm().ld();
        // 判断是否进行虚化
        if(focusData) {
            if(lastData === focusData || (lastData && lastData.getParetn() === focusData)) return;
            g3dView.setEye(eyeBack);
            g3dView.setCenter(centerBack);
            // 还原模型的原透明度
            g3dView.dm().each(function (d) {
                if (d === focusData) return;
                d.s(opacityMap[d.getId()]);
            });
            
            focusData.s('select.brightness', 0.7);
            focusData = null;
            eyeBack = null;
            centerBack = null;
        }
    });
```
### *快速查询功能实现*
*在 HT 的组件中有提供快速查询插件 QuickFinder ，此次我们就运用该插件来实现简单的档案编号查询*
```
// 初始化 输入框
    var textField = new ht.ui.TextField;
    textField.setIcon("imgs/search.json");
    textField.setIconPosition("left");
    // 初始化查询器，条件：id
    var finder = new ht.QuickFinder(library.view.dm, "id");
    // 输入框点击查询按钮时触发
    textField.on('p:value', function(e) {
        var dm = library.view.dm;
        var value = e.newValue;
        var datas = finder.find(value);
        // 查询到对应的图元时，我们将第一个结果进行选中
        if (datas && datas.size() > 0) {
            library.view.dm.sm().ss(datas.get(0));
        }
    });
```
![](https://images2018.cnblogs.com/blog/591709/201809/591709-20180902233714147-838769437.gif)
## *总结*
*经过以上功能的实现，一个基础的智慧档案管理系统就成形了。当然做为一个智慧管理系统，这些还是远远不够的，例如档案动态监控、档案室内人员走动监控、视频监控、温度监控、灾害报警等等模块都是后期可以完善的地方。这里只是简单地为大家提供了一个基于 HTML5 WEBGL 的 3D 解决方案。同样原理，智能楼宇、智能机房、智能城市也可以基于此来实现。*
*最终效果可查看：[https://hightopo.com/demo/intelligent-archives/](https://hightopo.com/demo/intelligent-archives/)![](https://images2018.cnblogs.com/blog/591709/201809/591709-20180902233754447-727236652.gif)*
