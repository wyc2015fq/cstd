
# linux chromium could not load shockwave flash解决办法！ - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月06日 13:35:00[Jlins](https://me.csdn.net/dyllove98)阅读数：3237


这个问题困扰了我一个小时啊，各处百度都得不到结果啊！（这是goole禁用到无奈啊！）
在解决问题之前，给大家梳理下两个知识点
1.adobe 官网对linux系统的flash支持最终版是11.2，以后都不会提供高版本了，但是会安全保持更新。以下是官网解释。[http://get.adobe.com/cn/flashplayer/](http://get.adobe.com/cn/flashplayer/)
\#注意:: Adobe Flash Player 11.2 将是支持 Linux 平台的最后一个版本。Adobe 将继续为 Flash Player 11.2 for Linux 提供安全更新。
2.chromium中的到flash插件有两个，chromium自带的11.9版本（或者更高），自己安装到11.2版本。ps：说到这里想必大家都已经知道原因了吧！
解决办法：
chromium地址栏：chrome://plugins ---> details--->adobe  flash player --->disable 高版本的，enbale 11.2版本，之后重启chromium就可以了。
ps：
我还发现chromium自带的高版本flash在其指定的路径下是没有对应到flash文件的，所以自然就加载不了。（这个路径在detail中给出的。）



