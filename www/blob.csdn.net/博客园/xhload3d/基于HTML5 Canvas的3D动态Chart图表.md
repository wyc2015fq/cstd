# 基于HTML5 Canvas的3D动态Chart图表 - xhload3d - 博客园
发现现在工业SCADA上或者电信网管方面用图表的特别多，虽然绝大部分人在图表制作方面用的是echarts，他确实好用，但是有些时候我们不能调用别的插件，这个时候就得自己写这些美丽的图表了，然而图表轻易做不成美丽的。。。看到有一个网站上在卖的图表，感觉挺好看的，就用 HT for Web 3D 做了一个小例子，挺简单的，也挺好看的，哈哈~
Demo 地址： [http://www.hightopo.com/demo/3DBar/index.html](http://www.hightopo.com/demo/3DBar/index.html)[](http://www.hightopo.com/demo/WireframeAnim/index.html)
动态效果图如下：
![](http://img.blog.csdn.net/20171101182139997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGhsb2FkM2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个例子用 HT 实现真的很容易，首先创建一个 HT 中最基础的 dm 数据模型，然后将数据模型添加进 g3d 3d 组件中，再设置 3d 中的视角并把 3d 组件添加进 body 元素中：
```
1 dm = new ht.DataModel();
2 g3d = new ht.graph3d.Graph3dView(dm);
3 g3d.setEye(0, 185, 300);
4 g3d.addToDOM();
5 g3d.getView().style.background = '#000';
```
接着就是造这五个chart图表条了，我的思路是这样的，里层有一个节点，外层一个透明的节点，底部一个 3d 的文字显示当前的百分比。
里层的节点非常容易，我直接用的 HT 封装的 ht.Node 创建了一个新的节点对象，然后通过 node.s 方法来设置 node 节点的样式：
```
1 var node = new ht.Node();
 2 node.s({
 3     'shape3d': cylinderModel,
 4     'shape3d.color': color,
 5     '3d.movable': false
 6 });
 7 node.a({
 8     'myHeight': s3[1],
 9 });
10 node.p3([p3[0], s3[1]/2, p3[2]]);
11 node.s3(s3);
12 dm.add(node);
```
其中要说明的是，'shape3d':cylinderModel 这个样式的设置，首先，shape3d 属性指定显示为 3d 模型的图标效果，cylinderModel 是用 HT 自定义的一个 3d 模型，可参考 [HT for Web 建模手册](http://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html#ref_createCylinderModel)：
```
1 cylinderModel = ht.Default.createCylinderModel(1000, 0, 1000, false, false, true, true);
```
然后设置了一个动态变化的属性 myHeight，在 HT 中，node.a 方法是预留给用户存储业务数据的，我们可以在这边添加任意多个属性。
接下来我们要创建的是外部的透明节点，这个节点的构造方式基本上与内部节点相同，就是多了一点“透明”的样式设置：
```
1 var cNode = new ht.Node();
 2 cNode.s({
 3     'shape3d': cylinderModel,
 4     'shape3d.transparent': true,
 5     'shape3d.opacity': 0.2,    
 6     'label.color': '#fff',
 7     '3d.movable': false
 8 });
 9 cNode.p3([p3[0], 50, p3[2]]);
10 cNode.s3(20, 100, 20);
11 dm.add(cNode);
```
要先设置 ‘shape3d.transparent’ 为true，再设置 ‘shape3d.opacity’ 透明度。
最后是 3d 文字，呈现 3d 文字需要一个 json 格式的 typeface 字体，然后通过 ht.Default.loadFontFace 来加载 json 格式的字体到内存中，详情请参考 [HT for Web 3D 手册](http://hightopo.com/guide/guide/core/3d/ht-3d-guide.html#ref_text)：
```
1 ht.Default.loadFontFace('./wenquanyi.json', function(){
 2     //......
 3     var text = new ht.Node();
 4     text.s3([5, 5, 5]);
 5     text.p3(cNode.p3()[0]-5, -10, 0);
 6     dm.add(text);
 7     text.s({
 8         'shape3d' : 'text',
 9     'shape3d.text': node.a('myHeight')+'%',
10     'shape3d.text.curveSegments': 1,
11     '3d.movable': false
12     });
13 });
```
因为我们用的 typeface 字体的绘制方式是无数个三角形构成的一个字，会占用很大的内存，所以我把图形的曲线的精细度调得较低，但是还是很清晰，如果你们能找到性能更好的字体，可以使用并且告知我一下，我们目前没找到占用内存小的字体。
最后，要动态的变化 chart 图表中的柱形图，我们得设置动画，并且将 3d 字体也同步更新数值：
```
1 setInterval(function(){
 2     if(node.a('myHeight') < 100){
 3         node.a('myHeight', (node.getAttr('myHeight')+1));
 4     node.s3(20, node.a('myHeight'), 20);
 5     node.p3(p3[0], node.a('myHeight')/2, p3[2]);
 6     }else{
 7         node.a('myHeight', 0);
 8     node.s3([20, 0, 20]);
 9     node.p3([p3[0], 0, p3[2]]);
10     }
11     if (text) text.s('shape3d.text', node.a('myHeight')+'%');
12 }, 100);
```
这里，我自定义的属性 “myHeight” 就起到了决定性的作用，我用这个属性来存储变量，而且可以任意更改变量的值，这样就能实现动态绑定的效果了。
还有不懂的可以留言，或者直接去我们官网上查看手册 [HT for Web](http://hightopo.com/)，有更多你想不到的效果能快速实现哦~ 
