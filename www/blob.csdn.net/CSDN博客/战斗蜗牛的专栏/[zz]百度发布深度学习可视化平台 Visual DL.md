# [zz]百度发布深度学习可视化平台 Visual DL - 战斗蜗牛的专栏 - CSDN博客





2019年03月16日 14:31:53[vbskj](https://me.csdn.net/vbskj)阅读数：86








版权声明：本文为EnweiTech原创文章，未经博主允许不得转载。    https://blog.csdn.net/English0523/article/details/79087274

恩威AI 科技评论消息，1 月 16 日，百度 ECharts 团队发布旗下知名开源产品 ECharts 的最新 4.0 版本，并宣布品牌升级为「百度数据可视化实验室」（http://vis.baidu.com/）。除了这两大消息外，团队还正式发布深度学习可视化平台 Visual DL，以及其他一系列重量级产品，包括 ECharts GL 1.0 正式版，ZRender 4.0 全新版本，WebGL 框架 ClayGL 等。

百度数据可视化实验室的产品矩阵如下图所示，内容涵盖基础库、各种可视化产品以及应用产品。

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5dadbaa488b.png?imageMogr2/format/jpg/quality/90)

在官网中，百度数据可视化实验室分享了其发展和规划：

> - 基于基础的可视化规范，依托 ZRender、ClayGL 基础库，实现了强大的 ECharts、ECharts GL。
- 以 ECharts 为核心，整合 MapV 等完善的数据可视化产品，并针对各类场景、各种需求开发了图说、Gallery、插件等完善的应用产品。
- 从基础到框架到上层应用，可扎实、强大、便捷的满足一切数据可视化需求。


据雷锋网 AI 科技评论了解，ECharts 为使用 JavaScript 实现的开源可视化库，可以流畅的运行在 PC 和移动设备上，兼容当前绝大部分浏览器，底层依赖轻量级的矢量图形库 ZRender，提供直观，交互丰富，可高度个性化定制的数据可视化图表。可以看到，ECharts 团队从单一的技术产品研发团队一举进化为推进可视化工作的联盟。

百度 ECharts 团队高级技术经理祖明在知乎专栏中也介绍道，

新成立的百度数据可视化实验室，希望能够依托完善的产品支撑及丰富的经验，致力于提供更强大的数据可视化产品、参与数据可视化规范的制定、积极参与可视化基础知识的推广、传播数据可视化学术界及业界的研究成果及解决方案，和广大伙伴一同推动数据可视化行业的发展。

除了宣布品牌升级，此次涉及到的多个产品更新如下：

全新 ECharts 4.0

从 ECharts 官网可以看到，升级为 4.0 版本之后，ECharts 的特性变得更加丰富：

> - 丰富的可视化类型
- 多种数据格式无需转换直接使用
- 千万数据的前端展现
- 移动端优化
- 多渲染方案，跨平台使用
- 深度的交互式数据探索
- 多维数据的支持以及丰富的视觉编码手段
- 动态数据
- 绚丽的特效
- 通过 GL 实现更多更强大绚丽的三维可视化
- 无障碍访问


具体细节，大家可以前往官网详细了解：http://echarts.baidu.com/

深度学习可视化平台 Visual DL

在此次的更新中，深度学习可视化平台 Visual DL （https://github.com/PaddlePaddle/VisualDL）为 ECharts 团队与百度 PaddlePaddle 共同打造。据悉，ECharts 团队自 2017 年起就与 PaddlePaddle 团队进行了深度合作。

据 ECharts 团队介绍，他们希望通过可视化的方法将模型训练过程中的各个参数以及计算的数据流图实时地展现出来，以帮助模型训练者更好地理解、调试、优化模型。

> 
用户只需要通过 Visual DL 提供的接口将模型相关的各种参数数据写入日志中，然后 Visual DL 会读取日志中的模型相关数据将其展示出来，这些数据包括模型训练过程中的各种定量的度量、用户传入的或者中间训练过程生成的各种图片、以及神经网络训练过程中的计算数据流图。


从该项目的 GitHub 上可以看到部分可视化案例：

下图是对参数分布和张量进行可视化：

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5db90893479.gif)

下图是对张量和中间生成的图像进行可视化：

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5db90ae536d.gif)

下图是展示训练过程中错误的出现趋势：

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5db90c491f4.gif)

ECharts GL 1.0正式版

