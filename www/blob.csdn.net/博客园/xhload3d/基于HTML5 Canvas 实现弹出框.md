# 基于HTML5 Canvas 实现弹出框 - xhload3d - 博客园
　　用户鼠标移入时，有弹出框出现，这样的需求很常见。这在处理HTML元素实现时简单，但是如果是对 HTML5 Canvas 构成的图形进行处理，这种方法不再适用，因为 Canvas 使用的是另外一套机制，无论在 Canvas 上绘制多少图形，Canvas 都是一个整体。而图形本身实际都是 Canvas 的一部分，不可单独获取，所以也就无法直接给某个图形增加 JavaScript 事件。然而，在 [HT for Web](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html) 中，这种需求很容易实现，先附上[Demo](http://www.hightopo.com/demo/blog_meadow_20170605/index.html)：[http://www.hightopo.com/demo/blog_meadow_20170605/index.html](http://www.hightopo.com/demo/blog_meadow_20170605/index.html)
![](https://images2015.cnblogs.com/blog/591709/201706/591709-20170605092753809-1442393663.png)
　　这个场景图是基于 HT for Web 的 JSON 文件，可能大家对怎么生成这样的 JSON 文件有疑惑，其实这里是基于这个麻雀虽小五脏俱全的“[HTML5拓扑图编辑器](http://www.hightopo.com/demo/2deditor_20151010/HT-2D-Editor.html)”[（](http://www.hightopo.com/demo/2deditor_20151010/HT-2D-Editor.html)[http://www.hightopo.com/demo/2deditor_20151010/HT-2D-Editor.html](http://www.hightopo.com/demo/2deditor_20151010/HT-2D-Editor.html)）进行了扩展，很容易就自定义出满足我需求拓扑编辑器。不仅如此，在这个 Demo 中，定义的三种类型弹框的矢量图 ‘tips1.json’、‘tips2.json’、‘tips3.json’ 是通过这个[矢量编辑器](http://www.hightopo.com/demo/vector-editor/index.html)（[http://www.hightopo.com/demo/vector-editor/index.html](http://www.hightopo.com/demo/vector-editor/index.html)）简单绘制了下，也还蛮好用。在上述场景中，用户将鼠标移入到草地等对象时，会有弹出框显示它的详细信息。
具体实现如下：
**准备工作**
 　　引入我们的 [HT](http://www.hightopo.com/)（[http://www.hightopo.com/](http://www.hightopo.com/)）：
```javascript;gutter
<script src='ht.js'></script>
```
```javascript;gutter
dataModel = new ht.DataModel();
graphView = new ht.graph.GraphView(dataModel);
graphView.addToDOM();
```
[HT](http://www.hightopo.com/)提供了自定义的 JSON 格式的矢量描述形式，以 [HT](http://www.hightopo.com/) 标准定义的 JSON 矢量格式，也可以作为图片进行注册和使用， [HT](http://www.hightopo.com/) 的矢量方式比传统格式更节省空间，缩放不失真，戳 [HT for Web](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html) 了解详细信息。这里，将三种形状的 JSON 弹出框注册成图片以便后续调用：
```javascript;gutter
ht.Default.setImage('tips1', 'symbols/tips1.json');
ht.Default.setImage('tips2', 'symbols/tips2.json');
ht.Default.setImage('tips3', 'symbols/tips3.json');
```
　然后获取有交互效果的对象，其中各个对象中的属性名是给各个图元设置好的标签名：　
```javascript;gutter
//树
var tree = {
     'tree1' : true,
     'tree2' : true,
     'tree3' : true
};
//草地
var grass = {
     'grass1' : true,
     'grass2' : true,
     'grass3' : true
 };
            
//山
var mountain = {
    'mountain': true
};
```
**弹出框**
    其实弹出框的本质是一个 Node，当用户鼠标移入移出时，
1、控制 Node 的隐藏和显示可以达到弹框的效果；
2、鼠标位置的改变伴随着 Node 位置的改变；
3、鼠标移入到不同的对象上时， Node 上的贴图也跟着发生变化；
4、Node 中的属性值也随着鼠标位置发生变化。
    所以，要实现弹框，首先应新建 Node，并将其的层级设置为‘higher’，在这之前还需要将场景图的 JSON 文件反序列化，并且给反序列化后的图元均设置为层级‘lower’，防止被已有的图元挡住：
```javascript;gutter
ht.Default.xhrLoad('meadow.json', function(text) {
    const json = ht.Default.parse(text);                    
    if(json.title) document.title = json.title;
    dataModel.deserialize(json);
    //设置层级
    dataModel.each(function(data){
        data.setLayer('lower');
    });
    //新建node
    var node = new ht.Node();                    
    node.s('2d.visible',false);
    node.setLayer('higher');
    dataModel.add(node);
})
```
  然后，对底层的 DIV 监听 mousemove 事件，判断鼠标的位置是否在上述三个对象之上，根据对象类型，调用 layout() 函数对 Node 重新布局：
```javascript;gutter
graphView.getView().addEventListener('mousemove', function(e) {
     node.s('2d.visible',false);
     var hoverData = graphView.getDataAt(e);
     pos = graphView.getLogicalPoint(e);
     if(!hoverData) return; 
     if(tree[hoverData.getTag()]){ 
        layout(node, pos, 'tips1');
     } else if (grass[hoverData.getTag()]) {
        layout(node, pos, 'tips2');
     } else if (mountain[hoverData.getTag()]) {
        layout(node, pos, 'tips3');
     }
});
```
　　layout()函数所做的事情，已经在前面详细的阐述，其中，弹框中属性值的更新是将 JSON 文件的的 text 属性进行数据绑定，绑定的格式很简单，只需将以前的参数值用一个带 func 属性的对象替换即可，func 的内容有一下几种类型：
1、function类型，直接调用该函数，并传入相关 Data 和 view 对象，由函数返回值决定参数值，即 func(data, view)；调用。
2、string类型：
      style@***开头，则返回 data.getStyle(***)值，其中***代表 style 的属性名。
      attr@***开头，则返回 data.getAttr(***)值，其中***代表 attr 的属性名。
      field@***开头，则返回 data.***值，其中***代表 attr 的属性名。
      如果不匹配以上几种情况，则直接将 string 类型作为 data 对象的函数名调用 data***(view)，返回值作为参数值。
    除了 func 属性外，还可以设置 value 属性作为默认值，如果对应的 func 取得的值为 undefined 或者 null 时，则会采用 value 属性定义的默认值，详情可见 [HT for Web数据绑定手册](http://www.hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html)（[http://www.hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html](http://www.hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html)）。例如，在这里，'tips1.json'文件中对阳光值进行数据绑定的结果如下：
```javascript;gutter
"text": {
        "func": "attr@sunshine",
        "value": "阳光值"
      },
```
　下面贴上 layout() 函数的源代码：
```javascript;gutter
function layout(node, pos, type){
                node.s('2d.visible',true);
                node.setImage(type);                   
                if(type == 'tips1'){
                    node.setPosition(pos.x + node.getWidth()/2, pos.y - node.getHeight()/2);
                    node.a({
                        'sunshine'  :   '阳光值   :     '+ (pos.x/1000).toFixed(2),
                        'rain'  :   '雨露值   :     '+ (pos.y/1000).toFixed(2),
                        'love'  :   '爱心值   ：    ***'
                    });
                } else if(type == 'tips2'){
                    node.setPosition(pos.x , pos.y - node.getHeight()/2);
                    node.a({
                        'temp'  :   '温度   :     30',
                        'humidity'  :   '湿度   :     '+Math.round(pos.x/100)+'%'
                    });
                } else if(type == 'tips3'){
                    node.setPosition(pos.x - node.getWidth()/2, pos.y - node.getHeight()/2);
                    node.a({
                        'hight'  :   '海拔   :    ' + Math.round(pos.y)+'米',
                        'landscapes'  :   '地貌   :    喀斯特'
                    });
                }
            }
```
**云移动**
    最后，我们的 Demo 还有个云移动的动画效果，在 HT 的数据模型驱动的图形组件的设计架构下，动画可理解为将某些属性由起始值逐渐变为目标值的过程， [HT](http://www.hightopo.com/) 提供了ht.Default.startAnim的动画函数，ht.Default.startAnim 支持 Frame-Based 和 Time-Based 两种方式的动画：
   Frame-Based 方式用户通过指定 frames 动画帧数，以及 interval 动画帧间隔参数控制动画效果；
   Time-Based 方式用户只需要指定 duration 的动画周期的毫秒数即可，HT 将在指定的时间周期内完成动画。
    详情见 [HT for Web。](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)
    在这里我们用的是 Time-Based 方式，源码如下：
```javascript;gutter
var cloud = dataModel.getDataByTag('cloud');
parent = dataModel.getDataByTag('mountain');
round1 = parent.getPosition().x - parent.getWidth()/2 + cloud.getWidth()/2;
round2 = parent.getPosition().x + parent.getWidth()/2 - cloud.getWidth()/2;
end = round1;
//云运动动画
var animParam = {
    duration: 10000,
    finishFunc: function() { 
         end = (end == round1) ? round2 : round1;
         ht.Default.startAnim(animParam);
    },
    action: function(v, t) {
         var p = cloud.getPosition();
         cloud.setPosition(p.x + (end - p.x) * v , p.y);
     }
};
ht.Default.startAnim(animParam);
```
　　最后，再次放上我们的 [Demo](http://www.hightopo.com/demo/blog_meadow_20170605/index.html) （[http://www.hightopo.com/demo/blog_meadow_20170605/index.html](http://www.hightopo.com/demo/blog_meadow_20170605/index.html)），供大家参考。
![](https://images2015.cnblogs.com/blog/591709/201706/591709-20170605093537856-1841068625.gif)
