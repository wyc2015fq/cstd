# win7 开机加速系列-下篇 - 三少GG - CSDN博客
2013年01月21日 14:23:51[三少GG](https://me.csdn.net/scut1135)阅读数：1309
个人分类：[杂记/八卦](https://blog.csdn.net/scut1135/article/category/684984)

[Windows7开机加速全攻略六：注册表清理](http://www.win7china.com/html/5399.html)
**六、开机优化提速之注册表清理**
为什么Windows系统，包括 [Windows7](http://www.win7china.com/)、[Vista](http://www.vista123.com/)、XP等系统开机的时候，硬盘灯狂闪呢？
这个问题，不该问或许，毕竟，需要读硬盘上的数据才能进行加载系统啊，尤其是对注册表的读和写。是的，在Windows这个庞大的系统里面，无论是开机还是平时运行Windows，注册表的读写可以说是最频繁的。哪怕你只活动一下鼠标，Windows就会对注册表读取n次，注册表毕竟是整个Windows系统的核心信息所在地。
新装完的系统，注册表文件文件就40M前后，可是等你安装了许多软件之后，你会发现，系统启动速度也慢了，平时运行软件什么的也慢了。不仅仅Windows系统本身，那些Office、Visual Studio、SQL Server等大型软件，更是往注册表里面写入了几千甚至几万行的数据。你浏览网站输入过的网址记录、Windows打开过的文件、右下角图标的记录、最近运行的软件记录、Winrar等等软件的设置和运行记录，一股脑的都往注册表里面塞，你说，这个注册表能不大么？
注册表大了，系统要不停读写，系统开机的时候和运行的时候，能不变慢么？
怎么办？
清理！下面，大家和[软媒](http://www.ruanmei.com/)的[Win7之家](http://www.win7china.com/)团队一起，研究下如何搞定注册表！
什么，你害怕系统坏掉？害怕某些软件不能正常运行？
别怕，[Windows7优化大师](http://www.win7china.com/windows7master/)、[魔方](http://mofang.ithome.com/)的注册表清理等功能，提供了注册表完全备份、本次清理内容备份两大备份功能，确保你的清理没有任何后遗症。当然，我们建议每次只备份清理内容即可，闪电备份，闪电还原，还不容易出错！如果你不备份就进行清理，只能打心底的佩服你，同时默默的为您祈祷祝福。毕竟，每个用户安装的软件都不同，清理功能只能把最常用软件和系统本身列入白名单。
在进行实战之前，特别强调一下，进行系统注册表的扫描、清理的时候，尽量不要同时进行其他软件的操作，跑到一边喝茶看书听歌是最好的选择，因为系统进行各项操作时都会一直在不停的读写注册表，保持注册表操作时候的“不受”打扰，请务必注意！
**另外，作为新一代优化大师，目前**[**魔方**](http://mofang.ithome.com/)**的注册表清理和垃圾文件清理的速度比**[**Windows7优化大师**](http://www.win7china.com/windows7master/)**快几十倍！但是，全新的Win7优化大师2010年5-6月会面世，比魔方还要快！！**
**1、用Win7系统清理大师来清理注册表**
运行[Win7优化大师](http://www.win7china.com/windows7master/)，点击上方的“系统清理”，打开系统清理大师，点击左侧的“注册表清理”。OK，一般就直接点击“扫描注册表”就可以直接开始了，扫描过程中，您可以暂停扫描，也可以直接取消掉本次扫描，取消的时候会把取消前的扫描结果都罗列出来的：
![](http://img.win7china.com/NewsUploadFiles/20100327_134913_46_u.jpg)
当然，如果您是个老手，你完全可以点击左下角的选项，这儿方便您自定义扫描位置和内容，非常的简单：
![](http://img.win7china.com/NewsUploadFiles/20100327_135121_62_u.gif)
当然，您还可以设定您要跳过的键值，还可以自行添加：
![](http://img.win7china.com/NewsUploadFiles/20100327_135215_437_u.gif)
是不是很人性化和强大？是的，[Windows7优化大师](http://www.win7china.com/windows7master/)的注册表清理功能是目前国内外已知软件里面扫描数量上是最强的（比[魔方](http://mofang.ithome.com/)还要强），这点请大家实际评测，不想耍嘴皮来吹嘘什么。
好了，扫描完了，我们下一步要做什么？
对，清理！那要注意什么呢？
全选或者选择您要清理的项目，点击清理吧。[Windows7优化大师](http://www.win7china.com/windows7master/)默认情况下会自动备份您的清理项目，放心，并不会占用太多磁盘空间，只是对您清理的地方做自动备份！
选项中也可以更改这个，如果您实在实在是高手，不愿意占用几百K的影片空间——
![](http://img.win7china.com/NewsUploadFiles/20100327_135448_703_u.gif)
如果您在选项里面关闭了自动点击一下窗体上方的“备份选中项目”，那么，你也可以手动备份。当然，不建议您备份整个注册表，一是速度慢，而是系统运行状态下，备份还原整个注册表需要很久，而且很容易还原失败。记住，只备份本次要清理的选中项目即可。记住，清理之前一定要备份本次清理的内容，就多花10秒钟的时间！清理注册表项目前先把你选中的条目备份一下。[Windows7优化大师](http://www.win7china.com/windows7master/)中的系统清理大师会自动帮你保存成
 .reg 文件。一旦发现清理失败，可以“闪电”恢复到清理前的状态。大家可以双击当初备份生成的 .reg 文件进行直接导入，也可以点击右上角的导入、还原注册表按钮进行闪电恢复。更高效、更省时、更放心的进行注册表清理。
使用[Windows7优化大师](http://www.win7china.com/windows7master/)进行清理的时候，建议您在没有安装其他大型软件的时候进行，这样更安全些，当你扫描出来几万条数据的时候，你不要害怕喔！备份选中的这些项目，清理！……整个世界清净了~
**2、使用[Windows7优化大师](http://www.win7china.com/windows7master/)来压缩注册表**
清理掉了数据，注册表体积就会自动变小么？
未必！
还要进行注册表的碎片整理，就是俗称的压缩！这一点，了解数据库的朋友们应该都懂。初级用户只要知道必须要整理下碎片，清除那些无用的空白处就可以了。
从安全角度考虑，[Win7优化大师](http://www.win7china.com/windows7master/)取消了这个注册表碎片整理功能，原因是64位系统下会出现故障。如果您是64位[Win7](http://www.win7china.com/)系统，暂时不建议您进行压缩注册表。
注册表垃圾少了很多，我们的开机时间又缩短了，这月的电费又少了几分几毛钱，聚少成多啊同志们，再说，时间和效率，不是无价的么？
写到这儿，注册表的优化清理工作就告一段落了，打字打的手痛，一直在计算着，如果这次是投稿到计算机类的杂志，稿费起码要有个……算了吧，大家还是到处多说说[Windows7优化大师](http://www.win7china.com/windows7master/)和[魔方](http://mofang.ithome.com/)的好话吧，这比什么都重要，先行谢过了
 ：）
稍事休息，继续我们的下一篇章，Win7系统开机提速攻略之——开机加载软件管理！
[Windows7开机加速全攻略七：开机加载软件管理](http://www.win7china.com/html/5400.html)
**七、Win7开机加载项管理**
　　前面的几篇实战攻略，讲的基本上都是进入Windows登录窗口之前的优化和加速，当然，注册表清理和压缩对进入系统桌面也是有加速作用的，OK，下面开始讲从输入密码（或指纹）开始进入Windows桌面时候的一些优化，还是那句话，要想优化，必须知道这段时间系统到底在干了什么。
　　简单的说，在从登录窗口到完全进入Windows桌面（你的鼠标指针不在圆圈状旋转的时候），系统主要干的工作就是从注册表里面加载Windows全局的配置还有你这个登录用户的“个性”配置，从这些配置文件中，按部就班的依次工作，例如加载你的系统主题、加载你的桌面壁纸、播放你指定或者默认的登陆声音、加载开机启动项目里面的各种软件。
　　在这篇文章里面，我们就直接拿出最该让人动手核查和管理的软件开机加载项目，从这儿先开刀，不仅仅这儿能显著提升你的系统开机速度，更关键的是，还能揪出一些系统隐患，例如一些木马和病毒都是在这儿作为常驻你内存的入口的。
　　当然，这篇文章非常的难写，难写之处在于，不同的用户用的不同的硬件，用的不同的电脑品牌或者diy自装机，这样，就导致了管理这些开机启动加载的软件时候的分辨难度，因为在这些加载项目里面，很多是和硬件的驱动有关系的。
　　好了，下面就先谈下如何清理这些开机启动加载的软件。
**1、软件如何让Windows系统列入启动加载的列表中**
　　正规的软件，你能很容易的发现它的所在，而那些狡猾或不怀好意的软件就会东躲西藏，让还不十分熟悉电脑的人难以找到。下面就把各种随windows运行而启动的程序可能躲藏的地方，一一给大家指出来：
　　1）经典的启动——“启动”文件夹 
　　点击“开始菜单”-“所有程序”，是不是看到了这个“启动”文件夹？，这就是最经典的Windows启动位置，右击“启动”菜单选择“打开”即可将其打开，其中的程序和快捷方式都会在系统启动时自动运行。 把想要加载的东西的快捷方式（文本文件的快捷方式等等也行）拖到开始菜单按钮，再拖到所有程序上面的这个启动文件夹里面，OK，下次开机启动就列入这个项目了。
　　2）有名的启动——注册表启动项 
　　注册表是启动程序藏身之处最多的地方，主要有以下几项： 
　　A、Run键 
　　Run键是病毒最青睐的自启动之所，该键位置是[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run]和[HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run]，其下的所有程序在每次启动登录时都会按顺序自动执行。 
　　还有一个不被注意的Run键，位于注册表[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\Run]和[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\Explorer\Run]，也要仔细查看。 
　　B、RunOnce键 
　　RunOnce位于[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunOnce]和[HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunOnce]键，与Run不同的是，RunOnce下的程序仅会被自动执行一次。 
　　C、RunServicesOnce键 
　　RunServicesOnce键位于[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunServicesOnce]和[HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunServicesOnce]下，其中的程序会在系统加载时自动启动执行一次 
　　D、RunServices键 
　　RunServices继RunServicesOnce之后启动的程序，位于注册表[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunServices]和[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\RunServices]键。 
　　E、RunOnceEx键 
　　该键是Windows XP/2003及其之后的Vista等系统特有的自启动注册表项，位于[HKEY_CURRENT_USER\\SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnceEx]和[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnceEx]。 
　　F、Load键 
　　[HKEY_CURRENT_USER\Software\Microsoft\WindowsNT\CurrentVersion\Windows]下的load键值的程序也可以自启动。 
　　G、Winlogon键 
　　该键位于位于注册表[HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon]和[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon]，注意下面的Notify、Userinit、Shell键值也会有自启动的程序，而且其键值可以用逗号分隔，从而实现登录的时候启动多个程序。 
　　H、其他注册表位置 
　　还有一些其他键值，经常会有一些程序在这里自动运行，如： 
　　[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System\Shell][HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\ShellServiceObjectDelayLoad][HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System\Scripts][HKEY_LOCAL_MACHINE\Software\Policies\Microsoft\Windows\System\Scripts] 
　　小提示： 
　　注册表的[HKEY_LOCAL_MACHINE]和[HKEY_CURRENT_USER]键的区别：前者对所有用户有效，后者只对当前用户有效。
　　想研究的朋友们，推荐大家看下《[牛！揪出病毒木马流氓软件藏身注册表的7个窝点](http://www.win7china.com/html/5392.html)》。
　　3）古老的启动——自动批处理文件 
　　从DOS时代过来的朋友肯定知道autoexec.bat(位于系统盘根目录)这个自动批处理文件，它会在电脑启动时自动运行，早期许多病毒就看中了它，使用deltree、format等危险命令来破坏硬盘数据。如“C盘杀手”就是用一句“deltree /y c:\*.*”命令，让电脑一启动就自动删除C盘所有文件，害人无数。 
　　小提示 
　　★在Windows 98中，Autoexec.bat还有一个哥们——Winstart.bat文件，winstart.bat位于Windows文件夹，也会在启动时自动执行。 
　　★在Windows Me/2000/XP/Vista和[Windows7](http://www.win7china.com/)中，上述两个批处理文件默认都不会被执行。 
　　4）常用的启动——系统配置文件 
　　在Windows的配置文件(包括Win.ini、System.ini和wininit.ini文件)也会加载一些自动运行的程序。 
　　A、Win.ini文件 
　　使用“记事本”打开Win.ini文件，在[windows]段下的“Run=”和“LOAD=”语句后面就可以直接加可执行程序，只要程序名称及路径写在“＝”后面即可。 
　　小提示 
　　“load=”后面的程序在自启动后最小化运行，而“run=”后程序则会正常运行。 
　　B、System.ini文件 
　　使用“记事本”打开System.ini文件，找到[boot]段下“shell=”语句，该语句默认为“shell=Explorer.exe”，启动的时候运行Windows外壳程序explorer.exe。病毒可不客气，如“妖之吻”病毒干脆把它改成“shell=c:\yzw.exe”，如果你强行删除“妖之吻”病毒程序yzw.exe，Windows就会提示报错，让你重装Windows，吓人不？也有客气一点的病毒，如将该句变成“shell=Explorer.exe 其他程序名”，看到这样的情况，后面的其他程序名一定是病毒程序如图2所示。 
　　C、wininit.ini 
　　wininit.ini文件是很容易被许多电脑用户忽视的系统配置文件，因为该文件在Windows启动时自动执后会被自动删除，这就是说该文件中的命令只会自动执行一次。该配置文件主要由软件的安装程序生成，对那些在Windows图形界面启动后就不能进行删除、更新和重命名的文件进行操作。若其被病毒写上危险命令，那么后果与“C盘杀手”无异。 
　　小提示 
　　★如果不知道它们存放的位置，按F3键打开“搜索”对话框进行搜索； 
　　★单击“开始→运行”，输入sysedit回车，打开“系统配置编辑程序”，在这里也可以方便的对上述文件进行查看与修改。 
　　5）智能的启动——开/关机/登录/注销脚本 
　　在Windows 2000/XP/[Vista](http://www.vista123.com/)/[Windows7](http://www.win7china.com/)中，单击“开始→运行”（[Windows7](http://www.win7china.com/)系统中可以在开始菜单的搜索框中输入gpedit.msc），输入gpedit.msc回车可以打开“组策略编辑器”，在左侧窗格展开“本地计算机策略→用户配置→管理模板→系统→登录”，然后在右窗格中双击“在用户登录时运行这些程序”，单击“显示”按钮，在“登录时运行的项目”下就显示了自启动的程序。
![](http://img.win7china.com/NewsUploadFiles/20100327_141419_890_u.gif)
　　6）定时的启动——任务计划 
　　在默认情况下，“任务计划”程序随Windows一起启动并在后台运行。如果把某个程序添加到计划任务文件夹，并将计划任务设置为“系统启动时”或“登录时”，这样也可以实现程序自启动。通过“计划任务”加载的程序一般会在任务栏系统托盘区里有它们的图标。大家也可以双击“控制面板”中的“计划任务”图标查看其中的项目。
**2、揪出并清理开机启动时加载的项目**
　　上面的这些介绍，是参考的网上资料所得，目的是让让大家基本了解下开机自动加载的软件的“藏身之处”，实际上，现在的软件已经能自动从这些位置获取这些项目，从而方便大家进行管理。
　　1）[Win7](http://www.win7china.com/)系统自带的“系统配置”程序——“msconfig.exe”
　　在开始菜单中输入msconfig.exe 或者Win键+R键打开运行窗口输入msconfig.exe，运行即可。运行后点击“启用”，即可进行开机加载项管理。
![](http://img.win7china.com/NewsUploadFiles/20100327_141539_609_u.gif)
　　注意，msconfig 这个系统配置程序不能读出计划任务中的开机启动项目。
　　2）使用[Windows7优化大师](http://www.win7china.com/windows7master/)进行管理开机加载项目
　　打开[Win7优化大师](http://www.win7china.com/windows7master/)点击上面的“系统设置”，再点击左侧的“启动设置”——
![](http://img.win7china.com/NewsUploadFiles/20100327_141807_937_u.jpg)
　　其他软件基本只能勾选和反勾选，会把一些没用的选项都罗列出来，[Windows7优化大师](http://www.win7china.com/windows7master/)则可以彻底的删除这些项，省的“碍眼”。当然也可以不彻底删除他们，只要点击右下角的保存设置即可，那些被禁止开机启动的项目依然会显示在列表中供你以后再恢复开机启动他们。
[Windows7优化大师](http://www.win7china.com/windows7master/)目前也是没有加入对计划任务中开机启动项的支持，这个会在以后的版本中加以加强和提供。
**3、到底那些开机启动项目可以禁止掉？**
这个问题就是最复杂的地方所在，上面说过，不同的电脑安装了不同的驱动可能需要开机加载，不同的电脑里面安装了不同的应用软件，很难分辨！
但是，也是有一些基本原则或者大家习惯的：
1）声卡、显卡相关的驱动开机加载程序，一般可以直接禁止
2）QQ、MSN等IM软件和[杀毒软件](http://www.souxia.com/search.aspx?site=1&wd=%C9%B1%B6%BE%C8%ED%BC%FE)、系统防火墙一般都会允许开机自动加载（当然要看个人习惯了）
3）涉及[输入法](http://www.vista123.com/sogoupinyin/)类的加载项可以禁止，例如Microsoft Pinyin 等
4）播放器类的加载项可以禁止
5）Google Toolbar等IE[浏览器](http://www.saayaa.com/)插件类的开机启动项目可以直接禁止掉
6）[Windows7](http://www.win7china.com/)系统桌面的小工具，这个要看个人喜好了
7）未知的程序，请大家自己在[搜索引擎](http://www.souxia.com/)里面搜索一下，然后决定，基本搜索的目的就是看看是不是恶意程序，基本一搜就可以清楚的。
除此之外，就没别的更傻瓜型的了么？
Oh，大家忘了一直在兢兢业业努力工作的[软媒](http://www.ruanmei.com/)技术和编辑团队了吧，[Windows7优化大师](http://www.win7china.com/windows7master/)正在抓紧研究这方面的工作，力争以后会对这些加载项目给出直接的判断和建议，当然，这一切需要大量的用户反馈和资料收集，需要逐步的积累，但是既然想到了，我们肯定会努力去做的。
软件存在的目的，不就是为了提高大家的效率，让大家更省心么？
今天码字实在太多，手腕狂痛，要是还有谁在今后说[Windows7优化大师](http://www.win7china.com/windows7master/)和[Win7之家](http://www.win7china.com/)坏话的，大家帮我们批评一下他们，今天那些人再来捣乱就太不人道了
 ：）
OK，这个开机启动加速攻略马上就快结束了，这样，争取在下一篇里面都解决掉吧。继续努力下一篇——开机启动优化之效果换性能。
[Windows7开机加速全攻略八：特效换性能](http://www.win7china.com/html/5401.html)
**八、牺牲Win7系统特效，换取速度最佳性能**
终于到了终篇，今天重新编排的时候，想想当初码了数万计的字，着实……￥@%@&……，但一想到这次最少能帮助到几十万人甚至[Windows7优化大师](http://www.win7china.com/windows7master/)和[Vista优化大师](http://www.vista123.com/vistamaster/)的2000多万的下载用户，咬牙，继续！
前面有读者朋友们建议老板给我加薪，我着实感动的很，是啊，拼了今天，要是没啥回报，那可真。。。但是谁给我涨工资呢？我。。。。上头没有比我再大的老板了啊。。。刺客的命难道就那么的苦？总之这一系列文章，[刺客](http://www.cike.cn/)就代表[软媒公司](http://www.ruanmei.com/)的所有员工当做答谢朋友们长期以来对我们工作的支持吧。反正我每天也不务正业，就是上网溜达看新闻……
前面几篇文章里面，要提到的几个影响开机速度的大项都已经讲的差不多了，作为收尾之篇，这儿主要还是为了本文的全面性和系统性，尽可能的把影响开机启动的要素都提及一下。本文里面主要谈及牺牲下系统的美观，换取更快的开机加载速度，当然，对于很多新电脑用户朋友来说，就忽略吧，毕竟新电脑的性能都很强劲的。
**1、Win7系统主题**
[Windows7](http://www.win7china.com/)默认的Aero主题大家知道是很耗费内存的，在从登录桌面到完全进入Windows桌面的时候，用默认主题和用Windows经典主题，所耗费的时间差别是显而易见的。当然，美观和系统进入桌面的速度，你也是要做出选择。现在的电脑配置很高，还是爱美的人多，这个就别折腾了。
设置方法：[Win7](http://www.win7china.com/)桌面空白处鼠标右键，点击“个性化”，往下拉一下找到“基本和高对比度主题”，选择基本或是经典即可，当然，经典的最省资源：
![](http://img.win7china.com/NewsUploadFiles/20100327_143233_375_u.gif)
大家可以实际测试一下，其实也不难理解，使用经典主题后，系统从登陆画面到Windows桌面加载的时间会少的很多。日常操作Windows系统的时候，速度也非常的明显。
**2、设置纯色背景代替壁纸**
为了节能，为了清爽，你大可以把漂亮的壁纸换成纯色背景，现在都是较大的屏幕，一个壁纸动辄1m2m的，当然加载的时候也会省时间，不过，我个人可是宁可要漂亮的桌面壁纸。
纯色背景大家总会设置吧，还是桌面空白处右键点击个性化，然后点击桌面背景，下拉列表里面选择纯色即可，挑选一下自己中意的颜色就行了。
![](http://img.win7china.com/NewsUploadFiles/20100327_143341_265_u.gif)
**3、设置最佳系统性能，提速Win7启动和应用**
在Windows7系统里面同时按下Win键+Pause键，嗯，是不是出来了系统属性窗口，嘿嘿，这个快捷键好多人还不知道貌似——
出来的系统属性窗口中，点击左侧的“高级系统性置”——
再点击下“高级”选项卡——点击“高级标签”：
![](http://img.win7china.com/NewsUploadFiles/20100327_143705_687_u.gif)
点击这个“设置”按钮进去吧——
![](http://img.win7china.com/NewsUploadFiles/20100327_143754_171_u.gif)
视觉效果里面设置为最佳性能，可以理解吧？这样同样可以进行系统的减负和加速。当然，牺牲了特效。**还是那句话，我个人不会这么干**。但是，天下很多用低配置电脑跑[Windows7](http://www.win7china.com/)的朋友们啊。
好了，以上的部分就是牺牲一些特效来换取系统加载速度的部分了。
到此，本文就差不多该结束了。但是当我看到这上面的图片时，突然看到了虚拟内存的选项，便想到，这个真的差点让我忽略。好吧，就放到下面的尾声里面提及吧。
**九、查漏补缺的尾声**
**1、第一个漏：减压硬盘，提速开机**
是的，如果您的内存在2g以上，确实想建议您关掉您的虚拟内存，毕竟，目前电脑里面所有影响电脑性能的部件里面，硬盘是最大最大的瓶颈了。更何况，很多本本都是用的5400转速的硬盘。多加些内存，把虚拟内存也就是交换文件给禁掉，提升的效率还是很明显的。
用上面说的方法刚才的方法打开系统高级选项，这次不改视觉效果，点下高级标签：
![](http://img.win7china.com/NewsUploadFiles/20100327_144042_406_u.gif)
**2、第二个漏：讯盘**
讯盘是什么？相信大部分朋友都知道的，如果不清楚的搜索一下吧，配备1-2g的讯盘的笔记本电脑，启动速度是绝对的明显加快的。这个大家自己查下资料吧，因为这个牵扯到硬件配备范畴，属于经济能力范畴，而完全属于软件设置等技术类，在此就这么简单一带而过吧。毕竟提到的越多，越显得[刺客](http://www.cike.cn/)的知识面广博一些（说到此处脸还真的没红）。
**3、尾声**
是的，写完了。
肯定有些地方被忽略了，或者在本文中还存在一些值得商榷的地方，但这都没关系，我们可以把这篇文章随时的及时修正到新的版本，不是么？以后再发现漏掉什么，就再做修补吧。
本文的目的，在于让大家一起探讨下长期以来困扰很多人的系统开机速度问题，不求对每个人都有用，只求有收获的朋友们心情高兴就可以了。我们坚信，完整看过本文的人会有些收获的。
对了，大家动手实战的时候，切莫忘记用秒表计时，看看到底前后效果如何的！
还是那句话，对于高性能电脑的朋友，其实啥也不做都可以，但是，我们不能拿自己对电脑知识的兴趣和自己的硬件配置去衡量天下所有人。
最后，刺客水平实在有限，工作时间也确实紧张，久未动笔，如有不妥，诸君莫怪。
——《全剧终》——
