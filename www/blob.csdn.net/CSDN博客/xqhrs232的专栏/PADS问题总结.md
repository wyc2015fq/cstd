# PADS问题总结 - xqhrs232的专栏 - CSDN博客
2013年07月10日 11:27:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2052
原文地址::[http://www.astiny.com/?p=1164](http://www.astiny.com/?p=1164)
1问：PADS2007为什么每次打开以前的覆铜都看不见，非要重新覆铜，各位大侠请指教。谢谢
答：1.这好像是软件为了节省内存而采取的做法，其实也不用重新覆铜，点 view——nets，然后确定，就可以显示覆铜了。
2. 不同楼上，应用pour manager下的HATCH，恢复灌铜
2问：为什么在pasd中我把阻焊层的颜色打开了，确看不见pin的阻焊窗．只能看见过孔的，导入到cam中，就可以看见了，为什么，难到在pads里边真的看不见吗？
答：power pcb 中是看不到PIN的阻焊窗，因POWER PCB软件中没有阻焊层，只能在GB设置里面先设好，在导放CAM350才可以看到。这就是POWER PCB软件缺限,呵呵！经高人指点得出的结果！
3问：画PCB时需要修改PCB封装， ECO to pcb后发现封装没有改变，望指教。非常感谢！
答：1.我一般是先在PCB里面删除掉要更改的元件后,再导入ECO，对于修改PCB DECAL的比较适用。
2．是不是没有比较封装啊？(我试过了,不行啊)
![图片](http://www.astiny.com/wp-content/uploads/auto_save_image/2012/10/021637390.jpg)
3．很简单： 在PCB文件里右键要修改的元件（与你元件库中已修改的封装同名的）Edit Decal ，现在打开显示的是旧封装，不管它，点选File-Open 你库中已修改的封装，会问要不要保存现在的文件，否。 打开封装后再Exit Decal Edit 退回Layout，问你：点ＡＬＬ。全部改了。
![图片](http://www.astiny.com/wp-content/uploads/auto_save_image/2012/10/021639TIV.jpg)
![图片](http://www.astiny.com/wp-content/uploads/auto_save_image/2012/10/02164223p.jpg)
4问：ORCAD原理图导入到PADS是不是导入不了元件值啊？
答：主要流程如下：
1.OrCAD输出网络表时要添加{Value}，使得输出的网络表里含元件值，然后转入Pads才有可能有元件值
2.Pads里右键选select comp，全选所有元件，然后右键弹出点选属性，在label一栏点图标new，就ok了。
5问：pads logic菜单错误怎么办？
答：tools-customize 把里面的菜单栏和键盘都reset一下，就ok了。
6问：DXF的文件我导入以后怎样转成PCB的板框？
答：避免AUTOCAD 文件轉 POWER PCB 單位出錯方法避免AUTOCAD转POWER單位出錯問題的方法：
1.在AUTOCAD中先選中圖形使用”PURGE”命令將所有附屬圖層,只留0層;
2.在AUTOCAD中先選中圖形,使用”MOVE”命令將圖移動至0點{鍵入0,0}
3.在AUTOCAD中鍵入”W”命令(WRITE BLOCK),設置原點,選擇圖形，要注意单位的更改。通常使用的是　　　”METERS”來做單位,確認後會自動存儲為 “NEW BLOCK.DWG”.關閉文件.
4.打開剛剛存儲的”NEW BLOCK.DWG”文件,檢查無問題後另存為 AUTOCAD R12/LT2 DXF 格 式的文件. E365 5.使用 POWER PCB 導入”IMPORT”,導入後可看見 DXF－File Unit 為” METRIC” 是正確的。
6.Power PCB 中選擇已導入的圖框，後進行組合(Combine)，再將圖框放入改為其他層面。 在AutoCAD中把线弄成闭合的2D Line导出后，在Power PCB中可直接用Scale命令改成板框。
AutoCAD改闭合线的方法：
1． 在命令栏中输入：PE，选择其中一条线，回车 ，按J 后选择所要闭合的线，直接回车、回车便可。
7问：pads logic 原理图 请问如何在一根导线上放置网络标志 或者多个？
答：logic不允许这样做。一根信号线只允许一个网络。有时候原理图为了标识清楚，可用TEXT来表示。
8问：所谓paste mask是指？
答：所谓paste mask是指PCB裸板上的SMD焊盘刮锡膏以后的那一层.通常为了在回流焊机上贴片而涂上的焊锡膏,我们平时说的钢网就是专门对应于这一层。
9问：用ORCAD画原理图，用PDAS画板。做好的东西的基础上改动一些元器件。可以在原来做好的板的基础上改吗？
答：ORCAD原理图修改完成后，再生成网表，与PCB进行COMPARE ECO，然后进行UPDATE就可以了。
10问：用ORCAD画原理图，PADS画PCB，原理图中的元件封装应该怎么设？
答：1．要用PADS画PCB，那封装就肯定要设置为PADS中得封装，需要注意的是用ORCAD画原理图设置封装时需要对应得封装是PADS封装库中得Part Type，而不是Decal。
11问：Layer25层的作用？
答：Layer25层是插装的器件才有的，只是在出负片的时候才有用，一般只有当电源层定义为CAM Plane的时候geber文件才会出负片（split/Mixe也是出的正片），如果不加这一层，在出负片的时候这一层的管脚容易短路。
PowerPCB中对电源层和地层的设置有两种选择，CAM Plane和Split/Mixed。Split/Mixed主要用于多个电源或地共用一个层的情况，但只有一个电源和地时也可以用。它的主要优点是输出时的图和光绘的一致，便于检查。而CAM Plane用于单个的电源或地，这种方式是负片输出，要注意输出时需加上第25层。
第25层包含了地电信息，主要指电层的焊盘要比正常的焊盘大20mil左右的安全距离，保证金属化过孔之后，不会有信号与地电相连。这就需要每个焊都包含有第25层的信息。而我们自己建库时往往会忽略这个问题。
Layer25层的替代设置：
在PADS的焊盘设置中，有一个AntiPad的设置，只要能使这一项（选择焊盘类型即可），其焊盘的初始设置值即为普通焊盘+24mil或0.6mm，看这一设置的功能及效果看，可以替代Layer25的作用，而且这样的设置感觉上做法也较为正规一些。只是相对来说Layer25的作法历史悠久，很多人已经习惯了，新手们可以试试。
还有一点就是使用Layer25层可以在建元件的时候就设置好这一项，而AntiPad则需要在布板中设置，对于过孔的处理就差不多，可以给过孔加layer25也可以设置过孔的AntiPad。
具体Antipad的设置，博客中已经有一篇文章里已附了一个PDF文档，大家可以看看。
总的来说，不管是用Layer25还是Antipad，其最终的目标有两个：一是上面提到的金属化过孔时防止短路；二是减小过孔的感生电容电感。过路高手如有不同意见，还望赐教…
之后把pads的颜色调整好，否则因为颜色是黑色元件值会显示不出来。
12问：能预先设置线宽吗? 走线时老是要走不同线宽, 但是走线时每次都输入宽度很麻烦! 如果改规则也不方便! 有预先设置线宽的无模式命令或快捷键吗?
答：我的建议就是先设置好一个走线宽度，走完与它相关的走线后，再变更一下宽度就好了，也挺方便的w，毕竟多 数 的走线宽度是差不多的，也就电源与地那会有所不同吧，用w也挺方便啊。
13问：批量改变元件所在层, 比如元件在TOP层，现在需要将元件移动到BUTTOM层。
答：可以的，选择所需改动元件，右击，选择properties,里面可以改层。
14问：Pads怎么能设置网络的线宽, 在布置电源和地时线宽和信号线都布一致，有没有方法让同一个网络的线宽是一致的，比如让vcc都是0.5的宽度，GND是1的。
答：setup–design rules—-选net ,在左边nets中选你要设置的net,如选GND，点Clearance，在Recommended下填入想要的线宽即可。。。
15问：flood与hatch的区别?
答：flood:重新灌铜，hatch:按你上次灌铜后的轨迹恢复灌铜。
16问：在layout中pcb已经布好了线，但是封装不对，想换封装，又不删除布线，怎么办呢？
答：选中它，右键，DEIT DECAL，然后调出正确的封装。退出，OK
17问：在拐角处怎么会有方框？怎么消除？
答：在PREFERENCE参数对话框里，把ROUTING标签里的PREFERENCES里的SHOW TRACKS关掉，就没有了。
18问：看到好多的原理图，如果有一个信号在其它页里也有，就会标示出来，如 DAA[1.3.5] 这样。
答：
![图片](http://www.astiny.com/wp-content/uploads/auto_save_image/2012/10/021645CSr.jpg)
19问：求助 PADS两块pcb板怎么才能合拼成一块，没原理图。
答：使用reuse 功能。打开一块板子的PCB,全选,点键按MAKE REUSE.会提示你保存,输入自定义的文件名.即可将它保存为reuse 文件. 然后打开另一个PCB ,按ECO TOOLBAR 下的add reuse,即可指定文件,将刚才保存过的reuse 文件加进来.选择你要摆放的位置即可. 在添加过程中,Refdes. 名会要求你增加前缀or后缀
20问：在PADS PCB中怎么设置非金属孔?
答：应该是在PAD STACK 的PLATED。
![图片](http://www.astiny.com/wp-content/uploads/auto_save_image/2012/10/021646D95.jpg)
21问：想去除电路板上的一些覆铜和走线上的绿漆，使之裸露，请问如何做？
答：在SOLDER MASK层上编辑就可以。
22问：那1盎司是多少呢?到底是多厚呢?
答：35UM。
23问：焊盘间距0.5MM，焊盘大小0.27MM的BGA怎么扇出？
答：fanout不出，打在焊盘上吧，钻0.1mm的激光孔，然后用塞孔工艺。不然就只能用盲孔了。
24问：solder mask&paste mask
答：solder mask:阻焊.通常开窗补偿为4-10mil，paste mask:锡膏。通常比solder mask大。
我们从PCB板厂拿到的板就是solder mask, 而送去SMT加工的话,就需要做钢网,就是锡膏层,paste top和paste bottom这两层。
PROTEL中是这样的：
![图片](http://www.astiny.com/wp-content/uploads/auto_save_image/2012/10/021647Ozk.jpg)
Top solder为加锡的地方。
25问：怎么在管脚名称上放低电平触发标志上划线？
答：这个和Protel 是一样的。
26问：在pads logic中找了半天，没有找到自动元件排列选项。请问logic是不是没有这项功能？
答：pads2007 logic的確沒元件编号自动排列, pcb端有renumber功能,可以用ECO方式傳回logic。
27问：怎么样设置成网状敷铜？
答：tools->options->gridsE Hatch grid copper:此外设置为显示格点的3倍或更大，wkeepout:显示格点的倍数或相同就可以。
28问：最近有個新案,用到BAG176封裝的IC,球徑(直徑):0.4+/-0.05mm ,兩點間距e:0.65。請問,BGA的過孔大小應該設置為多少,還安全間距。如果將間距設置為:0.1mm,那走線最粗只能是:0.05mm
![图片](http://www.astiny.com/wp-content/uploads/auto_save_image/2012/10/021650TZw.jpg)
答：用不着6mil，外两排可以拉出BGA外面打孔。内两排打盘中孔，板厚不能超1.2mm，层数限六层。
29问：前两天我的一个同学问了我一个问题，为什么铺了铜还是有很多的飞线，好像没有铺铜一样，不过检查错误却没有误，怎么回事呢？
答：PADS就是这样的，检查连接性没错就好，飞线可以不用理它。
30问：为什么BGA要自动扇出呢？
答：自动fanout效率比较高，而且整齐。fanout完成后，再进行手工布线
