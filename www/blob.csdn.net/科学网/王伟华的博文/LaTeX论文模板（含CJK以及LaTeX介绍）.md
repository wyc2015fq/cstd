# 科学网—LaTeX论文模板（含CJK以及LaTeX介绍） - 王伟华的博文




# LaTeX论文模板（含CJK以及LaTeX介绍）                           

已有 19794 次阅读2013-1-18 00:16|个人分类:[科技论文写作、搜索经验集锦](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=104604&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|关键词:LaTeX模板|[LaTeX模板](misc.php?mod=tag&id=175726)






  LaTeX论文模板的一些小小的总结（含如何生成可以搜索复制有书签的PDF）。这里给出三个小例子，前两个CJK GBK编码，第二个在latex+latex+dvipdf 时可以少一个gbk2uni 的过程；第三个是CJKutf8 UTF8编码；第三种是最简单的方式。然后再对CJK宏包环境做一些详细的说明。最后给出一个完整的使用CJK宏包的源文件。

一

% GBK 编码

% run1: latex+ latex+ gbk2uni+ latex+ dvipdf

% run2: pdflatex+gbk2uni+pdflatex

documentclass{article}

usepackage{CJK}

usepackage{ifpdf}

ifpdf

usepackage{ccmap}%使pdflatex 可以搜索

fi

usepackage[driverfallback=dvipdfmx,CJKbookmarks]{hyperref}

 begin{document}

 begin{CJK*}{GBK}{song}

section{测试}

基于CJK的可以搜索、复制、有书签的中文PDF。

 end{CJK*}

 end{document}
二

% GBK编码，第一种方式不需要用gbk2uni

% run1: latex+ latex+ dvipdf

% run2: pdflatex+gbk2uni+pdflatex

documentclass{article}

usepackage{CJK}

usepackage{ifpdf}

ifpdf

usepackage{ccmap}

else

AtBeginDvi{special{pdf:tounicode GBK-EUC-UCS2}}%将GBK转换为unicode

fi

usepackage[driverfallback=dvipdfmx,CJKbookmarks]{hyperref}

 begin{document}

 begin{CJK*}{GBK}{song}

section{测试}

基于CJK的可以搜索、复制、有书签的中文PDF。

 end{CJK*}

 end{document}
三

% !Mode:: "TeX:UTF-8"%UTF8编码

% run1: latex + latex + dvipdf

% run2: pdflatex+pdflatex

documentclass[12pt,a4paper]{article}

usepackage{CJKutf8}

usepackage[driverfallback=dvipdfmx,unicode]{hyperref}%不能用CJKbookmarks

 begin{document}

 begin{CJK*}{UTF8}{song}

section{测试}

基于CJK的可以搜索、复制、有书签的中文PDF。

 end{CJK*}

 end{document}




说明：1. 所有中文字符都要包含在环境

begin{CJK} [fontenoding]{encoding}{family}

......

end{CJK}[fontenoding]{encoding}{family}

或


begin{CJK*}[fontenoding]{encoding}{family}

......

end{CJK*}[fontenoding]{encoding}{family}

中。这两个环境不仅可以用在源文件的主体部分也可以用在源文件的导言部分。

2. 一个完整的使用CJK宏包的源文件大致是如下的形式：

doucumentclass{article}

usepackage{CJK}


begin{CJK} [fontenoding]{encoding}{family}

......                                     %包含中文信息的定义和设置

end{CJK}[fontenoding]{encoding}{family}

 begin{document}


begin{CJK} [fontenoding]{encoding}{family}

......                                    %包含中文字体的文件的主要内容。

end{CJK}[fontenoding]{encoding}{family}

 end{documnet}



   更详细地用法请查阅各种LaTeX的网站或书籍，比如我比较喜欢的一本《LATEX2e及常用宏包使用指南》（李平编著，清华版），网上也可以找到该书的电子版，我在这里就不重复上传了。



下面先给一个超链接： [TeX/LaTeX 常用宏包简介](http://blog.sina.com.cn/s/blog_9d88a57701015gel.html)
再来一个转载：

**论文编译程式Latex 介绍**


文章来源: [http://hi.baidu.com/leibniz%5Fgone/blog/item/be320b2460b8c233c89559c4.html](http://hi.baidu.com/leibniz%5Fgone/blog/item/be320b2460b8c233c89559c4.html)
（本文为自由发布的文档，你可以对其进行任何的拷贝，传播，修改。但原始文档版权仍归原作者所有。如有直接引用的情况，请注明文档出处和作者姓名）

（本篇介绍在写作过程中参照了多篇网络上的教程及帮助文档，在此对那些作者表示感谢，感谢他们为我们做了绝大部分前期的文档工作，由于太多，此处不一一列举；另外也感谢王垠，是他带给我去坚定地使用Linux和Latex的意志――这意志帮助我克服了学习过程中的种种困难；最后感谢图灵奖得主Knuth教授，感谢他做出伟大的TeX出来，供后人使用。）


第一章：为什么用Linux

Latex是什么，说它是一个“排版软件”，可能大家都会莫名其妙，我要一排版软件来干什么？但是如果我说它是一个“写论文用的软件”，那你可能就会很有兴趣了。毕竟作为大学生，不可能不写论文的。
 其实Latex不仅可以写论文，还可以处理日常生活中的各种文档工作。如写信，写书，画表格，甚至做幻灯片（对，我没打错，它就是可以做幻灯片，而且很容易就能做出世界一流水平的幻灯片）等等。

本文纯粹是从使用者的角度来讲解Latex的使用方法的（当然，这样说的原因在于我本身尚未深入地研究TeX的底层体系），因此力求讲得通俗易懂。如果有什么不妥或不完善的地方请大家能时指出，欢迎热烈批评！

为什么要用Latex？

你一定会说日常生活中的各种文档工作不是用WORD就能进行处理了吗，何必那么麻烦再去学习一样新的东西呢？不是的。世界上还有很多更优秀的东西我们还没有发现，如果能掌握这些更优秀的工具，那么工作效率会事半功倍。可以说当今中国的大学生们（包括中国的很多大学教授们）的计算机方面的思想已经被微软所固化了。“言文字处理必谈WORD”，“言程序开发必学VB， VC”，“言操作系统必选WINDOWS”。我这儿并不是想排斥微软，打倒微软什么的，（诚心地说它毕竟还为推动中国的信息化的进步做出了很大贡献）我只是想说明作为优秀大学的学生的我们，作为中国21世纪的主力军的我们，不能再被一种并不先进的思想所禁锢了！我们必须掌握最先进的最有前途的思想，知识和能力。而TEX就是这种先进思想的典型代表。
 它是计算机科学家图灵奖得主Knuth教授设计的一款权威的科技论文排版软件！更重要的它是开源 (Open Source)的，Knuth教授无偿公开了它的所有源代码。正因为这个原因，无数的爱好者们大胆尝试TeX。并在其基础上开发了一个宏集 ――Latex。Latex方便好用，被广泛传播，成了当今世界科技界最权威的论文排版软件。

下面，我就来详细地比较一下Latex与WORD各自的优缺点。
 1：“所想即所得” vs. “所见即所得”
 WORD所遵循的思想是“所见即所得”。因此，用它写东西非常直观。而且打印的效果与在屏幕上看到的效果一模一样。另外，它还具有一定的智能化（尽管这个功能很多时候会帮倒忙）。
 而Latex所遵循的思想是“所想即所得”。意思就是你脑子里想到什么，就可以做出什么样的版式效果来。这说明它有非常强大的功能，但对这一种强大的掌握有一条相对来说陡峭一点的学习曲线。
 用WORD写文章直接往里面写就行了，然后用那些数不清的格式按钮对文档进行格式编排。写这些文档必须在WORD软件环境里面（你无法拿记事本来写.doc的文档）。
 而用Latex写文章可以使用任何一个文本编辑器。Latex有它自己专用的文件后缀名，但它的格式完全是文本文件格式。这意味着你可以使用vim，gedit,记事本等中的任何一款写Latex文档。只不过在保存文件时把后缀名改.txt为.tex就可以了。
  用Latex写文章需要像写程序那样来写（请不要为我的语言所吓倒，那不是像C语言那样的程序代码，要简单得多），写完之后再编译。初看起来这不是自找麻烦吗？仅仅写一篇文章就要编个程，是不是有病了。不是的，你仔细想想：一篇论文不仅包括文字，还有图片，表格，公式（对我们来说很重要），还有很多各种各样的细微的格式和版式。把这些因素加在一起，就不是仅用WORD就能很好地处理的了。而对Latex来说，这些都是小菜一碟。
 Latex文档中，是通过插入一些标记符来标记其逻辑结构的（这方面它和HTML, XML有点类似）。这些标记符都是一些常用的英文单词，十分好记。只要花点时间用熟悉了，就可以很快地写出一篇格式漂亮的文章出来。
 万事开头难嘛，最难的不是记忆那些命令，而是转变一种思维，就像使用LINUX一样。（Latex和LINUX搭配是天籁之合，我的讲解也是以LINUX上的Latex为主。建议大家读读王垠的《完全用LINUX工作，摈弃WINDOWS》）

 2：如果你经常编排带数学公式的文章，你可能会发现，WORD编排出来的段落不是那么令人满意，如果左对齐，那么每行的右边参差不齐，很不美观；而如果选用两边对齐，那么WORD为了达到这点要求，可能就加大了某些词与词之间的间距，那样也达不到我们满意的文章格式。能不能在行末自动地把一个单词在音节处拆开， 并加上连字符呢？ Latex可以。
    你一定对MathType不陌生。公式编辑器用多了，你可能就发现，WORD把公式当作图片来处理。如果你要修改文章字体的大小，这时需要一个一个地调整公式的大小，这简直是无法承受的工作量；而且，你经常会发现，WORD的公式大小并不是和正文那么适配，许多时候看起来要么大了，要么小了，尤其是碰到大行的公式，页面编排都可能因此受到影响了。而LatexX美观的公式混排，统一的公式正文大小，会令你有耳目一新，相见恨晚的感觉。

 3：你是否有这样一种经验：当你用WORD写完一篇大一点的含有很多插图，表格和公式的论文交给导师审阅后，导师要求你在某个地方再详细一点，多列一些数据，多插几幅图片；而在另外一些地方简略一点。这时你不得不插入新的图片（表格，公式），结果一插，把后面的原先排好的版式全打乱了。这样你又不得不重新排一遍，排好后，检查时，又发现哪个地方公式不对，得重新写。于是用MathType写了一个“体积”大一点的公式，再插入文档中，结果又发现，后面的版式又莫名其妙地被改动了。又得重新来一遍，气死人了。就这样反反复复，一个本来不复杂的工作耗费了你太多的时间，同时把心情也搞孬(我们念pie,四声)了。真不爽！
 还有在论文中每个图（表，公式）都有编号，当你插入一个新的图时，就打乱了先前的编号顺序，所以你还得一个一个地将先前的位于这幅图片后的那些图片（表格，公式）的编号手工改正过来。天哪，这又是一件多么恐怖的事情――我这可是科技论文啊？密密麻麻的图片，公式。你叫我如何改啊！而且这还是一遍，说不定要使一篇论文最终定稿要改N次呢！
 现在你是否感觉到已经忍受不了呢？你需要一种解脱。不用怕，不用恼，救星到来啦！Latex，专门解决这个问题。
 Latex处理这个问题是基于如下一种机制：
  它完全用一种逻辑结构标记符来标记哪里是文章题目、作者，哪部分是摘要，哪里是小节标题，哪里是图片，哪里是表格，哪里是公式，哪里是脚注，哪里是页眉，哪里需要两栏排版，哪里则只用单栏等等等等。它对每种不同的对像都设计了一个计数器，用来记录这一类型的对象到现在是第几个。比如图片计数器，编译程序在编译时会按从前到后的顺序寻找论文中所有图片，并且找到一个便给它编一个号（从1号开始依次递增），一直到文章末尾。
 利用这种机制，即使在中途又插入了很多新的图片（表格，公式）的情况下，只要它们的逻辑顺序是对的（废话，逻辑顺序不对就是你写论文的水平不够了！），那么Latex就会重新从头到尾再编一次号，这样前述的那种令人头痛的问题就迎刃而解了。思想就是把无谓的重复劳动拿给机器处理，机器处理这些小东西来速度是不用担心的。于是，你被解放出来了！什么是工作效率，这就是！

 4：很重要的一个问题是：WORD不太稳定。当你编辑一份含大量公式的文档时，会发现WORD有时会crash，有时甚至会莫名其妙地消失。这时如果我们的工作还未保存下来的话，那就太不幸了，只有重新输入一遍。而且弄得自己人心惶惶的。
 Latex就不存在这个问题，它是一种纯文本格式，所占用的空间（硬盘，内存）极小。再加上极稳定的编辑器VIM，几乎不会出现crash的现象。

 5：从美学观点来看，我实在不敢敬同WORD中的公式佷漂亮这一说法。你去图书馆随便找一本比较有名气一点的外文杂志或外文著作。它们那里面的公式多么漂亮，整体布局多么赏心悦目。虽然我不敢说那些书全是用Latex编排的，但至少可以说明一点：WORD中的公式符号不好看。虽然我不敢说那些书全是用Latex编排的，但我敢说那些书里面大部分都是用Latex及类似的（基于TeX的）软件包编排的。不信，你去比较一下那些书里面的符号和Latex教材（外面书店有卖的）里面的符号形状是不是一样的。有点使用Latex经验的人一眼就可辨别其中奥秘。
 这是什么意思？
 对！它意味着，使用Latex，你能用很简单的方法排版出具有专业水准格式的论文和杂质甚至书籍出来！

 6：比较现实的是，现在很多国内外杂志，要求寄去的文章按Latex格式排版(可能有些杂志可以接收WORD文档，那你可能又要在茫茫软件海洋中寻找英文WORD，因为国外根本就没有中文WORD，而你的中文WORD文档在英文WORD中什么都看不出来)。那么会用Latex就更是你迫在眉睫的需要。

    Latex就和vi一样，在你对他一点不懂的时候，你总觉得他很难相处，但是在你了解他之后，你会越来越发现他的好。然后会爱不释手，然后会五体投地，然后会――进步:-)



Latex讲解第二章：Latex发行版的版本和使用基本方法

Latex软件的版本
 Linux和Windows平台都有各自的Latex版本。Linux下的版本主要是teTex，Windows下的版本主要有mikTex。而中国有一个比较著名的版本CTeX，它是CTeX中文套装的简称，属于二次开发版本。它是把 mikTex和一些常用的相关工具，如GSview，winEdt，yap等包含在一起制作的一个简易安装程序。并且它对其中的中文支持进行了配置，使得安装后马上就能处理中文。
 GSview是一个浏览.ps（一种打印格式）的工具。
 winEdt是一个专为TeX用户设计的编辑器。（但是用过它的人都会感觉到，这个编辑器打开速度太慢，功能难用，字体，界面也不好看。难怪经常被网上的人骂为垃圾）
 yap是用查看.dvi格式的一个工具，支持“反向位置查找”功能。（这个用起来还不错）

 而Linux下的teTeX，我并没有直接使用。我用的是由CTAN组织制作的一款软件包TeXLive2004（以TE为核心），里面包含了几乎所有的宏包，文档及查看、转换工具。我们常用到的主要有如下几个：
 latex 用于编译.TeX文档（这个是核心）
 xdvi  用于查看.dvi文档（这个文档是由前面那个.TeX文档经编译后生成的）
 dvipdfm 用于将.dvi文件转换成.pdf文件。

 转换成.pdf文件后，就可以方便地与别人交流了。（要知道，pdf格式可是当今世界上的几乎所有较正式的组织的电子档案的首选保存格式呀。比如，AD公司（www.analog.com）的所有芯片的技术资料全是以pdf格式提供）

 而据我的使用经验，TeXLive2004安装程序并不太适合中国用户，因为它完全不含有任何一种中文字体（当然，每一款比较好的中文字体都是有版权的）,尽管网上有怎样添加的方法，但是要添加起字体来特别特别麻烦――我前段时间一直为此大伤脑筋呢！
  于是我将其中的绝大部分东西抽取出来，与我搜集的十三种中文字体（转换后的。注：那些字体那是有版权的，请不要用于商业目的）合起来做成了一个软件包。同时还对里面的一些配置文件作了修改，使Linux下的用户直接也能使用中文字体了。（我称其为Latex2004完全版，包里面有详细的安装说明文档）

 到现在，可能大家会有这样一个疑问：现在软件有了，那该怎样使用呢？而且前面介绍那么多工具。那么用LATEX写论文到底是一种什么流程呢？下面就来回答这个问题。
 下面就以一个实例来说明。
 第1步：编写好一个.tex文档。并保存。假设文件名取为example.tex，存放在~/work/目录下。
 第2步：打开一个终端。输入 cd work 进入到~/work/目录下。
 第3步：在终端中输入 latex example.tex 对example.tex这个文档进行编译。通过编译，会生成一个新文件：example.dvi（当然原来那个example.tex文件还在那里，没有消失）。
 .dvi文件是“与设备无关的文件”，这里的设备指CPU、显示器，打印机等。这意味着这种格式的文件无论在哪台机器上，无论是怎样的显示器，无论在哪个操作系统上，看到的效果都完全是一样的。
 当然，这样一种格式的文件得用专门的工具来查看。Linux下，这个工具就是xdvi。
  第4步：在终端中输入 xdvi example.dvi 就可以查看刚才由.tex文档生成的.dvi 文件了。在这一步中，你可以检查你所输入的那些东西是不是都出应有的效果了（当然，这个应有的效果就是你头脑中的预计――所想即所得，就初步体现在这里了），如果发现有错误的文字或不合适的版面，那么就可以再回到第1步去，修改后，再编译，再查看。直到自己满意为止。
 第5步：在终端中输入 dvipdfm example.dvi 就会将example.dvi文件转换成为 example.pdf 文件。这时就可以用pdf浏览器查看了。
 至此，一篇论文就基本完成了。归纳起来，就是如下步骤：
      latex  dvipdfm
 .tex------->.dvi------->.pdf

 下一章，我就将开始正式讲解怎样写latex文档。


Latex讲解第三章：Latex文档的基本结构及基本样式

首先建议大家如果真的有心学，最好是去书店里买本书仔细看看，因为细节知识是非常多的。我这里只能讲解一些概要，不可能面面俱到。

 Latex文档的基本结构如下：

 文档类型声明
 宏包包含区
 自定义命令区（全局设定区）
 标题区         
 正文区--------正文
             |
    -----参考文献

 将上述结构翻译成代码即为如下：

 %文档类型声明
 documentclass[a4paper,11pt,onecolumn]{article}

 %宏包包含区
 usepackage{CJK}

 %自定义命令区（全局设定区）
 renewcommand{baselinestretch}{1.5} %定义行间距

 %标题区
 title{  }
 author{ }

 %正文区
  begin{document}
 ....
  end{document}

 上述即为Latex文档的基本结构了。大家可能还不十分懂这些命令到底起什么作用，但是根据各个英文单词的含意，应该能够猜测到一些意思了吧。同时，这也展示了Latex一个特点：每个命令都使用完整的很容易懂的英文单词，而不是缩写，所以很明了，学习起来不难。

 下面我给出一个英文文档的基本例子。
 documentclass[a4paper,11pt]{article}
  begin{document}
 Hello, world!
 end{document}

 就这样四行简短的代码，会得到图1的效果。
 documentclass[a4paper,11pt]{article} 用来说明你这篇文章用多大的纸，默认多大的字号，并且文档类型是什么。documentclass 是命令，是每一篇文章开头必须写的。a4paper 指定现在用A4的纸；11pt 指定默认字体的大小；article 用来说明这篇文档是篇论文（当然计算机是不会认得你要写论文的，只不过它预先定义了一些格式，以article 称呼的这种格式适用于论文，所以就叫它为论文格式了）。
 字体大小那个选项一般不行去改动它，不写也行，系统默认为11pt。不写的话就为：documentclass[a4paper]{article}。写论文一般用A4的纸，当然，与这个选项对应的其它选项还有b5paper 等，可根据实际需要来改变。
 而article 这个位置一定不能空。与之对应的参数有 book(书), report(报告), letter(信件) 等。
  然后是begin{document}和end{document}，这也是两个必须写的命令，它们是一对语句，不能只写一个而不写另一个，即必须匹配。而且在一篇文档中只能写一次这对语句。从这对语句中又可反映出Latex的一个特点：很多语句是成对出现的，而且命令名都是begin{}和 end{}的形式。
 begin{document}和end{document}中间的那个 Hello, world! 就是文章的内容了。你在图1中看一下，是不是在左上角有个 Hello, world! :-)

 好了，就这样一个文档就写好了。简单吧。

 但──一个严重的问题是：这个文档只能处理英文。只要打一个汉字上去，在编译时就会出错！怎么办？
 下面我就来详细讲一讲 Latex 是如何处理中文的。

 Knuth 在发明 TeX 的时候，根本没有考虑到还要处理中文字符（以及其它许多亚洲字符）──它发明 TeX 的目的就是为了排版它的巨著《计算机算法艺术》。尽管这样，Knuth 却采用了一种先进的设计思想，从底层留下了扩展接口，并将其全面公开。这样，当时他本人没有实现的一些功能，就可以通过宏包的形式加以扩展实现。这就是为什么到现在为止 30多年过去了，TeX 在底层还几乎没有改动（只发现两处小错误）。说到这里，确实不得不佩服Knuth，什么叫牛人，这就是。计算机科学家写出的软件与一般程序员的软件就是不同！

 Latex中处理中文，需要用一个叫做 CJK 的宏包（宏包就是预先定义了一些命令及格式的一个文档，学过 C 语言的同学都应该了解的）。
    CJK是由Werner Lemberg开发的支持中、日、韩、英文字的宏包。CJK的特点是不需要象CCT那样预处理，支持PDFLatex和Type1字体，因此得到越来越多中国TeXer的喜爱，逐渐成为中文LaTeX的主流。而我提供的那个包里面就已经含有了。（可以毫不夸张地说，几乎所有已经出现的宏包在那个软件包里面都有自己的位置）
 这个宏包通过一个字符映射表，来实现汉字的处理。讲下去就比较深了，这里暂且说到这么多。

 下面我就给出一个处理中文的基本样例。

 documentclass[a4paper]{article}
 usepackage{CJK}
 begin{document}
 begin{CJK*}{GBK}{song}
 你好，世界！
 end{CJK*}
 end{document}

 生成的效果图如图2 。
 下面细细讲解一下新增的语句的作用。
 usepackage{CJK}表示把CJK这个宏包包含进来。usepackage{ }就是包含宏包的命令。
  begin{CJK*}{GBK}{song}和end{CJK*}是中文字符的定界语句。意思就是要想显示中文，就得用这两句把所有中文包含起来。当然，英文字符在这里面是不受影响的。begin{CJK*}{GBK}{song}中的最后一项，就是指宋体的意思。（我的安装包说明里面有十三种字体对应的符号名字。在Latex中使用时就用那些符号名字。）当然，你可以把它改为 kai(楷书), hei(黑体), xihei(细黑), li(隶书) 等等。

 比如我将song改为kai，那么将得到如图3的效果。

 看，用Latex写中文文档也不复杂吧。这个文档还太小，还体现不出Latex的优势，以后用熟了，写大文档了，就能深刻地体会到了。

 在这里，我要着重强调一下，Linux下的字符编码问题。
 现在的Linux基本上都通用UTF-8编码作为其默认编码，这是事实，我们最好不要去改变它，否则很多程序的字体会出理乱码。但Latex无法支持UTF-8编码（其实可以支持，但是好像要下一个宏包，在我提供的那个里面没有，我也没用过，不会用）。于是我们后退一步，绕过这个问题。在把文档写好后，只要保存时选定编码就行了。

 具体这样来实现：
 对gvim来说，把文档写好后。在命令模式下，输入
 set fileencoding=GB18030
 如果程序提示modifiable if off。那么，可以先输入
 set modifiable
 再输入
 set fileencoding=GB18030
 就可以保存为GB16030编码的文件了。这样在编译时，才不会出错。

 对gedit来说，把文档写好后，只需要点击保存按钮。就会出现一个保存对话框。如图4 。在此对话框的下面的字符编码中选择GB18030就可以了（没在列表中的话，可以添加）。

 由于保存时忘记理性编码而出现编译通不过的情况经常发生，而且还会感觉莫名奇妙。所以大家在发现编译通不过时，首先就查一下是不是编码问题。



Latex讲解第四章：Latex内容总体概观（最后一章） 

 我写这四篇讲解的目的是因为我怀着这样一种心理：我学习Latex的过程中遇到了太多的困难，花了太多的时间，费了太多的精力（买了一本教材但是是很老的，与当前版本不适合）。我不希望每一个想学习Latex的人因为计算机和教程的原因（网络上也有很多教程，但是大多是英文的，中文的教程都不是很完整，适用于初学者的不多，而且过滤这些信息要花大量时间）而半途而废或代价太高。因此我觉得自己有义务要把自己的亲身使用经验告诉大家。
 由于本讲解只是入门性的介绍，目的是给大家介绍这么一款软件和思想，同时让大家了解基本的操作。故不打算再做更深入的讲解了。正所谓“师傅领进门，修行靠个人”，要想真正的掌握它，就得由大家自己去找书看，去上网问了。同时，学习这个软件，必须要学会思考。不思考的人永远学不会。

 这一章，我就来列举Latex里面主要包括哪些知识。

字符集

 文字字符集
 数字字符集

环境集

 居中
 begin{center}
 end{center}
 左对齐
 begin{flushleft}
 end{flushleft}
 右对齐
 begin{flushright}
 end{flushright}
 列举环境
 begin{itemize}
 end{itemize}

 begin{enumerate}
 end{enumerate}
 保持环境
 verb|  |

 begin{verbatim}
 end{verbatim}
 小页环境
 begin{minipage}{size}
 end{minipage}
 诗歌环境
 begin{verse}
 end{verse}

 浮动表格环境
 begin{table}
 end{table}

 固定表格环境
 begin{tabular}
 end{tabular}

 浮动图片环境
 begin{figure}
 end{figure}

 数学环境
 $ $
 $$ $$

 begin{equation}
 end{equation}

 begin{array}
 begin{array}

 begin{eqnarray}
 end{equarray}

10种字号命令
 {tiny  }
 {scriptsize  }
 {footnotesize  }
 {small  }
 {normalsize  }
 {large  }
 {Large  }
 {LARGE  }
 {huge  }
 {Huge  }

英文字体命令
 {rm  } 罗马字体
 {bf  } 黑体
 {it  } 意大利字体
 {sc  } 小号大写字体
 {sl  } 斜体
 {tt  } 打字机字体
 {cal } 花体
 {em  } 强调型字体

几种数字的格式
 arabic{}
 roman{}
 Roman{}
 alph{}
 Alph{}

命令

文献头命令
 标题命令
 title{ }
 作者命令
 author{ }
 日期命令
 date{ }
 页注命令
 thanks{}
 footnote{}

章节命令
 部分命令
 part{}
 章命令
 chapter{}
 节命令
 section{}
 subsection{}
 subsubsection{}
 subsubsubsection{}
 附录命令
 appendix{}

使用上面所说的命令，处理一般的文字文档应该没多大问题了。但是一篇文章中还有三个很重要的部分：公式，图片，表格现在我们还没法处理。

下面就来讲一讲。

数学格式命令
 很多，这里就不写出来了。我提供的资料中有很多是讲这个的。我这里就举一个例子，让大家见识一下Latex中处理公式的思想。

 薛定谔方程
 $$i hbar frac{partial}{partial t}psi(r,t)
   = [-frac{hbar^2}{2m}nabla^2+V(r)]psi(r,t)$$
 此方程效果如图1，怎么样，感觉不错吧。
  上面式子中用$$ $$括起来的部分，就是公式的Latex写法。大家肯定吓一跳吧。其实思路很简单，它就是定义了一套规则（如^表示上标，_表示下标，frac{}{} 表示分数，第一个括号中的是分子，第二个括号中的是分母）和一些特殊字符表示方法，通过这个规则Latex能够实现将一个二维的公式写法化为一维的表达式写法。这样就便于用文本文件的方式进行存储，并用Latex编译程序进行编译再现。而且这种写法有个好处是很容易扩展。这种思想就需要自己慢慢去体会了。

插入图片
 这个需要详细说明一下。
 Latex中只支持*.eps格式的图片。因此在插入时，如果不是.eps格式的文件，则首先要用GIMP或ImageMagick工具包转换到.eps格式。然后把图片放到与.tex文件同一个目录下。（务必注意，我在初学时就是因为不知道把图片放在哪里而浪费了一个通宵）
 Latex文档中本身并不包含图片，它是在编译时加载图片。而且在加载时，还可以控制图片显示的长度和宽度。
 要在Latex文档中加载图片，必须要包括graphicx宏包。
 下面即是典型代码：

 begin{figure}
 begin{center}
 includegraphics[width=10cm,height=6cm]{1.eps}%就在前面括号中写图片名
 renewcommand{figurename}{图}
 caption{}
 label{}
 end{center}
 end{figure}

 这段代码功能是把名为1.eps的图片调进来，放在浮动图片环境中，并居中放置。
 includegraphics[width=10cm,height=6cm]{1.eps}中的includegraphics 即为调用图片的命令。[]中的参数用于指定加载图片后图片显示的宽度和高度。
 renewcommand{figurename}{图}用于将其默认的标号（为figure）改为“图”
 caption{}给这幅图片取一个名字或作一段说明用于显示出来。
 label{}中也写一个名字用来标明这幅图片，方便前面或后面的交叉引用。

插入表格

 插入表格没有图片步骤多，但也要注意一些细节问题。

 begin{table}
 centering
 renewcommand{tablename}{表}
 caption{}
 begin{tabular}{|l|l|l|}
 hline
  1 & 2 & 3\
 hline
  8.2 & 7.8 & 12.7\
  75.7 & 152.8 & 249.9\
 hline
 end{tabular}
 end{table}

 这段代码用来生成一个3行3列的浮动表格，并左右置中放置。
 begin{tabular}{|l|l|l|}中的|l|l|l|表示有三列，列与列之间用竖线隔开（共四根竖线）。
 hline表示画一根横线。
 1 & 2 & 3\中，&表示一个单元格与一个单元格的逻辑分隔符。而\则表示换行。

好了，公式，图片，表格大家都见识过了。都有一个初步的印象了。下面就该自己去使用了，多练一练才会熟悉。

 好，Latex内容的讲解就到此为止。

 学习Latex，除了买本书来看外，还要注意以下几点：
 1：仔细看书，打好基础；
 2：要多练习，要多做笔记，善于总结；
 3：多思考，因为是“先想才有所得”，才会实现“所想即所得”；
 4：有不懂的问题要向周围会的同学多问，如果无人可请教。可以到论坛上去问，那里有非常多的热心人。并且要经常从论坛上收集或摘抄下很好的代码供自己使用。（积累啊）
 5：有时不懂的问题用Baidu或Google也可以搜索出来哟。
 6：不要怕看英文文档，那里面有最原汁原味的知识；
 7：非常重要的一点，要学会做模板，并且要以做一个完全适合自己的模板为一大任务。尽管网上有很多很好的模板，但是不一定适合你。做好模板以后，你才会发觉， Latex真的是太好用了，这次论文用的这个模板，无需修改或只修改一小部分就可以供下次写论文时使用。这样既提高了速度，又提高了正确率，而且有一个明显的累积进步过程──这会使你感觉到很爽很充实的。


 好了，废话就这么多了。




Tex公式简单小结




1.自动编号的单行公式环境是
begin{equation}
…
end{equation}

不参与自动编号的单行公式环境：
[
…
]

人工编号的单行公式可以使用Tex原有的行间公式标记
$$公式 eqno 编号 $$ 将编号放在右边
$$公式 leqno 编号 $$ 将编号放在左边

引用时候可以直接用$编号$即可。

例如，$$a^2+b^2=c^2 eqno (**)$$
由公式($**$)即可得到结论。

一般情况下，行间公式 $$…$$也可以用[…]表示
但对于这种人工编号的公式，不能用[..]代替$$…$$.

2.单个公式很长，需要换行，但仅允许生成一个编号时，可以用split命令

begin{equation}
begin{split}
a &= b 
c &= d
end{split}
end{equation}

注意：每行只允许出现一个“&”，使用split命令后，编号上下居中显示。

3.多行公式：

begin{eqnarray}
左 & 中 & 右
左 & 中 & 右
…
end{eqnarray}
该环境对多行公式每行都加自动编号，如果相对某行不加编号，可在换行之前添加命令nonumber

如果要改变公式的自动编号，可以重设计数器初始值：
setcounter{equation}{数}
下一个编号自动加1。

4.方程组的排版：
多个公式，每个公式自动编号。

1) gather环境
是下面align环境的一种特殊情形。
begin{gather}
a &= b 
c &= d 
…
end{gather}
>>1.如果其中某几行使用同一个编号，则需要内嵌一个split环境。
>>2.命令notag可使当前行不编号。
2) align环境
可使几组公式并排在一起，即在同一行显示多个公式，方法是跟以前一样，使用”&”对齐。
可替代gather环境。
3) 以上几种方程组环境，无论每个公式多小，都会占满一行。使用相应的gathered，aligned环境，则只占据公式的实际宽度，整体作为一个特大的符号与其他符号一同处理。
这个结构还可以添加位置参数，以决定与其他符号的竖直对齐方式(b,t)。而且这种环境不再具有自动编号功能。

