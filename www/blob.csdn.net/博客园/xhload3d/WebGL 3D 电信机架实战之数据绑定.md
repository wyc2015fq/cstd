# WebGL 3D 电信机架实战之数据绑定 - xhload3d - 博客园
# 前言
在前端中，视图层和数据层需要进行单向或者双向数据绑定，大家都已经不陌生了，有时候 2D 做的比较顺了之后，就会想要挑战一下 3D，不然总觉得痒痒的。这个 3D 机架的 Demo 我觉得非常有代表性，首先，3D 机架用途非常广，尤其是在电信行业，就算不是机架，在比如工业方面 3D 模型以及数据绑定的应用也是非常广泛的，毕竟现在工业物联网已经是大趋势了。
# 效果图
![](https://images2018.cnblogs.com/blog/591709/201803/591709-20180322163108826-1186413826.gif)
上面动图中，闪烁灯是在不断变化的，由于需要显示的效果美观一点，也实际一点，我截的图还是比较完整的，但是这个闪烁的部分有点看不清楚，在 [http://www.hightopo.com/demo/serviceShow/](http://www.hightopo.com/demo/serviceShow/) 上可以直接运行查看效果。
# 代码实现
 虽然上面 gif 图中显示的一个是 2D 的一个是 3D 的，但是构建的步骤以及需要的内容是一样的，所以本文只针对 3D 的模型进行代码实现。
## 场景搭建
搭建一个 3D 场景是非常快速的，只需要三行代码：
```
dm = new ht.DataModel();//创建一个数据容器 数据容器也可以通过 g3d.getDataModel() 获取
g3d = new ht.graph3d.Graph3dView(dm);//创建一个 3D 场景，将数据容器作为参数传递进去，这样数据容器中的内容就可以显示在 3D 场景中了
g3d.addToDOM();//将 3D 场景添加到 body 体中
```
## 3D 机架模型构建
虽然可以叫设计师直接给我一个 obj 格式的模型，但是我觉得这个比较简答，还是不要麻烦人家了。。。首先是创建一个六面体，模型上面的贴图是我以前用的一个 json 格式的文件，用来作为这个六面体的正面贴图，这些部分都是写在 json 文件里面的，我先截取一小部分的 json 内容，然后用 js 代码复现：
```
{
  "c": "ht.Node", //一个 ht.Node 类型的元素
  "i": 1277,//id
  "p": {//通过 set/get 来设置/获取的元素的部分。如 setPosition/getPosition
    "tag": "service",//设置元素标签 用来作为唯一标识
    "image": "symbols/机柜.json",//设置节点图片
    "rotationX": 1.5708,//设置节点 X 轴旋转角度
    "position": {//设置节点位置
      "x": 0,
      "y": 225
    },
    "anchor": {//设置节点锚点
      "x": 0.5,
      "y": 0.54
    },
    "anchorElevation": 1, //设置节点 y 轴锚点
    "width": 507,//设置节点宽度
    "height": 980, //设置节点长度
    "tall": 450, //设置节点高度
    "elevation": 451 //控制Node图元中心位置所在3D坐标系的y轴位置
  },
  "s": {//设置图元的 style 样式，HT 预定义的一些样式属性，通过 node.s('all.color') 获取和设置节点的样式
    "all.color": "#DDDDDD",  //设置节点六面颜色
    "top.image": "symbols/机柜.json",//设置节点顶部图片
    "front.visible": true, //设置节点正面是否可见
    "back.visible": true,
    "left.visible": true,
    "right.visible": true,
    "bottom.visible": true
  }
}
```
这部分的 json 内容大体上就是创建了一个 ht.Node 节点，然后对这个节点设置了一些属性，包括节点坐标，节点的大小，以及一些 style 样式设置。
那么，如何用代码来创建这样一个节点呢？
```
var node = new ht.Node();//创建一个 ht.Node 类型的节点
node.setTag('service'); //设置节点的标签
node.setImage('symbols/机柜.json'); //设置节点图片
node.setRotationX(Math.PI/2);//设置节点x轴旋转
node.setPosition(0, 225);//设置节点位置
node.setAnchor(0.5, 0.54);//设置节点锚点
node.setAnchorElevation(1);//设置节点y轴方向的锚点
node.setWidth(507);//设置节点的宽度
node.setHeight(980);//设置节点的长度
node.Tall(450);//设置节点的高度
node.setElevation(451);//控制Node图元中心位置所在3D坐标系的y轴位置
node.s({ //设置节点样式
    'all.color': '#ddd', //六面颜色
    'top.image': 'symbols/机柜.json'， //节点顶部图片
    'front.visible': true,//设置节点正面可见
    'back.visible': true,
    'left.visible': true,
    'right.visible': true,
    'bottom.visible': true
});
```
其实整个 json 就是由多个这种类型的图元组合而成的。我们来拆析一下，整个 3D 机架实际上是由十个图元组合而成的，第一个是整体的 3D 机柜（也就是我们上面 json 内容中创建的部分），剩下的九个都是需要动态变化闪烁灯的设备，也就是我红框框起来的部分：
![](https://images2018.cnblogs.com/blog/591709/201803/591709-20180322165055033-333399961.png)
这些设备的创建方式跟上面的 3D 机架是类似的，只不过对应的节点尺寸小点，贴图不一样，坐标不一样罢了。但是下面的这九个节点的贴图似乎有点不一样？上面有闪烁的灯，并且不止一盏！怎么动态获取他们呢？
## 矢量--数据绑定
不得不说到矢量这个概念。[矢量](http://en.wikipedia.org/wiki/Vector_graphics)在 HT for Web 中是矢量图形的简称，常见的 png 和 jpg 这类的栅格位图， 通过存储每个像素的颜色信息来描述图形，这种方式的图片在拉伸放大或缩小时会出现图形模糊，线条变粗出现锯齿等问题。 而矢量图片通过点、线和多边形来描述图形，因此在无限放大和缩小图片的情况下依然能保持一致的精确度。
这些有点都是次要的，最重要的是这个矢量可以进行数据绑定（这个数据绑定是绑定到节点中的），而且绑定方式非常容易！
矢量采用 json 格式描述，使用方式和普通的栅格位图一致，通过 node.setImage('hightopo.json') 或者 node.setIcon('hightopo.json') 等设置到数据模型中。
矢量 json 描述必需包含 width、height 和 comps 参数信息：
- width 矢量图形的宽度
- height 矢量图形的高度
- comps 矢量图形的组件 Array 数组，每个数组对象为一个独立的组件类型，数组的顺序为组件绘制先后顺序
由于这张图绘制的还是比较复杂的，所以我就将设置了数据绑定的矩形部分的矢量绘制代码粘贴出来：
```
{
    "width": 48, //一个矢量图标必备的宽度 矢量详细内容请参考 [HT for Web 矢量手册](http://hightopo.com/guide/guide/core/vector/ht-vector-guide.html)
    "height": 262,//一个矢量图标必备的高度
    "comps": [//一个矢量图标必备的 Array 数组组件
        {//数组组件中的第一个元素
            "type": "rect",//类型为矩形
            "background": {//设置矩形背景色
                "func": "attr@rectBg1", //HT 用一个带func属性的对象替换以前的参数值 这里就是进行数据绑定的地方
                "value": "rgb(255,0,0)"//如果 func 值为 undefined 或者 null 时，采用这个值
            },
            "shadow": true,//设置“阴影”
            "shadowColor": { //“阴影”颜色
                "func": "attr@shadowColor1", // 这边将“阴影”也进行了数据绑定，为的是能够实现灯“发光”的效果
                "value": "rgba(255,0,0,0.35)"//设置备选值
            },
            "shadowOffsetX": 0, //设置阴影横轴偏移量
            "shadowOffsetY": 0,//设置阴影纵轴偏移量
            "rect": [//设置该组件的宽高以及坐标
                4.38544,//x 轴坐标
                23.52679,//y 轴坐标
               14.46481, //width 组件宽度
               6.1554//height 组件高度
           ]
       }
    ]
}
```
因为一个矢量图形中有 5 个“闪烁灯”，所以我添加了 5 个组件，也就是在 comps 参数里面添加了五个元素，绑定的数据不同，为了省事，我将绑定的数据名都设置为“rectBg”后面加一个数字，这些数字依次递增。
我们就是将这样一张矢量图设置为节点的 front.image 作为节点正面显示图的：node.s('front.image', 'symbols/内部设备2.json')。
## JSON 反序列化
会不会有人好奇 json 文件里面的内容是如何转换成 3D 模型的？
说实在的，步骤依然很简单：
```
ht.Default.xhrLoad('scene/service3d.json', function(text) {//xhrLoad 函数是一个异步加载文件的函数
    dm.deserialize(text);//反序列化数据容器，解析用于生成对应的Data对象并添加到数据容器 这里相当于把 json 文件中生成的 ht.Node 节点反序列化到数据容器中，这样数据容器中就有这个节点了
});
```
由于 xhrLoad 函数是一个异步加载函数，所以如果 dm 数据容器反序列化未完成就直接调用了其中的节点，那么会造成数据获取不到的结果，所以一般来说我是将一些逻辑代码写在这个函数内部，或者给逻辑代码设置 timeout 错开时间差。
首先，由于数据都是存储在 dm 数据容器中的（通过 dm.add(node) 添加的），所以我们要获取数据除了可以通过 id、tag 等独立的方式，还可以通过遍历数据容器来获取多个元素：
```
var infos = [
    { shadowColor: 'shadowColor1', background: 'rectBg1' },
    { shadowColor: 'shadowColor2', background: 'rectBg2' },
    { shadowColor: 'shadowColor3', background: 'rectBg3' },
    { shadowColor: 'shadowColor4', background: 'rectBg4' },
    { shadowColor: 'shadowColor5', background: 'rectBg5' }
];
var len = infos.length; //获取数组中的长度
var datas = dm.toDatas(function(d) { return d.getDisplayName() === 'device'; });//以过滤函数构建新的元素集合并返回（ht.List 数组类型）
setInterval(function() {
    datas.forEach(function(data) {
        var info = infos[Math.floor(Math.random() * len)];
        var shadowName = info.shadowColor;
        var bgName = info.background;
        if (data.a(shadowName) === 'rgba(255, 0, 0, 0.35)') {//若节点业务属性“阴影”颜色为红色，则设置为绿色
            data.a(shadowName, 'rgba(0, 255, 0, 0.35)');
        }
        else {//若节点业务属性“阴影”颜色为绿色，则设置为红色
            data.a(shadowName, 'rgba(255, 0, 0, 0.35)');
        }
        if (data.a(bgName) === 'rgb(255, 0, 0)') {//若节点业务属性背景颜色为红色，则设置为绿色
            data.a(bgName, 'rgb(0, 255, 0)');
        }
        else {//若节点业务属性背景颜色为绿色，则设置为红色
            data.a(bgName, 'rgb(255, 0, 0)');
        }
    });
}, 1000);
```
# 结束语
整体来说这个 Demo 还是比较有价值的，一是快速实现了 3D 机柜模型，二是对模型上的元素进行了数据绑定。只是想让你们知道，清晰的图片绘制没有那么难~ 3D 的世界没有那么难~ 数据绑定也没有那么难！我希望也能让您发现这并不是件难事。
