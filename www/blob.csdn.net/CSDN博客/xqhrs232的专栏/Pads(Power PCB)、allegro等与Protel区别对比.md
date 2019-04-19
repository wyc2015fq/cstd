# Pads(Power PCB)、allegro等与Protel区别对比 - xqhrs232的专栏 - CSDN博客
2013年07月10日 11:25:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2605
原文地址::[http://www.pcbdown.cn/html/jishuwenzhang/20110627/4239.html](http://www.pcbdown.cn/html/jishuwenzhang/20110627/4239.html)
- 
【基本区别】
Protel自由，可以单独放置过孔、焊盘。器件；PADS中不支持放置焊盘，过孔也只有在选择好网络后才能放置与所选网络相连的过孔。
Protel怎么设置过孔类型？好像不可以设置
如何查找器件？filter菜单打开，右上，下拉里面选择器件
pads里面是ss，find里面选项很多
pads的bug比较多，经常出错
AD6中能在焊盘和走线铜皮上显示网络名称，PADS 最新版本（PADS2007）也只能在焊盘上显示器件的管脚名称。
AD6中支持在PCB上单独放置过孔、焊盘，而且过孔和焊盘可以随意编辑，而PADS中不支持放置焊盘，过孔也只有在选择好网络后才能放置与所选网络相连的过孔。
【自从见到了PowerPCB就再也不想用Protel了】
　　本人95年开始接触protel，那时候版本是3.16[dos]，后来升级到3.31，中间接触了很多windows版本的protel，但都没适应，因为dos版用得太熟了，那些快捷键简直就像是长在手上一样，根本不用脑子手指头就敲出来了。99年底换上了99se，用得还算熟练。2000年底一个非常专业的朋友推荐了powerpcb（他在单位用的是psd，正版的），用blazerouter给我演示了一下动态布线，我一下子就被吸引过去了，天哪，交互布线居然可以这样！用推挤可以在密密麻麻的线路走出一条漂亮的网络，我试了试，布线效率是protel的一倍还要高，而且非常美观。从此再也不用protel了。 
　　后来接触了很多其他的软件，如allegro、wg、orcad、pcad，也看过别人演示en和cr5000。对这些软件基本有一些基本的印象。现在的pcb软件分为两个档次，
高档的是en、wg、allegro、cr5000，这些软件的特点是规则约束非常严格，布线算法非常优秀，
前一个特点对 protel熟练的朋友来说是非常不习惯的——还没画板子就要大把大把的设置，要是用protel我的板子恐怕已经完工了。
其实对于一个复杂的有数千个甚至更多、或者高频高速的板子来说，规则的约束是非常重要的，是一次成功完成pcb设计的必要保障，即使原理图是自己画的也无法保证准确理解几千个网络的正确含义。
　　低档软件中、powerpcb是出类拔萃的，尽管交互布线的速度跟wg相比显得太慢了。而protel除了界面花哨以外，没看出来有什么过人之处。而orcad和pcad比protel还要差一些，尽管我认为orcad是最好的原理图软件。
　　本人这些年对pcb软件的理解是：设计pcb的核心内容是拉线，只有拉线爽的软件才是好软件！！！
　　不过诸位protel粉丝也不要太伤心，如果你的设计基本上围绕直插器件、只有低密度smd元件、双面板，那么protel还算是不错的工具，没有太大必要更换。如果你觉得protel的自动布线太烂，也没关系，用specctra来自动布线，这两种软件结合得很好。如果你的设计是四层以上，含有大量 bga一类的高密度封装，那么你不要犹豫，赶紧换软件，这会大大提高你的工作效率。推荐三种：1、wg，拉线之王；2、pads（powerpcb）最好学，书籍也最多；3、allegro高速板事实上的工业标准。 
_)(^$RFSW#$%T
【？】
最近安装使用了dxp 2004，感觉比99se好多了，powerpcb用了几次，不熟悉，就放弃了，不过还是想了解这2个软件的异同，下面是我的使用感受，同时想听听大家的看法，
我觉得还是protel dxp 好，功能强大，例如：双向查找，点击可以在pcb和sch之间切换；原理图看起来比较舒服，可以用鼠标右键拖动原理图；原理图和pcb之间的差异显示直观，相互更新顺手，例如在原理图中把三极管的封装库的2个管脚对应关系调换了，更新下pcb即可；自动布线规则设置不错，在设计原理图时就可以定义布线时的参数等，你可以定义每一条线的参数；总体感觉dxp是windows下的软件，POWERPCB界面黑糊糊的，感觉就是DOS基础上发展来的。
不过protel dxp自动布局方面不太好用，主要还是自己搞，再者dxp软件安装体积太大了，我的有2.10G！（我打了所有的DXP布丁，布丁有3，4百兆，主要是库类，安装软件就1G多），机器资源要求高。还有一个问题我不会用：就是如何定义自动布线时，过孔不能放置在贴片下？
->关于与原理图交互。powerpcb可以与powerlogic、viewdraw、orcad(orcad需安装pcbnavigator)三种原理图软件交互。
OrCAD的原理图看起来更舒服。
自动布局，没有哪种软件好一点的，都要靠人工。 koooooo
个人观点，Protel最好的版本是99SE，DXP和2004都是垃圾。当然99SE也很垃圾，跟其他软件相比。
相对Protel，PowerPCB最大的优势就是布线，动态布线。用Protel的人只知道人工布线和自动布线，不知道动态布线，因为Protel的交互布线就是拖着OnlineDRC的人工布线，一点交互都没有。
pads界面粗糙，库少，行话多，不少功能靠蒙出不来、得看手册或教程，缺点是不少。但是能做出很好的产品来，板子形状任意，器件角度任意，加上HyperLynx，处理一般的板子挺够用的了
PowerPCB本身的动态布线确实很糟糕，要想很舒服的布线，利用强大的推挤功能，必须切换到BlazeRouter里面去。而BlazeRouter只能动态布线和自动布线，调整元件布局和设定规则，又必须切换回到PowerPCB。- 
为什么不能像其他软件那样，把BlazeRouter集成到PowerPCB里面，不是像现在这样来回切换？估计就是个工作流程问题，我现在已经很习惯 PowerPCB了，摆零件，设规则，用BlazeRouter自动布线来检查布局是否合理，当所有的准备工作都完成之后，最后打开 BlazeRouter正式开始布线。准备工作非常繁琐，有点像某些宗教信徒做仪式前净身沐浴。开始布线之后就很爽了，BlazeRouter的推挤还是非常优秀的，除了速度慢以外，可以比美Allegro和WG。
作为入门级的专业软件，PowerPCB的流程还算是简单的，WG比PowerPCB麻烦的更多，Allegro则比WG麻烦得多，据说还有天下最麻烦的EN，没用过。CR5K也是一种专业软件，估计也好不到哪去，楼主应该只是不适应POWERPCB的流程而已。
相比之下，业余级的软件Protel则基本上没有工作流程，想怎么办就怎么办，乍看上去很爽，但是非常糟糕的布线算法，几乎不存在的动态布线，还有规则约束的不严肃，使得他只能是一种业余的软件，不适合大的公司做复杂正式的产品。
OrCAD的原理图确实不错，大家都这么说，但其LayoutPlus个人感觉还不如Protel。
【AD6(protel）与PADS基本功能比较】
AD6(protel）与PADS基本功能比较 
一、 原理图部分 
1、 库 
⑴ DxDesigner 的原理图库与PCB的库是相互独立的，而且每个原理图符号库都是一个文件，很难实现统一管理；AD6可以使用集成库来统一管理，不仅是原理图符号库与PCB封装库，还能把混合电路仿真库、信号完整性分析模型库以及3D模型库一起实现统一管理。 
⑵ DxDesigner 在建库时有建立向导，可以引导使用者迅速建立元器件符号库，在向导中可以从Office文档中拷贝器件的Database，从而一次性完成管脚的输入；AD6虽然没有向导功能，但是可以使用smart grid paste/insert功能可以达到同样的效果。 
⑶ DxDesigner在做Fracture符号库时必须添加关联属性，而且在调用时选择不方便，尤其在放到原理图上以后不方便换，AD6在做Fracture符号库很方便只要选择Add part就可，而且符号库放到原理图上以后非常方便更换。 
⑷ DxDesigner只能实现与ODBC数据库的关联，不能创建真正的数据库，而AD6两者都可以。 
2、 原理图编辑 
⑴ DxDesigner和AD6 都支持层次化原理图设计，但是DxDesigner 不支持多通道设计，在DxDesigner里面必须使用拷贝复制功能来实现多路设计，而AD6只需添加Repeat属性即可。 
⑵ DxDesigner在设计界面下没有查找功能，只能到数据表格中去查找 ，查找后批量修改比较麻烦，AD6 可以通过find similar 功能灵活实现全局查找，而且修改不同的参数也非常方便。 
⑶ AD6中有Snippets摘录功能，实现同一项目及不同项目的拷贝复用，DxDesigner同一项目中只能使用简单的复制 ，不同项目中只能对整张原理图进行复用。 
⑷ AD6 中支持器件的管脚交换、与FPGA设计的链接（如原理图器件 的管脚定义可以直接来源于FPGA器件商的引脚约束文件），DxDesigner必须使用中间工具IO Designer来完成与FPGA器件的引脚约束文件的同步。 
⑸ AD6支持Smart Paste功能，可以对文本、网络标号、端口号、注释等拷贝后进行相互间的转换，这也是AD6独有的特点，DxDesigner 是无法实现的。 
⑹ AD6的快捷菜单、快捷键都是用户可自定义的，而且非常简单，DxDesigner里面的快捷键必须通过修改安装文件才能实现，快捷菜单不能进行自定义。 
⑺ AD6中的混合电路仿真功能比较强大，支持XSPICE/PSPICE仿真模型，仿真类型有：直流工作点分析、瞬态/傅立叶分析、直流扫描分析、交流小信号分析、噪声分析、直流传输函数分析、温度扫描分析、参数扫描、蒙特卡罗分析，而且使用的集成库中包含各种模型，这样使用同一原理图即可以仿真又可以做 PCB设计，DxDesigner现在使用的DxSim仿真器是原来的Analog Design, 库文件比较少，仿真类型不多，还很难实现PCB和仿真使用同一原理图。 
⑻ AD6中可以实现两个设计的比较，DxDesigner 不能实现，只能到PCB中去比较两个网表。 
⑼ DxDesigner在输出BOM材料清单时，虽然通过变量派生模块可以输出Excel、HTML等格式，但是输出属性项没有选择性，AD6在格式上灵活多样，在输出属性项方面更是灵活选择，可以从数据库中选择，也可以从PCB中选择。 
⑽ DxDesigner中可以实现元器件的Scale（缩放）和输出EDIF格式的导入导出，AD6目前还不支持此类功能。 
二、 PCB部分 
1、 库 
⑴ AD6中有IPC标准封装库向导，支持BGA、CFP、QCFP、PLCC、CHIP、SOIC、SOT23、SOP等多种标准类型，PADS中只有DIP、SOP、BGA等少数几种不是IPC标准的封装向导。 
⑵ AD6中器件的焊盘形状支持多边形，过孔也支持正方形，而PADS中都不支持，只能通过不正规的方法来实现。 
2、 PCB设计 
⑴ AD6中能在焊盘和走线铜皮上显示网络名称，PADS 最新版本（PADS2007）也只能在焊盘上显示器件的管脚名称。 
- 
⑵ AD6中支持在PCB上单独放置过孔、焊盘，而且过孔和焊盘可以随意编辑，而PADS中不支持放置焊盘，过孔也只有在选择好网络后才能放置与所选网络相连的过孔。 
⑶ AD6中有统一的封装管理器对当前PCB中所用到的封装进行统一管理、编辑、修改，PADS中只能单独或同类型的一起修改。AD6 同样可以使用find familiar功能进行选择性查找和修改，PADS中没有此功能。 
⑷ AD6中有统一的敷铜管理，对设计中的敷铜区域进行统一管理，PADS中没有这种管理，但是它在Plane层上支持正片，并且能自动分割。 
⑸ AD6中能实现和FPGA设计的同步和正反标注，PADS中做不到，因为它不包含FPGA的设计模块。 
⑹ AD6中还有Board Insight放大显示和悬浮显示功能、切片功能、逃逸式布线功能、3D显示功能、切割选择功能、PCB翻转功能，这些都是非产实际的、对提高设计质量和效率的功能，但是PADS中都不支持。 
⑺ AD6可以输出ODB++的数据格式，但PADS中不能输出次格式，而这正是衡量工具档次的标准。 
⑻ AD6带有CAM编辑功能，不仅可以对当前PCB进行编辑也可以导入其它Gerber文件或ODB++文件进行编辑，还能进行规则检查和验证，PADS必须与其它CAM工具（如CAM350）统一使用。 
⑼ 目前高档的PCB工具都带有拼版功能来满足不同设计的需求，AD6不仅能对统一设计进行拼版，还能对不同设计进行拼版，而PADS只能通过Mentor的CAMCAD工具来完成拼版功能。 
⑽ PADS中的布线器与PCB的基本设计环境是相互独立的，数据交换不能实现双向同步，布线器做好的设计必须先存盘后再用PCB设计环境打开。而AD6的各个环境都是无缝连接的，能很好地实现真正的数据同步。 
⑾ PADS中可以实现自动变线宽的功能、裸片设计模块（Bonding）和RF设计模块，目前AD6还不支持自动变线宽和裸片设计，但RF的功能可以实现其中的一部分（如：加屏蔽线）。 
三、 高速分析 
1、 布局布线前分析 
AD6支持信号完整性分析、串扰分析、终端匹配参数扫描分析 
Hyperlynx的LineSim支持信号完整性分析、串扰分析和电磁兼容性分析 
2、 布局布线后分析 
AD6支持信号完整性分析、串扰分析、终端匹配参数扫描分析 
Hyperlynx的BoardSim支持信号完整性分析、串扰分析和电磁兼容性分析0402 @ 2008-9-09
PADS虽不能直接输出ODB++，但可以通过别的方式实现，如先输出CAM文件，再用CAM350读入并输出ODB++文件
【EDA公司介绍】 
　　早期全球EDA企业有1000多家，后来发展到10家左右，其中Cadence、Mentor、Zuken主要是高端产品，他们的软件要求在工作站上运行，操作系统都是unix，而且价格昂贵。因为80年代就有EDA软件了，那个时候只有UNIX支持图形界面，并且工作站的性能要比PC机高出很多，所以一直延续至今，现在的大公司还是用工作站。而protel,powerPCB他们主要面向低端用户，对计算机的配置要求不高，一般在windows下运行。一般的PC机就可以很好的满足要求了。
　　随着CPU和相关电脑硬件水平的不断提高，Cadence、Mentor、Zuken开始推出windows下的产品，这方面Cadense走的比较快。好像是2000的样子吧，由于互连网泡沫的破裂，EDA产业进行了从新洗牌，上面的几家公司进行了重组。
　　从市场占有率来说， Mentor公司现在最高，Cadence公司第二，Zuken 公司第三。单个的PCB工具，Allegro在中国高端用户中软件占有率应该是最高的，其次是PowerPCB、Protel ，在中国大陆使用人比较多。
　　具体来说：Mentor公司的产品是boardstation(en)和expeditionpcb(wg)，以及收购来的 Pads(PowerPCB)。en是传说中的pcb无敌高手，那些只考虑工期不考虑成本，总是做8层～12层pcb的通讯和军工研究所必杀绝技。wg好像所有的bbs都同意这是地球上最好的布线工具。powerpcb就不说了，用的人也不少。Mentor公司收购了PowerPCB后，继续两条腿走路，高端的还是原来的Mentor,现在最新版 Mentor EN2006；低端的产品还是PowerPCB，新名字叫PADS2005,最新的叫PADS2007，但是PADS2005SP2是一个比较稳定的版本。 
　　还有德国一个小软件非常流行，是EAGLE，在欧美很有市场。
　　Cadence公司的产品是concept/allegro和收购来的orcad。allegro最近很火爆，特别是现在计算机主板以及显卡等附加值高的产品都是allegro格式，尽管三五年前powerpcb才是业界标准。 Cadence公司收购了ORCAD，并将orcad的强项原理图设计capture CIS和CADENCE原来的原理图设计concept HDL，PCB工具allegro及其它信号仿真等工具一起推出并统称为CADENCE PSD,现在叫SPB，最新版本16.0 ,ORCAD也集成到了SPB里了，15.5开始就没有ORCAD这个概念了，以前的ORCAD
 CAPTURE CIS 现在叫 DESIGN ENTRY CIS。
　　Zuken是另一家EDA大鳄,是日本的，高端产品cr5000,低端的叫CADSTAR 除了日资和与日本有业务往来的企业外还有很多公司用zuken的软件，国内的一些研究所以及一些老的电视机企业在用，LG也在用ZUKEN的工具，NOKIA也在用。
- 
Mentor :
　　1.Powerlogic+Powerpcb；2.viewdraw + Expedition；3. Boardstation。现在MENTOR主推的原理图是DxDesigner（viewdraw的升级版本）,PADS 和Expedition都用这个， Boardstation 的原理图工具是Design Archive 和Board Archive。
Cadence:
　　 原理图工具主要有Capture和Concept，PCB工具有Allegro Layout Plus。
Zuken: 
　　1. Cadstar；2. CR3000、CR5000。CR3000升级成了CR5000，CR5000有两个PCB工具，PWS和BD，截至到2007年，CR5000的版本是BD10.0, PWS18.0, CADSTAR9.0。
大公司使用的工具:
　　Intel：Concept+Allegro+ SpecctraQuest
　　Dell：viewdraw+ Allegro + SQ原理图也有一部分是Capture
　　Huawei：viewdraw+ Allegro + SpecctraQuest+Expedation
　　ZTE：Concept+Allegro+ SpecctraQuest+Expedation
　　UT：Concept+Allegro+ SpecctraQuest 手机部用的是PowerPCB
　　Csico：Concept+Allegro+ SpecctraQuest
　　Hp：Concept+Allegro+ SpecctraQuest 从Boardstation转成Alllegro 流程
- 
　　Moto：Concept+Allegro+ SpecctraQuest 从Boardstation转成Alllegro 流程
//============================================================================================================================
备注：：
1》PCB布线的方式----人工布线++++自动布线++++动态布线/交互布线
