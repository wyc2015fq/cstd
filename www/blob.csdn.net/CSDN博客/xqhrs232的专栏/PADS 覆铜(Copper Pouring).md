# PADS 覆铜(Copper Pouring) - xqhrs232的专栏 - CSDN博客
2014年02月21日 14:21:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3236
原文地址::[http://hi.baidu.com/ken2232/item/b6b8ce4010f15286823ae1f6](http://hi.baidu.com/ken2232/item/b6b8ce4010f15286823ae1f6)
==================================]===
**一个元件，多种外型的切换。**
调出元件属性 Component Properties -> Decal中可选择。
通用快捷键为：alt + 快捷字母
====================================
特别注意：当 copper pour 需要和需要和焊盘连成一片的时候，需要选择Flood over。
但是，焊盘有4种Round/Square?Rectangle/Oval，如果copper pour中同时包含这4种焊盘，则要**分别设置 这 4 种焊盘的 Flood Over 选项**。
![](http://hiphotos.baidu.com/ken2232/pic/item/98b862194134970a5724a58995cad1c8a6865db6.jpg)
====================
绘制pcb时，覆铜的选项中有pour over all same net objects 、do not pour over all same net objects等？
pour over all same net objects:对于相同网络的焊盘、导线以及覆铜全部进行连接和覆盖。do not pour over all same net objects：仅仅对相同网络的焊盘进行连接，其他如覆铜、导线不连接。pour over same net polygons only：仅仅对相同网络的焊盘、覆铜进行连接，其他如导线不连接。
===============
[http://www.baisi.net/thread-1352814-1-1.html](http://www.baisi.net/thread-1352814-1-1.html)
灌铜时如何把相同网络的焊盘都盖住而不是花焊盘连接呀
你可以把Drilled Thermals的Width设置大一点
setup----->preference---->Thermals
1. Drilled Thermals-----> 包含 via 及 dip pad 的設定
2. Non-drilled Thermals----> SMD pad 的設定
3. min spoke--->就是你提到 4 條線連接的設定
4.Pad Shape---->可依焊盤不同形狀做連接方式
5.如你所問,接地 copper 要整個包覆焊盤就需選 Flood over 
===================
======================================================
请问一下，怎么将焊盘全铺上铜
![](http://hiphotos.baidu.com/ken2232/pic/item/ae44980e28381f3005f5d814a9014c086f06f015.jpg)===我选的是flood
 over
如图。。。。。
  Non-drill Thermals 选择Pad Shape很重要，否则不成功。
![](http://hiphotos.baidu.com/ken2232/pic/item/3763b73e1f30e9245548c98c4c086e061c95f715.jpg)
====================================================================
第十三节 – 覆铜(Copper Pouring)
许多印制电路板(Printed Circuit Board)设计系统支持各种类型覆铜(Copper Pouring)或区域填充方式，但是很少能够达到PADS Layout 的覆铜(Copper Pour) 如此功能强大有具有很大的灵活性。一旦你学习了一些基本的策略后，你就可以快速地建立并编辑用于屏蔽(Shielding)的绝缘铜皮区域、电源和地线层的区域。
本教程的这节将介绍以下内容：
· 建立覆铜(Copper Pour)外边框(Outline)
· 灌注(Flooding)覆铜边框(Pour Outline)
· 编辑覆铜(Copper Pour)的填充(Hatch)
· 覆铜的一些高级功能
· 贴铜(Copper)操作
建立覆铜(Copper Pour)的边框(Outline)
覆铜边域(Pour outline)定义了需要进行覆铜(Copper Pour)的几何图形。
当你使用灌注(Flood)命令建立被灌注的铜区域时，覆铜边框(Pour outline)现在暂时是不可见的。PADS Layout 填充铜皮(Copper Hatching)后，并不同时显示覆铜边框(Pour outline)。覆铜边框(Pour outline)还是存在的；
**如果你打入PO，然后按回车(Enter)，**还是可以看到它们的。这个命令可以来回切换显示，即在显示覆铜边框(Pour outline)和已经覆铜填充(Poured Copper Hatch) 之间切换。
打开前面保存的设计文件
在你继续本教程之前，打开previewsplit.pcb 文件。
1. 从工具条(Toolbar)中选择打开(Open)图标。
2. 当Save old file before reloading?提示出现后，选择No。
3. 在文件打开(File Open)对话框中，双击名为previewsplit.pcb 的文件。
定义覆铜边框(Pour Outline)
1. 从工具条(Toolbar)中选择绘图(Drafting)工具盒图标。
2. 从绘图(Drafting)工具盒中选择覆铜(Copper Pour)图标。
采用绘制平面层(Plane)区域边框同样的方法绘制覆铜边框(Pour outline):
3. 键入G25，设置设计栅格(Design Grid)为25。
4. 键入L1，设置当前层为主元件面(Primary Component Side)层，将Pour
outline 画在第一层。
提示：如果你在画之前没有注意到此步骤，可能将图形画在了其他层，你可以通过选择图形，点击鼠标右边选择属性，在对话框中将其切换到需要的层即可。
5. 点击鼠标右键，弹出菜单(Pop-up Menu)，然后选择多边形(Polygon)。
6. 通过在下面位置处，按鼠标左键，建立一个矩形(Rectangle):
X2500,Y1875
X2500,Y325
X3000,Y325
7. 在X3000、Y1875 处双击鼠标完成操作，弹出Add Drafting 对话框。
![](http://hiphotos.baidu.com/ken2232/pic/item/b24d50fa1e178a8205fd3206f603738dab77e8c1.jpg)
8. 改变已经存在的宽度Width 值为12。
9. 在网络指派(Net Assignment)框内，选择GND。
10. 然后选择OK，保持这些改变，关闭对话框。
11. 从绘图(Drafting)工具盒中点中选择(Select)图标，退出覆铜(Copper Pour) 方式。
12. 从点击鼠标右键，从弹出菜单(Pop-up Menu)中选择任意目标(Select Anything)。
13. 在你建立的矩形(Rectangle)的右边边框上按鼠标。
14. 从弹出菜单(Pop-up Menu)选择拉出圆弧(Pull Arc)。
![](http://hiphotos.baidu.com/ken2232/pic/item/ff9781118a82b901f76c8e95738da9773b12efc1.jpg)
15. 向覆铜边框(Pour Outline)的右边拉出圆弧，使得它和板子的边框相对应，
按鼠标完成操作。
提示：通过按住Shift 的同时按覆铜边框(Pour Outline)的任意一点，可以选择整个覆铜边框(Copper Pour Outline)。
![](http://hiphotos.baidu.com/ken2232/pic/item/0541595a10385343cf31d29c9313b07ecb808806.jpg)
灌注(Flooding)覆铜边框(Pour Outline)
现在你已经准备灌注(Flood)覆铜边框(Pour outline)。
1. 当覆铜边框(Pour outline)还处于被选择状态时，从弹出菜单(Pop-up Menu) 中选择灌注(Flood)。
![](http://img02.banzhu.net/User_UploadFiles/2009/1/3/200913000278085341.gif)
2. 当出现Proceed With Flood? 提示时，选择 是(Y) 按钮。
![](http://hiphotos.baidu.com/ken2232/pic/item/2e7c1584b9014a9048ff0b1ba9773912b11beec1.jpg)
在所有覆铜边框(Pour outline)被灌注后，你将看到已经覆了铜(Poured Copper)的区域。
![](http://hiphotos.baidu.com/ken2232/pic/item/f6378f3e5343fbf287d2f39eb07eca8064388f06.jpg)
编辑覆铜填充(Copper Pour Hatch)
填充区域(Hatch Areas)是根据填充边框(Hatch Outline)建立的区域，你可以采用和编辑铜皮边框(Pour outline)一样的方法，编辑这些填充边框(Hatch Outline)，通过选择它们然后从弹出菜单(Pop-up Menu)中选择命令进行执行。当你改变填充边框(Hatch Outline)以后，你必须重新生成内部的具体填充内容。
使用下面两种方法可以重新生成内部的具体填充内容：
· 从绘图(Drafting)工具盒选择填充(Hatch)图标，对于被选择的区域重新生成。
· 选择工具/覆铜管理器(Tools/Pour Manager)，然后选择填充(Hatch)表格。
![](http://hiphotos.baidu.com/ken2232/pic/item/bae926074a90f603cc71d1e13912b31bb251edc1.jpg)
1. 选择填充所有的(Hatch All)或者快速填充(Fast Hatch)重新生成填充(Hatch)。填充所有的(Hatch All)将重新填充所有的区域，包括以前灌注过(Flooded)的或者被修改的(Modified)。快速填充(Fast Hatch)将重新填充被修改的(Modified)，但不包括已经填充的(Hatched)。
2. 选择开始(Start)按钮，执行重新填充过程。
3. 选择关闭(Close)，退出覆铜管理器(Pour Manager)。
注意： 记住，在你编辑完任何填充边框(Hatch Outlines)之后，避免使用灌注所有的(Flood All)。灌注(Flood)仅仅是灌铜(Pour)；填充(Hatch)仅仅是对于你需要的边框。
保存设计备份以一个新的文件名保存设计。
1. 选择文件/另存为(File/Save As)，文件另存为(File Save As)对话框将出现。
2. 在文件名(File Name)字符框内打入previewpour.pcb。
3. 选择保存(Save)。
PADS Layout 保存改变，并且使previewpour.pcb 成为当前文件。
注意：以下部分为了介绍灌铜部分的高级功能和贴铜功能，你可以在此进行练习，但是不做为此教程的步骤和存盘部分。
通过鼠标点击指派网络。
当画完成一个Copper Pour 的外形Outline 之后，选中整个Outline 的shape，鼠标右键弹出菜单中选择Query/Modify，弹出Query/Modify Drafting 对话框。这种指派网络的方法是点击此窗口右下角的Assign Net by Click 按钮，然后缩小此对话框到，到PCB 板图上直接查找需要的指派的网络位置，点击鼠标左键即可完成网络的选择，而不需要到网络列表中查找。当按下此按钮时，你可以观察到PADS Layout 工作界面的左下角出现的提示**：Assign Net to Copper: Click at a Pin,Via,Copper, Link or Trace of Net to assign.。提示你可以通过点击相应网络的管脚、过孔、铜皮或走线等来指派网络。**
![](http://hiphotos.baidu.com/ken2232/pic/item/2053cc45fbf2b211a5d0d0f3ca8065380dd78e06.jpg)
Flood over via 的设置
如果在灌铜时，需要将过孔全覆盖(Flood over)，请点击Drafting Properties 界面的右上角的选项Options 按钮，将弹出如下对话框。
![](http://img02.banzhu.net/User_UploadFiles/2009/1/3/2009130001185427315.gif)
![](http://hiphotos.baidu.com/ken2232/pic/item/896ad596f603738d698b4184b31bb051fa19ecc1.jpg)
将选项Flood over vias 选中即可，他们分别对应的效果如下图，左边是正常的热焊盘的灌铜效果，右图是Flood over vias 的灌铜效果。
**需要提醒的是：这项设置只针对被设定的这块Copper Pour，而且它只影响via，对焊盘pad 如需此效果，需要另外设置？ 如何设置 ？。**
![](http://img02.banzhu.net/User_UploadFiles/2009/1/3/200913000926761280.gif)![](http://hiphotos.baidu.com/ken2232/pic/item/632864f4b211931387bdaa0d65380cd790238d06.jpg)
定义Copper Pour 的优先级
当有多个Copper Pour 重叠时，我们可以设定各个Copper Pour 的优先级等级来进行灌铜。如下两个部分互相重叠的Copper Pour，我们可以分别设定他们的优先级进行灌铜。为了区别两个网络，我们用不同颜色予以区别。
![](http://hiphotos.baidu.com/ken2232/pic/item/7afb6905738da977f8eecb8db051f8198418e3c1.jpg)
1. 鼠标右键弹出菜单选择Select Shapes，点击选择左边黄色一个shape，鼠标右键选择Query/Modify 弹出对话框，点击右上角的Options 按钮，弹出Flood &Hatch Preferences 对话框，在其右下角位置的Flood Priority 处输入 1 。点击OK按钮，对弹出的Proceed with flood？对话框，选择否。
![](http://hiphotos.baidu.com/ken2232/pic/item/c668ec8ba977391271e7c8c7f8198618347ae2c1.jpg)![](http://hiphotos.baidu.com/ken2232/pic/item/827a0c15b07eca805cfb6c5a912397dda0448306.jpg)
2. 对右边绿色一块Copper Pour 进行同样的操作，将其优先级值设定为 2 。
3. 现在开始灌铜，选择菜单Tools/Pour Manager…/Flood，选择Flood All，并点击Start 按钮。灌铜结果如下，黄色Copper Pour 优先于绿色的。
![](http://hiphotos.baidu.com/ken2232/pic/item/43e636713912b31b73ad808f8618367ad8b4e1c1.jpg)
4. 使用无模命令输入PO，我们可以看到显示的是外框线outline 的形式，这时我们再次编辑黄色的优先级，与前面类似的操作，将其优先级设定为 3 。
5. 我们再做一次灌铜操作，灌铜效果如下，绿色Copper Pour 优先于黄色的。
![](http://hiphotos.baidu.com/ken2232/pic/item/a3782f78ca8065383614f1ae97dda144ac348206.jpg)
从以上的操作，我们可以看出，**设置的数字越低，其优先级越高。**
提示：可以设置的优先级数字范围从0 到250。
贴铜(Copper)功能
贴铜Copper 与灌铜Copper Pour 的不同点在于，画完Copper 的外形框之后，对其内部全部铺铜，而不避让任何的网络和元件等等目标；而Copper Pour 的外形框完成之后，进行Flood，它将以完全间距的距离避开不同网络的焊盘、过孔等目标，而对于同一网络的目标，采用花孔或者Flood over 进行连接。
下面我们来看看贴铜的操作过程。
在上面打开的PCB 图的情况下，
1. 从工具条(Toolbar)中选择绘图(Drafting)工具盒图标。
2. 从绘图(Drafting)工具盒中选择覆铜(Copper)图标。
3. 点击鼠标右键，从弹出菜单中选择外形线为多边形(Polygon)。
4. 这时我们可以点击鼠标左键，开始Copper 外形线的绘制，绘制完成一个封闭的多边形以后，最后双击鼠标左键可以完成封闭多边形的绘制。这时弹出一个Add Drafting 的对话框，如果所画的铜皮属于某个网络，请在Net 列表中选择一个网络名，指派这个Copper 为此网络，例如选择GND 网络。当然，你也可以使用我们前面介绍的使用Assign Net by Click 按钮进行网络的指派。另外，在此你也需要指定此Copper 所在的层Layer，通过界面中间的下拉列表进行选择。
![](http://hiphotos.baidu.com/ken2232/pic/item/991ca614b31bb0513ce5fe8e367adab448ede0c1.jpg)
5. 指派完成，点击OK 按钮，你将完成一个Copper 的绘制。如下图。
![](http://img02.banzhu.net/User_UploadFiles/2009/1/3/2009130001637169297.gif)
6. 现在我们在这个图形的中间挖出一个圆形，看看如何操作；点击工具条上的Copper Cut Out 图标。
7. 点击鼠标右键，弹出菜单选择绘制圆形(Circle)。
![](http://hiphotos.baidu.com/ken2232/pic/item/8015558665380cd7aae0f750a144ad3458828106.jpg)
8. 在刚才图形上，选择圆心位置，点击鼠标左键。拖动鼠标，将出现一个圆形，根据你的需要，拖出一个合适半径的园。再次点击鼠标左键完成。
![](http://hiphotos.baidu.com/ken2232/pic/item/faebfa3e0cd79123ab1ec1c9ad345982b3b78006.jpg)
9. 可是这时你什么也看不到！因为两个图形重叠在一起了。这时取消绘图状态，点击工具条上的Select 图标。在点击鼠标右键从弹出菜单中选择Select Shapes。通过鼠标的左键的拖动一个较大范围，将两部分Copper 都包含在内。这时两部分Copper 都被选中并高亮。点击鼠标右键弹出菜单选择合并Combine 选项。
![](http://hiphotos.baidu.com/ken2232/pic/item/09792c1db051f81943e44eecdab44aed2c73e7c1.jpg)
10. 这时你可以发现已经将两部分Copper 合并了。效果如下：
![](http://hiphotos.baidu.com/ken2232/pic/item/83702f57f8198618f086a2224aed2e7389d4e6c1.jpg)
===========================================================================
[总结：覆铜(copper) 灌铜(copper pour) 以及Plane(平面层)](http://hi.baidu.com/zhengju444/article/details/5197675)2010-01-16
 09:02347人阅读[评论](http://hi.baidu.com/ken2232/blog/modify/c274210fdf453b046b60fbdd#comments)(0)[举报](http://hi.baidu.com/ken2232/blog/modify/c274210fdf453b046b60fbdd#report)[http://blog.csdn.net/zhengju444/article/details/5698449](http://blog.csdn.net/zhengju444/article/details/5698449)
覆铜(copper)：绘制一块实心铜，从而将覆盖区域内所有的连线和过孔连接起来，而不会考虑是否属于一个网络。用途：芯片需要大面积覆铜来散热。电源连接处需要尽可能的宽，可以同覆铜来实现。不具备自动避让功能。
no plane灌铜(copper pour)：仅仅连接有相同网络的过孔，可以通过这个来连接，用于no plane的。具备自动避让功能的。必须使用铺铜的命令(COPPER POUR)，用法同外层线路，**不会自动移除独立焊盘，可走线，不可以在大块铜皮上进行其他网络的分割。也就是说不能出现大块铜皮包围小块铜皮的现象。可以走线。**
split/mixed(分割平面层)：用于split/mixed平面类型的，用Plane connect来进行连接。必须使用内层分割命令（PLane AREA)，可**自动移除内层独立焊盘，可走线**，可以方便的在大片铜皮上进行其他网络的分割，内层分割的智能化较高。可以走线。
注意：
**no plane 可以走线，可以分割平面，用copper pour灌铜；**
cam plane 不能走线，也不能分割平面，**只能一个负片输出；**
split/mixed 可以分割平面；该层允许布线，用plane area分割区域；
思路：
1.首先，采用四层板子来制作，分别为:top gnd power bottom ;分别对应类型为：no plane split/mixed split/mixed no plane四种类型；
2.然后，对于gnd进行两个地的分割划分；对于电源也进行几个电源的分割区域划分（plane area的方式）；对于top和bottom两个走线层，也进行必要的划分区域（灌铜的方法不影响走线的）来进行；
VCC和GND层设置为NO PLANE ,cam plane还是split/mixed plane
NO PLANE主要用于小面积铺铜,而且铺铜区的连线比较多(TOP,BOTTOM).split/mixed plane主要用于面积比较大的铺铜,专门给VCC和GND层,这些层连线不多,主要通个整个平面连接,可减少电源和地的波动所以用PLANE大面积铺铜比较合适
