# 基于HTML5 Canvas实现用户交互 - xhload3d - 博客园
　　很多人都有这样的疑问，基于HTML5 Canvas实现的元素怎么和用户进行交互？在这里我们用到[HT for Web](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)（[http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)）写了个Demo进行示例。
　　场景如下所示，在该场景中双击温度和湿度下的Node，会生成输入框供用户填写内容，这之后，用户按下“Enter”键可以将输入内容传到Node中，同时删除输入框，地址：：[http://www.hightopo.com/demo/GraphInput/display.html](http://www.hightopo.com/demo/GraphInput/display.html)
![](https://images2015.cnblogs.com/blog/591709/201705/591709-20170523112940038-2057551864.png)
接下来我们探讨一下具体实现：
准备工作如下：
```
<script src='ht.js'></script>
```
```
dataModel = new ht.DataModel();
graphView = new ht.graph.GraphView(dataModel);
graphView.addToDOM();
```
1、利用系统中定义好的矢量资源进行反序列化来实现场景图：
```
ht.Default.xhrLoad('TemperatureIndex.json', function(text) {
    var json = ht.Default.parse(text);                    
    if(json.title) document.title = json.title;
    dataModel.deserialize(json);
}
```
2、双击事件
　　本例双击会产生输入框，在我们的HT中，GraphView默认内置了一些交互器，以实现基本的选择、单双击、缩放、平移和编辑等交互的功能，内置的交互器有：
![](https://images2015.cnblogs.com/blog/591709/201705/591709-20170523113225663-175284877.png)
　　内置的Interactor在交互过程中会派发事件，可通过GraphView#addInteractorListener进行监听，简写为mi（详情可看[HT for Web 入门手册](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_graphviewinteraction)[http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_graphviewinteraction](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_graphviewinteraction)），在这里，我们用内置的graphView.addInteractorListener监听双击事件：
```
graphView.addInteractorListener(function(e){
    if (e.kind !== 'doubleClickData') return;
    if (currentInput) removeInput();
                        
    var data = e.data;
    if (clickableTags[data.getTag()]){
        setTimeout(function(){
            createInput('input', data);
        }, 0);
    } 
 });
```
3、创建输入框
　　在双击事件发生时，首先需要判断发生双击事件的元素是不是场景中定义的标签名‘temperature’和‘humidity’的node图元，我们用clickableTags对象来保存两个node：
```
var clickableTags = {
    'temperature': true,
    'humidity': true
}
```
　　在双击的图元是‘temperature’或者‘humidity’时，调用createInput()函数生成输入框，createInput()代码如下：
```
function createInput(tagName,node){
    if (currentInput) {
        removeInput(graphView, currentInput);
        return;
    } else {                    
        var element = document.createElement(tagName);                
        graphView.getView().appendChild(element);
        element.bindingNode = node;
        ht.Default.setFocus(element);
        currentInput = element;
        layout(currentInput);//布局
        return currentInput;
    }
}
```
　　在createInput()函数中，用全局变量currentInput保存着当前生成的输入框元素，为保证再次生成输入框时，调用removeInput()清除上次生成的输入框元素，从而不影响性能。
4、布局
　　生成的输入框应该放在哪儿？这就是layout()函数中所做的事情。layout()函数修改生成的输入框的位置信息，让其在GraphView拓扑图组件上的位置刚好的node图元的位置相同。
```
function layout(element){
    var rect = element.bindingNode.getRect();
    var x =  rect.x;
    var y =  rect.y;
    element.style.position = 'absolute';
    element.style.width = rect.width + 'px';
    element.style.height = rect.height + 'px';
    element.style.top = y + 'px';
    element.style.left = x + 'px';
    element.style.background = '#fff';
    element.style.color = '#000';
    element.style.textAlign = 'center';
}
```
　　以‘temperature’为例，在点击标签名为‘temperature’的node图元时，会在其上生成一个输入框，获取该node图元的宽、高、位置信息，并分别赋值给绝对定位后输入框的宽、高、位置，这样即可让输入框刚好覆盖住node图元。
5、平移和缩放
　　可能细心思考的朋友也会发现，在对整个场景图进行平移和缩放时，按照上诉布局方式，输入框的位置和大小却没有跟随着node图元的位置进行改变，所以我们在布局时还需要思考到平移、缩放事件。
　　首先，layout函数的内容中，元素的宽、高、位置信息必须加入平移和缩放产生的结果，所以，最终layout代码如下：
```
function layout(element){
    var rect = element.bindingNode.getRect();
    var zoom = graphView.getZoom();
    var tx = graphView.tx();
    var ty = graphView.ty();
    rect.x *= zoom;
    rect.y *= zoom;
    rect.width *= zoom;
    rect.height *= zoom;
    var x = tx + rect.x;
    var y = ty + rect.y;
    element.style.position = 'absolute';
    element.style.width = rect.width + 'px';
    element.style.height = rect.height + 'px';
    element.style.top = y + 'px';
    element.style.left = x + 'px';
    element.style.background = '#fff';
    element.style.color = '#000';
    element.style.textAlign = 'center';
}
```
　　其次，我们需要对平移和缩放事件添加监听，以便能在该事件发生时，再次调用layout()函数将输入框的位置进行同步，在这里，我们用内置的交互器addPropertyChangeListener（简写为mp），监听zoom、translateX、translateY属性的变化：
```
var changeProperties = {
    'zoom': true,
    'translateX': true,
    'translateY':true
}
```
```
graphView.mp(function(e) {
    if (changeProperties[e.property]) {
        var elements = document.getElementsByTagName('input');
        for (var i = 0; i < elements.length; i++) {
            layout(elements[i]);                                
        }
    }
});
```
6、更新node
　　大家在Demo中可以发现，我们按下Enter键时，输入的文字会同步到node中，其实这里做了两件事： 给node设值后删除输入框。
　　a、给node设值，是用一个名为setText()的函数来实现的，实现代码如下：
```
function setText(tagName){
    var element = document.getElementsByTagName(tagName);
    if(!element) return;
    for (var i = 0; i < element.length; i++) {
        var value = (element[i].value) ? element[i].value : 32 ;
        element[i].bindingNode.s('text', value);
    }
}
```
　　在检测输入框中值得存在性后，给node图元赋值用到我们HT的setStyle（简写为s）方法。
　　b、删除输入框
```
function removeInput(){
    if(!currentInput) return;
    graphView.getView().removeChild(currentInput);
    currentInput = null;
}
```
　　c、添加Enter的事件监听器
　　因为没有监听键盘的内置交互器，所以我们通过graphView.getView().addEventListener直接对底层的div添加监听。
```
graphView.getView().addEventListener('keydown', function(event){
    if(ht.Default.isEnter(event)){
         setText('input');
         removeInput();
    } else if(ht.Default.isEsc(event)){
         removeInput();
    }                        
}, false);
```
　　最后，再次贴上Demo地址（[http://www.hightopo.com/demo/GraphInput/display.html](http://www.hightopo.com/demo/GraphInput/display.html)），希望能够帮助那些需要在拓扑图中加入原生HTML的朋友，也望大家不吝赐教。
