# 基于 CSS3 Media Queries 的 HTML5 应用 - weixin_33985507的博客 - CSDN博客
2016年06月21日 16:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
先来介绍下 media，确切的说应该是 CSS media queries（CSS 媒体查询），媒体查询包含了一个媒体类型和至少一个使用如宽度、高度和颜色等媒体属性来限制样式表范围的表达式。CSS3 加入的媒体查询使得无需修改内容便可以使样式应用于某些特定的设备范围。
那么该怎么定义 media 呢，看下面的代码，你肯定能猜出个大概。
```
<!-- link元素中的CSS媒体查询 -->
<link rel="stylesheet" media="(max-width: 800px)" href="example.css" />
<!-- 样式表中的CSS媒体查询 -->
<style>
@media (max-width: 600px) {
  .facet_sidebar {
    display: none;
  }
}
</style>
```
关于解释，文档中是这么说的，当媒体查询为真时，相关的样式表或样式规则就会按照正常的级联规则被应用。即使媒体查询返回假， <link> 标签上带有媒体查询的样式表仍将被下载（只不过不会被应用）。
所以呢，这也是一种弊端，如果说对某个页面定义了多个样式标准来因对不同的 media 属性的话，那在页面的加载时间将会受到影响，但是话有说回来，在当前网络快速发展的时代，网速也在不断地完善和提高，因此影响并不大，几乎可以忽略不计。
media 还可以通过逻辑操作符（and、not、only 等）来组成 media 表达式，书写更复杂的过滤条件，这些表达式我就不再这边一一说明了，想深入了解的同学，可以阅读相关的说明文档：[https://developer.mozilla.org/en-US/docs/Web/CSS/Media_Queries/Using_media_queries](https://developer.mozilla.org/en-US/docs/Web/CSS/Media_Queries/Using_media_queries) 这里面有做详细的介绍。
接下来我们来用几个 Demo 来演示下 media 的用法及表现。
既然我们今天的目的是探讨如何监听 devicePixelRatio 属性的变化，那么我们就以在不同的 devicePixelRatio 值情况下，来改变某个 div 的 background 样式，具体的代码如下：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <title></title>
        <style media="screen">
            @media screen and (min-resolution: 2dppx) {
                #image {
                    background : red;
                }
            }
            @media screen and (min-resolution: 1dppx) {
                #image {
                background: #000;
            }
            }
        </style>
    </head>
    <body>
        <div id="image" style="width:100px; height:100px"></div>
    </body>
</html>
```
代码有了，那么要怎么测试呢？在一般情况下，devicePixelRatio 属相是不会变化的，但是肯定会存在特殊情况的，就比如说，你的电脑接了两个显示器，而且两台浏览器的 devicePixelRatio 属性是不一样的，那么恭喜你，你已经具备测试条件，只需要将页面从一个屏拖到另外一个屏，这样你就可以看到效果了。
有去测试的同学会发现，div 的背景色并没有想代码中设置的那样，在不同的 devicePixelRatio 属性值下，展现出不同的颜色，这是为什么呢？
这代码是我最开始写代码，运行后发现没效果，起初我也不知道原因，在跨屏拖动页面的时候，在浏览器控制台中，我找到了原因。那么到底是什么原因导致设置无效的呢？我们来看看两个屏幕下的 Style 内容截图，左边是 min-resolution 等于 1，右边是等于 2
![](https://images2015.cnblogs.com/blog/591709/201606/591709-20160621014839022-913784671.png)![](https://images2015.cnblogs.com/blog/591709/201606/591709-20160621014847475-1463577990.png)
对比着两个图，可以发现，在 min-resolution 等于 2 的情况下，在里面定义的属性被覆盖掉了，并没有生效，这是为什么呢？
要解释的话，这里恐怕需要补充一点知识，就是关于 min- 和 max- 的前缀，在代码中的所起到的具体效果，在文档中是这么描述的：大多数媒体属性带有 “min-” 和 “max-” 前缀，用于表达 “大于等于” 和 “小于等于”。这避免了使用与HTML和XML冲突的 “<” 和 “>” 字符。如果你未向媒体属性指定一个值，并且该特性的实际值不为零，则该表达式被解析为真。如果浏览器运行的设备上没有该属性值，包含这个属性值的表达式一般返回假。
其实上面的说明已经帮我解释清楚了，我再通俗地和大家解释一下：当 devicePixelRatio 为 1 时，只有 min-resolution: 1dppx 这个条件满足，因此 div 的颜色是黑色没错；当 devicePixelRatio 为 2 时，两个 media 都满足条件，同时 CSS 的规则是后加载的样式将会覆盖先加载的样式，由于我么将 min-resolution: 1dppx 的 media 写在后面，因此如果两个 media 都满足条件的话， min-resolution: 1dppx 的 media 将会覆盖 min-resolution: 2dppx 的 media，因此不管你把页面拖到那个屏幕，那个 div 的背景色都是黑色。
那么我们将两个 media 调换一下位置，问题就顺利地解决了。
```
<style media="screen">
    @media screen and (min-resolution: 1dppx) {
        #image {
            background: #000;
        }
    }
    @media screen and (min-resolution: 2dppx) {
        #image {
            background : red;
        }
    }
