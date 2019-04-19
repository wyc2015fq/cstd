# 基于 HTML5 Canvas 的 3D 模型列表贴图 - xhload3d - 博客园
少量图片对于我们赋值是没有什么难度，但是如果图片的量大的话，我们肯定希望能很直接地显示在界面上供我们使用，再就是排放的位置等等，这些都需要比较直观的操作，在实际应用中会让我们省很多力以及时间。下面这个例子给出了解决的方法，当然大家有需要的话，可自行下载更改代码，变成自己的项目。
本例地址： [http://hightopo.com/guide/guide/core/listview/examples/example_custom.html](http://hightopo.com/guide/guide/core/listview/examples/example_custom.html)
实现图如下：
![](https://images2017.cnblogs.com/blog/591709/201712/591709-20171206181148175-158231922.png)
首先，创建场景，HT 中有一个 BorderPane 面板组件是拿来页面排布的，可以排布 html 标签，也可以排布 HT 的组件，这里我们将整个页面分为三个部分，顶部工具条 toolbar、左侧列表 listView 和中间 3d 场景 g3d，再将这个面板组件添加进 html body 体中：
```
borderPane = new ht.widget.BorderPane();//面板组件                
toolbar = new ht.widget.Toolbar(); //工具条                    
                                
listView = new ht.widget.ListView(); //列表组件
g3d = new ht.graph3d.Graph3dView();// 3d 组件
                
borderPane.setTopView(toolbar);//将 toolbar 放置到面板中的顶部
borderPane.setLeftView(listView, 350); //将 listView 放置到面板中的左侧
borderPane.setCenterView(g3d); //将 g3d 放置到面板中的中间
                
borderPane.addToDOM(); //将面板组件添加进 body 中
```
addToDOM 函数是 HT 封装好的将 HT 组件添加进 body 体中的一个方法，其实现逻辑如下：
```
addToDOM = function(){   
    var self = this,
          view = self.getView(),//通过 getView 函数获取组件的底层 div
          style = view.style;
    document.body.appendChild(view); //body 添加孩子 view           
    style.left = '0';
    style.right = '0';
    style.top = '0';
    style.bottom = '0';      
    window.addEventListener('resize', function () { self.iv(); }, false);//窗口大小变化时，立即刷新组件            
}
```
我们一个部分一个部分来解析，从最上层的 toolbar 工具条开始，如下：
![](https://images2017.cnblogs.com/blog/591709/201712/591709-20171206183855863-1705084157.png)
工具条也是分为三个部分，一是左侧的搜索框，二是中间的分割线，三是右侧的点击按钮。
我们首先向工具条 toolbar 中添加这三个元素，具体添加方法请参考 [HT for Web 工具条手册](http://hightopo.com/guide/guide/core/toolbar/ht-toolbar-guide.html)：
```
toolbar.setItems([//设置工具条元素数组              
    {
        id: 'text',
        label: 'Search',
        icon: 'images/search.png',
        textField: {
            width: 120
        }
    },
    'separator',
    {
        label: 'Sort by price',
        type: 'toggle',//toggle表示开关按钮
        selected: true,
        action: function(){
            listView.setSortFunc(this.selected ? sortFunc : null);
        }
    }
]);
```
接下来向左侧的 listView 列表中添加数据，这个数据就是 product.js 中的变量 products，通过遍历这个数组变量，将这个数组中的所有值都填充到 listView 列表中：
![](https://images2017.cnblogs.com/blog/591709/201712/591709-20171206194119034-1353974715.png)
```
products.forEach(function(product){//products 是在product.js 文件中定义的
    var data = new ht.Data();
    data.a(product);//设置数据 data 的 attr 属性
    、listView.dm().add(data);//将 data 添加进 listView 的数据容器中
});
```
然后对 listView 列表进行一系列的样式属性的设置：行高、背景、icon 图标、文字提示等等。代码如下，解释都在代码中了，还有不懂的请查阅 [HT for Web 列表手册](http://hightopo.com/guide/guide/core/listview/ht-listview-guide.html)：
```
listView.setRowHeight(50);//设置行高
listView.drawRowBackground = function(g, data, selected, x, y, width, height){//绘制行背景色，默认仅在选中该行时填充选中背景色，可重载自定义
    if(this.isSelected(data)){//选中时
        g.fillStyle = '#87A6CB';
    }
    else if(this.getRowIndex(data) % 2 === 0){//偶数行时
        g.fillStyle = '#F1F4F7';
    }
    else{
        g.fillStyle = '#FAFAFA';
    }
    g.beginPath();
    g.rect(x, y, width, height);
    g.fill();
};
// HT 通过 ht.Default.setImage('name', json) 函数来注册图片
ht.Default.setImage('productIcon', {
    width: 50,
    height: 50,
    clip: function(g, width, height) {//clip 用于裁剪绘制区域
        //利用canvas画笔绘制，实现自定义裁剪任意形状的效果
        //这里是将图片裁剪成圆形
        g.beginPath();
        //x, y, radius, startAngle, endAngle, anticlockwise
        g.arc(width/2, height/2, Math.min(width, height)/2-3, 0, Math.PI * 2, true);
        g.clip();
    },
    comps: [//矢量图形的组件Array数组，每个数组对象为一个独立的组件类型，数组的顺序为组件绘制先后顺序
        {
            type: 'image',
            stretch: 'uniform',//图片始终保持原始宽高比例不变化，并尽量填充满矩形区域
            rect: [0, 0, 50, 50],//指定组件绘制在矢量中的矩形边界 [x, y, width, height]四个参数方式，分别代表左上角坐标x和y，以及宽高width和height
            name: {func: function(data){return data.a('ProductId');}}//图片的名字为 data.a('ProductId') 返回的值
        }
    ]
});
listView.setIndent(60);//设置indent缩进，该参数一般用于指定图标的宽度                 
listView.getIcon = function(data){//返回data对象对应的icon图标，可重载自定义
    return 'productIcon';//这个是前面 ht.Default.setImage 函数注册过的矢量图形
};  
                
listView.enableToolTip();//开启文字提示
listView.getLabel = function(data){//返回data对象显示的文字，默认返回data.toLabel()，可重载自定义
    return data.a('ProductName') + ' - $' + data.a('UnitPrice').toFixed(2);
};                
listView.getToolTip = function(e){//根据传入的交互事件，返回文本提示信息，可重载自定义
    var data = this.getDataAt(e);//传入逻辑坐标点或者交互event事件参数，返回当前点下的数据元素
    if(data){
        return '<span style="color:#3D97D0">ProductId: </span>' + data.a('ProductId') + '<br>' +  
                       '<span style="color:#3D97D0">ProductName: </span>' + data.a('ProductName') + '<br>' +  
                       '<span style="color:#3D97D0">QuantityPerUnit: </span>' + data.a('QuantityPerUnit') + '<br>' + 
                       '<span style="color:#3D97D0">Description: </span>' + data.a('Description');                        
    }
    return null;
};
```
列表组件中还封装了一个很方便的函数 setSortFunc，用于设置排序函数，用户也可以自定义，目前我们希望对这些”商品“进行排序：
```
sortFunc = function(d1, d2){//自定义排序函数
    return d1.a('UnitPrice') - d2.a('UnitPrice');
};
listView.setSortFunc(sortFunc);//HT 定义的 设置排序函数
```
因为我们要进行数据的搜索，就要对数据以及显示方面进行过滤，因为在数据变化时，HT 无法获知需要更新，这时候就要我们手动对有显示变化的部分调用更新函数 invalidate 简写为 iv。
我们对文本输入框的键盘弹起事件进行事件的监听，然后判断我们输入的值在 listView 列表中是否存在等操作对显示界面进行过滤：
```
// 对text文本框进行键盘按键弹起事件监听 toolbar.getItemById('text').element.getElement().onkeyup = function(e){
    listView.invalidateModel();//无效模型，最彻底的刷新方式 “完全刷新”
};
//如果文本框输入的值在
listView.setVisibleFunc(function(data){//设置可见过滤器                    
    var text = toolbar.v('text');//getValue(id)根据id获取对应item元素值，简写函数为v(id)
    if(text){              
        return data.a('ProductName').toLowerCase().indexOf(text.toLowerCase()) >= 0;//indexOf()方法返回在类型数组中可以找到给定元素的第一个索引，如果不存在，则返回-1
    }
    return true;
});
```
 第三个部分，右侧 3d 场景，利用的是 HT 的三维组件 ht.graph3d.Graph3dView，然后在 3d 场景上添加两个节点，作为对照：
```
//创建两个节点放到 3d 场景中
var node = new ht.Node();
node.s3(30, 30, 30);//设置三维大小
node.p3(-30, 15, 0);//设置三维坐标
node.s('all.color', '#87A6CB');//设置 node 的六个面颜色                
g3d.dm().add(node);//将新建的 node 添加进 3d 场景的数据容器中
                
var node = new ht.Node();
node.s3(30, 30, 30);
node.p3(30, 15, 0);
node.s('all.color', '#87A6CB');
node.setElevation(15);
g3d.dm().add(node);
                
g3d.setEye(-100, 100, 80);//设置 3d 场景的眼睛（或Camera）所在位置，默认值为[0, 300, 1000]
g3d.setGridVisible(true);//设置是否显示网格
g3d.setGridColor('#F1F4F7');//设置网格线颜色
```
整个场景创建完毕，接下来就是将 listView 中显示的 icon 图标拖拽到 3d 中的节点上，作为贴图。列表组件中封装了一个拖拽的功能 handleDragAndDrop，这个函数有两个参数，event 交互事件和 state 当前状态，我们对拖拽事件的不同状态进行不同的处理：
```
listView.handleDragAndDrop = function(e, state){//该函数默认为空，若该函数被重载，则pan平移组件功能将被关闭
    if(state === 'prepare'){//state当前状态，先后会有prepare-begin-between-end四种过程
        var data = listView.getDataAt(e);//传入逻辑坐标点或者交互event事件参数，返回当前点下的数据元素
        listView.sm().ss(data);//设置选中当前事件所在的数据元素
        if(dragImage && dragImage.parentNode){
            document.body.removeChild(dragImage);
        }
        dragImage = ht.Default.toCanvas('productIcon', 30, 30, 'uniform', data);
        // toCanvas(image, width, height, stretch, data, view, color)将图片转换成Canvas对象
        productId = data.a('ProductId');
    }
    else if(state === 'begin'){
        if(dragImage){
            var pagePoint = ht.Default.getPagePoint(e);//返回page属性坐标
            dragImage.style.left = pagePoint.x - dragImage.width/2 + 'px';//实时更新拖拽时的图标的位置
            dragImage.style.top = pagePoint.y - dragImage.height/2 + 'px';
            document.body.appendChild(dragImage);//在 html body 体中添加这个拖拽的图片
        }
    }
    else if(state === 'between'){
        if(dragImage){
            var pagePoint = ht.Default.getPagePoint(e);//返回page属性坐标
            dragImage.style.left = pagePoint.x - dragImage.width/2 + 'px';
            dragImage.style.top = pagePoint.y - dragImage.height/2 + 'px';   
                            
            if(ht.Default.containedInView(e, g3d)){//判断交互事件所处位置是否在View组件之上，一般用于Drog And Drop的拖拽操作判断
            //这边做了两个判断，一个是鼠标在拖拽的时候未松开，一个是鼠标拖拽的时候松开了。
                if(lastFaceInfo){//鼠标未松开的情况下，贴图显示旧值
                //data.face 默认值为front，图标在3D下的朝向，可取值left|right|top|bottom|front|back|center
                    lastFaceInfo.data.s(lastFaceInfo.face + '.image', lastFaceInfo.oldValue);
                    lastFaceInfo = null;
                }
                //鼠标松开时，将新值赋给这个面
                var faceInfo = g3d.getHitFaceInfo(e);//获取鼠标所在面信息
                if(faceInfo){
                    faceInfo.oldValue = faceInfo.data.s(faceInfo.face + '.image');//获取面的“老值”
                    faceInfo.data.s(faceInfo.face + '.image', productId);//front/back/top/bottom/left/right.image 设置这些面的贴图
                    lastFaceInfo = faceInfo;
                }
            }
        }
    }
    else{//拖拽结束之后，所有值都回到初始值
        if(dragImage){//有从列表中拖拽图片
            if(lastFaceInfo){//有赋“图片”到 3d 中的节点上
                lastFaceInfo.data.s(lastFaceInfo.face + '.image', lastFaceInfo.oldValue);
               lastFaceInfo = null;
           }                             
           if(ht.Default.containedInView(e, g3d)){                               
               var faceInfo = g3d.getHitFaceInfo(e);
               if(faceInfo){
                   faceInfo.data.s(faceInfo.face + '.image', productId);
               }
           }                                                        
           if(dragImage.parentNode){
               document.body.removeChild(dragImage);
           }
           dragImage = null;  
           productId = null;
       }
   }
};
```
