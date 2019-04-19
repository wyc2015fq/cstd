# 基于 HTML5 WebGL 的 3D 机房 - xhload3d - 博客园
## 前言
用 WebGL 渲染的 3D 机房现在也不是什么新鲜事儿了，这篇文章的主要目的是说明一下，3D 机房中的 eye 和 center 的问题，刚好在项目中用上了，好生思考了一番，最终觉得这个例子最符合我的要求，就拿来作为记录。
## 效果图
![](https://images2018.cnblogs.com/blog/1159588/201803/1159588-20180301120748536-971837269.gif)
[http://hightopo.com/demo/3DRoom/index.html](http://hightopo.com/demo/3DRoom/index.html)
这个 3D 机房的 Demo 做的还不错，比较美观，基础的交互也都满足，接下来看看怎么实现。
## 代码生成
### 定义类
首先从 index.html 中调用的 js 路径顺序一个一个打开对应的 js，server.js 中自定义了一个 Editor.Server 类由 HT 封装的 ht.Default.def 函数创建的（注意，创建的类名 Editor.Server 前面的 Editor 不能用 E 来替代）：
```
ht.Default.def('Editor.Server', Object, {//第一个参数为类名，如果为字符串，自动注册到HT的classMap中；第二个参数为此类要继承的父类；第三个参数为方法和变量的声明
    addToDataModel: function(dm) { //将节点添加进数据容器
        dm.add(this._node);// ht 中的预定义函数，将节点通过 add 方法添加进数据容器中
    },
    setHost: function() { //设置吸附
        this._node.setHost.apply(this._node, arguments); 
    },
    s3: function() {//设置节点的大小
        this._node.s3.apply(this._node, arguments);
    },
    setElevation: function() {//控制Node图元中心位置所在3D坐标系的y轴位置
        this._node.setElevation.apply(this._node, arguments);
    }
});
```
### 创建 Editor.Server 类
![](https://images2018.cnblogs.com/blog/1159588/201803/1159588-20180301173908668-1527729418.png)
这个类可以创建一个 ht.Node 节点，并设置节点的颜色和前面贴图：
```
var S = E.Server = function(obj) {//服务器组件
    var color = obj.color, 
        frontImg = obj.frontImg;
    var node = this._node = new ht.Node();//创建节点
    node.s({//设置节点的样式 s 为 setStyle 的缩写
        'all.color': color,//设置节点六面的颜色
        'front.image': frontImg //设置节点正面的图片
    });
};
```
这样我在需要创建服务器组件的位置直接 new 一个新的服务器组件对象即可，并且能够直接调用我们上面声明的 setHost 等函数，很快我们就会用上。
接下来创建 Editor.Cabinet 机柜类 ，方法跟上面 Editor.Server 类的定义方法差不多：
```
ht.Default.def('Editor.Cabinet', Object, {
    addToDataModel: function(dm) {
        dm.add(this._door);
        dm.add(this._node);
        this._serverList.forEach(function(s) { 
            s.addToDataModel(dm); 
        });
    },
    p3: function() { 
        this._node.p3.apply(this._node, arguments);//设置节点的 3d 坐标
    }
});
```
### 创建 Editor.Cabinet 类
![](https://images2018.cnblogs.com/blog/1159588/201803/1159588-20180301173841255-1971073346.png)
这个类相对于前面的 Editor.Server 服务器组件类要相对复杂一点，这个类中创建了一个柜身、柜门以及机柜内部的服务器组件：
```
var C = E.Cabinet = function(obj) {
    var color = obj.color,
        doorFrontImg = obj.doorFrontImg,
        doorBackImg = obj.doorBackImg,
        s3 = obj.s3;
    var node = this._node = new ht.Node(); // 柜身
    node.s3(s3);//设置节点的大小 为 setSize3d
    node.a('cabinet', this);//自定义 cabinet 属性
    node.s({//设置节点的样式 为 setStyle
        'all.color': color,//设置节点六面的颜色
        'front.visible': false//设置节点前面是否可见
    });
    if (Math.random() > 0.5) {
        node.addStyleIcon('alarm', {//向节点上添加 icon 图标
            names: ['icon 温度计'],//包含多个字符串的数组，每个字符串对应一张图片或矢量(通过ht.Default.setImage注册)
            face: 'top',//默认值为front，图标在3D下的朝向，可取值left|right|top|bottom|front|back|center
            position: 17,//指定icons的位置
            autorotate: 'y',//默认值为false，图标在3D下是否自动朝向眼睛的方向
            t3: [0, 16, 0],//默认值为undefined，图标在3D下的偏移，格式为[x,y,z]
            width: 37,//指定每个icon的宽度，默认根据注册图片时的宽度
            height: 32,//指定每个icon的高度，默认根据注册图片时的高度
            textureScale: 4,//默认值为2，该值代表内存实际生成贴图的倍数，不宜设置过大否则影响性能
            visible: { func: function() { return !!E.alarmVisible; }}//表示该组图片是否显示
        });
    }
    var door = this._door = new ht.DoorWindow();//柜门
    door.setWidth(s3[0]);//置图元在3D拓扑中的x轴方向的长度
    door.setHeight(1);//设置图元在3D拓扑中的z轴长度
    door.setTall(s3[1]);//控制Node图元在y轴的长度
    door.setElevation(0);//设置图元中心在3D坐标系中的y坐标
    door.setY(s3[2] * 0.5);//设置节点在 y 轴的位置
    door.setHost(node);//设置吸附
    door.s({//设置节点样式 setStyle
        'all.color': color,//设置节点六面颜色
        'front.image': doorFrontImg,//设置节点正面图片
        'front.transparent': true,//设置节点正面是否透明
        'back.image': doorBackImg,//设置节点背面的图片
        'back.uv': [1,0, 1,1, 0,1, 0,0],//自定义节点后面uv贴图，为空采用默认值[0,0, 0,1, 1,1, 1,0]
        'dw.axis': 'right'//设置DoorWindow图元展开和关闭操作的旋转轴，可取值left|right|top|bottom|v|h
    });
    var serverList = this._serverList = []; 
    var max = 6,
        list = E.randomList(max, Math.floor(Math.random() * (max - 2)) + 2); //global.js 中声明的获取随机数的函数 
    var server, h = s3[0] / 4;
    list.forEach(function(r) {
        var server = new E.Server({ //服务器组件
            color: 'rgb(51,49,49)',
            frontImg: '服务器 组件精细'
        });
        server.s3(s3[0] - 2, h, s3[2] - 4);//设置节点大小
        server.setElevation((r - max * 0.5) * (h + 2));//设置节点中心点在 y 轴的坐标
        server.setHost(node);//设置节点的吸附
        serverList.push(server);//向 serverList 中添加 server 节点
    });
};
```
上面代码中唯一没提到的是 Editor.randomList 函数，这个函数是在 global.js 文件中声明的，声明如下：
```
var E = window.Editor = {
    leftWidth: 0,
    topHeight: 40,
    randomList: function(max, size) {
        var list = [], ran;
        while (list.length < size) {
            ran = Math.floor(Math.random() * max);
            if (list.indexOf(ran) >= 0) 
                continue;
            list.push(ran);
        }
        return list;
    }
};
```
好了，场景中的各个部分的类都创建完成，那我们就该将场景创建起来，然后将这些图元都堆进去！
### 场景创建
如果熟悉的同学应该知道，用 HT 创建一个 3D 场景只需要 new 一个 3D 组件，再将通过 addToDOM 函数将这个场景添加进 body 中即可：
```
var g3d = E.main = new ht.graph3d.Graph3dView(); //3d 场景
```
main.js 文件中主要做的是在 3D 场景中一些必要的元素，比如墙面，地板，门，空调以及所有的机柜的生成和排放位置，还有非常重要的交互部分。
墙体，地板，门，空调和机柜的创建我就不贴代码出来了，有兴趣的请自行查看代码，这里主要说一下双击机柜以及与机柜有关的任何物体（柜门，服务器设备）则 3D 中 camera 的视线就会移动到双击的机柜的前方某个位置，而且这个移动是非常顺滑的，之前技艺不精，导致这个部分想了很久，最后参考了这个 Demo 的实现方法。
为了能够重复地设置 eye 和 center，将设置这两个参数对应的内容封装为 setEye 和 setCenter 方法，setCenter 方法与 setEye 方法类似，这里不重复赘述：
```
// 设置眼睛位置
var setEye = function(eye, finish) {
    if (!eye) return;
    var e = g3d.getEye().slice(0),//获取当前 eye 的值
        dx = eye[0] - e[0],
        dy = eye[1] - e[1],
        dz = eye[2] - e[2];
    // 启动 500 毫秒的动画过度
    ht.Default.startAnim({
        duration: 500,
        easing: easing,//动画缓动函数
        finishFunc: finish || function() {}, //动画结束后调用的函数
        action: function(v, t) {//设置动画v代表通过easing(t)函数运算后的值，t代表当前动画进行的进度[0~1]，一般属性变化根据v参数进行
            g3d.setEye([ //设置 3D 场景中的 eye 眼睛的值，为一个数组，分别对应 x，y，z 轴的值 
                e[0] + dx * v,
                e[1] + dy * v,
                e[2] + dz * v
            ]);
        }
    });
};
```
我没有重复声明 setCenter 函数不代表这个函数不重要，恰恰相反，这个函数在“视线”移动的过程中起到了决定性的作用，上面的 setEye 函数相当于我想走到我的目标位置的前面（至少我定义的时候是这种用途），而 sCenter 的定义则是将我的视线移到了目标的位置（比如我可以站在我现在的位置看我右后方的物体，也可以走到我右后方去，站在那个物体前面看它），这点非常重要，请大家好好品味一下。
双击事件倒是简单，只要监听 HT 封装好的事件，判断事件类型，并作出相应的动作即可：
```
g3d.mi(function(e) {//addInteractorListener 事件监听函数
    if (e.kind !== 'doubleClickData')  //判断事件类型为双击节点
        return;
    var data = e.data, p3;
    if (data.a('cabinet')) //机身
        p3 = data.p3();
    else {
        host = data.getHost(); //获取点击节点的吸附对象
        if (host && host.a('cabinet')) {//如果吸附对象为 cabinet
            p3 = host.p3();
        }
    }
    if (!p3) return;
    setCenter(p3); //设置 center 目标的要移向位置为 cabinet 的位置
    setEye([p3[0], 211, p3[2] + 247]); //设置 eye 眼睛要移向的位置
});
```
### 顶部导航栏
![](https://images2018.cnblogs.com/blog/1159588/201803/1159588-20180301173750565-2026049627.png)
一开始看到这个例子的时候我在想，这人好厉害，我用 HT 这么久，用 HT 的 ht.widget.Toolbar 还没能做出这么漂亮的效果，看着看着发现这原来是用 form 表单做的，厉害厉害，我真是太愚钝了。
```
var form = E.top = new ht.widget.FormPane(); //顶部 表单组件
form.setRowHeight(E.topHeight);//设置行高
form.setVGap(-E.topHeight);//设置表单组件水平间距 设置为行高的负值则可以使多行处于同一行
form.setVPadding(0);//设置表单顶部和顶部与组件内容的间距
form.addRow([null, {//向表单中添加一行组件，第一个参数为元素数组，元素可为字符串、json格式描述的组件参数信息、html元素或者为null
    image: {
        icon: './symbols/inputBG.json',
        stretch: 'centerUniform'
    }
}], [40, 260]);//第二个参数为每个元素宽度信息数组，宽度值大于1代表固定绝对值，小于等于1代表相对值，也可为80+0.3的组合
form.addRow([null, null, {
    id: 'searchInput',
    textField: {}
}, {
    element: '机房可视化管理系统',
    color: 'white',
    font: '18px arial, sans-serif'
}, null, {
    button: {
        // label: '视图切换',
        icon: './symbols/viewChange.json',
        background: null,
        selectBackground: 'rgb(128,128,128)',
        borderColor: 'rgba(0, 0, 0, 0)',
        onClicked: function() {
            E.focusTo();
        }
    }
}, null, {
    button: {
        // label: '告警',
        icon: './symbols/alarm.json',
        togglable: true,
        selected: false,
        background: null,
        selectBackground: 'rgb(128,128,128)',
        borderColor: 'rgba(0, 0, 0, 0)',
        onClicked: function(e) {
            E.setAlarmVisible(this.isSelected());
        }
    }
}, null], [40, 42, 218, 300, 0.1, 50, 10, 50, 10]);
```
以上都只是能实现，但是并没有真正地添加进 html 标签中，也就意味着，现在界面上什么都没有！别忘了在页面加载的时候将 3D 场景添加进 body 中，同时也别忘了将 form 表单添加进 body 中，并且设置窗口大小变化事件时，form 表单也需要实时更新：
```
window.addEventListener('load', function() {
    g3d.addToDOM(); //将 3D 场景添加进 body 中
    document.body.appendChild(E.top.getView()); //将 form 表单组件底层 div 添加进 body 中
    window.addEventListener('resize', function() {//窗口大小变化事件监听
        E.top.iv();//更新 form 表单的底层 div 
    });
});
```
这里说明一下 addToDOM 函数，对于了解 HT 的机制非常重要。HT 的组件一般都会嵌入 BorderPane、SplitView 和 TabView 等容器中使用，而最外层的 HT 组件则需要用户手工将 getView() 返回的底层 div 元素添加到页面的 DOM 元素中，这里需要注意的是，当父容器大小变化时，如果父容器是 BorderPane 和 SplitView 等这些 HT 预定义的容器组件，则 HT 的容器会自动递归调用孩子组件invalidate 函数通知更新。但如果父容器是原生的 html 元素， 则 HT 组件无法获知需要更新，因此最外层的 HT 组件一般需要监听 window 的窗口大小变化事件，调用最外层组件 invalidate 函数进行更新。
为了最外层组件加载填充满窗口的方便性，HT 的所有组件都有 addToDOM 函数，其实现逻辑如下，其中 iv 是 invalidate 的简写：
```
addToDOM = function(){   
    var self = this,
        view = self.getView(),   
        style = view.style;
    document.body.appendChild(view); //将场景的底层 div 添加进 body 中           
    style.left = '0';//HT 默认将所有的组件底层div的position设置为absolute
    style.right = '0';
    style.top = '0';
    style.bottom = '0';      
    window.addEventListener('resize', function () { self.iv(); }, false); //窗口大小变化监听事件，通知组件变化更新          
}
```
这样，所有的代码就结束了，可以自己右键“检查”，network 中可以获取相对应的 json 文件。
