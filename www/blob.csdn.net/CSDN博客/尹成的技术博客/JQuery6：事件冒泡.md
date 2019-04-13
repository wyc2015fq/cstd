
# JQuery6：事件冒泡 - 尹成的技术博客 - CSDN博客

2018年11月14日 15:07:34[尹成](https://me.csdn.net/yincheng01)阅读数：56


\#事件冒泡
\#\#\#什么是事件冒泡
在一个对象上触发某类事件（比如单击onclick事件），如果此对象定义了此事件的处理程序，那么此事件就会调用这个处理程序，如果没有定义此事件处理程序或者事件返回true，那么这个事件会向这个对象的父级对象传播，从里到外，直至它被处理（父级对象所有同类事件都将被激活），或者它到达了对象层次的最顶层，即document对象（有些浏览器是window）。
\#\#\#事件冒泡的作用
事件冒泡允许多个操作被集中处理（把事件处理器添加到一个父级元素上，避免把事件处理器添加到多个子级元素上），它还可以让你在对象层的不同级别捕获事件。
\#\#\#阻止事件冒泡
事件冒泡机制有时候是不需要的，需要阻止掉，通过 event.stopPropagation() 来阻止
`$(function(){
    var $box1 = $('.father');
    var $box2 = $('.son');
    var $box3 = $('.grandson');
    $box1.click(function() {
        alert('father');
    });
    $box2.click(function() {
        alert('son');
    });
    $box3.click(function(event) {
        alert('grandson');
        event.stopPropagation();
    });
    $(document).click(function(event) {
        alert('grandfather');
    });
})
......
<div class="father">
    <div class="son">
        <div class="grandson"></div>
    </div>
</div>`\#\#\#阻止默认行为
阻止右键菜单
`$(document).contextmenu(function(event) {
    event.preventDefault();
});`\#\#\#合并阻止操作
实际开发中，一般把阻止冒泡和阻止默认行为合并起来写，合并写法可以用
`// event.stopPropagation();
// event.preventDefault();
// 合并写法：
return false;`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