</style>
```
以上是根据不同的 media 条件设置不同的样式，这是 CSS 的做法，在 JavaScript 中，没有专门的方法来监听 window.devicePixelRatio 属性变化，那么该怎么监听 devicePixelRatio 属性的变化呢?方法也很简单，看看下面的代码，你一定就明白了：
```
window.matchMedia('screen and (min-resolution: 2dppx)').addListener(function(e) {
    console.info(e, window.devicePixelRatio);
});
```
稍微解释下，通过 window.matchMedia(‘media expression’) 方法获取到对应的 media，然后通过 addListener(function(e) {}) 来监听 media 的变化。
有玩过 Canvas 的朋友一定知道，要想绘制出来的内容效果最佳的话，Canvas 自身的 width 和 height 属性值与 style 中的 width 和 height 的比例应该恰好等于 devicePixelRatio 的值，所有如果你在切换不同 devicePixelRatio 属性值的屏幕时，没有重新设置 Canvas 的宽高的话，绘制出来的画面将不是最佳的效果。
接下来我们基于 [HT for Web](http://www.hightopo.com/) 的 3D 模型来做一个小实验。实验的内容是这样的，在 GraphView 中有一辆车根据某条路线前行，当拖到另外一个屏幕的时候，换辆车子。先来看看效果图：
![](https://images2015.cnblogs.com/blog/591709/201606/591709-20160621015606006-992760480.png)
![](https://images2015.cnblogs.com/blog/591709/201606/591709-20160621015616897-240297018.png)
上面两张图分别是在不同的屏幕中的截图，车子动起来的效果可以访问以下链接：
实验的地址是：[http://www.hightopo.com/demo/media/index.html](http://www.hightopo.com/demo/media/index.html) 以下是实验的具体代码：
```
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <title>HT for Web</title>
        <style media="screen">
            @media screen and (min-resolution: 2dppx) {}
            html, body {
                padding: 0px;
                margin: 0px;
            }
        </style>
        <script src="../../oldhtforweb/lib/core/ht.js"></script>
        <script src="../../oldhtforweb/lib/plugin/ht-modeling.js"></script>
        <script src="../../oldhtforweb/lib/plugin/ht-obj.js"></script>
        <script>
            ht.Default.setImage('road', './images/road.jpg');
            var init = function() {
                g3d = new ht.graph3d.Graph3dView();
                var dm = g3d.dm();
                g3d.addToDOM();
                g3d.setEye(1200, 300, 0);
                g3d.getNote = function(data) {
                    if (data.getTag() !== 'carNode') return null;
                    return 'DevicePixelRatio : ' + window.devicePixelRatio;
                };
                var carIndex = 0;
                window.matchMedia('screen and (min-resolution: 2dppx)').addListener(function() {
                    carIndex = (carIndex + 1) % 2;
                    var obj = result[carIndex];
                    carNode.s('shape3d', obj.name);
                    ht.Default.setDevicePixelRatio();
                });
                var polyline = createPath(dm, 300),
                    params = {
                        delay: 0,
                        duration: 10000,
                        easing: function(t){
                            return (t *= 2) < 1 ? 0.5 * t * t : 0.5 * (1 - (--t) * (t - 2));
                        },
                        action: function(v, t){
                            var length = g3d.getLineLength(polyline);
                            var offset = g3d.getLineOffset(polyline, length * v),
                                point = offset.point,
                                px = point.x,
                                py = point.y,
                                pz = point.z,
                                tangent = offset.tangent,
                                tx = tangent.x,
                                ty = tangent.y,
                                tz = tangent.z;
                            carNode.p3(px, py - 9, pz);
                            carNode.lookAt([px + tx, py + ty - 9, pz + tz], 'front');
                        },
                        finishFunc: function(){
                            ht.Default.startAnim(params);
                        }
                    },
                    carList = [ 'fordFocus', 'concept-sedan-01v2'],
                    result = [], carNode = new ht.Node();
                carNode.setTag('carNode');
                carList.forEach(function(name, index) {
                    ht.Default.loadObj('./objs/'+name+'/'+name+'.obj', './objs/'+name+'/'+name+'.mtl', {
                        cube: true,
                        center: true,
                        shape3d: name,
                        finishFunc: function(modelMap, array, rawS3) {
                            var k = 110 / rawS3[0];
                            rawS3 = rawS3.map(function(v) { return v * k; });
                            result[index] = {
                                'name' : name,
                                'modelMap' : modelMap,
                                'array' : array,
                                'rawS3' : rawS3
                            };
                            if (index === 0) {
                                var node = carNode;
                                node.s({
                                    'wf.width' : 0,
                                    'shape3d' : name,
                                    'note.position' : 44,
                                    'note' : 'DevicePixelRatio : ' + window.devicePixelRatio,
                                    'note.face' : 'top',
                                    'note.autorotate' : true,
                                    'note.font' : '46px arial, sans-serif'
                                });
                                node.s3(rawS3);
                                node.r3(0, Math.PI, 0);
                                dm.add(node);
                                polyline.setElevation(rawS3[1] * 0.5 + 2);
                                ht.Default.startAnim(params);
                            }
                        }
                    });
                });
            };
            var createPath = function(dm, radius) {
                var polyline = new ht.Polyline();
                polyline.setThickness(2);
                polyline.s({
                    'shape.border.pattern': [16, 16],
                    'shape.border.color': 'rgba(0, 0, 0, 0)',
                    'shape3d.resolution': 300,
                    '3d.selectable': false
                });
                dm.add(polyline);
                var cx = 0,
                    cy = radius * Math.PI * 0.5,
                    count = 500,
                    points = [{ x: radius, y: -cy, e: 0 }],
                    segments = [1];
                for (var k = 0; k < count + 1; k++) {
                    var angle = k * Math.PI / count;
                    points.push({
                        x: cx + radius * Math.cos(angle),
                        y: cy + radius * Math.sin(angle),
                        e: 0
                    });
                    segments.push(2);
                }
                cy *= -1;
                radius *= -1;
                for (var k = 0; k < count + 1; k++) {
                    var angle = k * Math.PI / count;
                    points.push({
                        x: cx + radius * Math.cos(angle),
                        y: cy + radius * Math.sin(angle),
                        e: 0
                    });
                    segments.push(2);
                }
                polyline.setPoints(points);
                polyline.setSegments(segments);
                var shape = new ht.Shape();
                shape.setPoints(points);
                shape.setSegments(segments);
                shape.s({
                    'top.visible' : false,
                    'bottom.image' : 'road',
                    'bottom.reverse.flip' : true,
                    'bottom.uv.scale' : [13, 1],
                    'back.visible' : false,
                    'front.reverse.flip' : true,
                    '3d.selectable': false
                });
                shape.setThickness(180);
                shape.setTall(15);
                shape.setClosePath(true);
                dm.add(shape);
                return polyline;
            };
        </script>
    </head>
    <body onload="init();">
    </body>
</html>
```
来介绍下这次 Demo 中都用到的了 [HT for Web](http://www.hightopo.com/) 的那些技术。
首先是车子，车子并不是通过 [HT for Web](http://www.hightopo.com/) 生成的，而是通过专业的 3D 工具设计，然后导出 obj 和 mtl 文件，HT for Web 对 obj 和 mtl 文件进行解析，然后显示在 Graph3dView 中，更多具体的介绍可以查阅我么的 obj 文档：[http://www.hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html](http://www.hightopo.com/guide/guide/plugin/obj/ht-obj-guide.html)
在 obj 文档中，你会看到一个一个飞机的例子，飞机沿着设定好的路线飞行，你应该会想，这个寻路是怎么实现的呢？其实很简单，我们将路线切割成一个个很小很小的单元，然后根据算法依次获取到小单元的坐标设置到移动的物体上，这样物体就动起来了。
在 Demo 中，有一条很精致的马路，这条马路就是一个 Shape 节点，根据车的路径生成的马路，Shape 是一个六面体，因为首尾相连了，所以没有左右面，在这个例子中，我将马路的 back 和 top  面隐藏了，然后 bottom 面支持翻转，让 bottom 面的贴图显示在内表面上，这样马路就建成了。

