# 基于 HTML5 的工业互联网云平台监控机房 U 位 - xhload3d - 博客园
# 前言
机柜 U 位管理是一项突破性创新技术--继承了 RFID 标签（电子标签）的优点的同时，完全解决了 RFID 技术（非接触式的自动识别技术）在机房 U 位资产监控场应用景中的四大缺陷，采用工业互联网云平台监控机房 U 位的方法，具有高可靠性、高准确性、精准定位、免维护的特点，满足了 U 位级实时监控、智能运维闭环管理的需求。设备上架、下架与迁移，自动变更和实时记录，（用户评价）：部署工业互联网云平台监控机房 U 位后节省了 99% 的登记变更记录的时间，而且实现了变更后数据 100% 的准确，在这之前是难以想象的，真正实现运维管理最后的工作。
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180504110053309-1069714849.gif)
# 代码生成
## 场景搭建
整个 Demo 由最左侧的树，中间部分的列表以及右边的拓扑图整体构成，为了让整个布局干净一点，这里结合 splitView 和 borderPane 两种布局方式来进行。首先将场景分为左右两个部分，左边为树，右边是列表和拓扑图的组合：
```
treeView = this.treeView = new ht.widget.TreeView(),// 树组件
splitView = this.splitView = new ht.widget.SplitView(treeView, null, 'h', 280);// 分割组件，将场景分为左右两个部分，左边为树组件，右边为空，左边的宽度为280，右边的组件先设置为空到时候根据具体情况分配 
this.splitView.addToDOM();
```
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180504152730497-1519795273.png)
布局结束记得将最外层组件的最底层 div 添加到 body 中，HT 的组件一般都会嵌入 BorderPane、SplitView 和 TabView 等容器中使用，而最外层的HT组件则需要用户手工将 getView() 返回的底层 div 元素添加到页面的 DOM 元素中，这里需要注意的是，当父容器大小变化时，如果父容器是 BorderPane 和 SplitView 等这些HT预定义的容器组件，则HT的容器会自动递归调用孩子组件 invalidate 函数通知更新。但如果父容器是原生的 html 元素， 则 HT 组件无法获知需要更新，因此最外层的 HT 组件一般需要监听 window 的窗口大小变化事件，调用最外层组件 invalidate 函数进行更新。
为了最外层组件加载填充满窗口的方便性，HT 的所有组件都有 addToDOM 函数，其实现逻辑如下，其中 iv 是 invalidate 的简写：
```
addToDOM = function(){
    var self = this,
          view = self.getView(),//获取组件的底层 div
          style = view.style;
    document.body.appendChild(view);//将组件底层div添加进body中
    style.left = '0';//ht 默认将所有的组件的position都设置为absolute绝对定位
    style.right = '0';
    style.top = '0';
    style.bottom = '0';
    window.addEventListener('resize', function () { self.iv(); }, false);//窗口大小改变事件，调用刷新函数
}
```
右边的拓扑图部分是在监听选中变化事件的时候更新的，当然，初始化设置的选中树上的第一个节点就触发了选中变化事件：
```
cms.treeView.sm().ss(cms.treeView.dm().getDatas().get(0));// 设置选中树上的第一个节点
treeView.sm().ms(function(){// 监听选中变化事件
    var ld = treeView.sm().ld();// 获取最后选中的节点
    if (ld) self.updateForm(ld.a('type'));
});
CMS.prototype.updateForm = function(type){
    var self = this,
        ld = this.treeView.sm().ld();// 获取树上选中的最后一个节点
    if (type === self.TYPE_RACK_SPACE) {// 如果是在树上选中了节点，那么点击“添加机柜”就直接在树上选中的节点下生成
        if (!this.rackBuild) {
            this.rackBuild = new RackBuild(this);// 此类中定义了场景的中间列表部分，右边拓扑图部分以及对应的逻辑
        }
        this.rackBuild.setData(ld);// 在树上添加一个新的节点
        this.splitView.setRightView(this.rackBuild.getHTView());// 设置分割组件右边的内容为整个场景的中间“列表”内容+右边的拓扑内容
    }
}
```
上面代码中 splitView.setRightView 函数意为设置右侧组件，有了这个函数，我就可以动态地改变 spliteView 组件中的右侧组件了。
## 初始化树
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180506100723488-829842520.png)
既然布局布好了，就该向具体的位置添加内容了。先来看看如何向树上添加节点。首先我定义了一个初始化的树上的值 treeData，通过遍历这个数组创建树上的节点以及节点上的父子关系：
```
var treeData = [{
    name: 'Racks',
    type: 8,
    children: [
        {
            name: 'rack1',
            type: 18,
            usize: 32
        }, {
            name: 'rack2',
            type: 18
        }
    ]
}];
CMS.prototype.loadTreeData = function(){// 加载树上的节点
    var self = this;
    setTimeout(function(){
        var data = treeData;
        data.forEach(function(d) {// 遍历 treeData 数组的值
            self.createData(d, null);// 第一个节点父亲为空
        });
        self.treeView.expandAll();// 展开树
    }, 10);
}
```
通过 createData 函数创建节点，并给节点设置父子关系：
```
CMS.prototype.createData = function(data, parent){// 在树上创建一个节点
    var self = this,
        htData = new ht.Data(),// 新建 Data 类型节点
        dm = this.treeView.dm();// 获取树的数据容器
    htData.a(data);// 设置节点业务属性 data
    htData.setName(data.name)// 设置节点的 name 属性
    if (parent) {
        htData.setParent(parent);// 设置父亲节点
    }
    dm.add(htData);// 将节点添加到数据容器中
    if (data.children) {// 如果节点中有 children 对象
        data.children.forEach(function(d){// 遍历 children 对象
            self.createData(d, htData);// 再创建 children 对象中的节点作为孩子节点
        });
    }
    return htData;
}
```
## 创建场景右边部分
眼尖的同学在前面的代码中可能注意到了一个未声明的 RackBuild 类，在此类的声明中我们将场景的右半部分主要分为左右两个部分，左边又分为上下两个部分，右边也分为上下两个部分。
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180504160211004-1124823713.png)
这里先将整个右边的部分进行布局，下面代码中的变量 listBorder 为上图的左半部分，变量 borderPane 为上图的右半部分，至于鹰眼组件部分，是添加到在 borderPane 的上层：
```
listView = this.listView = new ht.widget.ListView(),// 列表组件
listForm = this.listForm = new ht.widget.FormPane(),// 表单组件
listBorder = this.listBorder = new ht.widget.BorderPane(),// 场景中间边框面板组件
gv = this.gv = new ht.graph.GraphView(),// 拓扑组件
borderPane = this.borderPane = new ht.widget.BorderPane(),
toolbar = this.toolbar = new ht.widget.Toolbar(),// 工具条组件
splitView = this.splitView = new ht.widget.SplitView(listBorder, borderPane, 'h', 220),// 分割组件
overview = this.overview = new ht.graph.Overview(gv),// 鹰眼组件
overviewDiv = overview.getView();// 获取鹰眼组件底层 div
overviewDiv.style.height = '120px';// HT 的组件默认都是绝对定位的
overviewDiv.style.width = '120px';
overviewDiv.style.left = '0';
overviewDiv.style.bottom = '0';
overviewDiv.style.zIndex = 10;
borderPane.getView().appendChild(overview.getView());// 将鹰眼组件底层 div 添加到面板组件的底层 div 中
listBorder.setTopView(listForm);// 设置顶部组件
listBorder.setCenterView(listView);// 设置中间组件
listBorder.setTopHeight(32);// 设置顶部组件高度
listForm.setVPadding(2);// 设置表单顶部和顶部与组件内容的间距
listForm.setHPadding(4);// 设置表单左边和右边与组件内容的间距
listForm.addRow([// 添加一行组件
    {
        comboBox: {// 组合框类
            labels: ['All', 'Pathch Panel', 'Switch', 'Server', 'Backbone Switch/Router'],// 设置下拉可选值对应文本
            values: [-1, 5, 9, 10, 11],// 设置下拉可选值
            value: -1,// 设置当前值，可为任意类型
            onValueChanged: function(e) {// 值变化触发函数
                var val = this.getValue();// 获取当前的值
                self.listTypeFilter = val;
                self.listView.ivm();// 最彻底的刷新方式
            }
        }
    }
], [0.1], 28);// 参数二为行内元素的宽度，参数三为该行高度
borderPane.setCenterView(gv);// 设置中间组件
borderPane.setTopView(toolbar);// 设置顶部组件
borderPane.setTopHeight(32);// 设置中间组件高度
```
从上面的代码可以看出，splitView 为最外层组件，通过 getHTView 函数返回这个组件，在前面动态设置整个场景的右半部分的组件的时候我们就是通过设置 this.splitView.setRightView(this.rackBuild.getHTView()) 设置场景的右半部分为 rackBuild 的底层 div：
```
getHTView: function(){// 获取最外层组件
    return this.splitView;
}
```
### 添加工具条内容
![](https://img-blog.csdn.net/20180506105754890?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMxNjE0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
toolbar 工具条中总共的元素就三个：添加机柜，编辑机柜和删除机柜。这三个元素只需要通过 setItems 的方式添加到 toolbar 工具条组件上即可，元素的具体定义如下：
```
var toolbarItems = [// 工具条上三个的元素
    {
        icon: self.getToolbarIcon('toolbar.add.rack'),// 用的是我们前面声明过的图片
        toolTip: 'Add a rack',// 文字提示显示内容
        action: function(){// 点击按钮后触发的函数
            self._editingRack = null;
            self.addRackForm.reset();
            self.addRackDialog.show();// 弹出对话框，添加一个新的机架，并填写该机架的信息
        }
    },{
        icon: self.getToolbarIcon('toolbar.edit.rack', function(){// 判断右侧拓扑图上最后选中的节点 来决定这个图标的显示颜色（如果没有选中机柜，那么此图标显示颜色为灰色）
            return self.gv.sm().ld() instanceof Rack;
        }),
        toolTip: 'Edit rack info',
        action: function(){
            var ld = self.gv.sm().ld();// 获取 gv 中最后选中的节点
            if (!ld) return;
            self._editingRack = ld;
            self.addRackForm.v('name', ld.a('name'));// 弹出框中的 name 赋值为 ld 的业务属性 name 的值
            self.addRackForm.v('usize', ld.a('usize'));// 弹出框中的 usize 赋值为 ld 的业务属性 usize 的值
            self.addRackDialog.show();// 点击此按钮会出现弹出框
        }
    },{
        icon: self.getToolbarIcon('toolbar.delete', function(){
            return self.gv.sm().ld() instanceof Rack;// 判断右侧拓扑图上最后选中的节点的类型
        }),
        toolTip: 'Delete a rack',
        action: function(){
            self.handleRemoveRack();// 在拓扑图上删除机柜，并删除树上此机柜对应的节点
        }
    },
]
```
接下来只要把这个 item 添加到 toolbar 中并设置一下排布的方式即可：
```
toolbar.setItems(toolbarItems);// 设置工具条元素数组
toolbar.setStickToRight(true);// 设置工具条是否向右对齐排布
toolbar.enableToolTip(true);// 工具条允许文字提示
```
上面出现的点击 toolbar 工具条按钮触发的事件中有一个“弹出对话框”的操作，通过 this.addRackDialog.show() 来实现，addRackDialog 对象定义在 initDialog 函数中，作用为创建一个 dialog 对话框，我们设置此对话框中的内容为一个 form 表单进行显示，同时还设计了两个按钮，“OK”按钮作为执行创建/更改机柜的属性，“Cancel”按钮不执行其他操作，只是将对话框隐藏：
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180504164656758-1073945419.png)
```
initDialog: function(){// 初始化点击“增改”出现的对话框
    var self = this,
        addRackDialog = this.addRackDialog = new ht.widget.Dialog(),
        addRackForm = this.addRackForm = new FormPane(),// 此类继承于 ht.widget.FormPane
        labelWidth = 72;
    addRackForm.addRow([// 添加行
        'Name',{
            id: 'name',
            textField: {}
        }
    ], [labelWidth, 0.1]);
    addRackForm.addRow([
        'Height(U)',{
            id: 'usize',
            textField: {
                type: 'number'
            }
        }
    ], [labelWidth, 0.1]);
    addRackDialog.setConfig({// 配置对话框的标题，尺寸，内容等
        title: "New Rack",// 对话框的标题
        content: addRackForm,// 指定对话框的内容
        width: 320,// 指定对话框的宽度
        height: 220,// 指定对话框的高度
        draggable: true,// 指定对话框是否可拖拽调整位置
        closable: true,// 可选值为true/false，表示是否显示关闭按钮
        resizeMode: "none",// 鼠标移动到对话框右下角可改变对话框的大小 none 表示不可调整宽高
        buttons: [// 指定对话框按钮组内容
            {
                label: "Ok",// 按钮显示文本
                action: function(button, e) {// action为回调函数，当此按钮被当点击时，回调函数会执行
                    var formData = addRackForm.getValueObject(), rack;
                    if (!formData.usize) {// 如果没有填写 Height 的值，则默认高度为18
                        formData.usize = 18;
                    }
                    if (self._editingRack) {// 如果是“编辑rack信息”的弹框
                        rack = self._editingRack;
                        rack.a(formData);
                        rack.a('treeNode').a(rack.getAttrObject());// 
                    }
                    else {// “增加”新的机柜
                        rack = self.createRack(formData);// 创建一个新的 rack 模型
                        self.gv.dm().add(rack);// 在拓扑图上添加这个rack
                        // update tree
                        formData.type = self.cms.TYPE_RACK;
                        var treeNode = self.cms.createData(formData, cms.treeView.sm().ld());
                        rack.a('treeNode', treeNode);
                    }
                    self.gv.fitContent(1);// 添加元素之后，让所有的图元显示在界面上
                    addRackDialog.hide();// 隐藏对话框
                }
            }, {
                label: 'Cancel',
                action: function(){
                    addRackDialog.hide();// 隐藏对话框
                }
            }
        ],
        buttonsAlign: "right"
    });
}
```
上面代码出现的 FormPane 类，继承于 ht.widget.FormPane 类，在 htwidget.FormPane 的基础上修改也增加了一些函数，主要的内容还是 ht.widget.FormPane 的实现，文章篇幅有限，这里就不贴代码了，有兴趣的可以参考 FormPane.js 文件。
 实现了添加和编辑机柜的两个功能，删除机柜的功能实现上非常容易，只要将节点从拓扑图和树上移除即可：
