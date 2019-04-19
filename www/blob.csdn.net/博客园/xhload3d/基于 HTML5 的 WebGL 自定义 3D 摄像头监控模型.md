# 基于 HTML5 的 WebGL 自定义 3D 摄像头监控模型 - xhload3d - 博客园
**前言**
随着视频监控联网系统的不断普及和发展, 网络摄像机更多的应用于监控系统中，尤其是高清时代的来临，更加快了网络摄像机的发展和应用。
在监控摄像机数量的不断庞大的同时，在监控系统中面临着严峻的现状问题：海量视频分散、孤立、视角不完整、位置不明确等问题，始终围绕着使用者。因此，如何更直观、更明确的管理摄像机和掌控视频动态，已成为提升视频应用价值的重要话题。所以当前项目正是从解决此现状问题的角度，应运而生。围绕如何提高、管理和有效利用前端设备采集的海量信息为公共安全服务，特别是在技术融合大趋势下，如何结合当前先进的视频融合，虚实融合、三维动态等技术，实现三维场景实时动态可视化监控，更有效的识别、分析、挖掘海量数据的有效信息服务公共应用，已成为视频监控平台可视化发展的趋势和方向。目前，在监控行业中，海康、大华等做监控行业领导者可基于这样的方式规划公共场所园区等的摄像头规划安放布局，可以通过海康、大华等摄像头品牌的摄像头参数，调整系统中摄像头模型的可视范围，监控方向等，更方便的让人们直观的了解摄像头的监控区域，监控角度等。
以下是项目地址：[基于 HTML5 的 WebGL 自定义 3D 摄像头监控模型](http://www.hightopo.com/demo/Camera/index.html)
**效果预览**
**整体场景-摄像头效果图**
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074129353-858216626.gif)
**局部场景-摄像头效果图**
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074159610-98573873.gif)
**代码生成**
**摄像头模型及场景**
项目中使用的摄像头模型是通过 3dMax 建模生成的，该建模工具可以导出 obj 与 mtl 文件，在 HT 中可以通过解析 obj 与 mtl 文件来生成 3d 场景中的摄像头模型。
项目中场景通过 HT 的 3d 编辑器进行搭建，场景中的模型有些是通过 HT 建模，有些通过 3dMax 建模，之后导入 HT 中，场景中的地面白色的灯光，是通过 HT 的 3d 编辑器进行地面贴图呈现出来的效果。 
**锥体建模**
3D 模型是由最基础的三角形面拼接合成，例如 1 个矩形可以由 2 个三角形构成，1 个立方体由 6 个面即 12 个三角形构成， 以此类推更复杂的模型可以由许多的小三角形组合合成。因此 3D 模型定义即为对构造模型的所有三角形的描述， 而每个三角形由三个顶点 vertex 构成， 每个顶点 vertex 由 x, y, z 三维空间坐标决定，HT 采用右手螺旋定则来确定三个顶点构造三角形面的正面。
HT 中通过 **ht.Default.setShape3dModel(name, model)** 函数，可注册自定义 3D 模型，摄像头前方生成的锥体便是通过该方法生成。可以将该锥体看成由 5 个顶点，6 个三角形组成，具体图如下：
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074221681-1867969782.jpg)
**ht.Default.setShape3dModel(name, model)**
1. **name** 为模型名称，如果名称与预定义的一样，则会替换预定义的模型 
2. **model** 为JSON类型对象，其中 **vs** 表示顶点坐标数组，**is** 表示索引数组，**uv** 表示贴图坐标数组，如果想要单独定义某个面，可以通过 **bottom_vs，bottom_is，bottom_uv，top_vs，top_is， top_uv**等来定义，之后便可以通过** shape3d.top.*， shape3d.bottom.***  等单独控制某个面
**以下是我定义模型的代码:**
```
// camera 是当前的摄像头图元
// fovy 为摄像头的张角的一半的 tan 值
var setRangeModel = function(camera, fovy) {
    var fovyVal = 0.5 * fovy;
    var pointArr = [0, 0, 0, -fovyVal, fovyVal, 0.5, fovyVal, fovyVal, 0.5, fovyVal, -fovyVal, 0.5, -fovyVal, -fovyVal, 0.5];
    ht.Default.setShape3dModel(camera.getTag(), [{
        vs: pointArr,
        is: [2, 1, 0, 4, 1, 0, 4, 3, 0, 3, 2, 0],
        from_vs: pointArr.slice(3, 15),
        from_is: [3, 1, 0, 3, 2, 1],
        from_uv: [0, 0, 1, 0, 1, 1, 0, 1]
    }]);
}
```
我将当前摄像头的 tag 标签值作为模型的名称，tag 标签在 HT 中用于唯一标识一个图元，用户可以自定义 tag 的值。通过 pointArr 记录当前五面体的五个顶点坐标信息，代码中通过 from_vs, from_is, from_uv 单独构建五面体底面，底面用于显示当前摄像头呈现的图像。
代码中设置了锥体 style 对象的 **wf.geometry** 属性，通过该属性可以为锥体添加模型的线框，增强模型的立体效果，并且通过** wf.color，wf.width** 等参数调节线框的颜色，粗细等。
相关模型 style 属性的设置代码如下：
```
1 rangeNode.s({
 2     'shape3d': cameraName,
 3     // 摄像头模型名称
 4     'shape3d.color': 'rgba(52, 148, 252, 0.3)',
 5     // 锥体模型颜色
 6     'shape3d.reverse.flip': true,
 7     // 锥体模型的反面是否显示正面的内容
 8     'shape3d.light': false,
 9     // 锥体模型是否受光线影响
10     'shape3d.transparent': true,
11     // 锥体模型是否透明
12     '3d.movable': false,
13     // 锥体模型是否可移动
14     'wf.geometry': true // 是否显示锥体模型线框
15 });
```
**摄像头图像生成原理**
**透视投影**
透视投影是为了获得接近真实三维物体的视觉效果而在二维的纸或者画布平面上绘图或者渲染的一种方法，它也称为透视图。 透视使得远的对象变小，近的对象变大，平行线会出现先交等更更接近人眼观察的视觉效果。
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074233279-699657637.png)
如上图所示，透视投影最终显示到屏幕上的内容只有截头锥体( View Frustum )部分的内容， 因此 Graph3dView 提供了 eye, center, up, far，near，fovy 和 aspect 参数来控制截头锥体的具体范围。具体的透视投影可以参考 **HT for Web** 的 [3D](https://hightopo.com/guide/guide/core/3d/ht-3d-guide.html) 手册。
根据上图的描述，在本项目中可以在摄像头初始化之后，缓存当前 3d 场景 eyes 眼睛的位置，以及 center 中心的位置，之后将 3d 场景 eyes 眼睛和 center 中心设置成摄像头中心点的位置，然后在这个时刻获取当前 3d 场景的截图，该截图即为当前摄像头的监控图像，之后再将 3d 场景的 center 与 eyes 设置成开始时缓存的 eyes 与 center 位置，通过该方法即可实现 3d 场景中任意位置的快照，从而实现摄像头监控图像实时生成。
**相关伪代码如下：**
```
1 function getFrontImg(camera, rangeNode) {
 2     var oldEye = g3d.getEye();
 3     var oldCenter = g3d.getCenter();
 4     var oldFovy = g3d.getFovy();
 5     g3d.setEye(摄像头位置);
 6     g3d.setCenter(摄像头朝向);
 7     g3d.setFovy(摄像头张角);
 8     g3d.setAspect(摄像头宽高比);
 9     g3d.validateImp();
10     g3d.toDataURL();
11     g3d.setEye(oldEye);;
12     g3d.setCenter(oldCenter);
13     g3d.setFovy(oldFovy);
14     g3d.setAspect(undefined);
15     g3d.validateImp();
16 }
```
经过测试之后，通过该方法进行图像的获取会导致页面有所卡顿，因为是获取当前 3d 场景的整体截图，由于当前3d场景是比较大的，所以 toDataURL 获取图像信息是非常慢的，因此我采取了离屏的方式来获取图像，具体方式如下：
   1. 创建一个新的 3d 场景，将当前场景的宽度与高度都设置为 200px 的大小，并且当前 3d 场景的内容与主屏的场景是一样的，HT中通过 new ht.graph3d.Graph3dView(dataModel) 来新建场景，其中的 dataModel 为当前场景的所有图元，所以主屏与离屏的 3d 场景都共用同一个 dataModel，保证了场景的一致。
   2. 将新创建的场景位置设置成屏幕看不到的地方，并且添加进 dom 中。
   3. 将之前对主屏获取图像的操作变成对离屏获取图像的操作，此时离屏图像的大小相对之前主屏获取图像的大小小很多，并且离屏获取不需要保存原来的眼睛 eyes 的位置以及 center 中心的位置，因为我们没有改变主屏的 eyes 与 center 的位置， 所以也减少的切换带来的开销，大大提高了摄像头获取图像的速度。
**以下是该方法实现的代码：**
```
1 function getFrontImg(camera, rangeNode) {
 2     // 截取当前图像时将该摄像头所属的五面体隐藏
 3     rangeNode.s('shape3d.from.visible', false);
 4     rangeNode.s('shape3d.visible', false);
 5     rangeNode.s('wf.geometry', false);
 6     var cameraP3 = camera.p3();
 7     var cameraR3 = camera.r3();
 8     var cameraS3 = camera.s3();
 9     var updateScreen = function() {
10         demoUtil.Canvas2dRender(camera, outScreenG3d.getCanvas());
11         rangeNode.s({
12             'shape3d.from.image': camera.a('canvas')
13         });
14         rangeNode.s('shape3d.from.visible', true);
15         rangeNode.s('shape3d.visible', true);
16         rangeNode.s('wf.geometry', true);
17     };
18 
19     // 当前锥体起始位置
20     var realP3 = [cameraP3[0], cameraP3[1] + cameraS3[1] / 2, cameraP3[2] + cameraS3[2] / 2];
21     // 将当前眼睛位置绕着摄像头起始位置旋转得到正确眼睛位置
22     var realEye = demoUtil.getCenter(cameraP3, realP3, cameraR3);
23 
24     outScreenG3d.setEye(realEye);
25     outScreenG3d.setCenter(demoUtil.getCenter(realEye, [realEye[0], realEye[1], realEye[2] + 5], cameraR3));
26     outScreenG3d.setFovy(camera.a('fovy'));
27     outScreenG3d.validate();
28     updateScreen();
29 }
```
上面代码中有一个 getCenter 方法是用于获取 3d 场景中点 A 绕着点 B 旋转 angle 角度之后得到的点 A 在 3d 场景中的位置，方法中采用了 HT 封装的 ht.Math 下面的方法，以下为代码：
```
1 // pointA 为 pointB 围绕的旋转点
 2 // pointB 为需要旋转的点
 3 // r3 为旋转的角度数组 [xAngle, yAngle, zAngle] 为绕着 x, y, z 轴分别旋转的角度 
 4 var getCenter = function(pointA, pointB, r3) {
 5     var mtrx = new ht.Math.Matrix4();
 6     var euler = new ht.Math.Euler();
 7     var v1 = new ht.Math.Vector3();
 8     var v2 = new ht.Math.Vector3();
 9 
10     mtrx.makeRotationFromEuler(euler.set(r3[0], r3[1], r3[2]));
11 
12     v1.fromArray(pointB).sub(v2.fromArray(pointA));
13     v2.copy(v1).applyMatrix4(mtrx);
14     v2.sub(v1);
15 
16     return [pointB[0] + v2.x, pointB[1] + v2.y, pointB[2] + v2.z];
17 };
```
这里应用到向量的部分知识，具体如下：
**OA + OB = OC**
**![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074251110-1058475078.jpg)**
方法分为以下几个步骤求解：
   1.  **var mtrx = new ht.Math.Matrix4()** 创建一个转换矩阵，通过 **mtrx.makeRotationFromEuler(euler.set(r3[0], r3[1], r3[2]))** 获取绕着 r3[0]，r3[1]，r3[2] 即 x 轴，y 轴，z 轴旋转的旋转矩阵。
   2. 通过** new ht.Math.Vector3()** 创建 v1，v2 两个向量。
   3.** v1.fromArray(pointB)** 为建立一个从原点到 pointB 的一个向量。
   4. **v2.fromArray(pointA)** 为建立一个从原点到 pointA 的一个向量。
   5. **v1.fromArray(pointB).sub(v2.fromArray(pointA))** 即向量 OB - OA 此时得到的向量为 AB，此时 v1 变为向量 AB。
   6. **v2.copy(v1)** v2 向量拷贝 v1 向量，之后通过 **v2.copy(v1).applyMatrix4(mtrx)** 对 v2 向量应用旋转矩阵，变换之后即为 v1向量绕着 pointA 旋转之后的的向量 v2。
   7. 此时通过 **v2.sub(v1)** 就获取了起始点为 pointB，终点为 pointB 旋转之后点构成的向量，该向量此时即为 v2。
   8. 通过向量公式得到旋转之后的点为 [pointB[0] + v2.x, pointB[1] + v2.y, pointB[2] + v2.z]。
项目中的 3D 场景例子其实是 [Hightopo](http://www.hightopo.com) 最近贵州数博会，HT 上工业互联网展台的 VR 示例，大众对 VR/AR 的期待很高，但路还是得一步步走，即使融资了 23 亿美金的 Magic Leap 的第一款产品也只能是 [Full of Shit](https://mp.weixin.qq.com/s/Z-gcPasf6E4OdNbqBwBhFA)，这话题以后再展开，这里就上段当时现场的视频照片：
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074306832-325606716.png)
**2d 图像贴到 3d 模型**
通过上一步的介绍我们可以获取当前摄像机位置的截屏图像，那么如何将当前图像贴到前面所构建的五面体底部呢？前面通过 from_vs, from_is 来构建底部的长方形，所以在 HT 中可以通过将五面体的 style 中 shape3d.from.image 属性设置成当前图像，其中 from_uv 数组用来定义贴图的位置，具体如下图：
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074323039-360588324.jpg)
**以下为定义贴图位置 from_uv 的代码：**
**`1 from_uv: [0, 0, 1, 0, 1, 1, 0, 1]`**
from_uv 就是定义贴图的位置数组，根据上图的解释，可以将 2d 图像贴到 3d 模型的 from 面。
**控制面板**
HT 中通过 **new ht.widget.Panel()** 来生成如下图的面板：
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074334164-1462206036.gif)
面板中每个摄像头都有一个模块来呈现当前监控图像，其实这个地方也是一个 canvas，该 canvas 与场景中锥体前面的监控图像是同一个 canvas，每一个摄像头都有一个自己的 canvas 用来保存当前摄像头的实时监控画面，这样就可以将该 canvas 贴到任何地方，将该 canvas 添加进面板的代码如下：
`1 formPane.addRow([{ 2 element: camera.a('canvas') 3 }], 240, 240);`
代码中将 canvas 节点存储在摄像头图元的 attr 属性下面，之后便可以通过** camera.a('canvas')** 来获取当前摄像头的画面。
在面板中的每一个控制节点都是通过** formPane.addRow** 来进行添加，具体可参考 HT for Web 的[表单手册](http://www.hightopo.com/guide/guide/plugin/form/ht-form-guide.html)。之后通过 ht.widget.Panel 将表单面板 formPane 添加进 panel 面板中，具体可参考 HT for Web 的[面板手册](http://www.hightopo.com/guide/guide/plugin/panel/ht-panel-guide.html)。
**部分控制代码如下：**
```
1 formPane.addRow(['rotateY', {
 2     slider: {
 3         min: -Math.PI,
 4         max: Math.PI,
 5         value: r3[1],
 6         onValueChanged: function() {
 7             var cameraR3 = camera.r3();
 8             camera.r3([cameraR3[0], this.getValue(), cameraR3[2]]);
 9             rangeNode.r3([cameraR3[0], this.getValue(), cameraR3[2]]);
10             getFrontImg(camera, rangeNode);
11         }
12     }
13 }], [0.1, 0.15]);
```
控制面板通过 **addRow** 来添加控制元素，以上代码为添加摄像头绕着 y 轴进行旋转的控制，**onValueChanged** 在 slider 的数值改变的时候调用，此时通过** camera.r3()** 获取当前摄像头的旋转参数， 由于是绕着 y 轴旋转所以 x 轴与 z 轴的角度是不变的，变的是 y 轴的旋转角度，所以通过** camera.r3([cameraR3[0], this.getValue(), cameraR3[2]])** 来调整摄像头的旋转角度以及通过 **rangeNode.r3([cameraR3[0], this.getValue(), cameraR3[2]])** 来设置摄像头前方锥体的旋转角度，然后调用之前封装好的 **getFrontImg** 函数来获取此时旋转角度下面的实时图像信息。
项目中通过 Panel 面板的配置参数 **titleBackground: rgba(230, 230, 230, 0.4) **即可将标题背景设置为具有透明度的背景，其它类似的 titleColor, titleHeight 等标题参数都可以配置，通过 separatorColor,separatorWidth 等分割参数可以设置内部面板之间分割线的颜色，宽度等。最后面板通过** panel.setPositionRelativeTo('rightTop')** 将面板的位置设置成右上角，并且通过 **document.body.appendChild(panel.getView())** 将面板最外层的 div 添加进页面中， **panel.getView()** 用来获取面板的最外层 dom 节点。
**具体初始化面板代码如下：**
```
1 function initPanel() {
 2     var panel = new ht.widget.Panel();
 3     var config = {
 4         title: "摄像头控制面板",
 5         titleBackground: 'rgba(230, 230, 230, 0.4)',
 6         titleColor: 'rgb(0, 0, 0)',
 7         titleHeight: 30,
 8         separatorColor: 'rgb(67, 175, 241)',
 9         separatorWidth: 1,
10         exclusive: true,
11         items: []
12     };
13     cameraArr.forEach(function(data, num) {
14         var camera = data['camera'];
15         var rangeNode = data['rangeNode'];
16         var formPane = new ht.widget.FormPane();
17         initFormPane(formPane, camera, rangeNode);
18         config.items.push({
19             title: "摄像头" + (num + 1),
20             titleBackground: 'rgba(230, 230, 230, 0.4)',
21             titleColor: 'rgb(0, 0, 0)',
22             titleHeight: 30,
23             separatorColor: 'rgb(67, 175, 241)',
24             separatorWidth: 1,
25             content: formPane,
26             flowLayout: true,
27             contentHeight: 400,
28             width: 250,
29             expanded: num === 0
30         });
31     });
32     panel.setConfig(config);
33     panel.setPositionRelativeTo('rightTop');
34     document.body.appendChild(panel.getView());
35     window.addEventListener("resize",
36     function() {
37         panel.invalidate();
38     });
39 }
```
在控制面板中可以调整摄像头的方向，摄像头监控的辐射范围，摄像头前方锥体的长度等等，并且摄像头的图像是实时生成，以下为运行截图：
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074359161-1813880965.gif)
以下是本项目采用的 3D 场景结合 VR 技术实现的操作：
![](https://images2018.cnblogs.com/blog/591709/201808/591709-20180813074423850-451548743.gif)
