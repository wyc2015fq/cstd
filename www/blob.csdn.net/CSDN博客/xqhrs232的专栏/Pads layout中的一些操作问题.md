# Pads layout中的一些操作问题 - xqhrs232的专栏 - CSDN博客
2014年02月21日 15:21:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2253
原文地址::[http://blog.163.com/fengtao_zhang@126/blog/static/16940162520125810524348/](http://blog.163.com/fengtao_zhang@126/blog/static/16940162520125810524348/)
相关文章
1、Pads_layout中的一些操作问题----[http://wenku.baidu.com/link?url=9si_btWurQfgsyyy1Mxa1brnwUkZwnX0icPgBfwJ-03dp8OsXzHwOUHFW6ZxpyZCo6ZZKQRIIwMvWqF6nuIUgYUsdzpG2gIDtMzvXbtcAAm](http://wenku.baidu.com/link?url=9si_btWurQfgsyyy1Mxa1brnwUkZwnX0icPgBfwJ-03dp8OsXzHwOUHFW6ZxpyZCo6ZZKQRIIwMvWqF6nuIUgYUsdzpG2gIDtMzvXbtcAAm)
2、PADS2007layout设置和基本操作步骤1----[http://wenku.baidu.com/link?url=UAwqCkYC_jMuoC0godoGkTwAFoaiyISCxo_7AkFPSQxhpkXoDUSLqyq0ynuZnbFsu4hGYICd9OfvyrA7KcC920nbQ1YIZVBic_9wySuwy1y](http://wenku.baidu.com/link?url=UAwqCkYC_jMuoC0godoGkTwAFoaiyISCxo_7AkFPSQxhpkXoDUSLqyq0ynuZnbFsu4hGYICd9OfvyrA7KcC920nbQ1YIZVBic_9wySuwy1y)

1.Pad和Via有什么区别？
PAD是焊盘，VIA是过孔，通孔焊盘和过孔都会打穿板子。PCB实物做出来焊盘那个孔周围是没有阻焊层的
，可以焊锡在上面，而过孔则没有。
通孔焊盘和VIA都必须设置25 layer，且Drill Size(钻孔大小)与其他层一致，但Diameter(焊盘大小)要
比其他层大20mil(0.5mm)以上。
焊盘外径D一般不小于 (d+1.2)mm，高密度数字电路的D最小可到 (d+1.0)mm，其中d为钻孔直径。
1-1.请问PowerPCB如何设置才能在走线打孔的时候信号线自动用小孔，电源线用大孔？
先在PAD STACKS中将你要用的VIA式样定制好，然后到Desing Ruels中先定义Default Routing Rules使用
小的VIA，再到Net Ruels选中电源的Net，在Routing中定义成大的VIA。如不行，可以敲入“VA”，将VIA
Mode设成Automatic，它就会按规则来了。
【不同网络设置线宽】在PowerPCB中是否有对不同网络分别进行线宽的设置吗？
可以的！ 
design rules->default-》Clearance-》Trace Width设置缺省值。
design rules->Class (或Net)中选中网络-》Clearance-》Trace Width 生成新的条件规则
则实线不同网络不同走线宽度OK！
1-2. 测点优先级：Ⅰ. 表贴焊盘 (Test pad)   Ⅱ. 零件脚(Component lead)   Ⅲ. 贯穿孔(Via hole)
1.如何添加和自定义过孔或盲孔？
Setup ---> Design ---> Rules---> Default---> Routing 中Vias的Availabe和Selected匀为空白，请
问怎样设置过孔？
那你就新建一个VIA类型SETUP->PAD STACKS->在PAD STACK TYPE中选VIA->ADD VIA……然后Setup ---
>Design --->Rules---> Default---> Routing 中，把新建的VIA添加到SELECT VIA!
1-3.怎么加测试点?
【SCH上手动增加测试点】
原理图中就增加测试点符号，并PCB库中做好对应测试点的封装(表贴封装、via封装)，然后在layout中导
入网表即可
----------------------------------
【PCB上直接手动加测试点】
1) 连线时，点鼠标右键在end via mode 中选择end test point 
2) 选中一个网络的某段走线，右键-Add testpoint(只能加Via测试点)或Add Via，选中Via或Pad修改属
性为TestPoint
3)将焊盘 (表贴封装、via封装)做成一个部件，在ECO模式下用添加Component的方式增加进来，并修改属
性为测试点
--------------------------------
【自动增加针床测试用的测试点】在PADS Layout中有专门加入测试点的方法。具体是：PADS Layout--
tools--DFT Audit中可以选择添加测试点的类型，在添加过程中会生成报告。这里还要说明的是，在PADS
中目前自动添加只能添加过孔类型的测试点，原因是为了做针床测试。
2-0.在铺铜时画铺铜区时，如要在TOP 和BOTTOM均要铺GND的铜，是否需要在TOP和BOTTOM分层画铺铜区后
，再分层进行灌铜？
在灌铜时，各层均需要分别画铜皮框，如果一样的外形，就可以Copy。画完后现在Tools下的 Pour
Manager中的 Flood all 即可。
2-1.如何控制所铺铜皮为网格或实心？
1.Opttions->Grids->Hatch grid中可以设置Copper值，
2.选中铜皮，右键->Properties,在Drafting Properties中有Width设置值。
软件以十字交叉网格来生成铜皮，网格线宽为Width，网格间距为Hatch grid。当Width>Grid值时，铜皮
为实心；当Width<Grid值时，铜皮为网格。
【注】：Flood灌铜，也产生这样的效果。无模命令po显示Copper pour区边框并选中后，右键-
>Properties,在Drafting Properties中有Width设置值;在Options设置页面中，如果选中Default，则采
用Opttions->Grids->Hatch grid设置值；若在Hatch grid中重新填入值，则采用新值(忽略默认值)，然
后Flood。
2-2.如何控制灌铜区的显示模式？
a（1）无模命令PO 切换显示模式Pour Outline<-> Hatch Outline 或
a（2）Tools-Options-Drafting-Hatch-Display Mode中勾选上Pour Outline或Hatch Outline
注，Pour Outline (显示为Copper Pour约束区的框线，只能进行Flood，可在Options选项中选择是
dafault的Hatch Grid还是自行填入的值) 
    Hatch Outline (显示为所有的填充影线轮廓，总效果就是铜皮整体，Flood和Hatch均可)
