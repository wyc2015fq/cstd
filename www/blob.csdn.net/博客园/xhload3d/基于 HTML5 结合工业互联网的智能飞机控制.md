# 基于 HTML5 结合工业互联网的智能飞机控制 - xhload3d - 博客园
# 前言
从互联网+的概念一出来，就瞬间吸引了各行各业的能人志士，想要在这个领域分上一杯羹。现在传统工业生产行业运用互联网+的概念偏多，但是在大众创业万众创新的背景下，“互联网＋”涌出了层出不穷的“玩法”，智慧城市、隧道交通、智慧园区、工业生产，甚至是这次要说的智能飞机！异地协同制造的范围，目前多局限于主机制造厂之间，发动机和机载系统介入得很少。“互联网+飞机”可通过提高各类飞行器的有效监控能力、应急处置能力来大幅提高航行安全水平。“在提高这两大能力后，像飞机失联这类事件将不再发生。”当飞机飞离预定航线时，地面可以即时监控，甚至在飞机遭遇恶意操控时，地面也可以接管，而且“互联网+飞机”将对每架飞机的各项数据了如指掌，有效提高航行的安全。我认为，“互联网+飞机”将超出传统的“互联网+飞机制造”阶段，让互联网在飞机全寿命使用过程中发威，这可为传统制造业转型升级提供重大机遇。
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180805233224930-1425417331.gif)
[http://hightopo.com/guide/guide/plugin/obj/examples/example_path.html](http://hightopo.com/guide/guide/plugin/obj/examples/example_path.html)
# 代码部分
## 加载飞机模型
首先，最重要的是我们的飞机模型，前面有文章写到过，HT 内部封装了一个方法 ht.Default.loadObj（[https://hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html#ref_loadobj](https://hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html#ref_loadobj)）来加载 OBJ 文件：
```
ht.Default.loadObj('obj/plane.obj', 'obj/plane.mtl', {                    
    center: true,
    r3: [0, -Math.PI/2, 0], // make plane face right
    s3: [0.15, 0.15, 0.15], // make plane smaller
    finishFunc: function(modelMap, array, rawS3){
        if(modelMap){                            
            modelMap.propeller.r3 = {// propeller 螺旋桨
            func: function(data){
                return [data.a('angle'), 0, 0]; 
            }
        };                             
        // 设置模型的大小为原来的 1 1.2 1.2 倍（相当于 x 轴放大了 1 倍，y 轴放大了 1.2 倍，z 轴放大了 1.2 倍）
        modelMap.propeller.s3 = [1, 1.2, 1.2]; 
        modelMap.propeller.color = 'yellow';
    } 
});
```
要将 obj 解析后的模型信息绑定到图元，需先调用建模手册（[https://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html](https://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html)）中模型注册（[https://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html#ref_register](https://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html#ref_register)）章节介绍的 ht.Default.setShape3dModel(name, model) 函数进行注册，之后图元只需将 style 的 shape3d 属性设置为注册的名称。当然我们现在将这个方法封装了一下，采用更简便的方法来加载模型，但是还是需要加载的原理：
```
// models/plane.json
{
    "modelType": "obj",
    "obj": "obj/plane.obj",
    "mtl": "obj/plane.mtl"// 要是没有 mtl 文件，则设置为 ""
}
```
 之后通过设置节点的 style 的 shape3d 属性设置为这个 json：node.s('shape3d', 'models/plane.json')。
注意！不管使用哪种方法来加载模型，mtl 文件中如果有使用贴图，贴图的路径需要是相对于 obj 文件的路径。
前面代码中的 modelMap.propeller 是 OBJ 文件中定义好的 modelMap 对象中的 propeller 对象，可以试着打印 modelMap 看看输出结果。
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180805234554874-733033186.png)
## 加载机尾指示灯
这个方法里的 finishFunc(modelMap, array, rawS3) 用于加载后的回调处理，具体查阅 HT for Web OBJ 手册（[http://hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html#ref_loadobj](http://hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html#ref_loadobj)），我们还添加了一个在 OBJ 模型中没有的飞机尾部的“红色闪烁指示灯”，这里用到的是组合模型 array（所有材质组成的数组，里面有至少一个模型），我们在 array中加入一个新的球模型：
```
// 添加一个指示灯的圆形模型
array.push({
    shape3d: ht.Default.createSmoothSphereModel(),
    t3: [-40, 10, 0],
    s3: [6, 6, 6],
    color: {
        func: function(data){
            return data.a('light') ? 'red': 'black';
        }
    }
});
```
这里的 shape3d 是 HT 封装的一个属性名，通过 setShape3dModel(name, model) 函数注册的或者是通过 getShape3dModel(name) 函数返回的注册过的 3D 模型，如何注册 3D 模型可查阅 HT for Web 建模手册（[http://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html#ref_register](http://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html#ref_register)）。
color 属性名对应了一个对象，这边的定义是这样的，color 直接通过 data.getAttr('a') 获取 data.setAttr(‘a’, value) 中的值，这样做有两个好处，一是可以不污染 HT 的常用属性操作，所以 HT 专门定义了这个 attr 属性类型，是 HT 预留给用户存储业务数据的；二是这样也很方便数据绑定，我们可以通过在需要更改属性的地方调用 setAttr 方法，非常方便。
接着我们通过 ht.Default.setShape3dModel(name, model) 来将我们刚刚组合好的模型 array 注册成我们要的“plane”模型：
```
ht.Default.setShape3dModel('plane', array);
```
## 创建模型节点
注册好模型后肯定是要调用这个模型，我们可以通过 shape3d 属性来调用这个模型，并且在这个模型中自定义上面代码中出现过的 light 属性和 angle 属性：
```
plane = new ht.Node();
plane.s3(200, 200, 200);
plane.s3(rawS3);
plane.s({
    'shape3d': 'plane',
    'shape3d.scaleable': false,
    'wf.visible': true,// 线框是否可见
    'wf.color': 'white',
    'wf.short': true // 是否显示封闭的线框，true为不封闭的短线框
});
plane.a({
    'angle': 0,
    'light': false
});
```
## 动画
因为飞机还有螺旋桨、指示灯两个功能，我们还得对这两个模型做动画效果，可查阅 HT for Web 动画手册（[http://hightopo.com/guide/guide/plugin/animation/ht-animation-guide.html](http://hightopo.com/guide/guide/plugin/animation/ht-animation-guide.html)），通过用户在 form 表单上选择的结果来决定飞机飞行持续时间、看飞机的视角、飞机沿着“航线”飞行所要旋转的角度、机尾指示灯的“闪烁”功能等等，最后别忘了飞机停止飞行时，如果要让飞机继续飞行，就得回调这个动画，并且设置灯不再闪烁，别忘了要启动动画：
```
params = {
    delay: 1500,
    duration: 20000,
    easing: function(t){ 
        return (t *= 2) < 1 ? 0.5 * t * t : 0.5 * (1 - (--t) * (t - 2));
    },
    action: function(v, t){
        var length = g3d.getLineLength(polyline),
        offset = g3d.getLineOffset(polyline, length*v),
        point = offset.point,
        px = point.x,
        py = point.y,
        pz = point.z,
        tangent = offset.tangent,
        tx = tangent.x,
        ty = tangent.y,
        tz = tangent.z;
        plane.p3(px, py, pz);
        plane.lookAt([px + tx, py + ty, pz + tz], 'right');
                        
        var camera = formPane.v('Camera');
        if(camera === 'Look At'){
            g3d.setCenter(px, py, pz);
        }
        else if(camera === 'First Person'){                            
            g3d.setEye(px - tx * 400, py - ty * 400 + 30, pz - tz * 400);
            g3d.setCenter(px, py, pz);                            
        }
                        
        plane.a('angle', v*Math.PI*120);                        
        if(this.duration * t % 1000 > 500){
            plane.a('light', false);
        }else{
            plane.a('light', true);
        }                        
    },
    finishFunc: function(){
        animation = ht.Default.startAnim(params);
        plane.a('light', false);
    }                  
};                               
               
animation = ht.Default.startAnim(params);
```
 其实最让我们好奇的是描绘的路径跟飞机本身的飞行并没有关系，还有那么多左拐右拐的，要如何做才能做到呢？
## 绘制飞机轨道
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180805234109466-1862741070.png)
接下来我们来描绘路径，首先这个路径是由 ht.Polyline 作为基础来描绘的：
```
polyline = new ht.Polyline();   
polyline.setThickness(2);
polyline.s({
    'shape.border.pattern': [16, 16],
    'shape.border.color': 'red',
    'shape.border.gradient.color': 'yellow',
    'shape3d.resolution': 300,
    '3d.selectable': false
});
dataModel.add(polyline);
```
上面的代码只是向 datamodel 数据模型中添加了一个 polyline 管线而已，不会显示任何东西，要显示“航道”首先就要设置航道所在的点，我们先设置航道的初始点：
```
points = [{ x: 0, y: 0, e: 0 }];
segments = [1];
```
 这个 points 和 segments 是 HT for Web Shape 手册（[http://hightopo.com/guide/guide/core/shape/ht-shape-guide.html](http://hightopo.com/guide/guide/core/shape/ht-shape-guide.html)）中定义的，points 是 ht.List 类型数组的定点信息，顶点为 { x: 100, y: 200 } 格式的对象；segments 是 ht.List 类型的线段数组信息，代表 points 数组中的顶点按数组顺序的连接方式。
图中“航道”左侧的多个圆形轨道也是通过设置 points 和 segments 来设置的：
```
for(var k=0; k<count+1; k++){
    var angle = k * Math.PI * 2 * round / count;
    points.push({
        x: cx + radius * Math.cos(angle),
        y: cy + radius * Math.sin(angle),
        e: k * height / count
    }); 
    segments.push(2);
}
```
 接下来几个拐点也是这种方法来实现的，这里就不赘述了，如果你还没看手册的话，这里标明一点，segments 只能取值 1~5，1 代表一个新路径的起点；2 代表从上次最后点连接到该点；3 占用两个点信息，第一个点作为曲线控制点，第二个点作为曲线结束点；4 占用3个点信息，第一和第二个点作为曲线控制点，第三个点作为曲线结束点；5 不占用点信息，代表本次绘制路径结束，并闭合到路径的起始点：
```
points.push({ x: cx+radius, y: 0, e: height/2 });
points.push({ x: 0, y: 0, e: height/2 });
segments.push(3);
points.push({ x: radius, y: -radius, e: height/2*0.7 });
points.push({ x: radius*2, y: radius, e: height/2*0.3 });
points.push({ x: radius*3, y: 0, e: 0 });
segments.push(4);   
points.push({ x: 0, y: 0, e: 0 });
segments.push(2);
```
 我们已经把路径上的点都添加进“航道”中了，接下来要把点都设置到管道上去才会显示在界面上：
```
polyline.setPoints(points);
polyline.setSegments(segments);
```
## 飞机跑道
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180805234038800-2130604327.png)
“跑道”就比较简单了，只是一个 Node 节点然后设置基础效果而已，没什么特别的：
```
runway = new ht.Node();
runway.s3(-cx+radius*3, 1, 200);
runway.p3(cx+runway.getWidth()/2, -22, 0);
runway.s({
    'all.color': '#FAFAFA',
    'all.transparent': true,
    'all.reverse.cull': true,
    'all.opacity': 0.8,
    '3d.selectable': false
});
dataModel.add(runway);
```
 最后，在界面上添加一个 formPane 表单面板，定义好之后可以直接添加到 body 上，这样就不会跟 graph3dView 有显示的联系了。
## 表单面板
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180805233948980-1010115836.png)
formPane 可以用 formPane.addRow （[https://hightopo.com/guide/guide/plugin/form/ht-form-guide.html#ref_func](https://hightopo.com/guide/guide/plugin/form/ht-form-guide.html#ref_func)）方法动态添加行，这个方法中可以直接对动态变化的数据进行交互，例如本例中的是否有动画 Animation，我们利用 checkBox 来记录选中或者非选中的状态：
```
{
    checkBox: {
        label: 'Animation',
        selected: true,
        onValueChanged: function(){
            if(this.isSelected()){
                animation.resume();
            }else{
                animation.pause();
            }                               
        }
    }
}
```
 也可以通过设置“id”来记录动态改变的值，然后 formPane 就会通过调用 formPane.v(id) 来获取当前值。
# 最后
工业互联网（Industrial Internet）的概念最早由通用电气（GE）在 2012 年提出，即让互联网进入产业链的上游，从根本上革新产业。根据飞常准的数据显示，美国已有 78% 的航班提供机上互联服务。在航天航空领域，工业互联网会打破软件、硬件和人员之间的信息壁垒，依靠大数据的分析，让飞机建立自己的声音，表达给飞行员和维修人员飞行员，具体飞行状况如何或者哪里需要维修。工业互联网技术的深入应用，正在改变着民航飞机的使用效率和制造成本。
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180805233450799-675544218.png)
[https://hightopo.com/demo/large-screen/index.html](https://hightopo.com/demo/large-screen/index.html)
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180805233619087-1931121658.gif)
[https://hightopo.com/demo/cloud-monitor/demo4.html](https://hightopo.com/demo/cloud-monitor/demo4.html)
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180805233659051-815391558.png)
[https://hightopo.com/demo/HTBuilding/index.html](https://hightopo.com/demo/HTBuilding/index.html)
