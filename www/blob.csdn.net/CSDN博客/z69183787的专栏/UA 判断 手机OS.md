# UA 判断 手机OS - z69183787的专栏 - CSDN博客
2017年01月04日 14:32:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1710
var ua = navigator.userAgent,
isAndroid = /android/ig.test(ua),
isIos = /iphone|ipod|ipad/ig.test(ua),
isMicroMessenger = /MicroMessenger/ig.test(ua);//判别是否为微信
