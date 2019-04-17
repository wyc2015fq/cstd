# 浏览器上跑：TensorFlow开源了实时人物分割模型，秒速25帧，24个部位 - 知乎
# 



> 分栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-82a075083e67079b8e2e6e8f3d676cc9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='420' height='316'></svg>)
TensorFlow开源了一个**实时**人物分割模型，叫**BodyPix**。

这个模型，在浏览器上用**TensorFlow.js**就能跑。

而且，**帧率**还很可观，在默认设定下：

> 用2018版15吋MacBook Pro跑，每秒**25帧**。
用iPhone X跑，每秒**21帧**。

注意，有线上**Demo**可以玩耍。

## **怎样分割**

在BodyPix眼里，人体可以分成**24个部位**。比如，左手、右前小腿、后背等等。

对每个像素来说，模型首先需要判断，它是不是人体的一部分。
![](https://pic2.zhimg.com/v2-657fc824b1dc090b99b931e39b8646d9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='817'></svg>)
如果判断属于人体，再分辨它到底在人体的哪个部位：
![](https://pic4.zhimg.com/v2-8a78549691ea50f14543d234f57a2fdf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='836'></svg>)
这样，黑色剪影就变成了色彩丰富的分割成果。

## **开始食用**

现在，来仔细看一下，这个模型该怎么用。

首先明确，如果不和其他模型搭配食用，BodyPix只适用于**单人影像**。

## **第一部分：导入**

用npm install @tensorflow-models/body-pix安装，然后用es6模块来导入：


```python
import * as bodyPix from '@tensorflow-models/body-pix'; 

 async function loadAndUseBodyPix() {
    const net = await bodyPix.load();
    // BodyPix model loaded
 }
```


或者，通过页面上的bundle来食用，就不用安装了：


```python
<html> 
      <body> 
          <!-- Load TensorFlow.js --> 
          <script src="https://cdn.jsdelivr.net/npm/@tensorflow/tfjs@0.13.3"></script> 
          <!-- Load BodyPix -->
          <script src="https://cdn.jsdelivr.net/npm/@tensorflow-models/body-pix"></script> 
              bodypix.load().then(function(net) {
                  // BodyPix model loaded
              });
         </script>
     </body>
 </html>
```


## **第二部分A：把人抠出来**

这里，只是把像素分成人和非人。
![](https://pic2.zhimg.com/v2-df8a0289af6ae00a6954696fc4125521_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='420'></svg>)
每个像素，都会给出一个**0-1**之间的数值。

然后，设定一个阈值 (如0.5) 。数值大于等于阈值视为**1**，小于视为**0**。

要召唤出一个API，叫**estimatePersonSegmentation**，来执行这个任务：


```python
const imageElement = document.getElementById('image');

 // load the BodyPix model from a checkpoint
 const net = await bodyPix.load();

 // arguments for estimating person segmentation.
 const outputStride = ;
 const segmentationThreshold = 0.5;
```


## **第二部分B：区分身体部位**
![](https://pic2.zhimg.com/v2-661385e10a94270383abb58d43ceacb9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='360'></svg>)
24个身体部位，对应0-23的数字。非人体则是-1。

这里，要召唤出另一个API，叫**estimatePartSegmentation**：


```python
const imageElement = document.getElementById('image');
 
  // load the BodyPix model from a checkpoint
  const net = await bodyPix.load();
 
  // arguments for estimating body part segmentation.
  const outputStride = ;
  const segmentationThreshold = 0.5;
 
 // load the person segmentation model from a checkpoint
 const net = await bodyPix.load();

 const partSegmentation = await net.estimatePartSegmentation(imageElement, outputStride, segmentationThreshold);
```


更详细的食用步骤，请至文底**博客**传送门。

## **多人也可以？**

BodyPix如果可以和**人物检测器**合并使用，就可以做**多人**分割任务了：

先把每个人用方框圈起来，然后再在每个方框里做分割。

但凭一己之力，BodyPix暂时还搞不定多人。

不过，对于一个能在浏览器上运行的实时模型，单人25帧每秒，已属优秀。

除了开源模型之外，TensorFlow团队还提供了线上**Demo**，打开摄像头就可以玩耍了：
![](https://pic1.zhimg.com/v2-e78daa8e57d9edfb88566e411f595030_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='273'></svg>)△ Demo的帧率没有那么高
Demo传送门：
[https://storage.googleapis.com/tfjs-models/demos/body-pix/index.html](https://link.zhihu.com/?target=https%3A//storage.googleapis.com/tfjs-models/demos/body-pix/index.html)

BodyPix传送门：
[https://github.com/tensorflow/tfjs-models/tree/master/body-pix](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tfjs-models/tree/master/body-pix)

博客传送门：
[https://medium.com/tensorflow/introducing-bodypix-real-time-person-segmentation-in-the-browser-with-tensorflow-js-f1948126c2a0](https://link.zhihu.com/?target=https%3A//medium.com/tensorflow/introducing-bodypix-real-time-person-segmentation-in-the-browser-with-tensorflow-js-f1948126c2a0)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


