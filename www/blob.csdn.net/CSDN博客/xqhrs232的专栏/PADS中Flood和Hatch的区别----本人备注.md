# PADS中Flood和Hatch的区别----本人备注 - xqhrs232的专栏 - CSDN博客
2013年07月17日 13:44:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1466
原文地址：：[http://www.eda365.com/thread-156-1-1.html](http://www.eda365.com/thread-156-1-1.html)
flood比较正确的说法应该叫灌铜，是指对用（Copper Pour)画幅出来的闭合区域根据设定规则进行铺铜的一个动作。而铺铜是指用Copper手动画铜皮。而对于Flood和Hatch的区别，在帮助中可以找到：4 z9 y% L- ^7 g- s1 f! v0 ~9 L
Flooding recalculates the pour area and recreates all clearances for the current obstacles within the pour outline, observing clearance rules. Hatching refills (with hatch lines) existing pour polygons for the current session; it does not recalculate
 the pour area. Each time you open a design file, you must flood or hatch the design; this information is not saved. In most cases, you can simply Hatch. Use Flood if you make changes to the copper pour polygon that could create clearance violations or if you
 change clearance rules.  n! b& e% t# E  g6 _6 w+ l  h
$ M' |0 L5 K( Y+ i+ I8 ~Flooding会重新计算灌注区域并重新计算当前填灌区域的外形线内障碍的所有间距，和一些注意的间距规则。Hatching则用来（用填充线）重新填充当前会话内已经存在的填灌多边形，而并不会重新计算填充填灌区域。每次打开一个设计文件时，你应当对这个设计进行flood或hatch；这些信息是不保存的。大部份情况下，你只要简单的Hatch一下就够了。当你对灌铜多边形的修改会引起规则冲突时，或当你修改了间距规则时，请使用flood。
# 1、pads layout灌铜命令中的flood和hatch有什么区别么？
# [http://zhidao.baidu.com/question/354551309.html](http://zhidao.baidu.com/question/354551309.html)
比较好理解的方法可以这样理解:flood是用来重新给PCB板灌铜或刚LAY好的板子灌铜.而hatch是用来恢复灌铜,因为PADS当你关提已铺好铜的PCB文件,再重新打开PCB时,看到的文件是看不到铜了,只有铜框,当你想看到你以前铺好铜的文件只要点中hatch就能恢复你以前铺铜的状态.比如你发一个PCB文件给你老大检查你的PCB看合不合格,你老大为了在不变动你的文件,当然包括也不改变你的铺铜方式,你老大只要点一下hatch这个功能,文件就恢复为你铺好铜的PCB完整文件.2、[http://jinwei976.blog.163.com/blog/static/127866253201001615324711/](http://jinwei976.blog.163.com/blog/static/127866253201001615324711/)3、[**用pads的用户请注意PADS中Flood和Hatch的区别**](http://www.amobbs.com/thread-5530241-1-1.html)[http://www.amobbs.com/thread-5530241-1-1.html](http://www.amobbs.com/thread-5530241-1-1.html)Flooding会重新计算灌注区域并重新计算当前填灌区域的外形线内障碍的所有间距，和一些注意的间距规则。 
Hatching则用来（用填充线）重新填充当前会话内已经存在的填灌多边形，而并不会重新计算填充填灌区域。每次打开一个设计文件时，你应当对这个设计进行flood或hatch；这些信息是不保存的。大部份情况下，你只要简单的Hatch一下就够了。当你对灌铜多边形的修改会引起规则冲突时，或当你修改了间距规则时，请使用flood。
(以上摘自网络）
电路板工厂是不会去改变你的铺铜规则，也就是不会去用flooding进行铺铜  都只是对你的规则进行填充，这是最简单的道理，而有很多的工程师铺铜之后，又进行了线路修改，而没有进行重新flooding ，就提交给电路板工厂，而电路板工厂只是对hatching进行铺铜，这样的结果就会直接导致你的线路板出现问题！总是有个别工程师出现这类问题！ 
//==============================================================备注：1》Flood一般是在PCB Layout完成后第一次使用，或者PCB有改动(比如设计规则)才重新使用Flood来一遍；Flood完成后如果没有重大的改动要显示覆铜一般用Hatch命令就好！
2》Flood好的PCB保存后，一般都不会保存覆铜，只会保存有铜框，所以你打开一个覆铜好的PCB文件一般要进行Hatch一遍才可以将覆铜好的区域显示出来
3》Tools/Pour Manager/Flood++++Tools/Pour Manager/Hatch这两个页面下的Start按钮执行的时间是大不一样的；Tools/Pour Manager/Flood/Start执行起来要好久，
而Tools/Pour Manager/Hatch/Start按钮一下就执行完成了，
Flood是重新来一遍(会重新计算灌注区域并重新计算当前填灌区域的外形线内障碍的所有间距，和一些注意的间距规则),
Hatch则用来（用填充线）重新填充当前会话内已经存在的填灌多边形，而并不会重新计算填充填灌区域。
4》每次打开一个设计文件时，你应当对这个设计进行flood或hatch；这些信息是不保存的。
大部份情况下，你只要简单的Hatch一下就够了。当你对灌铜多边形的修改会引起规则冲突时，或当你修改了间距规则时，请使用flood。
