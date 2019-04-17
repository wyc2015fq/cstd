# 三行代码实时追踪你的手，有浏览器就够了 | Demo·代码 - 知乎
# 



> 栗子 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-c9566ad01414a88226528c3fd4d35399_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='333'></svg>)
> “Are You OK？”
“O！K！”

人脸不管做了多么一言难尽的表情，五官也不太会四处乱跑。

手就不一样了，手势百媚千娇，镜头看到的画面就百媚千娇。

所以，AI怎么识别手呢？
![](https://pic1.zhimg.com/v2-ec2284893f2e22733cbe33d34186de94_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='363' height='241'></svg>)
一位叫做Victor Dibia的程序猿说，“三行代码”就能实时追踪你的手，只要有个**浏览器**。

你还可以用手在屏幕上涂鸦，成为实至名归的灵魂画“手”：
![](https://pic4.zhimg.com/v2-3d9dcc2826fa3853f97985c786f6a6eb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='455' height='252'></svg>)
Victor说，只要召唤出他的**Handtrack.js**库，模型便可以在网页上跑起来。

他还提供了线上Demo给大家玩耍。

[Hackernoon](https://link.zhihu.com/?target=https%3A//hackernoon.com/handtrackjs-677c29c1d585)的小伙伴们纷纷鼓了个掌：
![](https://pic1.zhimg.com/v2-8fe6920a4a82691fc8378ddf2fc27690_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='198' height='76'></svg>)
## **十魔乱舞**

进了Demo，按下**开始检测**的按钮，打开摄像头，就可以放飞自我了。

在下的手不甚优美，但还是忍不住乱舞了好一阵子。
![](https://pic1.zhimg.com/v2-d1a645481201042412e93ca12802b5ac_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='319'></svg>)
手指自由挥舞，边框就敏捷地跟随。

除了随性活动手指之外，还尝试了一下狼人夜间的战术交流：
![](https://pic3.zhimg.com/v2-8a5788ef305ef42f662baf66bc37f2c2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='420' height='317'></svg>)
翻译一下：你，起跳预言家，给8号发个查杀。

(作为一个菜鸡玩家，通常夜里是轮不到我分配工作的，此处纯属借机过瘾。)

书归正传，手势频繁变换，AI依然**紧追不舍**；

除了打叉叉的时候，只识别了一只手。也许是因为手的侧面朝着屏幕，面积小，不容易识别吧。

如果你伸了两只手，AI却画了三个边框，可以调整一下左边的滑动条：
![](https://pic1.zhimg.com/v2-3753f3a92be9e4697c274519e5026284_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='822' height='122'></svg>)
这是置信阈值 (Confidence Threshold) ，默认0.7，分值高于0.7就会认定是手，显示出边框；如果调高到0.8，那些得分0.7的疑似人手，就不会显示边框了。

除此之外，Demo也有**涂鸦**功能：
![](https://pic3.zhimg.com/v2-0ab78f67b5af822abd58eef03650b8aa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='603'></svg>)
鉴于在下绘画功力有限，略去动态展示，大家可以自己去画一波。

## **模型如何食用？**

Handtrack.js库，是程序猿用TensorFlow.js搭起来的。[源码在此](https://link.zhihu.com/?target=https%3A//github.com/victordibia/handtrack.js)。

里面的模型是用**Egohands**数据集养成，4800张人手的图片，背景有室内也有户外。
![](https://pic2.zhimg.com/v2-cd1be7038004ee35f582921dfb97fea5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='560' height='277'></svg>)
有了这个模型，就不用自己训练了。直接拿来用，方法有两种：

**第一种方法**是在脚本标签 (Script Tag) 里，填上它的URL：


```python
<script src="https://cdn.jsdelivr.net/npm/handtrackjs/dist/handtrack.min.js"> </script>
```


把脚本标签添加到html页面之后，就可以用handTrack变量来引出这个库了：


```python
const img = document.getElementById('img');  
 handTrack.load().then(model => { 
     model.detect(img).then(predictions => {
       console.log('Predictions: ', predictions) // bbox predictions
     });
 });
```


上面这几句代码，是给手加了边框。只要提交每一帧视频，就可以追踪里面的手了，不论是摄像头实时拍摄的，还是已经拍好的视频。

**第二种方法**，是把这个库当做一个NPM包来安装：


```python
npm install --save handtrackjs
```


然后，从NPM导入就可以了。

Victor说，大家可以用这个方法，来开发自己喜欢的应用。比如，做个游戏：
![](https://pic4.zhimg.com/v2-feaf436f5d7570206c878a6efd0462e7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='268'></svg>)
## **One More Thing**

这个模型是训练好了。

但Victor说，也不是不让大家训练。

还说，想自己养成一只实时手部追踪模型的同学们，可以参照这个项目 (600星) ：
[https://github.com/victordibia/handtracking](https://link.zhihu.com/?target=https%3A//github.com/victordibia/handtracking)

最后，送你Handtrack.js库的源代码：
[https://github.com/victordibia/handtrack.js](https://link.zhihu.com/?target=https%3A//github.com/victordibia/handtrack.js)

线上Demo：
[https://victordibia.github.io/handtrack.js/](https://link.zhihu.com/?target=https%3A//victordibia.github.io/handtrack.js/)

Egohands数据集：
[http://vision.soic.indiana.edu/projects/egohands/](https://link.zhihu.com/?target=http%3A//vision.soic.indiana.edu/projects/egohands/)

教程原文：

[https://hackernoon.com/handtrackjs-677c29c1d585](https://link.zhihu.com/?target=https%3A//hackernoon.com/handtrackjs-677c29c1d585)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤




