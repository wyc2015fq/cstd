# Matlab设置Legend横排、分块 - fighting！！！ - CSDN博客
2018年09月08日 09:50:51[dujiahei](https://me.csdn.net/dujiahei)阅读数：2162
|12345678910111213|`高级用法1：指定``legend``显示的位置：``legend``({``'str1'``,``'str2'``,``'strn'``},``'Location'``,``'SouthEast'``);``比较鸡肋，画好图后树手动拖动就好了``高级用法2：指定显示某几条曲线的``legend``：``例如你有25条曲线，想显示其中1，6，11，16，21的``legend``H = ``plot``(data);``legend``(H([1 6 11 16 21],``'1,'``6``','``11’,``'16'``,``'21'``);``高级用法3：``legend``横排``hl = ``legend``(H([1 6 11 16 21],``'1,'``6``','``11’,``'16'``,``'21'``);``set``(hl,``'Orientation'``,``'horizon'``)``高级用法4：不显示方框：``hl = ``legend``(H([1 6 11 16 21],``'1,'``6``','``11’,``'16'``,``'21'``);``set``(hl,``'Box'``,``'off'``);`|
　　查看这个的初衷是因为四条曲线的对比图，但是有些曲线差别太大，比如A在[1.8,1.82]，B在[0.9,1.1]，CD曲线在中间这样的话，因为A曲线的存在，中坐标最大值是1.82但是变化很不明显，所以该曲线在图的最上方，而且紧挨着图的上边界，因为图例在右上方，这样肯定会覆盖曲线，所以想着横排。
![](https://images2015.cnblogs.com/blog/387014/201604/387014-20160408103658937-806623975.png)
　　但是这种方法也不好做好的事拆开。
　　Matlab提供的legend函数，给出的legend经常覆盖了某些曲线（尤其是用IEEE模板的时候，一般图片宽度不超过315px).
|123456789|`a=``linspace``(0,2*``pi``,100);``y1=100*``sin``(a);``y2=50*``cos``(a);``y3=``tan``(a);``y4=``log``(a);``y=[y1;y2;y3;y4];``figure``p=``plot``(a,y)``legend``(``'sin'``,``'cos'``,``'tan'``,``'log'``)`|
　　如下图所示：
![Matlab画图实现两个或多个legend,不同排版](http://www.ilovematlab.cn/data/attachment/forum/month_1010/20101004_a428e57c016645fcdc92yPgqjvPLcVAe.jpg)
　　Matlab画图实现两个或多个legend,不同排版
 　　这显然不是你想要的，你的reviewers也会肯定让你修改的！
*　　原因是Matlab的Legend函数，只能水平或者竖直排版！*
**　　解决方法：**把legend分成几个，相对独立，这样可以使用鼠标随意移动，确保不遮挡曲线。
　　结果如下图所示：
![Matlab画图实现两个或多个legend,不同排版](http://www.ilovematlab.cn/data/attachment/forum/month_1010/20101004_23f595e0c55a6d57681epjZV3stge9js.jpg)
　　位置可以随便写，图绘制好厚，手动拖动。
|12345678910111213|`a=``linspace``(0,2*``pi``,100);``y1=100*``sin``(a);``y2=50*``cos``(a);``y3=``tan``(a);``y4=``log``(a);``y=[y1;y2;y3;y4];``figure``p=``plot``(a,y)``legend``(p(1:2),``'sin'``,``'cos'``);``ah=``axes``(``'position'``,``get``(``gca``,``'position'``),...``            ``'visible'``,``'off'``);``legend``(ah,p(3:4),``'tan'``,``'log'``,``'location'``,``'west'``);`|
