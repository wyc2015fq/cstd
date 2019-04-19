# PowerPCB (PADS)常见问题全集 - xqhrs232的专栏 - CSDN博客
2013年07月02日 23:27:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1259
 原文地址::[http://hi.baidu.com/yingjiaolqf/item/8b01f8cd7e4a423698b498fe](http://hi.baidu.com/yingjiaolqf/item/8b01f8cd7e4a423698b498fe)
相关网帖
1、PADS常见问题答疑----[http://wenku.baidu.com/view/c8dfdbe4f8c75fbfc77db2cb.html](http://wenku.baidu.com/view/c8dfdbe4f8c75fbfc77db2cb.html)
**走线很细，不是设定值`有时将预拉线布好线后，所布的线变成了一根很细的线而不是我们所设定的线宽，但是查看它的属性也还是一样的最小线宽显示值的设定大于route线宽。setup--preferences--global--minimum display 或者使用 R X 这个快捷命令，X表示需要设定的值走线宽度无法修改，提示wrong width value关于线宽的rules设置有误setup – design rules –default—clearance—trace width 修改最小值 默认值 和最大值布线的时候不能自动按照安全间距避开走线没有打开规则在线检查DRO 关闭在线规则检查 DRP 打开在线规则检查**PowerPCB 如何import Orcad 的netlist
Orcad中的tools->create netlist，other的formatters选取padpcb.dll，再将其后缀名.net改为.asc即可。
在PowerPCB 中如何删层
4.0 以下的版本不可直接删层，可将不需要的层上的资料删掉，出gerber时不用出就好了；4.0 以上版本的可直接
修改层数。
**PowerPCB 中如何开方槽?4.0 以上版本的可在编辑pad中选择slot parameters 中slotte来进行设置，但只能是椭圆形的孔；也可在机械层直接标示。**在PowerPCB 中如何将其它文件中相同部分复制到新的文件中
可用以下部骤:
第一，在副图选择要粘贴的目标，按右键选择make reuse ，弹出一个菜单随变给个名字，ok 键即可。生成一个
备用文件。
第二，在按右键选择reset origin (产生选择目标的坐标)将鼠标移到该坐标上可以坐标值（在窗口的右下角处）。
第三，调出主图，将板子的格点改为“1”mil。按make like reuse 键，打开第一步生成的文件后，用“S”命令敲入第
二步生成的坐标。按左键确定。在贴完后，在按鼠标右键点击break origin。弹出一个窗口按“OK”即可。
**如何在PowerPCB 中加入汉字或公司logo将公司logo或汉字用bmp to pcb将。bmp档转换为protel的。pcb格式，再在protel中import，export *.dxf文檔，在PowerPCB中import即可。**如何在PowerPCB 中设置盲孔
先在padstack中设置了一个盲孔via，然后在setup -- design rules -- default -- routing的via设置中加入你所设置的
盲孔即可。
hatch和flood 有何区别，hatch 何用？如何应用
hatch是刷新铜箔，flood是重铺铜箔。一般地第一次铺铜或file修改后要flood，而后用hatch。
铺铜（灌水）时如何自动删除碎铜
1)setup-preferences-Thermals中，选中Remove Isolated copper；
或 2) 菜单Edit—Find---菜单下Find By--Isolated pour—OK
如何修改PowerPCB 铺铜（灌水）的铜箔与其它组件及走线的间距
如果是全局型的，可以直接在setup － design rules 里面设置即可，如果是某些网络的，那么选中需要修改的网
络然后选右键菜单里面的show rules进入然后修改即可，但修改以后需要重新flood，而且最好做一次drc检查。
PowerPCB 中铺铜时怎样加一些via 孔
可将过孔作为一part，再在ECO下添加part；
也可以直接从地走线，右键end(end with via)。
**自动泪滴怎么产生？需对以下两进行设置:1) setup->preferences->routing->generate teardrops->ok2) preferences->Teardrops->Display Teardrop->ok**
**最坏灰太狼备注：文章最后上图（1）（2）说明pads2007中怎么添加去除泪滴**
手工布线时怎么加测试点?
1) 连线时，点鼠标右键在end via mode 中选择end test point
2) 选中一个网络，然后在该网络上选一个合适的过孔修改其属性为测试点，或者添加一焊盘作为测试点。
PowerPCB 怎么自动加ICT
一般地，密度比较高的板都不加ICT。而如果要加ICT，可在原理图里面设置test piont，调入网表;也可以手工加。 十
**为什么走线不是规则的?设置setup/preferences/design/，选diagonl;将routing 里面的pad entry项去掉。当完成PCB 的LAYOUT，如何检查PCB 和原理图的一致在tools->compare netlist，在original design to compare与new design with change中分别选取所要比较的文件， 将output option下的Generate Differences Report选中，其它选项以自己实际情况来选取，最后run即可。**在PowerPCB 中gerber out 时多出一个贯孔，而job file 中却没有，这是怎么回事
这应为PowerPCB的数据库太乱了，可能是修改的次数太多造成的。解决方法可export *.asc文件，再重新import
一次。
**如何直接在PowerPCB 3.6 下生成组件清单通过File－Report－Parts List1/2。如何将一个器件的某个引脚的由一条网络改为另一条网络？打开eco，用delete connettion删除原来的连接，不要删除网络噢。再用add a connetion添加一个连接。**如何对已layout 好的板子进行修改?
为确保原理图与PCB 一致，先在原理图中进行修改，然后导出netlist，再在PCB 中导入，但要注意，如果要删
除某些网络或零件，则需手动删除。
**CAM Gerber 文件时（SOLDER MASK BOTTOM）出现“maximum number ofapertures exceeded”的提示，无法输出文件?选中你想要输出gerber的层，进入edit document对话杠，再选 device setup (前提要在photo状态下)，在photo plottersetup对话框的下方有一个aperture count项，在其后输入数字，然后regenerate即可。PowerPCB gerber out 时*.rep，*.pho，*.drl，*.lst 各表示什么意思*.pho GERBER数据文件*.rep D 码文件（线，焊盘的尺寸，必不可少的）*.drl 钻孔文件*.lst 各种钻孔的坐标以上文件都是制板商所需要的。**PowerPCB 如何能象PROTEL99 那样一次性更改所有相同的或所有的REF 或TXT
文字的大小？还有，怎么更改一个VIA 的大小而不影响其它VIA 的大小？
可以通过鼠标右键选择“Document”，然后就可以选中所需要的ref 或文字了。如果要更改一个Via的大小，需要
新建一种类型的Via。
**PowerPCB 如何打印出来?可以在菜单File－Cam……中进行，建立一个新的CAM Document后，然后Edit，Output Device选择Print，运行RUN即可。一般先进行打印预览（Preview Selections），看是否超出一页的范围，然后决定是否缩小或放大。**请问PowerPCB如何设置才能在走线打孔的时候信号线自动用小孔，电源线用大孔？
先在PAD STACKS中将你要用的VIA式样定制好，然后到Desing Ruels中先定义Default Routing Rules使用小的
VIA，再到Net Ruels 选中电源的Net，在Routing中定义成大的VIA。如不行，可以敲入“VA”，将VIA Mode设
成Automatic，它就会按规则来了。
**要想在PowerPCB 中放置单个焊盘，是否就要在组件库中做一个单焊盘的组件？不一定，如果单个焊盘有网络连接，则可以改成放过孔，毕竟放组件不利于DRC。放过孔的方法：选中某一网络（NET），单击右键，选ADD VIA即可，可以连续放多个。最好打开在线DRC。PowerPCB 在铺铜时画铺铜区时，如要在TOP 和BOTTOM均要铺GND 的铜，是**否需要在TOP 和BOTTOM 分层画铺铜区后，再分层进行灌铜？
在灌铜时，各层均需要分别画铜皮框，如果一样的外形，就可以Copy。画完后现在Tools下的 Pour Manager 中
的 Flood all 即可。
GND 的过孔或者器件的插脚铺铜时只有单面GND 连通所铺的铜
你可以到Setup－preference－Thermals选项中，将“Routed Pad Thermals”选项打勾
PowerPCB 3.5 中的菜单Setup ---> Design ---> Rules---> Default---> Routing 中
Vias的Availabe和Selected 匀为空白，在此情况下是没有过孔，各层无法连接。请
问怎样设置过孔？
那你就新建一个VIA类型SETUP->PAD STACKS->在PAD STACK TYPE 中选VIA->ADD VIA……然后Setup
--->Design --->Rules---> Default---> Routing 中，把新建的VIA添加到SELECT VIA!
如何在PowerPCB 中象在PROTER 中一样，将一组器件相对位置不变的一起旋转？
操作之前，敲入"DRI(忽略DRC)"或“DRO(关闭DRC)”，然后必须先将需要选转的器件和线等选中，然后定义为
一个Group，然后就可以点击右键进行Group的旋转操作了。（鼠标点击处为旋转的基准点！）
在PowerPCB 4.0 里有几个图标，其提示分别为plane area， plane area cut off ，
auto plane separate ，他们有什么功能啊？另外我见到有的PCB 里，用plane area
cut off 画个圆，PCB 生产时就是一个孔?
这是在内层作分割时用的几个命令，第一个为在内层指定分割的区域，第二个为在区域里挖除块，第三个是有几
个区域你先全定义为一个然后再从这一个上去分。在PowerPCB 中是否有对各层分别进行线宽的设置吗？
可以的！
design->default设置缺省值。
design->conditional rule setup生成新的条件规则：按照你的说明，应该选source rule object：all
against rule object：layer/bottom
点击create，生成新的规则。按要求修改，OK！
**在PowerPCB 布线时，违背了规则中定义的走线方向的走线往往会出现一个菱形的小框以作提醒，但我发现在一块板子的设计过程中难免会出现这样的情况，请问此情况对设计以及以后的制板有什么影响吗？出现的小菱形说明，布线没有定位在网格点上。这是很正常的情况，对设计以及以后的制板没有任何影响。此功能可以在 setup - preferences - routing 中取消 show tacks 选项，小菱形就不会出现了！**4 层板，如果有几个电源和地，是否中间层要定义成split/mixed？
我们一般都不使用CAM Plane设置，均设置为信号层，这样在以后的电源分割时可以随意分割，较方便！
如果是6 层板，走线和电源地层怎么分配？是不是线、地、线、线、电、线？
6 层板如果一定要走4 层信号线的话，肯定有两个信号层相邻。可以采用你上面的迭层方式，也可以采用：Signal1、
GND、Signal2、VCC、Signal3、Signal4，这样，Signal2可以走一些要求较高的信号线，如高速数据和时钟等。
**PowerPCB 的25 层有何用处？POWERPCB的25 层存储为电源、地的信息。如果做多层板，设置为CAM PLANE就需要25 层的内容。设置焊盘时25 层要比其它层大20MIL，如果为定位孔，要再大些。在PowerPCB 的Dynamic Route 状态下，有时新的走线会影响走完的线。而且有时走线并不随鼠标变化，总是走出一些弯弯曲曲的线。我觉得这个问题可以说是问题也可说不是，因为PowerPCB中有几种布线方式，除了Dynamic Route还有一般的走线和总线布线和草图布线，这几种布线方式应该结合来用，才能达到好的效果，有时候Dynamic Route走的线很难看，这时候我通常采用一般的走线方式，一般能达到好的效果；有时候一般走线方式走的很难看或很难走通，又应该用DynamicRoute，结合几种走线方式才能使得板子走线美观！**
在布线过程中，如果打开DRP，有些芯片的管脚引不出线来，但是鼠线是确实存在
的。如果关掉DRO，这个管脚就能引线出来了，是为什么？哪有设置？
设计规则中的设置值太大了（如Pad到trace、trace到trace等安全间距设置等），或者默认走线的线宽值设置的太
大了，而芯片管脚的间距又小。都有可能造成上述问题。
用PowerPCB 铺铜时发现一个问题，就是如果在一个copperpour 的outline 里面再
画一个copperpour 的outline，里面的copperpour 在做foolding 时就不会被铺铜。
这是正常的情况。两个copper pour如果是不同的网络，不能相互包含。
在这种情况下，只能通过画几个互相不包含的铜皮框来解决。
如何在PowerPCB 中加入埋孔？
在Via的设置中，先增加一种过孔的名称，然后对其进行设置：在“Through”和“Partial”的选项中选择“Partial”，通过以下的两个“Start”和“End Layer”选项就可以设置盲埋孔的开始层和终止层了。
但是如果使用盲埋孔的话，会增加PCB板成本。如果可以不用的话，尽量不用！省钱！
为什么PowerPCB 中铺铜有时是整块，有时是网状，应该在哪里设置？
当你使用灌铜的线宽大于或等于线间距时，就为实铜；当灌铜的线宽小于线间距时，就为网格铜。线宽在铜皮框的属
性中就可看到，线间距在Preferences中设置。
PowerPCB 里NC Drill 和Drill drawing 层有什么区别？
NC Drill 是一些钻孔数据，提供给钻孔机使用。
Drill Drawing 是一个钻孔图表，可以直接由Gerber来看钻孔大小，钻孔数，位置。
PowerPCB 中走线怎样自动添加弧形转角？
在走线过程中点击右键，选择“Add Arc”即可。
**PowerPCB 的内层如何将花孔改为实孔？修改相应的铜皮框的属性，在Preferences中将“Flood Over Via”选项勾上即可！在PowerPCB 中，pin number and pin name(alphanumberic)有什么区别pin number：只能是数字1、2、3 ……pin name(alphanumberic)：可以是整数，也可以是字符。原理图中好多组件管脚是以字符命名的(如BGA器件的管脚)，那么你在做pcb的组件库时就给组件字符名。这时你就要去定义pin name为字符。做组件时，如何将组件的管脚号由数字（如1，2，3）改为字母（如b，c，e）?file/library/parts/edit/general/，在options里的Alphanumeric。。。。。。打钩，选Alphanumeric pin项，在name处填上相应的字母。注意name要与number对应。**有没有办法让文件中的丝印字符（Ref）排列整齐？
没有，只能自已动手排啦！
定义了几种过孔，将菜单SETUP/ DESIGN RULES/ DEFAULT/ ROUTING/ VIA 也
已经进行了设置，可是为什么选择via type 时其它的都看不到，只有standardvia 呢
应该设置默认项。SETUP--> DISIGN RULES-->Default-->ROUTING-->把要用的过孔加到selected区。
**PowerPCB 中怎样在铜箔上加via 呢?1、把via当作组件，并给过孔添加到gnd 或电源的connection。2、从地或电源引出，用右键end via mode的end via 添加过孔。PowerPCB 图中内层GND 的铜箔避开Via时，为什么Gnd 的信号的Via 也会避开Preferences->thermals->pad shape下在round，square，rectangle，oval都选择flood over。**
PowerPCB5.0 为什么覆铜会将所有的孔都盖住，而不是让开孔？？而且这种现象都
是时有时无，也就是说和设置无关，
导出asc文件——>将ASC文件导入PowerPCB3.6（存为.pcb文件）——>用PowerPCB4.0打开——>问题解决。我试过
将ASC文件导入PowerPCB4.0，但问题依旧存在。（没试过将ASC导入PowerPCB5.0）
在power pcb 中如何针对层设置不同的线宽。
设置步骤如下：
首先是按通常方法设置缺省值，可设置为10mil（即顶层希望的线宽）。set up-----design rules----conditional rule setup 。
若希望底层的所有线宽均为12mil，则source rule object选择all，against rule object选择layer bottom。
点击creat，matrix，出现线宽线距设置对话框，可作相应设置。
**铺铜后，发现pad孔与铺铜断开,不知是哪儿设置不对？Preferences->thermals->routed pad thermals打钩。**
怎样使用PowerPCB 中本身自带的特性阻抗计算功能？
1、在setup/layer definition中把需要定义为地或电源层相应层定义为CAM PLANE。
2、并在layer thinkness中输入你的层迭的结构，比如各层的厚度、板材的介电常数等。
通过以上的设置，选定某一根网络并按CTRL+Q，就可以看到该网络相关的特性阻抗、延时等。
**用PowerPCB 自动布板（BlazeRounter）时，能否设置倒角（135度）选项？BlazeRounter是先走直角，然后通过优化成倒角，所以倒角的大小是可以设置的。1）Tools ——BlazerRouter ——Routing Strategy—— Setup；2）在Miters的选项中相应的项打勾。**可以将现有pcb 文件中的器件存入自己的器件库中吗？
可以。打开pcb文件，选择想保存的器件，点击鼠标右键，在弹出的菜单中选中save to library，在弹出的对话框中
选择想要存入的库，ok！
在PowerPCB 中如何快速绕线？
第一步：在setup/preferences面板的design下的miters中设置为arc，且ratio为3.5。
第二步：布直角的线。
第三步：选中该线，右击鼠标，选中add miters命令即可很快画出绕线。
**在PowerPCB 中如何快速删除已经定义的地或电源铜皮框？第一步：将要删除的铜皮框移出板外。第二步：对移出板外的铜皮框重新进行灌水。第三步：将铜皮框的网络重新定义为none，然后删除。对于大型的pcb板几分钟就可以删除了，如果不用以上方法可以需要几个小时。友情提示：如果用PowerPCB4.01，那么删除铜皮的速度是比较快的**PowerPCB4.0，将外框*.dxf 的文件导入，之后文件很容易出现数据库错误，怎么办？
好的处理办法是：把*.dxf文件导入一个新的pcb文件中，然后从这个pcb文件中copy所需的text、line到设计的
pcb文件中，这样不会破坏设计的pcb文件的数据。
**PowerPCB 中可以自动对齐器件吗？对齐元器件可以先选中多个器件，然后点击右键，选择Align...功能可以进行各个方向的对齐；选择Create Array可以设置组件排列间距参数，然后进行排列。执行过creat array的几个器件，将会成为一个联合体，下次想单独移动某一器件时，需要先从右键菜单中选择breakunion。**PowerPCB 里除了自动标注尺寸外还有没有别的测距方法？
可以用快捷键Q，也可以使用ctrl+PageDown。
**PowerPCB 中怎样给器件增加标识？选择该器件，按右键选择Query/Modify，左下方选择"Labels"，选择"NEW" ，增加，即可。另一种简单方法：选择器件后，直接按右键选择Add NEW Labels，进行相应操作就可以了。PowerPCB 组件库中组件外形应该在丝印层还是在 all layers组件外形最好定于 all layers ，这样不会出问题。**在PowerPCB 里如何打方孔？
PowerPCB只可以定义圆孔或长椭圆孔。
如果孔边不需要焊盘，可用board outline and cut out 命令画出即可；
若是想要带焊盘的方孔，可以用2D LINE在钻孔层画一个你所需要的方孔，在两个布件层放置想要的贴片焊盘即可。
若在旁边附上说明文字，就更加清楚了。
对含有特殊形式内孔的焊盘，可也参照上述方法制作。
**powerpcb 里NC Drill 和Drill drawing 层有什么区别？NC Drill 是一些钻孔数据,提供给钻孔机使用。 Drill Drawing 是一个钻孔图表,可以直接由Gerber来看钻孔大小，钻孔数，位置。**PowerLogic 中有copy 功能吗
1）先将选中的原理图部分做“Make Group”，然后再将其“Copy to File”。然后在需要粘贴的
新页中，选择“Add Item－Paste From File”即可。以上操作均使用右键菜单。
2）PowerLogic每次只能打开一个文件，但是可以打开好几个PowerLogic程序，这样你可以打开以
前地设计，选择要拷贝地部分，然后使用右键菜单make group以下，再使用Ctrl＋C复制，在新
的设计中Ctrl＋V一把，搞定！！！
PowerPCB中也有效的。
3）PowerLogic要拷贝相同地部分，也可以在整个窗口画面左下方 ，最左边第一个功能键， 把他
按下去 ，然后你使用鼠标所拖曳选择的部份，就会自动成为group， 这样就可以拷贝了。
PowerLogic 怎样自动重新Annotation?
PowerLogic中不能自动标注，而PowerPCB可以进行自动标注，产生eco文件后，在PowerLogic
中进行反标注！
**最坏灰太狼备注pads2007 试用版**
**图1**
**开打菜单按图点击**
**![](http://hiphotos.baidu.com/%D7%EE%BB%B5%BB%D2%CC%AB%C0%C7/pic/item/cc87c734b1c3790f0a55a9dc.jpg)图2**
**点击“Options...”后点击图2。如果要去掉泪滴把图中2个项目勾选去掉即可**
**![](http://hiphotos.baidu.com/%D7%EE%BB%B5%BB%D2%CC%AB%C0%C7/pic/item/1d767c0a2b141dab0a7b82ae.jpg)图3**
**是泪滴外形的相关设定，有自动的，有用户的。**
**![](http://hiphotos.baidu.com/%D7%EE%BB%B5%BB%D2%CC%AB%C0%C7/pic/item/39c6fb118f4e091ddd5401b6.jpg)**
