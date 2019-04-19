# 基于 HTML5 WebGL 的 3D 服务器与客户端的通信 - xhload3d - 博客园
这个例子的初衷是模拟服务器与客户端的通信，我把整个需求简化变成了今天的这个例子。3D 机房方面的模拟一般都是需要鹰眼来辅助的，这样找产品以及整个空间的概括会比较明确，在这个例子中我也加了，这篇文章就算是我对这次项目的一个总结吧。对了，这个例子我是参考一个 DEMO 来完成的，和示例中的显示有些不同，大家莫惊奇。
例子链接： [http://www.hightopo.com/demo/3DEdgeFlow/index.html](http://www.hightopo.com/demo/3DEdgeFlow/index.html)
本文动图：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180126153649709-1984823502.gif)
这个例子没有设计师的参与，样式上面大家将就将就，但是我觉得还不错啊，哈哈~
进入正题，整个例子差不多用了 200 行的代码来实现的，这就是我喜欢用 [HT](http://www.hightopo.com) 的原因，现在 Web3d 技术兴起，大体就是分为两派：插件派和 HTML5 派。HT 就是基于 HTML5 的，不需要安装任何插件，啊，跑题了。。。
首先，还是从场景的搭建开始，这个界面是在 body 体上添加了三个部分：3d 组件，属性组件以及拓扑组件（2d 组件）。添加的方式是这样的：为了最外层组件加载填充满窗口的方便性，HT 的所有组件都有 addToDOM 函数，其实现逻辑如下，其中 iv 是 invalidate 的简写：
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
因为这个函数是将 style 中的位置都固定了，所以不能将所有的组件都用这个函数，我们按照这个函数的方式将拓扑组件和属性组件添加进界面中，3d 组件直接利用 addToDOM 函数即可：
```
dm = new ht.DataModel();
g3d = new ht.graph3d.Graph3dView(dm);//3d 组件
g3d.addToDOM();//将组件添加进 body 体中
g3d.setDashDisabled(false);//开启虚线流动
g3d.setMovableFunc(function(){//重载移动函数
    return false;//返回false，所有的图元都不可移动
});
g3d.setEye([-813, 718, 1530]);//设置 eye 
g3d.setCenter([140, -25, 217]);//设置 center（target）
gv = new ht.graph.GraphView(dm);//2d 组件
gv.getView().className = 'graphview';//HT 组件根层都是一个 div，通过 getView() 函数获取
document.body.appendChild(gv.getView());//将拓扑组件添加进 body 体中
gv.fitContent(true);//缩放平移整个拓扑以展示所有的图元
propertyView = new ht.widget.PropertyView(dm);//属性组件   
propertyView.getView().className = 'property';
propertyView.setWidth(240);//设置组件宽度
propertyView.setHeight(150);//高度
document.body.appendChild(propertyView.getView());
```
拓扑组件和属性组件的样式我就不再赘述了，只是设置了一个背景颜色以及 left right top bottom 位置而已。这里要声明一下，HT 组件一般都以设置 position 为 absolute 的绝对定位方式。
大家可能会好奇，这个鹰眼怎么生成的？在 HT 中，只要 2D 和 3D 共用同一个数据容器 dataModel 即可共同拥有所有在这个 dataModel 中的元素，并且位置都是对应的，只需要类似这种做法即可：
```
dm = new ht.DataModel();
g3d = new ht.graph3d.Graph3dView(dm);
gv = new ht.graph.GraphView(dm);
```
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180126153740490-1841932218.png)
是不是非常简单。。。可以省去大把时间开发。。。
这个例子中除了连线之外的所有元素都是 ht.Node 类型的节点，所以我们将这个节点的创建方法封装一下，好重复利用：
```
function createNode(p3, s3, name, shape){//创建节点
    var node = new ht.Node();//创建类型为 ht.Node 类的节点
    dm.add(node);//将节点添加进数据容器 dataModel 中
    node.s({//设置节点的样式，s 为 setStyle 的简写
        'shape3d': shape,//指定节点的形体，这边是传入 3d 模型的 json 文件
        'label.position': 23,//文字显示位置
        'label.transparent': true,//文字在3d下是否透明 可消除字体周围的锯齿
        'label.color': '#eee',//文字颜色
        'label.t3': [0, 0, -151],//文字在3d下的偏移
        'label.r3': [0, Math.PI, 0],//文字在3d下的旋转
        'label.scale': 2//文字缩放
    });
    node.r3(0, Math.PI, 0);//节点旋转
    node.p3(p3);//设置节点在 3d 下的位置
    node.s3(s3);//设置节点在 3d 下的大小
    node.setName(name);//设置节点的显示名称
    return node;//返回节点
}
```
以及连线的创建：
```
function createEdge(exchange, service){//创建连线
    var edge = new ht.Edge(exchange, service);
    dm.add(edge);
    edge.s({
        'edge.width': 4,//连线宽度
        'edge.color': 'red',//连线颜色
        'edge.dash': true,//是否显示虚线
        'edge.dash.color': 'yellow',//虚线颜色
        'edge.dash.pattern': [32, 32],//连线虚线样式默认为[16, 16]
    });
    edge.a({//用户自定义属性 为 setAttr 的缩写
        'flow.enable': true,//是否启用流动
        'flow.direction': 1,//方向
        'flow.step': 4//步进
    });
    return edge;
}
```
我们界面中的显示的连线都是虚线流动的，HT 默认是关闭虚线流动的功能的，通过下面这句来开启虚线流动的功能：
```
g3d.setDashDisabled(false);//开启虚线流动
```
同时我们还需要设置动画，控制时间间隔，使得连线虚线偏移，形成一种“流动”的状态，动画请参考 [schedule 调度手册](http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-guide.html)：
```
```
```
flowTask = {
    interval: 40,
    action: function(data){
        if(data.a('flow.enable')){
            data.s('edge.dash.offset', data.s('edge.dash.offset')+(data.a('flow.step')*data.a('flow.direction')));
        }
    }
};
dm.addScheduleTask(flowTask);//添加flowTask动画
```
以下是界面上出现的所有的服务器以及客户端的节点的声明，都是基于 createNode 和 createEdge 函数创建的：
```
floor = createNode([0, 5, 0], [1000, 10, 500]);//地板图元
floor.s({//设置图元的样式 s 为 setStyle 的缩写
    'all.color': 'rgb(47, 79, 79)'//六面体的整体颜色
});
exchange = createNode([0, 300, -400], [200, 20, 150], 'H3C 核心交换机', 'models/机房/机柜相关/机柜设备6.json');//交换机
//五台不同作用的服务器
service1 = createNode([-400, 140, 0], [100, 260, 100], '备用', 'models/机房/机柜相关/机柜2.json');
service2 = createNode([-200, 140, 0], [100, 260, 100], '网站', 'models/机房/机柜相关/机柜2.json');
service3 = createNode([0, 140, 0], [100, 260, 100], 'OA', 'models/机房/机柜相关/机柜2.json');
service4 = createNode([200, 140, 0], [100, 260, 100], '广告', 'models/机房/机柜相关/机柜2.json');
service5 = createNode([400, 140, 0], [100, 260, 100], '受理', 'models/机房/机柜相关/机柜2.json');
//创建交换机与服务器之间的连线
createEdge(exchange, service1);
createEdge(exchange, service2);
createEdge(exchange, service3);
createEdge(exchange, service4);
createEdge(exchange, service5);
//第二台交换机
exchange2 = createNode([-100, 60, 400], [200, 20, 150], 'Procurve Switch 2010-23 交换机', 'models/机房/机柜相关/机柜设备6.json').s('label.color', '#000');
createEdge(exchange2, service1);
createEdge(exchange2, service2);
createEdge(exchange2, service3);
createEdge(exchange2, service4);
createEdge(exchange2, service5);
floor2 = createNode([-100, 5, 800], [1000, 10, 500]);
floor2.s({
    'all.color': 'rgb(47, 79, 79)'
});
device1 = createNode([-400, 20, 650], [200, 20, 100], 'VLS 12000(上)', 'models/机房/机柜相关/机柜设备6.json');
device2 = createNode([100, 20, 650], [200, 20, 100], 'VLS 12000(下)', 'models/机房/机柜相关/机柜设备6.json');
device3 = createNode([-200, 20, 800], [200, 20, 100], 'HP Strage Works 8/8 SAN Switch(上)', 'models/机房/机柜相关/机柜设备6.json');
device4 = createNode([200, 20, 800], [200, 20, 100], 'HP Strage Works 8/8 SAN Switch(下)', 'models/机房/机柜相关/机柜设备6.json');
device5 = createNode([-300, 20, 950], [200, 20, 100], 'EVA 8400 HSV450(上)', 'models/机房/机柜相关/机柜设备6.json');
device6 = createNode([100, 20, 950], [200, 20, 100], 'EVA 8400 HSV450(下)', 'models/机房/机柜相关/机柜设备6.json');
createEdge(exchange2, device1);
edge1 = createEdge(exchange2, device2);
createEdge(device1, device3);
createEdge(device1, device4);
createEdge(device2, device3);
createEdge(device2, device4);
createEdge(device3, device5);
createEdge(device3, device6);
createEdge(device4, device5);
createEdge(device4, device6);
dm.sm().ss(edge1);//设置选中 edge1
```
最后，需要在属性栏中添加属性，这里我们只对“连线”进行了属性的显示及调整，总共 5 个属性，包括我通过 setAttr（简写为 a）自定义的属性 flow.enable、flow.direction、flow.step 和样式属性 edge.color 以及 edge.dash.color。我们通过 name 属性结合 accessType 属性实现对 Data 节点的存取：
![](https://images2017.cnblogs.com/blog/591709/201712/591709-20171227104449323-865352197.png)
```
//连线的属性
edgeProperties = [
    {
        name: 'flow.enable',//用于存取 name 属性
        accessType: 'attr',//操作存取属性类型
        displayName: 'Enable Flow',//用于存取属性名的显示文本值，若为空则显示 name 属性值
        valueType: 'boolean',//用于提示组件提供合适的 renderer 渲染，boolean 类型，显示为勾选框
        editable: true//设置该属性是否可编辑
    },
    {
        name: 'flow.direction',
        accessType: 'attr',
        displayName: 'Flow Direction',
        enum: {//枚举类型属性 传递数值和文字数组
            values: [-1, 1],
            labels: ['正向流动', '反向流动']
        },
        editable: true
    },
    {
        name: 'flow.step',
        displayName: 'Flow Step',                        
        editable: true,
        accessType: 'attr',
        slider: {//表单插件中的滑动条 所以要添加 ht-form.js 
            min: 0,//最小值
            max: 10,//最大值
            step: 0.1//步进
        }
    },
    {
        name: 'edge.color',
        accessType: 'style',
        displayName: 'Edge Color',
        editable: true,
        valueType: 'color',//颜色类型，以填充背景色的方式显示
        colorPicker: {//颜色选择框
            instant: true//获取和设置是否处于即时状态，默认为true，代表作为表格和属性页的编辑器时，将实时改变模型值
        }
    },
    {
        name: 'edge.dash.color',
        displayName: 'Dash Color',
        accessType: 'style',
        valueType: 'color',
        editable: true,
        colorPicker: {
            instant: true
        }
    }
];
propertyView.setEditable(true);//设置属性组件可编辑
propertyView.addProperties(edgeProperties);//添加连线属性
```
是不是非常简单~  快动手实践一下吧！
