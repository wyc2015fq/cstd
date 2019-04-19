# 基于 HTML5 WebGL 的 3D 工控裙房系统 - xhload3d - 博客园
# 前言
工业物联网在中国的发展如火如荼，网络基础设施建设，以及工业升级的迫切需要都为工业物联网发展提供了很大的机遇。中国工业物联网企业目前呈现两种发展形式并存状况：一方面是大型通讯、IT企业的布局；一方面是传统工业软件和工业网络企业自发地延伸，由产品提供商发展为方案供应商。什么叫做裙房？裙房是指附属于主高楼并与之连成一体的低层建筑。本文的 Demo 是针对于裙房做的，但是在工业监控系统中有很多雷同的部分，比如动画、点击切换、点击隐藏、故障展示、开关、数据展示等等，都是比较通用的一些功能。所以针对这个 Demo 将这些内容做一个记录，在这个 Demo 中我也遇到了一些问题，如何解决的都会拿出来跟大家分享。
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180608142203255-2086902366.gif)
[http://www.hightopo.com/demo/annexMonitor/](http://www.hightopo.com/demo/annexMonitor/)
# 代码实现
整个 Demo 我们要实现的部分有：
- 3D 场景的搭建
- 场景中叶轮的转动
- 容器水位的上升下降
- 叶轮转动故障展示
- 开启/关闭动画
- 灯光的开启/关闭
- 点击切换模型
- 点击隐藏/显示属性窗口
- 文本内容/颜色变换
上面的功能看起来蛮多的，实际上实现起来还是比较容易的，总共就用了 200+ 行的代码。
## 3D 场景搭建
 三维场景地基的搭建就 2 行代码：
```
var g3d = new ht.graph3d.Graph3dView();// Hightopo 的 3D 组件（三维场景地基）
g3d.addToDOM();// 将 3D 组将添加到 body 体中
```
HT 的组件一般都会嵌入 BorderPane（[https://hightopo.com/guide/guide/core/borderpane/ht-borderpane-guide.html](https://hightopo.com/guide/guide/core/borderpane/ht-borderpane-guide.html)）、SplitView（[https://hightopo.com/guide/guide/core/splitview/ht-splitview-guide.html](https://hightopo.com/guide/guide/core/splitview/ht-splitview-guide.html)） 和 TabView（[https://hightopo.com/guide/guide/core/tabview/ht-tabview-guide.html](https://hightopo.com/guide/guide/core/tabview/ht-tabview-guide.html)） 等容器中使用，而最外层的 HT 组件则需要用户手工将 getView() 返回的底层 div 元素添加到页面的 DOM 元素中，这里需要注意的是，当父容器大小变化时，如果父容器是 BorderPane 和 SplitView 等这些HT预定义的容器组件，则 HT 的容器会自动递归调用孩子组件 invalidate 函数通知更新。但如果父容器是原生的 html 元素， 则 HT 组件无法获知需要更新，因此最外层的 HT 组件一般需要监听 window 的窗口大小变化事件，调用最外层组件 invalidate 函数进行更新。
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
接下来我们要向场景中添加各种模型，用代码生成模型是非常无敌痛苦的，我们将整个场景的模型都放到一个 JSON 文件中，并通过 ht.Default.xhrLoad 方法将这个 JSON 转换为 3D 场景显示在界面上：
```
var dm = g3d.dm();// 获取 HT 3D 组件的数据容器
ht.Default.xhrLoad('scenes/system.json', function(text) {
    dm.deserialize(text);// 将函数的 text（json）参数传给 deserialize 反序列化方法，可将 json 内容中的元素添加到 dataModel 数据容器中进行显示
}
```
 ht.Default.xhrLoad 方法是一个异步加载 json 文件的方法，第一个参数为传入的 json 文件，路径是相对于 html 文件的，第二个参数是回调函数，在传入的 json 文件解析完毕之后做的操作。此方法为异步加载，因此需要对 dm 数据容器中的数据进行获取或操作的话，需要将获取/操作的代码写在 dm.deserialize(text) 方法之后，因为此时 dm 数据容器中才有节点。
上面将 JSON 文件发序列化到 dm 数据容器中后界面显示如下：
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180607165301770-1131693701.png)
上图中整个场景的背景是我后期用代码添加的，通过前面的 addToDOM 函数可以知道我们可以通过 getView 方法获取 HT 3D 组件的底层 div，因此要在此 div 上添加一张背景图也就不难了。剩下的 3D 模型部分都是由 JSON 反序列化出来的。
## 场景中叶轮的转动
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180607172343993-256207216.gif)
当然转动不可能是整个模型在转动，而是中间的“滚轮”在转动，这要求设计师在创建模型的时候就将这个部分分离出来，然后我给此部分设置 tag 唯一标识为“yelun”，通过 dm.getDataByTag('yelun') 即可获取到这个节点，然后给这个节点设置旋转动画。
 HT 中调度进行的流程是，先通过 DataModel（[https://hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html](https://hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html)） 添加调度任务，DataModel 会在调度任务指定的时间间隔到达时， 遍历 DataModel 所有图元回调调度任务的 action 函数，可在该函数中对传入的 Data 图元做相应的属性修改以达到动画效果。
根据上面对调度任务的说明，我们了解到向 dm 数据容器中添加调度任务会遍历整个数据容器，数据容器中内容不多的时候可能感觉不到，但当数据容器中内容多且模型重的情况下，对 dm 数据容器进行过滤就非常有必要了，而且如果添加多个调度任务都遍历了整个数据容器，那么对电脑的性能要求可想而知。我一开始使用的时候就是遗漏了对 dm 数据容器的过滤，因为场景不大，所以一开始没有感觉，后来加了灯光后很重，就立马出现问题了，但是一直找不到原因，后来在高人指点下才发下遗漏了对 data 的过滤判断。
因此，调度任务传入的参数对象中 action 方法传入了一个 data 值，用于设置当前动画的对象，不是此对象的直接可以 return 掉，不做任何操作：
```
var task = [];
var yelun = dm.getDataByTag('yelun');// 获取 tag 为 yelun 的节点
// 创建一个动画调度任务
task.yelunTask = {
    interval: 100,// 动画持续时间
    action: function(data) {// 动画内容
        if (data !== yelun) return;
        // 设置 yelun 节点的 x 轴旋转为当前 x 轴旋转值再加上 Math.PI/12
        yelun.setRotationX(yelun.getRotationX() + Math.PI/12);
    }
}
dm.addScheduleTask(task.yelunTask);// 将调度任务添加到数据容器中
```
## 容器水位的上升下降
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180608142837257-1858828457.gif)
这里将容器水位的上升下降放到一个动画调度任务里了，也就是说通过 dm 数据容器操作这个调度任务就能够同时操作这两个部分的动画，将上一小节中的 yelunTask 调度任务的 action 更改一下，因为上面的代码只对 yelun 节点进行了操作，我们需要对装水的容器也进行操作。首先获取装水的容器，这里将这个节点的唯一标识 tag 设置为“cylinder”：
```
var cylinder = dm.getDataByTag('cylinder');
```
然后更改调度中的 action 部分代码：
```
action: function(data) {
    if (!(data === yelun || data === cylinder)) return;
    // 叶轮转动
    yelun.setRotationX(yelun.getRotationX() + Math.PI/12);
    // 容器水位变化
    if (cylinder.getTall() === 100) {
        cylinder.setTall(0);// 容器水位高度到达 100 的值时，重置为 0 
    }
    else cylinder.setTall(cylinder.getTall() + 1);
}
```
## 叶轮转动故障展示
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180608143033431-1421688732.png)
因为没有数据的传输，所以这边故障信息我只能自己造假数据了，我创建了一个 10 以内的整数随机数，判断这个值是否为 1，如果为 1 就将运作正常的图标变换成告警图标，同时我还通过这个值来设置 dm 数据容器添加/移除调度任务来控制当前叶轮转动/停止、容器水位变化与否：
```
var alarm = dm.getDataByTag('alarm');// 获取告警图标节点
setInterval(function() {
    var random = Math.floor(Math.random()*5);
    if (random === 1) {
        alarm.s('shape3d.image', 'symbols/电信/故障 2.json');// 设置告警图标节点为“故障”图标
        dm.removeScheduleTask(task.yelunTask);// 将叶轮的动画加上
    }
    else {
        alarm.s('shape3d.image', 'symbols/电信/正常 2.json');// 设置告警图标节点为“正常”图标
        dm.addScheduleTask(task.yelunTask);// 移除叶轮的动画
    }
}, 1000);
```
## 开启/关闭动画
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180607192839877-852431412.png)
上一小节我们已经提到了开启/关闭动画的方式，这边我们运用 form 表单，手动操作动画的开启和关闭（注：这里只说明第一行的“水流开关”）。
首先，我们需要创建一个 formPane 表单组件（[https://hightopo.com/guide/guide/plugin/form/ht-form-guide.html](https://hightopo.com/guide/guide/plugin/form/ht-form-guide.html)），在这个表单组件中添加行数据，这边操作动画的开启和关闭我是用的 checkbox，值变化只有 true 和 false，这个情况用这个是比较优的选择。然后通过监听这个 checkbox 的值的变化事件，设置动画的开启（添加）或者关闭（移除）。
```
function createForm(task) {
    var form = new ht.widget.FormPane();// 创建 form 表单组件对象
    form.setWidth(160);// 设置表单组件的宽度
    form.setHeight(90);// 设置表单组件的高度
    // 设置表单组件底层 div 的样式属性
    form.getView().style.right = '10px';
    form.getView().style.top = '10px';
    form.getView().style.background = 'rgba(255, 255, 255, 0.2)';
    form.getView().style.borderRadius = '5px';
    document.body.appendChild(form.getView());// 将 form 表单底层 div 添加到 body 体中
    // 水阀开启和关闭
    form.addRow([// 给 form 表单添加一行数据
        {
            checkBox: {// 复选框类，HT 将此封装到 form 中 实际上创建了一个 ht.widget.CheckBox 组件
                label: '水流开关',// 设置 checkbox 的文本内容
                labelColor: '#fff',// 设置 checkbox 文本颜色
                selected: true,// 设置此 checkbox 是否选中
                onValueChanged: function() {// 监听值变化事件
                    if (this.isSelected()) dm.addScheduleTask(task.arrowTask);// 如果这个 checkBox 选中，则添加动画(开启水阀)
                    else dm.removeScheduleTask(task.arrowTask);// 如果这个 checkBox 未被选中，则移除动画(关闭水阀)
                }
            }
        }
    ], [0.1]);// 设置这个行数据中列的宽度
    return form;
}
```
addRow 方法上面代码中一言两语解释不清楚，参考如下说明：
addRow(items, widths, height, params) 添加一行组件
- items为元素数组，元素可为字符串、json 格式描述的组件参数信息、html 元素或者为 null 的空
- widths 为每个元素宽度信息数组，宽度值大于 1 代表固定绝对值，小于等于 1 代表相对值，也可为 80+0.3 的组合
- height 为行高信息，值大于 1 代表固定绝对值，小于等于 1 代表相对值，也可为 80+0.3 的组合，为空时采用默认行高
- params 为 json 格式的额外参数，例如插入行索引以及行边框或背景颜色等，如{index: 2, background: 'yellow', borderColor: 'red'}
```
```
上面代码中提到的 arrowTask 是对场景中的“箭头”流动添加的动画调度任务，通过控制 form 表单中 checkbox 复选框是否选中可直接操作 dm 是否添加/移除动画调度任务。
## 灯光的开启/关闭
控制灯光的开启和关闭，这里也是通过 form 表单上的 checkbox 复选框来进行操作的。一般建议不要使用灯光，渲染太烧性能了，这里只是为了效果而添加做一个说明。
首先我们需要创建一个“灯”节点，然后通过设置样式属性 setStyle 来设置灯的类型、颜色、灯照范围等等属性：
```
// 添加灯光
var light = new ht.Light();// 创建一个灯节点（继承于 ht.Node） （[https://hightopo.com/guide/guide/core/lighting/ht-lighting-guide.html](https://hightopo.com/guide/guide/core/lighting/ht-lighting-guide.html)）
light.p3([15, 120, 50]);// 设置此节点的位置
light.setTag('light');// 设置此节点的唯一标识
dm.add(light);// 将此节点添加到 dm 数据容器中进行显示
light.s({// 设置此节点的样式属性 setStyle  简写为 s
    'light.type': 'point',// 设置灯类型
    'light.color': 'rgb(252,252,149)',// 设置灯颜色
    'light.range': 1400,// 设置灯照范围
    '3d.visible': false// 设置此节点在 3d 上不可见
});
```
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180608143259325-404302042.png)
然后在 form 表单上添加一行用来控制灯的开关、灯的颜色灯功能：
```
// 9、灯光开启和关闭 以及颜色切换
form.addRow([// form 中添加一行
    {
        id: 'lightDisabled',// 设置此项的 id 值，可通过 form.getItemById 获取此项
        checkBox: {// 复选框组件
            label: '开关灯',// 设置复选框文本内容
            labelColor: '#fff',// 设置复选框文本颜色
            selected: true,// 设置复选框是否选中
            onValueChanged: function() {// 监听值变化事件
                dm.getDataByTag('light').s('light.disabled', !this.getValue());// 获取灯节点并设置是否关闭灯光效果，light.disabled 属性默认为false，可设置为true关闭灯效果
            }
        }
    },
    {
        colorPicker: {// 颜色选择器组件
            value: 'rgb(252,252,149)',// 设置当前值
            instant: true,// 设置是否处于即时状态，将会实时改变模型值
            onValueChanged: function() {// 监听值变化事件
                dm.getDataByTag('light').s('light.color', this.getValue())// 设置灯的颜色为当前选中的颜色
            }
        }
    }
], [0.1, 0.1]);
```
## 点击切换模型
 HT 将事件监听封装到 mi 事件（[https://hightopo.com/guide/guide/core/3d/ht-3d-guide.html#ref_interactionlistener](https://hightopo.com/guide/guide/core/3d/ht-3d-guide.html#ref_interactionlistener)）中，mi 方法中有多种事件，这里我们需要的是单击节点的事件监听 clickData 事件，通过判断事件类型 e.kind 是否为 clickData，之后对节点的设置模型即可：
```
var waterPump6 = dm.getDataByTag('水泵06');// 获取 tag 为“水泵06”的节点
waterPump6.s({// 设置该节点的样式属性
    'note': '点我切换模型',// 设置标注文字内容
    'note.transparent': true,// 设置标注在 3D 下是否透明
    'note.t3': [0, 0, -50],// 设置标注在 3D 下的偏移
    'note.reverse.flip': true//设置标注背面是否显示正面的内容
});
g3d.mi(function(e) {// 监听 3D 组件上的事件
    if(e.kind === 'clickData') {// 点击节点事件
        // 模型点击切换
        if (e.data === waterPump6 && e.data.s('shape3d') === 'models/裙房系统/水泵.json') e.data.s('shape3d', 'models/fengji.json');// 设置点击节点的 shape3d 样式属性
        else if (e.data === waterPump6 && e.data.s('shape3d') === 'models/fengji.json') e.data.s('shape3d', 'models/裙房系统/水泵.json');// 设置点击节点的 shape3d 样式属性
    }
});
```
 HT 设置模型是通过设置节点的样式属性 node.setStyle（简写为 node.s）为 shape3d 来实现的。
## 点击隐藏/显示属性窗口
上面说到了事件的监听，既然同为点击事件，我们就在一个监听事件里面进行具体的操作即可，在上面的 if (e.kind === 'clickData') 判断中添加显示/隐藏属性窗口的逻辑：
```
var waterPump5 = dm.getDataByTag('水泵05');
waterPump6.s({
    'note': '点我切换模型',
    'note.transparent': true,
    'note.t3': [0, 0, -50],
    'note.reverse.flip': true
});
g3d.mi(function(e) {
    if(e.kind === 'clickData') {
        // 模型点击切换
        if (e.data === waterPump6 && e.data.s('shape3d') === 'models/裙房系统/水泵.json') e.data.s('shape3d', 'models/fengji.json');
        else if (e.data === waterPump6 && e.data.s('shape3d') === 'models/fengji.json') e.data.s('shape3d', 'models/裙房系统/水泵.json');
        
        // 模型点击 隐藏/显示属性窗口
        if (e.data === waterPump5) {// 判断点击的图元是否为 waterPump5
            if(giveWater.s('3d.visible')) {// 判断当前属性窗口是否为显示状态
                giveWater.s('3d.visible', false);// 设置属性窗口不可见
                e.data.s('note', '点我显示属性窗口');// 更改标注中的显示内容
            }
            else {
                giveWater.s('3d.visible', true);// 设置属性窗口可见
                e.data.s('note', '点我隐藏属性窗口')// 更改标注中的显示内容
            }
        }
    }
});
```
## 文本内容/颜色变换
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180608111628899-1362875133.png)
通过 tag 获取场景中对应的属性窗口的节点，此节点为一个面板，相当于六面体有六面，这个节点类型就只有一面，并通过设置属性 shape3d.image 设置此节点上的图片为 tooltips.json 矢量图标（[https://hightopo.com/guide/guide/core/vector/ht-vector-guide.html](https://hightopo.com/guide/guide/core/vector/ht-vector-guide.html)）。矢量在 Hightopo（HT）中是矢量图形的简称，常见的 png 和 jpg 这类的栅格位图， 通过存储每个像素的颜色信息来描述图形，这种方式的图片在拉伸放大或缩小时会出现图形模糊，线条变粗出现锯齿等问题。 而矢量图片通过点、线和多边形来描述图形，因此在无限放大和缩小图片的情况下依然能保持一致的精确度。而且 HT 的矢量图形还有一个非常重要的特点，就是能够对矢量图形上的任何一个部分都进行数据绑定，也就是说上图中的五张图，我们可以只绘制一张图，通过数据绑定来改变这张图上的文本以及数值内容。
矢量图标中的数据绑定可以用在工业中的生产看板、大屏中的数据显示等等，都能够以一种高效的方式进行产品的整合。
矢量图形的数据绑定能够再写一篇文章进行阐述了，这里就不多提，大家自行去官网上查看“矢量手册”以及“数据绑定手册”，说明的比较详细。
获取到对应的节点之后，通过 node.a 方法可以获取和设置数据绑定（[https://hightopo.com/guide/guide/core/databinding/ht-databinding-guide.html#ref_vector](https://hightopo.com/guide/guide/core/databinding/ht-databinding-guide.html#ref_vector)）的属性，这里我们绑定的是文本内容“label”和数值“value”以及数值颜色“valueColor”:
```
var billboardArray = [];
// 通过 tag 获取节点
var temperature1 = dm.getDataByTag('回水温度1');// 获取 tag 为"回水温度1"的节点
billboardArray.push(temperature1);
var temperature2 = dm.getDataByTag('回水温度2');
billboardArray.push(temperature2);
var returnPress = dm.getDataByTag('回水压力');
billboardArray.push(returnPress);
var givePress = dm.getDataByTag('供水压力');
billboardArray.push(givePress);
var giveTemp = dm.getDataByTag('供水温度');
billboardArray.push(giveTemp);
var giveWater = dm.getDataByTag('供水流量');
billboardArray.push(giveWater);
// 文字标签内容变换
billboardArray.forEach(function(billboard) {
    billboard.a('label', billboard.getTag());// 设置数据绑定属性为 label 的属性值为当前节点的 tag 内容
});
// 文字标签数字变换+颜色变换 更改图标中绑定的 value 属性值
setInterval(function() {
    billboardArray.forEach(function(billboard) {
        var random = Math.random()*100;
        billboard.a('value', random.toFixed(2));
        // 设置图标中“数值内容颜色”
        if (random > 70 && random <= 80) billboard.a('valueColor', '#00FFFF');
        else if (random > 80 && random <= 90) billboard.a('valueColor', '#FFA000');
        else if (random > 90) billboard.a('valueColor', '#FF0000');
        else billboard.a('valueColor', '');
    });
}, 1000);
```
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180608142203255-2086902366.gif)
