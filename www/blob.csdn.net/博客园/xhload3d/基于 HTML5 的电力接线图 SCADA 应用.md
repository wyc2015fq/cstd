# 基于 HTML5 的电力接线图 SCADA 应用 - xhload3d - 博客园
在电力、油田燃气、供水管网等工业自动化领域 Web SCADA 的概念已经提出了多年，早些年的 Web SCADA 前端技术大部分还是基于 Flex、Silverlight 甚至 Applet 这样的重客户端方案，在 HTML5 流行前 VML 和 SVG 算是真正纯种 Web 方案也是有不少应用，近些年随着 HTML5 的流行，加上移动终端以及浏览器对 HTML5 支持的普及，越来越多新项目开始采用真正纯 HTML5 的方案，更具体的应该说是大数据量应用性能高于 SVG 的 Canvas 方案，已经逐渐成为当今 Web SCADA 前端技术的首选标配方案。
例子代码下载：[http://www.hightopo.com/demo/electric-bling/index.html](http://www.hightopo.com/demo/electric-bling/index.html)
示例图片（图中“发光”的部分是会闪烁的）：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180115144411537-1372557034.png)
这个例子我依旧是用 [HT for Web](http://www.hightopo.com/) 进行开发的，其中重复的部分我都封装为一个“图标”了，这边说的“图标”指的就是矢量图标。矢量在 HT for Web 中是矢量图形的简称，常见的 png 和 jpg 这类的栅格位图， 通过存储每个像素的颜色信息来描述图形，这种方式的图片在拉伸放大或缩小时会出现图形模糊，线条变粗出现锯齿等问题。 而矢量图片通过点、线和多边形来描述图形，因此在无限放大和缩小图片的情况下依然能保持一致的精确度。
在 HT for Web 中所有能用栅格位图的地方都可用矢量图形替代，例如 GraphView 组件上的图元图片，TreeView 和 TableView 上的图标等， 甚至整个 HT 框架做出来的系统界面可以实现全矢量化，这样 GraphView 组件上的图元缩放都不会失真，并且不再需要为 Retina 显示屏提供不同尺寸的图片， 在 [devicePixelRatio](http://www.quirksmode.org/blog/archives/2012/07/more_about_devi.html) 多样化的移动时代， 要实现完美的跨平台，矢量可能是的最低成本的解决方案。
在 HT 中，矢量采用 JSON 格式描述，使用方式和普通的栅格位图一致，通过 ht.Default.setImage('hightopo', jsonObject) 进行注册， 使用是将相应图片注册名设置到数据模型即可，如 node.setImage('hightopo') 和 node.setIcon('hightopo') 等。
矢量 json 描述必需包含 width、height 和 comps 参数信息：
- width 矢量图形的宽度
- height 矢量图形的高度
- comps 矢量图形的组件 Array 数组，每个数组对象为一个独立的[组件类型](http://www.hightopo.com/guide/guide/core/vector/ht-vector-guide.html#ref_type)，数组的顺序为组件绘制先后顺序
同时可设置以下可选参数信息：
- visible 是否可见，默认为 true
- opacity 透明度，默认为 1，可取值范围 0~1
```
```
- ``color 染色颜色，设置上该颜色后矢量内部绘制内容将会融合该染色值
- clip 用于裁剪绘制区域，可设置两种类型：boolean
- boolean 类型，控制绘制时超出 width 和 height 区域的内容是否被裁剪，默认为 false 不裁剪
- function 类型，可利用 canvas 画笔绘制，实现自定义裁剪任意形状的效果
那么我们来看看这个图标是怎么用 HT 绘制的：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180115152955974-429903102.png)
从图片上可以看出来，这个图标由一条直线、一个矩形以及一个箭头组成，我们把这个图标取名为 arrow：
```
ht.Default.setImage('arrow', {//注册图片 arrow
    "width": 60,//矢量图形的宽度
    "height": 30,//矢量图形的高度
    "comps": [//矢量图形的组件 Array 数组，每个数组对象为一个独立的组件类型，数组的顺序为组件绘制先后顺序
        {//绘制直线部分
            "type": "shape",//多边形
            "borderWidth": 1,//边框宽度
            "borderColor": "rgb(255,0,0)",//边框颜色
            "points": [//点信息 `points `以 `[x1, y1, x2, y2, x3, y3, ...] `的方式存储点坐标
                1.4262,
                14.93626,
                51.46768,
                14.93626
            ]
        },
        {//绘制箭头尖的部分
            "type": "shape",
            "borderWidth": 1,
            "borderColor": "rgb(255,0,0)",
            "rotation": 4.71239,//旋转
            "points": [
                45.50336,
                9.63425,
                52.88591,
                13.92955,
                60.26846,
                9.63425,
                52.88591,
                20.23827,
                45.50336,
                9.63425
            ]
        },
        {//绘制矩形部分
            "type": "rect",//矩形
            "background": {//背景颜色
                "func": "attr@lightBg",
                "value": "rgb(255,0,0)"
            },
            "borderColor": "#979797",
            "shadow": {//阴影
                "func": "attr@shadow",
                "value": false
            },
            "shadowColor": {//阴影颜色
                "func": "attr@shadowColor",
                "value": "rgba(255,0,0,0.7)"
            },
            "shadowBlur": 32,//阴影模糊
            "shadowOffsetX": 0,//阴影横偏移
            "shadowOffsetY": 0,//阴影纵偏移
            "rect": [//指定矩形的区域 [x, y, width, height] 起始位置的坐标和矩形的大小值 
                11.44694,
                10.43626,
                30,
                9
            ]
        }
    ]
});
```
每一个数组对象中的基本类型与 style 的 [shape](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_styleshape) 参数是完全一一对应， 只是将 style 中的名称改成骆驼式命名法去掉了`.`分隔符，查找对应的 style 属性请参考 [HT for Web 风格手册](http://www.hightopo.com/guide/guide/core/theme/ht-theme-guide.html)，有些后期添加的属性可能在风格手册中还没有添加，大家只要知道这么一个属性就行了，一般看属性名就知道这个属性的功能了。
上面代码中有一段可能让大家疑惑的点我没有在代码中解释，接下来我们着重来讲一下这个部分的内容：[数据绑定](http://www.hightopo.com/guide/guide/core/databinding/ht-databinding-guide.html)。从文章一开始的图片我们知道，这个图标中的矩形部分是会变颜色的。
数据绑定意味将 Data 图元的数据模型信息，与界面图形的颜色、大小和角度等可视化参数进行自动同步， HT 的预定义图形组件默认就已与 DataModel 中的 Data 数据绑定，例如用户修改 Node 的 position 位置值， 则 GraphView 和 Graph3dView 上的相应图元位置会自动同步变化。
传统的数据绑定有单向绑定和双向绑定的概念，但 HT 系统的设计模式使得绑定更加简化易于理解，HT 只有一个 DataModel 数据模型， 绑定 DataModel 的图形组件并没有组件内部的其他数据模型，所以组件都是如实根据 DataModel 来呈现界面效果，因此当用户拖拽图元移动时， 本质也是修改了数据模型中 Node 的 position 位置值，而该属性变化触发的事件通过模型再次派发到图形组件，引发图形组件根据新的模型信息刷新界面。
绑定的格式很简单，只需将以前的参数值用一个带 func 属性的对象替换即可，func 的内容有以下几种类型：
- function 类型，直接调用该函数，并传入相关 Data 和 view 对象，由函数返回值决定参数值，即 func(data, view) 调用。
- string 类型：
- func@*** 开头，则返回 data.getStyle(***) 值，其中 *** 代表 style 的属性名
- attr@*** 开头，则返回 data.getAttr(***) 值，其中 *** 代表 attr 的属性名
- field@*** 开头，则返回 data.*** 值，其中 *** 代表 data 的属性名
- 如果不匹配以上情况，则直接将 string 类型作为 data 对象的函数名调用 data.***(view)，返回值作为参数值
除了 func 属性外，还可设置 value 属性作为默认值，如果对应的 func 取得的值为 undefined 或 null 时，则会采用 value 属性定义的默认值。 例如以下代码，如果对应的 Data 对象的 attr 属性 lightBg 为 undefined 或 null 时，则会采用 rgb(255, 0, 0) 颜色：
```
"background": {//背景颜色
    "func": "attr@lightBg",// 返回的是 getAttr('lightBg')的值
    "value": "rgb(255,0,0)"//设置默认值
}
```
同理，上面代码中的 shadow 和 shadowColor 也都是以这种方式来进行数据绑定的，绑定的数据只与这个数组对象部分有关，所以就算这个图标是一张图片，我们还是能单独控制局部改变颜色等等。想了解所有的 func 的使用可以参考这个例子[http://www.hightopo.com/guide/guide/core/databinding/examples/example_piebinding.html](http://www.hightopo.com/guide/guide/core/databinding/examples/example_piebinding.html)，所有的类型都用上了，非常实用。
我在代码中就是通过控制这几个绑定的属性来改变这个数组对象的颜色的，灯要闪烁，肯定会有“发光”的感觉才更真实，那么这里还需要解释一个内容，shadow 这个属性，解释为阴影，什么是阴影？比较好的一种解释就是，在一个矩形框中，由矩形中心点触发，由内至外颜色逐渐变浅，有一点像虚化，下面这张图片就是阴影的展示：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180115163705131-488408844.png)
接着是搭建场景，大家可以直接使用 lightBling/displays/电力 下的 大厦.json 文件，在这个文件中，我设置了部分的“箭头”图标的 tag 标签。在 HT 中，一般建议 id 属性由 HT 自动分配，用户业务意义的唯一标示可存在 tag 属性上，通过 Data#setTag(tag) 函数允许任意动态改变 tag 值， 通过 DataModel#getDataByTag(tag) 可查找到对应的 Data 对象，并支持通过 DataModel#removeDataByTag(tag) 删除 Data 对象。
不过我是直接在 json 中添加 “tag” 属性，具体的 json 拓扑结构说明如下：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180115165244881-1460237006.png)
我们用到的 大厦.json，我拿一部分出来解析一下：
```
{
    "c": "ht.Node",//类名，用来反序列化
    "i": 274997,//id 值
    "p": {//get/set 类型属性 这里面的所有属性都可通过 get/set获取和设置
        "displayName": "灯-红",//显示名称
        "tag": "alarm",//标签 可通过 getTag 和 setTag 来获取和设置
        "image": "symbols/隧道用图标/交通灯/灯/灯-红.json",//图片 引用的路径为相对路径 这边调用的“红灯”图标的 json 文件
        "position": {//坐标
          "x": 70.9971,
          "y": 47.78651
        }
    },
    "s": {//对应 setStyle 属性
        "2d.movable": false,//2d 下不可以动 若要开启，直接设置 setStyle('2d.movable', true) 即可，下面同理
        "2d.editable": false//2d 下不可编辑
    }
}
```
其实整个不需要动画的部分都是 json 文件中的内容，大家可以根据上面的 json 拓扑结构来解析图纸的 json。那么问题来了，如何在 GraphView 中载入图纸的 json 文件？HT 封装了 ht.Defautl.xhrLoad 函数用来将对应的图纸 json 载入到 GraphView 上，参数为 text 文本，需要通过 ht.Default.parse 转义成 json：
```
ht.Default.xhrLoad('displays/电力/大厦.json', function(text){
    var json = ht.Default.parse(text);
    window.gv.dm().deserialize(json);//反序列化，并将反序列化的对象加入 `DataModel`
});
```
此时，DataModel 中的内容就是这个 json 文件反序列化出来的所有图元了，所以我们可以通过 DataModel 任意获取和改变 json 中的图元的样式和属性。其中，setAttr/getAttr 中的属性我们必须先定义一下，不然 HT 又不知道这个节点是否有这个用户自定义的属性：
```
for(var i = 0; i < gv.dm().size(); i++){
    var data = gv.dm().getDatas().get(i);//获取 datamodel 中的对应 i 的节点
    if(data.getTag()){//如果这个节点有设置 tag 值
        data.a('shadow', false);//设置自定义属性，并且给一个值
        data.a('shadowColor', 'rgba(255,0,0,0.9)');
        data.a('lightBg', 'rgb(255, 0, 0)');
        data.a('alarmColor', 'red');
    }
}
```
这下我们就可以任意操作上面已经声明过的属性了，当然，HT 默认的属性我们也能任意操作！我在 json 文件中设置了几个 tag 标签，light1~light15 以及 alarm 标签，我们可以通过 data.getTag() 来获取这个节点对应的标签名，或者通过 dataModel.getDataByTag(tagName) 已知标签名来获取对应节点。
动画的部分 HT 有三种动画的方式，针对的点不同，这里我用到的是 [schedule](http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-guide.html) 主要用于在指定的时间间隔进行函数回调处理。HT 中调度进行的流程是，先通过 DataModel 添加调度任务，DataModel 会在调度任务指定的时间间隔到达时， 遍历 DataModel 所有图元回调调度任务的 action 函数，可在该函数中对传入的 Data 图元做相应的属性修改以达到动画效果。
以下是我例子中的动画相关代码：
```
var blingTask = {
    interval: 1000,//间隔毫秒数
    action: function(data){//间隔动作函数
        if(data.getTag() === 'light1' || data.getTag() === 'light13' || data.getTag() === 'light5' || data.getTag() === 'light6' || data.getTag() === 'light10' || data.getTag() === 'light11' || data.getTag() === 'light12' || data.getTag() === 'light14' || data.getTag() === 'light15'){
            if(data.a('lightBg') === 'rgb(255, 0, 0)'){//如果属性lightBg值为这个，就做以下一系列的动作 
                data.a('lightBg', 'rgb(0, 255, 0)');
                data.a('shadow', true);
                data.a('shadowColor', 'rgba(0, 255, 0, 0.9)');
            }else if(data.a('lightBg') === 'rgb(0, 255, 0)'){
                data.a('lightBg', 'rgb(255, 255, 0)');
                data.a('shadow', true);
                data.a('shadowColor', 'rgba(255, 255, 0, 0.9)');
            }else{
                data.a('lightBg', 'rgb(255, 0, 0)');
                data.a('shadow', true);
                data.a('shadowColor', 'rgba(255, 0, 0, 0.9)');
            }
        }else if(data.getTag() === 'alarm'){
            if(data.a('alarmColor') === 'red'){
                data.a('alarmColor', 'rgb(0, 255, 0)');
            }else{
                data.a('alarmColor', 'red');
            }
        }
    }
};
window.gv.dm().addScheduleTask(blingTask);//添加动画进 DataModel 中
```
其他部分我相信大家都能看得懂，实在不就去[官网](http://hightopo.com)(或者[这个链接](https://www.kancloud.cn/hightopo/structure)也行，里面还有常见问题)查查对应的文档，写得很清楚哦~
