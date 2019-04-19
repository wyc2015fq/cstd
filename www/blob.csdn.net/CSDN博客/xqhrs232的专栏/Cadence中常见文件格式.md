# Cadence中常见文件格式 - xqhrs232的专栏 - CSDN博客
2013年11月12日 21:56:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2350
原文地址::[http://wenku.baidu.com/link?url=Z_TMmtetJ9Xh2z2EHzXzhT5RNI2CZ7u5aeNLGGHHjozVjAbn9f4fHYQI2rFNx2lgQU38Zd9X4x0OUANGW-H4DzOVaEFe2g2Gb3-aQsvCc3K](http://wenku.baidu.com/link?url=Z_TMmtetJ9Xh2z2EHzXzhT5RNI2CZ7u5aeNLGGHHjozVjAbn9f4fHYQI2rFNx2lgQU38Zd9X4x0OUANGW-H4DzOVaEFe2g2Gb3-aQsvCc3K)
Allegro中常见的文件格式
.brd 工具：PCB Design Expert   PCB布线 
.ddb       工具：Protel
.art  工具：CAM350  Allegro PCB Design file/impot ARTwork
.d      工具：pads2005
.drl     工具：Protel
.opj     设计项目工程
.olb     创建新的元件库
allegro/APD.jrl ：记录开启 Allegro/APD 期间每一个执行动作的 command .
                产生在每一次新开启 Allegro/APD 的现行工作目录下 .
env ： 存在 pcbenv 下,无扩展名,环境设定档.
allegro/APD.ini ： 存在 pcbenv 下,记录 menu 的设定.
allegro/APD.geo ： 存在 pcbenv 下,记录窗口的位置.
master.tag ： 开启 Allegro/APD 期间产生的文字文件 ,记录最后一次存盘的 database
文件名称,下次开启 Allegro/APD 会将档案 load 进来.从 Allegro/APD.ini
搜寻 directory = 即可知道 Master.tag 存在的位置 .
lallegro.col ： 存在 pcbenv 下,从设定颜色的调色盘 Read Local 所写出的档案.只会影
响到调色盘的 24 色而不会影响 class/subclass 的设定.
.brd ： board file (Allegro).
.mcm ： multi-chip module (APD) ,design file.
.log ： 记录数据处理过程及结果.
.art ： artwork 檔.
.txt ： 文字文件,如参数数据,device 文件 .. 等.
.tap ： NC drill 的文字文件.
.dat ： 资料文件.
.scr ： script 或 macro 记录文件.
.pad ： padstack 檔.
.dra ： drawing 档, create symbol 前先建 drawing ,之后再 compiled 成 binary symbol 档.
.psm ： package symbol ,实体包装零件.
.osm ： format symbol , 制造,组装,logo图形的零件.
.ssm ： shape symbol , 自订 pad 的几何形状,应用在 Padstack Designer.
.bsm ： mechanical symbol , 没有电器特性的零件.
.fsm ： flash symbol , 负片导通孔的连接方式.
.mdd ： module ,模块,可在 Allegro 建立,包含已 placed , routed 的数据.
.sav ： corrupt database,当出现此种档案时,表示你的板子的数据结构已经破坏，情况不严重可以用DB Doctor修复。
文件后缀名
文件类型
.brd
普通的板子文件
.dra
Symbols或Pad的可编辑保存文件
.pad
Padstack文件，在做symbols时可以直接调用
.psm
Library文件，存package>part symbols
.osm
Library文件，存格式化  symbols
.bsm
Library文件，存机构  symbols
.fsm
Library文件，存flash symbols
.ssm
Library文件，存shape symbols
.mdd
Library文件，存module definition
.tap
输出的包含NC drill数据的文件
.scr
Script 和macro 文件
.art
输出的底片文件
.log
输出的一些临时信息文件
.color
View层面切换文件
.jrl
纪录操作Allegro的事件
Cadence SPB 15.5整个软件系统分为18个功能模块：
1)Design Editor
  Design Entry HDL 允许采用表格、原理图、Verilog HDL 设计，是以前版本的Concept HDL