b Tools-Options-Drafting-Hatch-View可以设置填充效果：
                    Pour Outline 模式                           Hatch Outline 模式
    Noraml       显示为Copper Pour约束区的框线            显示填充影线，总体效果显示为所灌出
的整片铜皮(实心/网格)*
    No Hatch     显示为Copper Pour约束区的框线            不显示填充影线，显示为所灌出的整片
铜皮的轮廓框线*
    See Through 显示为Copper Pour约束区外框的中心线      显示为填充影线的中心线
2-3.增加Copper CutOut或 Copper Pour CutOut等后，都要用Tools-Pour Manager（Flood All和Hatch
All）重新灌铜一次，Priority项设置的数字越低，其优先级越高
2-4.如何放置铜皮和剪切铜皮？
a.Copper画铜皮外形(必须先执行DRO要关闭DRC)，设置填充线的width；放置Copper Cutout，两者通过右
键中combine结合起来，OK
2-5.如何打开自动移除孤立铜皮设置？
a.Tools-Options-Thermals中勾选Remove isolated Copper；
b.Tools-Options-Split/Mixed Plane中勾选Remove isolated Copper；
这两者时连动的，任何一个地方设置了，另外一个地方自动跟着设置。
【手动去除】菜单Edit—Find---菜单下Find By--Isolated pour—OK
2-6.1.如何设置铺铜边缘到板框的间距？
在Clearance Rules中设置好 Copper与Board的clearance
2-6.2.如何修改PowerPCB铺铜（灌水）的铜箔与其它组件及走线的间距？
如果是全局型的，可以直接在setup － design rules里面设置即可，如果是某些网络的，那么选中需要
修改的网络然后选右键菜单里面的show rules进入然后修改即可，但修改以后需要重新flood，而且最好
做一次drc检查。 
2-6.3对于一过孔（为GND网）或一器件的插脚（为GND网）。现在要同时对顶层和底层的GND铺铜。为什么
只允许插脚的单面GND连通所铺的铜？PROTEL中两面均可连接，PowerPCB中怎么解决？
你可以到Setup－preference－Thermals选项中，将“Routed Pad Thermals”选项打勾试试！
2-7.hatch和flood有何区别，hatch何用？如何应用?
hatch是刷新铜箔，flood是重铺铜箔。一般地第一次铺铜或file修改后要flood，而后用hatch。
2-8．PowerPCB中铺铜时怎样加一些via孔？
(1)可将过孔作为一part，再在ECO下添加part； 
   (2)直接从地走线，右键end(end with via)。
