# 基于HTML5 WebGL实现 json工控风机叶轮旋转 - xhload3d - 博客园
突然有个想法，如果能把一些用到不同的知识点放到同一个界面上，并且放到一个盒子里，这样我如果要看什么东西就可以很直接显示出来，而且这个盒子一定要能打开。我用[HT](http://www.hightopo.com/)实现了我的想法，代码一百多行，这么少的代码能实现这种效果我觉得还是牛的。
demo地址： [http://www.hightopo.com/demo/Wall3D/index.html](http://www.hightopo.com/demo/Wall3D/index.html)
先来看看效果图：
![](https://images2017.cnblogs.com/blog/591709/201710/591709-20171026231621320-64141724.gif)
这个例子最基础的就是最外层的盒子了，所以我们先来看看如何实现它：
```
1 var box = new ht.CSGBox();
2 dataModel.add(box);
```
用[HT](http://www.hightopo.com/)可以很轻易地实现这个盒子，在[HT](http://www.hightopo.com/)中封装了很多基础图元类型，我们经常用到的ht.Node也是其中一个，这样我们可以不用反复地写相同的代码来完成基础的实现。
这个例子中用的封装好的基础图元是ht.CSGBox，一个盒子模型，可以参考[HT for Web 建模手册](http://127.0.0.1:9090/ht-for-web-6.1.5/guide/plugin/modeling/ht-modeling-guide.html#ref_csgbox)，我们在手册中可以看到，在CSGBox中我们只能操作这个盒子的各个面，如果你想要自己设置一些特殊的功能，只需要操作ht.Style（[HT for Web 风格手册](http://127.0.0.1:9090/ht-for-web-6.1.5/guide/core/theme/ht-theme-guide.html#ref_style)）即可。[](http://www.hightopo.com/)
要想实现在盒子上的一个面上添加贴图，我能想到的只有[HT](http://www.hightopo.com/)封装的ht.Default.setImage函数了。
我想你们都注意到了盒子上有模型化的水泵，上面有扇叶在旋转，那么这个水泵是怎么生成的呢？我用我们的2d编辑器写了一个水泵模型通过graphView.serialization(datamodel)将模型序列化成json格式的文件，然后在这边引用的时候我再调用graphView.deserialize(json)来将json文件导出成可视化的2d模型并设置animation动画，再立即刷新到这个水泵，否则就算设置了动画，水泵上的扇叶旋转也不会生效。
```
1 ht.Default.xhrLoad('displays/demo/pump.json', function(text){
 2     const json = ht.Default.parse(text);
 3     pumpDM.deserialize(json);
 4     var currentRotation = 0;
 5     var lastTime = new Date().getTime();
 6 
 7     setInterval(function(){
 8         var time = new Date().getTime();
 9         var deltaTime = time - lastTime;
10         currentRotation += deltaTime * Math.PI / 180 * 0.3;
11         lastTime = time;
12 
13         pumpDM.getDataByTag('fan1').setRotation(currentRotation);
14         pumpDM.getDataByTag('fan2').setRotation(currentRotation);
15         box.iv();
16         // g3d.iv();这边也可以刷新g3d，但是局部刷新更省
17         pumpGV.validateImpl();
18     }, 10);
19 }, 10);
```
这个时候我不能把水泵的graphView和g3d都加到底层div上，并且我的意图是把水泵的graphView加到g3d中的CSGBox中的一面上，所以为了让水泵显示出来 必须设置水泵所在的graphView的宽高，而这个宽高必须比我json画出来的图占的面积要大，不然显示不完整。如果想看这个宽高对显示的影响，可以自己改改看来玩玩。
```
1 pumpGV.getWidth = function() { return 600;}
2 pumpGV.getHeight = function(){ return 600;}
3 pumpGV.getCanvas().dynamic = true;//设置这个是为了让canvas能动态显示
```
这边还要特别说明一个函数getDataByTag(tagName)这个函数是获取标识号，在HT中tag属性是唯一标识，虽然HT中也有id，但是id是HT中Data类型对象构造时内部自动被赋予的一个id属性，可以通过data.getId()和data.setId(id)获取和设置，Data对象添加到DataModel之后不允许修改id值，可以通过dataModel.getDataById(id)快速查找Data对象。
一般我们建议id属性由HT自动分配，用户业务意义的唯一标示可存在于tag属性上，通过data.setTag(tag)函数允许任意动态改变tag值，通过dataModel.getDataByTag(tag)可查找到对应的Data对象，并支持通过dataModel.removeDataByTag(tag)删除Data对象。
你可能会好奇在代码中我们并没有提到“fan1”这个tag标识，这个标识是在水泵的json中设置的，关于扇叶的一个标识，我们获取到扇叶，然后设置其旋转。
echarts图表的显示也是很基础的，但是我们会发现，在将echarts图表添加进graphView中它的动画效果会不显示，所以我们首先要将这个echarts图表所在的图表的dynamic设置为true，即将其设置为动态：
```
1 function charts(option){
2     var info = {
3         canvas: document.createElement('canvas')
4     };
5     info.canvas.dynamic = true;//设置info.canvas为动态
6     info.chart = echarts.init(info.canvas);
7     info.chart.setOption(option);
8     return info.canvas;
9 }
```
最后，只需要将这两个回传的canvas传入ht.Default.setImage中即可：
```
1 ht.Default.setImage('echart', charts(option));
2 ht.Default.setImage('pump', pumpGV.getCanvas());
```
ht.Default.drawImage函数生成新的图实际上就是在canvas上画图，所以我们只要把我们已经画好的canvas传到ht.Default.setImage就可以生成图片了。
