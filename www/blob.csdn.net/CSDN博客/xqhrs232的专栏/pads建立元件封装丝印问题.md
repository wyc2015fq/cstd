# pads建立元件封装丝印问题 - xqhrs232的专栏 - CSDN博客
2014年09月19日 23:38:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4108
原文地址::[http://www.eda-china.com/index.php?a=show&id=1472&m=Article](http://www.eda-china.com/index.php?a=show&id=1472&m=Article)
相关文章
1、PADS软件里元器件的丝印怎么画----[http://zhidao.baidu.com/link?url=G1QpynVKpqGePSespF2DKW4L6QNpU4xoJI8WRPKo7XktPTJuM_CWRS-95zE_7T5d4u64Ef5Zl47wxIHVfHBqNq](http://zhidao.baidu.com/link?url=G1QpynVKpqGePSespF2DKW4L6QNpU4xoJI8WRPKo7XktPTJuM_CWRS-95zE_7T5d4u64Ef5Zl47wxIHVfHBqNq)
2、PADS各层的意义 
 ----[http://keendawn.blog.163.com/blog/static/88880743201201494447373/](http://keendawn.blog.163.com/blog/static/88880743201201494447373/)
关于pads封装的丝印问题，详细说明（自己总结一下）：
       封装外框丝印最合理放置位置是《all layers》层，这样在关掉丝印层的同时，元件的丝印外框还能显示，利于走线；
       注意：只要2D线放在all layers 层，就自动识别为丝印，不必担心影响走线问题。
//==========================================================================================
PADS各层的意义、用途如下：
TOP 顶层 - 用来走线和摆元器件，SMD的PCB元件及走线一般都放置在顶层。
BOTTOM 底层 - 用来走线和摆元器件
LAYER-3至LAYER-20 一般层，不是电气层，可以用来扩展层电气层，也可以用来做一些标示，比如出gerber做阻抗线的指示
21solder mask top 阻焊层 （顶层露铜层,就是没有绿油覆盖）
22paste mask bottom 底层钢网（底层锡膏层）
23paste mask top 顶层钢网（顶层锡膏层）
24drill drawing 孔位层（钻孔层）
26silkscreen top 顶层丝印，丝印就是在电路板上面印标示的数据
27assembly drawing top顶层装配图
28solder mask bottom底层露铜（底层阻焊层）
29silksceen bottom底层丝印
30assembly drawing bottom底层装配图
LAYER-25 是插装的器件才有的，只是在出负片的时候才有用，一般只有当电源层定义为CAM Plane的时候geber文件才会出负片（split/Mixe也是出的正片），如果不加这一层，在出负片的时候这一层的管脚容易短路。　第25层包含了地电信息，主要指电层的焊盘要比正常的焊盘大20mil 左右的安全距离，保证金属化过孔之后，不会有信号与地电相连。这就需要每个焊盘都包含有第25层的信息
