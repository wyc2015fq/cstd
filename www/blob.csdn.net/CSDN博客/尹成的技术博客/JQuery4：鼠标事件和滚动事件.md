
# JQuery4：鼠标事件和滚动事件 - 尹成的技术博客 - CSDN博客

2018年11月14日 15:56:12[尹成](https://me.csdn.net/yincheng01)阅读数：98


\#鼠标事件
\#\#\#1.click和dblclick
事件方法一般都有三种使用方式：我们就以click为例来介绍这些方式，接下来的方法不再冗余介绍，只是谈论最重要的知识点。
\#\#\#\#第一种方式
`<div id="test">点击触发<div>
$("ele").click(function(){
    alert('触发指定事件')
})
$("#test").click(function(){
     $("ele").click()  //手动指定触发事件 
});`\#\#\#\#第二种方式
`<div id="test">点击触发<div>
$("#test").click(function() {
    //this指向 div元素
});`\#\#\#\#第三种方式
`<div id="test">点击触发<div>
$("#test").click(11111,function(e) {
    //this指向 div元素
    //e.data  => 11111 传递数据
});`好了，大致就是这三种方式，第二种和第三种用到的比较多。
\#\#\#\#dblclick的使用方式
`$('#btn').dblclick(function() {
    $('p').html('按钮被点击了!');
});`\#\#\#2.mousedown和mouseup
前面的click方法是由这两个方法组成的，即一个click的事件是由鼠标按下和鼠标释放两个事件构成的。但是这里需要注意一些问题。我们讲click事件的完整性，是讲在某个元素上先后完成mousedown和mouseup，一般没有时间限制。但是，如果在元素上按下鼠标，不释放，然后拖动鼠标离开这个元素，虽然在理论上这仍然是一个mouseup事件，可是事件并不是在此元素上触发的，所以认为这个对于此元素来讲click事件是不完整的，也就是不予触发click事件方法。讲明白这点，接下来的知识才不会冲突:任何的鼠标按下的操作都是mousedown事件，任何鼠标释放的操作都是mouseup事件。好了，至于其他的更晦涩难懂但是不影响使用的书面语我就不赘述了。
还有一个知识点是需要说一下的，可以用event 对象的which区别按键，敲击鼠标左键which的值是1，敲击鼠标中键which的值是2，敲击鼠标右键which的值是3。如下面的例子：
`$("button:eq(0)").mousedown(function(e) {     
alert('e.which: ' + e.which)  
})`\#\#\#3.mousemove
此方法和前面的方法在使用上没有任何区别，所以这里不再演示。但是有两个问题需要注意：mousemove事件是当鼠标指针移动时触发的，即使是一个像素；如果处理器做任何重大的处理，或者如果该事件存在多个处理函数，这可能造成浏览器的严重的性能问题。
\#\#\#4.mouseover和mouseout
用来监听用户的移入移出操作，相当于JS中的onmouseover()与onmouseout()事件监听方法。使用方法没有特别之处
\#\#\#5.mouseenter和mouseleave
这两个事件方法就比较特别了，大部分情况下使用出来的效果不会有任何的区别。什么事件冒泡鬼的，我测试了n多的例子，最后还是发现慕课网的例子可以看出区别。书面都说在子元素上的mouseenter和mouseleave事件不会触发父元素的相关方法，但是我的测试全都是会触发的。气的我的脸都是绿的。慕课网的老师给出的例子是这样的：
`<body>
    <h2>.mouseover()方法</h2>
    <div class="left">
        <div class="aaron1">
            <p>鼠标离开此区域触发mouseover事件</p>
            <a>mouseover事件触发次数：</a><br/>
            <a>mouseover冒泡事件触发次数：</a>
        </div>
    </div>
 
    <h2>.mouseenter()方法</h2>
    <div class="right">
        <div class="aaron2">
            <p>鼠标进入此区域触发mouseenter事件</p>
            <a>mouseenter事件触发次数：</a><br/>
            <a>mouseenter冒泡事件触发次数：</a>
        </div>
    </div>
    <br/>
   
    
    <script type="text/javascript">
 
        var i = 0;
        $(".aaron1 p").mouseover(function(e) {
            $(".aaron1 a:first").html('mouseover事件触发次数：' + (++i))
        })
 
        var n = 0;
        $(".aaron1").mouseover(function() {
            $(".aaron1 a:last").html('mouseover冒泡事件触发次数：' + (++n))
        })
 
 
    </script>
 
 
    <script type="text/javascript">
 
        var i = 0;
        $(".aaron2 p").mouseenter(function(e) {
            $(".aaron2 a:first").html('mouseenter事件触发次数：' + (++i))
        })
 
        var n = 0;
        $(".aaron2").mouseenter(function() {
            $(".aaron2 a:last").html('mouseenter冒泡事件触发次数：' + (++n))
        })
 
    </script>
 
 
</body>`如果在p元素与div元素都绑定mouseover事件，鼠标在离开p元素，但是没有离开div元素的时候，触发的结果:
1、p元素响应事件
2、div元素响应事件
这里的问题是div为什么会被触发？ 原因就是事件冒泡的问题，p元素触发了mouseover，他会一直往上找父元素上的mouseover事件，如果父元素有mouseover事件就会被触发，所以在这种情况下面，jQuery推荐我们使用 mouseenter事件，mouseenter事件只会在绑定它的元素上被调用，而不会在后代节点上被触发。
然后我还是很疑惑，满头的雾水。╮(╯▽╰)╭，算了，既然不是大牛，先不要纠结这么多了。以后不出问题就没事，出问题了就换另外一种方法试试，二者既然都有自己的独特的地方，那就有它们最适用的地方。
\#\#\#6.hover
hover的底层封装了mouseenter和mouseleave，即一个方法完成移入移出一套动作的监听。但是你会发现在许多情况下你用mouseover和mouseout重写这个方法，达到的效果是一样的，但是注意了，细微的差别不容忽视，还有就是项目做大了会出现很多问题的，所以建议多使用mouseenter和mouseleave。hover的使用方法如下：
`$("#div2").hover(
function() {
$(this).css('background-color', 'pink');}, 
function() {
$(this).css('background-color', '#bbffaa');}
);`\#\#\#7.focusin和focusout
这两个方法监听光标的聚焦和失焦事件，对，没错，在输入框输入文本的地方用的最多。下面是我自己写的一个小例子。
`<!DOCTYPE html>
<html lang="en">
 
<head>
    <meta charset="UTF-8">
    <title>jQuery鼠标事件之focusin事件和focusout事件</title>
    <script src="../jquery_js/jquery-3.2.0.min.js"></script>
    <style type="text/css">
    label,
    input {
        font-family: "微软雅黑";
    }
    
    input {
        color: #AFABAB;
        border: 1px solid blue;
    }
    </style>
</head>
 
<body>
    <div>
        <label>输入用户名:</label>
        <input type="text" id="name" value="饶舸璇">
    </div>
    <script type="text/javascript">
    // 鼠标聚焦到输入框的时候，字体颜色变为黑色，输入框边框变红
    $("#name").focusin(function() {
        $(this).css({
            color: 'black',
            border: '1px solid red'
        });
        $(this).val('');
    });
    // 鼠标移出输入框
    $("#name").focusout(function() {
        $(this).css({
            color: '#AFABAB',
            border: '1px solid blue'
        });
        $(this).val('CSDN');
    });
    </script>
</body>
 
</html>`\#滚动事件
\#\#\#1、获取和设置元素的尺寸
`width()、height()    获取元素width和height  
innerWidth()、innerHeight()  包括padding的width和height  
outerWidth()、outerHeight()  包括padding和border的width和height  
outerWidth(true)、outerHeight(true)   包括padding和border以及margin的width和height`\#\#\#2、获取元素相对页面的绝对位置
`offse()`\#\#\#3、获取可视区高度
`$(window).height();`\#\#\#4、获取页面高度
`$(document).height();`\#\#\#5、获取页面滚动距离
`$(document).scrollTop();  
$(document).scrollLeft();`\#\#\#6、页面滚动事件
`$(window).scroll(function(){  
    ......  
})`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

