# POWERPCB常用技巧 - xqhrs232的专栏 - CSDN博客
2013年07月15日 14:55:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1330
原文地址::[http://freshpassport.blog.51cto.com/2359526/620564](http://freshpassport.blog.51cto.com/2359526/620564)
转载
POWERPCB常用技巧：（一）
 1。移动元件时可按TAB键翻转。
 2。加任何形状的孔（包括方形元件脚）：可在24层用二维线画。
 3。加裸铜（即阻焊，在加大导通电流和增加焊接附着力时经常用到）用铺铜功能在28层加。
 4。把当前PCB板用到的元件封装存入自已的库：圈住所有元件按右键选SAVE TO LIB...,再选所需的库即可.(把别人的库占为已有算是不费吹灰之力)
 5.在POWERPCB把二维线图形存入库:先用二维线功能画好所需的图形(可加上文字)把它们全部圈住按右键选SAVE TO LIB...,再选所需的库即可.(方便日后调用)
 在POWERPCB中，将两块板合并成一块板的方法：
 1.打开要复制的线路板文件，打开ECO模式
 2.全选（可用单击鼠标右键选择SELECT ALL)
 3.复制
 4.打开复制到的线路板文件，打开ECO模式
 5.粘帖
 6.移动到合适位置
 7.    OK！
 powerlogic中好像没有copy功能吧！比如说在protel中我想在一个project中将一张图上的内容粘到另一张图上式可以做到的，在powerlogic中好像不能这么做，只能在同一页图中进行粘贴，这样好像不利于资源的重复利用，不知道大家有没有什么方法，我好像没找到！
 可以是可以，但是比较麻烦！
 先将选中的原理图部分做“Make Group”，然后再将其“Copy to File”。然后在需要粘贴的新页中，选择“Add Item－Paste From File”即可。以上操作均使用右键菜单。
 请教:POWERPCB如何能象PROTEL99那样一次性更改所有相同的或所有的REF或TXT文字的大小,还有,怎么更改一个VIA的大小而不影响其他VIA的大小.这功能POWERPCB真不如PROTEL99SE.
 可以通过鼠标右键选择“document”，然后就可以选中所需要的ref或文字了。如果要更改一个Via的大小，需要新建一种类型的Via。
 先在PAD STACKS中将你要用的VIA式样定制好，然后到Desing Ruels中先定义Default Routing Rules使用小的VIA，再到Net Ruels选中电源的Net，在Routing中定义成大的VIA。
 敲入“VA”，将VIA Mode设成Automatic,它就会按规则来了。
 不一定，如果单个焊盘有网络连接，则可以改成放过孔，毕竟放元件不利于DRC，
 放过孔的方法:选中某一网络（NET），单击右键，选ADD VIA即可，可以连续放多个。最好打开在线DRC
 相同的铜皮框在TOP和BOTTOM 是重叠的，在进行单层FLOOD时怎样选中TOP或BOTTOM的铜皮框？我现在总是只能选中TOP 层的铜皮框，因为是重叠的即使换层也只能选中TOP层的铜皮框，请问解决方法？（除FLOOD ALL）
 右键Select Shapes选取任一层铜皮的外框，然后点点Tab键看看，或者右键的cycle!
 CAM PLANE和Split/Mixe都可用来设为地或电源层，CAMPLANE是负片，里面不能走线，但Split/Mixe是正片，里面可以有走线，可以灌铜。
 按shift键同时，用十字架选中某一段route，点鼠标右键就可看到unroute了(删除走线）
 要在color里把copper项以及相应的层设置上颜色并且如果是新打开的已经覆铜的板子要Hatch一下才能看到要copper pour的区域吧。
 我想你可能是想问flood和hatch的区别。
 Hatch和Flood的区别解释如下：
 我们知道PowerPCB里的覆铜区域的边界是由弧线和折线等构成的，这个边界描述了需要覆铜区域的范围。Flood是根据现有的设计规则进行覆铜，软件会根据设计规则重新计算并确定覆铜区域的边界。Hatch呢，就是简单地在已有覆铜区域边界的覆铜区域内覆铜，或简单理解为”覆铜区域内的覆铜可视化“，经常新打开一个曾经覆铜的板子需要做这个操作。
 形象一点，Flood字面是洪水的意思，水流冲过，一切都要更新了，所有的边界也要重新界定了，当然，自然的洪水要遵循自然规律，覆铜的flood则会遵循设计规则。Hatch，字面意思是孵化的意思，孵化是在一个壳里孵化长大，这个“壳”就是覆铜区域的边界了，铜在里面...孵化长大直到填满。
 Flood之后需要删除孤立铜皮，这个删除操作实际上就是删除了一些不需要的覆铜区域。
 有不当或错误之处欢迎指正。
 以下是引用hellen80在2003-5-20 10:00:00的发言：
 请问，在POWERPCB中，如何在BOARD OUTLINE 中加入导角，BOARD OUTLINE的尺寸已定，在四个角加上45度的导角，如何加，好像用ADD CORNER,尺寸不能设定？
 退出现有命令，点鼠标右键，选择“Select Board Outline"命令
 选中靠近Board Outline拐角的地方，点鼠标右键，Split，
 然后随便移动一下鼠标，Left click.
 再选中拐角，编辑一下就可以了。
 如果是要加圆弧拐角，再Pull Arc。
 不知道说明白没有，欢迎讨论。
 有无办法让pcb板翻个一下。镜像一下?
 有辦法的,將pcb版框點選後按右鍵選imirro,就ok了
 4层板删除layer2、layer3层，变成2层板的做法如下：
 第一步：删除layer2层的电特性数据，包括走在该层的traces、copper、via。
 第二步：删除layer2层的电特性数据，包括走在该层的traces、copper、via。
 第三步：进入菜单setup/Layer Definition面板。在Electrical layers栏中点击modify按钮。在弹出的对话框中输入2。如果layer2、layer3已经没有电特性数据，那么4层就变成2层板了。如果layer2、layer3已经还有电特性数据，会出现一个警告说layer2、layer3已经还有电特性数据。按第一、第二步骤删除它，再进入第三步。
 第四步：将原先布在layer2、layer3的相关部分布到top、bottom层即可。
  还有一个在使用POWERPCB LAYOUT时要注意的！！
 有时候，我们会做一个比较大的设计，这是文件会很大，而且，操作时间也很长，这样如果有一点误操作，很可能就把powerpcb的数据库破坏，经常会强制关闭设计或造成无法同步，甚至不能输出*.asc文件。因此，在你的一个设计时间比较长时，最好，先导出一个*.asc文件，再把这个文件导入生成新的*.pcb这样数据库能整理一次，能避免上面提到的错误发生。改板时，也推荐大家，先这样做一遍，能省掉你以后不少麻烦！！！！！！！！！！
 删除铜皮还有一个方法，
 1。选择FILTER用SHAPE
 2。框住要删除的铜皮
 3。按DEL键，零碎的铜皮就变成了一个框（加铜皮的时候定义的）
 4。再按DEL键，框也没有了。
 完成删除。特别适合整块删除的操作
 画铜皮框的时候和放置器件的时候可以把栅格设大一点
 比较容易定位！！！！！！！
 power pcb3.5版本的 library怎么转到power pcb4.0中?
 在POWERPCB的目录下找到 Libconv4.exe文件。运行它，按要求输入power pcb3.5的库文件就可以转为power pcb4.0库了。
 关于在powerpcb中会速绕线的方法：
 第一步：在setup/preferences面板的design下的miters中设置为arc，且ratio为3.5。
 第二步：布直角的线。
 第三步：选中该线，右击鼠标，选中add miters命令即可很快画出绕线。
 你可先走直角线，把线绕等长，但一般要饶长一点，因为在直角生成弧度角后，线长将变短！
 第一步，走直角线
 第二步，按着shift点线，选中后，右键点出菜单，选择“add miters”,这样就可得到点特性比较好的弧形走线了，而且也比较快！
 在我用的powerpcb5.0应用中为何找不到add miters栏???
 你先选中net，然后点右键看还能不能找到add miters栏。^_^
 那是你选没有选中拐角而已。
 介绍一个有效的hatch方法：
 Select net(任意网络) -> view net -> apply(或OK)->完成，
 大家试一试，在很多情况下，这种方法绝对比工具栏上的Hatch工具高效的多！！！
 还有个经验与大家分享，就是但某一块板可以在某一块已有的板上作修改而得，有新原理图导入时，最好关闭DRP状态，否则可能会破坏原又走线！
"POWERPCB4.0中，如何把内层的接地花孔改为实孔？内层是CAM PLANE。
 还有混合层不让我铺铜，是那里要设置？
"
 我们布板的时候所有的层都是定义为no plane属性。所以不会存在以上的问题。如果想接地花孔改为实孔，只要把铺铜的属性改为flood over vias就可以了。
 不知你们要使用cam plane和split/mixs的层属性，对PCB设计会带来什么好处？？
 设置cam plane是对于电源/地层分布比较均匀时使用，比较方便，分割时用2Dline就可以了；
 如果同一层电源/地较多，而且较混乱，我通常还是采用split/mixs，用智能分割覆铜，不会出错！
 如果打一个一个的打地过孔，可以这样做：
 1、设置GND网络地走线宽度，比如20mil。
 2、设置走线地结束方式为END VIA。
 3、走线时，按ctrl+鼠标左键就可以快速地打地过孔了。
 如果是打很多很整齐地过孔，可以使用自动布线器blazerouter，自动地打。当然必须设置好规则：
 1、把某一层设置为CAM层，并指定GND网络属性给它。
 2、设置GND网络地走线宽度，比如20mil。
 3、设置好过孔与焊盘地距离，比如13mil。
 4、设置好设计栅格和fanout栅格都为1mil。
 5、然后就可以使用fanout功能进行自动打孔了。
 各位是怎么打地孔地？
 我得方法是：选择一个网络，单击右键，选择ADDVIA。加过孔前先将网格设置地打一点，比如20mil，40mil。
 生产椭圆形的焊盘，只要设置好参数即可，如下图所示：
 powerpcb中好像没有打方孔器件的功能。圆孔的有。
 如果要打的话，可以用2d－line的方式画出方形，然后叫制板产家挖掉就可以了。
 走线层和平面层的区分除了正，负片之分外，是不是如果电源或地没有线就用平面层？有线必须用走线层。
 CAM PLANE（电源/地层）层没有走线，是负片；而走线层和SPLIT/MIXE PLANE(混合层)是正片，混合层也同时可以用来走线的。
 我也想请教bgsky这个问题，另外想加个问题：表面铺铜皮地怎样最快？
 ddwe：
 首先，覆铜层改为split/mixs；点击智能分割图标，画好覆铜外框，然后点击右健，选择anything的选择模式，光标移到覆铜外框，进行分割；最后进行灌铜！
 michaelpcb：
 选择覆铜模式，画好外框，右健选择shape，选中覆铜外框，属性改为gnd，flood即可；最后还要删除孤岛。
 智能分割覆铜应该选“PLANE AREA”
 注意：在画外框之前，必须把该层改为split/mixe，否则不能选中！接下来的操作和前面介绍一样。
 最后多说一句，显示覆铜外框必须在preferences->split/mixed plane->mixed plane display中设置
 在覆铜时，copper、copper pour、plane aera、auto separate有什么不同？
 分割用2D line吗？有些基本概念我不太清楚，恳请各位高手解答！
 灌铜是选flood，还是tools->pout manager?
 .在覆铜时，copper、copper pour、plane aera、auto separate有什么不同？
 copper：铜皮
 copper pour：快速覆铜
 plane aera：智能覆铜/电源、地覆铜（使用时必须在layer setup中定义层为split/mixe，方可使用）
 auto separate：智能分割（画好智能覆铜框后，如果有多个网络，用此项功能模块进行分割）
 2.分割用2D line吗？
 在负向中可以使用，不过不够安全。
 3.灌铜是选flood，还是tools->pout manager?
 flood：是选中覆铜框，覆铜。
 pout manager：是对所有的覆铜进行操作。
 非常感谢，我在使用时发现一个问题：
 无论是在route层用copper pour覆铜，还是在Split/Mixed层用plane aera覆铜，当我想在一片大的覆铜区内挖出一片小覆铜区时，如果大覆铜区把小覆铜区完全包围起来，那么小覆铜区就不能灌铜，必须先删掉大覆铜区，给小覆铜区灌铜，再做大覆铜区。而且保存文件再打开时，小覆铜区又没铜了，还要删除大覆铜区，给小覆铜区灌铜，很麻烦，有没有好的解决办法？
 你可以先画好小覆铜区，并覆铜；然后才能画大覆铜区覆铜！
 顺序不能倒置！
 补充一点：
 以后打开文件，只要ｈａｔｃｈ即可。
 如果要修改，只有删除大覆铜框，重画！
 可以看一下本站的powerpcb5.0解密全过程。
![POWERPCB常用技巧 - James - ~~！城市幽灵-------------](http://www.willar.com/skin/skin_1/small/url.gif)[http://www.2008cool.com/bbs/list.asp?boardid=14](http://www.2008cool.com/bbs/list.asp?boardid=14)
 阻焊层是指被绿油覆盖，不直接以铜皮的形式出现在板上的那层。
 热焊盘在电源或地层中也称花孔，在表层铺设大片的铜皮并希望这些铜皮毫无连接关系的独立放在那里，这时一般都会将它们与地或电源网络连接起来，铜皮与这些网络中链接的焊盘或过孔称其为热焊盘。
 请问在powerpcb4.0中，为什么Ctil+Alt+c有时有用，有时不起作用？
 先设置好层，然后在打开WINDOW\STATUS...，将pair中的2该为4，OK！
 powerpcb制作gerber文件的步骤到这里看看：
 假如是双面板，则需要以下GERBER文件：
 顶层线路（ROUTING LAYER TOP）
 底层线路（ROUTING LAYER BOTTOM）
 顶层丝印（SILKSCREEN TOP）
 底层丝印（SILKSCREEN BOTTOM）
 顶层阻焊（SOLDERMASK TOP）
 底层阻焊（SOLDERMASK BOTTOM）
 打孔数据（NC DRILL）
 简单地说：菜单File -> CAM... -> Add... -> document的下拉菜单中选择各种层的输出名，如走线层、白油层、绿油层等。各层中所需要的元素可根据您的需要进行选择。
 为保证输出的gerber文件准确无误，当然需要先预览各层gerber文件，检查各层输出是否您所期望的。也可以使用CAM350对生成后的Gerber进行查看。 
 这里只介绍了一个过程，详细步骤可以看看它的说明书。 
 我在生成gerber文件后，运行run,出现：No symbol for size:0.052-used symbol:Z 
 No symbol for size:0.1-used symbol:Y 
 No symbol for size:0.037-used symbol:X 
 No symbol for size:0.11-used symbol:W 
 No symbol for size:0.028-used symbol:V 
 No symbol for size:0.02-used symbol:U 
 请问这是什么原因？ 
 这个是打孔图，出现提示是告诉你一共有多少个类型的drill，你点击"OK"就行了，不必理会它 
  POWERPCB快捷键分享 
 POWERPCB快捷键分享 
 命令字符命令                       含义及用途 
 C          补充格式， 在内层负片设计时用来显示Plane 层的焊盘及Thermal。使用方法是， 从键盘上输入C 显示， 再次输入C 可去除显示。 
 D         打开/关闭当前层显示， 使用方法是， 从键盘上输入D 来切换。建议设计时用D 将Display Current Layer Last＝ＯＮ 的状态下。. DO 贯通孔外形显示切换。ＯＮ时孔径高亮显示， 焊盘则以底色调显示。使用方法是， 从键盘上输入DＯ 来切换。 
 E         布线终止方式切换， 可在下列３ 种方式间切换。End No Via 布线时Ｃtrl+点击时配线以无VIA 方式终止End Via 布线时Ｃtrl+点击时配线以VIA 方式终止End Test Point 布线时Ｃtrl+点击时配线以测试PIN 的VIA 方式终止使用方法是， 从键盘上输入E 来切换。 
 I        数据库完整性测试， 设计过程中发现系统异常时， 可试着敲此键。 
 L <n>     改变当前层到新的n 层（F4也可换层） 
 <n>       可为数字或是名字， 如(L 2) or (L top)。 
 N <s>     用来让NET 高亮显示，<s>为要显示的信号名。可以堆栈方式逐个显示信号， 如N GND 会高亮显示整个GND。N-会逐个去除信号N 将会去除所有的高亮信号 
 O <r>     选择用外形线来显示焊盘与配线。 
 PO       自动敷铜外形线on/off 切换。 
 Q        快速测量命令。可以快速.测量dx，dy 和d 。注意精确测量时将状态框中的Snaps to the design grid 取消。 
 QL       快速测量配线长度。可对线段、网络、配线对进行测量。测量方式如下： 首先选择线段、网络或者配线对， 然后输入QL 就会得到相关长度报告。 
 R <n>     改变显示线宽到 <n>, 如, R 50。RV 在输出再使用文件 Reuse 时， 用于切换参数设定。有关详细信息请参见 "To Make a Like Reuse in Object Mode" SPD 显示 split/mixed planes 层数据， 该命令控制 split/mixed planes 参数对话框中的一个参数。SPI 显示 plane 层的thermal 。该命令控制 split/mixed planes 参数对话框中的一个参数。SPO 显示 split/mixed planes 层的外形线。该命令控制 split/mixed planes 参数对话框中的一个参数。T 透明显示切换。在复杂板子设计时很有用。Text 文字外形线显示切换。W <n> 改变线宽到<n>， 如W 5 。 
  数字键盘命令： 
 7   全部显示  8  向上移动一个栅格  9  缩小  4  向左移动一个栅格 6  向右移动一个栅格  1  刷新  2向下移动一个栅格  3  放大  0中心显示  。 删除被选目标 
 双击鼠标左键进入走线模式（也可以F2） 
 .DOS版Protel软件设计的PCB文件为何在我的电脑里调出来不是全图？ 
     有许多老电子工程师在刚开始用电脑绘制PCB线路图时都遇到过这样的问题，难道是我的电脑内存不够吗？ 我的电脑可有64M内存呀！可屏幕上的图形为何还是缺胳膊少腿的呢？不错，就是内存配置有问题，您只需在您的CONFIG.SYS文件（此文件在C:\根目录下，若没有，则创建一个）中加上如下几行，存盘退出后 重新启动电脑即可。 
 DEVICE=C:\WINDOWS\SETVER.EXE 
 DEVICE=C:\WINDOWS\HIMEM.SYS 
 DEVICE=C:\WINDOWS\EMM386.EXE 16000 
 PROTEL布局方法 
 同时打开SCH和PCB，PCB中要无元件被选中。 
 在原理图中选择相关电路元件，再Tools-Select PCB components。 
 自动转到了PCB中并显示所有被选中元件，用Tools-交互布局的矩形内部排列功能单独放一起，再X-A一次。 
 这一堆元件同理可更细分，如把电源滤波电容再单独分出来等。 
 这一堆元件布局完成后可联合起来。 
 一、打开PCB文件，将文件的元件生成一个新元件库
 &Design/Make Library 生成元件库并保存好(现改名为NEW.LIB)
 将新建的元件库导出到一个指定目录
 二、打开自己的元件库文件。在documents文件管理器中导入刚建立的NEW.LIB元件库。
 三、同时打开两个元件库（以KGX.LIB和NEW.LIB为例）。
 四、打开新建元件库NEW.LIB文件在PROTEL99SE编辑器左边的Browse PCBLib管理器下选择要保存到KGX.LIB元件库的元件，如果全选可以单击最上一个，再按住键盘的SHIFT键，再单击最下面一个元件名进行全选。
 五、单击鼠标右键，选择COPY（复制）
 六、选择自己的元件库KGX.LIB，在左边的Browse PCBLib管理器下的窗口内空白处单击鼠标右键，选择Paste粘贴。软件就会将元件粘贴过来。
 七、单击保存按钮进行保存。
 八、将KGX.LIB导出到指定目录。
     到此就完成了元件的入库工作。
Layer25层的作用：
Layer25层是插装的器件才有的，只是在出负片的时候才有用，一般只有当电源层定义为CAM Plane的时候geber文件才会出负片（split/Mixe也是出的正片），如果不加这一层，在出负片的时候这一层的管脚容易短路。
PowerPCB中对电源层和地层的设置有两种选择，CAM Plane和Split/Mixed。Split/Mixed主要用于多个电源或地共用一个层的情况，但只有一个电源和地时也可以用。它的主要优点是输出时的图和光绘的一致，便于检查。而CAM Plane用于单个的电源或地，这种方式是负片输出，要注意输出时需加上第25层。
第25层包含了地电信息，主要指电层的焊盘要比正常的焊盘大20mil左右的安全距离，保证金属化过孔之后，不会有信号与地电相连。这就需要每个焊都包含有第25层的信息。而我们自己建库时往往会忽略这个问题。
Layer25层的替代设置：
在PADS的焊盘设置中，有一个AntiPad的设置，只要能使这一项（选择焊盘类型即可），其焊盘的初始设置值即为普通焊盘+24mil或0.6mm，看这一设置的功能及效果看，可以替代Layer25的作用，而且这样的设置感觉上做法也较为正规一些。只是相对来说Layer25的作法历史悠久，很多人已经习惯了，新手们可以试试。
还有一点就是使用Layer25层可以在建元件的时候就设置好这一项，而AntiPad则需要在布板中设置，对于过孔的处理就差不多，可以给过孔加layer25也可以设置过孔的AntiPad。
具体Antipad的设置，博客中已经有一篇文章里已附了一个PDF文档，大家可以看看。
总的来说，不管是用Layer25还是Antipad，其最终的目标有两个：一是上面提到的金属化过孔时防止短路；二是减小过孔的感生电容电感。
过路高手如有不同意见，还望赐教...
Layout时的一个鼠标小技巧 
发表于 2007/12/28 23:48:42 
        在layout时,有些人久了可能会习惯使用小键盘, 但我觉得用小键盘,在其他快捷操作时,手来回换总是不方便...还要时不时看键盘...不舒服...所以看过不少"高手"们用小键盘,自已也用了一小段时间,还是觉得不方便, 我觉得小键盘的好处中,有一个就是可以有上下左右键来移动视图, 今天用时突然想起这个问题,就找了一下, 这点可以通过鼠标实现,前提是你的鼠标是带轮的(我想现在基本都是了), 上下移动就是上下动轮子这就不用说了, 但在按住shift键的情况下,上下滚轮就转为左右移动视图的功能了,免了再去动滑动条或使用insert那样,视图会刷一下,影响注意点....嘿嘿!!怕以后可能会忘记,所以就在这里做个笔记,也跟大家共享一下
PADS非常用操作备忘 
发表于 2007/12/9 23:24:11 
1.输入网表：
       在PADS layout中，输入网表有两种方法，一种是使用logic中的同步器；另一情况是当你用其它软件(如ORCAD)绘制原理图，而需要用layout来布PCB时，可以通过：
           File/Import将网表输入
元件类型库(Part Type)
封装库(PCB Decal)
逻辑封装库(logic Decal)
线型库主要用于绘制原理图的背景版图
3.电路模块的拷贝
将原理图拷贝到*.grp文件，可以由此建立一个常用的电路模块库可以通过Paste from file来由库文件中调用电路模块。
4. Copy as bitmap 将原理图中的电路模块或接口转成BMP文件，复制到剪切板中。可以用于作设计说明文档。具体步骤为：先选edit/copy as bitmap,然后在原理图中选择你要复制的范围电路，可以将该部分电路位置作个调整，以便更好的选择。如果有文档背景要求，可以先作个单色过彩色的颜色方案（如把背景色调成白色，以适应文档背景色）
4. PADS layout中,Preferences/Design选项卡中,Stretch Traces During Component Move选项的作用：
   选择该选项后，在交换元件管脚或门时，走线将重新布置，即依然保持走线连接关系；不选择该选项，在移动元件时，系统将以鼠线连接走线、管脚和门，而原先已走的线将保持不动。
5. PADS layout中，Preferences/Drafting...，"Min. Hatch Area"(最小铜皮区)设置最小铺铜区的面积，单位为
当前设计单位的平方。
"Smoothing Radius"(圆滑半径)：设置铺铜拐角处的圆角半径，一个较大的圆滑半径会得到一个更圆滑的圆角。
6. 两个自动布线时有用的设置：
    Design Rules/Default/Routing中，Routing options区域 "Allow Shove" (允许移动已经布线网络)，"Allow Shove Protected"(允许移动受保护的走线)
7. 焊盘出线及其与过孔关系设置
    Design Rules/Default/Pads Entry：在这里可以设置焊盘的出线角度，如可以设置禁止以不规则角度与焊盘相连；设置是否允许在焊盘上打过孔。
8. 中间挖空的铜皮的建立：分别利用Copper 及Copper Cutout 建立两个符合要求的区域，选择这两个区域，通过右键菜单的combine，操作完成（早上起来用到这个，居然忘了，找了好久，好记性不如烂笔头啊）
PowerPCB的实用资料1 
发表于 2007/11/25 20:03:37 
PowerPCB设计问题集.pdf
powerpcb心得.pdf
POWERPCB中thermal及antipad的设置.pdf
POWRPCB高级技巧.pdf
高速PCB的过孔设计.pdf
使用PowerPCB过程中遇到问题的总结.pdf
PCB的层类型及Gerber文件 
发表于 2007/11/25 2:14:30 
1.PADS中的层类型（plane type）
   a. No plane：通常指走线层，如Top   Bottom,以及中间走线层，以正片的形式输出
   b. CAM plane：以负片的形式输出，层分割以2D线来实现，不用铺铜，通常用于
                             电源层跟地层，且占用的数据量要小得多，但有一个缺点就是不
                             会检查设计规则，即分配到这层的网络，就不会再检查安全间距
                             及连接性等，因此，分割层需要自已保证无误，
   c. Split/Mixed plane：混合层，以正片的形式输出，需要铺铜，但其铺铜与No plane
                             不同，可以选择分割块按块铺，统一操作是在tool/pour manager的
                              plane connect页中操作,该层在进行规则校验时会检查规则。
    ***  使用Mixed plane做电源层或地层时，层分割过程可能会出层某一块铜皮被另
            一块铜皮全包围，或有重叠的情况，进行pour 操作后，经常出现被覆盖的现
            象，在这种情况下，需要设置分割块的优先级别（flood priority），级别越
           低，越优先铺铜，即重叠部分划归优先级别低的
2.常用的Gerber文件
   a. 走线层，（包括信号层及电源层和地层），可能的层类型包括上面提到的全部
                        三种类型
   b. 文字层（湿印层）用于标示零件范围、方向及序号。一般包括（Top/Bottom 
                     Silkscreen Layer）
   c. 阻焊层，在不需要焊接区域涂上一层阻焊及防氧化材料，一般为绿油，这类
                       主要是Soldermask Layer Top/Bottom
* 当需要堵住过孔时(即平时说的过孔盖油)，可以该Gerber文件设置中去掉via，预览时见不到过孔，即满足要求。
   d. 钻孔层：（drilldraw）
   e. 铜网层：主要用于组装工厂在放置SMD零件于电路板前，需透过此冶具在SMD
                       零件的PAD上涂一层锡膏，这个一般由(Paste mask Layer Top/Bottom)来
                       实现
   f. 装配图：(Assembly)
 ***另外，有些贴装厂可能会要求提供坐标文件，这种文件实现上其实就是通过
        file/export实现，在export配置窗的section中，只选择part,即不导出网络，通常还
       会要求你导成低版本兼容格式，一般在format单选框中选PowerPCB V4.0基本就
       够了
刚找到的一个，比我说的直观，有图Powerpcb输出gerber文件步骤.pdf
layout中shift+click的妙用 
发表于 2007/11/24 20:35:09 
组合快捷键shift+click在不同场合有不同用法：
1.走线过程用于添加过孔
2.选择焊盘或走线可以选定网络
3.选择shape的一部分即可选中shape，包括层分割或覆铜分隔线及其它形状2D图形，这一点比较有用。
用associate建库元件，在cam中的问题 
发表于 2007/11/23 17:40:46 
  用Associate建库的元件，如一些电源芯片（SOT-223,TO-263封装等），建库时用Associate将大端焊盘与管脚关联....或者是建不规则焊盘。
    上面方法建的库，在CAM输出时，SOLDER层会出现看不见管脚焊盘的情况，（关联中的cpper可以见到）,这种情况可以通过设置SOLDER层的GERBER设置,SOLDER GERBER由TOP/BOTTOM和SOLDER MASK TOP/BOTTOM组成,选择TOP/BOTTOM,设置pins with associated copper设置框内的设置，即可解决问题
layout快捷键2 
发表于 2007/11/23 16:07:23 
上次在无模式命令里转了一圈，其实，使用工具不过是一种习惯，花点时间养好习惯会事半功倍，至于做出来的东东如何，还得看每个人的用心度了，不过操作好会使你的灵感触发点更多一些。           废话完毕，学习了
=================================================================
Ctrl+A      全选               跟logic一样
Ctrl+B      显示全板视图，跟logic类似，这里视图边为板的边框
Ctrl+C      copy
Ctrl+D      redraw  刷新
Ctrl+E      move
Ctrl+F      用于改变元件的安装面，白一点说就是将顶层的元件放到底面，或进行相反的操作
Ctrl+G     好像没有这个快捷方式
Ctrl+H     使选定的对象（可以是一个范围）以高亮显示，取消这种状态可以单击一次对像或再选定一次即可
Ctrl+I      元件的任意角度旋转，好像是说不大建议这样做吧，说是会给生产时的机贴带来麻烦，不过有时还真能加点艺术元素
Ctrl+J     这个好像有，但我没看见，反正是操作时有反应就是
Ctrl+K    这个还没搞懂平时有什么用
Ctrl+L     对齐，可以是对元件对齐，也可以是标号对齐操作，这个可是布局必不可少的，如果你想好看
Ctrl+M   length minimization   平时没用过
Ctrl+N   new
Ctrl+O  open
Ctrl+P   不是print，很像没用
Ctrl+Q   query/modify     查询对像的属性，这个很有用啊
Ctrl+R   rotate 90            元件旋转90度
Ctrl+S   save                   保存
Ctrl+T   应该是用于teardrop操作的，但不知道怎么用啊
Ctrl+U  取消高亮，不过跟前面提的重选定方法，这个好像反而复杂点
Ctrl+V  paste
Ctrl+W zoom
Ctrl+X  cut
Ctrl+Y  stretch   用来拉线，这个是调整走线时几乎必用的
Ctrl+Z   undo
============================================================
tab   cycle   循环选择，多个重叠对像的循环选择
F2   route   走线
F3   dynamic route 动态走线，不过需要打drp(DRC prevent)前提下才能用
F5   select pin pair   选择管脚对    操作：选择管脚对的一部分（鼠线，管脚，走线），按F5，
F6   select net   选择网络 操作类F5
============================================================
Ctrl+Alt+G：preference设置窗口
Ctrl+Alt+F：Filter  过滤器窗口
Ctrl+Alt+N：网络配色显示窗口
Ctrl+Alt+S：状态窗口
Ctrl+Alt+J：增加跳线，我们对于布单面板的哥们比较有用点，走不通就跳
Ctrl+Alt+C：颜色设置窗口
PADS手记(二):Layout中的快捷键1--无模式命令 
发表于 2007/11/23 1:40:02 
本人确认还未知道全部的快捷键, 写这个主要是做个积累过程,作个笔记
=======================================================
无模式命令:
AA：任意角走线
AO：正交走线
      AD：对角线走线             以上三个也可以通过preference中design页line/trace angle项
                                                 进行设置
      C：  反显显示背景，可用于查看负片的铺铜情况
      D：开关当前层显示
      DO：用于开关选择是否显示drill孔
      DRP：DRC prevent
      DRW：DRC warn
     DRI：Ignore clearance
      DRO：DRC off                  上面四个操作的设置在preference/design页,online-DRC项中设置
      E：这个也是刚留意的，使用一个可使End Via Mode的设置向下跳一项，
            即按end  no via/end via/end testpoint这三个设置循环跳
      F：不知道有什么用
      G ：同时设置design grid和via grid
GR ：设置design grid
      GV ：设置via grid
GD ：设置display grid
      GP：开关极坐标显示
HC：画2D line的画圆模式
      HH：画2D line的画不封闭线段模式
      HP：画多义线模式
      HR：画长方形模式
      L ：层切换
N ：高亮显示指定网络名；不加网络名，只敲N则取消所有高亮显示
O：切换显示实心线／线外框轮廓,只显示外轮廓感觉挺酷,但好像没什么用
      PL <n> <n>：设置层对
      Q：长度测量，这个应该是布局的时候常用吧
QL：这个刚知道用法的，首先选定一些网络或一些管脚对，使用这个命令来统计这些对象的长度，它会跳出一个文本窗，它是分层统计的，会列出没个网络在各个层的走线长度。
      R ：设置实显宽度的最小尺寸，小于最小尺寸统一以一细线代，大于则显示实际宽度
      RE： 取消上一次的撤消操作
      S：可查找器件或坐标
SS：查找并高亮显示对象
      SH：推挤方式的开关切换
      T：透明显示，这一模块可以看清不同层重叠的线以不同颜色显示，但这种模块也有一个不足之足就是选定一个对象后（对象高亮），不能通过鼠标点取别处而取消其高亮，而是需要刷新。
      UN：undo撤消操作
      V：过孔操作
W：改变线宽
      X：切换是否显示文本的外框，不包括元件标号
       上面看得不爽就看这个吧，人家的文档能力比我强
PADS logic的一个BUG 
发表于 2007/11/23 0:36:01 
        在PADS2005中,有时会出现无法跨文档copy选定的对象,具体情况如下,用logic打开两个原理图设计图,copy其中一个设计图A中的电路模块(Ctrl+C)到剪贴板中,然后在另一个设计图B中进行粘贴,出现无法粘贴的情况(粘贴不出东东),
        上面这种情况解决方法: 在设计图B中任选定一个对象,进行复制操作....当然不用实际完成操作,在复制对象粘住鼠标即可.....按ESC取消....
         进行完上面在设计图B中的操作后,就可以从设计图A中copy电路模块到设计图B了
PADS手记(一) logic快捷键 (07 1122) 
发表于 2007/11/22 13:38:49 
今天是第一次写, 先总结一下吧:
===============================================================================
1. PADS logic快捷操作:
    F2                     -------   add connection   连接线
    M                       -------   popmenu            弹出右键菜单
    Ctrl+A               -------    select All            全选
    Ctrl+B               -------    sheet                  切换到整线sheet可以的视图状态
    Ctrl+C               -------    copy                   复制选定对象(可以是多选或选一范围): 也可以在按住Ctrl同时拖动选定对象进行该操作
    Ctrl+D                -------   redraw               刷新
    Ctrl+E               -------    move                  移动选定对象(可以是多选或选一范围): 也可以直接拖动
    Ctrl+F               -------    X mirror               X轴镜向操作(对象需处于拖动状态)
    Ctrl+N               -------    new                   新建设计
    Ctrl+O              -------    open 打开设计
    Ctrl+P               -------    print                   打印
    Ctrl+Q               -------   query/modify       查询选定（单个）对象的属性(part, net等):  也可以通过双击对象进行 该操作
    Ctrl+R               -------   rotate 90             对单个对象进行旋转90度操作(对象需处于拖动状态)
    Ctrl+S               -------   save                    保存
    Ctrl+V                -------   paste                   粘贴
    Ctrl+W              -------    zoom                  切换到放大镜操作状态：单击左键放大视图，单击右键缩小
    Ctrl+X               -------    cut                       剪切      
    Ctrl+Alt+C         -------   display color       展出颜色设置窗口
    Ctrl+Shift+F      -------   Y mirror              Y轴镜向操作(对象需处于拖动状态)
    Ctrl+Alt+G          -------   preference         设置窗口
    Ctrl+Alt+S         -------    调出状态窗口
    Ctrl+Alt+F         -------    过滤器窗口
    Ctrl+Shift+O      -------    打开pads layout link窗口
    无模式命令：
s                      --------  search                 查找对像或坐标
     q                      --------  measure             测量长度
     un                   --------   undo                   撤消上一次操作
     re          --------    redo 恢复上一次撤消
===================================================================================
