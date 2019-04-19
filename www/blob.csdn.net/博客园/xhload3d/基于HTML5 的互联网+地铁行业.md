# 基于HTML5 的互联网+地铁行业 - xhload3d - 博客园
# 前言
近几年，互联网与交通运输的融合，改变了交易模式，影响着运输组织和经营方式，改变了运输主体的市场结构、模糊了运营与非营运的界限，也更好的实现了交通资源的集约共享，同时使得更多依靠外力和企业推动交通信息化成为可能。互联网交通具有巨大的潜力、发展迅速、生态浩大，需要政府顺势而为，紧跟发展态势，加强引导、转变职能、创新政府管理与市场监管的模式。而城市轨道交通的投资体量巨大，一条线路动辄上百亿元，如何保证投资效益、提升投资管理工作的精确性和可控性是当务之急。应引入“互联网+”思维，研发基于“互联网+”的城市轨道交通系统。在分析现有投资管理存在问题基础上，这里对该系统的应用特点及主要功能进行了深入阐述，旨在完善提高系统的功能分布和用户体验。
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171119110705265-1516373653.gif)
[http://www.hightopo.com/demo/metro/demo2.html](http://www.hightopo.com/demo/metro/demo2.html)
# 代码实现
## 场景搭建
我们先来搭建基础场景，因为这个场景是在 2D 的基础上实现的，因此我们需要用拓扑组件 ht.graph.GraphView 来搭建基础场景：
```
dataModel = new ht.DataModel();// 数据容器，用来存储数据节点
graphView = new ht.graph.GraphView(dataModel);// 拓扑组件
graphView.addToDOM();// 将拓扑组件添加到 body 体中
```
上面代码中出现的 addToDOM 方法将组建添加到 DOM 的 body 中，HT（[https://hightopo.com](https://hightopo.com/)）的组件一般都会嵌入 BorderPane、SplitView 和 TabView 等（均为 HT 的组件）容器中使用，而最外层的 HT 组件则需要用户手工将 getView() 返回的底层 div 元素添加到页面的 DOM 元素中，这里需要注意的是，当父容器大小变化时，如果父容器是 BorderPane 和 SplitView 等这些 HT 预定义的容器组件，则 HT 的容器会自动递归调用孩子组件 invalidate 函数通知更新。但如果父容器是原生的 html 元素， 则 HT 组件无法获知需要更新，因此最外层的 HT 组件一般需要监听 window 的窗口大小变化事件，调用最外层组件 invalidate 函数进行更新。
为了最外层组件加载填充满窗口的方便性，HT 的所有组件都有 addToDOM 函数，其实现逻辑如下，其中 iv 是 invalidate 的简写：
```
addToDOM = function(){
    var self = this,
        view = self.getView(),// 获取组件的底层 div
        style = view.style;
    document.body.appendChild(view);// 将组件底层div添加进body中
    style.left = '0';// ht 默认将所有的组件的position都设置为absolute绝对定位
    style.right = '0';
    style.top = '0';
    style.bottom = '0';
    window.addEventListener('resize', function () { self.iv(); }, false);// 窗口大小改变事件，调用刷新函数
}
```
## 场景导入
在 HT 中，常用的一种方法将场景导入到内部就是靠解析 JSON 文件，用 JSON 文件来搭建场景的好处之一就是可以循环利用，我们今天的场景就是利用 JSON 画出来的。接下来 HT 将利用  ht.Default.xhrLoad 函数载入 JSON 场景，并用 HT 封装的 DataModel.deserialize(json) 来反序列化（[http://hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html#ref_serialization](http://hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html#ref_serialization)），并将反序列化的对象加入 DataModel：
```
ht.Default.xhrLoad('demo2.json', function(text) {
    var json = ht.Default.parse(text);
    if(json.title) document.title = json.title;//将 JSON 文件中的 titile 赋给全局变量 titile 
    dataModel.deserialize(json);//反序列化
    graphView.fitContent(true);//缩放平移拓扑以展示所有图元，即让所有的元素都显示出来
});
```
在 HT 中，Data 类型对象构造时内部会自动被赋予一个 id 属性，可通过 data.getId() 和 data.setId(id) 获取和设置，Data 对象添加到 DataModel 之后不允许修改 id 值，可通过 dataModel.getDataById(id) 快速查找 Data 对象。但是一般建议 id 属性由 HT 自动分配，用户业务意义的唯一标示可存在 tag 属性上，通过 Data#setTag(tag) 函数允许任意动态改变 tag 值，通过DataModel#getDataByTag(tag) 可查找到对应的 Data 对象，并支持通过 DataModel#removeDataByTag(tag) 删除 Data 对象。我们这边通过在 JSON 中设置 Data 对象的 tag 属性，在代码中通过 dataModel.getDataByTag(tag) 函数来获取该 Data 对象：
```
{
    "c": "ht.Block",
    "i": 3849,
    "p": {
        "displayName": "通风1",
        "tag": "fan1",
        "position": {
            "x": 491.24174,
            "y": 320.88985
        },
        "width": 62,
        "height": 62
    }
}
```
```
var fan1 = dataModel.getDataByTag('fan1');
var fan2 = dataModel.getDataByTag('fan2');
var camera1 = dataModel.getDataByTag('camera1');
var camera2 = dataModel.getDataByTag('camera2');
var camera3 = dataModel.getDataByTag('camera3');
var redAlarm = dataModel.getDataByTag('redAlarm');
var yellowAlarm = dataModel.getDataByTag('yellowAlarm');
```
我在下图中做了各标签对应的元素：
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171119120928327-1733373857.png)
## 动画
接着我们对需要旋转、闪烁的对象进行设置，HT 中对“旋转”封装了 setRotation(rotation) 函数，通过获得对象当前的旋转角度，在这个角度的基础上再增加某个弧度，通过 setInterval 定时调用，这样就能在一定的时间间隔内旋转相同的弧度：
```
setInterval(function(){
    var time = new Date().getTime();
    var deltaTime = time - lastTime;
    var deltaRotation = deltaTime * Math.PI / 180 * 0.1;
    lastTime = time;
    fan1.setRotation(fan1.getRotation() + deltaRotation*3);
    fan2.setRotation(fan2.getRotation() + deltaRotation*3);
    camera1.setRotation(camera1.getRotation() + deltaRotation/3);
    camera2.setRotation(camera2.getRotation() + deltaRotation/3);
    camera3.setRotation(camera3.getRotation() + deltaRotation/3);
    if (time - stairTime > 500) {
        stairIndex--;
        if (stairIndex < 0) {
            stairIndex = 8;
        }
        stairTime = time;
    }
    for (var i = 0; i < 8; i++) {// 因为有一些相似的元素我们设置的 tag 名类似，只是在后面换成了1、2、3，所以我们通过 for 循环来获取
        var color = stairIndex === i ? '#F6A623' : '#CFCFCF';
        dataModel.getDataByTag('stair_1_' + i).s('shape.border.color', color);
        dataModel.getDataByTag('stair_2_' + i).s('shape.border.color', color);
    }
    if (new Date().getSeconds() % 2 === 1) {
        yellowAlarm.s('shape.background', null);
        redAlarm.s('shape.background', null);
    }
    else {
        yellowAlarm.s('shape.background', 'yellow');
        redAlarm.s('shape.background', 'red');
    }
}, 5);
```
 当然还是可以通过 HT 封装的动画进行操作，不过这里不一次性投食太多，有兴趣的可以看看我发布的 [https://www.cnblogs.com/xhload3d/p/9222549.html](https://www.cnblogs.com/xhload3d/p/9222549.html) 等其它文章。
HT 还封装了 setStyle 函数用来设置样式，可简写为 s，具体样式请参考 HT for Web 样式手册（[http://hightopo.com/guide/guide/core/theme/ht-theme-guide.html](http://hightopo.com/guide/guide/core/theme/ht-theme-guide.html)）：
```
for (var i = 0; i < 8; i++) {// 因为有一些相似的元素我们设置的 tag 名类似，只是在后面换成了1、2、3，所以我们通过 for 循环来获取
    var color = stairIndex === i ? '#F6A623' : '#CFCFCF';
    dataModel.getDataByTag('stair_1_' + i).s('shape.border.color', color);
    dataModel.getDataByTag('stair_2_' + i).s('shape.border.color', color);
}
```
 我们还对“警告灯”的闪烁进行了定时控制，如果是偶数秒的时候，就将灯的背景颜色设置为“无色”，否则，如果是 yellowAlarm 则设置为“黄色”，如果是 redAlarm 则设置为“红色”：
```
if (new Date().getSeconds() % 2 === 1) {
    yellowAlarm.s('shape.background', null);
    redAlarm.s('shape.background', null);
}
else {
    yellowAlarm.s('shape.background', 'yellow');
    redAlarm.s('shape.background', 'red');
}
```
# 总结
2015 年 3 月，李克强总理在政府工作报告中首次提出“互联网+”行动计划。之后，国务院印发《关于积极推进“互联网+”行动的指导意见》，推动互联网由消费领域向生产领域拓展，从而进一步提升产业发展水平，增强行业创新能力。在此“互联网+”的背景之下，城市轨道交通行业应当紧跟时代潮流，将“互联网+”思维引入工程投资管理之中，研发一种基于“互联网+”的城市轨道交通工程投资管理系统，从而提升造价管理系统的功能分布和用户体验。“互联网+”通过行业跨界，寻找互联网与城市轨道交通工程的相关性，将传统行业的数据进行信息化处理，将原本有限的数据进行提升、分析和流转，利于“互联网+”的乘数效应，显著提升工程投资管理中数据的实时动态和完整精确。
![](https://images2018.cnblogs.com/blog/591709/201807/591709-20180731075132492-1284446848.gif)
[https://hightopo.com/demo/metro/demo1.html](https://hightopo.com/demo/metro/demo1.html)
![](https://images2018.cnblogs.com/blog/591709/201807/591709-20180731075345957-1162764629.jpg)
**[https://hightopo.com/demo/large-screen/index.html](https://hightopo.com/demo/large-screen/index.html)**
