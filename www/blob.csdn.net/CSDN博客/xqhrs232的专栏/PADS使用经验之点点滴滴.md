# PADS使用经验之点点滴滴 - xqhrs232的专栏 - CSDN博客
2013年07月15日 14:32:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1916
原文地址::[http://www.52rd.com/bbs/Archive_Thread.asp?SID=261521&TID=3](http://www.52rd.com/bbs/Archive_Thread.asp?SID=261521&TID=3)
1.    鼠线的处理
Layout的缺省设置并不是让飞线最短化。一开始布元件时，飞线实在是密如蛛网，晕头转向。Tools\length minimization (CRTL＋M) 也没有用，硬着头皮在缺省设置下完成了元件布局。几欲faint。后来才发现其实没有设置好。正确或者说方便的设置应该是让飞线最短并且在移动中始终最 短。
Setup\design rules\default rules\routing\ topology type\minimize 这样在按CTRL＋M，很多飞线就消失了，也就最短。
2.    移动中鼠线处理
Setup\preference\length minimize\during moving 这样移动元件是飞线始终最短。很多飞线其实是地线。可以把地线的飞线先hide起来。并把地线的net设置成比较特殊的颜色。这样就布局就方便多了。*
3.    字体属性的修改
单击鼠标右键，选select components, 再单击右键，选 select all （CTRL＋A）。再单击右键，选query/modify (CTRL+Q)， part outline width 输入想要的宽度，下面选label， 选Ref. Des. Press the big button under it. 弹出新窗口， input the value you want at size and width. Press OK. Then the size, width , even the part outline width are same.
4.    加过孔
击老鼠右键，check “select net”, select the net you want to add via, usually, GND net, the GND is high light. Then right click mouse again, select “ add via”, then you can add vias which are connected to GND net. Freely and put them wherever you want.   Remember, if the GND net is hide and set to a special color, no connection for these vias, but they are same color as other pads and trace in GND net.
5.    覆铜。覆铜应该是PADS的一大优点。快了很多。对于焊盘可以选择铜是盖过去(flood over)还是用对角（orthogonal, diagonal）连接。对某一个形状的焊盘只能一种设置。如果有几个圆形焊盘希望铜铺过去，而几个相同的圆形焊盘想用梅花连接。
一。PowerPCB如何import Orcad的netlist?
Orcad中的tools->create netlist，other的formatters选取padpcb.dll，再将其后缀名.net改为.asc即可。
三．请问 PowerPCB3.6的library如何加载到4.0中?
通过PowerPCB V4.0中的库转换文件Libconv4.exe将pt3库转换为pt4库！
四．在PowerPCB中如何删层?
4.0以下的版本不可直接删层，可将不需要的层上的资料删掉，出gerber时不用出就好了；4.0以上版本的可直接修改层数。
五．PowerPCB中如何开方槽?
4.0以上版本的可在编辑pad中选择slot parameters中slotte来进行设置，但只能是椭圆形的孔；也可在机械层直接标示。
六．在PowerPCB中如何将其它文件中相同部分复制到新的文件中?
可用以下部骤:
第一，在副图选择要粘贴的目标，按右键选择make reuse ，弹出一个菜单随变给个名字，ok键即可。生成一个备用文件。
第二，在按右键选择reset origin (产生选择目标的坐标)将鼠标移到该坐标上可以坐标值（在窗口的右下角处）。
第三，调出主图，将板子的格点改为“1”mil。按make like reuse 键，打开第一步生成的文件后，用“S”命令敲入第二步生成的坐标。按左键确定。在贴完后，在按鼠标右键点击break origin。弹出一个窗口按“OK”即可。
七．如何在PowerPCB中加入汉字或公司logo?
将公司logo或汉字用bmp to pcb将。bmp档转换为protel的。pcb格式，再在protel中import，export *.dxf文檔，在PowerPCB中import即可。
八．如何在PowerPCB中设置盲孔?
先在padstack中设置了一个盲孔via，然后在setup -- design rules -- default -- routing的via设置中加入你所设置的盲孔即可。
九．hatch和flood有何区别，hatch何用？如何应用?
hatch是刷新铜箔，flood是重铺铜箔。一般地第一次铺铜或file修改后要flood，而后用hatch。
十．铺铜（灌水）时如何自动删除碎铜?
1)setup-preferences-Thermals中，选中Remove Isolated copper；
或 2) 菜单Edit—Find---菜单下Find By--Isolated pour—OK
十一．如何修改PowerPCB铺铜（灌水）的铜箔与其它组件及走线的间距？
如果是全局型的，可以直接在setup － design rules里面设置即可，如果是某些网络的，那么选中需要修改的网络然后选右键菜单里面的show rules进入然后修改即可，但修改以后需要重新flood，而且最好做一次drc检查。
十二．PowerPCB中铺铜时怎样加一些via孔？
(1)可将过孔作为一part，再在ECO下添加part；
   (2)直接从地走线，右键end(end with via)。
