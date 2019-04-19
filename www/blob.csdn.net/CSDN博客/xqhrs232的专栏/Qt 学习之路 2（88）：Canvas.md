# Qt 学习之路 2（88）：Canvas - xqhrs232的专栏 - CSDN博客
2017年10月11日 14:16:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：480
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://www.devbean.net/2015/09/qt-study-road-2-qml-canvas/](https://www.devbean.net/2015/09/qt-study-road-2-qml-canvas/)
相关文章
1、Qt 学习之路 2（89）：Canvas（续）----[https://www.devbean.net/2015/11/qt-study-road-2-qml-canvas-2/](https://www.devbean.net/2015/11/qt-study-road-2-qml-canvas-2/)
在 QML 刚刚被引入到 Qt 4 的那段时间，人们往往在讨论 Qt Quick 是不是需要一个椭圆组件。由此，人们又联想到，是不是还需要其它的形状？这种没玩没了的联想导致了一个最直接的结果：除了圆角矩形，Qt Quick 什么都没有提供，包括椭圆。如果你需要一个椭圆，那就找个图片，或者干脆自己用 C++ 写一个吧（反正 Qt Quick 是可以扩展的，不是么）！
为了使用脚本化的绘图机制，Qt 5 引入的`Canvas`元素。`Canvas`元素提供了一种与分辨率无关的位图绘制机制。通过`Canvas`，你可以使用
 JavaScript 代码进行绘制。如果熟悉 HTML5 的话，Qt Quick 的`Canvas`元素与 HTML5 中的`Canvas`元素如出一辙。
`Canvas`元素的基本思想是，使用一个 2D 上下文对象渲染路径。这个 2D 上下文对象包含所必须的绘制函数，从而使`Canvas`元素看起来就像一个画板。这个对象支持画笔、填充、渐变、文本以及其它一系列路径创建函数。
下面我们看一个简单的路径绘制的例子：


```
```
|123456789101112131415161718192021222324252627282930313233|import QtQuick2.0Canvas{id:root// 画板大小width:200;height:200// 重写绘制函数onPaint:{// 获得 2D 上下文对象varctx=getContext("2d")// 设置画笔ctx.lineWidth=4ctx.strokeStyle="blue"// 设置填充ctx.fillStyle="steelblue"// 开始绘制路径ctx.beginPath()// 移动到左上点作为起始点ctx.moveTo(50,50)// 上边线ctx.lineTo(150,50)// 右边线ctx.lineTo(150,150)// 底边线ctx.lineTo(50,150)// 左边线，并结束路径ctx.closePath()// 使用填充填充路径ctx.fill()// 使用画笔绘制边线ctx.stroke()}}|
上面的代码将在左上角为 (50, 50) 处，绘制一个长和宽均为 100 像素的矩形。这个矩形使用钢铁蓝填充，并且具有蓝色边框。程序运行结果如下所示：
![Canvas](http://files.devbean.net/images/2015/09/qq-canvas-demo.png)
让我们来仔细分析下这段代码。首先，画笔的宽度设置为 4 像素；使用`strokeStyle`属性，将画笔的颜色设置为蓝色。`fillStyle`属性则是设置填充色为 steelblue。只有当调用了`stroke()`或`fill()`函数时，真实的绘制才会执行。当然，我们也完全可以独立使用这两个函数，而不是一起。调用`stroke()`或`fill()`函数意味着将当前路径绘制出来。需要注意的是，路径是不能够被复用的，只有当前绘制状态才能够被复用。所谓“当前绘制状态”，指的是当前的画笔颜色、宽度、填充色等属性。
在 QML 中，`Canvas`元素就是一种绘制的容器。2D 上下文对象作为实际绘制的执行者。绘制过程必须在`onPaint`事件处理函数中完成。下面即一个代码框架：

``
|12345678|Canvas{width:200;height:200onPaint:{varctx=getContext("2d")// 设置绘制属性// 开始绘制}}|
`Canvas`本身提供一个典型的二维坐标系，原点在左上角，X 轴正方向向右，Y 轴正方向向下。使用`Canvas`进行绘制的典型过程是：
- 
设置画笔和填充样式
- 
创建路径
- 
应用画笔和填充
例如：


```
```
|1234567891011121314|onPaint:{varctx=getContext("2d")// 设置画笔ctx.strokeStyle="red"// 创建路径ctx.beginPath()ctx.moveTo(50,50)ctx.lineTo(150,50)// 绘制ctx.stroke()}|
上面这段代码运行结果应该是一个从 (50, 50) 开始，到 (150, 50) 结束的一条红色线段。
由于我们在创建路径之前会将画笔放在起始点的位置，因此，在调用`beginPath()`函数之后的第一个函数往往是`moveTo()`。
## 形状 API
除了自己进行路径的创建之外，`Canvas`还提供了一系列方便使用的函数，用于一次添加一个矩形等，例如：

``
|12345678910111213141516171819|importQtQuick2.0Canvas{id:rootwidth:120;height:120onPaint:{varctx=getContext("2d")ctx.fillStyle='green'ctx.strokeStyle="blue"ctx.lineWidth=4// 填充矩形ctx.fillRect(20,20,80,80)// 裁减掉内部矩形ctx.clearRect(30,30,60,60)// 从左上角起，到外层矩形中心绘制一个边框ctx.strokeRect(20,20,40,40)}}|
代码运行结果如下：
![canvas rectangle](http://files.devbean.net/images/2015/09/qq-canvas-rect.png)
注意蓝色边框的位置。在绘制边框时，画笔会沿着路径进行绘制。上面给出的 4 像素边框，其中心点为路径，因此会有 2 像素在路径外侧，2 像素在路径内侧。
## 渐变
`Canvas`元素可以使用颜色进行填充，同样也可以使用渐变。例如下面的代码：


```
```
|123456789|onPaint:{varctx=getContext("2d")vargradient=ctx.createLinearGradient(100,0,100,200)gradient.addColorStop(0,"blue")gradient.addColorStop(0.5,"lightsteelblue")ctx.fillStyle=gradientctx.fillRect(50,50,100,100)}|
运行结果如下所示：
![canvas 渐变](http://files.devbean.net/images/2015/09/qq-canvas-gradient.png)
在这个例子中，渐变的起始点位于 (100, 0)，终止点位于 (100, 200)。注意这两个点的位置，这两个点实际创建了一条位于画布中央位置的竖直线。渐变类似于插值，可以在 [0.0, 1.0] 区间内插入一个定义好的确定的颜色；其中，0.0 意味着渐变的起始点，1.0 意味着渐变的终止点。上面的例子中，我们在 0.0 的位置（也就是渐变起始点 (100, 0) 的位置）设置颜色为“blue”；在 1.0 的位置（也就是渐变终止点 (100, 200) 的位置）设置颜色为“lightsteelblue”。注意，渐变的范围可以大于实际绘制的矩形，此时，绘制出来的矩形实际上裁减了渐变的一部分。因此，渐变的定义其实是依据画布的坐标，也不是定义的绘制路径的坐标。
## 阴影
路径可以使用阴影增强视觉表现力。我们可以把阴影定义为一个围绕在路径周围的区域，这个区域会有一定的偏移、有一定的颜色和特殊的模糊效果。我们可以使用`shadowColor`属性定义阴影的颜色；使用`shadowOffsetX`属性定义阴影在 X 轴方向的偏移量；使用`shadowOffsetY`属性定义阴影在 Y 轴方向的偏移量；使用`shadowBlur`属性定义阴影模糊的程度。不仅是阴影，利用这种效果，我们也可以实现一种围绕在路径周边的发光特效。下面的例子中，我们将创建一个带有发光效果的文本。为了更明显的显示发光效果，其背景界面将会是深色的。下面是相应的代码：

``
|123456789101112131415161718192021|importQtQuick2.0Canvas{id:rootwidth:280;height:120onPaint:{varctx=getContext("2d")// 背景矩形ctx.strokeStyle="#333"ctx.fillRect(0,0,root.width,root.height);// 设置阴影属性ctx.shadowColor="blue";ctx.shadowOffsetX=2;ctx.shadowOffsetY=2;ctx.shadowBlur=10;// 设置字体并绘制ctx.font='bold 80px sans-serif';ctx.fillStyle="#33a9ff";ctx.fillText("Earth",30,80);}}|
首先，我们利用 #333 填充了一个背景矩形。矩形的起始点位于原点，长度和宽度分别绑定到画布的长度和宽度。接下来定义阴影的属性。最后，我们设置文本字体为 80 像素加粗的 sans-serif，会绘制了“Earth”单词。代码运行结果如下所示：
![canvas 阴影](http://files.devbean.net/images/2015/09/qq-canvas-shadow.png)
注意观察字母旁边的发光效果，这其实是使用阴影制作的。
## 图像
`Canvas`元素支持从多种源绘制图像。为了绘制图像，需要首先加载图像；使用`Component.onCompleted`事件处理函数可以达到这一目的：


```
```
|1234567891011121314151617181920212223242526272829|onPaint:{varctx=getContext("2d")// 绘制图像ctx.drawImage('assets/earth.png',10,10)// 保存当前状态ctx.save()// 平移坐标系ctx.translate(100,0)ctx.strokeStyle='red'// 创建裁剪范围ctx.beginPath()ctx.moveTo(10,10)ctx.lineTo(55,10)ctx.lineTo(35,55)ctx.closePath()ctx.clip()// 根据路径裁剪// 绘制图像并应用裁剪ctx.drawImage('assets/earth.png',10,10)// 绘制路径ctx.stroke()// 恢复状态ctx.restore()}Component.onCompleted:{loadImage("assets/earth.png")}|
代码运行结果如下：
![canvas image](http://files.devbean.net/images/2015/09/qq-canvas-image.png)
左侧的地球图像绘制在左上角坐标为 (10, 10) 的位置；右侧的图像应用了路径裁剪。图像和路径都可以被另外的路径裁剪，只需使用`clip()`函数即可。调用该函数之后，所有的绘制都将限制在这个路径中，也就是所谓“裁剪”。裁剪会在恢复上次状态时被取消。
