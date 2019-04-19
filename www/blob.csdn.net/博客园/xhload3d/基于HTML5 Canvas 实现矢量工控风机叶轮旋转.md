# 基于HTML5 Canvas 实现矢量工控风机叶轮旋转 - xhload3d - 博客园
之前在[拓扑](http://www.hightopo.com/)上的应用都是些静态的图元，今天我们将在拓扑上设计一个会动的图元——叶轮旋转。
先看看最后我们实现的效果：[http://www.hightopo.com/demo/fan/index.html](http://www.hightopo.com/demo/fan/index.html)
![](https://images2015.cnblogs.com/blog/591709/201610/591709-20161014121211640-1160214789.png)
我们先来看下这个叶轮模型长什么样
![](https://images0.cnblogs.com/blog2015/591709/201507/200109541767916.png)
从模型上看，这个叶轮模型有三个叶片，每一个叶片都是不规则图形，显然无法用上我们[HT for Web](http://www.hightopo.com/)的基础图形来拼接，那么我们该怎么做呢？很简单，在HT for Web中提供了[自定义图形](http://www.hightopo.com/)的方案，我们可以通过自定义图形来绘制像叶片这种不规则图形。
在绘制叶片之前，我们得先来了解下HT for Web的自定义图形绘制的基本知识：
绘制自定义图形需要制定[矢量](http://www.hightopo.com/)类型为shape，并通过points的Array数组指定每个点信息， points以[x1, y1, x2, y2, x3, y3, ...]的方式存储点坐标。曲线的多边形可通过segments的Array数组来描述， segment以[1, 2, 1, 3 ...]的方式描述每个线段：
1: moveTo，占用1个点信息，代表一个新路径的起点
2: lineTo，占用1个点信息，代表从上次最后点连接到该点
3: quadraticCurveTo，占用2个点信息，第一个点作为曲线控制点，第二个点作为曲线结束点
4: bezierCurveTo，占用3个点信息，第一和第二个点作为曲线控制点，第三个点作为曲线结束点
![](https://images0.cnblogs.com/blog2015/591709/201507/200107375048703.png)
5: closePath，不占用点信息，代表本次路径绘制结束，并闭合到路径的起始点
对比闭合多边形除了设置segments参数外，还可以设置closePath属性： * closePath获取和设置多边形是否闭合，默认为false，对闭合直线采用这种方式，无需设置segments参数。
好了，那么接下来我们开始设计叶片了
![](https://images0.cnblogs.com/blog2015/591709/201507/200110203631384.png)
```
ht.Default.setImage('vane', {
    width: 97,
    height: 106,
    comps: [
        {
            type: 'shape',
            points: [
                92, 67,
                62, 7,
                0, 70,
                60, 98
            ],
            segments: [
                1, 2, 2, 2
            ],
            background : 'red'
        }
    ]
});
```
我们在[矢量](http://www.hightopo.com/)中定义了4个顶点，并且将这4个顶点通过直线勾勒出叶片的大致形状，虽然有些抽象，但是，接下来将会通过增加控制点和改变segment参数来让这个叶片发生蜕变。
首先我们通过bezierCurveTo方式向第一个和第二个顶点之间的线段添加两个控制点，从而绘制出曲线，以下是points及segments属性：
```
points: [
    92, 67,
    93, 35, 78, 0, 62, 7,
    0, 70,
    60, 98
],
segments: [
    1, 4, 2, 2
]
```
![](https://images0.cnblogs.com/blog2015/591709/201507/200111216914080.png)
这时候与上一个图相比较，有一条边一件有些弧度了，那么接下来就来处理第二条边和第三条边
![](https://images0.cnblogs.com/blog2015/591709/201507/200111418329892.png)![](https://images0.cnblogs.com/blog2015/591709/201507/200111522072357.png)
```
points: [
    92, 67,
    93, 35, 78, 0, 62, 7,
    29, 13, 4, 46, 0, 70,
    28, 53, 68, 60, 60, 98
],
segments: [
    1, 4, 4, 4
]
```
看吧，现在是不是有模有样了，现在叶片已经有了，那么接下来要做的就是使用三个这样的叶片拼接成一个叶轮。
将已有的资源拼接在一起需要用到[矢量](http://www.hightopo.com/)中的image类型类定义新的矢量，具体的使用方法如下：
```
ht.Default.setImage('impeller', {
    width: 166,
    height: 180.666,
    comps : [
        {
            type: 'image',
            name: 'vane',
            rect: [0, 0, 97, 106]
        },
        {
            type: 'image',
            name: 'vane',
            rect: [87.45, 26.95, 97, 106],
            rotation: 2 * Math.PI / 3
        },
        {
            type: 'image',
            name: 'vane',
            rect: [20.45, 89.2, 97, 106],
            rotation: 2 * Math.PI / 3 * 2
        }
    ]
});
```
![](https://images0.cnblogs.com/blog2015/591709/201507/200112489578085.png)
在代码中，我们定义了三个叶片，并且对第二个和第三个叶片做了旋转和定位的处理，让这三个叶片排布组合成一个叶轮来，但是怎么能让叶轮中间空出一个三角形呢，这个问题解决起来不难，我们只需要在叶片的points属性上再多加一个顶点，就可以填充这个三角形了，代码如下：
```
points: [
    92, 67,
    93, 35, 78, 0, 62, 7,
    29, 13, 4, 46, 0, 70,
    28, 53, 68, 60, 60, 98,
    97, 106
],
segments: [
    1, 4, 4, 4, 2
]
```
在points属性上添加了一个顶点后，别忘了在segments数组的最后面添加一个描述，再来看看最终的效果：
![](https://images0.cnblogs.com/blog2015/591709/201507/200113329109524.png)
到这个叶轮的资源就做好了，那么接下来就是要让这个叶轮旋转起来了，我们先来分析下：
要让叶轮旋转起来，其实原理很简单，我们只需要设置rotation属性就可以实现了，但是这个rotation属性只有在不断的变化中，才会让叶轮旋转起来，所以这个时候就需要用到定时器了，通过定时器来不断地设置rotation属性，让叶轮动起来。
恩，好像就是这样子的，那么我们来实现一下：
首先是创建一个节点，并设置其引用的image为impeller，再将其添加到DataModel，令节点在拓扑中显示出来：
```
var node = new ht.Node();
node.setSize(166, 181);
node.setPosition(400, 400);
node.setImage('impeller');
dataModel.add(node);
```
接下来就是添加一个定时器了：
```
window.setInterval(function() {
    var rotation = node.getRotation() + Math.PI / 10;
    if (rotation > Math.PI * 2) {
        rotation -= Math.PI * 2;
    }
    node.setRotation(rotation);
}, 40);
```
OK了，好像就是这个效果，但是当你选中这个节点的时候，你会发现这个节点的边框在不停的闪动，看起来并不是那么的舒服，为什么会出现这种情况呢？原因很简单，当设置了节点的rotation属性后，节点的显示区域就会发生变化，这个时候节点的宽高自然就发生的变化，其边框也自然跟着改变。
还有，在很多情况下，节点的rotation属性及宽高属性会被当成业务属性来处理，不太适合被实时改变，那么我们该如何处理，才能在不不改变节点的rotation属性的前提下令叶轮转动起来呢？
在[矢量](http://www.hightopo.com/)中，好像有数据绑定的功能，在手册中是这么介绍的：
绑定的格式很简单，只需将以前的参数值用一个带func属性的对象替换即可，func的内容有以下几种类型：
1. function类型，直接调用该函数，并传入相关Data和view对象，由函数返回值决定参数值，即func(data, view);调用。
2. string类型：
    2.1 style@***开头，则返回data.getStyle(***)值，其中***代表style的属性名。
    2.2 attr@***开头，则返回data.getAttr(***)值，其中***代表attr的属性名。
    2.3 field@***开头，则返回data.***值，其中***代表data的属性名。
    2.4 如果不匹配以上情况，则直接将string类型作为data对象的函数名调用data.***(view)，返回值作为参数值。
除了func属性外，还可设置value属性作为默认值，如果对应的func取得的值为undefined或null时，则会采用value属性定义的默认值。 例如以下代码，如果对应的Data对象的attr属性stateColor为undefined或null时，则会采用yellow颜色：
```
color: {
    func: 'attr@stateColor',
    value: 'yellow'
}
```
数据绑定的用法已经介绍得很清楚了，我们不妨先试试绑定叶片的背景色吧，看下好不好使。在[矢量](http://www.hightopo.com/)vane中的background属性设置成数据绑定的形式，代码如下：
```
background : {
    value : 'red',
    func : 'attr@vane_background'
}
```
在没有设置vane_background属性的时候，令其去red为默认值，那么接下来我们来定义下vane_background属性为blue，看看叶轮会不会变成蓝色：
```
node.setAttr('vane_background', ‘blue');
```
看下效果：
![](https://images0.cnblogs.com/blog2015/591709/201507/200115471605910.png)
果然生效了，这下好了，我们就可以让叶轮旋转变得更加完美了，来看看具体该这么做。
首先，我们先在节点上定义一个自定义属性，名字为：impeller_rotation
```
node.setAttr('impeller_rotation', 0);
```
然后再定义一个名字为rotate_impeller的矢量，并将rotation属性绑定到节点的impeller_rotation上：
```
ht.Default.setImage('rotate_impeller', {
    width : 220,
    height : 220,
    comps : [
        {
            type : 'image',
            name : 'impeller',
            rect : [27, 20, 166, 180.666],
            rotation : {
                func : function(data) { 
                    return data.getAttr('impeller_rotation'); 
                }
            }
        }
    ]
});
```
这时候我们在定时器中修改节点的rotation属性改成修改自定义属性impeller_rotation就可以让节点中的叶轮旋转起来，并且不会影响到节点自身的属性，这就是我们想要的效果。
![](https://images0.cnblogs.com/blog2015/591709/201507/200116501445536.png)
在2D上可以实现，在3D上一样可以实现，下一章我们就来讲讲叶轮旋转在3D上的应用，今天就先到这里，下面附上今天Demo的源码，有什么问题欢迎大家咨询。
[http://www.hightopo.com/demo/fan/index.html](http://www.hightopo.com/demo/fan/index.html)
http://player.youku.com/player.php/sid/XMTI4ODc1NjA1Mg==/v.swf
```
ht.Default.setImage('vane', {
    width : 97,
    height : 106,
    comps : [
        {
            type : 'shape',
            points : [
                92, 67,
                93, 35, 78, 0, 62, 7,
                29, 13, 4, 46, 0, 70,
                28, 53, 68, 60, 60, 98,
                97, 106
            ],
            segments : [
                1, 4, 4, 4, 2
            ],
            background : {
                value : 'red',
                func : 'attr@vane_background'
            }
        }
    ]
});
ht.Default.setImage('impeller', {
    width : 166,
    height : 180.666,
    comps : [
        {
            type : 'image',
            name : 'vane',
            rect : [0, 0, 97, 106]
        },
        {
            type : 'image',
            name : 'vane',
            rect : [87.45, 26.95, 97, 106],
            rotation : 2 * Math.PI / 3
        },
        {
            type : 'image',
            name : 'vane',
            rect : [20.45, 89.2, 97, 106],
            rotation : 2 * Math.PI / 3 * 2
        }
    ]
});
ht.Default.setImage('rotate_impeller', {
    width : 220,
    height : 220,
    comps : [
        {
            type : 'image',
            name : 'impeller',
            rect : [27, 20, 166, 180.666],
            rotation : {
                func : function(data) {
                    return data.getAttr('impeller_rotation');
                }
            }
        }
    ]
});
function init() {
    var dataModel = new ht.DataModel();
    var graphView = new ht.graph.GraphView(dataModel);
    var view = graphView.getView();
    view.className = "view";
    document.body.appendChild(view);
    var node = new ht.Node();
    node.setSize(220, 220);
    node.setPosition(200, 400);
    node.setImage('rotate_impeller');
    node.setAttr('impeller_rotation', 0);
    node.setAttr('vane_background', 'blue');
    dataModel.add(node);
    var node1 = new ht.Node();
    node1.setSize(166, 181);
    node1.setPosition(500, 400);
    node1.setImage('impeller');
    dataModel.add(node1);
    window.setInterval(function() {
        var rotation = node.a('impeller_rotation') + Math.PI / 10;
        if (rotation > Math.PI * 2) {
            rotation -= Math.PI * 2;
        }
        node.a('impeller_rotation', rotation);
        node1.setRotation(rotation);
    }, 40);
}
```