ECharts GL 是强大的高性能 WebGL 可视化解决方案，能满足大屏、VR 及 AR 高质量的展示需求。据雷锋网(公众号：雷锋网) AI 科技评论了解，在此次的正式版中，修复了之前大量的稳定性问题，完善了文档。新增功能有如下四点：

集成 Mapbox 地理可视化组件：

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5dca6db2bb0.png?imageMogr2/format/jpg/quality/90)

支持洋流风场等向量场的可视化：

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5dca76bdce2.png?imageMogr2/format/jpg/quality/90)

新增建筑群可视化的组件：

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5dca71a3c46.png?imageMogr2/format/jpg/quality/90)

更多的二维加速组件：

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5dca7449f4c.png?imageMogr2/format/jpg/quality/90)

该项目 GitHub 地址：https://github.com/ecomfe/echarts-gl

ZRender 4.0 版本升级

ZRender 是二维绘图引擎，它提供 Canvas、SVG、VML 等多种渲染方式。ZRender 也是 ECharts 的渲染器。

据祖明在其知乎专栏介绍，

作为 ECharts 的底层渲染库，ZRender 承担起了抹平平台差异、封装技术细节、提高渲染性能等责任。在此次发布的 4.0 版本中，SVG 渲染的支持使得 ZRender 的渲染能力有了进一步提升。

他也表示，ZRender 以基本图形元素为核心，提供了矩形、圆形、扇形、多边形、折线段、贝赛尔曲线等十多种基本图形，并支持图形直接的组合与剔除，最终以 Canvas、SVG 或VML 的形式输出，并且支持交互处理。

下图为用 ZRender 绘制的一个实例：

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5dcbf170e0e.png?imageMogr2/format/jpg/quality/90)

下面的例子创建了一个圆心在 [150, 50] 位置，半径为 40 像素的圆，并将其添加到画布中：

> 
var circle = new zrender.Circle({

   shape: {

       cx: 150,

       cy: 50,

       r: 40

   },

   style: {

       fill: 'none',

       stroke: '#F00'

   }});zr.add(circle);


该项目 GitHub 地址：https://github.com/ecomfe/zrender

高性能渐进式 WebGL 框架 ClayGL

ClayGL 是一个高性能 WebGL 框架，能够帮助用户快速搭建可视化、三维模型展示、交互广告等 Web3D 的应用，提供 Canvas、SVG、VML 等多种渲染方式。

据 ECharts 团队介绍，ClayGL 作为 ECharts GL 的底层 WebGL 支持框架，2017 年已经在手机百度中成功落地了三维模型展示的应用，充分验证了产品的可靠性。

下图为在 DOTA 2 中的案例：

![ç¾åº¦æ°æ®å¯è§åå®éªå®¤æ­£å¼æç«ï¼åå¸æ·±åº¦å­¦ä¹ å¯è§åå¹³å° Visual DL](https://static.leiphone.com/uploads/new/article/740_740/201801/5a5dcdbee39f1.jpg?imageMogr2/format/jpg/quality/90)



该项目的 GitHub 地址如下：https://github.com/pissang/claygl

总结

百度数据可视化实验室在产品矩阵研发上，从底层到应用实现全面覆盖， 如基础的 ZRender、ClayGL，以及 ECharts、ECharts GL 解决方案，另外还有一系列数据可视化产品如图说、Gallery 等。

而在产业领域合作上，该实验室牵手 PaddlePaddle 团队，推出 Visual DL 深度学习可视化工具，对于深度学习的发展具有极大的促进作用。深度学习可视化工具对于用户理解、调试和共享模型的结构有重大意义，能帮助大家更好地优化和调节网络。

在学术交流合作和教育学习培训上，百度数据可视化实验室还表示，依托于完善的产品支撑及丰富的经验，本持开放、赋能的核心价值观，他们会鼎力参与到可视化基础知识的推广、传播数据可视化行业经验等事情上，并积极参与高校的分享和交流活动。

目前他们在北京大学、清华大学、浙江大学等高校都有过相关交流与合作，也在积极推动教育培训，如打造 ECharts & WebVR 学院、在知乎建立可视化专栏「漂亮得不像实力派的可视化」。

相信该实验室未来会在打造可视化解决方案、加深产业领域合作、促进学界和业界的交流、推动教育培训上给我们带来更多惊喜。

--------------------- 

作者：天府云创 

来源：CSDN 

原文：https://blog.csdn.net/enweitech/article/details/79087274 

版权声明：本文为博主原创文章，转载请附上博文链接！



