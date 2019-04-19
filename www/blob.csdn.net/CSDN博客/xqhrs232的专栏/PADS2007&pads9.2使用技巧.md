# PADS2007&pads9.2使用技巧 - xqhrs232的专栏 - CSDN博客
2013年06月28日 17:52:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2213
原文地址::[http://hi.baidu.com/yuanvzhu/item/cdeea9ca4877091050505870](http://hi.baidu.com/yuanvzhu/item/cdeea9ca4877091050505870)
今天上传三个持续更新的文章，把自己学习中遇到的问题和解决的技巧贴上来，给自己和大家共勉吧!
1.用filter选择要删除的东东，然后框选，delete即可 或者：无模下右键Select Net-Select All-Delete
2.在Filter中只选Lable，在板图上框选整个电路板，将选中所有的元件标号，选择Property，可以同时修改所有元件标号的大小、字体的粗细等。
3.Solde Mask是用来画不要绿油的吗？这一层是在PCB板生产商生成的吗？//这一层是阻焊层，每个元件脚都有阻焊的。你要做好特殊的部分，正常的部分不用理。至于在哪里生成，就随意了。我一般是生成后给PCB厂家。
4.Paste Mask是用来干吗的？是不这一层只有去贴片厂才生成的？//这一层是锡膏涂布层，只有贴片元件的焊盘才有，一般你出好GERBER给钢网厂。
5.在PADS焊盘对话框中[Offset]编辑栏起什么作用？//主要是用在一些焊盘和过孔的中心不在一点上的焊盘，也可以灵活地应用在其他方面。
gerber 文件的生成，作用，等等。
6.加入公司格式框的步骤：Drafting Toolbar---from library---*******(库名字)库选择即可。
7.Gerber文件输出的张数，一共为N+8张：
     n张图为板子每层的连线图
     2张丝印图（silkscreen top/bottom）
     2张阻焊图（solder mask top/bottom）
     2张助焊图(paste mask top/bottom)
     2张钻孔图(drill/Nc drill)
8.在PADS输出光绘文件时去掉自动添加的文件名，不选择Plot Job Name即可。
9.脚间距,BSC是指基本值,其它还有TYP(典型值),REF参考值
10.在利用PADS2007做新的器件封装时，如果在多个库中有同一个封装，数据库在定义part与decal对应关系时容易出错。应尽量少出现同名的封装。删除同名的封装后要同新建立part，以便建立part与decal的数据库联系，不然就会出现灾难性的错误。
11.25层为内层负片的安全间距层，在DIP焊盘设计时要比孔径大20MIL。
12.在Lay有接插件的板子时，要注意接插件要表明电气连接意义，便于板子的维修和实验。
13.在画板子时要注意画出板子的各个层的数字，同时能敷铜的尽量敷铜。
14.做完后检查布线的粗细，能粗的不是信号线就尽量粗。
15.打开别人的PCB敷铜只有边框的处理办法：tool---pour manager--flood
16.自动推挤功能是在ROUTER中实现的，可以考虑以后多用router布板，然后再用layout修改。
17.PCB的敷铜灌铜的方法，设置：tools --option---thermals---drilled thermals --pad shape:4个均选为Flood over。然后下面选择Routed pad thermals。
             操作如下：drafting toolbar --copper pour --选择需要的敷铜区域，自封时右击，add drafting --选择layer ，net，option 中默认，flood over vias。点ok即可。
18.泪滴的正常补法：tools --optiongs ---routing--options 中选择generate teardrops。打开补泪滴功能。右击选择要补泪滴的线，选择deardrop properties ，设置相关的选项。
19,增加或者删除PCB层数的步骤:setup--layer definition--modify--输入电气层的数量
20.PCB增加边缘倒角的步骤：选择boardoutline---光标防止要倒角的位置左击--右击选择add miter--输入倒角半径--回车---选择倒角--右击--pull arc即可。注意对于在option中design--miters--ratio如果设置过大，则在拉弧形倒角时半径很大，改小即可做出来较小的倒角。
21.对于Solder Mask Layers
和Paste Mask layers这个两个概念，有很多初学者不太理解这两个层的概念，因为它们的确有一些相似的地方，就自己的看法说说，贡大家参考：
Solder Mask Layers:即阻焊层，就是PCB板上焊盘（表面贴焊盘、插件焊盘、过孔）外一层涂了绿油的地方，它是为了防止在PCB过锡炉（波峰焊）的时候，不该上锡的地方上锡，所以称为阻焊层（绿油层），我想只要见过PCB板的都应该会看到这层绿油的，阻焊层又可以分为Top Layers R和Bottom Layers两层，Solder层是要把PAD露出来吧，这就是我们在只显示Solder层时看到的小圆圈或小方圈，一般比焊盘大（Solder表面意思是指阻焊层，就是用它来涂敷绿油等阻焊材料，从而防止不需要焊接的地方沾染焊锡的，这层会露出所有需要焊接的焊盘，并且开孔会比实际焊盘要大）；在生成Gerber文件时候，可以观察Solder
 Layers 的实际效果。
Paste Mask layers:锡膏防护层，是针对表面贴（SMD）元件的，该层用来制作钢膜（片）﹐而钢膜上的孔就对应着电路板上的SMD器件的焊点。在表面贴装（SMD）器件焊接时﹐先将钢膜盖在电路板上（与实际焊盘对应）﹐然后将锡膏涂上﹐用刮片将多余的锡膏刮去﹐移除钢膜﹐这样SMD器件的焊盘就加上了锡膏﹐之后将SMD器件贴附到锡膏上面去（手工或贴片机）﹐最后通过回流焊机完成SMD器件的焊接。通常钢膜上孔径的大小会比电路板上实际的焊小一些﹐通过指定一个扩展规则﹐来放大或缩小锡膏防护层。对于不同焊盘的不同要求﹐也可以在锡膏防护层中设定多重规则，系统也提供2个锡膏防护层﹐分别是顶层锡膏防护层（Top
 Paste）和底层锡膏防护层（Bottom Paste）.
22.对于印制板，共点接地是非常重要的，导线的阻抗影响电压的采样，特别是大电流的情况下，走线本身的阻抗都比采样器件的走线大。同时采样的线也不能在高频信号附近，否则会受到高频信号的干扰。走线一般是1mm宽，35um厚,6mm长的为1毫欧，明天这个再好好查一下。70um则阻抗减半。
23.在用pads敷铜时，敷铜皮和hatch的灌铜是不一样的，在敷hatch的灌铜时，需要将外框线设置为0.254左右，远大于能显示的最小尺寸，这样的敷铜才是整体的铜块。
24.由于安装了win7，因此不得已升级原来的pads2007，可之后发现原来的器件的lib库却不能导入了，百度了一下，找到下面的转换方法：在你安装目录下去找这个程序就行了.
...\MentorGraphics\9.2PADS\SDD_HOME\Programs\Libconv9.exe 
也可以在开始菜单里面程序\Mentor Graphics SDD\PADS 9.2\里面有个Library Converter
使用上面的转换工具，即可自动转换为PADS9的后缀的库文件，验证可以使用。此方法来源：[http://www.ourdev.cn/bbs/bbs_content.jsp?bbs_sn=4549002&bbs_page_no=27&bbs_id=9999](http://www.ourdev.cn/bbs/bbs_content.jsp?bbs_sn=4549002&bbs_page_no=27&bbs_id=9999)
25.将altium designer 中的原理图网络导入 pads layout中的方法（来源网上）： 输出网表格式选PADS，生成一个.net文件和一个.prt文件。打开.prt文件，删除文件尾部的*END*.打开.net文件，删除文件头部的*PADS-PCB* 。全选.net文件，复制后粘贴到.prt文件末尾。将文件另存为.ASC文件。打开PowerPCB，Import保存的ASC文件即可。注意：Protel的封装一定要在PowerPCB的库中存在，对应Part项。导入ASC文件时有一警告，提示网表格式不对，可以不必理会。如果不想出现警告，将ASC文件第一行的*PADS-PCB*
 改为!PADS-POWERPCB-V5.0-BASIC! DESIGN DATABASE ASCII FILE 1.0
26.pads 9.2 layout 新增在pcb中显示网络功能，无模命令为 NN 和NNP 。
27.pads logic 中在制作原理图库文件时出现“incomplete information in part type ********:*** ”的错误原因为，按照教程的描述，在part information for part定义时，又重新定义了pins的属性，造成重复，如下图所示：
![](http://hiphotos.baidu.com/yuanvzhu/pic/item/ae5374eda1b652a5b11cb17b.jpg)
，删除生成的下面的signal pin即可。
28.在pads layout 中，如果将某部分器件设置成了一个组（group），则无法和其他器件一起进行移动和修改一些参数，解除或者解散这个组的方法是，选中组中的一个器件，右击-----select union ---- 选中了所以的组中的器件 ---- 放所有期间上右击---- 右键中的break 或者break allunion 等命令即可出现，进而可以进行删除或者解散组的操作。
29.在logic 导入layout时 会有part type 导入并显示在layout的顶层和底层上，关闭的方法是激活 器件的选择模式，选择器件，右击----properities（属性），在弹出的component properties 中，选择下面的属性设置区中的labels ，在label中选择 part type ，点击下面的part label properties ，在对话框中的attribute中 确认为part type，在 show属性中设置为none，而非value，即可关闭所有的part
 type的显示；
30.pads logic layout sch,pcb 高版本向低版本转换的格式，原理图转换为txt后缀的格式文件，pcb文件转换为asc文件，然后分别倒入即可；
31.有时候在layout时把某一个模块进行了 make reuse的复用操作，不管是否成功，这几个器件都会相互关联起来，并且无法进行单个的移动和修改，移动时的提示为：Reuse elements cannot be modified. Break the reuse first. 解决的办法为 右键----select anything（必须激活这个选择模式）----选择刚才的reuse器件----右键----break reuse----完成。（突然发现和上次遇到的28问题类似，但不一样，呵呵）
32.pads layout 的放大和缩小按键，滚轮等失效的解决办法：tools - customize - keyboard and mouse --如下图所示，
![](http://hiphotos.baidu.com/yuanvzhu/pic/item/5a3a9dcc7cd98d10cd60c47b213fb80e7aec9028.jpg)
点击reset all复位鼠标和按键的属性即可；
***********************************************************************************最后更新于11.12.15
