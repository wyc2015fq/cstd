# 基于 HTML5 的 WebGL 3D 版俄罗斯方块 - xhload3d - 博客园
# 前言
**摘要**：2D 的俄罗斯方块已经被人玩烂了，突发奇想就做了个 3D 的游戏机，用来玩俄罗斯方块。。。实现的基本想法是先在 2D 上实现俄罗斯方块小游戏，然后使用 3D 建模功能创建一个 3D 街机模型，最后将 2D 小游戏贴到 3D 模型上。
（ps：最后拓展部分实现将视频与3D模型的结合）
![](https://i.imgur.com/g6w17yt.gif)
[http://www.hightopo.com/demo/tetris/](http://www.hightopo.com/demo/tetris/)
# 代码实现
## 首先，先完成 2D 小游戏
在查看官方文档的过程中，了解到 HT 的组件参数都是保存在 ht.DataModel() 对象中，将数据模型在视图中进行加载后呈现各种特效。
```
gameDM = new ht.DataModel(); //初始化数据模型
g2d = new ht.graph.GraphView(gameDM); //初始化2d视图
g2d.addToDOM(); //在页面上创建视图
```
## 开始游戏模型的创建
- 第一步，先让我们为游戏创建一个框体，为游戏限定范围。在文档中，我们可以知道 ht.Node 是 graphView 呈现节点图元的基础类，除了可以显示图片外，还能支持多种预定义的图形。所以我打算使用该类创建4个长方形，用它们来做游戏的范围限定。
```
var lineNode = new ht.Node();
lineNode.s({
　　"shape": "rect", //矩形
　　"shape.background": "#D8D8D8", //设置底色
　　"shape.border.width": 1, //边框宽度 1
　　"shape.border.color": "#979797" // 边框颜色
});
lineNode.setPosition(x, y); // 设置图元展示位置，左上角为0, 0 图元坐标指向它们的中心位置
lineNode.setSize(width, height); // 设置图元宽、高属性
gameDM.add(lineNode); // 将设置好后的图元信息加入数据模型中
```
设置 x:552, y:111, width:704, height:22 后我们可以得到第一个图形：
![](https://i.imgur.com/llKnLSR.png)
边框的top已经有了，现在让我们再创建另外三条边来组成一个框体：
```
x:211, y:562, width:22, width:880
x:893, y:562, width:22, width:880
x:552, y:1013, width:704, width:22
```
得到效果如下：
![](https://i.imgur.com/Zcp4Bzd.png)
边框基本完成，在浏览的过程中发现4个边框可以被拖拽。接下来对边框初始化的方法进行调整：
```
lineNode.s({
　　"shape": "rect", //矩形
　　"shape.background": "#D8D8D8", //设置底色
　　"shape.border.width": 1, //边框宽度 1
　　"shape.border.color": "#979797", // 边框颜色
　　"2d.editable" : false, // 是否可编辑
　　"2d.movable" : false, //是否可移动
　　"2d.selectable" : false //是否可选中
});
```
- 生成方块，我的想法是生成多个正方形，将它们组合成我们需要的图形，通过坐标的计算来将它们摆放在相应的位置:
![](https://i.imgur.com/goalVtB.png)
方块生成后，开始对图形进行旋转操作。这其中有两个方案，第一种是将图形的翻转后的图形坐标按顺序保存在数组中，每次改变形状时取数组中的前一组或后一组坐标来进行改变；第二种是使用 ht.Block() 对象将对应的图元组合成一个整体，在变形时只需按对应的方向选择 90° 即可。在这里，我选择了第二中方式，代码如下： 
```
function createUnit(x, y) {
    var node = new ht.Node();
    node.s({
        "shape": "rect",
        "shape.background": "#D8D8D8",
        "shape.border.width": 1,
        "shape.border.color": "#979797"
    });
    node.setPosition(x, y);
    node.setSize(44, 44);
    gameDM.add(node);
    return node;
}
var block = new ht.Block();
block.addChild(createUnit(552, 133));
block.addChild(createUnit(552, 89));
block.addChild(createUnit(508, 133));
block.addChild(createUnit(596, 133));
block.setAnchor(0.5, 0.75); //设置组合的中心位置， 旋转时将安装此点来进行
block.setPosition(552, 144);
```
Block 设置中心点 Anchor 如下图：
![](https://i.imgur.com/eZV0gOC.png)
在设置旋转时，只需使用 setRotation 函数对 block 进行旋转即可：
```
block.setRotation(Math.PI*rotationNum/2); //rotationNum 是一个计数器，保存已经旋转次数，保证每次都是在上一次的基础上旋转90°
```
- 方块有了，现在就该让它动起来了。设置定时器，使方块每隔一段时间下降一定距离，并添加键盘的监听事件，以此实现 w：翻转、s：左移动、d：右移动、s：下移的操作，同时为了不使方块移动出边界，在每次位移时都将对坐标进行一次验证：
```
var offset = 44;
var intervalTime = 1000;
var topX = 552;
var topY = 111;
var leftSize = 211,
rightSize = 882,
bottomSize = 1002;
var rotationNum = 0;
window.addEventListener('keydown', function(e){
　　var index = 0;
　　var maxY = null;
　　if(e.keyCode == 87){
　　　　// up w
　　　　rotationNum ++;
　　　　block.setRotation(Math.PI*rotationNum/2);
　　　　if (!checkRotation(block)) {
　　　　　　rotationNum --;
　　　　　　block.setRotation(Math.PI*rotationNum/2);
　　　　}
　　} else if (e.keyCode == 65) {
　　　　// left a
　　　　moveBlock('x', -offset, block);
　　} else if (e.keyCode == 68) {
　　　　// right d
　　　　moveBlock('x', offset, block);
　　} else if(e.keyCode == 83){
　　　　// down s
　　　　moveBlock('y', offset, block);
　　}
}, false);
setInterval(function(){
　　if(!moveBlock("y", offset, block)){
　　　　//无法进行位移，创建新的方块
　　　　rotationNum = 0; //方块翻转次数归0
　　　　block = createNode(blockType); //生成新的方块
　　　　blockType = parseInt(Math.random()*100%5); //下一次生成的方块图形
　　}
}, intervalTime); //执行间隔
//移动方块，移动成功时返回：true，无法移动时返回：false
function moveBlock(axis, offset, block){
　　// 移动方块
　　var ids = [];
　　var yindexs = [];
　　var indexArr = new Array();
　　for(var i = 0; i < block.size(); i ++){
　　　　var childNode = block.getChildAt(i);
　　　　var childx = childNode.getPosition().x;
　　　　var childy = childNode.getPosition().y;
　　　　if (yindexs.indexOf(childy) == -1) {
　　　　　　yindexs.push(childy);
　　　　}
　　　　if(axis === 'x'){
　　　　　　childx += offset;
　　　　}else if (axis === 'y') {
　　　　　　childy += offset;
　　　　}
　　　　// 验证方块的移动是否超出边界
　　　　if(childx < leftSize || childx > rightSize || childy > bottomSize){
　　　　　　return false;
　　　　}
　　　　var obj = new Object();
　　　　obj.x = childx;
　　　　obj.y = childy;
　　　　indexArr.push(obj);
　　　　ids.push(childNode.getId());
　　}
　　//判断图形位移过程中是否与其他方块触碰
　　for(var j = 0; j < yindexs.length; j ++){
　　　　var indexY = yindexs[j];
　　　　if (axis === 'y') {
　　　　　　indexY += offset;
　　　　}
　　　　//getDatasInRect 方法能获取到一个范围中的所有图元信息
　　　　var nodeList = g2d.getDatasInRect({x:233, y:indexY, width:638, height:2}, true, false);
　　　　if(nodeList.length > 0){
　　　　　　// 触碰
　　　　　　for(var i = 0; i < nodeList.length; i++){
　　　　　　　　var x = nodeList.get(i).getPosition().x;
　　　　　　　　var y = nodeList.get(i).getPosition().y;
　　　　　　　　var id = nodeList.get(i).getId();
　　　　　　　　if (ids.indexOf(id) > -1) {
　　　　　　　　　　// 位移的图元
　　　　　　　　　　continue;
　　　　　　　　}
　　　　　　　　for (var k = 0; k < indexArr.length; k++) {
　　　　　　　　　　var obj = indexArr[k];
　　　　　　　　　　if (obj.x === x && obj.y === y){
　　　　　　　　　　　　// 该停下了
　　　　　　　　　　　　return false;
　　　　　　　　　　}
　　　　　　　　}
　　　　　　}
　　　　}
　　}
　　var blockX = block.getX();
　　var blockY = block.getY();
　　if (axis === 'x') {
　　　　blockX += offset;
　　}else if (axis === 'y') {
　　　　blockY += offset;
　　}
　　// 方块移动到新的坐标
　　block.setPosition(blockX, blockY);
　　return true;
}
// 验证方块是否可以进行翻转
function checkRotation(block){
　　for(var i = 0; i < block.getChildren().length; i++){
　　　　var node = block.getChildAt(i);
　　　　var childx = node.getPosition().x;
　　　　var childy = node.getPosition().y;
　　　　// 判断翻转后的图形是否会超出范围
　　　　if(childx < leftSize || childx > rightSize || childy > bottomSize){
　　　　　　return false;
　　　　}
　　}
　　return true;
}
```
- 在完成方块的位移与变形之后，我们的小游戏就只差最后一步了：对填充满的方块进行消除。在开始的时候，我们就知道所有的信息都是保存在数据模型当中，所以我们要消除方块。只需要将它们从数据模型中删除即可，实现代码如下：
```
function deleteBlock(block){
　　// 消除已经填充满的方格
　　var yindexs = []; // 要判断的y轴坐标
　　var num = 0;
　　for(var i = 0; i < block.size(); i ++){
　　　　var childNode = block.getChildAt(i);
　　　　var childy = childNode.getPosition().y;
　　　　var nodeList = g2d.getDatasInRect({x:233, y:childy, width:638, height:2}, true, false);
　　　　if (nodeList.length == 15) {
　　　　　　for(var i = 0; i < nodeList.length; i++){
　　　　　　　　gameDM.remove(nodeList.get(i)); // 在数据模型中移除对应的图元
　　　　　　}
　　　　　　num ++;
　　　　　　yindexs.push(childy);
　　　　}
　　}
　　if (yindexs.length > 0) {
　　　　for(var i = 0 ; i < yindexs.length; i++){
　　　　　　// 将被消除图元上方的图元进行组合，并整体向下移动一个位置
　　　　　　var yindex = yindexs[i];
　　　　　　var h = yindex - 133 - offset;
　　　　　　var moveList = g2d.getDatasInRect({x:233, y:133, width:638, height:h}, true, false);
　　　　　　var mblock = new ht.Block();
　　　　　　for(var i = 0; i < moveList.size(); i++){
　　　　　　　　mblock.addChild(moveList.get(i));
　　　　　　}
　　　　　　moveBlock('y', offset, mblock);
　　　　}
　　}
}
```
到此，一个简单的俄罗斯方块小游戏就实现了。当然，这个游戏还有很多可以拓展的地方，比如：更多的方块类型，游戏分数的统计，下一步预测窗体，游戏背景修改等。这些先不考虑，我们先开始下一步。
## 创建 3D 模型
在 3D 建模文档中了解到，HT 通过一个个三角形来组合模型。
- 首先，先将网络上查找到的街机模型进行拆分，将其中的各个模块拆分成三角形面：
![](https://i.imgur.com/O4XomuM.png)
如图所示，将0所在位置设置为原点(0,0,0)，我们打开画图工具根据标尺大概估计出每个坐标相对原点的位置，将计算好的坐标数组传入 vs 中，同时在is顶点索引坐标中将每个三角图形的组合传入其中：
```
ht.Default.setShape3dModel('damBoard', { // 为新模型起名
　　vs: [
　　　　0, 0, 0, //0
　　　　0.23, 0, 0,
　　　　0.23, 0.27, 0,
　　　　0.27, 0.28, 0, //3
　　　　0.27, 0.32, 0,
　　　　0.20, 0.33, 0,
　　　　0.18, 0.51, 0, // 6
　　　　0.27, 0.57, 0,
　　　　0.27, 0.655, 0,
　　　　0.20, 0.67, 0, // 9
　　　　0, 0.535, 0
　　],
　　is: [
　　　　0, 1, 2,
　　　　0, 2, 5,
　　　　2, 3, 4,
　　　　4, 2, 5,
　　　　5, 0, 10,
　　　　10, 5, 6,
　　　　6, 7, 8,
　　　　8, 6, 9,
　　　　9, 10, 6
　　]
});
```
与 2D 一样，我们创建一个 ht.Node() 的基础图元，类型设置为我们新注册的3D模型名称：
```
dataModel = new ht.DataModel();
g3d = new ht.graph3d.Graph3dView(dataModel);
g3d.addToDOM();
var node = new ht.Node();
node.s({
　　'shape3d': 'damBoard',
　　'shape3d.reverse.flip': true,
　　'3d.movable': false,
　　'3d.editable': false,
　　'3d.selectable': false
});
node.p3([0, 20, 0]);
node.s3([100, 100, 100]);
dataModel.add(node);
```
![](https://i.imgur.com/w4MswV0.png)
已经有个侧边了，我们可以将坐标系延z轴移动一定距离后得到另一个侧边的坐标数组同时再根据没个面的不同，分别设置 is 数组，将所有的面组合起来后，我们就将初步得到一个街机模型：
```
vs: [
　　0, 0, 0, //0
　　0.23, 0, 0,
　　0.23, 0.27, 0,
　　0.27, 0.28, 0, //3
　　0.27, 0.32, 0,
　　0.20, 0.33, 0,
　　0.18, 0.51, 0, // 6
　　0.27, 0.57, 0,
　　0.27, 0.655, 0,
　　0.20, 0.67, 0, // 9
　　0, 0.535, 0,
　　0, 0, 0.4, //11
　　0.23, 0, 0.4,
　　0.23, 0.27, 0.4,
　　0.27, 0.28, 0.4, //14
　　0.27, 0.32, 0.4,
　　0.20, 0.33, 0.4,
　　0.18, 0.51, 0.4, // 17
　　0.27, 0.57, 0.4,
　　0.27, 0.655, 0.4,
　　0.20, 0.67, 0.4, // 20
　　0, 0.535, 0.4,
]
```
![](https://i.imgur.com/gveTB4p.png)
- 模型不够美观，我们可以给模型的每个面进行贴图，参考文档中对模型 uv 参数的说明，我们可以知道 uv 对应的是模型中每个顶点在图片中的偏移量，图片的左上角为(0, 0)右下角为(1，1)， 以此我们可以为每个面设置贴图。如：
![](https://i.imgur.com/BbrI6KR.png)
```
ht.Default.setShape3dModel('damBoard', {
　　vs: vsArr,
　　is: isArr,
　　uv: [
　　　　0, 1,
　　　　0.81, 1,
　　　　0.81, 0.42,
　　　　1, 0.4,
　　　　1, 0.36,
　　　　0.725, 0.34,
　　　　0.65, 0.26,
　　　　1, 0.16,
　　　　1, 0.03,
　　　　0.75, 0,
　　　　0, 0.22,
　　　　, ,
　　　　, ,
　　　　, ,
　　　　, ,
　　　　, ,
　　　　, ,
　　　　, ,
　　　　, ,
　　　　, ,
　　　　, ,
　　　　, ,
　　], //uv中要将is中有使用到的点的偏移量都进行设值
　　image: '/image/side1.jpg' //图片地址
});
```
同理，为其他面也分别设置 uv，最终效果如下：
![](https://i.imgur.com/lW2s64u.png)
- 3D 模型整体已经建好了， 还需要给模型加上游戏按钮。在官方文档建模函数中，我们可以看到已经有大量封装完毕的图形供我们使用。在这里我选择使用 createRightTriangleModel 创建直角三角形的方法来创建操作按钮，使用 createSmoothSphereModel 函数来创建开始按钮：
```
ht.Default.setShape3dModel('button', ht.Default.createRightTriangleModel(true, true));
ht.Default.setShape3dModel('startButton', ht.Default.createSmoothSphereModel(20, 20, 0, Math.PI * 2, 0, Math.PI));
```
根据注册好的模型生成按钮：
```
createKeyboard('up', [21.5, 52.5, 26], [0, -Math.PI / 4, 0]);
createKeyboard('down', [25.5, 51.75, 26], [0, Math.PI * 3 / 4, 0]);
createKeyboard('left', [23.5, 52, 28], [0, Math.PI / 4, 0]);
createKeyboard('right', [23.5, 52, 24], [0, Math.PI * 5 / 4, 0]);
// 创建开始按钮
function createStartButton() {
　　var node = new ht.Node();
　　node.setTag('restart');
　　node.s({
　　　　'shape3d': 'startButton',
　　　　'shape3d.reverse.flip': true,
　　　　'shape3d.color': '#7ED321',
　　　　'3d.movable': false,
　　　　'3d.editable': false
　　});
　　node.p3([23.5, 52.5, 11]); // 按摆放位置
　　node.s3([3, 3, 3]); // 按钮放大倍数
　　dataModel.add(node);
}
// 创建操作按钮
function createKeyboard(tag, p3, r3) {
　　var node = new ht.Node();
　　node.setTag(tag);
　　node.s({
　　　　'shape3d': 'button',
　　　　'shape3d.reverse.flip': true,
　　　　'shape3d.color': 'red',
　　　　'3d.movable': false,
　　　　'3d.editable': false
　　});
　　node.p3(p3); // 按摆放位置
　　node.s3([1.5, 1.5, 1.5]); // 按钮放大倍数
　　node.r3(r3); // 将按钮按Y轴旋转，已保存按钮指向正确
　　dataModel.add(node);
}
```
最终效果如下：
![](https://i.imgur.com/j4GWFwA.png)
- 将 2D 小游戏贴到3D模型上，在文档中我们可以发现 setImage 属性不仅仅是只能设置正常的图片，还可以使用它来注册一个 canvas 图形组件。而2D视图可以通过 getCanvas() 来获取画布信息。
```
ht.Default.setImage('gameScrn', g2d.getCanvas());
ht.Default.setShape3dModel('scrn', {
　　vs: vsArr,
　　is: isArr,
　　uv: scrnUV,
　　image: 'gameScrn' // 将注册的2d画布信息当成屏幕的图片贴图信息
});
// 设置 2d 的画布大小
g2d.getWidth = function () { return 1000; }
g2d.getHeight = function () { return 600; }
g2d.getCanvas().dynamic = true;//设置这个是为了让canvas能动态显示
// 设置计时器，让2d画布上的每次改变都能及时的在3D模型上进行展示
setInterval(function () {
　　node.iv(); // 每次改变都需要对街机模型进行刷新，刷新时间为下一帧
　　g2d.validateImpl(); // 立即对2D上的图元进行刷新
}, 10);
// 设置500毫秒后，缩放平移整个2D画布以展示所有的图元
setTimeout(function () {
　　g2d.fitContent(true);
}, 500);
```
效果如下：
![](https://i.imgur.com/EyLJpOR.png)
- 在 2D 画布上，我们已经为游戏添加了键盘事件，现在我们只需要为 3D 模型上的5个按钮分别绑定对应方法即可：
```
g3d.mi(function (e) { // addInteractorListener 交互事件监听器的缩写
　　if (e.kind === 'clickData') { // 判断是否为点击事件
　　　　var tag = e.data.getTag();
　　　　if (tag === 'restart') {
　　　　　　gameAgain(node);
　　　　}
　　　　if (start) {
　　　　　　if (tag === 'up') {
　　　　　　　　block.setRotation(Math.PI * (1 + rotationNum) / 2);
　　　　　　　　rotationNum++;
　　　　　　　　if (!checkRotation(block)) {
　　　　　　　　　　// 边缘变形限制
　　　　　　　　　　rotationNum--;
　　　　　　　　　　block.setRotation(Math.PI * rotationNum / 2);
　　　　　　　　}
　　　　　　} else if (tag === 'down') {
　　　　　　　　moveBlock('y', offset, block);
　　　　　　} else if (tag === 'left') {
　　　　　　　　moveBlock('x', -offset, block);
　　　　　　} else if (tag === 'right') {
　　　　　　　　moveBlock('x', offset, block);
　　　　　　}
　　　　}
　　}
});
```
到此基本完成了在3D街机上玩游戏的功能。
![](https://i.imgur.com/DX88Zgr.gif)
[http://www.hightopo.com/demo/tetris/](http://www.hightopo.com/demo/tetris/)
# 拓展
上面只是一个简单的运用，既然可以将 2D 的 canvas 贴到3D上，那么是否也可以将视频贴上去呢。
实现代码如下：
```
<video id="video1" width="270" autoplay src="3D交互.mp4" style="display:none"></video>
var v = document.getElementById("video1");
var node = new ht.Node();
node.setSize(2200, 1100);
gameDM.add(node);
v.addEventListener('play', function () {
　　var i = window.setInterval(function () {
　　　　node.setImage(v);//将视频截图贴在图元上
　　　　g2d.validateImpl();//刷新2d画布
　　　　g3d.invalidateData(box);//刷新3d图纸中的街机模型
　　　　if (v.ended) {
　　　　　　clearInterval(i)
　　　　}
　　}, 20);
}, false);
```
实现上有什么问题可以直接留言或者私信或者直接去官网（[https://hightopo.com/](https://hightopo.com/)）上查阅相关的资料。
# 总结
在 3D 模型上的视频播放给予了我很大的兴趣。如果能将摄像头的画面转移到对应的 3D 场景中，那么我相信像一些日常的机房监控，智能城市和智能楼宇中的视频监控将更加的便捷与直观。