例如：
begin{equation}
left.
begin{aligned}[b]
a &= b+c 
d &= b+c
end{aligned}
right}
Longrightarrow
qquad a=d
end{equation}

这里更正参考文献中P149页的一个小错误。就是right}应该改为right}.




转载于网络，版权属于原作者

[LATEX论文排版学习资源汇总](http://bbs.sciencenet.cn/thread-1193405-1-1.html)[](http://bbs.sciencenet.cn/forum.php?mod=viewthread&tid=1193405&fromuid=81613)



latex  数学公式 [http://blog.sina.com.cn/s/blog_4b91d3b50100rfay.html](http://blog.sina.com.cn/s/blog_4b91d3b50100rfay.html)





***LaTeX*** 编辑部 [http://zzg34b.w3.c361.com/tutorial/ChineseBase.htm](http://zzg34b.w3.c361.com/tutorial/ChineseBase.htm)





在arxiv上下载latex源文件的方法(便捷的找到投稿杂志的latex源程序)


在学术google中 输入 “杂志名+ arXiv”，以 optics letter 为例：
便捷的找到投稿杂志的arXiv预印本文献
主要利用arXiv网站。
例如：
任意一个网站如：
[http://arxiv.org/format/hep-ph/0510330](http://arxiv.org/format/hep-ph/0510330)
截图如下：
选择下载源程序：
点击进入，便可下载论文源程序，以它作为模版了。

将下载后的文件加上后缀名该如： .tar.gz 或者 .dvi.gz 即可得到论文源程序（tex文档一个和论文所有的图）





[论文Latex漂亮模板以及Mathtype与Latex代码转换技巧](http://www.mathsccnu.com/forum.php?mod=viewthread&tid=1253&extra=page%3D1)









转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-654197.html](http://blog.sciencenet.cn/blog-81613-654197.html)

上一篇：[[转载]搞笑、恶搞三部曲：大雾版《北京北京》](blog-81613-652958.html)
下一篇：[[转载]图像去噪算法简介](blog-81613-654768.html)


