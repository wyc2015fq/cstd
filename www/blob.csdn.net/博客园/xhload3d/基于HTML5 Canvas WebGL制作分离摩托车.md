# 基于HTML5 Canvas WebGL制作分离摩托车 - xhload3d - 博客园
工业方面制作图表，制作模型方面运用到 3d 模型是非常多的，在一个大的环境中，构建无数个相同的或者不同的模型，构建起来对于程序员来说也是一件相当头疼的事情，我们利用 HT 帮大家解决了很大的难题，无数个例子可在官网上查找到[http://hightopo.com/demos/index.html](http://hightopo.com/demos/index.html)
本文 Demo 地址： [http://hightopo.com/guide/guide/plugin/obj/examples/example_objajax.html](http://hightopo.com/guide/guide/plugin/obj/examples/example_objajax.html)
这次我们的例子是将这些实例中的一小部分思想抽取出来，作为给大家的分析，看看这次实现的例子效果图：
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171105121544857-1492702055.png)
这个例子用了 HT 中的[树组件](http://hightopo.com/guide/guide/core/treeview/ht-treeview-guide.html) ht.widget.TreeView 和 HT 中加载 OBJ 格式文件的 [ht.Default.loadObj](http://hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html#ref_loadobj) 函数来加载图中的两辆摩托车，我们利用代码来从头开始解析这个例子的部分。
首先观察这个例子的界面展示，会发现这个界面是由左右两部分组成的，右边又是由上下两部分组成的，这种分割模式在 HT 中有很好的解决办法，可以完全利用 [ht.widget.SplitView](http://hightopo.com/guide/guide/core/splitview/ht-splitview-guide.html) 将界面分层，也可以利用 ht.widget.BorderPane 将界面分成上面、中间和下面。这里我们因为是示例，所以将两种方法都用上了：
```
1 dataModel = new ht.DataModel();//数据模型                
2 g3d = new ht.graph3d.Graph3dView(dataModel);//3d组件
3 toolbar = new ht.widget.Toolbar(item); //工具条
4 borderPane = new ht.widget.BorderPane();//面板组件
5 borderPane.setTopView(toolbar);//toolbar 放在上部
6 borderPane.setCenterView(g3d);//g3d 放在中间部分
7 treeView = new ht.widget.TreeView(dataModel); //树组件
8 mainSplit = new ht.widget.SplitView(treeView, borderPane, 'h', 0.2); //分割组件
```
其中 toolbar 中的 item 也是遵守 HT 设置规则的，item 是一个数组，数组中每一个元素都是toolbar上的一部分，这个例子中toolbar只有2个元素，也具有足够的代表性：
```
1 item = [
 2     {
 3         label: 'Editable',
 4         type: 'check',
 5         action: function(){
 6             g3d.setEditable(this.selected);
 7         }
 8     },
 9     {
10         id: 'size',
11         label: 'Size',
12         slider: {
13             width: 120,
14             min: 1,
15             max: 60,
16             value: 1,
17             thickness: 1,
18             onValueChanged: function(){
19             if(rawS3){
20                 var value = this.getValue();
21                 dataModel.each(function(data){
22                 if(data instanceof ht.Node){
23                     data.s3(rawS3[0] * value, rawS3[1] * value, rawS3[2] * value); 
24                     data.s({
25                         'note.scale': value/20,
26                         'note.t3': [0, -value, value]
27                     });                                            
28                 }                                        
29             });                                    
30         }
31     }
32 ];
```
我们可以通过设置 toolbar 中的 item 元素来设置对象格式类型，其中 type 可以设置为 check、toggle 和 radio，分别表示复选框、开关按钮和单选按钮，这里我们将是否可编辑 Editable 设置为复选框，可以通过控制这个元素来设置是否可编辑，后面的拉条也是 HT 中封装的 ht.Slider 滑动条来绑定 OBJ 对象的 size 大小，通过控制滑动条来控制 OBJ 的大小，详情请参考 [HT for Web 工具条组件手册](http://hightopo.com/guide/guide/core/toolbar/ht-toolbar-guide.html)。
接着要将最外层的组件添加进底层 div 中，没有阅读过我的文章的同学这边我解释一下，所有的 HT 组件最根层都是一个 div 组件，可通过组件的 getView 函数获得，默认和自定义交互时间监听一般添加在该 div 上（getView().addEventListener(type, func, false)），渲染层一般由 canvas 提供，用户可直接对根 div 和 canvas 层设置 css 样式，也可以添加新的 HTML 组件到根 div 上，作为 canvas 的兄弟组件一起呈现。详情请参考 [HT for Web 入门手册](http://hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)。
接着通过利用 ht.widget.loadObj 函数将 OBJ 格式文件导入模型：
```
1 ht.Default.loadObj('obj/scooter.obj', 'obj/scooter.mtl', {//左边不可分割的摩托 
 2     cube: true,
 3     center: true,
 4     shape3d: 'scooter',
 5     finishFunc: function(modelMap, array, rawS3){
 6         window.rawS3 = rawS3;
 7         if(modelMap){
 8             var node = new ht.Node();
 9             node.setName('All in ONE');
10             node.s({
11                  'shape3d': 'scooter',
12                  'wf.visible': 'selected',
13                  'note': 'One Node',
14                  'note.face': 'center',
15                  'note.position': 7,
16                  'note.background': 'blue',
17                  'note.autorotate': 'y' 
18             });
19             node.s3(rawS3);
20             node.p3(-300, 0, 0);
21             dataModel.add(node);
22         }
23         checkLoaded();
24     }
25 });
```
ht.Default.loadObj 函数有三个参数，objUrl OBJ 文件路径，mtlUrl MTL 文件路径，params JSON 结构参数，parmas 参数可以设置 ht.Default.parseObj(text, mtlMap, params) 第三个参数的控制信息，也就是说 ht.Default.parseObj 函数中的第三个参数所带的控制信息在 ht.Default.loadObj 函数中的第三个参数 params 中都可以使用，并且添加了 sync 和 finishFunc 参数，finishFUnc 参数是用于加载后的回调处理的函数，带有参数 modelMap、array 和 rawS3 ，可以在 finishFunc 中做任何事情！详情请参考 [HT for Web OBJ 手册](http://hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html#ref_loadobj)。
本例中有两个 motor 摩托车模型，一个是整体的模型，不能拆分，一个是可以拆分成部分的模型，接下来就来看看如何将 OBJ 文件中的模型拆分开来：
```
1 ht.Default.loadObj('obj/scooter.obj', 'obj/scooter.mtl', {//右边可分割的摩托             
 2 
 3     cube: true,
 4     center: true,
 5     finishFunc: function(modelMap, array, rawS3){
 6         if(modelMap){
 7             var lastNode = null,
 8             firstNode = null,
 9             parentNode = new ht.Data();    
10                             
11             parentNode.setName('Separate Scooter');
12             dataModel.add(parentNode);                                                
13 
14             for(var name in modelMap){                    
15                 var model = modelMap[name];   
16                 var shape3d = 'scooter:' + name;
17                 ht.Default.setShape3dModel(shape3d, model);                
18 
19                 var node = new ht.Node();
20                 node.setName(name);
21                 node.setParent(parentNode);
22                 node.s({
23                     'shape3d': shape3d,
24                     'wf.visible': 'selected'
25                 });
26                 node.setHost(lastNode);
27                 lastNode = node;
28                 if(!firstNode){
29                     firstNode = node;
30                 }
31                 node.s3(rawS3); 
32                 dataModel.add(node);
33             }
34             if(lastNode){
35                 firstNode.setHost(lastNode);
36                 firstNode.p3(300, 0, 0);
37                 firstNode.s({
38                     'note': 'A lot of Nodes host together',
39                     'note.face': 'center',
40                     'note.position': 7,
41                     'note.background': 'blue'
42                 });                                
43             }                             
44         } 
45         checkLoaded();
46     }
47 });
```
我们可以通过 modelMap 获得通过 ht.Default.parseObj 函数解析后的返回值，这个值通常就是 obj 格式的文件解析后返回的 map 结构的 json 对象，每一个材质都对应一个模型信息，详情请参考 [HT for Web OBJ 手册](http://hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html#ref_parseobj)。
 我们利用 ht.Default.parseObj 函数解析获得每个材质的模型信息，通过遍历整个模型，获得单独的模型信息，并且将其命名，这样我们就能显示每个模型的名称了，也能对每个部分的模型进行控制。
上面代码中 45 行出现的 checkLoaded 函数是方便控制树组件的展开合并的函数，我们在工业等各个领域中，用到“树”的概念是非常多的，所以这个例子也运用到用作解释：
```
1 function checkLoaded(){
 2     loadTask--;
 3     if(loadTask === 0){
 4         treeView.expandAll();
 5         treeView.selectAll();
 6 
 7         ht.Default.startAnim({
 8             action: function(t){
 9                 toolbar.v('size', 50*t);
10             }
11         });                     
12     }
13 }
```
因为“树”的部分我们只用了两个主节点，All in one 和 Seperate Scooter，我们定义 loadTask 变量值为 2，上面代码的意思是如果两个模型都加载完毕，那么就将树组件 treeView 展开，并且全部选中，再用代码控制 toolbar 中的值为 50*t 来调整模型的大小。
对于树组件 treeView，HT 封装了很多帮助组件，使得开发速度更快，比如在这个例子中用到的 setSortFunc 函数，是用来设置排序的；setCheckMode 函数是用来设置 check 模式的，大家一看到 check 就会想到多选框，在这里 HT 确实是通过设置 setCheckMode 函数来设置多选框，这个函数的参数可为：
- null：默认值，不启用check选择模式
- default：check模式的默认选择方式，即单击选中或取消选中，只影响当前点击中的data对象
- children：该check模式将同时影响点击中的data对象，以及其孩子对象
- descendant：该check模式将同时影响点击中的data对象，以及其所有子孙对象
- all：该check模式将同时影响点击中的data对象，以及其所有父辈和子孙对象
如果还是没有理解，大家可以试试将这个函数注释掉，你就能很清楚地明白它是做什么用的了。或者查看 [HT for Web 树组件手册](http://hightopo.com/guide/guide/core/treeview/ht-treeview-guide.html)。
以上就是今天的例子，实际开发中的例子会更复杂，逻辑也会更多，但通过 HT 来做，什么事情都变得 so easy！
