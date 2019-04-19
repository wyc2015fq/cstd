# Protel99分割内电层 - xqhrs232的专栏 - CSDN博客
2014年04月18日 23:38:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1276
原文地址::[http://blog.csdn.net/augusdi/article/details/17640619](http://blog.csdn.net/augusdi/article/details/17640619)
PROTEL99的电性图层分为两种，打开一个PCB设计文档按，快捷键L，出现图层设置窗口。左边的一种（SIGNAL LAYER)为正片层，包括TOP LAYER、BOTTOM LAYER和MIDLAYER，中间的一种(INTERNAL PLANES)为负片层，即INTERNAL LAYER。这两种图层有着完全不同的性质和使用方法。
      正片层一般用于走纯线路，包括外层和内层线路。负片层则多用来做地层和电源层。因为在多层板中的地层和电源层一般都是用整片的铜皮来作为线路（或做为几个较大块的分割区域），如果用MIDLAYER即正片层来做的画则必须用铺铜的方式来实现，这样将使整个设计数据量非常大，不利于数据交流传递，且会影响设计刷新速度。而用负片则只需在外层与内层的连接处生成一个花孔（THERMAL PAD）即可，对于设计和数据传递都非常有利。
      内层的添加与删除
      在一个设计中，有时会遇到变换板层的情况。如把较复杂的双面板改为四层板，或把对信号要求较高的四层板升级为六层板等等。这时需要新增电气图层，可以如下*作：
DESIGN-LAYER STACK MANAGER,在左边有当前层叠结构的示意图。点击想要添加新层位置的上面一个图层，如TOP,然后点击右边的ADD LAYER(正片）或ADD PLANE(负片），即可完成新图层的添加。
      注意如果新增的图层使PLANE(负片）层的话，一定要给这个新层分配相应的网络（双击该层名）!这里分配的网络只能有一个（一般地层分配一个GND就可以了），如果想要在此层（如作为电源层）中添加新网络，则要在后面的操作中做内层分割才能达到，所以这里先分配一个连接数量较多的网络即可。
      如点击ADD LAYER则会新增一个MIDLAYER（正片），应用方法和外层线路完全相同。
      如果想应用混合电气层，即既有走线又有电源地大铜面的方法，则必须使用ADD LAYER来生成的正片层来设计（原因见下）。
   内电层的分割
   如果在设计中有不只一组电源，那可以在电源层中使用内层分割来分配电源网络。这里要用到的命令是：
PLACE-SPLIT PLANE,在出现的对话框中设定图层，并在CONNECT TO NET处指定此次分割要分配的网络，然后按照铺铜的方法放置分割区域。放置完成后，在此分割区域中的有相应网络的孔将会自动生成花孔焊盘，即完成了电源层的电气连接。可以重复操作此步骤直到所有电源分配完毕。当内电层需要分配的网络较多时，做内层分割比较麻烦，需要使用一些技巧来完成。
      此处还需要注意一个问题：PROTEL中有两种大铜皮的电气连接方式（不包括PLACE FILL),一种为POLYGON PLANE,即普通的覆铜，此命令只能应用于正片层，包括TOP/BOT/MIDLAYER，另一种为SPLIT PLANE,即内电层分割，此命令只能应用于负片层即INTERNAL PLANE。应注意区分这两个命令的使用范围。
      修改分割铺铜的命令：EDIT-MOVE-SPLIT PLANE VERTICES

protel四层板及内电层分割入门
**一、准备工作**    新建一个DDB文件,再新建相关的原理图文件, 并做好相关准备设计PCB的准备工作，这个相信想画四层板的朋友都会, 不用我多讲了。
**二、新建文件**
    新建一个PCB文件, 在KeepOutLayer层画出PCB的外框, 如下图，用过Protel的朋友们应该都会。
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PUK32910.jpg)
**三、设置板层**    在PCB界面中点击主菜单Design 再点击Layer Stack Manager 如图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PUS06461.jpg)
    点击后弹出下面的层管理器对话框, 因为在Protel中默认是双面板，所以，我们看到的布线层只有两层。
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PUU2M06.jpg)
    现在我们来添加层，先单击左边的TopLayer, 再单击层管理器右上角的Add Plane按钮，添加内电层，这里说明一下，因为现在讲的是用负片画法的四层板，所以，需要添加内电层，而不是Add Layer。
    单击![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ12K343.jpg) 后，将在TopLayer的下自动增加一个![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ1521596.jpg) 层,双击该层，我们就可以编辑这一层的相关属性，如下图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ02QX4.jpg)
    在Name对应的项中，填入VCC，点击确定关闭对话框，也就是将该层改名为VCC，作为设计时的电源层。
    按同样的方法，再添加一个GND层。完成后如图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ222NW.jpg)
