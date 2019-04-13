
# DOT + graphviz 轻松画图神器 - bitcarmanlee的博客 - CSDN博客


2017年09月20日 09:49:32[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1429


`本人是个不会画图的屌丝，一方面是因为需要画图的地方很少，另一方面也是因为即使需要画图，也都是很简单的几个框几个圈几条线，所以就用word之类的原始工具直接上了，结果往往是痛苦的经历换来搓搓的一张图，而且图很难修改，遇到需求变更时往往牵一发而动全身，所以每次听说画图，总有背后一凉的感觉。最近经同事介绍，了解了一下DOT和graphviz，一看到这东西就感觉遇到了真爱，简单的几行代码，轻松搞定画图，不用担心布局，不用担心修改，而且告别烦人的鼠标，确实让人眼前一亮。`
## 一、简介DOT & graphviz
DOT
DOT是一种文本图形描述语言。DOT语言文件通常具有.gv或是.dot的文件扩展名。当然，在编写好.dot或者.gv的文件之后，需要有专门的程序处理这些文件并将其渲染成为图片，dot就是其中一款程序，它可以将DOT语言描述的图形渲染成.png、.jpg、.pdf等多种类型。
当然，作为工具，dot本身是很原始的，就像gcc之于c代码，g++之于cpp代码一样，或许某些程序员会热衷于在终端使用这些工具，但也有很多人喜欢交互式的界面，所以就有了gvedit之类的工具，它提供交互式的窗口来使用dot等工具渲染DOT语言描述的图形。
graphviz
graphviz是一个开源软件包，上述dot和gvedit等工具都在该软件包中。
所以，不妨简单的认为DOT是一门图形描述语言而graphviz是处理该语言文件的一个集成化的工具。
DOT & graphviz的局限性
graphviz中有很多工具可以将DOT语言的文本渲染成为图片，但正如我们所见，我们在享受方便的编码的同时，将图片的布局等任务交给了这些工具，虽然这些工具有很不错的布局算法支持，但仍不一定能满足我们的要求，所以当对图片的布局有特殊要求时，DOT & graphviz就显示出了它的局限性。当然，我们可以再使用其他图片编辑器校正DOT语言生成的图片，但这种时候，DOT & graphviz的方便性或许早就消失殆尽了。
什么人适合使用DOT & graphviz
就我个人体会而言，DOT & graphviz适合这些人使用：
1> 像我一样的画图小白并且喜欢操作键盘远胜于鼠标；
2> 没有熟练的掌握其他作图工具；
3> 对图片布局等没有特殊要求；
4> 要绘制的是流程图结构图之类的图而不是画小狗小猫山山水水。
二、使用DOT & graphviz
环境配置
graphviz的官网（[http://www.graphviz.org](http://www.graphviz.org)）上可以下载适用于多个OS的graphviz版本，包括Linux的多个发行版（Fedora、Ubuntu等）、Solaris、Windows、Mac等，下载对应版本安装即可。Windows下安装时，官网有提示需要手动配置环境变量。
开始战斗
完成了安装之后，就可以编写DOT文本并用graphviz下的工具渲染图片了。
打开gvedit，新建一个.gv或者点.dot的文件并输入DOT文本，在工具栏graph下选择layout（快捷键f5）即可在窗口中看到图片，graph下选择settings（快捷键shift+f5）可以进行设置，在设置里也可以看出有多种处理DOT文本的工具可以选择（默认dot），也可以选择多种导出的文件类型（默认.png）。
你还可以在终端直接调用dot命令处理文本并生成图片（again：需要配置环境变量）。以把test.dot导出为test.png为例，命令为：
![这里写图片描述](https://img-blog.csdn.net/20170920094452313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
dot-Tpng -o test.png test.dot该命令会在当前目录下生成test.png。
三、DOT语法
基本语法
graph（无向图）或者digraph（无向图）表示图，然后｛｝中的内容是对图的描述，注释风格和C类似（“//”用于单行注释，/**/用于多行注释）。如一个无向图：
graph graph1 {//无向图graph1a-- b          //节点a和b之间连线}节点
DOT中，节点可以不用声明直接使用。每个节点首次出现的名称做为该节点的唯一标识。
属性设置：对节点可以设置的常见通用属性有shape、label、style、color、fillcolor、rank等，对于不同的形状，可能还有不同的属性可以设置，如对于多边形可以设置边数等。节点属性设置时，node用于设置默认属性（对设置位置之后的点有效），在点后面用[]设置单独一个点的属性。
graph node_settings {  
    node [shape ="box", style ="filled", color ="black", fillcolor ="green"]//设置节点的默认形状，类型，颜色，填充颜色a[shape ="ellipse", color ="red", label ="this is a"]//设置节点a的颜色，注意默认节点类型是filled，所以这里的color只是设置a的边框颜色b [label ="two\nlines"]//label支持'\n'换行a-- ba-- c     //如果不需要设置c的属性，可以不用声明c而直接使用node [shape ="circle"]  
    d [label ="cicle"]  
    c-- d      //d使用之前最后出现的node设置，形状为circle{rank = samea, d}//设置a和d在同一层}边
边有有向边和无向边两种，无向边用于无向图，有向边用于有向图，不可混用。
属性设置：边的常见设置有style、color、weight、label、labelfontcolor、headlabel、taillabel、decorate等，对于有向边，还可以设置边的起点位置等（用n、e、s、w和相邻字母的组合表示位置）。和节点类似的，边属性设置时，用edge[]设置默认属性，在边之后用[]设置单独一条边的属性。
digraph edge_settings {  
    edge [color="green", decorate = false]//设置边的默认属性node [shape ="polygon", sides =4,color="blue"]  
    a -> b [style ="dotted",color="red", label ="a to b"]//设置style、color、labelb: se -> c: w [headlabel ="end",  taillabel ="start"]//设置边从b的“东南方”出发，从c的“西方”结束，设置有向边起点和重点的labeledge [style ="bond", decorate = true]//设置之后的边加粗并且标签和连线之间有线标注{c, f} -> {d, e} [label ="multi-lines"]//可以用这种方式同时画多条边}图
从前面的例子中已经可以看出，DOT语言可以描述无向图和有向图两种图，graph标识无向图，digraph标识有向图。
属性设置：在一个图的开头写入属性即可设置图形的属性，常用的图形属性有size、label、labelloc、labeljust、bgcolor、rankdir等。
子图，可以进行和“父图”类似的设置，唯一注意的是子图必须以”cluster”做为名称的开始。
下面是实现的官网首页上的图：
digraphgraph_settings {  
    start [shape ="Mdiamond"]  
    end [shape ="Msquare"]  
    subgraph cluster_sub1 {  
        label ="process \#1"labelloc ="t"bgcolor ="gray55"node [style ="filled", color ="white"]  
        a0 -> a1 -> a2 -> a3 -> a0  
    }  
    subgraph cluster_sub2 {  
        label ="process \#2"labelloc ="t"color ="blue"node [style ="filled", color ="black", fillcolor ="gray55"]  
        b0 -> b1 -> b2 -> b3  
    }  
    start -> {a0, b0}  
    a1 -> b3  
    b2 -> a3  
    {a3, b3} -> end  
}语法总结
DOT的语法非常简单，基本保证了随便可以“现炒现卖”，只不过用的越多可能对各种属性越熟悉罢了。具体的属性等可以参见官网的Document：[http://www.graphviz.org/Documentation.php](http://www.graphviz.org/Documentation.php)。
原文链接：[http://blog.csdn.net/stormdpzh/article/details/14648827](http://blog.csdn.net/stormdpzh/article/details/14648827)

