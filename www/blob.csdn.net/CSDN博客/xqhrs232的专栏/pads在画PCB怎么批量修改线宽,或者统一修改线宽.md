# pads在画PCB怎么批量修改线宽,或者统一修改线宽 - xqhrs232的专栏 - CSDN博客
2016年04月17日 00:05:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5261
原文地址::[http://zhidao.baidu.com/link?url=d1NwswHkHlMEkZFhyagOPKHBD4LKepc5kJW70Gi6mDHutPTzJEx6Kl8c-g3-suui2WOgq1bNsUMGs-MNyEa-7q](http://zhidao.baidu.com/link?url=d1NwswHkHlMEkZFhyagOPKHBD4LKepc5kJW70Gi6mDHutPTzJEx6Kl8c-g3-suui2WOgq1bNsUMGs-MNyEa-7q)

右键-select nets，然后框选整块板子，或者选择你要改变线宽的网络
再右键-Properties...，在弹出的Properties对话框中填入你要的线宽值即可，有可能改线宽会违规，从而造成修改失败，此时，可以按DRO无模式命令，以禁止规则，然后再修改即可
向左转|向右转
![](http://f.hiphotos.baidu.com/zhidao/wh%3D600%2C800/sign=026a74f60a55b3199cac8a737399ae1e/241f95cad1c8a78664be72f96709c93d71cf50b4.jpg)
//================================================================================================================================
备注::
1>不知道这样操作有没问题后遗症！！！
2>同一个网络的走线也可以这样来统一修改！！！
