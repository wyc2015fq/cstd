# 基于 HTML5 Canvas 绘制的电信网络拓扑图 - xhload3d - 博客园
电信网结构（telecommunication network structure）是指电信网各种网路单元按技术要求和经济原则进行组合配置的组合逻辑和配置形式。组合逻辑描述网路功能的体系结构，配置形式描述网路单元的邻接关系，即以交换中心（或节点）和传输链路所组成的拓扑结构。常见的网络拓扑结构有星型结构、总线结构、环形结构、树形结构、网状结构、混合型拓扑以及蜂窝拓扑结构等，本文的例子主要描绘的是总线型拓扑，在显示上相对其他的结构类型来说更清晰明了，绘制起来也非常容易。
虽然题目起的名字是电信网络拓扑图，几乎所有的拓扑图都能涵盖，例如基本网络图，网络拓扑图，机架图，网络通信图，3D网络图等等。
效果图如下：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180129182919390-1438018385.gif)
这个图看起来挺简单的，代码也少，但是内容不少。
首先，机柜01、机柜02、机柜03 都是 ht.Group “组”类型，ht.Group 类型用于作为父容器包含孩子图元，在 GraphView 拓扑图（[http://www.hightopo.com](http://www.hightopo.com)）上可通过双击进行展开合并，合并时会自定隐藏子孙图元节点， 如果有子节点有连线连接到外部时，合并的 Group 将代理进行连接。Group 的移动会带动孩子节点跟随， 孩子的位置和大小变化也会影响 Group 的展开图形和 position 位置。
这边提到一个代理连线的问题，“代理”两个字可以很好地表明代理连线意义。实际上就是如果组内部的节点与组外部的节点有连线，那么在组合并的时候，会在这个组会“代理”与外部节点之间的连线，这个就是代理连线。我们拿机柜02来说吧，机柜02内部有一个“电脑”与“内部网络交换机”之间有两条连线，那么当我们双击机柜02合并时，实际上就相当于机柜02与“内部网络交换机”之间有两条连线了。
那么，我们来看看如何绘制这个组以及组内部的节点吧，先创建“机柜02”的 Group 节点，因为整个例子我创建了三个 Group 节点，而且创建的方式都类似，因此把创建组的代码封装起来复用：
```
function createGroup(name, x, y) {
    var group = new ht.Group();//组类型 实际上也是一个节点
    group.setExpanded(true);//设置展开组
    group.setName(name);//设置组的名字
    group.s({//设置组的样式style
        'group.title.background': 'rgba(14,36,117,0.80)',//组展开后的title背景颜色，仅对group.type为空的类型起作用
        'group.background': 'rgba(14,36,117,0.40)',//组展开后的背景颜色
        'group.title.align': 'center'//组展开后的title文字水平对齐方式，默认值为'left'，可设置为center和right
    });
    group.setPosition(x, y);//设置组的位置
    group.setImage('images/服务器.json');//设置拓扑上展现的图片信息，在GraphView拓扑图中图片一般以position为中心绘制
    dataModel.add(group);//将创建的组节点添加进数据容器中
    return group;
}
```
组是可以通过双击展开合并的，展开的时候显示的是一个有标题栏的框（当然这些都是可以自定义的），合并的时候就显示上面代码中设置的 group.setImage 中的图片。
所有机柜内部的节点都是 ht.Node 类型的节点，所以我也封装了一下：
```
function createNode(image, parent, x, y) {
    var node = new ht.Node();//创建一个 Node 节点
    if (image) node.setImage(image);//设置节点的显示图片
    if (parent) node.setParent(parent);//设置节点的父亲
    if (x && y) node.setPosition(x, y);//设置节点的位置
    dataModel.add(node);//将节点添加进数据容器中
    return node;
}
```
生成机柜02：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180129155544515-300721048.png)
```
cabinet = createGroup('机柜02', 146, 445);//创建机柜02
createNode('images/正常.json', cabinet, 78, 440).s('label', '数据监控分析系统');//创建带有“正常”图片的节点，并设置这个节点的文字为“数据监控分析系统”
```
因为连线需要的是“源节点”以及“终节点”，这边源节点是中间的“内部网络交换机”，我们再创建这个节点：
```
var line = createNode();//创建一个节点
line.setSize(725, 20);//设置节点大小
line.setPosition(310, 325);//设置节点位置
line.s({//设置节点的style属性
    'shape': 'roundRect',//决定shape的形状，默认值为空，代表用image绘制。roundRect四周圆角矩形
    'shape.background': 'rgba(14,36,117,0.80)',//背景填充颜色，为null代表不填充背景
    'shape.border.color': '#979797',//边框颜色
    'shape.corner.radius': 10,//该参数指定roundRect类型的圆角半径，默认为空系统自动调节，可设置正数值
    'label': '内部网络交换机', //文字内容，默认为空
    'label.position': 45,//文字内容，默认为空
    'label.offset.x': 50,//文字水平偏移，对于Edge意味着沿着连线方向水平偏移
    'label2': '内部网络交换机',//HT默认除了label.*的属性外，还提供了label2.*的属性，用于满足一个图元需要显示双文字的情况
    'label2.position': 48,
    'label2.offset.x': 50,
    'label2.offset.y': 2,
});
```
不知道你们有没有注意到，有一个 label2 的样式属性，这个是 HT 为了能在一个节点上添加两个 label 文本而增加的功能，label 属性和 label2 的属性是完全相同的，只要在设置属性的时候用 label 和 label2 区分开来就可以。
源节点和终节点都具备了，可以制作连线了：
```
createEdge(line, createNode('images/电脑.json', cabinet, 185, 450), 'rgb(30,232,178)', -100, true);
//参数1 源节点，参数2 终节点，参数3 连线颜色，参数4 连线起始点的水平偏移，参数5 是否创建两条连线
```
还有一点有趣的，“交换机”的部分，最左侧蓝色方形的节点和中间长条的节点并不是一体的，而是分离的，但是我通过 setHost 进行节点与节点间的吸附，然后反吸附回来，这样操作上就相当于这两个节点是一体的：
```
var exchange = createNode('images/交换机.json', null, -53, 313);
exchange.setHost(line);//设置吸附
line.setHost(exchange);//反吸附 又设置line的吸附为exchange
```
因为 HT 会按照节点添加进数据容器中的顺序来进行层次的排列，我的交换机是在 line 的添加之后的，所以默认交换机的节点会显示在 line 之下，我们将默认的层级显示关闭，并设置交换机 exchange 显示在数据容器的顶部：
```
dataModel.setAutoAdjustIndex(false);//将自动调整data在容器中索引顺序的开关关闭
dataModel.sendToTop(exchange);//将data在拓扑上置顶
```
 代码就是这些，还有不懂的可以留言或者私信我也可以，大家一起探讨。