十三．自动泪滴怎么产生？
需对以下两进行设置:
1)     setup->preferences->routing->generate teardrops->ok  
2)     preferences->Teardrops->Display Teardrop->ok
十四．手工布线时怎么加测试点?
1) 连线时，点鼠标右键在end via mode 中选择end test point
2) 选中一个网络，然后在该网络上选一个合适的过孔修改其属性为测试点，或者添加一焊盘作为测试点。
十五．请问PowerPCB怎么自动加ICT？（ICT是 In Circuit Tester 的缩写，中文名称为! b9 S3 e! {, s7 k在线测试仪，是一种电路板自动检测仪器，又称为静态测试仪（因它只输入很小的电压或电流来测试，不会损坏电路板）。）
一般地，密度比较高的板都不加ICT。而如果要加ICT，可在原理图里面设置test piont，调入网表;也可以手工加。
十六．为什么走线不是规则的?
设置setup/preferences/design/，选diagonl;将routing里面的pad entry项去掉。
十七．当完成PCB的LAYOUT，如何检查PCB和原理图的一致?
在tools->compare netlist，在original design to compare与new design with change中分别选取所要比较的文件， 将output option下的Generate Differences Report选中，其它选项以自己实际情况来选取，最后run即可。
十八．在PowerPCB中gerber out时多出一个贯孔，而job file中却没有，这是怎么回事?
这应为PowerPCB的数据库太乱了，可能是修改的次数太多造成的。解决方法可export *.asc文件，再重新import一次。
十九．如何直接在PowerPCB 3.6下生成组件清单？
通过File－Report－Parts List1/2。
二十．如何将一个器件的某个引脚的由一条网络改为另一条网络？
打开eco，用delete connettion删除原来的连接，不要删除网络噢。再用add a connetion添加一个连接。
二十一．如何对已layout好的板子进行修改?
为确保原理图与PCB一致，先在原理图中进行修改，然后导出netlist，再在PCB中导入，但要注意，如果要删除某些网络或零件，则需手动删除。
二十二．CAM　Gerber文件时（SOLDER　MASK　BOTTOM）出现“maximum   number of apertures exceeded”的提示，无法输出文件?
选中你想要输出gerber的层，进入edit document对话杠，再选 device setup (前提要在photo状态下)，在photo plotter setup对话框的下方有一个aperture count项，在其后输入数字，然后regenerate即可。
二十三．请问PowerPCB gerber out时*.rep，*.pho，*.drl，*.lst各表示什么意思，在制板时哪些文件是制板商所需要的?
*.pho　GERBER数据文件
*.rep   D码文件（线，焊盘的尺寸，必不可少的）
*.drl　钻孔文件
*.lst   各种钻孔的坐标
以上文件都是制板商所需要的。
二十四．PowerPCB如何能象PROTEL99那样一次性更改所有相同的或所有的REF或TXT文字的大小？还有，怎么更改一个VIA的大小而不影响其它VIA的大小？
可以通过鼠标右键选择“Document”，然后就可以选中所需要的ref或文字了。如果要更改一个Via的大小，需要新建一种类型的Via。
二十五．请问PowerPCB 如何打印出来?
可以在菜单File－Cam……中进行，建立一个新的CAM Document后，然后Edit，Output Device选择Print，运行RUN即可。一般先进行打印预览（Preview Selections），看是否超出一页的范围，然后决定是否缩小或放大。
二十六．请问PowerPCB如何设置才能在走线打孔的时候信号线自动用小孔，电源线用大孔？
先在PAD STACKS中将你要用的VIA式样定制好，然后到Desing Ruels中先定义Default Routing Rules使用小的VIA，再到Net Ruels选中电源的Net，在Routing中定义成大的VIA。如不行，可以敲入“VA”，将VIA Mode设成Automatic，它就会按规则来了。
二十七．要想在PowerPCB中放置单个焊盘，是否就要在组件库中做一个单焊盘的组件？
不一定，如果单个焊盘有网络连接，则可以改成放过孔，毕竟放组件不利于DRC。放过孔的方法：选中某一网络（NET），单击右键，选ADD VIA即可，可以连续放多个。最好打开在线DRC。
二十八．对与常用的电阻电容和常用的集成器件也需要做PART吗？是否PowerPCB自带的有？直接调用吗？
当然，比较通用的一些器件PowerPCB均有自带。你可以自己进去浏览一下就知道了！如果库中已有的器件，就不用再建了，但要确认一下封装是否合适！
二十九．   PowerPCB在铺铜时画铺铜区时，如要在TOP 和BOTTOM均要铺GND的铜，是否需要在TOP和BOTTOM分层画铺铜区后，再分层进行灌铜？
在灌铜时，各层均需要分别画铜皮框，如果一样的外形，就可以Copy。画完后现在Tools下的 Pour Manager中的 Flood all 即可。
三十对于一过孔（为GND网）或一器件的插脚（为GND网）。现在要同时对顶层和底层的GND铺铜。为什么只允许插脚的单面GND连通所铺的铜？PROTEL中两面均可连接，PowerPCB中怎么解决？
你可以到Setup－preference－Thermals选项中，将“Routed Pad Thermals”选项打勾试试！
三十一．   PowerPCB 3.5中的菜单Setup ---> Design ---> Rules---> Default---> Routing 中Vias的Availabe和Selected匀为空白，在此情况下是没有过孔，各层无法连接。请问怎样设置过孔？
那你就新建一个VIA类型SETUP->PAD STACKS->在PAD STACK TYPE中选VIA->ADD VIA……然后Setup --->Design --->Rules---> Default---> Routing 中，把新建的VIA添加到SELECT VIA!
三十二如何能在PowerPCB中象在PROTER中一样，将一组器件相对位置不变的一起旋转？
操作之前，敲入"DRI(忽略DRC)"或“DRO(关闭DRC)”，然后必须先将需要选转的器件和线等选中，然后定义为一个Group，然后就可以点击右键进行Group的旋转操作了。（鼠标点击处为旋转的基准点！）
三十三．在PowerPCB 4.0 里有几个图标，其提示分别为plane area， plane area cut off ，auto plane separate ，他们有什么功能啊？另外我见到有的PCB里，用plane area cut off 画个圆，PCB生产时就是一个孔?
这是在内层作分割时用的几个命令，第一个为在内层指定分割的区域，第二个为在区域里挖除块，第三个是有几个区域你先全定义为一个然后再从这一个上去分。
三十四在PowerPCB中是否有对各层分别进行线宽的设置吗？
可以的！
design->default设置缺省值。
design->conditional rule setup生成新的条件规则：按照你的说明，应该选source rule object：all
against rule object：layer/bottom
点击create，生成新的规则。按要求修改，OK！
三十五．在PowerPCB布线时，违背了规则中定义的走线方向的走线往往会出现一个菱形的小框以作提醒，但我发现在一块板子的设计过程中难免会出现这样的情况，请问此情况对设计以及以后的制板有什么影响吗？
出现的小菱形说明，布线没有定位在网格点上。这是很正常的情况，对设计以及以后的制板没有任何影响。此功能可以在 setup - preferences - routing 中取消 show tacks 选项，小菱形就不会出现了！
三十六．   4层板，如果有几个电源和地，是否中间层要定义成split/mixed？
我们一般都不使用CAM Plane设置，均设置为信号层，这样在以后的电源分割时可以随意分割，较方便！
三十七根据大家地经验，如果是6层板，走线和电源地层怎么分配？是不是线、地、线、线、电、线？
6层板如果一定要走4层信号线的话，肯定有两个信号层相邻。可以采用你上面的迭层方式，也可以采用：Signal1、GND、Signal2、VCC、Signal3、Signal4，这样，Signal2可以走一些要求较高的信号线，如高速数据和时钟等。
三十八．   PowerPCB的25层有何用处？
POWERPCB的25层存储为电源、地的信息。如果做多层板，设置为CAM PLANE就需要25层的内容。设置焊盘时25层要比其它层大20MIL，如果为定位孔，要再大些。
    三十九.如何在PCB中显示元件的值？
在建库时，在工作介面中，原始值只有TYPE与NAME，怎样加入VALUE呢？
步骤：一。单击菜单栏EDIT/ATTRIBUTE Manager……./ADD/BROWSE Library Attributes/Value/ok/CLOSE.
      二．单击快捷钮（Drafting Toolbar）/add new lable/attribute中选VALUE/LAYER中选silkscreentop
      三．CTRL+ALT+C调出DISPLAY COLOR对话框，在SILKSCREEN TOP中Ateributes中选择相应的颜色就行了。
四十，为什么在走线时从焊盘出来总是（any angle任意方向）模式，用（AO/AD/AA等快捷键无作用）
此故障为软件设置有问题，解决方法有：单击菜单TOOL（或CTRL+ENTER）/OPTIONS/ROUTING/PAD Entry/Guide PAD entry此项前的勾去掉，然后单击APPLY，就行了。
四十一，如何将元件加入“属性”以便于在装件时整体查询？
步骤：1，右击需要加入的元件，在弹出的的下拉菜中选择ATTRIBUTE。
2，在弹出的对话框中选择“ADD”然后在Attribute下方的向下按钮中选择VALUE，在VALUE右边的框中输入值（如NE555）然后关闭对话框。
3 ^9 t; c: F* x  E! m3 X3，单击菜单栏“Edit”/fine弹出对话框，在FINE BY 中选择“ATTRIBUTE”
在ATTRIBUTE中选择“VALUE”在ATTRIBUTE VALUE中会找到加入的值，单击所需要的，按APPLY，就会在PCB中显示所有的相同值（高亮）
  四十二，PADS中走弧线的方法？
方法一：首先按CTRL+ENTER 进入设置菜单， 点design， 在MITERS那里改成ARC，其他2个参数和DOS版的一样 弧度大小设置， 如果选了AUTO MITERS好像也只会在在画铜箔时自动弧线，这里设置好了后，然后回到PCB中选择所有NET， 点右键 选 ADD MITERS 就会把所有转角改成弧线， 还有一种方法就是在走线过程中点右键 点ADD ARC 。
方法二：选择对象为tace/PIN ；选择一个拐角（两段线都要被选择）； 右键convert to arc; 拉出所需弧度即可。如果是直线，进入router,右键strech arc即可！
四十三，PADS2005中如何用三方软件输出EXECL BOM 清单？
将软件下载并保存在一个目录下，然后打开PADS，在TOOLS菜单/ BASIC SCRIPTS / BASIC SCRIPTS中载入刚才保存的文档，然后单击“RUN”就行了。
 麦斯艾姆（massembly）贴片知识课堂BOM清单标准化格式，用通俗的文字介绍专业贴片知识。    麦斯艾姆科技，全国首家PCB样板打板，元器件代采购，及PCB样板贴片的一站式服务提供者!
Massembly, 麦斯艾姆科技有限公司http://www.massembly.com/ 电话：0755 – 26552719 
