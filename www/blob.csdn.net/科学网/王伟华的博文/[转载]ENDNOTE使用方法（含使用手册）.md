# 科学网—[转载]ENDNOTE使用方法（含使用手册） - 王伟华的博文




# [转载]ENDNOTE使用方法（含使用手册）                           

已有 2994 次阅读2014-4-9 21:12|个人分类:[科技论文写作、搜索经验集锦](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=104604&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|文章来源:转载



**一、简介**
　　EndNote是一款用于海量文献管理和批量参考文献管理的工具软件，自问世起就成为科研界的必备武器。在前EndNote时代，文献复习阶段从各大数据库中搜集到的文献往往千头万绪、或重复或遗漏，难以管理，阅读所作的笔记则分散各处，难以高效地进行有机整合。到写论文时，大量的文献引用往往复杂异常，尤其修改时，牵一发而动全身。这些难题，EndNote可以凭一己之力，彻底解决。 
　　学校里目前盛行的依然是EndNote 9.0，而功能更为强大的V10.0正版需要$100，显然不宜在我们中间推广。故本文以V9.0为主要介绍对象。它的安装对大家并不构成问题，只需要注意这类中文支持功能较弱的软件，安装路径最好纯由英文字母和数字构成。 
　　打开EndNote 9.0，其界面如图1.1所示。“Reference Library”窗口显示文献的各个条项，单击条项名称(如"Author")按钮，所有记录就将按该条目顺序/逆序排列。预览窗口显示被选中文献的简单信息（图1.1中的预览格式是默认的"Annotated"，在下拉菜单中还有其他的格式）。“显示/隐藏Preview”按钮则用于切换预览窗口的显/隐属性。 
![](http://image.sciencenet.cn/album/201404/10/163232lvt9enn9oosqo8vm.jpg)



图1.1 EndNote 9.0 基本界面 

　　下面根据我们的专业特点和需要，结合我使用EndNote的一些心得，介绍这个软件的基本操作。 

**二、建立和编辑Enl文献图书馆**
　　EndNote数据库称为Reference Library，以*.enl格式存储，其中的数据存储于同名文件夹*.Data中。本文所举例子中的Library包括单记录图书馆“acedemic.enl”和数据文件夹“acedemic.Data”。 
　　(1)新建enl 
　　File-->New，或者单击工具栏第一个按钮"New Library"，都可以新建一个空白图书馆。 
　　(2)打开既有enl 
　　File-->Open-->Open Libray，或者单击工具栏第二个按钮"Open Library..."，都可以打开一个既有的enl图书馆。 
　　(3)新建记录 
　　Reference-->New Reference，或者在"Reference Library"窗口中右键-->"New Reference"，都可以手动添加新记录。 
　　(4)编辑记录 
　　"Reference Library"窗口中双击选中的记录，或者右键"Edit References"，都可以进行编辑。 
　　编辑记录界面如图2.1所示。一般需要用到的条项包括Author、Year、Title、Journal、Volume、Issue、Pages、Keywords、Abstract，读书笔记记录在Note中，如果有全文pdf，可以将其链接添加到Link to PDF，并将附件链接到Image中，如果文摘是从网上数据库下载的，URL中可记录其出处。其余条项相对用得不多。 

![](http://image.sciencenet.cn/album/201404/10/163536ptpftk3n3n3jjwwr.jpg)


图2.1 EndNote记录编辑窗口 

　　1) Author：每个作者列一行，格式可以是Ziaei, J. E.，Ziaei, J E，J E Ziaei，或者标准全名等。原则是姓置首，则后面必须跟逗号，若按照西方姓名规范置尾，则不必加逗号。逗号后应有空格。如果是中文名字，一般不需要添逗号区分姓和名。在EndNote中，编辑状态下，用红色显示的人名表示当前Library中该名字是第一次出现，若该人名先前记录中出现过，则黑色表示。 
　　2) Year，Title，Journal，Volume，Issue，Pages：这些条项照抄引用文献的显示即可。 
　　3) Abstract：同Author，每个关键词列一行。 
　　4) Link to PDF、Image：右键"Link to PDF"、"Image"即可添加。添加"Image"后，在Reference Library窗口中这条记录前方就会出现一个附件标志。EndNote 10.0中，可以直接将全文PDF拖放到这个栏位，更为便捷，而且链接永久有效，不会因为相对路径的变化而失效，而9.0在这一点上依然做得不好，所以使用这个版本建立参考文献图书馆时，必须做好合理的规划。 
　　(5)复制记录 
　　将一条记录复制到另一个文献图书馆中，只需要在Reference Library窗口选中目标记录，^C或右键“Copy”，再打开目标文献图书馆的enl文件，^V或右键“Paste”，即可将它复制进去。剪切的方法与之相似，^X/“Cut”即可。 
　　(6)删除记录 
　　在Reference Library窗口选中要删除的记录，右键"Delete References"即可。选中时可以用鼠标拖选多条记录，也可以按住Ctrl间隔点选。直接选中后按Del键无效。 