四、导入网络
    回到原理图的界面，单击主菜单Design ==> Update PCB如图:
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ24R152.jpg) =>![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ30BV0.jpg)
选择要更新的PCB文件，点击Apply ，
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ3243B1.jpg)
再点击左边的![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ3504295.jpg) ，查看我们在原理图中所做的设计是否正确。
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ42D3T.jpg)
这里，我们把![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ4113551.jpg) 项打上勾，只查看错误的网络。
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ445a51.jpg)
在这里，我们没有发现有任何错误网络时，可以单击![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ510VC.jpg) 将网络导入PCB文件了。
    这种导入网络的方法是Protel的原理图导入网络到PCB的一个很方便的方法，不用再去生成网络表了。同时，修改原理图后的文件，也可用此方法快速更新PCB文件。
**五、布局**
    由于这个基本大家都会，所以省略了,完成后如图:
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ6045454.jpg)
**六、设置内电层**
    我们再执行主菜单Design 下的Layer Stack Manager 弹出层管理器，
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZA04316.jpg)
双击VCC层，在弹出的对话框中，在Net name 的下拉对话框中选择VCC网络，给这一层真正定义为VCC网络，之前的只是取个VCC的名称而已，与VCC网络相同的元件管脚及过孔，均会与该层自动连接，从而不用布线。
    用同样的方法给GND层定义网络，将其定义为GND网络。点击OK关闭对话框。这时，我们发现，在PCB中，有些元件的PAD的中心有一个十字，这是因为，这个焊盘的网络是VCC或者GND，说明已经与相对应的网络连通。如图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZHS4R.jpg)
在图中，焊盘上的十字架的颜色就代表相对应连接内电层的颜色。如：内电层GND为棕色，则焊盘的十字也为棕色。
**七、布线**
[800字…………省略]
**八、内电层分割**
    当Top Layer与BottomLayer层没有足够的空间来布信号线时，而又不想增加更多的信号层, 我们就需要将这些信号线走在内电层上，做法如下：
    先确定要走在内电层的网络，再单击主菜单Place, 选择![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZZWB9.jpg) 项。如图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ92R323.jpg)
弹出分割内电层的对话框，如图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0PZ9455132.jpg)
在Connect to Net中选择一个要布在内电层的网络，这里，假如，我们选AA1网络，在![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P910136119.jpg) 中，我们设置为0.5mm,
 这就是在内电层中与其它网络的距离。这个尽量设大一点，至少在0.3mm以上，一般为0.5mm，因为，在内层中，间距太小会导致生产时不良率较高。在Layer中，我们选VCC, （ 建议不要在地层做内层分割，因为，在设计时，尽量保持的完整性，提高抗干扰能力。）
    设置好后点OK，再在PCB中找到AA1网络的PAD处画线，将要布在该层的PAD或VIA包围起来，形成一个闭合的多边形。（注意，这个线在内层中，是无铜区域，也就是在顶层的线与线中的间隙，所以，不能画到与焊盘重叠）如图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P911259131.jpg)
    这时，网络为AA1的PAD中心也多了个十字架，说明，AA1网络已经从VCC层将这里焊盘连接起来了，注：千万注意，在内电层中不要再对这些焊盘进行走线了。
**九、看内电层**
    在PCB内层设置完毕后，我们要来查看内层是否正确，当然，最先是用眼睛来查看整个板了。下面，我们来看看下面这个图:
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P9120B094.jpg)
    前面已经讲过，带十字PAD是已经和内电层对应网络连接OK的，我们要怎么查看这个PAD是否有铜铂连接呢？ 其实只要想象力丰富一点的人，就会知道，PCB中负片做法：就是有画线的地方就是没有铜铂的区域，那么，在上图中，黑色的区域在做出PCB板后，就是有铜铂的区域了，现在，我们可以用一种简单的方法去查看。
    首先，在PCB界面中，点击主菜单Tool(T下的选项![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P9142S443.jpg) (或按T
 、P快捷键），打开参数设置对话框:
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P915106306.jpg)
然后、单击![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P91453N57.jpg) 颜色选项卡，打开颜色设置对话框，如图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P91533NG.jpg)
    假如我们要查看的是VCC内电层，这里，我们将![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P91605D54.jpg) 的颜色设置为深一点的颜色，如棕黑色；再将背景色![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P916243223.jpg) 设为平常的红色，单击OK关闭对话框。
    再次，我们关掉其它的一些不需要的层，如：丝印层等，方便我们查看PCB，下面我们再来看一下PCB文件，如图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P916494110.jpg)
    在图中，红色区域就是有铜铂的地方（也就是内电层中没有画线之处），在棕黑色的区域，就是无铜铂区（就是内电层中有画线的区域），这样，这块板就像被铺了铜的PCB文件一样了。
    PCB的最后文件，如图：
![分割内电层](http://www.dzjs.net/upimg/userup/0901/0P91GOL4.jpg)
哈哈，一块四层板完成了!!!
————————本文来自：[http://www.dzjs.net/html/PCBjishu/2009/0108/3598_2.html](http://www.dzjs.net/html/PCBjishu/2009/0108/3598_2.html)
[http://bianmaozhan.blog.163.com/blog/static/147749952010282436477/](http://bianmaozhan.blog.163.com/blog/static/147749952010282436477/)