2)Design Entry CIS 对应于以前版本的Capture、Capture CIS
3)Design Entry HDL Rules Checker  Design Entry HDL规则检查工具
4)Layout Plus  原OrCAD的PCB设计工具
5)Layout Plus SmartRoute Calibrate  Layout Plus的布线工具
6)Library Explorer 数字设计库的管理
7)Online Documentation 在线帮助文档
8)Model Integrity 模型查看与验证工具
9)Package Designer 高密度IC封装设计和分析
10)PCB Editor 即PCB设计工具，包括：Allegro PCB Design 220(完整的PCB设计工具：包括Design Entry HDL、PCB Editor、PCB Router)、Allegro PCB Performance 220、Allegro PCB Design 610
11)PCB Librairan  Allegro库开发，包括焊盘、自定义焊盘Shape、封装符号、机械符号、Format符合Flash符号的开发
12)PCB Router   CCT布线器
13)PCB SI    建立数字PCB系统和集成电路封装设计的集成高速设计和分析环境，能够解决电气性能的相关问题：时序、信号完整性、串扰、电源完整性和EMI。
14)Allegro Physical Viewer    Allegro浏览器模块
15)Project Manager   Design Entry HDL 的项目管理器
16)Sigxplorer  网络拓扑的提取和仿真
17)Analogy Workbench(PSpice A/D)
18)PCB Editor Utilities  包括Pad Designer、DB Doctor、Batch DRC等工具。
设计过程：
使用Cadence PCB设计工具创建并完成PCB设计的过程：
1)设置Capture工作参数   为Capture定义和设置工作区
2)制作元器件    创建元器件库
3)创建原理图设计   包括：元器件摆放、网络连接和层次图设计等。
4)PCB设计预处理    包括：属性分配、封装指定、规则检查和网络表生成等。
5)配置Allegro工作环境  
6)建立焊盘与元件封装符号
7)加载网络表    加载Capture生成的网络表
8)建立板框、限制区域和板的叠层    定义PCB配置的物理参数
9)定义设计规则     
10)元件布局，摆放元器件
11)建立VCC和GND平面
12)对关键网络进行交互式布线
13)用SPECCTRA进行自动布线
14)最优化布线    使用Gloss命令优化自动布线的连线
15)完成布线
16)为生产PCB板产生坐标、报表
17)产生生产输出    产生Gerber文件等生产加工数据
PCB封装常见类型
DIP
SOIC
PLCC/QFP
PGA/BGA
TH DISCRETE
SMD DISCRETE
SIP
ZIP
设计流程：
1.前处理
1)原理图设计
2)创建网络表  产生送往Allegro的网络表，包括pstxnet.txt,pstsprt.txt,pstchip.dat。
3)建立元器件封装库
4)创建机械设计图
2.中处理
1)读取原理图的网络表   导入Allegro软件
2)摆放机械图和元件
3)设置电路板的层面
4)手工布线和自动布线
5)放置测试点
3.后处理
1)文字面处理
2)底片处理
3)报表处理   元件报表(Bill of Material Report)、元件坐标报表(Component Location Report)、信号线接点报表(Net List Report)、测试点报表(Testpin Report)等
mil&inch区别？
1inch = 1000mil = 2.54mm
1mm = 0.03937inch = 39.39mil
问：在Allegro中可以打开的文件有几类，各有什么不同？ 
答：在Allegro中可以打开的文件有四类。
      在Allegro中，执行File—Open命令，得到打开(Open)文件对话框，在文件类型选择栏，可以看到可以打开的四种文件类型，它们分别是设计 (Layout)文件，文件后缀为.brd；模块定义(Module Definition)文件，文件后缀为.mdd；设计(Layout)文件，文件后缀为.mcm ；符号绘制(Symbol drawing) 文件，文件后缀为.dra。 
     文件后缀为.brd的文件是 Allegro 的印制电路板设计文件。它可以包含元件的布局、布线、机械加工尺寸、各类定位孔、各类禁止区域信息、装配信息等，并由它最终产生印制电路板加工所需要的各种光绘文件、钻孔文件以及各类工艺文件。 
      文件后缀为.mdd的文件是Allegro的模块文件，它用于建立一个可重复使用的模块，以前文件的后缀为. Module，可以通过执行modpaste命令将文件的后缀为.module改变为.mdd。
       文件后缀为.mcm 的文件是 Cadence 的高级封装设计系统 Advanced Package Designer(APD)或Advanced Package Engineer(APE)保存的设计文件。
       文件后缀为.dra的文件是各种格式符号文件，包括封装符号(Package symbol)、机械符号(Mechanical symbol)、格式符号(Format symbol)、形状符号(Shape symbol)和嚗光符号(Flash symbol)。
PCB设计工具：
1、原理图端：Cadence Capture         Concept_HDL     Protel      Power Logical     DxDesigner   
2、PCB Layout：Cadence Allogro    Mentor Graphics      Power PCB     Expedition       BoardStation  
3、PCB仿真：Cadence SpecctraQuest     Mentor Graphics      Hypelynx  
Cadence基本的元件库：
Discrete.olb
MicroController.olb
Conector.olb
Gate.olb
Epude708.olb   // add
1. 原理图设计用capture
2. PCB用allegro
3. 自动布线用SPECCTRA
Cadence SPB:pcb设计系统的软件：
原理图输入：capture cis & concept HDL
PCB设计： PCB editor
PCB库管理和设计：Part Developer
Lib explorer
PCB Librarian
信号分析：PCB SI(Specctraquest)
Signoise
PCB布线器：PCB router（specctra）
模型编辑验证： Model integrity
