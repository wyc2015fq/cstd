# 解决Eclipse项目图标上的红色感叹号，红叉 - z69183787的专栏 - CSDN博客
2014年01月16日 19:55:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6738
从外部导入了几个jar包发现工程上出现红叉，但展开又没有详细的指明任何错误，可能过如下几步： 
1.检查Build Path中的各个依赖jar及library是否有warning的标志，若有，处理之； 
2.clean 工程； 
3.右键找到pde tools下的update classpath; 
**4.一般有什么问题都会在视图Problems中显示，所以可以在其中查找问题，今天我遇到的就是A cycle was detected in the build path of project ，这样我们可以如下**
引用
window->preference->java->complier->building->build path problems->cirluar dependency
设置为warning就可以了。
