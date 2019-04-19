# 快速创建 HTML5 Canvas 电信网络拓扑图 - xhload3d - 博客园
## 前言
电信网络拓扑图确实实用性很强，最近有个项目是基于这个的，为了写得丰富一点，还加了自动布局已经属性栏部分，不过这个 Demo 真的写得很用心，稍微改改都能直接运用到项目中去，还可以进行数据交互，总之希望能对大家有所帮助。
## 效果图
![](https://images2018.cnblogs.com/blog/591709/201803/591709-20180321115320465-923487856.jpg)
### 拓扑图
从上面的效果图中我们可以看到，整个页面分为 3 个部分，左边的 graphView 拓扑图部分，右下角的 tableView 表格部分，以及右上角的 propertyView 属性部分。我们先把整个场景划分出来，然后再向各个部分来添加具体的内容：
```
gv = new ht.graph.GraphView();
var tablePane = new ht.widget.TablePane(gv.dm());//表格面板组件
propertyView = new ht.widget.PropertyView(gv.dm());//formPane是在propertyView里的，所以要先定义
var rightView = new ht.widget.SplitView(propertyView, tablePane, 'v', 0.4);//分割组件，v分为上下层，比例为0.4:0.6
rightView.getView().style.borderLeft = '1px solid #000';
var borderPane = new ht.widget.BorderPane();//边框面板组件
borderPane.setRightView(rightView, 400);//设置 borderPane 右边组件为 rightView，宽度为400
borderPane.setCenterView(gv);//设置 borderPane 中间组件为 gv 
borderPane.addToDOM();//将 borderPane 组件添加进 body 中
```
上面代码中 new 出来的部分都是 HT 封装好的组件，相当于“类”，这里解释一下 SplitView 分割组件，分割组件用于左右或上下分割两个子组件， 子组件可为 HT 框架提供的组件，也可为 HTML 的原生组件，子组件以 position 为 absolute 方式进行绝对定位，这个组件中的参数分别为（左组件或上组件， 右组件或下组件，h 表示左右分割 v 表示上下分割，分割位置默认值为 0.5，如果设置值为 0~1 则按百分比分割，大于 1 代表左组件或上组件的绝对宽或高，小于 1 代表右组件或下组件的绝对宽或高）；还有 BorderPane 面板组件为布局容器，可在上、下、左、右、中的五个区域位置摆放子组件， 子组件可为 HT 框架提供的组件，也可为 HTML 原生组件，子组件以 position 为 absolute 方式进行绝对定位。这里我结合了 SplitView 和 BorderPane 将场景分成了三个部分。最后记得一定要将最终的布局容器添加进 body 中或者任意一个 HTML 标签中，这样才能在界面上显示，其中 addToDOM 的定义内容如下：
```
addToDOM = function(){   
    var self = this,
        view = self.getView(), //获取这个组件的底层 div 
        style = view.style;//获取底层 div 的样式 style 属性
    document.body.appendChild(view); //将底层 div 添加进 body 中
    style.left = '0';//HT 默认将组件都定义为决定定位 absolute，所以需要设置位置
    style.right = '0';
    style.top = '0';
    style.bottom = '0';      
    window.addEventListener('resize', function () { self.iv(); }, false);            
}
```
HT 的组件一般都会嵌入 BorderPane、SplitView 和 TabView 等容器中使用，而最外层的 HT 组件则需要用户手工将 getView() 返回的底层 div 元素添加到页面的 DOM 元素中，这里需要注意的是，当父容器大小变化时，如果父容器是 BorderPane 和 SplitView 等这些 HT 预定义的容器组件，则 HT 的容器会自动递归调用孩子组件 invalidate 函数通知更新。但如果父容器是原生的 html 元素， 则 HT 组件无法获知需要更新，因此最外层的 HT 组件一般需要监听 window 的窗口大小变化事件，调用最外层组件 invalidate 函数进行更新。
场景创建完毕。为了显示不同节点对应的属性的不同，我们在拓扑图上添加了七个节点：
```
function initModel(){
    var name = "设备";
    var count = 0;
    var root = createNode(name + count++, name + (++count));//参数1为name，参数2为tag
    root.setImage('./symbols/机房/服务器.json');
    root.setName('服务器');
    root.s('label.position', 3);
    gv.sm().ss(root);//默认选中root节点
    for (var i = 0; i < 2; i++) {
        var iNode = createNode(name + count++, name + (++count));//参数1为name，参数2为tag
        createEdge(root, iNode);
        for (var j = 0; j < 2; j++) {
            var jNode = createNode(name + count++, name + (++count));
            createEdge(iNode, jNode);
        }
    }
}
```
createNode 函数的声明如下：
```
function createNode(name, tag){//创建Node节点
    flag++;
    var node = new ht.Node();
    node.setName(name);
    node.setTag(tag);
    node.setImage('./symbols/机房/XX分系统.json');
    node.a('hidden', false);//自定义属性，可以控制node.a('hidden')来控制节点显隐
    node.a('接口类型', 'SATA');
    node.a('显卡', 'Nvidia');
    if(flag % 2 === 0){
        node.a('接口类型', 'IDE');
        node.a('显卡', 'ATI');
    }
    node.s('label.position', 11);
    gv.dm().add(node);//将节点添加进数据容器DataModel中
    node.tablePane1 = createTableView(serviceType, dataModel1);//创建表格面板
    node.tablePane2 = createTableView(serviceSize, dataModel2);
    node.tablePane3 = createTableView(version, dataModel3);
    node.formPane1 = createFormPane(node.tablePane1);//创建表单面板
    node.formPane1.title = '类型';//为了给后面dialog对话框的标题做准备
    node.formPane2 = createFormPane(node.tablePane2);
    node.formPane2.title = '内存';
    node.formPane3 = createFormPane(node.tablePane3);
    node.formPane3.title = '型号';
    if(flag % 3 === 0){
        node.formPane3.v('tag', '联想（Lenovo）服务器X3650M5 8871');
    }else{
        node.formPane3.v('tag', 'Lenovo IBM X3250 5458I21');
    }
    node.a('型号', node.formPane3.v('tag'));
    return node;
}
```
我们通过控制这个节点的 hidden 属性，利用 graphView 中的可视过滤器 setVisibleFunc 函数来控制节点的显隐：
```
gv.setVisibleFunc(function(data){
    if(data.a('hidden')){
        return false;
    }
    return true;
});
```
### 属性面板
有了节点自然就需要显示属性了，加上下面的 tablePane 表格面板中的值，一共添加了七种属性：
```
function createProperty(){//创建属性
    propertyView.addProperties([
        {
            name: 'name',//获取 name 属性，结合 accessType 属性最终实现对节点属性的存取，accessType 默认值为 null，如name为age，采用getAge()和setAge(98)的get/set或is/set方式存取(这边name为name，所以通过 getName() 获取)
            displayName: '名称'//设置属性名的显示文本值
        },
        {
            name: 'hidden',//获取 hidden 属性
            displayName: '隐藏这个节点',
            accessType: 'attr',//如name为hidden，采用getAttr('hidden')和setAttr('hidden', false)的方式存取
            icon: 'images/alert.gif',//设置属性名左侧显示的图标
            valueType: 'boolean',//用于提示组件提供合适的renderer渲染 布尔类型，显示为勾选框
            editable: true//设置该属性是否可编辑
        },
        {
            name: 'grade',
            displayName: '类型',
            accessType: 'attr',
            drawPropertyValue: function(g, property, value, rowIndex, x, y, w, h, data, view){//自定义属性值渲染函数
                var cb = function(v) {
                    data.a('grade', v);
                }
                return fillFormPane(data.formPane1, w, h, data.tablePane1, serviceType, cb);
            }
        },
        {
            name: 'number',
            displayName: '内存',
            accessType: 'attr',
            drawPropertyValue: function(g, property, value, rowIndex, x, y, w, h, data, view){
                var cb = function(v) {
                    data.a('number', v);
                }
                return fillFormPane(data.formPane2, w, h, data.tablePane2, serviceSize, cb);
            }
        },
        {
            name: '接口类型',
            accessType: 'attr',
            displayName: '接口类型'
        },
        {
            name: '显卡',
            accessType: 'attr',
            displayName: '显卡'
        },
        {
            name: '型号',
            accessType: 'attr',
            displayName: '型号',
        }
    ]);
}
```
第三第四个属性中 drawPropertyValue 属性的返回值为 fillFormPane 函数，这个函数的参数分别为（表单组件 formP，表单组件宽 w，表单组件高 h，表单组件中按钮点击生成弹出框中的表格组件 tableP，表格组件中的数组内容 arr，cb 函数将双击表格组件中的行返回的值赋值给 form 表单中的 ht.widget.TextField 文本框）。
- 第一个参数 formP 表单组件的创建，表单组件的创建就是创建一个表单组件，在表单组件中添加一个文本框以及一个按钮，这个步骤在 HT 中也是相当的简单：
- 
```
function createFormPane(tPane) {//创建表单面板
    var formPane = new ht.widget.FormPane();
    formPane.setPadding(0);//设置表单四周与组件内容的间距
    var tField = new ht.widget.TextField();//创建一个文本框
    tField.setText('');//文本框的内容为空
    tField.setDisabled(true);//文本框不可操作
    formPane.addRow([//向表单中添加行
        {
            id: 'tag',//唯一标示属性，可通过formPane.getItemById(id)获取添加到对应的item对象
            element: tField//属性值可为 HTML原生元素、FormPane内部自绘制的文本信息以及HT自带组件如Button、CheckBox和ComboBox等
        },
        {
            button:{//设置了该属性后HT将根据属性值自动构建ht.widget.Button对象，并保存在element属性上
                label:'...',//按钮上的文本内容
                onClicked: function(){//按钮点击事件
                    for(var i = 0; i < tPane.dm().size(); i++){//设置tablePane默认选中formPane对应的值
                        var data = tPane.dm().getDatas().get(i);
                        if(data.a('value') === formPane.v('tag')){
                            tPane.sm().ss(data);
                        }
                    }
                    return createDialog(tPane, formPane);//返回的是创建一个对话框，对话框的内容为表格面板
                }
            }
        }
    ], [0.5, 0.1]);//设置表格组件中第一个元素和第二个元素的显示比例。这个表格组件一共只有两个元素，一个文本框一个按钮，占比分别为 0.5 和 0.1
    return formPane;
}
```
createDialog 函数创建的过程也是简洁明了，通过 setConfig(config) 方法配置对话框的标题，尺寸，内容等，我给 createDialog 传了一个参数 tPane 表格组件，用作对话框显示的内容：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180121110936271-369992455.png)
- 
```
function createDialog(tPane){//创建弹出框
    dialog.setConfig({
        title: gv.sm().ld().getName()+"的"+formPane.title,//对话框的标题
        content: tPane, //直接将弹出框的内容设置为表格面板
        width: 400,//指定对话框的宽度
        height: 200,
        draggable: true,//指定对话框是否可拖拽调整位置
        closable: true,//表示是否显示关闭按钮
        maximizable: true,//表示对话框是否可被最大化
        resizeMode: "wh",//鼠标移动到对话框右下角可改变对话框的大小，wh表示宽高都可调整
        buttons: [//添加两个按钮
            {
                label: '取消',
                action: function(){
                    dialog.hide()
                }
            },
            {
                label: '确定',
            }
        ]
    });
    dialog.show();//显示对话框
}
```
-  第四个参数 tableP 表格组件，也没有什么特别的，就是创建一个表单组件，然后向表单组件中添加列，步骤简单，代码也相当简单：
```
function createTableView(arr, dm){//创建表格组件
    var tableView = new ht.widget.TableView(dm);
    tableView.addColumns([//用json的数组参数方式批量添加列信息
        {
            displayName: 'ID',//获取表头的列名内容
            drawCell: function(g, data, selected, column, x, y, w, h, tableView){//自定义单元格渲染方式
                var id = tableView.getRowIndex(data);//返回data对象所在的行索引
                ht.Default.drawText(g, 'row' + (id + 1), null, null, x, y, w, h, 'center');//绘制文字参数(g画笔对象，value文字内容，font文字字体，color文字颜色，x绘制开始的x坐标，y绘制开始的y坐标，w绘制的宽度，h绘制的高度，align文字水平对齐方式，vAlign文字垂直对齐方式)
            }
        },
        {
            displayName: 'Name',
            drawCell: function(g, data, selected, column, x, y, w, h, tableView){
                var id = tableView.getRowIndex(data);
                var info = arr[id];
                ht.Default.drawText(g, info, null, null, x, y, w, h, 'center');
            }
        }
    ]);
    return tableView;
}
```
解释完 fillFormPane 中的参数，来看看这个函数是如何定义的，基本上只差最后一步，点击 tablePane 表格组件中的元素，将这个元素返回给 formPane 表单组件中的 textField 文本框：
```
function fillFormPane(formP, w, h, tableP, arr, cb){//右边的formpane
    if(formP === undefined){
        return;
    }
    formP.setWidth(w);
    formP.setHeight(h);
    formP.setHGap(0);
    if(formP.v('tag') === 'undefined' || formP.v('tag') === '') {
        formP.v('tag', arr[0]);
    }
    tableP.onDataDoubleClicked = function(data){//表格组件中的data所在行被双击时回调
        var v = arr[data.a('index')];
        formP.v('tag', v);//根据id设置对应item元素值 为 setValue 的简写，这个id为tag的元素就是文本框
        dialog.hide();
        if (cb){cb(v);} //如果传入了cb参数，则设置data.a('number')/data.a('helloName')的值为表格中被双击的那一行的值，即赋值给第三第四属性
    }
    tableP.onDataClicked = function(data){//表格组件中的data所在行被单击时回调
        dialog.getConfig().buttons[1].action = function(){//点击“确定”才进行下面的操作
            var v = arr[data.a('index')];
            formP.v('tag', v);
            dialog.hide();
            if (cb){cb(v);} 
        }
    };
    return formP.getView();
}
```
```
function fillFormPane(formP, w, h, tableP, arr, cb){//右边的formpane
    if(formP === undefined){
        return;
    }
    formP.setWidth(w);
    formP.setHeight(h);
    formP.setHGap(0);
    if(formP.v('tag') === 'undefined' || formP.v('tag') === '') {
        formP.v('tag', arr[0]);
    }
    tableP.onDataDoubleClicked = function(data){//表格组件中的data所在行被双击时回调
        var v = arr[data.a('index')];
        formP.v('tag', v);//根据id设置对应item元素值 为 setValue 的简写，这个id为tag的元素就是文本框
        dialog.hide();
        if (cb){cb(v);} //如果传入了cb参数，则设置data.a('number')/data.a('helloName')的值为表格中被双击的那一行的值，即赋值给第三第四属性
    }
    tableP.onDataClicked = function(data){//表格组件中的data所在行被单击时回调
        dialog.getConfig().buttons[1].action = function(){//点击“确定”才进行下面的操作
            var v = arr[data.a('index')];
            formP.v('tag', v);
            dialog.hide();
            if (cb){cb(v);} 
        }
    };
    return formP.getView();
}
```
右上方属性栏的显示到这里就全部结束了，右下方的表格面板的创建方式也是雷同的，大家可以自行看代码理解。
### 自动布局
最后说一下整个界面节点的排布，HT 中的 autolayout 自动布局组件，即根据节点和连线关系，提供多种类型算法进行自动排布节点位置。 自动布局常用于图元较多，或连接关系教复杂时，不易于人工拖拽摆放的场景。我把各个布局方式通过按钮的方式呈现出来了，点击对应的按钮，布局方式就会根据按下的按钮设置的排布方式来自动布局：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180123222957225-2038271260.png)
[http://www.hightopo.com/demo/propertyEditor/index.html](http://www.hightopo.com/demo/propertyEditor/index.html)
## 代码实现
 首先，创建一个新的实例，传入需要自动布局的对象，可以为 DataModel、graphView 以及 graph3dView 三种，然后设置默认的排布方式：
```
autoLayout = new ht.layout.AutoLayout(gv);
setTimeout(function(){
    layout('towardsouth', true);//因为图片还没加载出来的时候，自动布局就按照节点的默认大小来布局的
}, 200);
```
接着创建 formPane 表单面板，添加进 body 中，放在 body 左上角，我不将所有代码粘出来了，就显示第一个布局的按钮就好：
```
function createDirectionForm(){
    var form = new ht.widget.FormPane();
    form.setWidth(200);//设置表单宽度
    form.setHeight(80);
    document.body.appendChild(form.getView());
    form.getView().style.background = '#fff';
    form.getView().style.boxShadow = '4px 16px 16px rgba(0, 0, 0, 0.1)';//设置阴影样式
    form.addRow([//这一行单独拿出来，作为标题
        {
            element: '自动布局:',//显示的文字
        }
    ], [0.1]);//数组内只有一个对象，就只设置一个对象的宽度就好
    form.addRow([
        {
            button: {
                icon: '布局/南布局.json',
                onClicked: function(){
                    layout('towardsouth', true);
                },
                background: null,
                labelColor: '#fff',
                groupId: 'btn',
                toolTip: '朝南布局',
                borderColor: null
            }
        },
        //....接下来添加剩下的6个按钮
    ], [0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1]);//数组中有七个对象，就要设置七个对象的宽度
    return form;
}
```
比较有趣的部分就是这些啦，感谢大家的阅读，如果有些地方我解释的不够清楚，可以私信给我，大家一起探讨探讨。