**三、批量导入文献**
　　手工编辑enl是很费时费力的。EndNote最重要的功能之一就是把科研人员从繁重的文献手工编辑工作中解放出来。很多在线数据库都提供了强大的Citation导出功能。我们专业主要使用的是中国期刊网、PubMed、Ovid、Elsevier等中英文文摘/全文数据库。下面以它们为例，简要介绍文献批量导入的操作。 
(一)基本步骤 
　　(1)查找所需文献 
　　这一步不需要多讲，属于文献检索的基本范畴。 
　　(2)选择所需文献 
　　在所要导出的文献条目前打勾选择所需文献。如果检索已经比较精细的话，可以不用打勾，即默认全部导出。
　　(3)导出citation 
　　1)PubMed（直连[http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?DB=pubmed](http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?DB=pubmed)）：结果预览窗口中，Display下拉菜单选择“MEDLINE”，Show下拉菜单选择一个合适的数值以确保能将所需文献全部导出(但一次最多只能导出200条记录)，旁边的下拉菜单可以选择默认的"Send to"或者"Text"。三步操作尽量快速地同时完成，网页自动刷新后即可显示完整格式的citation。 
　　2)Ovid（通过学校图书馆代理连接Online版）：在结果预览窗口下面的面板里，点选"Full"和"Direct Export"，按确定按钮。 
　　这时EndNote会自动运行，并跳出导入对话框。Filters选择框中选择Medline(Ovid)，即可导入当前处于激活状态的那个Library中。 
　　3)Elsevier（也要通过学校图书馆代理连接Science Direct）：基本导入方法和Ovid接近。关键在于选择正确的导出格式(MEDLINE或FULL)，以及自动导入时选择正确的Filter。（编选Filter是门很有技巧的学问，有时候需要自己编制，这部分内容需要参考相关手册） 
　　4)中国期刊网：EndNote的中文支持功能较弱，没有提供任何一种中文数据库的Filter，这就意味着除非能够自己编制Filter，否则中文数据库将无法批量导入。中国期刊网的Filter有一些民间版本，但是随着知网改版，类似于哈工大版那样“权威”的普及版Filter目前还没有出现。 
　　(4)导入citation 
　　1)PubMed：网页显示出完整的Citation以后，全部复制，并粘贴到一个记事本文件中，保存。打开EndNote，按工具栏第三栏中"Import..."按钮，即跳出导入对话框，"Import data file"框中打开保存citation的记事本，"Import Option"下拉菜单选择"PubMed(NLM)"，"Duplicates"下拉菜单可以选择默认的"Import All"，也可以选择"Discard Duplicates"剔除与Library中重复的记录。 
![](http://image.sciencenet.cn/album/201404/10/164804am2ci2lpcpl35pp7.jpg)



图3.1 Import对话框界面 

　　2)其他：如果由于版本原因，Ovid或Elsevier的citation无法直接自动导入EndNote，那么与PubMed一样，要先导出到一个文本文件中，再从文本文件中通过Filter导入到EndNote。 

（二）简便步骤 

　　直接打开EndNote，Tool-->Connect-->Connect...，在弹出的对话框中选择一个目标数据库，随后就跳出三栏的Search窗口，可以在这里进行关键字检索，结果会自动汇入当前处于激活状态的Library中，并且URL条项中显示每条文摘在数据库中的链接地址。

