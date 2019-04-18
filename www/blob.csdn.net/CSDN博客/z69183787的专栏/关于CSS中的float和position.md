# 关于CSS中的float和position - z69183787的专栏 - CSDN博客
2014年04月10日 06:45:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：795
个人分类：[前端-Css](https://blog.csdn.net/z69183787/article/category/2175041)

原文 http://learn.shayhowe.com/advanced-html-css/detailed-css-positioning
当构建页面排版时，有不同的方法可以使用。使用哪一种方法取决于具体页面的排版要求，在不同的情况下，某些方法可能好过于其他的方法。
比如，可以使用若干个浮动元素来构建一个整洁简洁的页面排版。或者，如果需要更复杂的排版要求，可以考略使用其他方法，比如使用相对定位和绝对定位。
在这篇文章中，我们首先要讨论元素浮动；然后，我们要讨论如何使用x,y和z轴控制元素的位置。
元素浮动
当构建一个页面的排版时，使用元素浮动是一种直观的方法去控制元素的位置。元素浮动可以让一个元素靠近或者远离另一个元素。它根据一个元素的大小和其父节点容器的大小来构造这个元素在排版中与其他元素之间的关系。
当一个元素被浮动时，这个元素是挨着前一个元素，还是出现在新的一行？这取决于它相邻元素的位置。
当然，元素浮动在提供强大威力的同时，也带来了很多问题。一个著名的问题就是，当一个父节点包括了若干浮动的子元素时，父节点的排版会考虑到浮动子元素的大小和位置，但是子元素并不会影响父节点的大小。在这种情况下，父节点的height变成了0，并且忽略其他属性。 很多时候，这个问题没有被注意到，特别是当父节点没有附合任何的CSS样式，而且嵌套的元素也看上去被正确地排列了。
如果没有正确地排列嵌套元素，那么可能引发排版上的错误。请参阅下面的这个例子， 类名为.box-set的DIV容器应该有浅灰色的背景，但是因为其嵌套元素都是浮动元素，所以原本期望的背景色没有出现。如果你查看.box-set的盒模型，你会发现它的height值为0。
```
<div class="box-set">
  <div class="box">Box 1</div>
  <div class="box">Box 2</div>
  <div class="box">Box 3</div>
</div>
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
.box-set {
  background: #e8eae9;
}
.box {
  background: #8ec63f;
  height: 100px;
  float: left;
  margin: 10px;
  width: 200px;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![](http://images.cnitblog.com/blog/427748/201302/01042810-859296573f3842d1a60354d640a9c265.png)
一个解决方法是在父容器闭合标签前加上一个空元素，并且设置它的clear : both。“清除浮动元素”这一招可以解决大多数情况，但是它并不是“语义正确”的。随着页面上需要清除的浮动元素 的数量增加，需要的空节点的数量也随之增加，但是这些空节点并没有任何的HTML语义。
幸运的是，有一些其他技巧同样使用，最著名的就是使用overflow和clearfix的技巧。
使用overflow
在父容器上设置 overflow:auto 可以让它自动包含子元素，从而具有了实际的height值，这样上例中的灰色背景将得到显示。
在IE6中使用这一招时需要给父节点容器设置height或者width属性。因为实际情况中height常常是可变的，所以是设置width的值。对于苹果平台上的IE，设置overflow:auto将会给容器增加滚动条，所以最好是设置 overflow : hidden 。
```
.box-set {
  background: #404853;
  overflow: auto;
}
```
![](http://images.cnitblog.com/blog/427748/201302/01044935-cd141463293d4bac869cc28cbe9714d6.png)
这一招有些副作用。例如，需要增加样式或者移动子元素超出了父容器的范围时，比如实现边框阴影或者加上一个下拉菜单。在下面的例子中你将看到，当边框阴影超出父容器的范围时就会被遮盖了，同时第二个子元素排列有问题。
![](http://images.cnitblog.com/blog/427748/201302/01050325-c1b4da49ba49416f89c47fd4547ba458.png)
使用clearfix
clearfix通常搭配 :before 或者 :after 伪元素使用。使用伪元素就是在包含浮动元素的父容器的前面或者后面创造一个隐藏的元素。通过对 :before 伪元素使用 display:table 来创建一个匿名的table-cell元素，从而防止子元素的top
 margin消失。这一招在IE6和7中具有一致的效果。同样的，通过设置 :after 伪元素来防止子元素的bottom margin消失。
另外，需要对父容器使用 *zoom 属性触发hasLayout机制来解决IE6和7的一致性。hasLayout机制决定了元素应该怎样渲染和框住它的内容，以及元素间的位置关系。
在下面使用clearfix的例子中，你将看到元素就算超出了父容器也可以显示了。（注意边框阴影）
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
.box-set:before,
.box-set:after {
  content: "";
  display: table;
}
.box-set:after {
  clear: both;
}
.box-set {
  *zoom: 1;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![](http://images.cnitblog.com/blog/427748/201302/01053249-fd253ab88e3d4eca8f665f6b4e6c8d80.png)
有效地包含浮动元素
具体使用哪一个技巧取决于你的个人习惯。有些人倾向于使用clearfix技巧，因为它在跨浏览器上的一致性更好。另一些人觉得clearfix需要更多的代码。因此，选择哪个技巧不重要，重要的是注释注明和写进文档。
一个常见的实现就是给所有需要包含浮动元素的父容器一个统一的类名，方便管理样式。比如使用下列代码，这样只需给需要包含浮动元素的父节点加上 group的类名即可。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
.group:before,
.group:after {
  content: "";
  display: table;
}
.group:after {
  clear: both;
}
.group {
  *zoom: 1;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
Position属性
有些情况下，你需要更多的对元素位置的控制，这时候就需要使用position属性了。position属性一共有五种不同的取值。
Position Static
这是position属性的默认取值，意味着这个元素没有也不接受任何对于该元素的位置偏移量属性。
在下列的例子中，所有的盒子都将一个接一个的堆起来。因为它们都是块级元素，并且没有被浮动。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<div class="box-set">
  <div class="box box-1">Box 1</div>
  <div class="box box-2">Box 2</div>
  <div class="box box-3">Box 3</div>
  <div class="box box-4">Box 4</div>
</div>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
.box-set {
  background: #e8eae9;
}
.box {
  background: #8ec63f;
  height: 80px;
  width: 80px;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![](http://images.cnitblog.com/blog/427748/201302/01065341-0b35e27fbcff4176936b215d84c4b3c4.png)
Position Relative
relative定位非常相识于static定位。最大的区别就是相对定位的元素可以使用偏移量：top, right, bottom, left。这四个偏移量可以让元素朝任何方向移动，从而精确的控制显示出来的位置。
注意，尽管相对定位的元素可以使用偏移量属性，但是它在网页排版时按照的却是跟static
 position相同的方式（这点区别于fixed position和absolute position）。另外，相对定位的元素可以覆盖其它元素，或者被其它元素覆盖，尽管它自身并没有偏移。
在下列的例子中，所有的盒子都是一个接一个的堆起来。但是他们按照各自被定义的offset相对于原本的位置做了改变。所以，有些盒子覆盖了其他的盒子，而不是推开其他盒子。如果一个元素是相对定位（甚至它还做了偏移），它周围的元素在排版时参考的依然是那个元素原本的位置（指在没有偏移时的位置）。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<div class="box-set">
  <div class="box box-1">Box 1</div>
  <div class="box box-2">Box 2</div>
  <div class="box box-3">Box 3</div>
  <div class="box box-4">Box 4</div>
</div>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
.box-set {
  background: #e8eae9;
}
.box {
  background: #8ec63f;
  height: 80px;
  position: relative;
  width: 80px;
}
.box-1 {
    top: 20px;
  }
.box-2 {
  left: 40px;
}
.box-3 {
  bottom: -10px;
  right: 20px;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![](http://images.cnitblog.com/blog/427748/201302/01071235-18cf2135dada4ed6ae9d8814ecc47a8b.png)
如果同时声明top和bottom的值，那么top值的优先级更高。如果同时声明left和right的值，那么优先级取决于网页的语言（例如，英语法语德语西班牙语）。比如，在英语的网页中，left的优先级更高；在阿拉伯语的网页中，right的优先级更高。
Position Absolute
绝对定位的元素接受偏移量。在排版中，绝对定位的元素从原来的位置上被抹去了（因此它后面的元素将占领它目前的位置），然后根据它的相对定位的父节点的位置定位。如果它没有相对定位的父节点，那么元素直接参考body容器定位。
对绝对定位的元素使用偏移量属性，这个元素将以相对定位的父节点作为参考系进行偏移。
如果没有对绝对定位的元素使用偏移量属性，那么该元素将被放置在父节点的左上角。如果只设置了top属性，那么该元素则只做垂直方向的偏移，而水平方向依然靠着父节点的最左边。
在下面的例子中，所有的盒子相对于父容器作了绝对定位。如果偏移量是正数，那么盒子被“往里推”了；如果偏移量是负数，那么盒子被“往外拉”了。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<div class="box-set">
  <div class="box box-1">Box 1</div>
  <div class="box box-2">Box 2</div>
  <div class="box box-3">Box 3</div>
  <div class="box box-4">Box 4</div>
</div>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
.box-set {
  background: #e8eae9;
  height: 200px;
  position: relative;
}
.box {
  background: #8ec63f;
  height: 80px;
  position: absolute;
  width: 80px;
}
.box-1 {
  top: 6%;
  left: 2%;
}
.box-2 {
  top: 0;
  right: -40px;
}
.box-3 {
  bottom: -10px;
  right: 20px;
}
.box-4 {
  bottom: 0;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![](http://images.cnitblog.com/blog/427748/201302/01082728-78f6ff1a6ed5402cb9fe6beba77b79b1.png)
如果绝对定位的元素有固定的height和width值：
如果同时声明top和bottom的值，那么top值的优先级更高。如果同时声明left和right的值，那么优先级取决于网页的语言（例如，英语法语德语西班牙语）。比如，在英语的网页中，left的优先级更高；在阿拉伯语的网页中，right的优先级更高。
如果绝对定位的元素没有固定的height或者width值：
如果元素并没有固定height值，可是却同时有top和bottom的值，那么该元素将横跨除了top和bottom之外剩下的所有高度。同理，如果元素并没有固定width值，可是同时有left和right的值，那么该元素将横跨除了left和right之外剩下的所有宽度。如果元素既没有固定height和width值，却同时有top, bottom, left, right值，那么该元素将占据除了四条边距离之外的所有空间。
Position Fixed
固定定位的元素跟绝对定位的元素很相似，只不过它参照的坐标系是浏览器的视口(viewport)。固定定位的元素不随着页面滚动而移动，它会始终保持在那个位置。另外，在IE6中使用固定定位有一个BUG，需要使用一些hack来解决。
一个典型的使用固定定位的例子是网页中的顶部横条或者底部横条。当用户滚动页面时，那个横条始终停留在浏览器视口的顶部或者底部。
```
<footer>Fixed Footer</footer>
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
footer {
  bottom: 0;
  left: 0;
  position: fixed;
  right: 0;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
z-index 属性
网页是一个二维空间，根据x轴和y轴展示各个元素。可是，如果你需要让某个元素放置在其他元素的上面（可在默认的排版中，她并不是在他的上面），这时你可以改变她的z-index属性，让她跑到他的上面。
元素在z轴的位置的位置取决于它在DOM中出现的先后顺序。如果两个元素发生重叠，先在DOM中出现的元素通常排在后出现元素的下面。改变先出现元素的z-index属性是一个简单的方法让他出现在更上面。拥有最高z-index属性的元素永远被排在最上面，与它在DOM树中的位置无关。
如果你需要设置一个元素的z-index属性，你必须首先设置这个元素是relative，absolute或者fixed定位。就像对于设置top, bottom, left, right属性一样。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<div class="box-set">
  <div class="box box-1">Box 1</div>
  <div class="box box-2">Box 2</div>
  <div class="box box-3">Box 3</div>
  <div class="box box-4">Box 4</div>
</div>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
.box-set {
  background: #e8eae9;
  height: 160px;
  position: relative;
}
.box {
  background: #8ec63f;
  border: 3px solid #f7941d;
  position: absolute;
}
.box-1 {
  left: 10px;
  top: 10px;
}
.box-2 {
  bottom: 10px;
  left: 70px;
  z-index: 3;
}
.box-3 {
  left: 130px;
  top: 10px;
  z-index: 2;
}
.box-4 {
  bottom: 10px;
  left: 190px;
  z-index: 1;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![](http://images.cnitblog.com/blog/427748/201302/01092650-be6c342f37124b9d93bb68716e2c8fd6.png)
