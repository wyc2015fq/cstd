# pads pcb中显示/隐藏飞线的方法 - xqhrs232的专栏 - CSDN博客
2013年06月19日 01:12:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：14730
个人分类：[PCB/EDA/硬件设计与仿真](https://blog.csdn.net/xqhrs232/article/category/906926)
原文地址::[http://hi.baidu.com/huxianfangyuan/item/2f4e55f9aa161bc60dd1c81e](http://hi.baidu.com/huxianfangyuan/item/2f4e55f9aa161bc60dd1c81e)
相关网帖
1、
# 以前没用过Pads，现在想画一个板子，为什么里面有的飞线不显示？板子是8层板，我该如何设置？如下图----[http://zhidao.baidu.com/question/518200759.html](http://zhidao.baidu.com/question/518200759.html)
2、pads中网络的连线显示不出来怎么办----[http://zhidao.baidu.com/question/164266055.html](http://zhidao.baidu.com/question/164266055.html)
3、pads中pcb中的网络线有部分隐藏了怎么办。一条网络有部分隐藏了，部分没隐藏。怎么将隐藏的部分显示出来
----[http://zhidao.baidu.com/question/495815185.html](http://zhidao.baidu.com/question/495815185.html)

**pads pcb中隐藏飞线的显示：**
**方法如下：**
1. Setup/Display Colors 弹出颜色设置对话框，
2. 进行设置connection的颜色设置。如下图：
![](http://hiphotos.baidu.com/huxianfangyuan/pic/item/78aa538bf6abe7e2fc1f1053.jpg)

//======================================================
备注：：
1、好像在VIEW/NETS这个对话框进行设置就可以显示出走线的样子
2、Ctrl+Alt+N打开网络显示设置对话框，在右边View List窗口中选中你想显示的网络或全部会中都可以，然后点击下面的单选项All即可显示飞线。你也可以把右边窗口的网络全部移移到左边窗口，这样就没有网络颜色设定，飞线也显示出来了。
3、同理，要把所有的飞线隐藏起来按下面的方法
Ctrl+Alt+N打开网络显示设置对话框，在右边View List窗口中选中你想显示的网络或全部会中都可以，然后点击下面的单选项NONE即可不显示飞线。
