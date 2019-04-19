# 基于 HTML5 WebGL 的 3D 仓储管理系统 - xhload3d - 博客园
仓储管理系统（WMS）是一个实时的计算机软件系统，它能够按照运作的业务规则和运算法则，对信息、资源、行为、存货和分销运作进行更完美地管理，使其最大化满足有效产出和精确性的要求。从财务软件、进销存软件CIMS，从MRP、MRPII到ERP，代表了中国企业从粗放型管理走向集约管理的要求，竞争的激烈和对成本的要求使得管理对象表现为：整和上游、企业本身、下游一体化供应链的信息和资源。而仓库，尤其是制造业中的仓库，作为链上的节点，不同链节上的库存观不同，在物流供应链的管理中，不再把库存作为维持生产和销售的措施，而将其作为一种供应链的平衡机制，其作用主要是协调整个供应链。但现代企业同时又面临着许多不确定因素，无论他们来自分供方还是来自生产或客户，对企业来说处理好库存管理与不确定性关系的唯一办法是加强企业之间信息的交流和共享，增加库存决策信息的透明性、可靠性和实时性。而这，正是WMS所要帮助企业解决的问题。
WMS软件和进销存管理软件的最大区别在于：进销存软件的目标是针对于特定对象（如仓库）的商品、单据流动，是对于仓库作业结果的记录、核对和管理——报警、报表、结果分析，比如记录商品出入库的时间、经手人等；而WMS软件则除了管理仓库作业的结果记录、核对和管理外最大的功能是对仓库作业过程的指导和规范：即不但对结果进行处理，更是通过对作业动作的指导和规范保证作业的准确性、速度和相关记录数据的自动登记（入计算机系统），增加仓库的效率、管理透明度、真实度降低成本比如通过无线终端指导操作员给某定单发货：当操作员提出发货请求时，终端提示操作员应到哪个具体的仓库货位取出指定数量的那几种商品，扫描货架和商品条码核对是否正确，然后送到接货区，录入运输单位信息，完成出货任务，重要的是包括出货时间、操作员、货物种类、数量、产品序列号、承运单位等信息在货物装车的同时已经通过无线方式传输到了计算机信息中心数据库。
由于市场需求量较大，我们来好好解析今天这个例子。
本例地址：[http://www.hightopo.com/demo/warehouse/index.html](http://www.hightopo.com/demo/warehouse/index.html)
动图如下：
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171121105130774-720911354.gif)
可以在 [http://download.csdn.net/download/u013161495/10136727](http://download.csdn.net/download/u013161495/10136727) 下载代码。具体运行代码请参考 readme.html。
这个例子是采用 es6 的模块化的方式部署的。打开 index.html 进入 lib/index.js，源码是在 src 文件夹中，我们直接进 src/view 下的 index.js 
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171121110235915-335591292.png)
 在顶部加载其他模块中含有 export 接口的模块：
