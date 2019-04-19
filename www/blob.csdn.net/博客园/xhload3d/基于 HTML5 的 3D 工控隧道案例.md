# 基于 HTML5 的 3D 工控隧道案例 - xhload3d - 博客园
隧道的项目我目前是第一次接触，感觉做起来的效果还蛮赞的，所以给大家分享一下。这个隧道项目的主要内容包括：照明、风机、车道指示灯、交通信号灯、情报板、消防、火灾报警、车行横洞、风向仪、COVI、微波车检以及隧道紧急逃生出口的控制。这个例子我主要讲一下风向仪、排风以及逃生出口的动画设置， 还有就是点击交通信号灯，弹出窗口可以选择当前信号灯的信息等内容。
本例效果：
![](https://images2018.cnblogs.com/blog/591709/201803/591709-20180322121431104-46773855.png)
我是用 [HT](http://hightopo.com) 做的整个例子，首先创建 3D 场景，HT 有 3D 组件，可以直接通过 new ht.graph3d.Graph3dView 3D 组件来创建一个实例，然后通过 getView() 函数获取组件的底层 div，既然是 div，那位置显示控制就容易得多了：
```
dm = new ht.DataModel();//数据容器，可以将显示在界面上的所有数据通过 dataModel.add 存储在数据容器中
g3d = new ht.graph3d.Graph3dView(dm);//3D 组件
g3d.addToDOM();//将 3D 组件的底层 div 添加到 body 中
```
HT 的组件一般都会嵌入 BorderPane、SplitView 和 TabView 等容器中使用，而最外层的 HT 组件则需要用户手工将 getView() 返回的底层 div元素添加到页面的 DOM 元素中，这里需要注意的是，当父容器大小变化时，如果父容器是 BorderPane 和 SplitView 等这些`HT`预定义的容器组件，则`HT`的容器会自动递归调用孩子组件 invalidate 函数通知更新。但如果父容器是原生的 html 元素， 则`HT`组件无法获知需要更新，因此最外层的 HT 组件一般需要监听 window 的窗口大小变化事件，调用最外层组件 invalidate 函数进行更新。
为了最外层组件加载填充满窗口的方便性，HT 的所有组件都有 addToDOM 函数，其实现逻辑如下，其中 iv 是 invalidate 的简写：
```
addToDOM = function(){   
    var self = this,
        view = self.getView(),  //获取组件的底层 div
        style = view.style;
    document.body.appendChild(view); //将组件的底层 div 添加进 body 中           
    style.left = '0';//HT 组件默认设置 position 样式属性为 absolute 绝对定位方式
    style.right = '0';
    style.top = '0';
    style.bottom = '0';      
    window.addEventListener('resize', function () { self.iv(); }, false);            
}
```
最让我开心的应该是我的开发基本上跟设计部分完全分离了，因为 HT 可以通过 ht.Default.xhrLoad 函数直接加载 json 文件的场景，这样我跟设计师就是双进程了，非常开心呢~加载场景有三个步骤，如下：
```
ht.Default.xhrLoad('scenes/隧道.json', function(text){//加载 json 场景
    var json = ht.Default.parse(text);//转义 json 文件
    dm.deserialize(json);//将 json 内容反序列化到场景中
    //可以在这个里面任意操作 datamodel 数据容器中的数据了
}
```
我在场景中添加了一些功能，包括前面提到过的一些动画操作，HT 封装好的 dataModel.addScheduleTask(task) 通过操作数据容器 dataModel 来控制加载动画，动画部分在参数 task 中声明，task 为 json 对象，可指定如下属性：
- interval：间隔毫秒数，默认值为 10
```
```
- enabled：是否启用开关，默认为 true
```
```
- action：间隔动作函数，该函数必须设置
 我的动画一共三个，两个隧道中各有一个风扇、一个风向仪以及一个卷闸门。设置这三个图元变化即可，我在 json 中分别将这三个图元的 tag 设置为 feng、feng2 以及 door，在代码中我就可以直接调用这三个图元的 tag 属性：
```
var task = {
    action: function(data){
        if(!data.getTag()) return;
        var tag = data.getTag();//获取图元的 tag 属性
    if(tag === 'feng'){
        data.r3(0, (data.r3()[1]+Math.PI/12), 0);//r3 为 3d 中的旋转，这里 y 轴在原来的基础上再旋转 Math.PI/12 角度
    }else if(tag === 'feng2'){
        data.r3(0, 0, data.r3()[2]+Math.PI/12);
    }else if(tag === 'door'){
            if(data.getTall() > 0){//获取图元的 tall 属性，高度
                data.setTall(data.getTall()-20);//设置高度为当前高度减去20
            }
        }
    }
}
dm.addScheduleTask(task);//在数据容器 dataModel 中添加调度任务
```
接着是创建 form 表单，在表单上添加一些信息，比如交通灯的切换等等，场景默认显示的右上角的 form 表单我们这里不做解释，内容跟点击交通灯出现的 form 表单差不多，所以我们主要说明一下点击交通灯时出现的表单：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180125094841428-1155789360.png)
表单中重复的部分比较多，我挑出三个部分来解释一下：文本部分、“当前状态”显示的图标以及下面“修改状态”中的图标点击选择部分：
```
form.addRow([//addRow 添加一行 我这个部分是添加一个标题
    {
        element: '交通灯控制',//这一行第一部分的显示文本
        align: 'center',//文本对齐方式
        color: 'rgb(0,210,187)',//文本颜色
        font: 'bold 16px arial, sans-serif'//文本字体
    }
], [0.1]);//记得要设置这行的宽度
form.addRow([ //这行中有两个部分，一个“设备描述”，一个 文本“0”，所以要设置两个宽度，宽度要放在一个数组中      
    '设备描述:',//第一部分
    {//第二部分
        element: '0',
        color: 'rgb(0,210,187)'
    }
],[80, 0.1], 34);//addRow 函数第二个参数为宽度设置，将上面内容的宽度依次放进这个数组中。第三个参数为高度
form.addRow([     
    '当前状态:',
    {//也可以将数组中的某个部分设置为空字符串，占据一些宽度，这样比例比较好调
        element: ''
    },
    {
        id: '105',//id唯一标示属性，可通过formPane.getItemById(id)获取添加到对应的item对象
        button: {//按钮，设置了该属性后HT将根据属性值自动构建ht.widget.Button对象，并保存在element属性上
            icon: 'symbols/隧道用图标/light.json',//按钮上的显示图标
            background: 'rgba(0,7,26,0.60)',//按钮背景
            borderColor: 'rgb(0, 7, 26)',//按钮边框颜色
            clickable: false//是否可点击
        }
    }
],[80, 0.1, 84], 30);
form.addRow([//如果和上面一行的距离差别与其它行间距不同，可以通过增加一行空行，设置高度即可                    
    '',
    {
        element: ''
    }
], [200, 0.1], 10);
form.addRow([                    
    '修改状态:',
    {
        element: ''
    },
    {
        button: {
            icon: 'symbols/隧道用图标/light.json',//设置按钮的图标
            background: 'rgba(0,7,26,0.60)',
            borderColor: 'rgb(0, 7, 26)',
            groupId: 'btn',//通过getGroupId和setGroupId获取和设置组编号，属于同组的togglable按钮具有互斥功能。后面的三个按钮也是设置了同一个 groupId
            onClicked: function(e){//点击后的回调函数
                btnClick('light'); 
            }
        }
    }
],[80, 0.1, 84], 30);
```
这个 form 表单的背景只是设置了一张图片而已：
```
background: url('assets/控制.png') no-repeat;
```
上面还有一个部分没有提及，就是点击按钮后调用的 btnClick 函数：
```
function btnClick(imageName){
    if(flag === 1){//做的判断是根据3d的事件来处理的，等下会提
        dm.getDataByTag('light').s({//通过getDataByTag获取节点，设置节点的style样式
            'back.image': 'symbols/隧道用图标/'+imageName+'.json',//设置图元的背面图片
            'front.image': 'symbols/隧道用图标/'+imageName+'.json'//设置图元你的前面图片
        });
    }else if(flag === 2){
        dm.getDataByTag('light1').s({
            'back.image': 'symbols/隧道用图标/'+imageName+'.json',
            'front.image': 'symbols/隧道用图标/'+imageName+'.json'
        });
    }else{}
    form.getViewById(105).setIcon('symbols/隧道用图标/'+imageName+'.json');//设置id为105的item内容显示的图标为form表单上点击的交通灯的按钮的图标
}
```
最后就是点击事件了，一个是点击 3D 中的交通灯后出现交通灯控制的 form 表单，还有一个就是点击 form 表单上的“修改状态”中的图标事件：
```
g3d.mi(function(e){//addInteractorListener 函数 监听场景中的事件
    form.getView().style.display = 'none';//所有的点击图元事件都先将form隐藏
    if(e.kind === 'clickData'){//点击图元
        if(e.data.getTag() === 'light'){//如果图元是正面的隧道的灯
        form.getView().style.display = 'block';//显示form表单
            form.iv();//刷新form表单，不然界面无法获知此时需要刷新form显示上面的内容，必须手动操作
        flag = 1;
        }else if(e.data.getTag() === 'light1'){//背面的隧道的灯，点击切换隧道中的灯的显示，另外一个隧道中的灯不可能一起改变，所以要区分开
        form.getView().style.display = 'block';
            form.iv();
        flag = 2;
    }else if(e.data.getTag() === 'wall'){//隧道墙体
        e.data.s({
            'shape3d.transparent': true,//打开设置透明的开关
        'shape3d.opacity': 0.2,//设置透明度
        '3d.selectable': false//点击后就不可选中隧道了
        });
    }
    }else if(e.kind === 'doubleClickBackground'){//点击空白处 设置隧道墙体不透明
        dm.getDatas().forEach(function(data){//遍历dataModel
        if(data.getTag() === 'wall'){
            data.s({
            'shape3d.transparent': false,//关闭设置透明的开关，这样可以不用控制透明度
            '3d.selectable': true//双击空白处可以使得隧道变得可选中
        });
        }
    });
    }
});
```
本文例子地址：[http://www.hightopo.com/demo/tunnel/index.html](http://www.hightopo.com/demo/tunnel/index.html)
