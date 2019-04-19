# Firefox下的坐标 - 左直拳的马桶_日用桶 - CSDN博客
2008年09月24日 16:50:00[左直拳](https://me.csdn.net/leftfist)阅读数：2492标签：[firefox																[ie																[table																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=ie&t=blog)](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
Firefox下的坐标
左直拳
<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />
FireFox下的坐标与IE有所不同。
1、鼠标坐标
要获取鼠标的坐标，离不开事件event这个对象。在IE里，event是全局对象，你可以在任何地方引用event，或者用event.srcElement来找到触发事件的对象。但是FF不行，必须在触发事件调用的函数里，将event作为参数传过去才能用，如onclick(event)。
鼠标当前坐标
IE：event.x和event.y。
FF：event.pageX和event.pageY。
通用：两者都有event.clientX和event.clientY属性。
鼠标当前坐标(加上滚动条滚过的距离) 
IE：event.offsetX和event.offsetY。
FF：event.layerX和event.layerY。
2、样式（style）坐标
IE：style.left，style.top
FF：style.left，style.top
IE：style.posLeft 和 style.posTop 
FF：没有。
3、对象坐标
通用：两者都有[object].offsetLeft 和[object].offsetTop
问题是，在FF（FireFox）中，它们是返回与BODY的偏移，而在IE中，是其所在容器中的水平及垂直偏移。
那么怎样在IE中也能获得与BODY的偏移呢？因为这个偏移才最有用，找到了这两个偏移，就相当于找到了在页面中的绝对位置。
可以变通一下：
var posX = objLookingFor.offsetLeft;
var posY = objLookingFor.offsetTop;
var aBox = objLookingFor;//需要获得位置的对象
do {
aBox = aBox.offsetParent;
posX += aBox.offsetLeft;
posY += aBox.offsetTop;
} while( aBox.tagName != "BODY" );
这样到最后，posX与posY就是与BODY的偏移。
但这也不是绝对的，有例外：例外，就是当该元素位于表格的TD中时，offsetLeft的值是该元素外边界至其所在TD左侧内侧边界的值，也即是说此时元素的offsetLeft表现类似于在IE中。这时获取坐标方法可以为：
//oElement是某对象
var width = oElement.offsetWidth;
var height = oElement.offsetHeight;
if( oElement.offsetParent.tagName == "BODY" || appname == "Netscape")
{
var pos = {x:0,y:0};
pos.x = oElement.offsetLeft;
pos.y = oElement.offsetTop;
var x = 0, y = 0;
while( oElement.offsetParent )
{
//累加从该元素起至父元素的offsetLeft和offsetTop
x += oElement.offsetParent.offsetLeft;
y += oElement.offsetParent.offsetTop;
//遇到table标签则表明累计的量都应该累加到该元素的offsetLeft和offsetTop(Table中按IE方式计算)
if( oElement.offsetParent.tagName.toLowerCase() == "table" )
{
pos.x += x;
pos.y += y;
x = 0;
y = 0;
}
oElement = oElement.offsetParent;
}
return [pos.x,pos.y,pos.x + width,pos.y + height];
}

