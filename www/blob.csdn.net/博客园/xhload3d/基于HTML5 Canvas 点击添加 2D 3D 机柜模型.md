# 基于HTML5 Canvas 点击添加 2D 3D 机柜模型 - xhload3d - 博客园
今天又返回好好地消化了一下我们的数据容器 DataModel，这里给新手做一个典型的数据模型事件处理的例子作为参考。这个例子看起来很简单，实际上结合了数据模型中非常重要的三个事件处理的部分：属性变化事件监听、选中变化事件监听以及数据模型变化事件监听。
为了让这个例子具现化，我将这个简单的例子做了一点改动，下面我会一一解释。
例子地址：[http://hightopo.com/guide/guide/core/datamodel/examples/example_datamodel.html](http://hightopo.com/guide/guide/core/datamodel/examples/example_datamodel.html)
![](https://images2018.cnblogs.com/blog/591709/201711/591709-20171123224347312-1201125123.png)
这是我改造之后的模样，将 dataModel 数据容器共享，通过对数据容器的增删事件的监听得到的现在的结果，并且在显示上做了一点“手脚”。下面我们从头解析这例子，你们会知道为什么我特地将这个简单的例子提出来。
首先，我们得创建场景将作为基础，整个场景我算是分为三个部分，顶部工具栏，2D 部分以及 3D 部分。顶部工具栏部分使用的纯 HTML 写的：
```
<button onclick="addData()">Add</button>
<button onclick="removeData()">Remove</button>
<button onclick="clearDataModel()">Clear</button>
<span id="property" class="output"></span>
<span id="model" class="output"></span>
<span id="selection" class="output"></span>
```
因为有点击事件，所以我们直接在 button 按钮上进行，后面的 span 标签显示纯文本内容。
我们知道，HT 的所有组件都是基于一个根部 div 的，要将这个 div 部署到 html 页面上很简单，但是 HT 内部对这个 div 设置了绝对定位，所以我们在添加这个 div 进 HTML 页面中时，也要设置绝对定位中的位置，我在页面中添加了一个 div，将 HT 的部分都添加进这个 div 中：
```
<div id="myDiv" style="border: 1px solid red; width: 800px; height: 600px;position: absolute; "></div>
```
```
dataModel = new ht.DataModel();
g3d = new ht.graph3d.Graph3dView(dataModel);
g3d.setGridVisible(true);//设置网格可见
g3d.setEye(185, 50, 470);//设置3d的眼睛位置
g3d.setCenter(200, 47, 10);//设置3d的中心位置， 这两个属性都是为了让用户看3d上的场景更舒服，更直接
g2d = new ht.graph.GraphView(dataModel);
g2d.setEditable(true);//设置2d图元可编辑
g2d.fitContent(true);//将所有的图元显示到页面上
splitView = new ht.widget.SplitView(g2d, g3d, 'v', 0.3);//分割组件，装了2d和3d两个场景
splitView.addToDOM();//将分割组件添加进body中，并设置绝对定位的位置
myDiv = document.getElementById('myDiv');
myDiv.appendChild(splitView.getView());//将分割组件添加进myDiv中
```
接着添加节点进入 dataModel 数据模型之中，我们这里做的是机房的机柜，本来想做的是服务器，手头上暂时只有这个资源，也不赖。我封装了一个增加函数，一个删除函数，还有一个清楚函数，分别对应的是工具栏上的“Add”、“Remove”以及“Clear”三个功能：
```
function addData() {
    var data = new ht.Node();
    data.setPosition(index*60, 50);
    data.setName('node'+index);
    data.setSize(40, 40);
    data.setImage('cabinet');
    data.s({
        'image.stretch': 'centerUniform',
    'shape3d': 'cabinet'
    });
    index++;
    dataModel.add(data);
    return data;
}
function removeData() {
    if(!dataModel.sm().ld()) return;
    dataModel.remove(dataModel.sm().ld());
}
function clearDataModel(){
    dataModel.clear();
    index = 0;
}
```
其中，代码中出现的“data.setImage('cabinet')”，是我通过 ht.Default.setImage('cabinet', 'imageURL') 方式定义的，调用的时候直接 data.setImage('imageName') 即可，具体参考 [HT for Web 入门手册](http://hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html) image 章节。
2D 的图片显示肯定和 3D 的模型显示是不一样的，2D 中我们直接用贴图就能解决，而 HT 3D 中支持 obj 格式的模型显示，就是这个部分：
![](https://images2018.cnblogs.com/blog/591709/201711/591709-20171124083435031-1020417645.png)
HT 封装了解析 obj 格式的函数 ht.Default.loadObj 函数用来导入模型，该函数有三个参数，第一第二分别为 obj 文件的路径和 mtl 文件的路径，第三个参数为 json 格式控制参数，具体参数请参考 HT for Web OBJ 手册 loadObj 函数章节（ps：用 obj 模型会导致跨域问题，要放到服务器上运行）：
```
ht.Default.loadObj('obj/机柜组件1.obj', 'obj/机柜组件1.mtl', {
     cube: true,//是否将模型缩放到单位1的尺寸范围内，默认为false
     center: true,//模型是否居中
     prefix: 'obj/',//路径前缀，如果前面参数写了路径前缀，这个不写也可以
     shape3d: 'cabinet',//指定 shape3d 名称
     finishFunc: function(modelMap, array, rawS3){//调用ht.Default.parseObj解析后的返回值，若加载或解析失败则返回值为空
         window.rawS3 = rawS3;//让当前模型的尺寸为原始尺寸
         if(modelMap){
             cabinet1 = addData();//添加两个节点到 dataModel 中
             cabinet2 = addData();
         }
    } 
});
```
现在，节点和模型都已经导入到场景中了，终于来到了我们今天的重点，事件交互部分。ht.DataModel 数据容器管理着 Data 数据的增删以及变化事件的派发，这里我们就这两种事件进行对 Data 数据的管理。
1. addDataModelChangeListener(function(e) {}, scope) 增加数据模型增删变化事件监听器，可用简写 mm(func, scope)， func 为监听器函数，scope 为监听器函数域（可选），在监听器函数中的 event 有两个属性： kind 和 data，其中 kind 为事件的类型：
- `e.kind === 'add'`代表添加`Data`对象，`e.data`为被添加的对象
- `e.kind === 'remove'`代表删除`Data`对象，`e.data`为被删除的对象
- `e.kind === 'clear'`代表容器被清除
这里我们将对模型的增删事件的监听结果传给 HTML 中的 id 为 model 的 span 作为内容：
```
var model = document.getElementById('model');
dataModel.addDataModelChangeListener(function(e) {
    if(e.kind === 'add')//如果事件类型为 add 增加节点
        model.innerHTML = e.data + ' added';//就将model 的内容替换为 添加的节点 added
    if(e.kind === 'remove')
        model.innerHTML = e.data + ' removed';
    if(e.kind === 'clear')
        model.innerHTML = 'dataModel cleared'
});
```
2. addDataPropertyChangeListener(function(e) {}, scope) 增加模型中 Data 数据属性变化事件监听器，可用简写 md(func, scope)，其中 event 事件有四种属性：
- `e.data`代表属性变化的对象
- `e.property`代表变化属性的名字
- `e.newValue`代表属性的新值
- `e.oldValue`代表属性的老值
- `Data`对象在设置属性值函数内调用`firePropertyChange(property, oldValue, newValue)`触发属性变化事件：
- `get/set`类型属性，如`setAge(98)`触发事件的`e.property`为`age`
- `style`类型属性名前加`s:`前缀以区分，如`setStyle('age', 98)`触发事件的`e.property`为`s:age`
- `attr`类型属性名前加`a:`前缀以区分，如`setAttr('age', 98)`触发事件的`e.property`为`a:age`
这里我们将对模型中 Data 的属性变化事件的监听结果传给 HTML 中的 id 为 property 的 span 作为内容：
```
var model = document.getElementById('model');
dataModel.addDataPropertyChangeListener(function(e) {
    property.innerHTML = e.data + '\'s ' + e.property + ' has changed, the old value is ' + e.oldValue + ' and the new value is ' + e.newValue; 
});
```
3. 最后，我们对选中的节点进行增加监听器，监听选中变化事件。ht.SelectionModel管理 DataModel 模型中 Data 对象的选择状态， 每个 DataModel 对象都内置一个 SelectionModel 选择模型，控制这个 SelectionModel 即可控制所有绑定该 DataModel 的组件的对象选择状态， 这意味着共享同一 DataModel 的组件默认就具有选中联动功能。
如果希望某些组件不与其他组件选中联动，可通过调用 view.setSelectionModelShared(false)， 这样该 view 将创建一个专属的 SelectionModel 实例。
综上所述有两种途径可得到 SelectionModel:
```
```
- `dataModel.getSelectionModel()`获取数据容器中组件共享的选中模型。
- `view.getSelectionModel()`获取当前组件使用的选中模型，`selectionModelShared`为`false`时，返回`view`专用的选择模型。
addSelectionChangeListener(function(e) {}, scope)增加监听器，监听选中变化事件，简写为 ms(func, scope)：
- `e.datas`包含所有选中状态变化的对象，之前选中现在取消选中，或之前没选中现在被选中的对象
- `e.kind === 'set'`代表此事件由`setSelection(datas)`引发
- `e.kind === 'remove'`代表此事件由`removeSelection(datas)`引发
- `e.kind === 'append'`代表此事件由`appendSelection(datas)`引发
- `e.kind === 'clear'`代表此事件由`clearSelection(datas)`引发
这里我们将对模型中 Data 的选中变化事件的监听结果传给 HTML 中的 id 为 selection 的 span 作为内容：
```
var selection = document.getElementById('selection');
dataModel.sm().addSelectionChangeListener(function(e){
    if(dataModel.sm().size() === 0) selection.innerHTML = 'Nothing selected';//如果选中模型的“长度”为0，即没有选中内容
    else if(dataModel.sm().size() === 1) selection.innerHTML = e.datas + ' selected';
    else selection.innerHTML = dataModel.sm().size() + ' datas selected';
});
```
以上，所有的代码全部分析完毕！大家可以天马行空，创建出属于你自己的3维模型！
