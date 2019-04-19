# 报告太君，来了一窝IE - 左直拳的马桶_日用桶 - CSDN博客
2011年05月13日 15:41:00[左直拳](https://me.csdn.net/leftfist)阅读数：1495
我操你，微软，我操你姥姥。
做网站的，浏览器兼容问题，其实是对IE6来说的，别的浏览器没有问题，但在IE6这里往往大相径庭，让人大吃一斤。
但是，有些东西，在IE6/7没有问题，到了IE8又有问题！而其他非IE系列浏览器是好好地。什么毛病？
狗日的，搞得烂就算了，还来了6、7、8、9一窝子，嫌一个还不够将我们折磨疯？
//微软，我操你姥姥
var isIE = !!window.ActiveXObject;
var isIE6 = isIE && !window.XMLHttpRequest;
var isIE8 = isIE && !!document.documentMode;
var isIE7 = isIE && !isIE6 && !isIE8;
