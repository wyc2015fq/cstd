# JS中获取对象的绝对位置 - 左直拳的马桶_日用桶 - CSDN博客
2006年11月03日 15:34:00[左直拳](https://me.csdn.net/leftfist)阅读数：6369
JS中获取对象的绝对位置
左直拳
网页中，可以用[object].offsetLeft和[object].offsetTop来获得该对象（object）与其所在容器中的水平及垂直偏移量。
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
