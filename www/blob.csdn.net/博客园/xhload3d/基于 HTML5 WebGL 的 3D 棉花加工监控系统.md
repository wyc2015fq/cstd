# 基于 HTML5 WebGL 的 3D 棉花加工监控系统 - xhload3d - 博客园
# 前言
现在的棉花加工行业还停留在传统的反应式维护模式当中，当棉花加下厂的设备突然出现故障时，控制程序需要更换。这种情况下，首先需要客户向设备生产厂家请求派出技术人员进行维护，然后生产厂家才能根据情况再派人到现场进行处理。由于棉花加工设备分布在中国各地乃至出口到世界各地，从客户反应问题到厂家派人到达现场的时间周期就会很长，少则 一天，个别偏远的地方可能会需要几天，不同程度地影响到企业生产活动的继续进行。传统的反应式维护存在以下缺点：售后服务响应速度慢；维护成本高；生产效率低下；停车率高；管理成本高；无法应对合格工程师不足的情况。
远程监控系统主要是通过分布于棉花加工生产线各种设备的传感器、开关信号、视频监控设备、 PLC 控制器等装置，通过智能联网设备集成到互联网和局域网上面，实现对生产、运营情况的随时掌握，建立网络范围内的监控数据和网上知识资源库，根据现场采集的设备运行数据进行远程诊断和在线维修。
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180603215847545-1772668791.gif)
[http://www.hightopo.com/demo/Plucker/](http://www.hightopo.com/demo/Plucker/)
# 代码实现
##  创建场景
首先是创建一个三维场景，通过将场景中的元素添加到保存数据的数据容器中即可：
```
var dm = new ht.DataModel();// 数据容器
var g3d = new ht.graph3d.Graph3dView(dm);// 三维组件
g3d.addToDOM();// 将三维组件添加到 body 体中
```
上面代码中出现的 addToDOM 方法是将调用此方法的组件通过 getView 方法获取到此组件的底层 div，随后将此 div 添加到 body 体中。HT 的组件一般都会嵌入 BorderPane、SplitView 和 TabView 等容器中使用，而最外层的HT组件则需要用户手工将 getView() 返回的底层 div 元素添加到页面的 DOM 元素中，这里需要注意的是，当父容器大小变化时，如果父容器是 BorderPane 和 SplitView 等这些HT预定义的容器组件，则HT的容器会自动递归调用孩子组件 invalidate 函数通知更新。但如果父容器是原生的 html 元素， 则 HT 组件无法获知需要更新，因此最外层的 HT 组件一般需要监听 window 的窗口大小变化事件，调用最外层组件 invalidate 函数进行更新。
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
整个大环境搭建好了后，我们需要向场景中添加 3D 模型，并进行位置的摆放，这里采用的是将整个场景的模型以及模型的摆放放在一个 JSON 格式的文件中，然后通过将这个 JSON 文件反序列化到数据容器 DataModel 中，即可呈现 JSON 文件中的场景内容以及模型的摆放位置：
```
ht.Default.xhrLoad('scenes/抓棉机.json', function(text) {// 加载 JSON 文件
    dm.deserialize(text);// 将 JSON 反序列化到数据容器中
});
```
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180603215955045-1130195553.png)
上面出现的 ht.Default.xhrLoad 方法是一个封装好的异步加载文件的函数，可以通过这种方法来加载 JSON 文件，因为此方法为异步加载，所以如果要操作此函数反序列化后的数据容器中的元素，需要在此函数中进行后续的操作。
## 场景动画
因为整个场景中的元素都是从此 JSON 文件中反序列化出来的，此 JSON 文件中保存的只有场景的内容，并不包括动画以及交互，对于不同部分的元素的动画也不同，我们需要单独将这些元素取出来，这里通过 dm.getDataByTag(tag) 方法实现，此方法通过 tag 唯一标识来获取节点的信息：
```
var equipment = dm.getDataByTag('equipment');// 获取轧棉机的节点
var hand = dm.getDataByTag('hand');// 获取轧棉机“手”节点
var light = dm.getDataByTag('light');// 获取轧棉机顶部的指示灯的节点
```
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180603220103119-1560929910.png)
获取到这些需要做动画的节点后，这里我用的是动画插件 setAnimation 方法来做的，动画插件更进一步对动画进行封装，用户只需用描述性的说明 HT 即可自动实现动画过程，动画插件可以将图元的一个或多个属性值 (如 width、height、opacity 等)从一个值平滑的缓动至另一个值，同时提供了丰富的缓动方式用于实现各种效果。但是使用这个插件前得先引入 ht-animation.js 文件：
```
<script src="ht.js"></script> <!--先引入ht.js-->
<script src="ht-animation.js"></script>
```
这里总共有三个部分有动画，采用的方法大致相同，这里仅对整个轧棉机的机身的左右移动的动画进行说明。
```
equipment.setAnimation({// 动画调用方法
    moveDown: {// 定义了一个名为 moveDown 的动画过程，这个动画过程改变图元的 x 轴坐标，将其从 623 变化至 -256
        from: 623,// 动画开始时的属性值
        to: -256,// 动画结束时的属性值
        interval: equipInterval,// 动画间隔，单位ms
        next: ["moveUp"],// 字符串类型，指定当前动画完成之后，要执行的下个动画，可将多个动画融合
        onUpdate: function(value) {// 回调函数，动画的每一帧都会回调此函数
            this.setX(value);// 设置该节点的 x 轴的值为当前动画 from 到 to 的值
            formPane.getItemById('xValue').element = value.toFixed(2);// 获取 form 表单上的 xValue 元素，同时改变此值
            formPane.iv();// 表单内容变化后要通知表单进行刷新变化
        }
    },
    moveUp: {// 定义了一个名为 moveUp 的动画过程，这个动画过程改变图元的 x 轴坐标，将其从 -256 变化至 623
        from: -256,
        to: 623,
        interval: equipInterval,
        next: ["moveDown"],
        onUpdate: function(value) {
            this.setX(value);
            formPane.getItemById('xValue').element = value.toFixed(2);
            formPane.iv();
        }
    },
    start: ['moveDown']// start是一个数组，用于指定要启动的一个或多个动画
});
```
注意，要使用动画，首先您需要调用 ht.DataModel#enableAnimation(interval) 启动全局动画定时器，默认 interval 为 16ms，如果不设置此参数值，则 DataModel 定时器每隔 16ms 左右就会遍历自己所有的 Data，根据 Data 的 animation 配置执行动画。
```
dm.enableAnimation();
```
## 表单创建
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180603220226272-2044096758.png)
前面代码中出现的 form 表单，是通过 createForm 方法创建的，此方法定义如下（PS：由于 form 表单的列表稍长，这里就选取几个比较有代表性的表单元素进行说明）：
```
// 创建 form 表单
function createForm() {
    var fp = new ht.widget.FormPane();// 创建表单组件实例
    fp.setWidth(200);// 设置表单组件的宽度
    fp.setHeight(250);// 设置表单组件的高度
    fp.getView().style.top = '8%';// 设置表单组件的样式
    fp.getView().style.right = 0;
    fp.getView().style.background = 'rgba(255, 255, 255, 0.3)';
    document.body.appendChild(fp.getView());// 将表单组件的底层 div 添加到 body 体中
    var equipment = dm.getDataByTag('equipment');// 通过 tag 唯一标识获取元素
    var hand = dm.getDataByTag('hand');
    fp.setLabelAlign('right');// 设置表单的文本对齐方式
    
    fp.addRow([// 向表单中添加一行  此方法的参数一为元素数组，可在一行中添加多个元素
        {// 元素一  显示文本内容为 “机器号”
            element: '机器号',
            color: '#fff'
        },
        {// 元素二 显示文本内容为 “Machine”
            element: 'Machine',
            color: '#fff'
        }
    ], [0.1, 0.1]);// 参数二为每个元素宽度信息数组，宽度值大于1代表固定绝对值，小于等于1代表相对值，也可为80+0.3的组合
    fp.addRow([// 向表单中添加一行  此方法的参数为一个数组，可在一行中添加多个元素
        {// 元素一  显示文本内容为 “机器号”
            element: '抓棉机动画',
            color: '#fff'
        },
        {// 元素二  显示文本内容为 一个按钮元素
            button: {
                label: '开始',
                onClicked: function() {// 按钮点击触发事件，启动轧棉机左右移动动画
                    dm.enableAnimation();// 启动全局动画定时器
                    var light = dm.getDataByTag('light');
                    if (!light) return;
                    lightColor(light);
                    colorTimer = setInterval(function() {
                        lightColor(light);
                    }, 1000);
                }
            }
        },
        {
            button: {
                label: '停止',
                onClicked: function() {// 按钮点击触发事件，关闭轧棉机左右移动动画
                    dm.disableAnimation();// 停止全局动画定时器
                    clearInterval(colorTimer);
                    var light = dm.getDataByTag('light');
                    if (light) light.s('all.color', 'rgba(0,0,255,0.51)');
                }
            }
        }
    ], [0.2, 0.1, 0.1]);// 将一行中的三个元素都分配宽度进行显示
    fp.addRow([// 向表单中添加一行  此方法的参数为一个数组，可在一行中添加多个元素
        {// 元素一 显示文本内容为“小车行走速度”
            element: '小车行走速度',
            color: '#fff'
        },
        {// 元素二  显示文本内容为一个 滑动条
            id: 'slide',
            slider: {// 滑动条
                value: equipInterval,// slider 当前值
                min: 0,// slider 最小值
                max: 300,// slider 最大值
                step: 20,// slider 每移动一步的值
                onValueChanged: function() {// slider 值变化触发事件
                    var anim = dm.getDataAnimation(equipment);// 获得参数data的动画配置
                    anim.moveDown.interval = this.getValue();// 设置 moveDown 动画的 interval 内容
                    anim.moveUp.interval = this.getValue();// 设置 moveUp 动画的 interval 内容
                }
            }
        }
    ], [0.1, 0.1]);
    fp.addRow([// 向表单中添加一行  此方法的参数为一个数组，可在一行中添加多个元素
        {
            element: 'X 轴',
            color: '#fff'
        },
        {
            id: 'xValue',
            element: equipment.getX().toFixed(2),// 获取轧棉机当前 x 轴值
            color: '#fff'
        }
    ], [0.1, 0.1]);
    return fp;
}
```
有趣的部分就说这么多，如果有什么建议和意见，欢迎留言或者私信~
# 总结
以前对 animation 动画用的比较少，这次也是特地用它仔细研究一下 animation 的机制，就 animation 设置的动画能够以一种“平和”的方式进行值的变化，动画也看起来比较有条理一些；当然还有能够设置下一次动画需要做什么的操作，这个设计也非常的人性化；同时还能通过 getDataAnimation 获取对象的动画配置，并通过此方法对对象的动画进行重新配置，这些优点都是值得拿出来跟大家分享的。
![](https://images2018.cnblogs.com/blog/591709/201806/591709-20180603220830061-1561691694.gif)
