# 基于HTML5快速搭建TP-LINK电信拓扑设备面板 - xhload3d - 博客园
　　今天我们以真实的TP-LINK设备面板为模型，完成设备面板的搭建，和指示灯的闪烁和图元流动。
　　先来目睹下最终的实现效果：[http://www.hightopo.com/demo/blog_tplink_20170511/index.html](http://www.hightopo.com/demo/blog_tplink_20170511/index.html)
![](https://images2015.cnblogs.com/blog/591709/201705/591709-20170518092538307-1550400612.gif)
1、TP-LINK面板
    我们从TP-LINK的设备面板开始，设备面板的示意图如下：
![](https://images2015.cnblogs.com/blog/591709/201705/591709-20170518113455572-1663387114.gif)
    显而易见，设备面板基本上能由[HT for Web](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)（[http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)）的基本图形（rect、circle、oval等）构成，而中间的接口需要用自定义图形来解决，下面让我们一步步实现它，准备工作如下：
    导入我们的[HT](http://www.hightopo.com/)（http://www.hightopo.com/）：
```
<script src="ht.js"></script>
```
     创建数据模型容器，并将其加入DOM：
```javascript;gutter
dataModel = new ht.DataModel();//创建数据模型容器  
graphView = new ht.graph.GraphView(dataModel);//创建拓扑图组件  
graphView.addToDOM();
```
    可能有人看到这里的addToDOM()很疑惑？没错，这是[HT](http://www.hightopo.com/)新增的API！之前我们要创建一个图形界面，不光需要在CSS样式中定义mian的top、left，还需要对window对象的resize事件进行监听等等，所以我们新增addToDOM()帮您做好这一系列的事情，可以看看源代码中的实现方式：
```javascript;gutter
p.addToDOM = function(){     
        var self = this,  
            view = self.getView(),     
            style = view.style;  
        document.body.appendChild(view);              
        style.left = '0';  
        style.right = '0';  
        style.top = '0';  
        style.bottom = '0';        
        window.addEventListener('resize', function () { self.iv(); }, false);  
        self.iv();  
    },
```
     准备工作完成后，就可以开始面板的绘制，对于基本图形，只需设置其相应地样式即可，例如有立体效果的按钮部分：
```javascript;gutter
{  
      type: "circle",  
      shadow: true,  
      background: "rgb(0,0,0)",  
      borderWidth: 1,  
      borderColor: "rgb(0,0,0)",  
      border3d: true,  
      gradient: "spread.horizontal",  
      gradientColor: "rgb(79,77,77)",  
      dashColor: "rgb(0,0,0)",  
      rotation: 3.141592653589793,  
      rect: [  
        677, 157,  
        43, 34  
      ]  
}
```
     对于自定义图形，前面也有介绍过，详见[HT for Web形状手册](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_shape)（[http://www.hightopo.com/guide/guide/core/shape/ht-shape-guide.html](http://www.hightopo.com/guide/guide/core/shape/ht-shape-guide.html)）。需要制定[矢量](http://www.hightopo.com/)类型为shape，其形状主要由points和segments这两个属性描述：
     points为ht.List类型数组的顶点信息，顶点为{x: 100, y:200}格式的对象；
segments为ht.List类型的线段数组信息，线段为1~5的整数，分别代表不同的顶点连接方式，segments主要用于绘制曲线，或者有跳跃断点的情况，其1~5的取值含义如下：
1：moveTo，占用一个点的信息，代表一个新路径的起点；
     2：lineTo，占用一个点信息，代表从上次最后点连接到该点；
     3：quadraticCurveTo，占用三个点信息，第一个点作为曲线控制点，第二个点作为曲线结束点；
     4：bezierCurveTo，占用三个点信息，第一和第二个点作为曲线控制点，第三个点作为曲线结束点；
     5：closePath，不占用点信息，代表本次路径绘制结束，并闭合到路径的起始点。
![](https://images2015.cnblogs.com/blog/591709/201705/591709-20170518113416197-1041482350.png)
     示例如下：
```javascript;gutter
ht.Default.setImage('tplink', {  
        width: 97,  
        height: 106,  
        comps: [  
            {  
          type: "shape",  
          background: "rgb(20,60,140)",  
          borderWidth: 8,  
          borderColor: "gray",  
          borderCap: "round",  
          points: [  
            269, 140,  
            359, 140,  
            359, 180,  
            329, 180,  
            329, 190,  
            299, 190,  
            299, 180,  
            269, 180,  
            269, 140  
          ]}  
        ]  
    });
```
    将所有的图形数据整合后，就形成我们的TPLINK面板的数据，整合方法如下：
```javascript;gutter
ht.Default.setImage('tplink', {  
        width: 97,  
        height: 106,  
        comps: [  
            {  
          type: "shape",  
          background: "rgb(20,60,140)",  
          borderWidth: 8,  
          borderColor: "gray",  
          borderCap: "round",  
          points: [  
            269, 140,  
            359, 140,  
            359, 180,  
            329, 180,  
            329, 190,  
            299, 190,  
            299, 180,  
            269, 180,  
            269, 140  
          ]},  
           {  
          type: "circle",  
          shadow: true,  
          background: "rgb(0,0,0)",  
          borderWidth: 1,  
          borderColor: "rgb(0,0,0)",  
          border3d: true,  
          gradient: "spread.horizontal",  
          gradientColor: "rgb(79,77,77)",  
          dashColor: "rgb(0,0,0)",  
          rotation: 3,  
          rect: [  
            677, 157,  
            43, 34  
          ]},  
          //...  
          //...  
          //...  
          //多个图形组件  
      ]  
    });
```
    这只是注册图片的其中一种方法，我们也可以直接通过url的方式进行注册（详情见[HT for Web入门手册](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)）：　
```javascript;gutter
ht.Default.setImage('tplink', 'symbols/TPLink.json');
```
    将注册的矢量图片名称设置到模型上：
```javascript;gutter
var node = new ht.Node(),  
    node.setImage('tplink');  
    dataModel.add(node);
```
    甚至在最新版的[HT](http://www.hightopo.com/)中，已经支持无需注册，直接调用setImage()，传入URL参数的方式（在我的Demo中就是使用的这种方法）。这种方法更加简洁，但是如果许多场景都应用到同一图片时，还是建议用户通过注册的图片的，避免多次修改URL：
```javascript;gutter
node.setImage('symbols/TPLink.json');
```
    好了，现在在浏览器中预览你的HTML文档，是不是有个TPLINK面板？
    最后，怎么让我们的指示灯闪烁起来呢？用[HT](http://www.hightopo.com/)开发的产品，要实现闪烁效果很简单，因为[HT](http://www.hightopo.com/)预定于图形组件默认就已与DataModel中的Data数据绑定，绑定的格式也很简单，只需将以前的参数值用一个带func属性的对象替换即可，详见[HT for Web数据绑定手册](http://www.hightopo.com/guide/guide/core/databinding/ht-databinding-guide.html)（[http://www.hightopo.com/guide/guide/core/databinding/ht-databinding-guide.html](http://www.hightopo.com/guide/guide/core/databinding/ht-databinding-guide.html)）。在这里指示灯的闪烁实际上是visible属性值变化产生的结果，所以我们只需要给visible属性数据绑定，如下所示：
```javascript;gutter
{  
          "type": "oval",  
          "visible": {  
            "func": "attr@visibility1"  
          },  
          "shadow": true,  
          "shadowColor": "rgba(208,240,2,0.35)",  
          "background": "rgb(178,184,141)",  
          "gradient": "radial.center",  
          "gradientColor": "rgb(247,255,0)",  
          "rect": [  
            79, 53,  
            31, 32  
          ]  
     },
```
```javascript;gutter
setInterval(function(){  
                    node.a('visibility1', !t_node.a('visibility1'));  
}, 400);
```
    到这里，你已经成功完成一个TPLINK面板的制作 (~ . ~)，当然还剩服务器的制作，这里就不再赘述，复杂TPLINK面板都完成了，服务器还远吗？
2、连线
　　大家也有注意到，我们的Demo中有两条连线，那连线应该怎么做呢？
[HT](http://www.hightopo.com/)默认提供的是直线和多点连线，但是在绘制流程图、组织结构图和思维导图等应用还需要更多的连线类型，  详情戳[HT for Web连线类型手册](http://www.hightopo.com/guide/guide/plugin/edgetype/ht-edgetype-guide.html)（[http://www.hightopo.com/guide/guide/plugin/edgetype/ht-edgetype-guide.html](http://www.hightopo.com/guide/guide/plugin/edgetype/ht-edgetype-guide.html)）。
![](https://images2015.cnblogs.com/blog/591709/201705/591709-20170518113523291-1711481716.png)
　　在我们的Demo中，两条连接服务器和TP-LINK的曲线，均是使用自定义的新连线类型。
　　ht.Default.setEdgeType(type, func, mutual)函数可以用来自定义连线类型：
　　其中：
      　　type：字符串类型的连线类型，对应style的edge.type属性；
　　　　fuc：函数类型，根据传入参数(edge, gap, graphView, sameSourceWithFirstEdge)返回走线的走向信息；
                edge：当前连线对象；
                gap：多条连线成捆时，笨连线对象对应中心连线的间距；
                graphView：当前对应的拓扑组件对象；
                sameSourceWithFirstEdge：boolean类型，该连线是否与同组的同一条连线同源；
                返回值为{points：new ht.List(...)，segments：new ht.List(...)}结构的连线走向信息，segments的取值同上；
　　　　mutual：该参数决定连线是否影响起始或者结束节点上的所有连线，默认为false代表只影响同source和target的EdgeGroup中的连线。
　　具体实现时，我们需要再引入：
```javascript;gutter
<script src='ht-edgetype.js'></script>
```
　　然后调用ht.Default.setEdgeType(type, func, mutual)函数，代码如下：
```javascript;gutter
ht.Default.setEdgeType('line', function(edge){  
                        var sourcePoint = edge.getSourceAgent().getPosition(),  
                            targetPoint = edge.getTargetAgent().getPosition(),  
                            points = new ht.List();         
                            points.add(sourcePoint);  
                            points.add({  
                                x: (sourcePoint.x + targetPoint.x)/2,   
                                y: (sourcePoint.y + targetPoint.y)/2 + 300  
                            });                    
                            points.add(targetPoint);                                                         
      
                        return {  
                            points: points,  
                            segments: new ht.List([1, 3])  
                        };                   
    });
```
　　创建一条新的连线时，注意这时候连线类型edge.type为我们自定义的连线类型‘line’：
```javascript;gutter
var edge = new ht.Edge();  
    edge.setSource(startNode);  
    edge.setTarget(endNode);  
    edge.setLayer('edgeLayer');  
    edge.s({  
           'edge.type': 'line',  
           'edge.color': '#0A3791',  
           'edge.width': 8,  
           'edge.center': true  
           });  
    dataModel.add(edge);
```
　　到这里连线已经基本完成，还有一点，大家可能对setLayer()方法不是很熟悉，其实这个方法是用于设置连线和图元的层级，因为默认的层级是edge在node之下，所以需要设置层级后，调用graphView的setLayers方法更改层级之间的关系：　
```javascript;gutter
graphView.setLayers(['nodeLayer', 'edgeLayer']);
```
　　若对自定义连线类型仍旧有疑问，可以戳[例子](http://www.hightopo.com/guide/guide/plugin/edgetype/examples/example_custom.html)（[http://www.hightopo.com/guide/guide/plugin/edgetype/examples/example_custom.html](http://www.hightopo.com/guide/guide/plugin/edgetype/examples/example_custom.html)）加深了解。
3、流动
　　先来看看[HT](http://www.hightopo.com/)产品中流动的炫酷效果（戳[地址](http://www.hightopo.com/guide/guide/plugin/flow/ht-flow-guide.html)（[http://www.hightopo.com/guide/guide/plugin/flow/examples/example_demo1.html](http://www.hightopo.com/guide/guide/plugin/flow/examples/example_demo1.html)）可看详情）：
![](https://images2015.cnblogs.com/blog/591709/201705/591709-20170518102800119-1539119774.gif)
　　在我的Demo中两条连线应用了不同方式的流动，但是两种方式需要ht.flow插件。这个插件在ht.Shape和ht.Edge类型上扩展了样式控制流动效果，用户可以通过ht.Shape.setStyle()和ht.Edge.setStyle()来操作这些样式，下面简单介绍几种样式：
  　　1、flow值为true和false，控制此ht.Shape和ht.Edge是否可流动，默认为false；
  　　2、flow.count，控制流动组的个数，默认为1；
  　　3、flow.step，控制流动的步进，默认为3；
  　　4、flow.element.image，字符串类型，指定流动组元素的图片，图片须提前通过ht.Default.setImage()注册；
   　　....
 　　等等，还有很多的样式任你玩，详情戳[地址](http://www.hightopo.com/guide/guide/plugin/flow/ht-flow-guide.html)（[http://www.hightopo.com/guide/guide/plugin/flow/ht-flow-guide.html](http://www.hightopo.com/guide/guide/plugin/flow/ht-flow-guide.html)）；
　　 这里必须要引入流动特效插件：
```javascript;gutter
<script src="js/ht-flow.js"></script>
```
　　在这里，我们先将流动的图片提前注册：
```javascript;gutter
ht.Default.setImage('arrow', 'symbols/arrow.json');
```
　　第一种方式中，直接在连线edge上设置流动相关的属性（做完后别忘了调用启动流动的API），在这里通过设置flow.element.image属性值为'arrow'的方式设置流动的图片：
```javascript;gutter
edge.setStyle({  
        'edge.type': 'line',  
        'edge.color': '#0A3791',  
        'edge.width': 8,  
        'edge.center': true,  
        'flow': true,  
        'flow.element.image': 'arrow',  
        'flow.element.count': 1,   
        'flow.element.max': 30,                                                 
        'flow.element.autorotate': true                         
    });  
    raphView.enableFlow(40);//启动流动；
```
　　刷新页面，arrow在edge上流动起来了！可能还有人会疑问“如果我的流动组元素不是图片，是图元呢？”，没错，这就是第二种方式！
　　第二种方式，针对的是流动元素组是图元的情况：
```javascript;gutter
var flowNode = new ht.Node();                   
    flowNode.setImage('arrow');
```
　　因为流动实际上是图元的位置随着时间发生了变化，所以，我们可以更改图元的位置来控制它的流动，通过调用flow插件现成的API- - -calculateLength计算出流动线的长度length，然后改变当前步进百分比currentPercentage，具体实现如下：
```javascript;gutter
graphView.validate();//刷新；  
    var length = graphView.calculateLength(edge),//流动线长度；  
        step = 4, //步进单位像素；  
        stepPercentage = step / length * 100, // 步进百分比；  
        currentPercentage = 0; //当前步进百分比；  
      
    setInterval(function(){  
        var pos = graphView.getPercentPosition(edge, currentPercentage);//第二个参数为百分比，范围0到100;  
        flowNode.setPosition(pos.x, pos.y);//改变流动节点的位置；  
        currentPercentage += stepPercentage;  
        if (currentPercentage > 100) currentPercentage = 0;  
    }, 400);
```
　　做完这些之后，刷新页面，怎么仍旧没有流动效果？
　　其实这里有一个坑，那就是在计算length之前，必须先调用graphView.validate()，为什么呢？为了提高效率，graphView并不是实时刷新，而是多个图元发生改变后统一刷新，所以这里的graphView.validate()的功能是进行刷新graphView.
　　最后，附上Demo的源码（[http://www.hightopo.com/demo/blog_tplink_20170511/index.html](http://www.hightopo.com/demo/blog_tplink_20170511/index.html)），希望大家不吝赐教。
