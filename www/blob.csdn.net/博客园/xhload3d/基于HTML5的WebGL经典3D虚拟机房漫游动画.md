# 基于HTML5的WebGL经典3D虚拟机房漫游动画 - xhload3d - 博客园
第一人称在 3D 中的用法要参考第一人称在射击游戏中的使用，[第一人称射击游戏（FPS](https://en.wikipedia.org/wiki/First-person_shooter)[）](https://en.wikipedia.org/wiki/First-person_shooter)是以第一人称视角为中心围绕枪和其他武器为基础的视频游戏类型 ; 也就是说，玩家通过主角的眼睛来体验动作。自从流派开始以来，先进的 3D 和伪 3D 图形已经对硬件发展提出了挑战，而多人游戏已经不可或缺。
![](http://img.blog.csdn.net/20171113175620075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGhsb2FkM2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Doom 的截图，这个流派的突破游戏之一，展示了第一人称射击游戏的典型视角
现在博物馆或者公司也经常使用到 3D 动画做宣传片等等，3D 动画演绎最大的优势，便是在于内容与形式上给人的真实感受。它比平面作品更直观，比 2D 动画更真实，所以更能给观赏者以置身于广告环境当中的感受,大大增强广告的说服力。3D 技术的发展甚至挑战受众的分辨能力，使受众的判断游离于与虚拟和现实之间。
而且 3D 特效的应用为创意提供了更加广阔的思维空间,并成为创意执行的可靠保证,并丰富了创意的形式和风格手段。根据广告主题的表现诉求,可以营造出梦幻般的神奇氛围来刺激打动受众,从而起到与受众沟通的目的。
3D动画宣传片将 3D 动画、特效镜头、企业视频、照片、未来前景等内容通过后期合成、配音、解说形成一部直观、生动、喜闻乐见的高品位的企业广告宣传片，让社会不同层面的人士对企业产生正面的、积极的、良好的印象，从而建立对企业的好感与信任，并信赖该企业的产品或服务。
现在 3D 发展地如此迅速也要感谢人类对于“现实”的追求，所以学好用好 3D 是未来成功必不可少的一部分。
本文例子的思路是进入一个机房参观，打开门的动作是再生动不过了， 再加上适当地转弯，基本上完全模拟了人在机房中参观的效果。还有一个好处就是，如果要演示给领导看而又不用操作，这种炫酷的效果领导一定会很满意！
[http://www.hightopo.com/demo/room-walkthrough/index.html](http://www.hightopo.com/demo/room-walkthrough/index.html)
![](http://img.blog.csdn.net/20171113175050600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGhsb2FkM2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
界面上的“reset”和“start”两个按钮是直接加在 body 体中的 button，并在这两个按钮上添加点击事件：
```
<div class="button" style="right: 50px;background-image: url(run.png);" onclick="startAnim();"></div>
<div class="button" style="right: 100px;background-image: url(reset.png);" onclick="reset();"></div>
```
整个场景由 HT 封装的 3D 组件搭建形成的，构造这么大的场景是需要一定量的代码的，为了简化，我把场景单独拿出来，并用 HT 封装的 ht.JSONSerializer 类将场景序列化为 json，代码中只引入了生成后的 json 文件，为了让大家更明确，我这边做个示例，假设已经搭建好 3D 场景了：
```
dm = new ht.DataModel();
g3d = new ht.graph3d.Graph3dView(dm);
//.......构建好场景
dm.serialize();//可以填入number参数，作为空格缩进值
```
既然我们已经搭建好环境，转成了 json 文件，代码中不好控制，这种情况下我们会将 DataModel 数据模型再反序列化，这个函数的功能就是将 json 格式转成对象，并将反序列化的对象传入到 DataModel 数据模型中，详情请参考 HT for Web 序列化手册([http://hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html](http://hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html))：
```
var g3d = window.g3d = new ht.graph3d.Graph3dView(),
    dataModel = g3d.dm(),
    view = g3d.getView(),
    path = null;
g3d.setMovableFunc(function(data) {
    return false;
});
g3d.setVisibleFunc(function(data) {
    if (data.getName() === "path") {
        return false;
    }
    return true;
});
g3d.setEye([523, 5600, 8165]);
g3d.setFar(60000);
dataModel.deserialize(json);
```
我们目前需要操作场景中的“门”、以及我们将要走的路线“path”，遍历 DataModel 数据模型，获取这两个数据：
```
for (var i = 0; i < dataModel.size(); i++) {
    var data = dataModel.getDatas().get(i);
    if (data.getName() === "门") {//json中设置的名称
       window.door = data;
    }
    if (data.getName() === "path") {
        path = data;
    }
    if (window.door && path) {//获取到door 和 path 的data之后就跳出循环
        break;
    }
}
```
这个例子中简单来说就只有四个动作，“重置”回到原点、“开始动作”、“向前移动”，“停止”。点击“开始”按钮，在“开始动作”中我们只做了一个动作，“开门”动作，动作结束之后调用“forward”函数向前移动：
```
function startAnim() {
    if (window.isAnimationRunning) {
        return;
    }
    reset();
    window.isAnimationRunning = true;//动画是否正在进行
    ht.Default.startAnim({
        frames: 30, // 动画帧数，默认采用`ht.Default.animFrames`。
        interval: 20, // 动画帧间隔，默认采用`ht.Default.animInterval`。   
        finishFunc: function() {// 动画结束后调用的函数。
            forward();
        }, 
        action: function(t){ // action函数必须提供，实现动画过程中的属性变化。
            door.setRotationY(-120 * Math.PI / 180 * t);
        }
    });
}
```
这边的“reset”函数就是“重置”回到原点的功能，我们通过这个函数将所有变化过的都恢复初始的位置，包括“门”的位置：
```
function reset() {
    if (window.isAnimationRunning) {
        return;
    }
    g3d.setCenter([0,0,0]);
    g3d.setEye([523, 5600, 8165]);
    window.forwardIndex = 0;
    door.setRotationY(0);
}
```
要“移动”，肯定需要走路的“路径”，也就是我们刚刚获取到的“path”，代码中是通过 window.points = path.getPoints()._as 这种方式获取“path”中的所有元素，但是这种直接调用私有变量的方法不好，因为 path.getPoints 函数获取的是 ht.List 类型的对象，可以直接通过以下方式来获取数组中的节点(之后代码出现的 points[fowardIndex] 可以直接用 pointsArr[fowardIndex] 来替代)：
```
var pointsArr = [];
for(var i = 0; i < path.getPoints().size(); i++){
    var point = path.getPoints().get(i);
    pointsArr.push(point);
}
```
初始化  window.forwardIndex = 0; 通过控制“path”中前后两点来设置 3D 场景中的 Eye 和 Center，这样就能营造一个我们是第一人的效果：
```
var point1 = points[forwardIndex],
     point2 = points[forwardIndex + 1];
var distanceX = (point2.x - point1.x),
     distanceY = (point2.y - point1.y),
     distance = Math.sqrt(distanceX * distanceX + distanceY * distanceY)-200;//两点之间的距离通过三角形勾股定理计算 怕碰墙所以-200
g3d.setEye([point1.x, 1600, point1.y]);//眼睛的位置
g3d.setCenter([point2.x, 1600, point2.y]);//“我”的位置
```
HT 中 3D 组件有一个 walk(step, anim, firstPersonMode) 方法，该函数同时改变eye和center的位置，也就是eye和center在两点建立的矢量方向上同时移动相同的偏移量。step为偏移的矢量长度值。firstPersonMode参数为空时则默认采用Graph3dView#isFirstPersonMode()当前值， 如果为第一人称模式调用walk操作，该函数会考虑Graph3dView#getBoundaries()边界限制。
```
g3d.walk(distance, {
    frames: 50,
    interval: 30,
    easing: function(t) {return t; },
    finishFunc: function() {
        forwardIndex += 1;
        if (points.length - 2 > forwardIndex) {//points.length = 5
            g3d.setCenter([point2.x, 1600, point2.y]);//把结束点变成起始点
            g3d.rotate(Math.PI / 2, 0, {
                frames: 30,
                interval: 30,
                easing: function(t) {return t;},
                finishFunc:function() { forward();}
            });
        } else {
            var lastPoint = points[points.length  - 1];//json 中path的points 的最后一个点
            g3d.setCenter([lastPoint.x, 1400, lastPoint.y]);
            g3d.rotate(-Math.PI / 2, 0, {
                frames: 30,
                interval: 30,
                finishFunc: function() {
                    window.isAnimationRunning = false;
                }
            });
        }
    }
});
```
不管“path”的点有多少个，这个判断语句还是能运作，只在最后一个点是跳出 finishFunc 动画结束后调用的函数，并将 window.isAnimationRunning 值设为 false 停止 startAnim 函数。如果不是最后一个点，用户“旋转”之后，回调 forward 函数。至此，全部代码解释完毕，很短的代码量，却做出了这么大的工程！
