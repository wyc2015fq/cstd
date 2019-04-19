# 基于 HTML5 的 WebGL 楼宇自控 3D 可视化监控 - xhload3d - 博客园
**前言**
智慧楼宇和人们的生活息息相关，楼宇智能化程度的提高，会极大程度的改善人们的生活品质，在当前工业互联网大背景下受到很大关注。目前智慧楼宇可视化监控的主要优点包括:
- 智慧化 -- 智慧楼宇是一个生态系统，像人一样拥有感知能力、自我判断能力以及控制能力。
- 绿色化 -- 绿色建筑在耗能、产能以及能源管理方面实现绿色化，楼宇安防实现绿色化监控。
- 运行成本可控制 -- 基于可持续发展的要求，现代建筑、商业建筑需运行50年以上，建筑在运行过程中能源消耗巨大，如何降低运营成本降低，使建筑在低碳、环保的状态下健康运行。
传统的 智慧楼宇/楼宇自动化/楼宇安防/智慧园区 常会采用 BIM（建筑信息模型 Building information modeling）软件，如 Autodesk 的 Revit 或 Bentley 这类建筑和工程软件，但这些 BIM 建模模型的数据往往过于庞大臃肿，绝大部分细节信息对楼宇自控意义不大，反而影响拖累了行业 **Web SCADA** 或 **Web 组态**监控的趋势，所以我们采用以 **Hightopo** 的 **HT for Web** 产品轻量化 HTML5/WebGL 建模的方案，实现快速建模、运行时轻量化到甚至手机终端浏览器即可 3D 可视化运维的良好效果。
本篇文章通过对智能建筑的建模，页面动画效果的实现，以及页面主要功能点进行阐述，帮助我们了解如何使用 [HT](https://hightopo.com/) 实现一个简单的智慧楼宇可视化监控，以及帮助我们了解智慧楼宇，楼宇自动化的优势。
预览地址：[基于 HTML5 的 WebGL 楼宇自控 3D 可视化监控](http://www.hightopo.com/demo/ht-smart-building/)[http://www.hightopo.com/demo/ht-smart-building/](http://www.hightopo.com/demo/ht-smart-building/)
**界面简介及效果预览**
界面通过 2d 图纸叠加在 3d 场景上来实现 2d 界面 与 3d 场景的融合，2d 界面通过自动布局的机制实现了手机端与电脑端的响应式呈现。
**界面初始化效果**
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143151913-1819448404.gif)
界面初始化过程中的动画包括地面路径的实时渲染，楼层的展开，楼层的辉光扫描，楼层报警点动态水波，楼层监测数据面板的实时变化等等。
**监控界面效果**
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143210537-1462172807.gif)
**监控界面包括：**
- 人员进入大厦的实时监控，面板中动态刷新人员进入大厦的头像以及当前大厦人数等实时信息。
- 大厦电梯运行情况实时监控，系统中展示电梯当前的运行位置以及是否在运行等信息。
- 大厦某个具体楼层监控数据的实时监控，通过柱状图的形式展示了当前楼层具体信息的大小。
- 大厦管道的实时监控，展示了当前智能建筑所有管道的运行情况。
**智能建筑建模**
该 3d 场景中所有的模型均为线段和六面体搭建，相比较通过 3d Max 建模然后通过 obj 导入来说场景中的三角面会少很多，更加的轻量化，例如场景中建筑的楼层，通过 **ht.Shape** 类绘制，该类中记录着楼层 **points** 点的信息以及 **segments** 为 **ht.List** 类型的线段数组信息，**segments** 代表着点的连接方式，用于告诉 **ht.Shape** 利用点的信息来绘制二次贝塞尔曲线或者三次贝塞尔曲线或者直线等信息，相关具体说明请参考 HT for Web 的[[形状手册]](http://www.hightopo.com/guide/guide/core/shape/ht-shape-guide.html)，以下为绘制单层的截图：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143743362-380396194.jpg)
通过上图可以知道构建完一层模型之后其它几层模型均为相同的，只是 y 轴的数值不同，通过叠加几层之后便可形成一幢大楼的轮廓。如果用户需要搭建智慧园区，智慧楼宇等场景，完全可以使用这种基于 HTML5/WebGL 建模的方案，减少考虑使用 BIM 建模模型。场景中的管道以及背景地图路线都为点连线之后构成，只是通过修改线的颜色粗细或者进行贴图来修改线或者面的样式，场景中的电梯为一个颜色为黄色的简单六面体，电梯线也为一条线段而已，所以场景中的模型都是轻量化的建模，使 3d 场景运行渲染的更加流畅，提升用户体验。
**场景关键动画代码分析**
**1. 地图路线动画代码分析**
通过上述**智能建筑建模**的分析我们可以知道线路都是为点与点之间进行连线而生成，所以当我们绘制完地图的路径之后可以得到所有点的信息，假如直线 **AB** 为地图中的某一条线段，那么我们可以知道点 **A** 以及点 **B** 的点的坐标，之后我们可以计算 **AB** 线段上任意一点 **C** 的点的坐标，然后通过连接 **A** 点与 **C** 点来形成一条与 **AB** 线段位置方向相同但是大小比 **AB** 线段短的线，直到 **AC** 线段的长度等于 **AB** 线段长度之后再进行下一条路径动画的绘制，以下为关键伪代码展示：
```
1  // currentIndex 为当前路径绘制到的点的索引 
 2  // points 为当前路径所有点的信息  currentPoints 为绘制过程中点的信息 
 3  // segments 为当前路径所有点的连接方式信息 currentSegments 为绘制过程中点的连接方式信息
 4  
 5  // 即上述此时 A 点信息
 6 let fromPoint = points[currentIndex]; 
 7  // 即上述此时 B 点信息
 8 let toPoint = points[currentIndex + 1]; 
 9  // 通过 AB 两点信息组成一条 AB 方向的向量
10 let pointVector = new ht.Math.Vector2(toPoint.x - fromPoint.x, toPoint.y - fromPoint.y); 
11  // 记录该向量的长度，用于判断 AC 是否大于等于 AB
12 let pointVectorLength = pointVector.length(); 
13 
14 let currentPoints = [], currentSegments = [];
15 
16 for(let i = 0; i < currentIndex + 1; i++) {
17     currentPoints.push({
18       x: points[i].x,
19       y: points[i].y
20     });
21     currentSegments.push(segments[i]);
22 }
```
通过上述代码可以知道我们获取到了 currentPoints 以及 currentSegments 的信息了，之后便要计算在 fromPoint(A点) 与 toPoint(B点) 连线上点的坐标，即 C 点，以下为计算 C 点的关键伪代码：
```
1  // addLength 为每次增加的线段长度值，该程序中使用 500 即每次长度增加 500
 2 let nextVectorLength = currentVectorLength + addLength, 
 3     tempPoint;
 4     
 5 roadData.currentVectorLength = nextVectorLength;
 6 
 7  // 判断 AC 线段的长度是否大于 AB 
 8 if(nextVectorLength > pointVectorLength) {
 9     nextVectorLength = pointVectorLength;
10     roadData.currentVectorLength = 0;
11     roadData.currentIndex = currentIndex + 1;
12 }
13 
14 pointVector.setLength(nextVectorLength);
15 
16  // 即为 C 点坐标
17 tempPoint = {x: pointVector.x + fromPoint.x, y: pointVector.y + fromPoint.y}; 
18  // 往 currentPoints 添加 C 点坐标
19 currentPoints.push(tempPoint); 
20  // 往 currentSegments 添加 C 点连接方式，此程序中都为直线连接，所以值都为 2
21 currentSegments.push(2); 
22  // roadNode 即为 ht.Shape 类 重新设置 ht.Shape 类点的信息
23 roadNode.setPoints(currentPoints);  
24  // 重新设置 ht.Shape 类点的连接信息
25 roadNode.setSegments(currentSegments);
```
以下为动画代码执行流程图
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143304528-941131602.jpg)
以下为绘制一条路线动画的截图：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143324316-51398477.gif)
程序中通过**向量**的计算方式来不断获取 **C** 点的坐标，当然也可以用其它方式来计算 **C** 点的坐标。
**2. 水波以及扫描动画代码分析**
水波以及扫描动画都是通过 **HT** 提供的修改图标矩形框信息 api 来进行控制，通过调度的方式不断修改图标矩形框大小来产生水波以及扫描的动画效果，**调度**的具体用法可以参考 HT for Web 的[[调度手册]](http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-guide.html)，以下为水波动画的关键伪代码：
```
1 // waterWaveNodes 所有水波节点
 2 let waterWaveTask = {
 3     interval: 100, // 指每隔 100 ms 调用 action 函数一次
 4     action: function(data){
 5         // 判断 waterWaveNodes 是包含 data
 6         if(waterWaveNodes.indexOf(data) > -1) { 
 7             // 获取此时图标矩形框信息 circleRect 是个长度为 4 的数组 分别表示 x, y, width, height
 8             let circleRect = data.a('circleRect'); 
 9    
10             if(circleRect) {
11                 // 通过修改高度来变大水波大小
12                 let nextHeight = circleRect[3] + 10; 
13                 
14                 // 高度最大值为 250 
15                 if(nextHeight < 250) { 
16                      // 对应修改 y 的大小，y 的增加大小为高度的一半
17                     circleRect[1] = circleRect[1] - 5;
18                     circleRect[3] = nextHeight;
19                     data.a('circleRect', circleRect);
20                     data.a('borderColor', 'rgba(255, 133, 133, ' + (1 - circleRect[3] / 250) + ')');
21                 }
22                 else {
23                     data.a('circleRect', [-0.5,128,257,0]);
24                     data.a('borderColor', 'rgba(255, 133, 133)');
25                 }
26                 
27             }
28             else {
29                 data.a('circleRect', [-0.5,128,257,0]);
30             }
31             
32         }
33     }    
34 };
35 dm3d.addScheduleTask(waterWaveTask); // 新增该调度任务
```
下图为水波在 2d 中的截图：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143334553-266741100.gif)
**3. 数字变化动画代码分析**
从程序的截图中可以看到在 2d 面板以及 3d 场景中都有数字在动态的变化，这部分主要通过数据绑定动态来修改数值的大小，关于数据绑定可以参考 HT for Web 的[[数据绑定手册]](http://www.hightopo.com/guide/guide/core/databinding/ht-databinding-guide.html)，也是通过调度来不断修改数值的大小，程序中我封装了产生随机数的代码，用于每次产生随机数之后绑定到对应的节点上，以下为修改 2d 面板上数字的变化伪代码：
```
1  // numNode(1-7) 为 2d 面板中对应数字的节点
 2  // data.a('ht.value', number) 即为动态修改 attr 上的 ht.value 信息，之后图纸会自动更新新赋予的数值
 3  // getRandomValue 为自己封装的产生随机数的方法
 4 this.change2dNumTask = {
 5         interval: 1000,
 6         action: (data) => {
 7             if(data === numNode1 || data === numNode2) {
 8                 data.a('ht.value', util.getRandomValue([500, 999], 5));
 9             }
10             if(data === numNode3 || data === numNode4) {
11                 data.s('text', util.getRandomValue([0, 30], 2) + '%');
12             }
13             if(data === numNode5) {
14                 data.a('ht.value', util.getRandomValue([0, 99999], 5, 3));
15             }
16             if(data === numNode6) {
17                 data.a('ht.value', util.getRandomValue([0, 100], 2));
18             }
19             if(data === numNode7) {
20                 data.a('ht.value', util.getRandomValue([0, 100], 2));
21             }
22        }        
23     };
24 dm2d.addScheduleTask(this.change2dNumTask); // 新增该调度任务
```
通过以上代码可以知道修改数值是通过修改节点的 attr 以及 style 对象的某个属性值来动态变化数值，当然在程序中 2d 面板可能还会隐藏，此时该调度任务就不需要执行，可以调用 removeScheduleTask 方法来移除此调度任务。
**4. 柱状图高度动画代码分析**
在 3d 场景中柱状体也是一个六面体，只是四周用了渐变的贴图，以及顶面用了一张纯色的贴图构造出来，每个六面体都有高度的信息，**HT** 中通过 **node.getTall()** 来获取当前六面体的高度值，根据上一节讲的数据绑定，我们可以在展示柱状图的时候循环获取所有柱状体节点的高度值大小假如命名为 **tall**，之后通过 **node.a('tall', tall)** 将该值存储到当前柱状图节点的 **attr** 对象上面，之后在柱状体初始化的时候可以不断修改高度值来动态改变高度，当高度值大于 **node.a('tall')** 则说明当前柱状体初始化的高度已经完成。以下为相关的伪代码：
```
1 charts.forEach((chart) => {
 2     !chart.a('tall') && chart.a('tall', chart.getTall()); // 将高度存储到 attr 上
 3     chart.setTall(0); // 设置初始高度为 0
 4 });
 5 this.chartAnimteTask = {
 6         interval: 25,
 7         action: function(data){
 8             if(charts.indexOf(data) > -1) {
 9                 if(finishNum !== chartLength) {
10                     if(data.getTall() !== data.a('tall')) {
11                         let nextTall = data.getTall() + deep; // deep 为每次增加的高度
12                         let tall = data.a('tall'); // 获取初始化高度 
13                         // 判断下一个高度是否大于初始化高度
14                         if(nextTall < tall) {
15                             data.setTall(nextTall);
16                         }
17                         else {
18                             data.setTall(tall);
19                             finishNum++;
20                         }
21                     }    
22                 }
23             }
24         }    
25 };
26 dm3d.addScheduleTask(this.chartAnimteTask); // 新增该调度任务
```
通过上面代码可以知道动画每一步的程序执行也是通过调度来完成的，与前文几个动画的实现方式类似。
**5. 3d 镜头推进代码分析**
3d 场景中视野的推进后退都是通过 HT api 提供的修改 **eye** 以及 **center** 的数值方法来实现，通过不断调用 setEye 以及 setCenter 方法来达到修改视角的目的，eye 类比人眼睛所处的位置，center 类比人眼睛聚焦的位置，以下为实现镜头推进关键的伪代码：
```
1 let e = ht.Default.clone(g3d.getEye()), // 获取当前眼睛的位置
 2     c = ht.Default.clone(g3d.getCenter()); // 获取当前眼睛聚焦的位置
 3  // eye 为需要修改的对应 eye 值
 4  // center 为需要修改的对应 center 值
 5  // 以下为分别获取 eye 与 center 在 xyz 三个坐标轴之间的差值
 6     let edx = eye[0] - e[0],
 7         edz = eye[1] - e[1],
 8         edy = eye[2] - e[2],
 9         cdx = center[0] - c[0],
10         cdz = center[1] - c[1],
11         cdy = center[2] - c[2];
12     // 开启不断修改 eye 与 center 的动画
13     ht.Default.startAnim({
14         duration: time ? time : 3000,
15         easing: function(t){ return t; },
16         finishFunc: function() {
17             if(typeof cb === 'function') {
18                 cb();
19             }
20         },
21         action: function (v) {
22             // v 为从 0-1 变换的值 
23             g3d.setEye([
24                 e[0] + edx * v,
25                 e[1] + edz * v,
26                 e[2] + edy * v
27             ]);
28             g3d.setCenter([
29                 c[0] + cdx * v,
30                 c[1] + cdz * v,
31                 c[2] + cdy * v
32             ]);
33         }
34 });
```
通过以上代码可以知道通过修改 eye 与 center 分别对应的 xyz 轴的值与当前 e 与 c 分别对应的 xyz 轴的值之间的距离来达到视角的变化。
以下为该代码的一个应用截图:
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143351484-102391694.gif)
**总结**
物联网通过各种信息传感设备，实时采集任何需要监控、连接、互动的物体或过程等各种需要的信息，与互联网结合形成的一个巨大网络。实现了物与物、物与人，所有的物品与网络的连接，方便识别、管理和控制。所以物联网带给我们的智慧楼宇的可视化监控需要监控的方面可能还有很多，该系统中针对人员出入，设备信息，管道信息等的监控实现了一个简单的智慧楼宇监控系统，物联网也将用户端延伸和扩展到了任何物品与物品之间，让我们更加了解搭建智慧园区，智慧校园等场景监控之后设备可视化，资产可视化带给我们的直观性。场景中的反光与景深等效果都是 HT 核心包提供的效果，所有的模型搭建与动画也都是通过 HT 核心包提供的 api 进行建模与动画驱动，所以在网页中展示会十分流畅，大大提高了用户的体验，并且在移动端表现也十分友好。
以下为移动端的程序运行截图：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143403750-863048551.gif)
程序运行截图：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143433288-2003134596.jpg)
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190201143440332-1364571048.jpg)
