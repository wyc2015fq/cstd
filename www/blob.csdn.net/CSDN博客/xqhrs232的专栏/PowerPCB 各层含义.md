# PowerPCB 各层含义 - xqhrs232的专栏 - CSDN博客
2013年06月25日 10:36:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：972
原文地址::[http://hi.baidu.com/chinaarm/item/d74efe4f1702f3e1bcf45156](http://hi.baidu.com/chinaarm/item/d74efe4f1702f3e1bcf45156)
PowerPCB 各层含义：
1)Top顶层
2)Bottom底层
3)-19)Layer i中间信号层
21)Solder Mask Top顶层掩膜层（阻焊层）
22)Paste Mask Bottom底层锡膏层
23)Paste Mask Top顶层锡膏层
24)Drill Drawing孔位图层，各个不同的孔径会有一个对应的符号。
25)Layer 25 对应着PCB画板中的元件阻焊开窗层(就是不上绿油所开的窗口
26)Silkscreen top顶层网印文字
27)Assembly Drawing Top顶层装配图，组装图
28)Solder Mask Bottom底层掩膜层（阻焊层）
29)Silksceen Bottom底层网印文字
30)Assembly Drawing Bottom底层装配图
其他软件中层的含义：
在EDA 软件的专门术语中，有很多不是有相同定义的。以下就字面上可能的意义来解释。
Mechnical: 一般多指板型机械加工尺寸标注层
Keepoutlayer: 定义不能走线、打穿孔(via) 或摆零件的区域。这几个限制可以独立分开定义。
Topoverlay: 无法从字面得知其意义。多提供些讯息来进一步讨论。
Bottomoverlay: 无法从字面得知其意义。可多提供些讯息来进一步讨论。
Toppaste: 顶层需要露出铜皮上锡膏的部分。
Bottompaste: 底层需要露出铜皮上锡膏的部分。
Topsolder: 应指顶层阻焊层，避免在制造过程中或将来维修时可能不小心的短路
Bottomsolder: 应指底层阻焊层。
Drillguide: 可能是不同孔径大小，对应的符号，个数的一个表。
Drilldrawing: 指孔位图，各个不同的孔径会有一个对应的符号。
Multilayer: 应该没有单独这一层，能指多层板，针对单面板和双面板而言。
Toppaste: 也即是面层贴片时开钢网要用的东东。
Bottompaste: 也即是底层贴片时开钢网要用的东东。
Internal Plane Layer内电层
由Protel2004产生的Gerber文件各层扩展名与PCB原来各层对应关系表:
Top (copper) Layer : .GTL
Bottom (copper) Layer : .GBL
Mid Layer 1, 2, ... , 30 : .G1, .G2, ... , .G30
Internal Plane Layer 1, 2, ... , 16 : .GP1, .GP2, ... , .GP16
Top Overlay : .GTO
Bottom Overlay : .GBO
Top Paste Mask : .GTP
Bottom Paste Mask : .GBP
Top Solder Mask : .GTS
Bottom Solder Mask : .GBS
Keep-Out Layer : .GKO
Mechanical Layer 1, 2, ... , 16 : .GM1, .GM2, ... , .GM16
Top Pad Master : .GPT
Bottom Pad Master : .GPB
Drill Drawing, Top Layer - Bottom Layer (Through Hole) : .GD1
Drill Drawing, other Drill (Layer) Pairs : .GD2, .GD3, ...
Drill Guide, Top Layer - Bottom Layer (Through Hole) : .GG1
Drill Guide, other Drill (Layer) Pairs : .GG2, .GG3, ...
//======================================================================================
备注：：
1》Solder Mask----阻焊层----也叫绿油层----电气特性是绝源不导通
2》Paste Mask ----锡膏层----也叫助焊层-----就是露铜后再加一层锡----一般焊盘都是这样的
3》Silkscreen----丝印层----也叫白油层----通常是为了显示文字与元器件编号/属性