3.画带异形铜皮的焊盘？
在Decal Editor中先画好异形铜皮，或引入dxf文件中的外形框改为Copper；
然后添加一个Terminal(Pad)并放好位置,选中Pad右键->associate->左键点击异形铜皮，则两者就粘合在
一起形成了异形焊盘。当要解除粘合时，选中异形焊盘，右键->unassociate
4.如何让走线在焊盘入口、出口产生泪滴？
Tools-Options-Routing-Options中勾选上Gnerate teardrops
5.如何关闭、打开热焊盘的十字型标记
Tools-Options-Thermals中勾选上Show general plane indication
Tools-Options-Split/Mixed Plane-Mixed plane display中勾选上Plane thermal indication
6.如何对元件推挤状态进行设置？
在Tools-Options-Design-Nudge中进行设置
7.如何显示和关闭钻孔？
无模命令 do；
或Tools-Options-Routing-Options中勾选上show drill holes
8.如何保护一些特殊的走线，不让其受自动布线影响？
选中需要保护的某部分布线，[按F6键(选择Nets)，此时选中全网络的走线]，Ctrl+Q或者右键-》
Properties-》Protect Routes
9.如何固定元件在PCB班上的位置，不让其移动？
选中需要固定的器件，Ctrl+Q或者右键-》Properties-》Glued
10.如何以极坐标方式移动原件？
a.Tools-Options-Grids-Radial move setup中设置好极坐标移动设置参数
b.选中器件，右键-》Radial Move
11.如何给特定的网络设置特殊的颜色？
View -》 Nets ，此处颜色的优先级比Setup -》 Display Colors中的颜色优先级要高
12.View中的Board显示和Extent显示有和区别？
Board显示板框范围内的PCB视图；Extent显示所有器件的PCB视图，若有器件在板框外，也能显示出来。
13.如何切换视图显示模式？
无模命令“O” 切换正常视图<->轮廓模式(走线和焊盘以边框代替实体显示，可以提高画面的刷新速度)
无模命令“T” 切换正常视图<->透明模式(显示被当前层挡住/与当前层重叠 的走线和器件)
14.如何切换DRC状态？
DRO 关闭DRC
DRP 禁止违反DRC的动作
DRW 警告违反DRC的动作
DRI 忽略违反DRC的动作
或者在Tools-Options-Design-On Line DRC中进行设置
15.Class、Nets，Group、Pin、Pairs的关系？
在Design rules设置中，Class指具有相同设计规则的一些Nets；Group指具有相同设计规则的Pin Pairs
16.PCB中如何更改器件的Decal封装？
方法一：选中器件，Ctrl+Q弹出Component Properties，在Decal选项框中选择。
方法二：打开ECO模式，选中器件更改Decal
16-2.PCB中如何更改器件的丝印外框宽度？
方法一：选中器件，Ctrl+Q弹出Component Properties，在Decal选项框中选择对应封装，重新设置Part
Outline Width值，点击Apply，弹出的对话框中选【确定】，OK。
【注：】此方法：1.将改动具有相同封装的所有器件的丝印外框宽度；2.仅影响PCB上的丝印，不影响封
装库中的丝印。
方法二：在Decal Editor中重新编辑相应封装的丝印外框宽度。
17.routing层对，via/drill层对，jumper层对？
Routing 层对设置：Tools-Options-Routing-Layer Pairs
Via/Drill层对设置：Setup-Drill Pairs
Jumper   层对设置: Setup-Jumpers
18.布局时，如何只显示器件，不显示鼠线和走线？
View-Color Display中设置Conection和Trace的颜色为背景颜色即可
19.Union的使用？
先放好union内组员的相对位置(如IC和去耦电容，将去耦电容放置在其下面或旁边)，全选-右键-Create
Union，取好Union的名字。则在以后的布局中，Union内成员是一体的，可以一起移动。
20.Cluster的使用？
选取单个或多个器件，右键-Crete Cluster，输入Cluster的名字，则PCB上这些器件Decal消失，出现一
个圆圈，方便用户布局
21.平面层有两种：
(1)CAM层用于分割成多个电平面区(Plane area)或单个电平面(Plane area)，出负片(gerber文件中白
色表示铜皮)，不需要Flood灌铜处理，不可走线，
    注：如使用CAM层，则板上的通孔焊盘和Via必须含有25 layer，焊盘直径要比顶层或底层大10mil左
