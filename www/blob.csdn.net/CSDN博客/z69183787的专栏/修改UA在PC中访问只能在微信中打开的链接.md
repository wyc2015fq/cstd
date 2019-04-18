# 修改UA在PC中访问只能在微信中打开的链接 - z69183787的专栏 - CSDN博客
2016年08月20日 13:35:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4391
通常一般在chrome下修改 UA 为 微信或qq 就能打开 那些在js端做了限制的 链接，
但今天发现 还有一些 js 限制了 navigator.platform，pc上的值为 WIN32 或 MAC 导致 环境判断失败。
UA：
安卓QQ内置浏览器UA:
Mozilla/5.0 (Linux; Android 5.0; SM-N9100 Build/LRX21V) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/37.0.0.0 Mobile Safari/537.36 V1_AND_SQ_5.3.1_196_YYB_D QQ/5.3.1.2335 NetType/WIFI
安卓微信内置浏览器UA:
Mozilla/5.0 (Linux; Android 5.0; SM-N9100 Build/LRX21V) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/37.0.0.0 Mobile Safari/537.36 MicroMessenger/6.0.2.56_r958800.520 NetType/WIFI
IOS内置QQ浏览器UA:
Mozilla/5.0 (iPhone; CPU iPhone OS 7_1_2 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Mobile/11D257 QQ/5.2.1.302 NetType/WIFI Mem/28
IOS微信内置浏览器UA:
Mozilla/5.0 (iPhone; CPU iPhone OS 7_1_2 like Mac OS X) AppleWebKit/537.51.2 (KHTML, like Gecko) Mobile/11D257 MicroMessenger/6.0.1 NetType/WIFI
Platform修改方法：
Object.defineProperty(navigator,'platform',{get:function(){return 'Android';}});
页面载入前 console 中敲入，可通过断点在页面onload前 敲入。