![](http://image.sciencenet.cn/album/201404/10/164743y8yhl8d9ls2y2okk.jpg)


图3.2 EndNote检索窗口 

　　这一方法十分简便，但是在EndNote9.0中似乎并不支持代理。必须直接通过公众网和账户/密码方式连接数据库，而多数数据库的账户/密码是要付费获得的。所以一般来说，只有PubMed这样的免费数据库比较适合这样操作。 

**四、显示格式的初级修改**
　　在预览窗口中，EndNote默认的Annotated格式是citation+abstract模式。除此以外，还有Author+Date、Numbered和Show All。这些格式中Annotated提供的信息全面而精简，比较适合筛选和预览。但是这一显示格式有些弊病，比如citation的姓名输出格式，和条项排列顺序，都不太符合文献引用的一般规范。可以做一些修改。 

　　Edit-->Output Sytle-->Edit "Annotated"，打开编辑窗口。 
　　(1)Citations/Author Lists，如图4.1所示。一般引用文献显示前三位作者，因此把默认数字如图修改。et al是否斜体显示根据实际情况取舍。（注：文本框中填写的逗号后要跟一个半角空格） 

![](http://image.sciencenet.cn/album/201404/10/1647169e798lbem2rwhb4a.jpg)


图4.1 Citation/Author Lists 

　　(2)Citations/Author Name，一般文献引用的作者姓名按"姓 名"格式显示，如图4.2。

![](http://image.sciencenet.cn/album/201404/10/1646541n8yfbecietvvn1u.jpg)


图4.2 Citations/Author Name 

　　(3)Bibliography/Templates。Journal Article的格式一般如图4.3所示。将Year选中后拖动到Volume前，加空格（空格显示为·）。Journal后可以加reference type（如[J]表示期刊，等），光标定位在Journal后，按Insert Field按钮，点"Reference Type"即可插入。Book的格式中，一般可以将Year移到City前，并嵌入Edition。其余几种文献类型的格式根据需要，同法修改。 

![](http://image.sciencenet.cn/album/201404/10/1646254gdtgdtcdod9nxcc.jpg)
图4.3 Bibliography/Templates 

　　在这里，Title前后加了引号，Journal下加了下划线，Volume加了粗，都是为了突出显示。如果要取消，选中相应条项，按工具栏上相应的字体按钮即可。 
　　(4)Bibliography/Author Lists和Author Name的修改同Citation。 
　　(5)Bibliography/Layout：End each reference with文本框中是预览显示格式。当前的显示格式已够用，如果需要加入Keywords或者Notes，光标固定在Abstract后，按Insert Field按钮，点End of paragraph、Tab及Keywords或Notes。个人不推荐这样做，因为预览起来比较乱。 
　　其余可改可不改。修改完后关闭，点保存修改。 

　　Annotated以外的其他显示格式的修改，可以通过Edit-->Output Style-->Open StyleManager实现。一般来说，Amer J Epidemol这类杂志的显示格式都是按杂志引文格式要求编制的，不需要再做修改。

**五、记录的导出**
　　文献图书馆可以导出为各种文本文档。首先，按照上一节所述，将显示格式调到所需的状态，然后按工具栏"Export..."按钮。在弹出对话框的保存类型下拉菜单中，有txt、rtf、htm和xml四种格式。rtf格式可以用word打开，另存为doc格式。如果把显示格式调为某一种杂志，那么导出的就是一张完整的citation列表。 

**六、文献的管理**
　　配合嵌套文件夹的使用，即可进行海量文献的管理。以综述为例，确定范围和题目后，就可以充分利用EndNote的功能进行文献搜集、整理和阅读了。 
　　(1)充分收集文摘 
　　通过在线数据库查找的文摘都可以导入EndNote文献图书馆中，为了充分收集资料，还有一些资料可能要通过图书馆复印的途径获得，这些资料一般数量不多，可以手动输入。 
　　我的经验是，尽量细化检索词组合，对每一种检索策略建立一个独立的enl图书馆，并放在同一级文件夹下。细化检索词是为了让每一个enl库中的记录尽可能少，最好不要超过100。 
　　(2)筛选文摘 
　　通过Annotated模式对每个enl图书馆中的记录进行逐一筛选。多数情况下，都可以筛掉很大一部分相关度较差的文献。对于要保留的文献，不妨在Notes中随时记录下观感或一些关键词，便于后面进一步整理。这一步工作比较枯燥，但却是后面所有工作的基础。 
　　(3)合并记录 
　　对检索策略相近的几个文献图书馆筛选完毕后，可以将它们合并成一个库。在这一步，推荐采用Edit-->Find Duplicates菜单筛去重复的记录。这样，就能形成一个次级库。通过这样的方法，在次级库中进行合并，就能获得一到若干个最终库。 
　　形成最终库时，需要对综述的结构有所规划。这些规划体现在Notes中。有些文献要用在背景部分，Notes中可以标注“backgroud”，有些可能要用在展望部分，Notes中可以标注“view”之类。通过Reference-->Search References操作，In "Notes" Search "Certain Keywords"，就可以把它们分别显示，并归入不同的目标库中。 
　　(4)收集全文 
　　在最终库里的文献就是所需要的。接下来尽可能把它们的全文都弄到手，并按二(4)节所述，做好Link to PDF和Image。以后写论文需要参考全文时，右键Open PDF即可，十分方便。 

**七、Word中的文献引用**
　　文献图书馆建完备后，就要着手写论文了。EndNote最杰出的优点之一就是可以自由地插入和调整citation。这在大文献量的论文写作中十分有用。 
　　安装EndNote后，Word中会相应地生成EndNote插件。在工具栏（如图7.1）和工具菜单中都有它的选项。假设我们要在论文中插入第一节所示的那篇文献{Ziaei,2004}，那么操作如下： 

![](http://image.sciencenet.cn/album/201404/10/1641128cmkpaad18ptam8c.jpg)


 图7.1 Endnote工具栏 

　　(1)光标移至插入位置。 
　　(2)工具-->EndNote 9-->Go to Endnote或EndNote工具栏第二个按钮打开EndNote。 
　　(3)在EndNote的Library窗口选中要插入的记录，来到Word，菜单/工具-->EndNote 9-->Insert Selected Citation，或EndNote工具栏第一栏第四个按钮，该记录即汇入Word插入点。 
　　(4)如果citation显示格式不对，那么通过工具-->EndNote 9-->Format Bibliograhpy或者EndNote工具栏第一栏第三个按钮，进入格式选择对话框，如图7.2。 

![](http://image.sciencenet.cn/album/201404/10/164555n35peycnecffydkf.jpg)
图7.2 Bibliography格式修改对话框 

　　在With Output下拉菜单里有备选的Bibliography Style，如果没有所需的，点Browse按钮，弹出所有Bibliograhpy的列表，选择一项并确定，所有citation都将被批处理为这一显示格式。如图7.3所示

![](http://image.sciencenet.cn/album/201404/10/164534qfvawqfhqgqhh5ku.jpg)


图7.3 Bibliography样式表对话框 

　　(5)要移动某条引用的话，只需要光标选中文中citation，拖放到新位置，其文末reference及序号都将随之自动发生变化。 
　　(6)删除某条引用，与普通文本操作完全一样，文末reference及序号同样自动更新。 
　　(7)一些论文中插入的Note，可以通过工具-->EndNote 9-->Insert Note或EndNote工具栏第二栏第二个按钮插入。这些插入的Note也将出现在文末reference中。 
　　(8)论文完成后，确认全部引用无误，通过工具-->EndNote 9-->Remove Field Codes或EndNote工具栏第二栏第五个按钮，将所有citation的格式宏移去并另存为一个doc文档，再进行相应的格式修饰，即可完成Reference的编辑。 

**八、中文文献的导入导出**
　　EndNote的中文支持功能比较薄弱，比如我们常用的中文期刊网、万方数据库和维普资讯网上的资源就缺少相应的Filter和Style文件。随着EndNote中文用户的增多，许多人开始为这些数据库制作相应的Filter和Style，以支持中文文摘的导入和导出。这些工作为EndNote的中文文献管理提供了技术基础。在这些工作中，哈尔滨工业大学开发的CvtCNKI.exe转换软件可以同时支持中文期刊网、万方数据库、维普资讯网甚至超星图书馆与EndNote Library的转换。因此下面将以CvtCNKI.exe转换插件为基础，分别以中文期刊网、万方数据库和维普资讯网为例，介绍如何将中文文献导入到EndNote。




![](http://image.sciencenet.cn/album/201404/10/164511sszorj1q6gjw855m.jpg)


图8-1 CvtCNKI界面 
(一)中文期刊网 
　　打开中国知网（CNKI）检索界面，键入检索词或复合检索式，搜索出结果后，勾选中意的文摘，在检索结果页面的导航栏上点击“存盘”按钮。弹出菜单中输出格式选择“自定义”，“自定义”复合框中点选全部选项，再点击“预览”。在预览窗口中全选输出结果，复制到一个文本文档（如CNKI.txt文件）中，然后保存。 
　　打开CvtCNKI.exe（目前已开发到v2.0.2.B2版），点“文件”按钮，选择保存有导出结果的文本文档，“选项”框架中“文献类型”相应地选择“期刊文章”或“学位论文”，“输出格式”选择“EndNote”。点击“转换”按钮，即生成同名的enw文件。点“察看转换结果”，即自动启动EndNote，在弹出对话框中随便选择一个既有enl文件，或新建一个enl图书馆文件，CvtCNKI即可自动将转换后的enw文件中的内容导入打开的enl图书馆。 

(二)万方数据库 
　　以学位论文库为例。打开万方检索界面，键入检索词或复合检索式，搜索出结果后，点击中意文摘后的“详细摘要信息”。在打开的详细摘要信息页面中，最顶上有“输出模式”下拉菜单，选择“详细输出格式.txt”，弹出菜单询问“是否保存”，点确定，保存为一个文本文档。注意：如果出现乱码，将文本文档另存一次，保存格式由“UTF-8”改为“ANSI”。如果仍然显示为乱码或者不能正确导入，将文本文档中的“■”全部改为回车。 
　　接下来用CvtCNKI进行导入，步骤同(1)。 

(三)维普资讯网（中文科技期刊数据库） 
　　打开维普资讯的检索页面，键入检索词或复合检索式，搜索出结果后，勾选中意文摘，点击导航栏上的“打印”，在弹出对话框中选择“全记录显示”。把新页面中显示的记录全选复制到一个文本文档中。 
　　接下来用CvtCNKI导入，步骤同(1)。 

　　新版本的CvtCNKI可以对许多个中文数据库的检索结果进行转换，并且EndNote、RiS等文献管理工具的不同格式文件也可以进行相互转换。尽管这个工具仍有这样那样的bug，作者的更新速度依然基本可以确保它的转换效率。利用这款工具，EndNote的中文支持功能就能够大大增强。




如果要实现EndNote与RevMan的文献共享，需要在EndNote的Style和Filters中安装有RevMan的相应插件。EndNote XI已经安装了相应的插件，也可以自己加装。 

1. EndNote导入RevMan
(1)EndNote导出，导出对话框中style选择RevMan4或者RevMan Ris Export（EndNote默认）。导出为txt 
(2)RevMan导入此txt，File==>Import==>Reference==>Ris格式，即可导入 

2.RevMan导入EndNote
(1)RevMan导出references，导出为txt 
(2)EndNote导入，导入Filters选择RevMan4或RevMan Ris（默认），即可导入




最新版的EndNote为X2版。这版较前几版有较大改进，最吸引人的地方是**自动下载文献全文**。虽然不能做到每篇都载到，但如果文后有可用全文链接，EN还是可以自动侦测并进行下载。 

校内用户请每次都小批量进行，不要一口气制作太大的自动下载队列。坏处有2：（1）命中率偏低；（2）大流量下载全文会加重服务器负荷，甚至导致瘫痪；（3）一次性大流量下载会触怒文献供应商，可能导致惩罚性封禁全校网段。具体请自行摸索。
















ENDNOTE这个工具从网上介绍来看还是不错的，我也下过一个美国某大学的算是OEM版，试用了一下感觉还可以，单机应有的功能基本都全了，但这类文献管理工具一旦用过就会发现，这种工具如果单是利用单机版的功能很多时候还是不够方便的，所以说选择此类工具的时候一定要与自己学校的图书馆支持联系起来。

一般国内高校图书馆中支持ENDNOTE的我感觉还是比较少，至少我所在学校是用NOTEEXPRESS，这种情况下用ENDNOTE显然不是首选了

用学校图书馆的定制版最大的好处就是可以连到网上数据库中自动下载题录及论文相关信息，当然要是有下论文的功能就更好了。这样可以得到自己所收藏论文的详细信息，日后整理起来很方便

对于NOTEEXPRESS的应用，我也只是用在了下载题录，建个人数据库管理论文和管理论文相应笔记这三个方面，感觉操作还是很简单，而且关键是有中文版存在，所以说详细的教程对于基本功能是没有太大必要了。




至于一些更为高级的功能，目前还没用到，随用随学吧.....










SCOPUS文献导入endnote的方法

在SCOPUS网站上输出选择的文献到文献管理软件时，有个output选项：

请不要选择complete format

选择complete format就会导致无法正确将文献导入到endnote,因为在scopus里有个

references选项,而endnote里没有对应的项,就会导致导出有错误.

所以大家在从网站输出数据时,output的地方可以选择:

specify fields to be exported

在选择下面需要输出的内容时只要不选择references就可以了，保存为ris文件。

在endnote里import,

选择原文件

import option：reference manager (RIS)

另两个用默认设置即可。

enjoy it！



engnote公司的回复：

Hello,

You can directly export data from Scopus into EndNote as follows:

1. From the Results screen, click on Export.
2. On the Export screen:
a) In the Export Format box, select RIS Format.
b) In the Content Format box, select either 揷itations only?or more detailed records.
3. Select your library, and the imported references will display on the screen.

For your reference, these instructions are also available on this page of our website:

[](http://www.endnote.com/endnote_export.asp#tc_qz_original=505452917)[http://www.endnote.com/endnote_export.asp](http://www.endnote.com/endnote_export.asp)

Please contact us if you have further questions.

Thanks for contacting ResearchSoft Customer Technical Support. Please send any replies to [isirs@contactual.com](mailto:isirs@contactual.com#tc_qz_original=505452917), and do not remove the case number (e.g. ~5500) from the Subject line.

We value your input and welcome your comments. For a chance to win a $50 American Express Gift Cheque please complete our brief customer satisfaction survey at [](http://www.zoomerang.com/survey.zgi?p=WEB225LEL5KV6M#tc_qz_original=505452917)[http://www.zoomerang.com/survey.zgi?p=WEB225LEL5KV6M](http://www.zoomerang.com/survey.zgi?p=WEB225LEL5KV6M)

Kayte Stasny
Customer Technical Support, Thomson Scientific ResearchSoft
(408) 987-5609, Mon-Fri, 6am-5pm Pacific time   
[](http://www.researchsoft.com/contact#tc_qz_original=505452917)[http://www.researchsoft.com/contact](http://www.researchsoft.com/contact)

Thomson Scientific ResearchSoft & its product names & acronyms used herein are trademarks, service marks, & registered trademarks used under license. The information contained in this email message & any files transmitted with it are confidential & intended solely for the use by the individual(s) originally addressed. Please disregard & delete this email if you are not the intended recipient.

[ENDNOTE使用方法～～～造福硕硕博博们.doc](http://blog.sciencenet.cn/home.php?mod=attachment&id=52349)


[Endnote教程(基础篇)ENDNOTE_X6.ppt](http://blog.sciencenet.cn/home.php?mod=attachment&id=52350)


[EndNoteX6简体中文版使用教程.pdf](http://blog.sciencenet.cn/home.php?mod=attachment&id=52351)


[Endnote_中科院经典教程.ppt](http://blog.sciencenet.cn/home.php?mod=attachment&id=52352)


[endnote_X6_中高级教程.rar](http://blog.sciencenet.cn/home.php?mod=attachment&id=52379)



转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-783442.html](http://blog.sciencenet.cn/blog-81613-783442.html)

上一篇：[中外数学家传记](blog-81613-783037.html)
下一篇：[八大经典数学难题](blog-81613-789176.html)


