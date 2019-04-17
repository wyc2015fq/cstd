# 三分钟训练眼球追踪术，AI就知道你在看哪个妹子 - 知乎
# 



> 圆栗子 编译整理
量子位 出品 | 公众号 QbitAI
![](https://pic4.zhimg.com/v2-52ee4744e5c78b3a03995b2462f989df_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='690' height='450'></svg>)
啊，老板的眼神飞过来了，还不快切回工作界面？

从前，我们几乎无从躲避来自身后的目光，但现在不一定了。

如果有个**眼球追踪**AI，加上**人脸识别**，或许就能在被老板盯上的瞬间，进入奋力工作模式。

戏是有点多。不过眼球追踪这件事，只要有电脑的前置摄像头，再有个浏览器，真的可以做到。
![](https://pic3.zhimg.com/v2-a9a12b2c27bf05cdf16daef9daa3e682_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='468'></svg>)
来自慕尼黑的程序猿Max Schumacher，就用**TensorFlow.js**做了一个模型，你看向屏幕的**某一点**，它就知道你在看的是哪一点了。

## **我来训练一把**

这个模型叫***Lookie Lookie***，不用服务器，打开摄像头就可以在浏览器上训练，**不出三分钟**就能养成一只小AI。

在下试了一试。

摄像头拍到的画面就显示在屏幕左上角，脸上是绿色的轮廓，眼睛被一个红色方框框住。
![](https://pic2.zhimg.com/v2-28b503e5390381d844fd8ab856a65915_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='312' height='136'></svg>)
收集数据的方式很简单，只要四处移动鼠标，眼睛跟着鼠标走，然后随时按下空格键，每按一次就采集一个数据点。

**第一波**，只要按20次空格，系统就提示，可以点击训练按钮了。

训练好之后，屏幕上出现一个**绿圈圈**。这时候，我的眼睛看哪里，绿圈圈都应该跟着我走的。
![](https://pic3.zhimg.com/v2-6266384a85ab2952d2e72f93f3a4bb2e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='368'></svg>)
可它似乎有些犹豫。系统又提示：现在数据不太够，可能还没训练好，再取一些数据吧。

那好，再取个二三十张图，训练第二波。

果然，这次绿圈圈跑得**自信**了一些，左看右看，它都驰骋 (比较) 如风。
![](https://pic4.zhimg.com/v2-42fbab9f53dc6f1b488f81a2d0af0327_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='580' height='137'></svg>)
相比之下，对于**上下**移动的目光，AI的反应似乎没有那么敏锐。大概是因为，电脑屏幕上下距离不够宽，眼球转动不充分吧。

不过，在训练数据如此贫乏的前提下，神经网络也算是茁壮成长了。

需要注意的是，收集数据的时候，脸**不要离屏幕太远** (也不要倒立🙃) 。

## **DIY全攻略 (上) ：架子搭起来**

作为一个不需要任何服务器就能训练的模型，如果要处理**整幅整幅**的视频截图，负担可能有些重。
![](https://pic4.zhimg.com/v2-e89efeae4290b7f43c570bca307ea16b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='275'></svg>)
所以，还是先**检测人脸**，再框出眼睛所在的部分。**只把**这个区域 (上图右一) 交给神经网络的话，任务就轻松了。

德国少年选择了**clmtrackr**人脸检测模型，它的优点也是跑起来轻快。

那么，先把它下下来：

> [https://raw.githubusercontent.com/auduno/clmtrackr/dev/build/clmtrackr.js](https://link.zhihu.com/?target=https%3A//raw.githubusercontent.com/auduno/clmtrackr/dev/build/clmtrackr.js)

然后，打开一个空的**html**文件，导入jQuery, TensorFlow.js，clmtrackr.js，以及main.js。代码如下：


```python
<!doctype html>
<html>
<body>
    <script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@tensorflow/tfjs@0.12.0"></script>
    <script src="clmtrackr.js"></script>
    <script src="main.js"></script>
</body>
</html>
```


这样，准备活动就做好了。下面正式开始。

## **· 导出视频流**

第一步，要经过你 (用户) 的**同意**，才能打开摄像头，渲染视频流，把画面显示在页面上。

先写这行代码 (此处默认用的是最新版本的Chrome) ：


```python
<video id="webcam" width="400" height="300" autoplay></video>
```


然后从main.js开始：


```python
$(document).ready(function() {
  const video = $('#webcam')[];

  function onStreaming(stream) {
    video.srcObject = stream;
  }

  navigator.mediaDevices.getUserMedia({ video: true }).then(onStreaming);
});
```


到这里，浏览器就该问你“要不要打开摄像头”了。

## **· 找到你的脸**

上文提到的clmtrackr.js人脸追踪器，这里就出场。

先在const video=…下面，**初始化**追踪器：


```python
const ctrack = new clm.tracker();
ctrack.init();
```


然后，在onStreaming() 里面，加下面这句话，就能让追踪器检测视频里的人脸了：


```python
ctrack.start(video);
```


写好这几行，它应该**已经**能看出你的脸。不相信的话，就让它**描出来**。

这里需要一个绘图工具。用html里面的<canvas>标签，在视频上面**重叠一张画布**。

在<video>下面，写上这一串代码：


```python
<canvas id="overlay" width="400" height="300"></canvas>
<style>
    #webcam, #overlay {
        position: absolute;
        top: ;
        left: ;
    }
</style>
```


这样，就有了跟视频**尺寸一样的画布**。CSS能保证画布和视频的位置完全吻合。

浏览器每做一次渲染，我们就要在画布上画点什么了。画之前，要先把之前画过的内容擦掉。

代码长这样，写在ctrack.init() 下面：


```python
const overlay = $('#overlay')[];
 const overlayCC = overlay.getContext('2d');
 
 function trackingLoop() {
   // Check if a face is detected, and if so, track it.
   requestAnimationFrame(trackingLoop);
 
   let currentPosition = ctrack.getCurrentPosition();
   overlayCC.clearRect(, , , );

  if (currentPosition) {
    ctrack.draw(overlay);
  }
}
```


现在，在onStreaming() 的ctrack.starg() 后面，调用trackingLoop() 。每一帧里，它都会重新运行。

这个时候，刷新一下浏览器，你的脸上应该有一个**绿色又诡异的轮廓**了。
![](https://pic3.zhimg.com/v2-72d40dd204170972db4d5de8718c76ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='300'></svg>)
## **· 眼睛截下来**

这一步，是要在眼睛周围画个**矩形框**。

cmltrackr很善良，除了画个轮廓之外，还有**70**个面部特征，我们可以选择自己需要的部分。



![](https://pic1.zhimg.com/v2-7f2dce34151d52f5426f1298385aa9a8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='407' height='420'></svg>)



这里，选23、28、24、26就够了，在每个方向上，往外扩大5个像素。

然后，矩形框应该足够覆盖重要面部信息了 (不离太远、不倒立) 。

现在，再拿**另外一张画布**，来捕捉这个截下来的矩形。这张画布**50 x 25**像素即可，只要把矩形框的尺寸调一下，就能放进去：


```python
<canvas id="eyes" width="50" height="25"></canvas>
<style>
    #eyes {
        position: absolute;
        top: ;
        right: ;
    }
</style>
```


下面这个函数，会返回 (x,y) 坐标，以及矩形的长宽。给它输入的是clmtrackr里面的位置阵列 (Position Array) ：


```python
function getEyesRectangle(positions) {
   const minX = positions[][] - ;
   const maxX = positions[][] + ;
   const minY = positions[][] - ;
   const maxY = positions[][] + ;
 
   const width = maxX - minX;
   const height = maxY - minY;
 
  return [minX, minY, width, height];
}
```


接下来，要把矩形框提取出来。具体方法是，在第一张画布上把它描成红色，再复制到第二张画布上。

替换trackingLoop() 里面的if块：


```python
if (currentPosition) {
   // Draw facial mask on overlay canvas:
   ctrack.draw(overlay);
 
   // Get the eyes rectangle and draw it in red:
   const eyesRect = getEyesRectangle(currentPosition);
   overlayCC.strokeStyle = 'red';
   overlayCC.strokeRect(eyesRect[], eyesRect[], eyesRect[], eyesRect[]);
 
  // The video might internally have a different size, so we need these
  // factors to rescale the eyes rectangle before cropping:
  const resizeFactorX = video.videoWidth / video.width;
  const resizeFactorY = video.videoHeight / video.height;

  // Crop the eyes from the video and paste them in the eyes canvas:
  const eyesCanvas = $('#eyes')[];
  const eyesCC = eyesCanvas.getContext('2d');

  eyesCC.drawImage(
    video,
    eyesRect[] * resizeFactorX, eyesRect[] * resizeFactorY,
    eyesRect[] * resizeFactorX, eyesRect[] * resizeFactorY,
    , , eyesCanvas.width, eyesCanvas.height
  );
}
```


现在，应该看得到眼睛周围的**红色矩形框**了。

## **DIY全攻略 (下) ：训练与测试**

## **· 收集数据**

眼球追踪，收集数据的方法其实有很多种。不过，让眼睛跟着鼠标走，是最简单的，随时按下空格都可以捕获一幅图像。

**1) 追踪鼠标**

想知道鼠标每时每刻都在什么位置，就给document.onmousemove加上一个EventListener。

这样做还可以把坐标归一化 (转化到 [-1, 1] 的范围里) ：


```python
// Track mouse movement:
 const mouse = {
   x: ,
   y: ,
 
   handleMouseMove: function(event) {
     // Get the mouse position and normalize it to [-, ]
     mouse.x = (event.clientX / $(window).width()) *  - ;
     mouse.y = (event.clientY / $(window).height()) *  - ;
  },
}

document.onmousemove = mouse.handleMouseMove;
```


**2) 捕捉图像**

这里要做的是，按下空格键**之后**的任务：从画布上捕捉图像，储存为张量。

TensorFlow.js提供了一个助手函数，叫tf.fromPixels() ，只要用它来储存第二张画布里走出的图像，然后归一化：


```python
function getImage() {
   // Capture the current image in the eyes canvas as a tensor.
   return tf.tidy(function() {
     const image = tf.fromPixels($('#eyes')[]);
     // Add a batch dimension:
     const batchedImage = image.expandDims();
     // Normalize and return it:
     return batchedImage.toFloat().div(tf.scalar()).sub(tf.scalar());
   });
}
```


注意注意，虽然把所有数据做成一个**大训练集**也是可以的，但还是留一部分做**验证集**比较科学，比如20%。

这样，便与检测模型的性能，以及确认它没有**过拟合**。

以下是添加新数据点用的代码：


```python
const dataset = {
   train: {
     n: ,
     x: null,
     y: null,
   },
   val: {
     n: ,
     x: null,
    y: null,
  },
}

function captureExample() {
  // Take the latest image from the eyes canvas and add it to our dataset.
  tf.tidy(function() {
    const image = getImage();
    const mousePos = tf.tensor1d([mouse.x, mouse.y]).expandDims();

    // Choose whether to add it to training (%) or validation (%) set:
    const subset = dataset[Math.random() > 0.2 ? 'train' : 'val'];

    if (subset.x == null) {
      // Create new tensors
      subset.x = tf.keep(image);
      subset.y = tf.keep(mousePos);
    } else {
      // Concatenate it to existing tensors
      const oldX = subset.x;
      const oldY = subset.y;

      subset.x = tf.keep(oldX.concat(image, ));
      subset.y = tf.keep(oldY.concat(mousePos, ));
    }

    // Increase counter
    subset.n += ;
  });
}
```


最后，把**空格键**关联进来：


```python
$('body').keyup(function(event) {
  // On space key:
  if (event.keyCode == ) {
    captureExample();

    event.preventDefault();
    return false;
  }
});
```


至此，只要你按下空格，数据集里就会增加一个数据点了。

## **· 训练模型**

就搭个最简单的**CNN**吧。
![](https://pic3.zhimg.com/v2-3fb72156eb00a71dec7e8af8e5b68886_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='285'></svg>)
TensorFlow.js里面有一个和Keras很相似的**API**可以用。

这个网络里，要有一个卷积层，一个最大池化，还要有个密集层，带两个输出值 (坐标) 的那种。

中间，加了一个dropout作为正则化器；还有，用flatten把2D数据降成1D。训练用的是Adam优化器。

模型代码长这样：


```python
let currentModel;
 
 function createModel() {
   const model = tf.sequential();
 
   model.add(tf.layers.conv2d({
     kernelSize: ,
     filters: ,
     strides: ,
    activation: 'relu',
    inputShape: [$('#eyes').height(), $('#eyes').width(), ],
  }));

  model.add(tf.layers.maxPooling2d({
    poolSize: [, ],
    strides: [, ],
  }));

  model.add(tf.layers.flatten());

  model.add(tf.layers.dropout(0.2));

  // Two output values x and y
  model.add(tf.layers.dense({
    units: ,
    activation: 'tanh',
  }));

  // Use ADAM optimizer with learning rate of 0.0005 and MSE loss
  model.compile({
    optimizer: tf.train.adam(0.0005),
    loss: 'meanSquaredError',
  });

  return model;
}
```


训练开始之前，要先设置一个**固定**的epoch数，再把批尺寸设成**变量** (因为数据集很小) ：


```python
function fitModel() {
   let batchSize = Math.floor(dataset.train.n * 0.1);
   if (batchSize < ) {
     batchSize = ;
   } else if (batchSize > ) {
     batchSize = ;
   }
 
   if (currentModel == null) {
    currentModel = createModel();
  }

  currentModel.fit(dataset.train.x, dataset.train.y, {
    batchSize: batchSize,
    epochs: ,
    shuffle: true,
    validationData: [dataset.val.x, dataset.val.y],
  });
}
```


然后，在页面上做个**训练按钮**吧：


```python
<button id="train">Train!</button>
 <style>
     #train {
         position: absolute;
         top: %;
         left: %;
         transform: translate(-%, -%);
         font-size: pt;
     }
</style>
```


还有JS：


```python
<button id="train">Train!</button>
 <style>
     #train {
         position: absolute;
         top: %;
         left: %;
         transform: translate(-%, -%);
         font-size: pt;
     }
</style>
```


## **· 拉出来遛遛**

**绿色圈圈**终于来了。AI判断你在看哪，它就出现在哪。

先写绿圈圈：


```python
<div id="target"></div>
 <style>
     #target {
         background-color: lightgreen;
         position: absolute;
         border-radius: %;
         height: px;
         width: px;
         transition: all 0.1s ease;
        box-shadow:   px px white;
        border: px solid rgba(,,,0.5);
    }
</style>
```


然后，想让绿圈圈动起来，就要**定期**把眼睛图像传给神经网络。问它你在看哪，它就回答一个坐标：


```python
function moveTarget() {
   if (currentModel == null) {
     return;
   }
   tf.tidy(function() {
     const image = getImage();
     const prediction = currentModel.predict(image);
 
     // Convert normalized position back to screen position:
    const targetWidth = $('#target').outerWidth();
    const targetHeight = $('#target').outerHeight();
    const x = (prediction.get(, ) + ) /  * ($(window).width() - targetWidth);
    const y = (prediction.get(, ) + ) /  * ($(window).height() - targetHeight);

    // Move target there:
    const $target = $('#target');
    $target.css('left', x + 'px');
    $target.css('top', y + 'px');
  });
}

setInterval(moveTarget, );
```


间隔设的是**100毫秒**，不过也可以改的。

总之，大功告成。
![](https://pic1.zhimg.com/v2-5a5876927f7d512ddea8e8af2cce7b94_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='225' height='150'></svg>)
## **鼻孔眼睛分不清？**

眼球追踪模型很有意思，不过还是有一些可爱的缺陷。

比如，算法还只能识别正面，脸稍微**侧一点**AI就会困惑。

比如，有时候会把**鼻孔**识别成眼睛。

比如，必须整张脸都出现在画面里，才能识别眼睛的所在，**捂住嘴**也不行。
![](https://pic4.zhimg.com/v2-6b0ba67f197dde5168736f718a5a2317_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='243' height='162'></svg>)△ 来自怪异君
Max也说，还有很多可以探索的空间。


自己训练传送门：
[https://cpury.github.io/lookie-lookie/](https://link.zhihu.com/?target=https%3A//cpury.github.io/lookie-lookie/)

代码实现传送门：
[https://github.com/cpury/lookie-lookie](https://link.zhihu.com/?target=https%3A//github.com/cpury/lookie-lookie)

教程原文传送门：
[https://cpury.github.io/learning-where-you-are-looking-at/](https://link.zhihu.com/?target=https%3A//cpury.github.io/learning-where-you-are-looking-at/)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


