# 基于HTML5和WebGL的3D网络拓扑结构图 - xhload3d - 博客园
现在，3D模型已经用于各种不同的领域。在医疗行业使用它们制作器官的精确模型；电影行业将它们用于活动的人物、物体以及现实电影；视频游戏产业将它们作为计算机与视频游戏中的资源；在科学领域将它们作为化合物的精确模型；建筑业将它们用来展示提议的建筑物或者风景表现；工程界将它们用于设计新设备、交通工具、结构以及其它应用领域；在最近几十年，地球科学领域开始构建三维地质模型，而且3D模型经常做成动画，例如，在故事片电影以及计算机与视频游戏中大量地应用三维模型。它们可以在三维建模工具中使用或者单独使用。为了容易形成动画，通常在模型中加入一些额外的数据，例如，一些人类或者动物的三维模型中有完整的骨骼系统，这样运动时看起来会更加真实，并且可以通过关节与骨骼控制运动。
这些种种都让我们前端开发者觉得如果我们可以不用学习unity3d或者其他游戏开发工具就能实现3D效果，而且能够精准的靠代码来控制移动或者方向就好了。。。于是我利用[HT For Web](http://www.hightopo.com/)中的[3D组件](http://www.hightopo.com/guide/guide/core/3d/ht-3d-guide.html)来实现了一个小例子，用了[HT](http://www.hightopo.com/)中[3D组件](http://www.hightopo.com/guide/guide/core/3d/ht-3d-guide.html)的大部分功能，做这个例子就是想把[3D组件](http://www.hightopo.com/guide/guide/core/3d/ht-3d-guide.html)好好的掌握，尽量放进一个例子中，到时候别人有需要就可以参考了，但是因为之前从来没有实现过3D的效果，再加上现在手册没有很完整，所以这个例子对我来说还是有点难度的。
本例位置：[http://www.hightopo.com/demo/3DTopology/index.html](http://www.hightopo.com/demo/3DTopology/index.html)
先来看看整体实现的效果图：
![](https://images2017.cnblogs.com/blog/591709/201710/591709-20171017113905474-830306380.gif)
用[HT for Web](http://www.hightopo.com/)，现有的3d模板创建三层底板不是问题，问题是要如何将图中第一层的“电脑”和“机柜组件”放上去？我是在网上down下来的obj格式的文件，然后我利用[HT](http://www.hightopo.com/)中的ht.Default.loadObj(objUrl, mtlUrl, params)函数将模型加载进去，其中的params部分可以参考[http://www.hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html](http://www.hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html)，代码如下：
```
ht.Default.loadObj('obj/机柜组件1.obj', 'obj/机柜组件1.mtl', {  
    cube: true,  
    center: true,  
    shape3d: 'box',  
    finishFunc: function(modelMap, array, rawS3){  
        window.rawS3 = rawS3;  
     　if(modelMap){  
　　      device2 = createNode('box', floor1);  
　　　　device2.p3([x1-120, y1+13, z1+60]);  
　　　　device2.s3(rawS3);  
　　　　createEdge(device1, device2);  
　　　　device3 = createNode('box', floor1);  
　　　　device3.s3(rawS3);  
　　　　device3.p3([x1+120, y1+13, z1+60]);  
　　　　createEdge(device1, device3);  
    　 }  
    }  
});
```
“电脑”上方有个红色的立体能旋转的“警告”，是依靠ht.Default.setShape3dModel函数([HT for Web 建模手册](http://www.hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html))注册的一个3d模型，在ht中，封装好的建模函数有很多，比较基础的就是球体，圆柱，立方体等等，这边我用的是构造环形的方法createRingModel来生成“警告”最外面的环，感叹号的上部分就是用的createSmoothSphereModel构造的球体，感叹号的下部分就是用createSmoothCylinderModel来构造的圆柱。我一开始直接使用了3d模型中封装好的函数，导致后来根本不知道函数中使用的参数是做什么用的，而且也不明白3d模型是怎么构成的，然后自己又重新看了前面的“模型基础”，才知道原来3d模型采用的一个面，最基础的是三角面，之后复杂的面也是由多个三角面来形成的，然后绕着一根特定的轴旋转之后形成的，当然，这个轴是你来决定的，不同的轴可以生成不同的形状，对于颜色等风格方面的设置可以参考[HT for Web 风格手册](http://www.hightopo.com/guide/guide/core/theme/ht-theme-guide.html)。至于如何让这个3d模型旋转起来，ht中封装了addScheduleTask(Task)方法，我在第三层Task中调用了ht封装的一个旋转函数setRotation来设置旋转的顺序和方向，并且指定了旋转的对象。以下是自定义“警告”的3d模型的方法（注意：因为本例的模型是自定义组合的，如果要设置整体模型的颜色要用“all.blend”style属性）：
```
var ringModel = ht.Default.createRingModel([8, 1, 10, 1, 10, -1, 8, -1, 8, 1], null, null, false, false, 100);  
var sphereModel = ht.Default.createSmoothSphereModel(8, 8, 0, Math.PI*2, 0, Math.PI, 2);  
var cylinderModel = ht.Default.createSmoothCylinderModel(8, true, true, 1, 2, 0, Math.PI*2, 8);  
var arr = [  
    {  
        r3: [Math.PI/2, 0, 0],  
        color: 'red'  
    },{  
        shape3d: sphereModel,  
        t3: [0, 4, 0],  
        color: 'red'  
    },{  
        shape3d: cylinderModel,  
        t3: [0, -3, 0],  
        color: 'red'  
    }  
];  
ht.Default.setShape3dModel('alarm', {  
    shape3d: arr  
});
```
要实现3D界面上展现2d图片，只要按照平常的路走就行，因为ht中的Graph3dView和其他的组件的根部都是div，在div上生成图片用的就是原生js，new Image()，再将image的src和大小赋值，并且加到3d面板上就行了，注意这里是加到3d的底层div上，要用g3d.getView().appendChild来添加，我们还可以看到管线上有虚线流动的痕迹，这是通过不断改变“shape3d.uv.offset”参数实现管道流动的特殊效果，详情请参考[HT for Web 形状手册](http://hightopo.com/guide/guide/core/shape/ht-shape-guide.html)。
想让2d图片在3d管线上移动则是使用g3d.toViewPosition(position)来获取3d模型的二维坐标，这个函数中的参数就是三维模型的3d坐标，我们可以直接将polyline管线上的点传入toViewPosition函数中，这边获取管线上的点在ht中已经封装好三个函数getLineCacheInfo、getLineLength、getLineOffset，这样就可以直接获取到二维坐标，然后将img的坐标设置上去即可，以下是img图片在管道上移动的代码：
```
var delta = 10;  
params = {  
    frames: Infinity,  
    interval: 50,  
    action: function(v, t){  
        var length = (polyline.a('total') || 0) % polyline.a('length') + delta;  
        var cache = ht.Default.getLineCacheInfo(polyline.getPoints(), polyline.getSegments());  
        var lineLength = ht.Default.getLineLength(cache);  
        var offset = ht.Default.getLineOffset(cache, length);  
        arr = [offset.point.x, offset.point.y, offset.point.z];  
        var position = g3d.toViewPosition(arr);  
            img.style.left = (position.x - 5) + 'px';  
            img.style.top = (position.y - 5) + 'px';  
            polyline.a('total', length);  
    }  
};  
anim = ht.Default.startAnim(params);
```
可以看到图中第二层中的立方体上有“SDH”的字样，我是通过设置shape3d.top.img: imgURL来实现的，这边的imgURL可以是图片的相对路径，也可以是ht中用ht.Default.setImage声明的img的名称，还可以是json格式构造的图片。在ht中，2D文字显现在3D上，则字体周围会出现“锯齿”，这个时候只要设置“label.transparent: true”即可。
我们还可以看到第二层上有两个特殊的多边形“平行四边形”和“梯形”，因为之前有客户说不知道如何使用createExtrusionModel这个模型函数（[HT for Web 建模手册](http://hightopo.com/guide/guide/plugin/modeling/ht-modeling-guide.html)），我索性就写了一下，平行四边形是靠createParallelogramModel模型函数，这个函数比较简单，createExtrusionModel(array, segments, top, bottom, resolution, repeatUVLength, tall, elevation)，array是你要形成的图形的坐标点，这边只是针对于xz轴上画的平面图形，segments指的是如何连接这几个坐标点，可参考[HT for Web 形状手册](http://hightopo.com/guide/guide/core/shape/ht-shape-guide.html)，top和bottom就是让你选择是否有顶部或者底部，resolution微分段数，我们描绘一段曲线的时候可能只要确认几个个别的点然后在每两个点之间的连线上把它分成多个段，这样这条线段就会变得平滑，ht为了用户能够轻松操作这些线段，就封装了这一个参数，repeatUVLength默认为空，设置值后顶部和底部的贴图将根据制定长度值进行重复，tall模型的高度，默认为5，elevation模型中心的`y`轴位置，默认值为0，设置这个值可以使xz上的平面绕着y轴旋转。
底层的一个环形的效果是通过一个算法来实现的，环形得确认这个环形上有多少个元素，然后算每两个之间的角度，在通过sin、cos来计算每一个元素的位置，得出了如下代码：
```
names = ['设备2', '设备3', '设备4', '设备5', '设备6', '设备7', '设备8', '设备9'];  
names.forEach(function(name, index) {  
　　　　x = 400, y = 200, angle = 45, r = 120;  
　　　　x = x3 + Math.sin((2 * Math.PI / 360) * angle * index) * r;  
　　　　y = z3 + Math.cos((2 * Math.PI / 360) * angle * index) * r;  
　　　　device = createRect([x, y3 + 15, y], [w * 0.1, 15, h * 0.1], '', '', floor3);  
　　　　createEdge(device5, device);  
});
```
