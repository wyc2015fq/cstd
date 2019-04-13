
# MTK 升级USB问题 - 嵌入式Linux - CSDN博客

2015年10月08日 10:17:54[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：645


问题：我们的开发环境是ubuntu里面安装xp ,经常是xp下没有正常识别preload模式下的usb.这样肯定不能升级不了。


设置：MTK preload下的USB　vid:0e8d pid:2000 revion 0100 知道这几个值了，在usb配置里面增加这个筛选项就可以了。