```
import sidebar from './sidebar.js';
import header from './header.js';
import BorderLayout from './common/BorderLayout.js';
import shelfPane from './common/shelfPane.js';
import chartPane from './common/chartPane.js';
import graph3dView from './3d/index';
```
我们将页面上的每个部分分开来放在不同的 js 文件中，就是上面加载的 js export 的部分，根层容器 BorderLayout（整体最外层的 div），整张图上的部分都是基于 borderLayout 的。
- 最外层容器 BorderLayout 是在 src/view/common 下的 BorderLayout.js 中自定义的类，其中 ht.Default.def(className, superClass, methods) 是 HT 中封装的自定义类的函数，其中 className 为自定义类名， superClass 为要继承的父类，methods 为方法和变量声明，要使用这个方法要先在外部定义这个函数变量，通过 functionName.superClass.constructor.call(this) 方法继承。BorderLayout 自定义类继承了 ht.ui.drawable.BorderLayout 布局组件，此布局器将自身空间划分为上、下、左、右、中间五个区域，每个区域可以放置一个子组件。为了能正常交互，重写 getSplitterAt 函数将 splitterRect 的宽度修改为 10，以及为了调整左侧 splitterCanvas 的尺寸，以便挡住子组件而重写的 layoutSplitterCanvas 两个方法：
```
let BorderLayout = function() {
    BorderLayout.superClass.constructor.call(this);
    this.setContinuous(true);
    this.setSplitterSize(0);
};
ht.Default.def(BorderLayout, ht.ui.BorderLayout, {//自定义类
    /**
     * splitter 宽度都为 0，为了能正常交互，重写此函数将 splitterRect 的宽度修改为 10
     * @override
     */
    getSplitterAt: function (event) {//获取事件对象下分隔条所在的区域
        var leftRect = this._leftSplitterRect, lp;
        if (leftRect) {
            leftRect = ht.Default.clone(leftRect);
            leftRect.width = 10;
            leftRect.x -= 5;
            if (event instanceof Event)
                lp = this.lp(event);
            else
                lp = event;
            if (ht.Default.containsPoint(leftRect, lp)) return 'left';
        }
        return BorderLayout.superClass.getSplitterAt.call(this, event);
    },
    /**
     * 调整左侧 splitterCanvas 的尺寸，以便挡住子组件
     * @override
     */
    layoutSplitterCanvas: function(canvas, x, y, width, height, region) {
        if (region === 'left') {
            canvas.style.pointerEvents = '';
            canvas.style.display = 'block';
            ht.Default.setCanvas(canvas, 10, height);
            canvas.style.left = this.getContentLeft() + this.tx() + x - 5 + 'px';
            canvas.style.top = this.getContentTop() + this.ty() + y + 'px';
        }
        else {
            BorderLayout.superClass.layoutSplitterCanvas.call(this, canvas, x, y, width, height, region);
        }
    }
});
export default BorderLayout;
```
- 左侧栏 sidebar，分为 8 个部分：顶部 logo、货位统计表格、进度条、分割线、货物表格、图表、管理组、问题反馈按钮等。
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171121115331790-1519663347.png)
可以查看 src/view 下的 sidebar.js 文件，这个 js 文件中同样加载了  src/view/common 下的TreeHoverBackgroundDrawable.js 和 ProgressBarSelectBarDrawable.js 中的  TreeHoverBackgroundDrawable 和 ProgressBarSelectBarDrawable 变量，以及 src/controller 下的 sidebar.js 中的 controller 变量：
```
import TreeHoverBackgroundDrawable from './common/TreeHoverBackgroundDrawable.js';
import ProgressBarSelectBarDrawable from './common/ProgressBarSelectBarDrawable.js';
import controller from '../controller/sidebar.js';
```
HT 封装了一个 ht.ui.VBoxLayout 函数，用来将子组件放置在同一垂直列中，我们可以将左侧栏要显示的部分都放到这个组件中，这样所有的部分都是以垂直列排布：
```
let vBoxLayout = new ht.ui.VBoxLayout();//此布局器将子组件放置在同一垂直列中；
vBoxLayout.setBackground('#17191a');
```
顶部 logo 是根据在 Label 标签上添加 icon 的方法来实现的，并将这个 topLabel 添加进垂直列 vBoxLayout 中：
```
let topLabel = new ht.ui.Label(); //标签组件
topLabel.setText('Demo-logo');//设置文字内容
topLabel.setIcon('imgs/logo.json');//设置图标，可以是颜色或者图片等
topLabel.setIconWidth(41);
topLabel.setIconHeight(37);
topLabel.setTextFont('18px arial, sans-serif');
topLabel.setTextColor('#fff');
topLabel.setPreferredSize(1, 64);//组件自身最合适的尺寸
topLabel.setBackground('rgb(49,98,232)');
vBoxLayout.addView(topLabel, {//将子组件加到容器中
    width: 'match_parent'//填满父容器 
});
```
对于“货位统计表格”，我们采用的是 HT 封装的 TreeTableView 组件，以树和表格的组合方式呈现 DataModel 中数据元素属性及父子关系，并将这个“树表”添加进垂直列 vBoxLayout 中：
```
let shelfTreeTable = new ht.ui.TreeTableView();//树表组件，以树和表格的组合方式呈现 DataModel 中数据元素属性及父子关系
shelfTreeTable.setHoverBackgroundDrawable(new TreeHoverBackgroundDrawable('#1ceddf', 2));//设置 hover 状态下行选中背景的 Drawable 对象
shelfTreeTable.setSelectBackgroundDrawable(new TreeHoverBackgroundDrawable('#1ceddf', 2));//设置行选中背景的 Drawable 对象 参数为“背景
shelfTreeTable.setBackground(null);
shelfTreeTable.setIndent(20);//设置不同层次的缩进值
shelfTreeTable.setColumnLineVisible(false);//设置列线是否可见
shelfTreeTable.setRowLineVisible(false);
shelfTreeTable.setExpandIcon('imgs/expand.json');//设置展开图标图标，可以是颜色或者图片等
shelfTreeTable.setCollapseIcon('imgs/collapse.json');//设置合并图标图标，可以是颜色或者图片等
shelfTreeTable.setPreferredSizeRowCountLimit();//设置计算 preferredSize 时要限制的数据行数
shelfTreeTable.setId('shelfTreeTable');
vBoxLayout.addView(shelfTreeTable, {
    width: 'match_parent',
    height: 'wrap_content',//组件自身首选高度
    marginTop: 24,
    marginLeft: 4, 
    marginRight: 4
});
```
我们在设置“行选中”时背景传入了一个 TreeHoverBackgroundDrawable 对象，这个对象是在 src\view\common 下的 TreeHoverBackgroundDrawable.js 文件中定义的，其中 ht.Default.def(className, superClass, methods) 是 HT 中封装的自定义类的函数，其中 className 为自定义类名， superClass 为要继承的父类，methods 为方法和变量声明，要使用这个方法要先在外部定义这个函数变量，通过 functionName.superClass.constructor.call(this) 方法继承。TreeHoverBackgroundDrawable 自定义类继承了 ht.ui.drawable.Drawable 组件用于绘制组件背景、图标等，只重写了 draw 和 getSerializableProperties 两个方法，我们在 draw 方法中重绘了 shelfTreeTable 的行选中背景色，并重载了  getSerializableProperties 序列化组件函数，并将 TreeHoverBackgroundDrawable 传入的参数作为 map 中新添加的属性：
```
let TreeHoverBackgroundDrawable = function(color, width) {
    TreeHoverBackgroundDrawable.superClass.constructor.call(this);
    this.setColor(color);
    this.setWidth(width);
};
ht.Default.def(TreeHoverBackgroundDrawable, ht.ui.drawable.Drawable, {
    ms_ac: ['color', 'width'],
    draw: function(x, y, width, height, data, view, dom) {
        var self = this,
            g = view.getRootContext(dom),
            color = self.getColor();
       
        g.beginPath();
        g.fillStyle = color;
        g.rect(x, y, self.getWidth(), height);
        g.fill();
    },
    getSerializableProperties: function() {
        var parentProperties = TreeHoverBackgroundDrawable.superClass.getSerializableProperties.call(this);
        return addMethod(parentProperties, {
            color: 1, width: 1
        });
    }
});
```
记住要导出 TreeHoverBackgroundDrawable ：
```
export default TreeHoverBackgroundDrawable;
```
HT 还封装了非常好用的 ht.ui.ProgressBar 组件，可直接绘制进度条：
```
let progressBar = new ht.ui.ProgressBar();
progressBar.setId('progressBar');
progressBar.setBackground('#3b2a00');//设置组件的背景，可以是颜色或者图片等
progressBar.setBar('rgba(0,0,0,0)');//设置进度条背景，可以是颜色或者图片等
progressBar.setPadding(5);
progressBar.setSelectBarDrawable(new ProgressBarSelectBarDrawable('#c58348', '#ffa866')); //设置前景(即进度覆盖区域)的 Drawable 对象，可以是颜色或者图片等
progressBar.setValue(40);//设置当前进度值
progressBar.setBorderRadius(0);
vBoxLayout.addView(progressBar, {
    marginTop: 24,
    width: 'match_parent',
    height: 28,
    marginBottom: 24,
    marginLeft: 14,
    marginRight: 14
});
```
我们在 设置“前景”的时候传入了一个 ProgressBarSelectBarDrawable 对象，这个对象在 src\view\common 下的 ProgressBarSelectBarDrawable.js 中定义的。具体定义方法跟上面的 TreeHoverBackgroundDrawable 函数对象类似，这里不再赘述。
分割线的制作最为简单，只要将一个矩形的高度设置为 1 即可，我们用 ht.ui.View() 组件来制作：
```
let separator = new ht.ui.View();//所有视图组件的基类，所有可视化组件都必须从此类继承
separator.setBackground('#666');
vBoxLayout.addView(separator, {
    width: 'match_parent',
    height: 1,
    marginLeft: 14, 
    marginRight: 14,
    marginBottom: 24
});
```
货物表格的操作几乎和货位统计表格相同，这里不再赘述。
我们将一个 json 的图表文件当做图片传给图表的组件容器作为背景，也能很轻松地操作：
```
let chartView = new ht.ui.View();
chartView.setBackground('imgs/chart.json');
vBoxLayout.addView(chartView, {
    width: 173,
    height: 179,
    align: 'center',
    marginBottom: 10
});
```
管理组和顶部 logo 的定义方式类似，这里不再赘述。
问题反馈按钮，我们将这个部分用 HT 封装的 ht.ui.Button 组件来制作，并将这个部分添加进垂直列 vBoxLayout 中：
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171121162941321-2020856671.png)
```
let feedbackButton = new ht.ui.Button();//按钮类
feedbackButton.setId('feedbackButton');
feedbackButton.setText('问题反馈：service@hightopo.com');
feedbackButton.setIcon('imgs/em.json');
feedbackButton.setTextColor('#fff');
feedbackButton.setHoverTextColor(shelfTreeTable.getHoverLabelColor());//设置 hover 状态下文字颜色
feedbackButton.setActiveTextColor(feedbackButton.getHoverTextColor());//设置 active 状态下文字颜色
feedbackButton.setIconWidth(16);
feedbackButton.setIconHeight(16);
feedbackButton.setIconTextGap(10);
feedbackButton.setAlign('left');
feedbackButton.setBackground(null);
feedbackButton.setHoverBackground(null);
feedbackButton.setActiveBackground(null);
vBoxLayout.addView(feedbackButton, {
    width: 'match_parent',
    marginTop: 5,
    marginBottom: 10,
    marginLeft: 20
});
```
视图部分做好了，在模块化开发中，controller 就是做交互的部分，shelfTreeTable 货位统计表格， cargoTreeTable 货物表格， feedbackButton 问题反馈按钮， progressBar  进度条四个部分的交互都是在在 src/controller 下的 sidebar.js 中定义的。通过 findViewById(id, recursive) 根据id查找子组件，recursive 表示是否递归查找。
shelfTreeTable 货位统计表格的数据绑定传输方式与 cargoTreeTable 货物表格类似，这里我们只对 shelfTreeTable 货位统计表格的数据绑定进行解析。shelfTreeTable 一共有三列，其中不同的部分只有“已用”和“剩余”两个部分，所以我们只要将这两个部分进行数据绑定即可，先创建两列：
```
let column = new ht.ui.Column();//列数据，用于定义表格组件的列信息
column.setName('used');//设置数据元素名称
column.setAccessType('attr');//在这里 name 为 used，采用 getAttr('used') 和 setAttr('used', 98) 的方式存取 set/getAttr 简写为 a
column.setWidth(65);
column.setAlign('center');
columnModel.add(column);
column = new ht.ui.Column();
column.setName('remain');
column.setAccessType('attr');
column.setWidth(65);
column.setAlign('center');
columnModel.add(column);
```
接着遍历 json 文件，将 json 文件中对应的 used、remain以及 labelColors 通过 set/getAttr 或 简写 a 的方式进行数据绑定：
```
for (var i = 0; i < json.length; i++) {
    var row = json[i];//获取 json 中的属性
    var data = new ht.Data();
    data.setIcon(row.icon);//将 json 中的 icon 传过来
    data.setName(row.name);
    data.a('used', row.used);
    data.a('remain', row.remain);
    data.a('labelColors', row.colors);
    data.setIcon(row.icon);
    treeTable.dm().add(data);//在树表组件的数据模型中添加这个data节点
    var children = row.children;
    if (children) {
        for (var j = 0; j < children.length; j++) {
            var child = children[j];
            var childData = new ht.Data();
            childData.setName(child.name);
            childData.setIcon(child.icon);
            childData.a('used', child.used);
            childData.a('remain', child.remain);
            childData.a('labelColors', child.colors);
            childData.setParent(data);
            treeTable.dm().add(childData);
        }
    }
}
```
最后在 controller 函数对象中调用 这个函数：
```
initTreeTableDatas(shelfTreeTable, json);//json 为 ../model/shelf.json传入
```
progressBar 进度条的变化是通过设置定时器改变 progressBar 的 value 值来动态改变的：
```
setInterval(() => {
    if (progressBar.getValue() >= 100) {
        progressBar.setValue(0);
    }
    progressBar.setValue(progressBar.getValue() + 1);
}, 50);
```
feedbackButton 问题反馈按钮，通过增加 View 事件监听器来监听按钮的点击事件：
```
feedbackButton.addViewListener(e => {
    if (e.kind === 'click') {//HT 自定义的事件属性，具体查看 [http://hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html](http://hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)
        window.location.href = "mailto:service@www.hightopo.com";//当前页面打开URL页面
    }
});
```
- 右侧根容器 splitLayout
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171121115445680-1296456808.png)
直接用的分割组件 ht.ui.SplitLayout 进行分割布局：
```
let splitLayout = new ht.ui.SplitLayout();//此布局器将自身空间划分为上、下两个区域或左、右两个区域，每个区域可以放置一个子组件
splitLayout.setSplitterVisible(false);
splitLayout.setPositionType('absoluteFirst');
splitLayout.setOrientation('v');
```
- 右侧头部 header
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171121115532305-2129048603.png)
这个 header 是从 src/view 下的 header.js 中获取的对象，为 ht.ui.RelativeLayout 相对定位布局器，分为 5 个部分：searchField 搜索框、titleLabel 主标题、temperatureLabel1 温度、humidityLabel1 湿度以及 airpressureLabel1 气压。
这里我们没有对“搜索框” searchField 进行数据绑定，以及搜索的功能，这只是一个样例，不涉及业务部分：
```
let searchField = new ht.ui.TextField();//文本框组件
searchField.setBorder(new ht.ui.border.LineBorder(1, '#d8d8d8'));//在组件的画布上绘制直线边框
searchField.setBorderRadius(12);
searchField.setBackground(null);
searchField.setIcon('imgs/search.json');
searchField.setIconPosition('left');
searchField.setPadding([2, 16, 2, 16]);
searchField.setColor('rgb(138, 138, 138)');
searchField.setPlaceholder('Find everything...');
searchField.getView().className = 'search';
header.addView(searchField, {
    width: 180,
    marginLeft: 20,
    vAlign: 'middle'
});
```
对于 titleLabel 主标题比较简单，和温度、湿度以及气压类似，我就只说明一下主标题 titleLabel 的定义：
```
let titleLabel = new ht.ui.Label();//标签组件
titleLabel.setId('title');
titleLabel.setIcon('imgs/expand.json');
titleLabel.setTextColor('rgb(138, 138, 138)');
titleLabel.setText('杭州仓库');
titleLabel.setHTextPosition('left');//设置文字在水平方向相对于图标的位置，默认值为 'right'
titleLabel.setIconTextGap(10);//设置图标和文字之间的间距
titleLabel.setBorder(new ht.ui.border.IndividualLineBorder(0, 0, 3, 0, '#3162e8'))//在组件的画布上绘制直线边框；与 LineBorder 不同的是，此边框可以单独绘制某一个或几个方向的边框
titleLabel.setTextFont('16px arial');
header.addView(titleLabel, {
    height: 'match_parent',
    width: 'wrap_content',
    align: 'center'
});
```
然后交互部分在 src/controller 下的 header.js 中做了右键点击出现菜单栏以及单击 titleLabel 的位置出现下拉菜单两种交互，通过控制鼠标的点击事件来控制事件的交互：
```
let title, contextMenu;
export default function controller (view) {
    title = view.findViewById('title');
    contextMenu = new ht.ui.ContextMenu();//右键菜单组件
    contextMenu.setLabelColor('rgb(138, 138, 138)');
    contextMenu.setHoverRowBackground('#3664e4');
    contextMenu.setItems([
        {
            label: '北京仓库'
        },
        {
            label: '上海仓库'
        },
        {
            label: '厦门仓库'
        }
    ]);
    contextMenu.addViewListener((e) => {
        if (e.kind === 'action') {//HT 自定义的事件类型
            title.setText(e.item.label);
        }
    });
    title.getView().addEventListener('mousedown', e => {
        if (contextMenu.isInDOM()) {//判断组件是否在 DOM 树中
            contextMenu.hide();//隐藏菜单
            document.removeEventListener('mousedown', handleWindowClick);//移除mousedown监听事件
        }
        else {//没有右键点击过
            var items = contextMenu.getItems();
            for (var i = 0; i < items.length; i++) {
                items[i].width = title.getWidth();
            }
            let windowInfo = ht.Default.getWindowInfo(),//获取当前窗口left|top|width|height的参数信息
                titleRect = title.getView().getBoundingClientRect();
            contextMenu.show(windowInfo.left + titleRect.left, windowInfo.top + titleRect.top + titleRect.height);
            document.addEventListener('mousedown', handleWindowClick);
        }
    });
}
function handleWindowClick(e) {
    if (!contextMenu.getView().contains(e.target) && !title.getView().contains(e.target)) {//判断元素是否在数组中
        contextMenu.hide();
        document.removeEventListener('mousedown', handleWindowClick);
    }
}
```
- 右侧下部分 RelativeLayout 相对布局器（相对于右侧下部分最根层 div），包含中间显示 3d 部分 graph3dView、双击货柜或货物才会出现的 shelfPane、以及出现在右下角的图表 chartPane，将这三部分添加进 RelativeLayout 相对布局容器：
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171121115837055-1200527140.png)
```
let relativeLayout = new ht.ui.RelativeLayout();//创建相对布局器
relativeLayout.setId('contentRelative');
relativeLayout.setBackground('#060811');
var htView = new ht.ui.HTView(graph3dView);
htView.setId('contentHTView');
relativeLayout.addView(htView, {//将 3d 组件添加进relativeLayout 相对布局器
    width: 'match_parent',
    height: 'match_parent'
});
relativeLayout.addView(shelfPane, {//将双击出现的详细信息 shelfPane 组件添加进relativeLayout 相对布局器
    width: 220,
    height: 'wrap_content',
    align: 'right',
    marginRight: 30,
    marginTop: 30
});
relativeLayout.addView(chartPane, {//将图表 chartPane 组件添加进relativeLayout 相对布局器
    width: 220,
    height: 200,
    align: 'right',
    vAlign: 'bottom',
    marginRight: 30,
    marginBottom: 30
})
```
然后将右侧相对布局器 relativeLayout 和右侧头部 header 添加进右侧底部容器 splitLayout：
```
let splitLayout = new ht.ui.SplitLayout();
splitLayout.setSplitterVisible(false);
splitLayout.setPositionType('absoluteFirst');
splitLayout.setOrientation('v');
splitLayout.addView(header, {
    region: 'first'//指定组件所在的区域，可选值为：'first'|'second'
});
splitLayout.addView(relativeLayout, {
    region: 'second'
});
```
再将左侧部分的 sidebar 和右侧部分的所有也就是 splitLayout 添加进整个底部容器 borderLayout，再将底部容器添加进 html body 体中：
```
let borderLayout = new BorderLayout();
borderLayout.setLeftWidth(250);
borderLayout.addView(sidebar, {
    region: 'left',// 指定组件所在的区域，可选值为：'top'|'right'|'bottom'|'left'|'center'
    width: 'match_parent'//组件自身首选宽度 
});
borderLayout.addView(splitLayout, {
    region: 'center'
});
borderLayout.addToDOM();//将 borderLayout 添加进 body 体中
```
 我们具体说说这个相对布局器内部包含的 3d 部分 graph3dView、双击货柜或货物才会出现的 shelfPane、以及出现在右下角的图表 chartPane。
