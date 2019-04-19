# 基于HTML5 Canvas 实现地铁站监控 - xhload3d - 博客园
伴随国内经济的高速发展，人们对安全的要求越来越高。为了防止下列情况的发生，您需要考虑安装安防系统： 提供证据与线索：很多工厂银行发生偷盗或者事故相关机关可以根据录像信息侦破案件，这个是非常重要的一个线索。还有一些纠纷或事故，也可以通过录像很容易找出相关人员的责任。 人防成本高：现在很多地方想到安全就想到要雇佣保安，每个保安每个月 800，每天 3 班倒，一班人员一年就需要将近 4 万元，相比于电子安防设备成本并不便宜，而且使用电子安防设备几年内就不太需要更换。所以人防成本相对也很高。人防辅助：多数情况下，完全靠人来保证安全是一件很困难的事情，很多事情需要电子保安器材（如监视器、报警器）辅助才更完美。特殊场合必须使用：在一些恶劣条件下（高热、寒冷、封闭等），人很难用肉眼观察清楚，或者环境根本不适合人的停留，必须使用电子安防设备。隐蔽性：使用电子安防设备，一般人不会感觉时时被监控，具有隐蔽性。24 小时安全保证：要达到 24 小时不间断的安全需要，电子设备是必须考虑的。远程监控：随着计算机技术与网络技术的发展，远程监控观看异地图象已经成为可能，现在已经有很多公司的负责人已经可以 INTERNET 及时观看世界各地的任何分公司情况，有利于及时了解情况。图象保存：数字录像技术的发展，使得影象可以通过计算机数字存储设备得以保存，可以保存时间更长，图象更清晰。生产管理：管理人员可以及时、直观的了解生产第一线的情况，便于指挥与管理。
鉴于监控系统在国内的需求量较大，对于大范围的监控，如：地铁站，更是需要监控系统来防止意外的发生，我们今天来给大家介绍一下如何创建一个地铁站监控系统的前端部分。
[http://www.hightopo.com/demo/metro/demo2.html](http://www.hightopo.com/demo/metro/demo2.html) 进入页面右键“审查元素”可查看例子源代码。
本例的动态效果如下：
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171119110705265-1516373653.gif)
 我们先来搭建基础场景，在 HT 中，非常常用的一种方法来将外部场景导入到内部就是靠解析 JSON 文件，用 JSON 文件来搭建场景的好处之一就是可以循环利用，我们今天的场景就是利用 JSON 画出来的。接下来 HT 将利用  ht.Default.xhrLoad 函数载入 JSON 场景，并用 HT 封装的 DataModel.deserialize(json) 来[反序列化](http://hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html#ref_serialization)，并将反序列化的对象加入 DataModel：
```
ht.Default.xhrLoad('demo2.json', function(text) {
    var json = ht.Default.parse(text);
    if(json.title) document.title = json.title;//将 JSON 文件中的 titile 赋给全局变量 titile 
    dataModel.deserialize(json);//反序列化
    graphView.fitContent(true);//缩放平移拓扑以展示所有图元，即让所有的元素都显示出来
});
```
在 HT 中，Data 类型对象构造时内部会自动被赋予一个 id 属性，可通过 data.getId() 和 data.setId(id) 获取和设置，Data 对象添加到 DataModel 之后不允许修改 id 值，可通过 dataModel.getDataById(id) 快速查找 Data 对象。一般建议 id 属性由 HT 自动分配，用户业务意义的唯一标示可存在 tag 属性上，通过 Data#setTag(tag) 函数允许任意动态改变 tag 值，通过DataModel#getDataByTag(tag) 可查找到对应的 Data 对象，并支持通过 DataModel#removeDataByTag(tag) 删除 Data 对象。我们这边通过在 JSON 中设置 Data 对象的 tag 属性，在代码中通过 dataModel.getDataByTag(tag) 函数来获取该 Data 对象：
```
var fan1 = dataModel.getDataByTag('fan1');
var fan2 = dataModel.getDataByTag('fan2');
var camera1 = dataModel.getDataByTag('camera1');
var camera2 = dataModel.getDataByTag('camera2');
var camera3 = dataModel.getDataByTag('camera3');
var redAlarm = dataModel.getDataByTag('redAlarm');
var yellowAlarm = dataModel.getDataByTag('yellowAlarm');
```
我在下图中做了各标签对应的元素：
![](https://images2017.cnblogs.com/blog/591709/201711/591709-20171119120928327-1733373857.png)
接着我们对需要旋转、闪烁的对象进行设置，HT 中对“旋转”封装了 setRotation(rotation) 函数，通过获得对象当前的旋转角度，在这个角度的基础上再增加某个弧度，通过 setInterval 定时调用，这样就能在一定的时间间隔内旋转相同的弧度：
```
setInterval(function(){
    var time = new Date().getTime();
    var deltaTime = time - lastTime;
    var deltaRotation = deltaTime * Math.PI / 180 * 0.1;
    lastTime = time;
    fan1.setRotation(fan1.getRotation() + deltaRotation*3);
    fan2.setRotation(fan2.getRotation() + deltaRotation*3);
    camera1.setRotation(camera1.getRotation() + deltaRotation/3);
    camera2.setRotation(camera2.getRotation() + deltaRotation/3);
    camera3.setRotation(camera3.getRotation() + deltaRotation/3);
    if (time - stairTime > 500) {
        stairIndex--;
        if (stairIndex < 0) {
            stairIndex = 8;
        }
        stairTime = time;
    }
    for (var i = 0; i < 8; i++) {//因为有一些相似的元素我们设置的 tag 名类似，只是在后面换成了1、2、3，所以我们通过 for 循环来获取
        var color = stairIndex === i ? '#F6A623' : '#CFCFCF';
        dataModel.getDataByTag('stair_1_' + i).s('shape.border.color', color);
        dataModel.getDataByTag('stair_2_' + i).s('shape.border.color', color);
    }
    if (new Date().getSeconds() % 2 === 1) {
        yellowAlarm.s('shape.background', null);
        redAlarm.s('shape.background', null);
    }
    else {
        yellowAlarm.s('shape.background', 'yellow');
        redAlarm.s('shape.background', 'red');
    }
}, 5);
```
HT 还封装了 setStyle 函数用来设置样式，可简写为 s，具体样式请参考 [HT for Web 样式手册](http://hightopo.com/guide/guide/core/theme/ht-theme-guide.html)：
```
for (var i = 0; i < 8; i++) {//因为有一些相似的元素我们设置的 tag 名类似，只是在后面换成了1、2、3，所以我们通过 for 循环来获取
    var color = stairIndex === i ? '#F6A623' : '#CFCFCF';
    dataModel.getDataByTag('stair_1_' + i).s('shape.border.color', color);
    dataModel.getDataByTag('stair_2_' + i).s('shape.border.color', color);
}
```
我们还对“警告灯”的闪烁进行了定时控制，如果是偶数秒的时候，就将灯的背景颜色设置为“无色”，否则，如果是 yellowAlarm 则设置为“黄色”，如果是 redAlarm 则设置为“红色”：
```
if (new Date().getSeconds() % 2 === 1) {
    yellowAlarm.s('shape.background', null);
    redAlarm.s('shape.background', null);
}
else {
    yellowAlarm.s('shape.background', 'yellow');
    redAlarm.s('shape.background', 'red');
}
```
整个例子就这么轻松地解决了，简直太轻松了。。。
有兴趣继续了解的小伙伴可以进入 [HT for Web 官网](http://hightopo.com/)查看各个手册进行学习。