右
(2) Split/Mixed层用于分割成多个电平面区(Plane area)或单个电平面(Plane area)，出正片(gerber
文件中黑色表示铜皮)，必须要用Flood进行灌铜处理，可走线
非平面层： No Plane层主要用于顶层、顶层等布线层，采用定义Copper Pour区，将表面分割成多个智
能铺铜区。
22.Paste Mask 助焊层，用于制造给焊盘刷锡膏的钢网 ，正片(黑色代表钢网的开窗，对应焊盘)
   Sloder Mask 阻焊层，用于给PCB表面刷绝缘绿油，负片(白色代表刷绿油部分)
23. （1）CAM输出钻孔drill时，选择Pad和Via，因为它们都有通孔。
    （2）进行CAM输出PHO文件时，一定要先用Tools-Pour Manager进行Flood操作，把所有层的铜全部灌
上，然后在生成PHO文件。
（3）请问PowerPCB gerber out时*.rep，*.pho，*.drl，*.lst各表示什么意思，在制板时哪些文件是
制板商所需要的?
*.pho　GERBER数据文件 
*.rep   D码文件（线，焊盘的尺寸，必不可少的） 
*.drl　钻孔文件 
*.lst   各种钻孔的坐标 
以上文件都是制板商所需要的。
23.布线工具说明？
Add Corner(将一条直线变成带角的折线，顶角随光标移动)、Split(将一条直线分成两部分，一部分固定
，另一部分变成两条折线随光标而动)、Add Route(重新走线)可以在DRC的任何一种模式下工作，因此走
线时要注意是否违反了设计规则；
   Dynamic Route(动态布线，自动绕开障碍物寻找路径，但不优化路径)、Sketch Route()、Auto
Route(智能布Pin Pairs间的线)、Bus Route(先选多个Pin脚，然后选总线布线)只能在DRC打开且为DRP时
才能使用。
注：若需要重新走一根已经走好的线，最好先删除走线，再重新用 动态布线或自动布线进行走线。
24．如何将一个器件的某个引脚的由一条网络改为另一条网络？
打开eco，用delete connettion删除原来的连接，不要删除网络噢。再用add a connetion添加一个连接
。建议直接在原理图中改动，然后重新生成网表，导入
25.25层有何用处？
POWERPCB的25层存储为电源、地的信息。如果做多层板，设置为CAM PLANE就需要25层的内容。设置焊盘
时25层要比其它层大20MIL，如果为定位孔，要再大些。
10.如何建立和保存启动文件？
打开Pads Layout或PCB文件，设置各种参数，File\Save as start-up File
11.如何将pcb decal中的封装对应的part type更换一个名字？
方法一： 在library manager中选中Parts选项，选中要改名的Part Types-》Copy-》在弹出的Name of
Part Types中填入新的名字
方法二： 在library manager中选中Decals选项，选中对应的PCB Decals-》Edit-》Edit Part->Save
As->在弹出的Name of Part Types中填入新的名字
12.尺寸标注的文本和箭头所在的层设置必须在Tools-Options-Dimensioning-Layers中设置，放置时其一
定在设置的层中，而忽略PCB当前活动层的设置
6.设计输出
    PCB设计可以输出到打印机或输出光绘文件。打印机可以把PCB分层打印，便于设计者和复查者检查；光绘文件交给制板厂家，生产印制板。光绘文件的输出十分重要，关系到这次设计的成败，下面将着重说明输出光绘文件的注意事项。
a. 需 VCC层和GND层）、丝印层（包括顶层丝印、底层丝印）、阻焊层（包括顶层阻焊和底层阻焊），另外还要生成钻孔文件（NC Drill）
b. 如果电源层设置为Split/Mixed，那么在Add document窗口的document项选择Routing，并且每次输出光绘文件之前，都要对PCB图使用Pour Manager的Plane Connect进行覆铜；如果设置为CAM Plane，则选择Plane，在设置Layer项的时候，要把Layer25加上，在Layer25层中选择Pads和Viasc. 在设备设置窗口（按Device Setup），将Aperture的值改为199
d. 在设置每层的Layer时，将Board Outline选上
e. 设置丝印层的Layer时，不要选择Part Type，选择顶层（底层）和丝印层的Outline、Text、Line
f. 设置阻焊层的Layer时，选择过孔表示过孔上不加阻焊，不选过孔表示家阻焊，视具体情况确定
g. 生成钻孔文件时，使用PowerPCB的缺省设置，不要作任何改动。
h. 所有光绘文件输出以后，用CAM350打开 “PCB检查表”检查。
7.电源的去耦电容：
数字信号布线区域中，用10uF电解电容或钽电容与0.1uF瓷片电容并联後接在电源/地之间.在PCB板电源入口端和最远端各放置一处，以防电源尖峰脉冲引发的干扰。
