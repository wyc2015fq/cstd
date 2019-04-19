# 快速开发基于 HTML5 网络拓扑图应用 - xhload3d - 博客园
采用 [HT](http://www.hightopo.com/) 开发网络拓扑图非常容易，例如《[入门手册](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html)》的第一个小例子麻雀虽小五脏俱全：[http://www.hightopo.com/guide/guide/core/beginners/examples/example_overview.html](http://www.hightopo.com/guide/guide/core/beginners/examples/example_overview.html)
![](https://images2015.cnblogs.com/blog/591709/201609/591709-20160927093353375-153926677.png)
该例子展示了如何构建两个节点、一条连线、以及一个 Group 的组合效果。结合 《[数百个 HTML5 例子学习 HT 图形组件 – 拓扑图篇](http://www.hightopo.com/blog/461.html)》可以容易理解构建一个拓扑界面基本上就是操作 [DataModel](http://www.hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html) 数据模型，以下为构建上图界面效果的模型代码部分：
```
// init data model
hello = new ht.Node();
hello.setPosition(60, 140);             
hello.setName('Hello');
hello.setStyle('note', 'I love HT');
hello.setStyle('note.background', '#FFA000');
dataModel.add(hello);
world = new ht.Node();
world.setPosition(260, 80);
world.setName('World');
world.setStyle('note', 'HT for your imagination');
world.setStyle('note.expanded', false);  
world.setStyle('border.color', 'red');                
dataModel.add(world);
edge = new ht.Edge(hello, world);
edge.setName('Hello World\nwww.hightopo.com');
edge.setStyle('label.color', 'white');
edge.setStyle('label.background', '#3498DB');                
dataModel.add(edge);  
group = new ht.Group();
group.setName('HT for Web ' + ht.Default.getVersion());
group.addChild(hello);
group.addChild(world);
group.addChild(edge);
dataModel.add(group);
```
当然真实的系统不会像上述代码这样写死内容，一般用户通过后台数据查询，然后再根据后台数据动态创建拓扑节点、连线、组、子网等图元内容，并填充如图元名字、连线颜色、告警内容等属性信息，而因为 [HT](http://www.hightopo.com/) 技术基于 HTML5，因此大部分工业控制 Web SCADA 客户都采用了 WebSocket 的实时通讯方式，关于 WebSocket 的使用可参考这篇文章：[3D拓扑自动布局之Node.js篇](http://www.hightopo.com/blog/84.html)
构建模型就是上面这样简单，剩下工作就是设置相应的图元属性达到信息展示和美观的效果，关于图元甚至整体 HT 图形组件风格的自定义，可参考《[HT for Web 风格手册](http://www.hightopo.com/guide/guide/core/theme/ht-theme-guide.html)》，这里我举个简单的小细节，可人有人留意到上面设置 label 的代码有点特别：
```
edge.setName('Hello World\nwww.hightopo.com');
```
这里 \n 顾名思义就是换行的意思，当然除了换行外，还可以有垂直布局等等花哨的展示形态，可参考《[网络拓扑图上文本的巧妙应用](http://www.cnblogs.com/xhload3d/p/5908848.html)》一文的介绍：
![](https://images2015.cnblogs.com/blog/591709/201609/591709-20160927100038188-24932462.png)
```
var list = [], node;
for (var i = 0; i < 4; i++) {
    node = new ht.Node();
    node.setImage('station');
    node.p(100 + i * 100, 100);
    dm.add(node);
    list.push(node);
}
node = list[0];
node.s({
    'label': '厦门',
    'label.font': '22px arial, sans-serif',
    'label2': 'Xiamen',
    'label2.position': 31,
    'label2.offset.y': 23
});
node = list[1];
node.s({
    'label': '图\n扑',
    'label.position': 14,
    'label.font': '22px arial, sans-serif',
    'label2': 'Hightopo',
    'label2.position': 14,
    'label2.offset.x': -7,
    'label2.rotation': -Math.PI / 2
});
node = list[2];
node.s({
    'label': '上\n海',
    'label.position': 20,
    'label.font': '22px arial, sans-serif',
    'label2': 'Shanghai',
    'label2.position': 20,
    'label2.offset.x': 6,
    'label2.rotation': -Math.PI / 2
});
node = list[3];
node.s({
    'label': '北京',
    'label.position': 3,
    'label.font': '22px arial, sans-serif',
    'label2': 'Beijing',
    'label2.position': 3,
    'label2.offset.y': -23
});
```
当然也可以达到动态旋转文字的效果，拖拽下面这个红色圆球达到动态旋转文字的效果：[http://www.hightopo.com/guide/guide/core/beginners/examples/example_label.html](http://www.hightopo.com/guide/guide/core/beginners/examples/example_label.html)
![](https://images2015.cnblogs.com/blog/591709/201609/591709-20160927100341531-1332188887.png)
从上面例子可以参考光光一个文字还有设置颜色、背景、字体、最大长度等等参数，如果加上 Position 的布局 3D 的布局那几乎可以基于文字独立写篇文章了，这里就不一而足了，有兴趣的可以玩玩位置手册的各种例子：[http://www.hightopo.com/guide/guide/core/position/ht-position-guide.html](http://www.hightopo.com/guide/guide/core/position/ht-position-guide.html)
![](https://images2015.cnblogs.com/blog/591709/201609/591709-20160927100806938-1842592441.png)
构建完图元连线关系，设置完图元风格属性，接下来主要就是图元摆放问题，对于工业控制领域的图元摆放一般是手工进行，所以 Web SCADA 工业控制领域一般会构建一套针对自己产品的 HMI 人机界面绘图工具，可参考《[基于HT for Web的Web SCADA工控移动应用](http://www.hightopo.com/blog/402.html)》文章：
![](https://images2015.cnblogs.com/blog/591709/201609/591709-20160927101417922-986856958.png)
对于电信网管拓扑应用，由于网络拓扑图元数据量往往常常非常巨大，虽然 HT 拓扑图组件性能非常强劲，承载好几甚至上十万的网络拓扑矢量图元都毫无压力，但如何实现这么多数据量图元的布局是个问题，如果是规规矩矩的自然比较容易，写两个 for 循环就能实现网格布局，可参考 [http://www.hightopo.com/demo/fan/index.html](http://www.hightopo.com/demo/fan/index.html) 这个数千个风机的例子，这种例子一般用于能源行业控制系统，通过采用 HT 的矢量格式，可动态控制风机的转速、颜色等参数，来直观形象的表达该风机的运行状态。
这个例子[http://www.hightopo.com/demo/fan/index.html](http://www.hightopo.com/demo/fan/index.html) 并没有针对手机做特别优化，但我特意用 iOS Safari 来跑，不得不说 Safari 10 还是配得上 HT 的这个例子，苹果还是不断努力在提升 HTML5 在其产品线中的性能，并且 Safari 10 号称已经 100% 支持 ES6 标准了，[https://developer.apple.com/library/prerelease/content/releasenotes/General/WhatsNewInSafari/Articles/Safari_10_0.html](https://developer.apple.com/library/prerelease/content/releasenotes/General/WhatsNewInSafari/Articles/Safari_10_0.html)
The [ECMAScript 2015](http://www.ecma-international.org/ecma-262/6.0/index.html) standard, also known as ES6, is completely supported, bringing this major JavaScript evolution to Safari on macOS and iOS.
![](https://images2015.cnblogs.com/blog/591709/201609/591709-20160927103105953-1413656876.png)
不过更多的电信网管拓扑图应用会采用 HT 的自动布局功能，可参考《[HT for Web 自动布局手册](http://www.hightopo.com/guide/guide/plugin/autolayout/ht-autolayout-guide.html)》和 《[HT for Web 弹力布手册](http://www.hightopo.com/guide/guide/plugin/forcelayout/ht-forcelayout-guide.html)》，利用好自动布局功能可以大大减少项目上线的实施工作量，且电信设备往往需要自动发现动态变化，几乎无法用手工完成这些事情。如果结合华为任正非老大最近的言论 [http://finance.sina.com.cn/chanjing/gsnews/2016-09-26/doc-ifxwevmf2247492.shtml](http://finance.sina.com.cn/chanjing/gsnews/2016-09-26/doc-ifxwevmf2247492.shtml) ，可以想象将来华为的电信网管拓扑，应该利用其独有的行业网络数据，依靠人工智能来提供自动布局算法布局，好吧，我扯远了刹车回主题。
![](https://images2015.cnblogs.com/blog/591709/201609/591709-20160927103416125-358415058.png)
其实自动布局无法提供算法进行图元摆放，以便达到业务展示的需求，可参考《[电信网络拓扑图自动布局 - 曲线布局](http://www.cnblogs.com/xhload3d/p/5883665.html)》和《[电信网络拓扑图自动布局 - 总线布局](http://www.cnblogs.com/xhload3d/p/5840954.html)》，这两篇文章详细分析了如果自定义出总线和沿着任意曲线布局的案例：
![](https://images2015.cnblogs.com/blog/591709/201609/591709-20160927104338610-265119265.png)
以上创建网络图元、设置图元连线关系、配置图元风格属性、进行图元布局摆放就是构建拓扑图的几个基本步骤，其实熟悉了 [HT](http://www.hightopo.com) 分分钟就能开发出像模像样的 HTML5 网络拓扑图应用，如果需要数据存储可参考《[HT for Web 序列化手册](http://www.hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html)》，用户可将整个拓扑图序列化成字符串的 JSON 格式内容，这样你可以保存到后台数据库，或者后台服务器文件皆可，HT 只是前端的图形组件，不介入后台通讯和存储，反正控制权在你，不收任何约束，可以随心所欲的设计你的网络拓扑图整体系统架构。
更多 HTML5 拓扑图例子可参考：[http://www.hightopo.com/demos/index.html](http://www.hightopo.com/demos/index.html)
![](https://images2015.cnblogs.com/blog/591709/201609/591709-20160927105743703-1099614351.png)
