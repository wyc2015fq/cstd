
# js获取div的坐标 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月15日 22:31:39[Jlins](https://me.csdn.net/dyllove98)阅读数：9791


html中最常使用的控件就是**div**了,那么如何获取div的坐标呢?
如何方法可以实现.
Js代码![收藏代码](http://hw1287789687.iteye.com/images/icon_star.png)
/***
* 获取div的坐标
* @param divObj
* @returns {{width: number, height: number, left: *, top: Window}}
*/
com.whuang.hsj.divCoordinate=function(divObj){
if(typeofdivObj =='string'){
divObj=com.whuang.hsj.
$$
id('divObj');
}
return{'width':divObj.offsetWidth,'height':divObj.offsetHeight,
'x':divObj.offsetLeft,'y':divObj.offsetTop,
'scrollLeft':com.whuang.hsj.getScroll().left,'scrollTop':com.whuang.hsj.getScroll().top};
}
// Cross browser gets the position of scroll
com.whuang.hsj.getScroll=function(){
return{
top:document.documentElement.scrollTop || document.body.scrollTop,
left:document.documentElement.scrollLeft || document.body.scrollLeft
}
}
![](http://dl2.iteye.com/upload/attachment/0103/2036/16f46b8e-0414-39b0-84a2-53d133ba5a83.jpg)
com.whuang.hsj.**divCoordinate**()方法介绍
功能:返回div坐标;
参数:div对象或div的id(字符串);
返回值:对象,有六个属性:
width:div自身的宽度;
height:div自身的高度;
x:div左上角的坐标x;
y:div左上角的坐标y;
scrollLeft:水平滚动条的位置
scrollTop:竖直滚动条的位置
测试页面:
Html代码![收藏代码](http://hw1287789687.iteye.com/images/icon_star.png)
<html>
<headlang="en">
<metacharset="UTF-8">
<title></title>
<scripttype="text/javascript"src="js/jquery-1.10.1.js"></script>
<scripttype="text/javascript"src="js/common_util.js"></script>
<scripttype="text/javascript">

function run(){
varloc=com.whuang.hsj.divCoordinate('divObj');
//            document.writeln();
com.whuang.hsj.
$$
id('text22').innerHTML="width:"+loc.width+" &nbsp; , &nbsp; height:"+loc.height+" &nbsp;, &nbsp;scrollTop:"+loc.scrollTop+" &nbsp;, &nbsp;scrollLeft:"+loc.scrollLeft+" &nbsp;,&nbsp;x:"+loc.x+" &nbsp;,&nbsp; y:"+loc.y;
}
</script>
</head>
<body>
<divstyle="width: 599px;height: 499px;background-color: mediumvioletred;"id="divObj">
</div>
<br>
<inputtype="button"value="run"onclick="run();">
<divid="text22"style="width: 400px;">
</div>
</body>
</html>
运行结果:
![](http://dl2.iteye.com/upload/attachment/0103/2040/a82a49c1-8da5-391f-a487-d908255c9691.jpg)
参考:
http://hw1287789687.iteye.com/admin/blogs/2156296
http://hw1287789687.iteye.com/admin/blogs/2155772
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