```
handleRemoveRack: function(){// 在拓扑图上删除机柜，并删除树上此机柜对应的节点
    var ld = this.gv.sm().ld();// 获取 gv 上选中的最后一个节点
    if (ld && ld instanceof Rack) {// 机柜是 Rack 类型
        this.cms.treeView.dm().remove(ld.a('treeNode'));// 移出树上的有 treeNode 属性的节点
        this.gv.dm().remove(ld);// 删除 gv 中的节点
    }
}
```
### 列表中元素拖拽
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180506101054049-833593982.gif)
所有的内容都创建完毕，接下来要考虑的就是交互的内容了。列表组件中有 handleDragAndDrop 函数实现拖拽的功能：
```
listView.handleDragAndDrop = this.handleListDND.bind(this);// 列表上拖拽事件监听
handleListDND: function(e, state){// 拖拽listView列表组件中的事件监听
    var self = this,
        listView = self.listView,
        gv = self.gv,
        dm = gv.dm(),
        dnd = self.dnd;
    // handleDragAndDrop 函数有 prepare-begin-between-end 四种状态
    if (state ==='prepare') {
        var data = listView.getDataAt(e);// 传入逻辑坐标点或者交互event事件参数，返回当前点下的数据元素
        listView.sm().ss(data);// 在拖拽的过程中设置列表组件中的被拖拽的元素被选中
        if (dnd && dnd.parentNode) {
            document.body.removeChild(dnd);
        }
        dnd = self.dnd = ht.Default.createDiv();// 创建一个 div
        dnd.style.zIndex = 10;
        dnd.innerText = data.getName();
    }
    else if (state === 'begin') {
        if (dnd) {
            var pagePoint = ht.Default.getPagePoint(e);// 返回页面坐标
            dnd.style.left = pagePoint.x - dnd.offsetWidth * 0.5 + 'px'; 
            dnd.style.top = pagePoint.y - dnd.offsetHeight * 0.5 + 'px';
            document.body.appendChild(dnd)
        }
    }
    else if (state === 'between') {
        if (dnd) {
            var pagePoint = ht.Default.getPagePoint(e);
            dnd.style.left = pagePoint.x - dnd.offsetWidth * 0.5 + 'px';
            dnd.style.top = pagePoint.y - dnd.offsetHeight * 0.5 + 'px';
            self.showDragHelper(e);
        }
    }
    else {// 拖拽“放开”鼠标后的操作
        if (ht.Default.containedInView(e, self.gv)) {// 判断交互事件所处位置是否在View组件之上
            if (dm.contains(self.dragHelper)) {// 判断容器是否包含该data对象
                var rect = self.dragHelper.getRect(),// 获取图元的矩形区域(包括旋转)
                    target = self.showDragHelper(e),// 
                    node,
                    ld = self.listView.sm().ld(),
                    uindex = target.getCellIndex(rect.y);
                node = self.createPane(rect, ld.getAttrObject(), target, uindex);// 创建设备
                dm.add(node);
                // update tree data
                var treeNode = self.cms.createData(ld.getAttrObject(), target.a('treeNode'));// 在树上创建节点，并设置父亲节点
                treeNode.a('uindex', uindex);
                node.a('treeNode', treeNode);
                dm.remove(self.dragHelper);
            }
        }
        document.body.removeChild(dnd);
        self.dnd = null;
    }
}
```
### 设备拖动
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180506101453613-1501981933.gif)
既然有了从列表组件上拖拽下来的交互动作，接下来应该是做设备在机柜上的拖拽改变位置的功能了，我们通过监听拓扑组件 gv 的交互事件来对节点移动进行事件处理：
```
gv.mi(this.handleInteractor.bind(this));// 监听交互
handleInteractor: function(e){// 移动机柜中的设备 的事件监听
    if (e.kind.indexOf('Move') < 0) return;// 如果非move事件则直接返回不做处理
    var self = this,
        listView = self.listView,
        gv = self.gv,
        dm = gv.dm(),// 获取数据容器
        target = gv.sm().ld(),// 获取最后选中的节点
        uHeight = target.a('uHeight') || 1;// target.a('uHeight')获取最后选中的节点的高度
    if (e.kind === 'prepareMove') {// 准备移动
        self._oldPosition = target.p();// 获取节点当前的位置
    }
    else if (e.kind === 'betweenMove') {// 正在移动
        self.showDragHelper(e.event, uHeight);
        dm.sendToTop(target);// 将data在拓扑上置顶，显示在最顶层 不会被别的节点遮盖
    }
    else if (e.kind === 'endMove') {// 结束移动
        var rack = self.showDragHelper(e.event, uHeight);
        if (dm.contains(self.dragHelper)) {// 判断容器是否包含该data对象
            target.p(self.dragHelper.p());// 设置节点的坐标
            target.a('uindex', rack.getCellIndex(target.p().y));// 设置节点的业务属性 uindex
            dm.remove(self.dragHelper);// 移除
            self._savable = true;
            self.toolbar.iv();
            target.setHost(rack);// 设置宿主节点
            target.setParent(rack);// 设置父亲节点
            // update tree
            var treeNode = target.a('treeNode');// 获取拓扑图上对应的树上的节点
            treeNode.setParent(rack.a('treeNode'));
        }
        else {
            target.p(self._oldPosition);
        }
    }
}
```
代码中的 showDragHelper 就是在设备拖动的过程中，显示在机柜上，设备下的作为占位的绿色的矩形，为了方面看到当前移动的位置在机柜上显示的位置。有兴趣的可以自己了解一下，篇幅有限，这里就不提了。
### 列表组件过滤
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180506101933687-374866782.gif)
会不会有同学对列表栏顶部的 form 表单做过滤有些好奇？这块代码非常简单，只需要对选中的类型进行过滤即可：
```
listView.setVisibleFunc(function(data){// 设置可见过滤器
    if (!self.listTypeFilter || self.listTypeFilter === -1)
        return true;
    return data.a('type') === self.listTypeFilter;// 根据节点的自定义属性 type 来判断节点属于哪个类型 返回与当前 form 表单中选中的名称相同的所有节点进行显示
});
```
主要的代码就解释到这里，其他部分的内容有兴趣的同学可以自己去抠代码了解。
工业互联网可视化监控除了 2D 机房的监控，当然还有 3D 的，涉及到工业生产以及建设的方方面面，目前 3D 的占比比较重，比较直观，视觉冲击也相对来说比较强，比如动车的整体构造体现：
![](https://images2018.cnblogs.com/blog/591709/201807/591709-20180708212224012-2044291633.png)
![](https://images2018.cnblogs.com/blog/591709/201807/591709-20180708212241435-1846718484.png)
利用工业互联网监控的 3D 工业园：
![](https://images2018.cnblogs.com/blog/591709/201807/591709-20180708212600006-1807252564.png)
工业互联网云平台可涉及到工业生产的方方面面，应用在工厂以及内部设备工作运行以及所回传数据的可视化，可以将任一时间段的数据以轨迹图、仪表盘等各种方式加以展现，工业互联网云平台可以广泛应用于智慧城市的各个领域，除了本例中展示的机房 U 位监控，拓扑图展示，数据的传输意外，其他的工业领域，三维建模的展示以及能源和通讯领域中，甚至轨道交通，医疗领域此工业互联网云平台都可应用。
