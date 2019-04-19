# PADS Layout中英文对照 - xqhrs232的专栏 - CSDN博客
2014年02月21日 15:10:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1194
个人分类：[PCB/EDA/硬件设计与仿真](https://blog.csdn.net/xqhrs232/article/category/906926)
原文地址::[http://www.eda-china.com/index.php?a=show&m=Article&id=283](http://www.eda-china.com/index.php?a=show&m=Article&id=283)
相关文章
1、PADS_中英文对照----[http://wenku.baidu.com/link?url=TjSdYqMtnrCKvy0PJtMuZ7Tyshu0KrNijsvAoxRnuTGXcvUtP8FP3Kr5j0NxgZ7j0Jq-WRu_JIV2c-JSOGE6g4GFaJN55D-umWRL3iFy_um](http://wenku.baidu.com/link?url=TjSdYqMtnrCKvy0PJtMuZ7Tyshu0KrNijsvAoxRnuTGXcvUtP8FP3Kr5j0NxgZ7j0Jq-WRu_JIV2c-JSOGE6g4GFaJN55D-umWRL3iFy_um)
# PADS PCB：
## **一 、Setup**
### 1、Preference优先设置
#### ⑴ global
◆     Pick Radius捕捉半径
◆     Keep Same View on Window Resize设计环境窗口变化是否保持同一视图
◆     Active Layer Comes to Front激活的曾显示在最上面层
◆     Minimum Display Width最小显示线宽，如果当前PCB 板中有小于这个值的线宽时，则此线不以其真实线宽显示而只显示其中心线
◆     Drag and attach附属拖动
◆     Drag and drop放下拖动对象就可完成移动
◆     No Drag Move禁止采用拖取移动方式
#### ⑵ Design
◆     Stretch Trace During Component Move移动元件时保持走线链接
◆     Miter倒角
◆     Keep Signal and Part Name保持信号和元件名称
◆     Include Traces not Attached定义一个区域时，内部包含的和块内没有相同网络的连接也作为块的一部分
◆     Line/Trace Angle 2D和走线的角度
◆     Drill oversize对沉铜进行全景补偿
#### ⑶ Routing
◆     Generate Teardrops产生泪滴
◆     Show Guard Band显示保护带，如果违反了操作，会在违规的临界点上用一个八边形来阻止用户的操作，可通过On-Line DRC设置
◆     Highlight Current Net当前选中的或正在操作的网络是否要高亮显示
◆     Show Drills Holes是否显示钻孔
◆     Show Tracks是否显示Tack，Tack是一种错误标志，当在层定义里定义的走线方向和实际的走向不一致时，就会有这种菱形的标记出现
◆     Show Protection显示保护线
◆     Show Test Points显示测试点，如果关闭此选项，测试点和过孔就表现为相同的形式了
◆     Show Trace Length显示线长，实时地显示走线的长度和已布线的总长度
◆     Centering-Maximum Channel设置最大的通道长度
◆     Unrouted Path Double Click用鼠标双击未连接的飞线，通过设置可以产生两种结果，一种是自动连线(Dynamic Route)，一种是手动连线(Add Route)，如果是自动连线，最好打开在线检查设计规则On-Line DRC
◆     Auto Protect Traces自动保护走线，保护一个网络的走线，包括长度受控的网络和走线末端的过孔
◆     Enable Bus Route Smoothing使总线圆滑，当完成总线布线后，进行一个圆滑的动作，这个设置只在总线布线模式下有效，它的优先级高于全局优化的优先级
◆     Guide Pad Entry允许连线以任何角度和焊盘连接
◆     Smooth Pad Entry/Exit允许对和焊盘成90°的连线进行优化，优化为45°的连线
◆     Minimum Amplitude(Times Trace Width)蛇形走线的高度，这个高度是按照线宽的整数倍来设置的
◆     Minimum Gap(Times Trace to Trace Clearance)蛇形走线时GAP的宽度，这个宽度是按照垂直线之间距离的整数倍来设置的
◆     Thermals热焊盘在电源和地层也称为花孔，为了对电路板进行好的屏蔽，通常会在顶层和底层甚至中间层铺大量的铜皮，并将其与地网络连接在一起，铜皮 与地网络连接的过孔或焊盘称为热焊盘，通常分为两种：通孔热焊盘(Drilled Thermals)和表面贴装的热焊盘(Non-drilled Thermals)
   ·Width热焊盘连接线的线宽
   ·Min.Spoke最少连接线，一个热焊盘上至少有几根连接线
   ·Pad Shape焊盘形状
   ·Flood over填满，创建完全连接的热焊盘
   ·Orthogonal正交，连线和焊盘的连接角度为正交
   ·No Connect不形成热焊盘
   ·Routed Pad Thermals元件的焊盘也可以形成热焊盘
   ·Show Genernal Plane Indicators是否显示内层的热焊盘，关闭这个选项，热焊盘就
表现为通常的焊盘了
   ·Remove Isolated Copper移除孤立的铜皮
   ·Remove violating Thermal Spokes移除冲突的热焊盘连接，违反规则的连接线应该被移除
◆     Auto Dimensioning自动尺寸标注
   ·General Settings通用设置
   ·Draw 1st起点标注线
   ·Draw 2nd终点标注线
   ·Pick Gap测量点到尺寸标注线一端之间的距离
   ·Circle Dimension圆弧测量
   ·Alignment and Arrow校准直线和标注箭头
   ·Alignment tool校直工具
   ·Text尺寸标注值文字
   ·Omit Text不需要尺寸标注文字
◆     Teardrops泪滴
   ·Auto Adjust允许在设计过程中根据不同的要求来自动调整泪滴
◆     Drafting
   ·Board component height restriction板上元件高度限制
   ·See through将铜皮显示成一些Hatch平行线
   ·Min.hatch最小铜皮面积
   ·Smoothing铜皮在拐角处的平滑度
   ·Pour outline显示整块铜皮的外框
   ·Hatch outline显示铜皮(Pour)中每一个Hatch的外框
◆     Grids
   ·Fanout Grid扇出栅格，仅用于BlazeRouter
   ·Radial Move Setup径向移动
   ·Inner Radius靠近原点的第一个圆环跟原点的径向距离
   ·Delta Radius除第一个圆环外，其他各圆环之间的径向距离
   ·Sites Per Ring在移动角度范围内最小移动角度的个数
   ·Auto Rotate移动元件时自动调整元件状态
   ·Disperse移动元件时自动疏散元件
   ·Use Discrete Radius移动元件时可以在径向上进行不连续地移动元件
   ·Initial使用最初的
   ·Let me Specify极的方向由自己设置
◆     Split/Mixed Plane混合分割层
   ·Plane Polygon Outline只显示分割层的外框
   ·Plane Thermal Indicators除了显示分割层以外还要显示热焊盘
   ·Generated Plane Date显示分割层上的所有数据
   ·Smoothing Radius设置分割层的铜皮的平滑度
   ·Auto Separate Gap设置分割的各个平面之间的距离
   ·Use Design Rules for Thermals and Antipads对花孔和反焊盘使用设计规则
   ·Die component模具元件
### 2、Layer Definition叠层设置
◆     No Plane布线层
◆     CAM Plane整个的平面层，比如电源和地层等
◆     Split/Mixed Plane分割后的平面层，比如存在多种电源和地的平面层
### 3、Pad Stacks焊盘叠设置
### 4、Drill Pairs钻孔层对设置
### 5、Jumpers跳线设置
### 6、ECO(Engineering Change Order)工程变更设置
◆     Write ECO files记录ECO文件
◆     Append to files追加到文件中
◆     Write ECO file after close ECO toolbox在关闭ECO工具盒或者退出ECO模式时，更新ECO文件数据
### 7、Design Rules设计规则设置
设计规则优先级：
(低)Default->Layer->Class->Net->Group->Pin pairs(高)
◆     Default默认设置
   ·Drill to Drill钻孔之间
   ·Body to Body元件体之间
   ·Clearance-Pad(通孔焊盘)、SMD(表贴焊盘)、Board(板框)
   ·Protected不对飞线进行优化
   ·Minimized采用网络的所有管脚对的连接最短的规则来产生飞线
   ·Serial Source对ECL电路适用，多个驱动源串在一起
   ·Parallel Source对ECL电路适用，多个驱动源并在一起
   ·Mid-driven这个规则适用于高速电路和ECL电路，最小化网络中所有管脚对的距离，中间驱动意味着网络可以尽量短，如果是一个源两个接收端的话，那么这个拓扑表现为源在中间接收端在两边，并且源到两个接收端等长
   ·Copper sharing铜皮共享一via过孔的铜皮共享
   ·Auto Route自动布线器可以自动对网络布线
   ·Allow Ripup自动布线器在布一个已经布过的网络时，允许删除现有的走线
   ·Allow Shove自动推挤功能，可以对一个已布的网络进行推挤和重布
   ·Allow Shove Protected自动布线器可以对一个已布的并且受保护的网络进行推挤和重布
   ·Layer Biasing设置约束生效的层
   ·Vias选择定义的过孔
   ·Parallelism平行长度
   ·Tandem纵向平行度
   ·Aggressor此网络是否是干扰源，可以定义电流较大和速率较高的信号为干扰源
   ·Shielding使用屏蔽功能，减少外界的干扰，通常用平面层作屏蔽信号
   ·Gap网络同屏蔽网络走线之间的距离
   ·Use Net屏蔽的网络
   ·Stub走线中出现分支会难以控制匹配和端接，较长的分支会引起反射以及过冲，所以要加以约束
   ·Match Length要求长度匹配
   ·Fanout Length扇出的长度
   ·Nets扇出的类型
   ·Pad Entry Quality焊盘引入的质量控制，在BlazeRouter中有效
   ·Via at SMD焊盘下放置过孔
◆     Class类设置
◆     Net网络设置
◆     Group组设置
◆     Pin Pairs管脚对设置
◆     Decal封装设置
◆     Component元件设置
◆     Conditional Rules条件规则设置
◆     Differentia Pairs差分对设置
◆     Report报表设置
## 二、Tools工具设置
### 1、Automatic Cluster Placement簇的自动布局
◆     Build Clusters创建簇
   ·Min.Top Level Count最小的顶层簇的数量
   ·Unglued Parts Number当前没有被锁定的元件的数目
   ·Build Mode创建模式，簇分为open簇和close簇，其区别在于是否在Query/Modify窗口选中了close选项，在选择创建模式的时 候，Rebuild open clusters指open的簇可以拆开重建Maintain Open Clusters指要保留open属性的簇
◆     Place Clusters放置簇
   ·Board Outline Clearance簇到板框的最小间距
   ·Percent Part Expansion簇之间的距离
   ·Efforts布局的努力程度
   ·Number of Iterations簇布局的次数
   ·Attemps Per Iterations每次布局的尝试
   ·% From Part Swappling元件、簇或组合交换的频率
   ·Create Pass大范围的布局
   ·Refine Pass小范围的微调
◆     Place Parts放置元件
   ·Eliminate Overlaps是否要消除元件重叠的情况
   ·Min % Expansion Allowed最小的元件空间扩展的比例
   ·Align Parts布局微调时，相邻的元件是否要对齐
   ·Only if No Overlaps相邻的元件要对齐的前提是没有元件叠加的情况
### 2、Disperse Componets打散元件
### 3、Length Minimization长度最小化
### 4、Cluster Manager簇管理器
### 5、Auto Nudge自动推挤
### 6、Specctra是Cadence公司的自动布线器
### 7、DX-Designer反标注
### 8、BoardSim板级防真
### 9、BlazeRouter自动布线器
### 10、CAM350菲林输出
### 11、Pour Manager灌铜管理器
### 12、Assembly Options装配选项
### 13、Verify Design验证设计
### 14、Compare Test Points比较测试点
### 15、Compare/ECO Tools比较网络表
### 16、DFT Audit(Design For Test)自动为设计插入测试点
