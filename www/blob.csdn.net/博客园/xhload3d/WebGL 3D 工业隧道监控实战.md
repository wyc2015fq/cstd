# WebGL 3D 工业隧道监控实战 - xhload3d - 博客园
# 前言
监控隧道内的车道堵塞情况、隧道内的车祸现场，在隧道中显示当前车祸位置并在隧道口给与提示等等功能都是非常有必要的。这个隧道 Demo 的主要内容包括：照明、风机、车道指示灯、交通信号灯、情报板、消防、火灾报警、车行横洞、风向仪、微波车检、隧道紧急逃生出口的控制以及事故模拟等等。
# 效果图
![](https://images2018.cnblogs.com/blog/591709/201803/591709-20180322115824909-201529258.png)
[http://www.hightopo.com/demo/tunnel2/index.html](http://www.hightopo.com/demo/tunnel2/index.html)
上图中的各种设备都可以双击，此时 camera 的位置会从当前位置移动到双击的设备的正前方；隧道入口的展示牌会自动轮播，出现事故时会展示牌中的内容会由“限速80，请开车灯”变为“超车道两车追尾，请减速慢行”；两隧道中间的逃生通道上方的指示牌是可以点击的，点击切换为蓝绿色激活状态，两旁的逃生通道门也会打开，再单击指示牌变为灰色，门关闭；还有一个事故现场模拟，双击两旁变压器中其中一个，在隧道内会出现一个“事故现场图标”，单击此图标，出现弹出框显示事故等等等等。
# 代码实现
## 场景搭建
整个隧道都是基于 3D 场景上绘制的，先来看看怎么搭建 3D 场景：
```
dm = new ht.DataModel();//数据容器
g3d = new ht.graph3d.Graph3dView(dm);// 3d 场景
g3d.addToDOM();//将场景添加到 body 中
```
上面代码中的 addToDOM 函数，是一个将组件添加到 body 体中的函数的封装，定义如下：
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
## JSON 反序列化
整个场景是由名为 隧道1.json 的文件导出而成的，我只需要用代码将 json 文件中的内容转换为我需要的部分即可：
```
ht.Default.xhrLoad('./scenes/隧道1.json', function(text) {//xhrLoad 函数是一个异步加载文件的函数
    var json = ht.Default.parse(text);//将 json 文件中的文本转为我们需要的 json 格式的内容
    dm.deserialize(json);//反序列化数据容器，解析用于生成对应的Data对象并添加到数据容器 这里相当于把 json 文件中生成的 ht.Node 节点反序列化到数据容器中，这样数据容器中就有这个节点了
});
```
由于 xhrLoad 函数是一个异步加载函数，所以如果 dm 数据容器反序列化未完成就直接调用了其中的节点，那么会造成数据获取不到的结果，所以一般来说我是将一些逻辑代码写在这个函数内部，或者给逻辑代码设置 timeout 错开时间差。
首先，由于数据都是存储在 dm 数据容器中的（通过 dm.add(node) 添加的），所以我们要获取数据除了可以通过 id、tag 等独立的方式，还可以通过遍历数据容器来获取多个元素。由于这个场景比较复杂，模型的面也比较多，鉴于设备配置，我将能 [Batch](http://hightopo.com/guide/guide/core/batch/ht-batch-guide.html) 批量的元素都进行了批量：
```
dm.each(function(data) {
    if (data.s('front.image') === 'assets/sos电话.png'){//对“电话”进行批量
        data.s('batch', 'sosBatch');
    }
    else if (data.s('all.color') === 'rgba(222,222,222,0.18)') {//逃生通道批量（透明度也会影响性能）
        data.s('batch', 'emergencyBatch');
    }
    else if (data.s('shape3d') === 'models/隧道/摄像头.json' || data.s('shape3d') === 'models/隧道/横洞.json' || data.s('shape3d') === 'models/隧道/卷帘门.json') {
        if(!data.s('shape3d.blend'))//个别摄像头染色了 不做批量
            data.s('batch', 'basicBatch');//基础批量什么也不做
    }
    else if (data.s('shape3d') === 'models/大型变压器/变压器.json') {    
        data.s('batch', 'tileBatch');
        data.setToolTip('单击漫游，双击车祸地点出现图标');
    }
    else if (data.getDisplayName() === '地面') {
        data.s('3d.selectable', false);//设置隧道“地面”不可选中
    }
    else if (data.s('shape3d') === 'models/隧道/排风.json') {
        data.s('batch', 'fanBatch');//排风扇的模型比较复杂，所以做批量
    }
    else if (data.getDisplayName() === 'arrow') {//隧道两旁的箭头路标
        if (data.getTag() === 'arrowLeft') data.s('shape3d.image', 'displays/abc.png');
        else data.s('shape3d.image', 'displays/abc2.png');
        data.s({
            'shape3d': 'billboard',
            'shape3d.image.cache': true,//缓存，设置了 cache 的代价是需要设置 invalidateShape3dCachedImage
            'shape3d.transparent': true //设置这个值，图片上的锯齿就不会太明显了（若图片类型为 json，则设置 shape3d.dynamic.transparent)
        });
        g3d.invalidateShape3dCachedImage(data);
    }
    else if (data.getTag() === 'board' || data.getTag() === 'board1') {//隧道入口处的情报板
        data.a('textRect', [0, 2, 244, 46]); //业务属性，用来控制文本的位置[x,y,width,height]
        data.a('limitText', '限速80，请开车灯');//业务属性，设置文本内容
        var min = -245;
        var name = 'board' + data.getId();
        window[name] = setInterval(function() {
            circleFunc(data, window[name], min)//设置情报板中的文字向左滚动，并且当文字全部显示时重复闪烁三次
        }, 100);
    }
    //给逃生通道上方的指示板 动态设置颜色
    var infos = ['人行横洞1', '人行横洞2', '人行横洞3', '人行横洞4', '车行横洞1', '车行横洞2', '车行横洞3'];
    infos.forEach(function(info) {
        if(data.getDisplayName() === info) {
            data.a('emergencyColor', 'rgb(138, 138, 138)');
        }
    });
    infos = ['车道指示器', '车道指示器1', '车道指示器2', '车道指示器3'];
    infos.forEach(function(info) {
        if (data.getDisplayName() === info) {
            createBillboard(data, 'assets/车道信号-过.png', 'assets/车道信号-过.png', info)//考虑到性能问题 将六面体变换为 billboard 类型元素
        }
    });
});
```
上面有一处设置了 tooltip 文字提示信息，在 3d 中，要显示这个文字提示信息，就需要设置 g3d.enableToolTip() 函数，默认 3d 组件是关闭这个功能的。
## 逻辑代码
### 情报板滚动条
我就直接按照上面代码中提到的方法进行解释，首先是 circleFunc 情报板文字循环移动的函数，在这个函数中我们用到了业务属性 limitText 设置情报板中的文字属性以及 textRect 设置情报板中文字的移动位置属性：
```
function circleFunc(data, timer, min) {//设置情报板中的文字向左滚动，并且当文字全部显示时重复闪烁三次
    var text = data.a('limitText');//获取当前业务属性 limitText 的内容
    data.a('textRect', [data.a('textRect')[0]-5, 2, 244, 46]); //设置业务属性 textRect 文本框的坐标和大小
    if (parseInt(data.a('textRect')) <= parseInt(min)) {
        data.a('textRect', [255, 2, 244, 46]);
    }
    else if (data.a('textRect')[0] === 0) {
        clearInterval(timer);
        var index = 0;
        var testName = 'testTimer' + data.getId();//设置多个 timer 是因为能够进入这个函数中的不止一个 data，如果在同一时间多个 data 设置同一个 timer，那肯定只会对最后一个节点进行动画。后面还有很多这种陷阱，要注意
        window[testName] = setInterval(function() {
            index++;
            if(data.a('limitText') === '') {//如果情报板中文本内容为空
                setTimeout(function() {
                    data.a('limitText', text);//设置为传入的 text 值
                }, 100);
            }
            else {
                setTimeout(function() {
                    data.a('limitText', ''); //若情报板中的文本内容不为空，则设置为空
                }, 100);
            }
            if(index === 11) { //重复三次 
                clearInterval(window[testName]);
                data.a('limitText', text);
            }
        }, 100);
        setTimeout(function() {
            timer = setInterval(function() {
                circleFunc(data, timer, min) //回调函数
            }, 100);
        }, 1500);
    }
}
```
由于 WebGL 对浏览器的要求不低，为了能尽量多的适应各大浏览器，我们将所有的“道路指示器” ht.Node 类型的六面体全部换成 billboard 类型的节点，性能能提升不少。
![](https://images2018.cnblogs.com/blog/591709/201803/591709-20180322180150108-1553868583.png)
[http://www.hightopo.com](http://www.hightopo.com)
设置 billboard 的方法很简单，获取当前的六面体节点，然后给这些节点设置：
```
node.s({
    'shape3d': 'billboard',
    'shape3d.image': imageUrl,
    'shape3d.image.cache': true
});
g3d.invalidateShape3dCachedImage(node); //还记得用 shape3d.image.cache 的代价么？
```
当然，因为 billboard 不能双面显示不同的图片，只是一个“面”，所以我们还得在这个节点的位置创建另一个节点，在这个节点的“背面”显示图片，并且跟这个节点的配置一模一样，不过位置要稍稍偏移一点。
### Camera 缓慢偏移
其他动画部分比较简单，我就不在这里多说了，这里有一个双击节点能将视线从当前 camera 位置移动到双击节点正前方的位置的动画我提一下。我封装了两个函数 setEye 和 setCenter，分别用来设置 camera 的位置和目标位置的：
```
function setCenter(center, finish) {//设置“目标”位置
    var c = g3d.getCenter().slice(0), //获取当前“目标”位置，为一个数组，而 getCenter 数组会在视线移动的过程中不断变化，所以我们先拷贝一份
        dx = center[0] - c[0], //当前x轴位置和目标位置的差值
        dy = center[1] - c[1],
        dz = center[2] - c[2];
    // 启动 500 毫秒的动画过度
    ht.Default.startAnim({
        duration: 500,
        action: function(v, t) {
            g3d.setCenter([ //将“目标”位置缓慢从当前位置移动到设置的位置处
                c[0] + dx * v,
                c[1] + dy * v,
                c[2] + dz * v
            ]);
        }
    });
};
function setEye(eye, finish) {//设置“眼睛”位置
    var e = g3d.getEye().slice(0),//获取当前“眼睛”位置，为一个数组，而 getEye 数组会在视线移动的过程中不断变化，所以我们先拷贝一份
        dx = eye[0] - e[0],
        dy = eye[1] - e[1],
        dz = eye[2] - e[2];
    // 启动 500 毫秒的动画过度
    ht.Default.startAnim({
        duration: 500,
        action: function(v, t) {//将 Camera 位置缓慢地从当前位置移动到设置的位置
            g3d.setEye([
                e[0] + dx * v,
                e[1] + dy * v,
                e[2] + dz * v
            ]);
        }
    });
};
```
后期我们要设置的时候就直接调用这两个函数，并设置参数为我们目标的位置即可。比如我这个场景中的各个模型，由于不同视角对应的各个模型的旋转角度也不同，我只能找几个比较有代表性的 0°，90°，180°以及360° 这四种比较典型的角度了。所以绘制 3D 场景的时候，我也尽量设置节点的旋转角度为这四个中的一种（而且对于我们这个场景来说，基本上只在 y 轴上旋转了）：
```
var p3 = e.data.p3(), //获取事件对象的三维坐标
    s3 = e.data.s3(),//获取事件对象的三维尺寸
    r3 = e.data.r3();//获取事件对象的三维旋转值
setCenter(p3);//设置“目标”位置为当前事件对象的三维坐标值
if (r3[1] !== 0) {//如果节点的 y 轴旋转值 不为 0
    if (parseFloat(r3[1].toFixed(5)) === parseFloat(-3.14159)) { //浮点负数得做转换才能进行比值
        setEye([p3[0], p3[1]+s3[1], p3[2] * Math.abs(r3[1]*2.3/6)]);//设置camera 的目标位置
    }
    else if (parseFloat(r3[1].toFixed(4)) === parseFloat(-1.5708)) {
        setEye([p3[0] * Math.abs(r3[1]/1.8), p3[1]+s3[1], p3[2]]);
    }
    else {
        setEye([p3[0] *r3[1], p3[1]+s3[1], p3[2]]);
    }
}
else {
    setEye([p3[0], p3[1]+s3[1]*2, p3[2]+1000]);
}
```
### 事故模拟现场
最后来说说模拟的事故现场吧，这段还是比较接近实际项目的。操作流程如下：双击“变压器”-->隧道中间某个部分会出现一个“事故现场”图标-->单击图标，弹出对话框，显示当前事故信息-->点击确定，则事故现场之前的灯都显示为红色×，并且隧道入口的情报板上的文字显示为“超车道两车追尾，请减速慢行”-->再双击一次“变压器”，场景恢复事故之前的状态。
在 HT 中，可通过 Graph3dView#addInteractorListener（简写为 mi）来监听交互过程：
```
g3d.addInteractorListener(function(e) {
    if(e.kind === 'doubleClickData') {
        if (e.data.getTag() === 'jam') return;//有“事故”图标节点存在
        if (e.data.s('shape3d') === 'models/大型变压器/变压器.json') {//如果双击对象是变压器
            index++;
            var jam = dm.getDataByTag('jam');//通过唯一标识tag标签获取“事故”图标节点对象
            if(index === 1){
                var jam = dm.getDataByTag('jam');
                jam.s({
                    '3d.visible': true,//设置节点在 3d 上可见
                    'shape3d': 'billboard',//设置节点为 billboard 类型
                    'shape3d.image': 'assets/车祸.png', //设置 billboard 的显示图片
                    'shape3d.image.cache': true,//设置 billboard 图片是否缓存
                    'shape3d.autorotate': true,//是否始终面向镜头
                    'shape3d.fixSizeOnScreen': [30, 30],//默认保持图片原本大小，设置为数组模式则可以设置图片显示在界面上的大小
                });
                g3d.invalidateShape3dCachedImage(jam);//cache 的代价是节点需要设置这个函数
             }
             else {
                 jam.s({
                     '3d.visible': false//第二次双击变压器就将所有一切恢复“事故”之前的状态
                });
                dm.each(function(data) {
                    var p3 = data.p3();
                    if ((p3[2] < jam.p3()[2]) && data.getDisplayName() === '车道指示器1') {
                        data.s('shape3d.image', 'assets/车道信号-过.png');
                    }
                    if(data.getTag() === 'board1') {
                        data.a('limitText', '限速80，请开车灯');
                    }
                });
                index = 0;
            }
                        
        }
    }
});
```
 既然“事故”节点图标出现了，接着点击图标出现“事故信息弹出框”，监听事件同样是在 mi（addInteractorListener）中，但是这次监听的是单击事件，我们知道，监听双击事件时会触发一次单击事件，为了避免这种情况，我在单击事件里面做了延时：
```
else if (e.kind === 'clickData'){//点击图元
    timer = setTimeout(function() {
        clearTimeout(timer);
        if (e.data.getTag() === 'jam') {//如果是“事故”图标节点
            createDialog(e.data);//创建一个对话框
        }
    }, 200);
}
```
在上面的双击事件中我没有 clearTimeout，怕顺序问题给大家造成困扰，要记得加一下。
弹出框如下：
![](https://images2018.cnblogs.com/blog/591709/201803/591709-20180322224048072-262159732.png)
这个弹出框是由两个 ht.widget.FormPane 表单构成的，左边的表单只有一行，行高为 140，右边的表单是由 5 行构成的，点击确定，则“事故”图标节点之前的道路指示灯都换成红色×的图标：
```
function createForm4(node, dialog) {//弹出框右边的表单
    var form = new ht.widget.FormPane();//表单组件
    form.setWidth(200);//设置表单组件的宽
    form.setHeight(200);//设置表单组件的高
    var view = form.getView();//获取表单组件的底层 div 
    document.body.appendChild(view);//将表单组件添加到 body 中
    var infos = [
        '编辑框内容为：2辆',
        '编辑框内容为：客车-客车',
        '编辑框内容为：无起火',
        '编辑框内容为：超车道'
    ];
    infos.forEach(function(info) {
        form.addRow([ //向表单中添加行
            info
        ], [0.1]);//第二个参数为行宽度，小于1的值为相对值
    });
    
    form.addRow([
        {
            button: {//添加一行的“确认”按钮
                label: '确认',
                onClicked: function() {//按钮点击事件触发
                    dialog.hide();//隐藏对话框
                    dm.each(function(data) {
                        var p3 = data.p3();
                        if ((p3[2] < node.p3()[2]) && data.getDisplayName() === '车道指示器1') {//改变“车道指示器”的显示图片为红色×，这里我是根据“事故”图标节点的坐标来判断“车道显示器”是在前还是在后的
                            data.s('shape3d.image', 'assets/车道信号-禁止.png');
                        }
                        if(data.getTag() === 'board1') {//将隧道口的情报板上的文字替换
                            data.a('limitText', '超车道两车追尾，请减速慢行');
                        }
                    });
                }
            }
        }
    ], [0.1]);
    return form;
}
```
# 结束语
这个工业隧道的 Demo 是我通过几天不断地完善完善而成的，可能还是有不足的地方，但是总体来说我是挺满意的了，可能之后还会继续完善，也得靠大家不断地给我意见和建议，我只希望在自己努力的同时也可以帮助到别人。整个 Demo 中，我主要遇到了两个问题，一个是我在代码中提到过的设置 timer 的问题，多个节点如果同时用一个 timer，那就只有最后一个节点能够显示出 timer 的效果；另一个是 getEye 和 getCenter 的问题，这两个值都是在不断变化的，所以得先拷贝一份数据，再进行数据的变换。
