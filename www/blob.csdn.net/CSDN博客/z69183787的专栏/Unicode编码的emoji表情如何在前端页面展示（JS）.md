# Unicode编码的emoji表情如何在前端页面展示（JS） - z69183787的专栏 - CSDN博客
2017年08月25日 14:23:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：18345
前言：项目中用到了emoji表情，后端传递数据时直接是以Unicode形式，在页面总是无法展示，找尽各种方法总算是试出了一种，虽然达到效果但是并不是特别理解其中的原理并且无比笨拙，贴在这用作笔记，如果哪位大神有更简洁的方法希望指导一下。anugular
 js
其中展示方法分为两种：
- 例：某Unicode编码的emoji表情为U+1F62C，取出其中的1f62c部分，拼接成😬这种形式（包括其中的";"）就可以直接在页面展示。有时则需要利用到trustAsHtml()[1]写一个filter来处理即可。 
- 利用String.fromCharCode()方法
 　　例：某Unicode编码的emoji表情为U+1F62C，取出其中的1f62c部分，拼接成0x1f62c，将得到的结果做参数传进下述方法[2]：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
function findSurrogatePair(point) {
  // assumes point > 0xffff
  var offset = point - 0x10000,
      lead = 0xd800 + (offset >> 10),
      trail = 0xdc00 + (offset & 0x3ff);
  return [lead.toString(16), trail.toString(16)];
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
得到一个数组：
![](http://images2015.cnblogs.com/blog/1013351/201612/1013351-20161219144405353-1964611657.png)
给该数组中的两项前面拼接上"0x",得到一个新的数组
```
["0xd83d", "0xde2c"]
```
接下来将数组中的两项直接传递进String.fromCharCode()方法中得到的结果双向绑定到界面即可显示出对应的emoji表情。
要注意的一点：如果该Unicode编码的emoji表情U+后面是四位，例如：U+2600或者U+26c4，则需要省去调用findSurrogatePair()方法这一步，即以U+2600为例，取到2600，拼接上0x得到0x2600，直接调用String.fromCharCode()方法得到的结果双向数据绑定到界面即可