(1) graph3dView
从 src\view\3d 文件夹中的 index.js 中获取 graph3dView 的外部接口被 src/view 中的 index.js 调用：
```
import graph3dView from './3d/index';
```
![](https://images2018.cnblogs.com/blog/591709/201711/591709-20171127162114909-277068225.png)
从这个 3d 场景中可以看到，我们需要“地板”、“墙面”、“货架”、“叉车”、“货物”以及 3d 场景。
在 3d 文件夹下的 index.js 中，我们从文件夹中导入所有需要的接口：
```
import {//这里导入的都是一些基础数据
  sceneWidth, sceneHeight, sceneTall,
  toShelfList, randomCargoType
} from './G.js';
// 模拟数据接口
import {
  stockinout,// 出入库
  initiate,// 初始化
  inoutShelf// 上下架
} from './interfaces';
import { Shelf } from './shelf';//货架
import { Floor } from './floor';//地板
import { Wall } from './wall';//墙面
import { Car } from './car';//叉车
import { g3d } from './g3d';//3d场景
import { getCargoById } from './cargo';//货物
```
g3d.js 文件中只设置了场景以及对部分事件的监听：
```
g3d.mi((e) => {// 监听事件 addInteractorListener
  const kind = e.kind;
  if (kind === 'doubleClickData') {//双击图元事件
    let data = e.data;//事件相关的数据元素
    if (data instanceof Shelf) {//如果是货架
      data.setTransparent(false);
      eventbus.fire({ type: 'cargoBlur' });//派发事件，依次调用所有的监听器函数
    }
    else {
      data = data.a('cargo');
      if (!data) return;
      data.transparent = false;
      eventbus.fire({ type: 'cargoFocus', data: data });
    }
    for (let i = shelfList.length - 1; i >= 0; i--) {//除了双击的图元，其他的图元都设置透明
      const shelf = shelfList[i];
      shelf.setTransparent(true, data);
    }
    return;
  }
  if (kind === 'doubleClickBackground') {//双击背景事件
    for (let i = shelfList.length - 1; i >= 0; i--) {//双击背景，所有的图元都不透明
      const shelf = shelfList[i];
      shelf.setTransparent(false);
    }
    eventbus.fire({ type: 'cargoBlur' });
    return;
  }
});
```
我们在 G.js 中定义了一些基础数据，其他引用的 js 中都会反复调用这些变量，所以我们先来解析这个文件：
```
const sceneWidth = 1200;//场景宽度
const sceneHeight = 800;//场景高度
const sceneTall = 410;//场景的深度
const globalOpacity = 0.3;//透明度
const cargoTypes = {//货物类型，分为四种
  'cask': {//木桶
    'name': 'bucket'
  },
  'carton': {//纸箱
    'name': 'carton'
  },
  'woodenBox1': {//木箱1
    'name': 'woodenBox1'
  },
  'woodenBox2': {//木箱2
    'name': 'woodenBox2'
  }
};
```
里面有三个函数，分别是“货架的 obj 分解”、“加载模型”、“随机分配货物的类型”：
```
function toShelfList(list) {//将货架的 obj 分解，
  const obj = {};
  list.forEach((o) => {//这边的参数o具体内容可以查看 view/3d/interface.js
    const strs = o.cubeGeoId.split('-');
    let rs = obj[o.rackId];
    if (!rs) {
      rs = obj[o.rackId] = [];
    }
    const ri = parseInt(strs[2].substr(1)) - 1;
    let ps = rs[ri];
    if (!ps) {
      ps = rs[ri] = [];
    }
    let type = 'cask';
    if (o.inventoryType === 'Import') {
      while((type = randomCargoType()) === 'cask') {}
    }
    const pi = parseInt(strs[3].substr(1)) - 1;
    ps[pi] = {
      id: o.cubeGeoId,
      type: type
    };
  });
  return obj;
}
function loadObj(shape3d, fileName, cbFunc) {//加载模型
  const path = './objs/' + fileName;
  ht.Default.loadObj(path + '.obj', path + '.mtl', {
    shape3d: shape3d,
    center: true,
    cube: true,
    finishFunc: cbFunc
  });
}
function randomCargoType() {//随机分配“货物”的类型
  const keys = Object.keys(cargoTypes);
  const i = Math.floor(Math.random() * keys.length);
  return keys[i];
}
```
 这个 3d 场景中还有不可缺少的“货物”和“货架”以及“叉车”，三者的定义方式类似，这里只对“货架”进行解释。我们直接在“货物”的 js 中引入底下的“托盘”的 js 文件，将它们看做一个整体：
```
import { Pallet } from './pallet';
import {
  cargoTypes,
  loadObj,
  globalOpacity
} from './G';
```
在 src\view\3d\cargo.js 文件中，定义了一个“货物”类，这个类中声明了很多方法，比较基础，有需要的自己可以查看这个文件，这里我不过多解释。主要讲一下如何加载这个“货物”的 obj，我们在 G.js 文件中有定义一个 loadObj 函数，我们在代码顶部也有引入，导入 obj 文件之后就在“货物”的库存增加这个“货物”：
```
for (let type in cargoTypes) {//遍历 cargoTypes 数组， G.js 中定义的
  const cargo = cargoTypes[type];
  loadObj(type, cargo.name, (map, array, s3) => {//loadObj(shape3d, fileName, cbFunc) cbFunc 中的参数可以参考 [obj 手册](http://hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html#ref_loadobj)
    cargo.s3 = s3;//将 cargo 的 s3 设置原始大小
    updateCargoSize();
  });
}
function updateCargoSize() {
  let c, obj;
  for (let i = cargoList.length - 1; i >= 0; i--) {
    c = cargoList[i];
    obj = cargoTypes[c.type];
    if (!obj.s3) continue;
    c.boxS3 = obj.s3;
  }
}
```
 还有就是界面上“货物”的进出库的动画，主要用的方法是 HT 封装的 ht.Default.startAnim 函数（[HT for Web 动画手册](http://hightopo.com/guide/guide/plugin/animation/ht-animation-guide.html)），出的动画与进的动画类似，这里不赘述：
```
// 货物进
  in() {
    if (anim) {//如果有值，就停止动画
      anim.stop(true);
    }
    this.x = this.basicX + moveDistance;
    this.opacity = 1;
    anim = ht.Default.startAnim({
      duration: 1500,
      finishFunc: () => {//动画结束之后调用这个函数，将anim设置为空停止动画
        anim = null;
      },
      action: (v, t) => {
        this.x = this.basicX + (1 - v) * moveDistance;//改变x坐标，看起来像向前移动
      }
    });
  }
```
墙和地板也是比较简单的，简单地继承 ht.Node 和 ht.Shape，这里以“墙”进行解释，继承之后直接在构造函数中进行属性的设置即可：
```
class Wall extends ht.Shape {//继承 ht.Shape 类
  constructor(points, segments, tall, thickness, elevation) {
    super();
    this.setPoints(points);//设置“点”
    this.setSegments(segments);//设置“点之间的连接方式”
    this.setTall(tall);//控制Node图元在y轴的长度
    this.setThickness(thickness);//设置“厚度”
    this.setElevation(elevation);//控制Node图元中心位置所在3D坐标系的y轴位置
    this.s({
      'all.transparent': true,//六面透明
      'all.opacity': 0.3,//透明度为 0.3
      'all.reverse.flip': true,//六面的反面显示正面的内容
      'bottom.visible': false//底面不可见
    });
  }
}
```
floor、wall、shelf 以及 car 这四个类都准备完毕，只需要在 src\view\3d\index.js 中 new 一个新的对象并加入到数据模型 dataModel 中即可，这里只展示 car “叉车”的初始化代码：
```
// init Car
const car = new Car();
car.addToDataModel(dm);
```
至于“货物”，我们在这个 js 上是采用定时器调用 in 和 out 方法，这里有一个模拟的数据库 interfaces.js 文件，有需求的可以看一下，这里我们只当数据来调用（进出库和上下架类似，这里只展示进出库的设置方法）：
```
// 轮训掉用出入库接口
setInterval(() => {
  const obj = stockinout();//出入库
  let type = 'cask';
  if (obj.inventoryType === 'Import') {
    while((type = randomCargoType()) === 'cask') {}//如果为“货物”类型为“木桶”
  }
  car.cargoType = type;
  if (obj.inOutStatus === 'I')//如果值为 “I”，则进库
    car.in();
  else//否则为“o”，出库
    car.out();
}, 30000);
```
(2) shelfPane
从 src\view\common 文件夹中的 shelfPane.js 中获取 graph3dView 的外部接口被 src/view 中的 index.js 调用：
```
import shelfPane from './common/shelfPane.js';
```
![](https://images2018.cnblogs.com/blog/591709/201711/591709-20171128221442987-1542412311.png)
shelfPane 是基于 Pane 类的，在 shelfPane.js 文件中引入这个类和事件派发器：
```
import Pane from './Pane.js';
import eventbus from '../../controller/eventbus';
```
Pane 类继承于 HT 封装的 ht.ui.TabLayout 类， 并做了一些特定的属性设置：
```
class Pane extends ht.ui.TabLayout {
    constructor() {
        super();
        this.setBorder(new ht.ui.border.LineBorder(1, 'rgb(150,150,150)'));//设置组件的边框
        this.setTabHeaderBackground(null);//设置标签行背景，可以是颜色或者图片等
        this.setHoverTabBackground(null);//设置 Hover 状态下的标签背景，可以是颜色或者图片等
        this.setActiveTabBackground(null);//设置 Active 状态下的标签背景，可以是颜色或者图片等
        this.setTitleColor('rgb(184,184,184)');//设置正常状态下标签文字的颜色
        this.setActiveTitleColor('rgb(255,255,255)');//设置 Active 状态下标签文字的颜色
        this.setTabHeaderLineSize(0);//设置标签行分割线宽度
        this.setMovable(false);//设置标签是否可拖拽调整位置，默认为 true
        this.setTabHeaderBackground('#1c258c');//设置标签行背景，可以是颜色或者图片等
        this.setTabGap(0);//设置标签之间的距离
    }
    getTabWidth(child) {//获取指定子组件的标签宽度
        const children = this.getChildren(),
            size = children.size();
        if (size === 0) {
            return this.getContentWidth();//获取内容宽度，即组件宽度减去边框宽度和左右内边距宽度
        }
        else {
            return this.getContentWidth() / size;
        }
    }
    drawTab(g, child, x, y, w, h) {//绘制标签
        const children = this.getChildren(),//获取子组件列表
            size = children.size(),
            color = this.getCurrentTitleColor(child),//根据参数子组件的状态(normal、hover、active、move)，获取标签文字颜色
            font = this.getTitleFont(child),//获取标签文字字体
            title = this.getTitle(child);//获取指定子组件的标签文本
        if (size === 1) {
            ht.Default.drawText(g, title, font, color, x, y, w, h, 'left');//绘制文字
        }
        else {
            ht.Default.drawText(g, title, font, color, x, y, w, h, 'center');
        }
        if (children.indexOf(child) <  size - 1) {
            g.beginPath();//开始绘制
            g.rect(x + w - 1, y + 4, 1, h - 8);
            g.fillStyle = 'rgb(150,150,150)';
            g.fill();
        }
    }
    show() {
        this.setVisible(true);//设置组件是否可见
    }
    hide() {
        this.setVisible(false);
    }
}
```
我们这个例子中的“信息”列表是一个表格组件，HT 通过 ht.ui.TableLayout 函数定义一个表格，然后通过 ht.ui.TableRow 向表格中添加行，这个例子中的“备注”、“编号”、“来源”、“入库”、“发往”以及“出库”都是文本框，这里拿“备注”作为举例：
```
let tableLayout = new ht.ui.TableLayout();//此布局器将自身空间按照行列数划分为 row * column 个单元格
tableLayout.setColumnPreferredWidth(0, 45);//设置列首选宽度
tableLayout.setColumnWeight(0, 0);//设置列宽度权重；如果布局器的总宽度大于所有列的首选宽度之和，那么剩余的宽度就根据权重分配
tableLayout.setColumnPreferredWidth(1, 150);
tableLayout.setPadding(8);//设置组件内边距，参数如果是数字，说明四边使用相同的内边距；如果是数组，则格式为：[上边距, 右边距, 下边距, 左边距]
// 备注
var tableRow1 = new ht.ui.TableRow();//TableLayout 中的一行子组件；
var label = new ht.ui.Label();//标签组件
label.setText('备注');//设置文字内容
label.setAlign('left');//设置文字和图标在按钮水平方向的整体对齐方式，默认为 'center'
label.setTextColor('rgb(255,255,255)');//设置文字颜色
var textField = new ht.ui.TextField();//文本框组件
textField.setFormDataName('remark');//设置组件在表单中的名称
textField.setBackground(null);//设置组件的背景，可以是颜色或者图片等；此值最终会被转换为 Drawable 对象
textField.setBorderRadius(0);//设置 CSS 边框圆角
textField.setColor('rgb(138,138,138)');//设置文字颜色
textField.setPlaceholder('无');//设置输入提示
textField.setBorder(new ht.ui.border.IndividualLineBorder(0, 0, 1, 0, 'rgb(138,138,138)'));//设置组件的边框
tableRow1.addView(label);//添加子组件
tableRow1.addView(textField);
tableLayout.addView(tableRow1);//将子组件加到容器中
```
“归类”和“模型”类似，都是下拉框，我们用 HT 封装的 ht.ui.ComboBox 组合框组件，跟 ht.ui.TextField 也是异曲同工，只是具体操作不同而已，HT 这样做使用上更简便更容易上手，这里我们以“模型”进行解析，在设置“下拉数据”的时候我们利用了 HT 中的数据绑定：
```
// 模型
var tableRow4 = new ht.ui.TableRow();
label = new ht.ui.Label();
label.setText('模型');
label.setAlign('left');
label.setTextColor('rgb(255,255,255)');
var comboBox = new ht.ui.ComboBox();
comboBox.setFormDataName('model');
comboBox.setBackground(null);
comboBox.setColor('rgb(232,143,49)');
comboBox.setDatas([////设置下拉数据数组
  { label: '纸箱', value: 'carton' },
  { label: '木箱1', value: 'woodenBox1' },
  { label: '木箱2', value: 'woodenBox2' },
  { label: '木桶', value: 'cask' }
]); 
comboBox.setIcon('imgs/combobox_icon.json');
comboBox.setHoverIcon('imgs/combobox_icon_hover.json');
comboBox.setActiveIcon('imgs/combobox_icon_hover.json');
comboBox.setBorderRadius(0);////设置 CSS 边框圆角
comboBox.setBorder(new ht.ui.border.IndividualLineBorder(0, 0, 1, 0, 'rgb(138,138,138)'));
tableRow4.addView(label);
tableRow4.addView(comboBox);
tableLayout.addView(tableRow4);
```
最后一个“染色”，HT 封装了 ht.ui.ColorPicker 颜色选择器组件，组件从 ht.ui.ComboBox 继承并使用 ht.ui.ColorDropDown 作为下拉模板，跟上面的下拉列表很类似，只是下拉的模板变了而已：
```
// 染色
var tableRow9 = new ht.ui.TableRow();
label = new ht.ui.Label();
label.setText('染色');
label.setAlign('left');
label.setTextColor('rgb(255,255,255)');
var comboBox = new ht.ui.ColorPicker();//颜色选择器组件
comboBox.setFormDataName('blend');//设置组件在表单中的名称
comboBox.getView().className = 'content_colorpicker';
comboBox.setBackground(null);
comboBox.setPreviewBackground(null);//设置预览背景；可以是颜色或者图片等
comboBox.getInput().style.visibility = 'visible';//获取组件内部的 input 框的 style 样式
comboBox.setReadOnly(true);//设置只读
comboBox.setColor('rgba(0,0,0,0)');
comboBox.setPlaceholder('修改货箱颜色');
comboBox.setIcon('imgs/combobox_icon.json');
comboBox.setHoverIcon('imgs/combobox_icon_hover.json');
comboBox.setActiveIcon('imgs/combobox_icon_hover.json');
comboBox.setBorderRadius(0);
comboBox.setBorder(new ht.ui.border.IndividualLineBorder(0, 0, 1, 0, 'rgb(138,138,138)'));
comboBox.setInstant(true);//设置即时模式；在这种模式下，每输入一个字符 value 属性变化事件就会立即被派发，否则只有失去焦点或敲回车时才被派发
tableRow9.addView(label);
tableRow9.addView(comboBox);
tableLayout.addView(tableRow9);
```
最后通过 ht.ui.Form 组件的 addChangeListener 事件监听函数监听 JSON 整体变化事件和 JSON 中单条数据变化事件，这两种事件的解释如下图：
![](https://images2018.cnblogs.com/blog/591709/201711/591709-20171128182302269-1714236080.png)
具体监听方法如下：
```
form.addChangeListener((e) => {
  const cargo = form.__cargo__;
  if (e.kind === 'formDataValueChange') {//JSON 中单条数据值变化事件
    const name = e.name;
    let value = e.newValue;
    if (name === 'blend') {
      if (value && value.startsWith('rgba')) {
        const li = value.lastIndexOf(',');
        value = 'rgb' + value.substring(value.indexOf('('), li) + ')';
      }
    }
    cargo.setValue(name, value);
  }
});
```
然后通过 HT 封装的事件派发器 ht.Notifier 将界面中不同区域的组件之间通过事件派发进行交互，根据不同的事件类型进行不同的动作：
```
eventbus.add((e) => {//增加监听器 事件总线；界面中不同区域的组件之间通过事件派发进行交互
  if (e.type === 'cargoFocus') {
    shelfPane.show();
    const cargo = e.data;
    form.__cargo__ = cargo;
    const json = form.getJSON();//获取由表单组件的名称和值组装成的 JSON 数据
    for (let k in json) {
      form.setItem(k, cargo.getValue(k));
    }
    return;
  }
  if (e.type === 'cargoBlur') {
    shelfPane.hide();
    return;
  }
});
```
(3) chartPane
 从 src\view\common 文件夹中的 chartPane.js 中获取 graph3dView 的外部接口被 src/view 中的 index.js 调用：
```
import chartPane from './common/chartPane.js';
```
 chartPane 和 shelfPane 类似，都是 Pane 类的对象，属性也类似，不同的是内容。因为今天展示的只是一个 Demo，我们并没有做过多的关于图表插件的处理，所以这里就用图片来代替动态图表，不过就算想做也是很容易的事，HT 运用第三方插件也是很容易上手的，可以看这边的例子 [http://hightopo.com/demo/large-screen/index.html](http://hightopo.com/demo/large-screen/index.html)，[HT 官网](http://hightopo.com)上有更多有趣的例子！
![](https://images2018.cnblogs.com/blog/591709/201711/591709-20171128222932784-1110219821.png)
回到正题，chartPane 图表面板的实现非常容易，将内部的子组件设置背景图片再添加进 chartPane 图表面板中即可：
```
import Pane from './Pane.js';
var chartPane = new Pane();
var view1 = new ht.ui.View();
view1.setBackgroundDrawable(new ht.ui.drawable.ImageDrawable('imgs/chart.png', 'fill'));//设置组件的背景 Drawable 对象；组件渲染时优先使用此 Drawable 对象，如果为空，再用 background 转换
var view2 = new ht.ui.View();
view2.setBackgroundDrawable(new ht.ui.drawable.ImageDrawable('imgs/chart.png', 'fill'));
chartPane.getView().style.background = 'rgba(18,28,64,0.60)';//设置背景颜色
chartPane.addView(view1, {//将子组件加到容器中
    title: '其他图表'
});
chartPane.addView(view2, {
    title: '库存负载'
});
chartPane.setActiveView(view2);//设置选中的子组件
```
整个例子解析完毕，有兴趣的小伙伴可以去 [HT 官网](http://hightopo.com)上自习查阅资料，好好品味，一定会发现更大的世界。
我的博客即将同步至腾讯云+社区，邀请大家一同入驻。

