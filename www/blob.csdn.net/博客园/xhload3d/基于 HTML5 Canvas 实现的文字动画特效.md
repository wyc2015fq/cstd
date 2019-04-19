# 基于 HTML5 Canvas 实现的文字动画特效 - xhload3d - 博客园
# 前言
文字是网页中最基本的元素，一般我们在网页上都是展示的静态文字，但是就效果来说，还是比较枯燥的。文字淡入淡出的动画效果在项目中非常实用，如果有某些关键的文字，可以通过这种动态的效果来提醒用户阅读。
# 动态效果图
![](https://images2018.cnblogs.com/blog/591709/201803/591709-20180320161217871-1782454002.gif)
[http://www.hightopo.com/demo/GraphAnimation/index.html](http://www.hightopo.com/demo/GraphAnimation/index.html)
这个 Demo 是不断重复地设置文字的大小和透明度，这些英文字母也是我自己利用 HT 的矢量绘制的，总的来说效果还是不错的。
# 代码实现
代码总共一百来行，比较简单，前面说过我的英文字母是用矢量绘制的，用矢量绘制图形的好处非常多，比如图片缩放不会失真，这样在不同的 Retina 显示屏上我也不需要提供不同尺寸的图片；还有就是用 JSON 的方式来描述矢量，格式比较简洁等等。
## 矢量图片
接下来绘制这些矢量图片（这里只拿出第一个 H 进行说明）：
```
{
    "width": 10,//设置矢量的宽
    "height": 10,//设置矢量的高
    "comps": [//矢量图形的组件Array数组，每个数组对象为一个独立的[组件类型](http://hightopo.com/guide/guide/core/vector/ht-vector-guide.html#ref_type)，数组的顺序为组件绘制先后顺序
    {
        "type": "text",//文本类型
        "text": "H",//文本内容
        "color": "rgb(69,69,69)",//文本颜色
        "align": "center",//文本在矢量中的对齐方式
        "opacity": {//文本透明度
            "func": "attr@text.opacity",//设置业务属性，对文本进行透明度的数据绑定
            "value": 1//如果func中的值为空或者undefined，那么就直接用这个值
        },
        "clipDirection": "bottom",//裁切方向为“从上到下”
        "rect": [//指定组件绘制在矢量中的矩形边界
            0,//代表左上角坐标x
            0,//代表左上角坐标y
            10,//代表组件的width
            10//代表组件的height
        ]
    }]
}
```
绘制矢量图形是通过 JSON 格式来书写的，主要的参数有三个：width、height 以及 comps。这三个参数都必须要写上，其中 width 为矢量图形的宽度，height 为矢量图形的高度，comps 里面是一个 Array 数组，数组中是一个个独立的对象，可以对这个对象设置一些预定义参数，也可设置一些可选的参数信息。
这里我将所有的文本信息存储在一个数组中，方便调用：
```
arr = [
    {label: 'H', image: 'symbols/H.json'},
    {label: 'T', image: 'symbols/T.json'},
    {label: 'f', image: 'symbols/f.json'},
    {label: 'o', image: 'symbols/o.json'},
    {label: 'r', image: 'symbols/r.json'},
    {label: 'W', image: 'symbols/W.json'},
    {label: 'e', image: 'symbols/e.json'},
    {label: 'b', image: 'symbols/b.json'},
];
```
第一个 label 对象为矢量图形的名称，第二个 image 对象我们设置的是 json 格式的图片，但是实际上可以为 img、canvas 对象或图片 url 或 base64 字符串。
## 创建节点对象
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180401184745130-242742834.png)
对象的图片已经生成，接下来就是创建对象了，这里英文字母总共 8 个，那么我们创建 8 个节点对象：
```
var s = 80;
  arr.forEach(function(obj, index) {
      var text = obj.label;
      name = 't' + text;
      window[name] = createNode(obj.image, 100+s*index, 100);
  });
function createNode(image, x, y) {//节点对象声明
    var node = new ht.Node();//这个类为 ht 中定义的节点
    node.setSize(0, 0);//设置节点大小
    if(image) node.setImage(image);//设置节点图片
    if(x && y) node.setPosition(x, y);//设置节点摆放位置
    dm.add(node);//将节点添加进数据容器 datamodel 中
    return node;
}
```
关于上面的 ht.Node 节点的生成，其实这个只是 HT 封装好的类，这个类上面有很多很方便的 API。然后将这个生成的节点添加进数据容器 dm 中，这个数据容器又是整个拓扑图 gv 的数据容器。
## 拓扑图生成
来看看如何生成这个拓扑图吧：
```
dm = new ht.DataModel();//数据容器
gv = new ht.graph.GraphView(dm);//拓扑图 通过 gv.getView() 可获得这个拓扑图的底层 div
gv.addToDOM();//将 gv 添加进 body 中
```
实际上 HT 的原理就是在一个 div 中的 canvas 上绘制图形，也就是说这个 gv 就是一个 canvas。
然后通过 getView 获取这个 canvas 的底层 div，这样我们就能将这个 div 添加到 html 页面的任何地方了，addToDOM 的定义如下：
```
addToDOM = function(){   
    var self = this,
        view = self.getView(),   //获取底层div
        style = view.style;
    document.body.appendChild(view);      //将底层div添加到body中      
    style.left = '0';//因为 HT 默认将组件的position设置为absolute 所以要设置位置
    style.right = '0';
    style.top = '0';
    style.bottom = '0';      
    window.addEventListener('resize', function () { self.iv(); }, false);//窗口大小变化触发事件，调用最外层组件invalidate（即iv）函数进行更新。
}
```
现在刷新页面，你会看到一片空白，为什么？因为前面设置节点的大小为 0 啊，怎么会显示，这个 Demo 的效果就是从无到有，又从有到无。那接下来看看如何“从无到有”。
## 文本动画
就像我刚刚说过的，要想让节点显示，肯定是需要设置节点的大小为我们肉眼可视的范围才会出现，但是我的目的不仅是从无到有，也是从小到大，这个能够一气呵成么？感觉好像代码内容简单，但是代码量却不小的一个任务，我定义了一个函数用来将节点从无到有，从小到大：
```
function setSize(node) {
    if(node) {
         var s = 80,
              size = node.getSize().width;//获取节点当前的大小中的宽度，因为我知道宽高都是一样的，所以简写了
        var sw = s - size;
        ht.Default.startAnim({//HT 封装的动画函数，内容也是 JSON 格式的对象
            duration: 1000,// 动画周期毫秒数
            easing: function(t) { return t*t },//动画缓动函数
            action: function(v, t) {//action函数必须提供，实现动画过程中的属性变化 第一个参数v代表通过easing(t)函数运算后的值，t代表当前动画进行的进度[0~1]，一般属性变化根据v参数进行 
                node.setSize(//设置节点的大小  (有一个缓动的过程 通过 sw*v 实现的)
                    size + sw*v, 
                    size + sw*v 
                ); 
            } 
        }); 
    } 
}
```
从大到小，从有到无的过程也跟上面类似，我就不赘述了。
![](https://images2018.cnblogs.com/blog/591709/201803/591709-20180320161217871-1782454002.gif)
要让这些字母按照时间的先后顺序出现和消失，肯定需要用到 setTimeout 方法，要想实现一次的显示消失是非常容易的，但是我在实现的过程掉到了 setTimeout 的一个陷阱中，只能说自己学艺不精吧。因为我们需要给不同的字母设置不同的出现和消失时间，一般比较简单的方法就是设置一个固定的值，然后乘以对应节点专属的 index：
```
function animateIn() {
    for(let i = 0; i < arr.length; i++) {
        var name = 't' + arr[i];
    animateLetterIn(window[name], i);//这个部分设置动画
    }
}
```
可是如果我直接在 for 循环中设置 setTimeout 的时间为动态变化的，那么这个动态变化的值肯定是只取 for 循环的最后一个值，所以我将 setTimeout 的方法抽取出来作为一个单独的函数：
```
function animateLetterIn(node, i) {
    setTimeout(function() {
        setSize(node);
    }, i * 200);//这时候这个 i 取的就是节点对应的 i 而不是最后一个值了
    if(i === arr.length - 1) {//当节点为最后一个节点时，设置节点淡出动画
        setTimeout(function() {
        animateOut();//节点淡出动画
    }, (arr.length + 3) * 200);
    }
}
```
节点淡出动画也是类似的方法，只是需要循环调用这些动画函数，这样才能做到无限循环字母的大小控制。
