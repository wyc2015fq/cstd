# 基于 HTML5 WebGL 的地铁站 3D 可视化系统 - xhload3d - 博客园
**前言**
工业互联网，物联网，可视化等名词在我们现在信息化的大背景下已经是耳熟能详，日常生活的交通，出行，吃穿等可能都可以用信息化的方式来为我们表达，在传统的可视化监控领域，一般都是基于 Web SCADA 的前端技术来实现 2D 可视化监控，本系统采用 Hightopo 的 HT for Web 产品来构造轻量化的 3D 可视化场景，该 3D 场景从正面展示了一个地铁站的现实场景，包括地铁的实时运行情况，地铁上下行情况，视频监控，烟雾报警，电梯运行情况等等，帮助我们直观的了解当前的地铁站。
系统中为了帮助用户更直观友好的浏览当前地铁站，提供了三种交互模式：
- 第一人称模式 -- 操作就类似行人或车在行进的效果，可以通过键盘鼠标控制前进后退。
- 自动巡检模式 -- 该模式下用户不需要任何操作，场景自动前进后退来巡查当前地铁站的场景。
- 鼠标操作模式 -- 左键旋转场景，右键平移场景。
本篇文章通过对地铁站可视化场景的搭建，动画代码的实现，交互模式的原理解析，以及主要功能点的实现进行阐述，帮助我们了解如何使用 **[HT](https://hightopo.com/)** 实现一个简单的地铁站可视化。
预览地址：[基于 HTML5 WebGL 的地铁站 3D 可视化系统](http://www.hightopo.com/demo/ht-subway/)[http://www.hightopo.com/demo/ht-subway/](http://www.hightopo.com/demo/ht-subway/)
**界面简介及效果预览**
**地铁运行效果**
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217222130192-613344785.gif)
地铁从站外开到站内的效果为透明度逐渐增加，速度逐渐降低。
**漫游效果**
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217222206241-1220111661.gif)
上述为自动巡检的漫游效果，场景自动进行前进旋转。
**监控设备交互效果**
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217222239338-650547517.gif)
当我们点击场景中的监控设备时可以查看当前设备的运行情况，运行数据等信息。
**场景搭建**
该系统中的大部分模型都是通过 3dMax 建模生成的，该建模工具可以导出 obj 与 mtl 文件，在 HT 中可以通过解析 obj 与 mtl 文件来生成 3d 场景中的所有复杂模型，当然如果是某些简单的模型可以直接使用 HT 来绘制，这样会比 obj 模型更轻量化，所以大部分简单的模型都是采用 **HT for Web** 产品轻量化 **HTML5/WebGL** 建模的方案，具体的解析代码如下：
```
1 // 分别为 obj 文件地址，mtl 文件地址
 2 ht.Default.loadObj('obj/metro.obj', 'obj/metro.mtl', {
 3     center: true,
 4     // 模型是否居中，默认为 false，设置为 true 则会移动模型位置使其内容居中
 5     r3: [0, -Math.PI / 2, 0],
 6     // 旋转变化参数，格式为 [rx, ry, rz]
 7     s3: [0.15, 0.15, 0.15],
 8     // 大小变化参数，格式为 [sx, sy, sz]
 9     finishFunc: function(modelMap, array, rawS3) {
10         if (modelMap) {
11             ht.Default.setShape3dModel('metro', array); // 注册一个名字为 metro 的模型
12         }
13     }
14 });
```
上面通过加载 obj 模型之后注册了一个名字为 **metro** 的模型，之后如果要使用该模型可以通过以下代码来实现：
```
1 var node = new ht.Node();
2 node.s({
3     'shape3d': 'metro'
4 });
```
上面代码新建了一个 **node** 对象，通过设置 style 对象的 **shape3d** 属性可以把模型名称为 **metro** 用到该 node 对象上去，之后便是我们场景中看到的地铁列车模型。
**动画代码分析**
**地铁动画代码的实现分析**
场景中地铁的运行是通过 HT 提供的调度插件来实现，**调度**的具体用法可以参考 HT for Web 的[调度手册](http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-guide.html)，该调度主要用于在指定的时间间隔进行函数回调处理，回调函数的第一个参数为 data 图元，也就是 3D 场景中的模型节点，我们可以判断当前 data 是否为我们刚才创建的 metro 那个节点来进行后续的操作，场景中模拟了一个左开的地铁和一个右开的地铁，两辆地铁会交替出现。在 3D 场景中肯定会有坐标系，HT 中是用 x, y, z 来分别表示三个轴，所以地铁的运动肯定是改变地铁在坐标系中的位置来实现地铁的运行，地铁坐标如下图所示：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217222835256-173997617.jpg)
通过上图可以知道地铁在 3D 场景中的坐标系，如果要实现地铁的移动则只需要将地铁往图中所示红色箭头的方向进行移动，即 **x** 轴的方向，通过 **setX** 这个方法不断的修改地铁的位置达到地铁行进的目的，代码中通过 getSpeedByX 以及 getOpacityByX 两个方法来不断获取此时的列车速度以及列车透明度，以下为关键代码实现：
```
1 let metroTask = {
 2     interval: 50,
 3     // 每五十秒执行一次
 4     action: (data) = >{ // 即上文所提回调函数
 5         // 判断当时传进来的节点是否为地铁列车节点
 6         if (data === currentMetro) {
 7             // 获取地铁此时的 X 轴位置以及行进的方向
 8             let currentX = data.getX(),
 9             direction = data.a('direction');
10             // 根据当前的 X 轴位置获取当前的列车速度
11             let speed = this.getSpeedByX(currentX);
12             // 根据当前的 X 轴位置获取当前的列车透明度
13             let opacity = this.getOpacityByX(currentX);
14             // 判断此时 X 轴位置是否超过某个值 即地铁是在某个范围内移动
15             if (Math.abs(currentX) <= 5000) {
16                 // 设置当前的透明度
17                 opacity !== 1 ? currentMetro.s({
18                     'shape3d.transparent': true,
19                     'shape3d.opacity': opacity
20                 }) : currentMetro.s({
21                     'shape3d.transparent': false
22                 });
23                 // 设置当前的 X 轴位置
24                 data.setX(currentX + direction * speed);
25                 // 判断此时地铁的速度为 0，所以此时应该执行开门的动画
26                 if (speed === 0) this.doorAnimation(currentMetro, direction);
27             }
28             // 右方向地铁开到头，进行复位
29             if (currentX > 5000 && direction === 1) {
30                 currentMetro = leftMetro;
31                 currentMetro.setX(5000);
32             }
33             // 左方向地铁开到头，进行复位
34             if (currentX < -5000 && direction === -1) {
35                 currentMetro = rightMetro;
36                 currentMetro.setX( - 5000);
37             }
38         }
39     }
40 };
41 dm3d.addScheduleTask(metroTask);
```
通过以上代码可以知道地铁在运行的过程中，主要通过修改地铁的 **x** 轴位置来产生前进的动画，并且需要让地铁在某个区间内进行运动，需要判断边界，而且为了模拟出真实的效果需要根据地铁当前的位置不断获取当前的列车速度以及列车透明度，以下为流程图：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217223350025-341511583.png)
上图所示的为地铁进站时候的流程，当地铁停靠完毕关门后需要进行出站，此时我们只需要把地铁位置重新设置一下不为 0 即可，以下为部分代码实现：
```
1 currentMetro.setX(direction * 10); // 设置出站列车的位置
```
当执行上面那句代码之后上方的 **metroTask** 调度任务执行到 getSpeedByX 这个方法之后获取到的 speed 速度不为 0，因此此时会继续执行地铁行进的动画，此时的速度就是由慢至快，透明度由深至浅。以下为开门动画执行流程：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217223609315-838709115.png)
**自动巡检代码的实现分析**
系统中自动巡检的实现主要是通过修改 3D 场景中的 **eye** 以及 **center** 的值，**HT** 中提供了 **rotate**，**walk** 两个方法来控制视角的旋转以及视角的行进，**rotate** 方法在非第一人称模式时，旋转是以 **center** 为中心进行旋转，也就是围绕中心物体旋转，当为第一人称时旋转以 **eye** 为中心进行旋转，也就是旋转眼睛朝向方向。**walk** 函数同时改变 **eye** 和 **center** 的位置，也就是 **eye** 和 **center** 在两点建立的矢量方向上同时移动相同的偏移量。该系统中我没有采用 **rotate** 函数而是自己实现了视角的旋转，因为原本的 rotate 函数旋转某个角度会马上旋转过去而不会有一个旋转的过程，所以我重新实现了旋转的方法，该系统中视角旋转是通过不断修改 **center** 的数值来实现，具体实现过程原理如下图所示：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217223813536-1894778998.jpg)
部分实现代码如下：
```
1 rotateStep() {
 2     // 即上图辅助点 C
 3     let fromCenter = this.fromCenter;
 4     // 即上图 B 点
 5     let toCenter = this.toCenter;
 6     // 每帧转一度
 7     let rotateValue = this.rotateFrame || Math.PI / 180;
 8     // 辅助点 C 与 B 点之间建立一个方向向量
 9     let centerVector = new ht.Math.Vector2(toCenter.x - fromCenter.x, toCenter.y - fromCenter.y);
10     let centerVectorLength = centerVector.length();
11     // 此时旋转百分比
12     let rotatePercent = rotateValue * this.stepNum / this.curRotateVal;
13     if (rotatePercent >= 1) {
14         rotatePercent = 1;
15         this.stepNum = -2;
16     }
17     let newLength = rotatePercent * centerVectorLength;
18     centerVector.setLength(newLength);
19     let newCenterVector = centerVector.add(fromCenter);
20     // 获取旋转过程中 center 的点信息
21     let newCenterPosition = [newCenterVector.x, this.personHeight, newCenterVector.y];
22     // 设置当前 center 的大小
23     this.g3d.setCenter(newCenterPosition);
24 }
```
通过上述代码就实现了场景中的视角旋转，并且可以通过修改 **rotateValue** 的值控制旋转的速度。
**电梯动画代码的实现分析**
场景中电梯是一个 obj 模型，3D 模型是由最基础的三角形面拼接合成，例如 1 个矩形可以由 2 个三角形构成，1 个立方体由 6 个面即 12 个三角形构成，以此类推更复杂的模型可以由许多的小三角形组合合成。因此 3D 模型定义即为对构造模型的所有三角形的描述，而每个三角形由三个顶点 vertex 构成，每个顶点 vertex 由 x, y, z 三维空间坐标决定，HT 中使用 **vs** 数组记录构成三角面的所有顶点坐标，所以如果想要让电梯运行起来，只需要把所有的顶点坐标往电梯运行的方向进行平移，以下为部分关键伪代码：
```
1 // vs 指的是构成电梯模型所有的三角面顶点坐标数组
 2 // 由于场景中电梯的运行方向为往对角线右上方运动，所以只需要修改 x 轴以及 y 轴坐标值
 3 // xStep yStep 为每次电梯运动的距离
 4 setInterval(() = >{
 5     // i+3 是因为 vs 数组的顺序为 x, y, z 轴 所以每次 i 偏移三个单位大小
 6     for (let i = 0, l = vs.length; i < l; i = i + 3) {
 7         // 该顶点坐标下一个 x 轴坐标的值
 8         let nextX = vs[i] - xStep;
 9         // 该顶点坐标下一个 y 轴坐标的值
10         let nextY = vs[i + 1] + yStep;
11         vs[i] = nextX < -0.5 ? 0.5 - (Math.abs(nextX) - 0.5) : nextX;
12         vs[i + 1] = nextY > 0.5 ? -0.5 + (Math.abs(nextY) - 0.5) : nextY;
13     }
14 },
15 200);
```
电梯运动动画如下图所示：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217224148875-2007009013.gif)
**监控功能展示及介绍**
**视频监控**
当点击场景中的摄像头之后右侧顶部会显示出当前摄像头的监控画面，以下为实现效果图:
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217224224092-1535497454.gif)
**烟雾报警监控**
烟雾报警会根据后台实时传递过来的状态值来变换当前烟雾报警模型的颜色，红色为报警状态，以下为实现效果图:
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217224253149-224991118.gif)
**电视列车到站时间监控**
日常地铁站中会有专门的电视来展示下一班地铁到站的时间表，该系统中也模拟该效果，不过该系统暂时做了电视的模型，时间暂无对接，以下为效果图：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217224314244-590196035.jpg)
**场景监控交互**
3D 场景中交互是比较简单的，主要是点击摄像头展示 2D 监控面板，在 2D 界面中主要是切换三种交互模式，三种交互模式为互斥的关系，以下是 3D 交互注册事件代码:
```
1 g3d.mi((e) = >{
 2     let {
 3         g2d,
 4         dm2d
 5     } = this;
 6     // 为点击类型
 7     if (e.kind === 'clickData') {
 8         // data 为当前点击的图元
 9         let data = e.data;
10         // 当前图元的 shape3d 类型
11         let shape3d = data.s('shape3d');
12         // 判断当前 shape3d 类型是否为摄像头
13         if (shape3d && shape3d.indexOf('摄像头') > 0) {
14             let cameraPanel = dm2d.getDataByTag('cameraPanel');
15             // toggle 切换摄像头 2d 面板
16             g2d.isVisible(cameraPanel) ? cameraPanel.s('2d.visible', false) : cameraPanel.s('2d.visible', true);
17         }
18     }
19     // 为点击 3d 场景背景类型
20     if (e.kind === 'clickBackground') {
21         let cameraPanel = dm2d.getDataByTag('cameraPanel');
22         // 隐藏摄像头 2d 面板
23         g2d.isVisible(cameraPanel) && cameraPanel.s('2d.visible', false);
24     }
25 });
```
**总结**
工业互联网将人，数据和机器连接起来，地铁站 3D 可视化系统则是一个很好的展现，HT 的轻量化，数据的可视化，机器的可视化，资产的管理化帮助我们更好的监控。而物联网将通过各种信息传感设备，实时采集任何需要监控、连接、互动的物体或过程等各种需要的信息，通过与 HT 的结合更好的展现出可视化的优势，当然地铁站还可以与 **VR** 进行结合，在各地科技展会中我们可以见到各种 **VR** 场景操作，**HT** 中也可以结合 **VR** 设备进行操作，可以戴上设备在地铁站中漫游，让人有身临其境的感觉，由于场景本身的轻量化，所以 **VR** 场景下的流畅性也是十分的高，让用户不会有头晕的感觉。当然系统本身也可以在移动端运行，以下为移动端运行截图：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217224707332-642327379.gif)
程序运行截图：
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217224728536-2122459047.jpg)
![](https://img2018.cnblogs.com/blog/591709/201902/591709-20190217224820627-367557523.jpg)
