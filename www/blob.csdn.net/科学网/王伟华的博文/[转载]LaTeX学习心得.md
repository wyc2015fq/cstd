# 科学网—[转载]LaTeX学习心得 - 王伟华的博文




# [转载]LaTeX学习心得                           

已有 56102 次阅读2011-6-9 19:34|个人分类:[科技论文写作、搜索经验集锦](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=104604&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|关键词:LaTeX|[LaTex](misc.php?mod=tag&id=4885)|文章来源:转载





{numberwithin{equation}{section},这条命令用于按章节公式编号}


[LaTeX学习笔记](http://latex.yo2.cn/articles/latex-blog-08-4-2.html) 2 Apr, 2008  [No Comment](http://latex.yo2.cn/articles/latex-blog-08-4-2.html#respond)
下面是一篇优秀的LaTeX学习笔记,
转自: [http://vincentme.org/post/LaTeX_study_note.php](http://vincentme.org/post/LaTeX_study_note.php)
LaTeX
LaTeX（LATEX，音译“拉泰赫”）是一种基于TeX的排版系统，由美国计算机学家Leslie Lamport在20世纪80年代初期开发，利用这种格式，即使使用者没有排版和程序设计的知识也可以充分发挥由TeX所提供的强大功能，能在几天，甚至 几小时内生成很多具有书籍质量的印刷品。对于生成复杂表格和数学公式，这一点表现得尤为突出。因此它非常适用于生成高印刷质量的科技和数学类文档。这个系 统同样适用于生成从简单的信件到完整书籍的所有其他种类的文档。
摘自中文维基百科-LaTeX条目。
文档
文档模式
字号
中文书籍一般使用基本字号为五号字（10.5pt），故在article中一般选用11pt，若不选默认为10pt。
空格
中文与英文之间一般加入小空隙排出来才好看，可以加”,”或者”~”。中文与行内公式之间也要加。
如果”.”不表示句子的结束，应加入”空格”，如”Mr. Wang”应输入为”Mr.空格Wang”。 因为当”.”表示句号时，TeX会加入一段额外的空隙。
[TeX](http://latex.yo2.cn/)遇到一个单词以大写字母结束时会自动认为这并不是句子的结束，故这种情况不必加入空格。但如果确实是句子的结束，就需要在”.”前加上”@”，如“I study in SJTU@.”。
分行
\ 
- ，*为指定的行间距。
mbox{文本}可以将文本保持在同一行内。数学模式中一般以此加入中文。而且mbox里也可以出现数学模式。
标题
在用author定义作者时可使用and命令，来包含多个作者的信息。
如
author{作者1\单位1\地址1
and 作者2\单位2\地址2}
在title、author、date中均可出现thanks命令来实现脚注。
对齐
左对齐、居中对齐、右对齐的环境分别为flushleft、center和flushright。也可以使用命令raggedright、centering和raggedleft使以后的文本按指定方式对齐
引用
quotation环境：有段首缩进(正常段落有缩进的话)，有正常段间距。
quote环境：无首行缩进，增大段间距。
verse环境：类似quote，但换行时为左端悬挂。
抄录
verbatim环境可原样输出其中的文本，忽视TeX命令。verbatim*会将空格以└┘的形式输出。
对简短的抄录，可使用verb|文字|和verb*|文字|。
盒子
mbox与fbox的区别是后者会生成一个框将里面的文字包起来。
makebox[宽度][位置]{文本}中可指定盒子宽度，文本在盒子中的位置（l：左端；r：右端；s：两端，默认是居中）。framebox与此类似，也是加一个框。
表格
begin{tabular}[竖向格式]{列格式}
…………
end{tabular}
竖向格式可以选择t：顶部与外部对齐；b：底部与外部对齐，不选默认为与外部居中对齐。
multicolumn{列数}{列格式}{文本}可以以指定的文本和列格式填充数列。
cline{a-b}画一条从a列到b列的水平线。
也可以使用可视化工具LaTable来输入表格。
脚注
在不能使用脚注的环境（数学模式、表格、盒子等）中，可以先声明一个脚注标记footnotemark，在禁止模式外再加入脚注文本footnotetext{脚注文本}。
图形
在[LaTeX](http://latex.yo2.cn/)中画图可使用amscd、diagrams、Xy-pic、MetaPost、PSTricks、PGF等宏包或软件，但都较复杂，如只需要画简单的图形，可以使用一款所见即所得作图工具WinFIG。
插入图形可使用graphicx宏包。使用LaTeX编译时选择dvips选项，能插入eps格式的图形。使用pdfLaTeX编译时选择pdfTex选项，能插入pdf、png、tif、jpg格式的图片，但不支持eps。
插入图片的基本命令是includegraphics{文件名}。
放大缩小图形可使用scalebox{横向放缩因子}[竖向放缩因子]{includegraphics{文件名}}。若不输入B则竖向放缩因子等于横向放缩因子。
数学模式
空白间隙
quad            即hspace{1em}，当前M的宽度
qquad            即hspace{2em}，两个M的宽度
;                5/18个M的宽度
:                4/18个M的宽度
,                3/18个M的宽度
!                -3/18个M的宽度
空格            产生一个空格
hspace{长度}        产生指定宽度
phantom{文本}        产生指定文本的宽度
行内公示
三种进入退出行内模式的方法
(……)
begin{math}……end{math}
$……$
行间公式
进入单行无编号公式环境也有三种方法
\[……\]
\begin{displaymath}……\end{displaymath}
$$……$$
使用$$……$$环境可以手动指定编号
$$公式    \eqno自定编号$$
加载amsmath宏包后，使用选项fleqn（就是声明加载宏包时使用usepackage[fleqn]{amsmath}）可以使本该居中对齐的行间公式改为左对齐。也可以在documentclass中使用该选项。
单行公式
自动编号的单行公式
begin{equation}
公式
end{equation}
单行公式中可以是作为一个整体出现的环境或者盒子，如array环境。
当一个公式太长不适合排在一行时，可以使用split环境（amsmath）。在split中可以换行、以&进行对齐，同时使多行公式合并为一个整体用在equation中。
多行公式
多行公式可以使用eqnarray环境，带星号可取消自动编号。
begin{eqarray}
左边公式  &  中间公式  &  右边公式  \
左边公式  &  中间公式  &  右边公式  \
…………………………………………………………
左边公式  &  中间公式  &  右边公式
end{eqarray}
中间公式一般是单个数学符号，如=、>等。
eqnarray自动为每行公式编号，如不希望某行被编号，则在\之前需加入nonumber。
gather环境（amsmath）使其中每一个公式居中对齐，notag可以使某行不自动编号，使用gather*则全部不自动编号。
align环境（amsmath）可以以&指定每一行上下对齐的位置。同样，notag可以使某行不自动编号，使用align*则全部不自动编号。
flalign环境（amsmath）与align基本相同，但是使公式在每一行左右对齐。
alignat环境（amsmath）与align类似，可指定几个列队之间的间距，但需要一个参数，即为每一行中&的个数加1后除以2。
如
begin{aligned}{2}
a&=b    &hspace{20pt)    c&=d\
e&=f    &            g&=h
end{aligned}
此时“a=b”与“c=d”之间的距离即为指定的20pt。
gathered、aligned和alignated环境（amsmath）可以将生成的公式作为一个“块”放在其他环境里，er相应的不加“ed”的环境是独占它所在的行。
case环境（amsmath）用来排版左侧带有花括号的方程组，如分段函数的定义等。
上标下标
在上标、下标环境输入中文应指定字体大小。一级下标为scriptsize；二级下标为tiny。
如 [
U_{mbox{scriptsize 极大值}}=8.3, V
]
输入字母左边的角标可以使用一个空字符
如 [
{}_{phantom{1}7}^{14}mathrm{N}^-_5
]
也可以使用amsmath宏包的sideset{左侧角标}{左侧角标}主体符号。
国际标准
向量用粗斜体表示。加载bm宏包后可用bm命令实现。
常数（如π、i、e等）和微分算子d用直立体表示。
定义下面三条命令
newcommand{dif}{mathrm{d}}
newcommand{me}{mathrm{e}}
newcommand{mi}{mathrm{i}}
后就可以用dif输入微分算子d了。
无衬线斜体表示矩阵与张量。
需定义新字体mathfsl：
DeclareMathAlphabet{mathsfsl}{OT1}{cmss}{m}{sl}
使用mathfsl{矩阵}来输入。
数与单位之间应有一个小空隙,并且单位的字体应为直立体。
定理环境
我一般要自定义下面几个定理环境。
newtheorem{theorem}{定理}[section]
newtheorem{definition}{定义}[section]
newtheorem{lemma}{引理}[section]
newtheorem{corollary}{推论}[section]
newtheorem{proposition}{性质}[section]
newtheorem{example}{例}[section]
newtheorem{remark}{注}[section]
如果文档类型为book或者report的话就应将主计数器改为chapter。
插入文本
既可以使用mbox{文本}，也可以使用amsmath提供的text{文本}。后者能自动根据环境改变字体大小。
在多行公式之间加入文本可用intertext{文本}。（需加载amsmath宏包）
多重角标
substack{a\b}（amsmath）可产生多重角标，例如用在求和号下方。a和b是居中对齐的。
也可以使用subarray环境（amsmath）。它可以指定对齐方式（l：左对齐；c：居中对齐）
如
begin{subarray}{l}
abcd\
edfg
end{subarray}
分式
一般的分式使用frac{分子}{分母}输入。
连分式可使用amsmath的cfrac{分子}{分母}输入，它可以使分子分母保持同样大的字体。
二项式系数
A atop B和A choose B均可产生A在上B在下的二项式系数表达式。
加载amsmath宏包后以上两个命令无效，需换用binom{A}{B}。
宏包
ctex
使用文档类型为ctexart、ctexbook、ctexrep或者加载了ctex、ctexcap宏包后，会增加以下六种改变字体命令，kaishu相当于CJKfamily{kai}，其余类似。
kaishu楷书songti宋体youyuan幼圆lishu隶书fangsong仿宋heiti黑体使用命令zihao{…}来改变字号，参数可以选择0、-0、1、-1……6、-6、7、8，分别对应初号、小初、一号、小一……七号、八号。
文档类型为ctexart、ctexbook、ctexrep或者加载了ctex、ctexcap宏包后，会自动加入CJK环境，不必再手动加入了。
bibtex
利用bibtex建立引用文献数据库时先在待编译的源文件同目录下建立一个后缀名为”.bib”的文件。以如下格式输入所有需要引用的文献资料
@ARTICLE{,
AUTHOR =       {},
normalauthor = {},
TITLE =        {},
JOURNAL =      {},
YEAR =         {},
miscyear =     {},
VOLUME =       {},
NUMBER =       {},
PAGES =        {},
MONTH =        {},
TypeofLit =    {},
date =         {},
modifydate =   {},
citedate =     {},
url =          {},
language =     {},
}
这可以用winedt的insert->BibTeX items输入，需要填写的是首字母大写的项目。
在winedt中同时打开TeX和bib文件，在需要引用的地方加入cite{}就会自动弹出一个对话框选择需要引用的项目。
在导言区加入引用文献的格式bibliographystyle{plain}，如果杂志有特殊要求它会提供bst文件。将plain改为bst的文件名就可以了。
在文末需要出现参考文献的地方加入bibliography{bib文件名，不带后缀名.bib}
fancyhdr
fancyhdr宏包可以设计版面布局。
lhead指定左页眉内容，rfoot指定右页脚内容，其余依次类推。
使用时加载fancyhdr宏包，并且指定pagestyle{fancy}。若使用ctexart等文档模式需要在选项中加入fancyhdr。
下面是我一般用的设置，供参考
fancyhf{}                                %清空页眉页脚
lhead{bfseries Shanghai Jiao Tong University}
rhead{heiti 上,海,交,通,大,学}
cfoot{thepage}
lfoot{small sffamily Materials Science and Engineering}
rfoot{kaishu 材料科学与工程学院}
编辑器
winedt
环境匹配
在输入begin{enviroment}后立即一个右花括号}后，会自动输出end{enviroment}并且换行居中。
工具
LaTable
可视化输入表格
http://www.g32.org/latable/index.html
WinFIG
一款所见即所得作图工具，支持TeX命令输出，兼容性好。
http://user.cs.tu-berlin.de/~huluvu/WinFIG.htm


[](http://latex.yo2.cn/articles/latex_fleqn.html)LaTeX公式左对齐 22 Dec, 2007  [](http://latex.yo2.cn/articles/latex_fleqn.html#respond)No Comment
LaTeX中公式默认是居中对齐的, 但有时我们希望它能左对齐, 使用的方法如下:
1. documentclass[a4paper,fleqn]{article} 这个对整篇文章有效.
2.  对某一个公式:
some text here\
[
your equation here
]\
and more text here.
3.  如果你说的是某一个公式有两三行, 你想把这几行做左对齐.
用begin{array}
your equation (1)
your equation (2)
end{array}。
还有其它方法,
例如使用
左对齐、居中对齐、右对齐的环境分别为flushleft、center和flushright。也可以使用命令raggedright、centering和raggedleft使以后的文本按指定方式对齐.
或者
加载amsmath宏包后，使用选项fleqn（就是声明加载宏包时使用usepackage[fleqn]{amsmath}）可以使本该居中对齐的行间公式改为左对齐.


[](http://latex.yo2.cn/articles/something_about_latex.html)Something about LaTeX(中文版) 22 Dec, 2007  [](http://latex.yo2.cn/articles/something_about_latex.html#respond)No Comment

Something about LaTeX(中文版)LaTeX
LaTeX（LATEX，音译“拉泰赫”）是一种基于TeX的排版系统，由美国计算机学家Leslie Lamport在20世纪80年代初期开发，利用这种格式，即使使用者没有排版和程序设计的知识也可以充分发挥由TeX所提供的强大功能，能在几天，甚至 几小时内生成很多具有书籍质量的印刷品。对于生成复杂表格和数学公式，这一点表现得尤为突出。因此它非常适用于生成高印刷质量的科技和数学类文档。这个系 统同样适用于生成从简单的信件到完整书籍的所有其他种类的文档。
摘自中文维基百科-LaTeX条目。

文档
文档模式
字号
中文书籍一般使用基本字号为五号字（10.5pt），故在article中一般选用11pt，若不选默认为10pt。
空格
中文与英文之间一般加入小空隙排出来才好看，可以加”,”或者”~”。中文与行内公式之间也要加。
如果”.”不表示句子的结束，应加入”空格”，如”Mr. Wang”应输入为”Mr.空格Wang”。 因为当”.”表示句号时，TeX会加入一段额外的空隙。
TeX遇到一个单词以大写字母结束时会自动认为这并不是句子的结束，故这种情况不必加入空格。但如果确实是句子的结束，就需要在”.”前加上”@”，如“I study in SJTU@.”。
分行
\ 
- 
，*为指定的行间距。
mbox{文本}可以将文本保持在同一行内。数学模式中一般以此加入中文。而且mbox里也可以出现数学模式。
标题
在用author定义作者时可使用and命令，来包含多个作者的信息。
如
author{作者1\单位1\地址1
and 作者2\单位2\地址2}
在title、author、date中均可出现thanks命令来实现脚注。
对齐
左对齐、居中对齐、右对齐的环境分别为flushleft、center和flushright。也可以使用命令raggedright、centering和raggedleft使以后的文本按指定方式对齐
引用
quotation环境：有段首缩进(正常段落有缩进的话)，有正常段间距。
quote环境：无首行缩进，增大段间距。
verse环境：类似quote，但换行时为左端悬挂。
抄录
verbatim环境可原样输出其中的文本，忽视TeX命令。verbatim*会将空格以└┘的形式输出。
对简短的抄录，可使用verb|文字|和verb*|文字|。
盒子
mbox与fbox的区别是后者会生成一个框将里面的文字包起来。
makebox[宽度][位置]{文本}中可指定盒子宽度，文本在盒子中的位置（l：左端；r：右端；s：两端，默认是居中）。framebox与此类似，也是加一个框。
表格
begin{tabular}[竖向格式]{列格式}
…………
end{tabular}
竖向格式可以选择t：顶部与外部对齐；b：底部与外部对齐，不选默认为与外部居中对齐。
multicolumn{列数}{列格式}{文本}可以以指定的文本和列格式填充数列。
cline{a-b}画一条从a列到b列的水平线。
也可以使用可视化工具LaTable来输入表格。
脚注
在不能使用脚注的环境（数学模式、表格、盒子等）中，可以先声明一个脚注标记footnotemark，在禁止模式外再加入脚注文本footnotetext{脚注文本}。
图形
在LaTeX中画图可使用amscd、diagrams、Xy-pic、MetaPost、PSTricks、PGF等宏包或软件，但都较复杂，如只需要画简单的图形，可以使用一款所见即所得作图工具WinFIG。
插入图形可使用graphicx宏包。使用LaTeX编译时选择dvips选项，能插入eps格式的图形。使用pdfLaTeX编译时选择pdfTex选项，能插入pdf、png、tif、jpg格式的图片，但不支持eps。
插入图片的基本命令是includegraphics{文件名}。
放大缩小图形可使用scalebox{横向放缩因子}[竖向放缩因子]{includegraphics{文件名}}。若不输入B则竖向放缩因子等于横向放缩因子。
数学模式
空白间隙
quad            即hspace{1em}，当前M的宽度
qquad            即hspace{2em}，两个M的宽度
;                5/18个M的宽度
:                4/18个M的宽度
,                3/18个M的宽度
!                -3/18个M的宽度
空格            产生一个空格
hspace{长度}        产生指定宽度
phantom{文本}        产生指定文本的宽度
行内公示
三种进入退出行内模式的方法
(……)
begin{math}……end{math}
$……$
行间公式
进入单行无编号公式环境也有三种方法
\[……\]
\begin{displaymath}……\end{displaymath}
$$……$$
使用$$……$$环境可以手动指定编号
$$公式    \eqno自定编号$$
加载amsmath宏包后，使用选项fleqn（就是声明加载宏包时使用usepackage[fleqn]{amsmath}）可以使本该居中对齐的行间公式改为左对齐。也可以在documentclass中使用该选项。
单行公式
自动编号的单行公式
begin{equation}
公式
end{equation}
单行公式中可以是作为一个整体出现的环境或者盒子，?????array环境。
当一个公式太长不适合排在一行时，可以使用split环境（amsmath）。在split中可以换行、以&进行对齐，同时使多行公式合并为一个整体用在equation中。
多行公式
多行公式可以使用eqnarray环境，带星号可取消自动编号。
begin{eqarray}
左边公式  &  中间公式  &  右边公式  \
左边公式  &  中间公式  &  右边公式  \
…………………………………………………………
左边公式  &  中间公式  &  右边公式
end{eqarray}
中间公式一般是单个数学符号，如=、>等。
eqnarray自动为每行公式编号，如不希望某行被编号，则在\之前需加入nonumber。
gather环境（amsmath）使其中每一个公式居中对齐，notag可以使某行不自动编号，使用gather*则全部不自动编号。
align环境（amsmath）可以以&指定每一行上下对齐的位置。同样，notag可以使某行不自动编号，使用align*则全部不自动编号。
flalign环境（amsmath）与align基本相同，但是使公式在每一行左右对齐。
alignat环境（amsmath）与align类似，可指定几个列队之间的间距，但需要一个参数，即为每一行中&的个数加1后除以2。
如
begin{aligned}{2}
a&=b    &hspace{20pt)    c&=d\
e&=f    &            g&=h
end{aligned}
此时“a=b”与“c=d”之间的距离即为指定的20pt。
gathered、aligned和alignated环境（amsmath）可以将生成的公式作为一个“块”放在其他环境里，er相应的不加“ed”的环境是独占它所在的行。
case环境（amsmath）用来排版左侧带有花括号的方程组，如分段函数的定义等。
上标下标
在上标、下标环境输入中文应指定字体大小。一级下标为scriptsize；二级下标为tiny。
如 [
U_{mbox{scriptsize 极大值}}=8.3, V
]
输入字母左边的角标可以使用一个空字符
如 [
{}_{phantom{1}7}^{14}mathrm{N}^-_5
]
也可以使用amsmath宏包的sideset{左侧角标}{左侧角标}主体符号。
国际标准
向量用粗斜体表示。加载bm宏包后可用bm命令实现。
常数（如π、i、e等）和微分算子d用直立体表示。
定义下面三条命令
newcommand{dif}{mathrm{d}}
newcommand{me}{mathrm{e}}
newcommand{mi}{mathrm{i}}
后就可以用dif输入微分算子d了。
无衬线斜体表示矩阵与张量。
需定义新字体mathfsl：
DeclareMathAlphabet{mathsfsl}{OT1}{cmss}{m}{sl}
使用mathfsl{矩阵}来输入。
数与单位之间应有一个小空隙,并且单位的字体应为直立体。
定理环境
我一般要自定义下面几个定理环境。
newtheorem{theorem}{定理}[section]
newtheorem{definition}{定义}[section]
newtheorem{lemma}{引理}[section]
newtheorem{corollary}{推论}[section]
newtheorem{proposition}{性质}[section]
newtheorem{example}{例}[section]
newtheorem{remark}{注}[section]
如果文档类型为book或者report的话就应将主计数器改为chapter。
插入文本
既可以使用mbox{文本}，也可以使用amsmath提供的text{文本}。后者能自动根据环境改变字体大小。
在多行公式之间加入文本可用intertext{文本}。（需加载amsmath宏包）
多重角标
substack{a\b}（amsmath）可产生多重角标，例如用在求和号下方。a和b是居中对齐的。
也可以使用subarray环境（amsmath）。它可以指定对齐方式（l：左对齐；c：居中对齐）
如
begin{subarray}{l}
abcd\
edfg
end{subarray}
分式
一般的分式使用frac{分子}{分母}输入。
连分式可使用amsmath的cfrac{分子}{分母}输入，它可以使分子分母保持同样大的字体。
二项式系数
A atop B和A choose B均可产生A在上B在下的二项式系数表达式。
加载amsmath宏包后以上两个命令无效，需换用binom{A}{B}。
宏包
ctex
使用文档类型为ctexart、ctexbook、ctexrep或者加载了ctex、ctexcap宏包后，会增加以下六种改变字体命令，kaishu相当于CJKfamily{kai}，其余类似。
kaishu楷书songti宋体youyuan幼圆lishu隶书fangsong仿宋heiti黑体使用命令zihao{…}来改变字号，参数可以选择0、-0、1、-1……6、-6、7、8，分别对应初号、小初、一号、小一……七号、八号。
文档类型为ctexart、ctexbook、ctexrep或者加载了ctex、ctexcap宏包后，会自动加入CJK环境，不必再手动加入了。
bibtex
利用bibtex建立引用文献数据库时先在待编译的源文件同目录下建立一个后缀名为”.bib”的文件。以如下格式输入所有需要引用的文献资料
@ARTICLE{,
AUTHOR =       {},
normalauthor = {},
TITLE =        {},
JOURNAL =      {},
YEAR =         {},
miscyear =     {},
VOLUME =       {},
NUMBER =       {},
PAGES =        {},
MONTH =        {},
TypeofLit =    {},
date =         {},
modifydate =   {},
citedate =     {},
url =          {},
language =     {},
}
这可以用winedt???insert->BibTeX items输入，需要填写的是首字母大写的项目。
在winedt中同时打开TeX和bib文件，在需要引用的地方加入cite{}就会自动弹出一个对话框选择需要引用的项目。
在导言区加入引用文献的格式bibliographystyle{plain}，如果杂志有特殊要求它会提供bst文件。将plain改为bst的文件名就可以了。
在文末需要出现参考文献的地方加入bibliography{bib文件名，不带后缀名.bib}
fancyhdr
fancyhdr宏包可以设计版面布局。
lhead指定左页眉内容，rfoot指定右页脚内容，其余依次类推。
使用时加载fancyhdr宏包，并且指定pagestyle{fancy}。若使用ctexart等文档模式需要在选项中加入fancyhdr。
下面是我一般用的设置，供参考
fancyhf{}                                %清空页眉页脚
lhead{bfseries Shanghai Jiao Tong University}
rhead{heiti 上,海,交,通,大,学}
cfoot{thepage}
lfoot{small sffamily Materials Science and Engineering}
rfoot{kaishu 材料科学与工程学院}
编辑器
winedt
环境匹配
在输入begin{enviroment}后立即一个右花括号}后，会自动输出end{enviroment}并且换行居中。
工具
LaTable
可视化输入表格
http://www.g32.org/latable/index.html
WinFIG
一款所见即所得作图工具，支持TeX命令输出，兼容性好。
http://user.cs.tu-berlin.de/~huluvu/WinFIG.htm

From: [](http://blogs.sun.com/yunpu/entry/something_about_latex)http://blogs.sun.com/yunpu/entry/something_about_latex


[](http://latex.yo2.cn/articles/latex_graph_subfigure.html)latex的subfigure排版并排放置的两张图 23 Oct, 2007  [](http://latex.yo2.cn/articles/latex_graph_subfigure.html#respond)No Comment
LaTeX插图始终是件麻烦的事情, 这里介绍了如何 [](http://latex.yo2.cn/articles/latex-graph1.html)在LaTeX文档中插入图片, 但有时还是不能满足某些特殊要求. 前面介绍了[](http://latex.yo2.cn/articles/latex_subfigure.html)使用Latex的subfigure竖排图形, 这是介绍使用latex的subfigure排版并排放置的两张图.
注意要使用宏包usepackage{graphicx}和usepackage{subfigure}
[CODE]
begin{figure}
begin{minipage}[t]{0.5linewidth}
centering
includegraphics[width=2.2in]{fig1.eps}
caption{fig1}
label{fig:side:a}
end{minipage}%
begin{minipage}[t]{0.5linewidth}
centering
includegraphics[width=2.2in]{fig2.eps}
caption{fig2}
label{fig:side:b}
end{minipage}
end{figure}
还有一种方法:
在某些情况下，有时会希望将并列的图形组成一组，而其中的每一幅图 都保持其独立性。 paisubfigure 宏包的 subfigure 命令将这一 组做为一幅图形，其中的每一幅图做为子图形。例如：
[code]
begin{figure}  centering  subfigure[Small Box with a Long Caption]{    label{fig:subfig:a} %% label for first subfigure    includegraphics[width=1.0in]{graphic.eps}}  hspace{1in}  subfigure[Big Box]{    label{fig:subfig:b} %% label for second subfigure    includegraphics[width=1.5in]{graphic.eps}}  caption{Two Subfigures}  label{fig:subfig} %% label for entire figureend{figure}详细还可以参见[](http://www.ctex.org/documents/latex/graphics/node111.html)这里.


[](http://latex.yo2.cn/articles/latex_subfigure.html)Latex使用－－subfigure竖排图形 23 Oct, 2007  [](http://latex.yo2.cn/articles/latex_subfigure.html#respond)No Comment
LaTeX插图始终是件麻烦的事情, 这里介绍了如何 [](http://latex.yo2.cn/articles/latex-graph1.html)在LaTeX文档中插入图片, 但有时还是不能满足某些特殊要求. 这里介绍使用latex的subfigure竖排图形. 参考[](http://www.lonelyword.com/?p=46)这里的资料.
注意要使用宏包usepackage{graphicx}和usepackage{subfigure}
IEEE给的模板是图形横排，subfigure不支持\换行
所以网上查了查，有个人用minipage放在subfigure{}中，在minipage{}里换行
[CODE]
begin{figure}
centering
subfigure[the first subfigure]{
begin{minipage}{0.2textwidth}
includegraphics[width=1textwidth]{fig1.eps} \
includegraphics[width=1textwidth]{fig2.eps}
end{minipage}
}
subfigure[the second subfigure]{
begin{minipage}{0.2textwidth}
includegraphics[width=1textwidth]{fig3.eps} \
includegraphics[width=1textwidth]{fig4.eps}
end{minipage}
}
end{figure}
比较有用


[](http://latex.yo2.cn/articles/latex_superscript_subscript.html)LaTeX上标下标的设置 23 Oct, 2007  [](http://latex.yo2.cn/articles/latex_superscript_subscript.html#respond)No Comment
[](http://latex.yo2.cn/)LaTeX可以通过这符号$^$和$_$来设置上标和下标. 使用可以参见这里: [](http://latex.yo2.cn/articles/latex_math_modle.html)LaTeX中插入数学公式.
下面来看一些复杂一点的例子: 如何看不清楚, 可以点击图片, 然后放大图片!
[](http://image.cache.yo2blog.com/wp-content/uploads/123/12348/2007/10/sideset1.gif)
[](http://image.cache.yo2blog.com/wp-content/uploads/123/12348/2007/10/sideset.gif)
[](http://image.cache.yo2blog.com/wp-content/uploads/123/12348/2007/10/leftidx01.gif)


[](http://latex.yo2.cn/articles/latex-zihaovspt.html)字号对应的pt数 17 Oct, 2007  [](http://latex.yo2.cn/articles/latex-zihaovspt.html#respond)No Comment
[](http://latex.yo2.cn/)LaTeX中的字体大小一般都要pt做单位, 跟我们平时熟悉的四号, 五号字格式不同, 下面列出它们之间的的对于关系.
在印刷出版上，中文字号制与点数制的对照关系如下：
% begin{macro}{Chinese Font Size}
% 1770年法国人狄道（F.A.Didot）制定点数制，规定1法寸为72点，即：1点=0.3759毫米。
% 狄道点数制在法国、德国、奥地利、比利时、丹麦、匈牙利等国比较流行。
% 1886年全美活字铸造协会以派卡（pica）为基准制定派卡点数制，规定1pica=12point（点），即：
% fbox{1点=0.013837英寸=0.35146毫米}\
% 20世纪初派卡点数制传入我国，并得到逐步推广。在实用中对常用点数以号数命名而产生了号数制，
% 二者换算如下（以pt代表“点”）：\
% begin{center}
% begin{tabular}{r@{ = }l}
% 初号& 42pt\
% 小初号& 36pt\
% 一号& 28pt\
% 二号& 21pt\
% 小二号& 18pt\
% 三号& 15.75pt\
% 四号& 14pt\
% 小四号& 12pt\
% 五号& 10.5pt\
% 小五号& 9pt\
% 六号 & 7.875pt\
% 七号 & 5.25pt
% end{tabular}
% end{center}
以下内容为程序代码:
newcommand{chuhao}{fontsize{42pt}{baselineskip}selectfont}
newcommand{xiaochuhao}{fontsize{36pt}{baselineskip}selectfont}
newcommand{yihao}{fontsize{28pt}{baselineskip}selectfont}
newcommand{erhao}{fontsize{21pt}{baselineskip}selectfont}
newcommand{xiaoerhao}{fontsize{18pt}{baselineskip}selectfont}
newcommand{sanhao}{fontsize{15.75pt}{baselineskip}selectfont}
newcommand{sihao}{fontsize{14pt}{baselineskip}selectfont}
newcommand{xiaosihao}{fontsize{12pt}{baselineskip}selectfont}
newcommand{wuhao}{fontsize{10.5pt}{baselineskip}selectfont}
newcommand{xiaowuhao}{fontsize{9pt}{baselineskip}selectfont}
newcommand{liuhao}{fontsize{7.875pt}{baselineskip}selectfont}
newcommand{qihao}{fontsize{5.25pt}{baselineskip}selectfont}
%%%%%%%%%%%%%%%%%%
另外一种办法:
CODE

usepackage{CJK}
usepackage{ifthen}
newcommand{CJKfontsize}[4]{%
fontsize{#1}{#2 plus#3 minus #4}selectfont}
newcommandzihao[1]{%
ifthenelse{equal{#1}{0}}{%
CJKfontsize{42bp}{50.4pt}{.5pt}{.3pt}}{}%
ifthenelse{equal{#1}{0-}}{%
CJKfontsize{36bp}{43.2pt}{.5pt}{.3pt}}{}%
ifthenelse{equal{#1}{1}}{%
CJKfontsize{26bp}{31.2pt}{.5pt}{.3pt}}{}%
ifthenelse{equal{#1}{1-}}{%
CJKfontsize{24bp}{28.8pt}{.5pt}{.3pt}}{}%
ifthenelse{equal{#1}{2}}{%
CJKfontsize{22bp}{26.4pt}{.5pt}{.3pt}}{}%
ifthenelse{equal{#1}{2-}}{%
CJKfontsize{18bp}{21.6pt}{.3pt}{.2pt}}{}%
ifthenelse{equal{#1}{3}}{%
CJKfontsize{16bp}{19.3pt}{.3pt}{.2pt}}{}%
ifthenelse{equal{#1}{3-}}{%
CJKfontsize{15bp}{18pt}{.3pt}{.2pt}}{}%
ifthenelse{equal{#1}{4}}{%
CJKfontsize{14bp}{16.8pt}{.3pt}{.2pt}}{}%
ifthenelse{equal{#1}{4-}}{%
CJKfontsize{12bp}{14.4pt}{.3pt}{.2pt}}{}%
ifthenelse{equal{#1}{5}}{%
CJKfontsize{10.5bp}{12.6pt}{.3pt}{.2pt}}{}%
ifthenelse{equal{#1}{5-}}{%
CJKfontsize{9bp}{10.8pt}{.2pt}{.1pt}}{}%
ifthenelse{equal{#1}{6}}{%
CJKfontsize{7.5bp}{9pt}{.2pt}{.1pt}}{}%
ifthenelse{equal{#1}{6-}}{%
CJKfontsize{6.5bp}{7.8pt}{.2pt}{.1pt}}{}%
ifthenelse{equal{#1}{7}}{%
CJKfontsize{5.5bp}{6.6pt}{.1pt}{.1pt}}{}%
ifthenelse{equal{#1}{8}}{%
CJKfontsize{5bp}{6pt}{.1pt}{.1pt}}{}}


然后：

CODE

zihao{5}这是五号字。 zihao{4-}这是小四号字。 \
zihao{3}这是三号字。 zihao{0}这是初号。

%%%%%%%%%%%%%%%%%

具体的数值对应关系
七号 5.25pt 1.845mm
六号 7.875pt 2.768mm
小五号 9pt 3.163mm
五号 10.5pt 3.69mm
小四号 12pt 4.2175mm
四号 13.75pt 4.83mm
三号 15.75pt 5.53mm
二号 21pt 7.38mm
一号 27.5pt 9.48mm
小初号 36pt 12.65mm
初号 42pt 14.76mm
参考科学出版社编写的《著译编辑手册》(1994年)
参考了CTeX论坛上[](http://bbs.ctex.org/viewthread.php?tid=23862&extra=&page=1)这篇文章.


[](http://latex.yo2.cn/articles/latex-excel2latex.html)LaTeX使用excel2latex插入表格 16 Oct, 2007  [](http://latex.yo2.cn/articles/latex-excel2latex.html#comments)1条评论
[](http://latex.yo2.cn/)LaTeX中绘制表格是比较麻烦的, 下面介绍一种工具, 可以直接将excel中的表格转化为latex源代码的格式. 那就是excel2latex, 它使用excel宏编写的, 使用方法也很简单, 用excel打开excel2latex.xla, 然后你就会在工具菜单上看到一个新的按钮. 选定你要转换的表格部分, 然后按下那个按钮, 你就可以得到表格的[](http://latex.yo2.cn/)LaTeX源代码了.
详情请见
[](http://www.ctan.org/tex-archive/support/excel2latex/)http://www.ctan.org/tex-archive/support/excel2latex/
说明:
  Excel2LaTeX                             =============                                 V2.0
It's difficult to create tables in LaTeX, especially if some columns arecalculated. Excel2LaTeX allows you to transform the current selection fromExcel to Latex. Most of the formating will be kept (bold, italic, borderlines, multicolumn cells, etc.). The LaTeX code can be copied to theclipboard or saved as a LaTeX file, which then can be included in anexisting LaTeX document using the input command .
INSTALLATION~~~~~~~~~~~~Just open the file Excel2LaTeX.xla in Excel. Then you will have an additionalmenu item in your 'Tools' menu and a new Toolbar with one button on it.If you plan to use the program frequently you can save it in your addindirectory and add it with tools/add-in. This way it will be loaded wheneverexcel is opened. 
USAGE~~~~~Just select the table to convert and hit the button. You will be given theoption to save the result to a file, or send it to the clipboard (so you canpaste it on your Latex editor).本地下载: [](http://upload.cache.yo2blog.com/wp-content/uploads/123/12348/2007/10/excel2latex.zip)excel2latex.zip


[](http://latex.yo2.cn/articles/latex-romannumber.html)LaTeX中显示大小写罗马数字 15 Oct, 2007  [](http://latex.yo2.cn/articles/latex-romannumber.html#respond)No Comment
[](http://latex.yo2.cn/)LaTeX不直接支持显示大小写罗马数字, 但是可以通过自定义命理来实现.
定义新的命令rmnum，Rmnum，用来显示大小写罗马数字
用法：rmnum{数字}，Rmnum{数字}
makeatletter
newcommand{rmnum}[1]{romannumeral #1}
newcommand{Rmnum}[1]{expandafter@slowromancapromannumeral #1@}
makeatother
在文章的导言区加入上面四行, 然后就可以在正文中使用命令rmnum{数字},Rmnum{数字}来分别显示大小写罗马数字.
下面来看一个最简单的使用罗马数字的例子.
documentclass{article}
%导言区定义新命令
makeatletter
newcommand{rmnum}[1]{romannumeral #1}
newcommand{Rmnum}[1]{expandafter@slowromancapromannumeral #1@}
makeatother
begin{document}
%命令使用方法
rmnum{1},rmnum{5},rmnum{55}\
Rmnum{1},Rmnum{5},Rmnum{55}
end{document}


[](http://latex.yo2.cn/articles/latex-learning-2.html)LaTeX排版心得 15 Oct, 2007  [](http://latex.yo2.cn/articles/latex-learning-2.html#comments)1条评论
本文保存了用[](http://latex.yo2.cn/)LaTeX和PDFLaTeX打印《统计软件教程》一书的心得。 本文排版结果展示了一些特殊技巧，要知道自己如何使用这些技巧要参考本文的LaTeX源程序。 内容包括： CJK中文的设置与使用； Emacs编辑器； PS与PDF文件生成，Type 1 PS字体的使用；带导航的PDF文件；插图； 页眉、页脚的自制；源程序排版；屏幕演示，等等。
另外还有一些参考文档。
- 
[](http://www.math.pku.edu.cn/teachers/lidf/docs/textrick/tricks.pdf)LaTeX排版心得 (PDF格式) 
- 
[](http://www.math.pku.edu.cn/teachers/lidf/docs/textrick/symbols-a4.pdf)符号大全 (所有常见、不常见符号的文档) 
- 
[](http://www.math.pku.edu.cn/teachers/lidf/docs/textrick/AmsLatex.pdf)AMS LaTeX文档 (对排版数学公式很有帮助) 
- 
[](http://www.math.pku.edu.cn/teachers/lidf/docs/textrick/files.zip)files.zip (源文件及辅助文件) 
- 
[](http://www.math.pku.edu.cn/teachers/lidf/docs/textrick/ppower4.zip)ppower4.zip (制作可逐行演示幻灯片用辅助软件)转自: [](http://www.math.pku.edu.cn/teachers/lidf/docs/textrick/)http://www.math.pku.edu.cn/teachers/lidf/docs/textrick/



一个latex命令







1    makeatletter renewcommand@biblabel[1]{${}$}makeatother

该命令可以在elsevier模板中使得参考文献的格式没有序号。

2    

usepackage{natbib}在latex模板写作中的心得

2010-11-19 10:28



IEEE类journal都提供非常好的tex模板，用起来不存在什么问题。但是elsevier却只提供了简单的tex文件，很多东西都得自己弄明白。usepackage{natbib}感觉就是一个非常容易出错的地方。用elsevier的模板写完后，发现其中的括号都是圆括号和分号。查了一下，大家都是有这个问题。这里总结一下我的使用心得。

1、usepackage{natbib}的介绍
http://wiki.ctex.org/index.php/Natbib#.E5.BC.BA.E5.88.B6.E5.A4.A7.E5.86.99
这个指出
natbib 重新实现了 cite 命令以适应作者--年和编号两种形式的引用，完全兼容标准的文献样式 plain, alpha, unsrt 等，也可以配合 harvard, apalike, chicago, astron, authordate 等样式要求。 


导入usepackage[option]{natbib} 


具体的 option 有- 
round: (default) 使用圆括号 
- 
square: 使用方括号 
- 
curly: 使用花括号 
- 
angle: 使用尖括号 
- 
colon: (default) 用引号分隔多个引用 
- 
comma: 用逗号分隔多个引用 
- 
authoryear: (default) 使用作者--年引用形式 
- 
numbers: 使用编号引用形式 
- 
super: 使用 Nature 那样的上标编号引用 
- 
sort: 多个引用按照首字母排序 
- 
sort&compress: 除排序外，多个引用可以合并 (如 3-6, 15) 
- 
longnamesfirst: 多个作者的文献第一次被引用时列出所有作者，以后的引用可以缩写为 et al. 
- 
sectionbib: 重定义了 thebibliography 使用 section* 而不是 chapter*。这样使用 chapterbib 宏包时，参考文献名称为 section 一级 
- 
nonamebreak: 在一行排版所有作者名字，这可能引起 overfull hbox 警告但是消除了跨行的 hyperref 问题 2、于是我简单地把我tex中的引用包部分改成了usepackage[suqare]{natbib}发现还是出现错误，而且文章中的圆括号和那个分号依旧。仔细阅读整个应用包情况，发现其实问题不是在natbib的问题，而是文章抬头documentclass[authoryear,preprint,review,12pt]{elsarticle}已经在引用natbib，下面你再用usagepackage{natbib}更改option当然就报错啦。于是问题就解决了，把usepackage[suqare]{natbib}删掉。然后把抬头改成
documentclass[authoryear,preprint,review,12pt,square,comma]{elsarticle}
结果编译没报错，得到想要的中括号和逗号。
![](http://ctc.qzs.qq.com/ac/b.gif)









作为一个好用的排版工具,latex在学术界是备受推崇的,不管是不是小菜鸟,都要学着用起来.


latex用法设置行间距的方法：
%setlength{baselineskip}{15pt}
renewcommand{baselinestretch}{1}

2.去掉容差报警的方法：
hbadness=10000 tolerance=10000 hfuzz=150pt

3.更改子级item，enumerate的图标的方法：
renewcommand{labelenumii}{(arabic{enumii}).}
renewcommand{labelenumiii}{[arabic{enumiii}]}
renewcommand{labelenumiv}{<arabic{enumiv}>}

renewcommand{labelitemi}{PlusCenterOpen }
renewcommand{labelitemii}{Checkmark }
renewcommand{labelitemiii}{ding {43} }
renewcommand{labelitemiv}{$clubsuit$}

4.使用下划线，删除线的方法：
usepackage{ulem}
uwave sout uwave{.....} sout{...}

如果用中文，应该是 CJKulem.sty 包， 不然不会正确分行。 如：
usepackage{CJKulem} LaTex会自动给CJKulem加上.sty的后缀

d a 给文字a下面加点，如下： d G d P d I d I d O

xout:斜删除线
sout :水平删除线
uwave:波浪线

uline,uuline:下划线，双下划线

renewcommand{baselinestretch}{1.3}

renewcommand{ULdepth} 可以手工修改下划线离基线的距离。

5.高亮，加行号源代码的方法：
usepackage{color}
definecolor{gray}{rgb}{0.8,0.8,0.8}
usepackage{listings}
lstset{numbers=left} lstset{language=C++} lstset{breaklines}
lstset{extendedchars=false} lstset{backgroundcolor=color{gray}}
lstset{keywordstyle=color{blue}bfseries} lstset{frame=none}
lstset{tabsize=4} lstset{commentstyle=color{red}}
lstset{stringstyle=emph}

6.制表位画简单表：
begin{tabbing}
..........=............=..........\
....> .........>.........\
.....>..........>.........\
...........>........>....
end{tabbing}

7.盒子：
mbox{看什么看，死鸟} fbox{看什么看，死鸟}
fbox 有一个外框

framebox[宽度][位置]{.....}
位置：l:左对齐 r:右对齐 s:伸展文本
framebox[2.5cm][l]{.........}
framebox[3cm][s]{XXX dotfill XXX}

8.点填充：
dotfill
framebox[3cm][s]{XXX dotfill XXX}

9.对单个字母加上斜划线的方法：
makebox[0pt][l]{/}S

10.给文本加上边注：
............... marginpar{这是边注一个}.........

.............marginpar{...\....\....}.........

加上竖线作边注：
marginpar{rule[Y轴方向坐标：+向上,-向下]{竖线宽度}{竖线长度}}
marginpar{rule[-17.5mm]{10mm}{20mm}}

11.verbatim:
begin{verbatim}...........end{verbatim}

begin{verbatim}* ...........end{verbatim*}将空格也显示也来。

12.画表有关知识：

1)63:42
32:44
要将num1:num2这样的表项中的:予以对齐，则可用：...r@{:}l...
2)合并多列写为一个表项： multicolumn{10}{c|}{itshape ........}

13.块注释：
iffalse %块注释命令开始
....................
fi %块注释命令结束

14.多个宏包的包含法：
%一般情形下的必用宏包
usepackage{textcomp}
usepackage{latexsym,amssymb,amsmath}
usepackage{CJK,CJKnumb}

15.宏包的使用说明的查询：
%可以把...MiKTeXTeXMFdoc文件夹移到另外的地方以方便查阅下面各种宏包的使用说明

16.特殊的宏包：
%usepackage{ifpdf} %切换latex和pdflatex命令编译
%usepackage{china2e} %特殊的汉字字符

17.有关宏包的说明：
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*
%可以把...MiKTeXTeXMFdoc文件夹移到另外的地方以方便查阅下面各种宏包的使用说明
%宏包的兼容性与宏包的顺序有关,一般不应改动下面宏包的前后顺序

%一般情形下的必用宏包
usepackage{textcomp}
usepackage{latexsym,amssymb,amsmath}
usepackage{CJK,CJKnumb} %其中CJKnumb用在fancyhdr宏包时,CJK 这个环境可以在preamble里使用,
%但要记得在begin{document}之前关掉之后可以再另外打开一个新的 CJK 环境.

%其他 miktex 自带选用宏包, 包括: 图形宏包, 公式宏包, 表格宏包, 页面设置宏包, 特殊文本元素宏包
%usepackage{ifpdf} %切换latex和pdflatex命令编译
%usepackage{china2e} %特殊的汉字字符
usepackage{makeidx}makeindex %产生方式是用命令(pdf)latex,(pdf)latex,texify,(pdf)latex

%图形宏包
ifxpdfoutputundefined %即可用latex或pdflatex编译
usepackage[dvips]{graphicx}
else
usepackage[pdftex]{graphicx}
fi%将eps或pdf都放在当前目录的figures子目录下, 在文中用graphicspath{{figures/}}
usepackage[centerlast]{caption2}%图形宏包, 下面的设置在我这儿有一点冲突, 所以弃用
%usepackage[subfigure,caption2]{ccaption}%构造连续标题,无标题,双语标题或匿名标题
usepackage{color}%彩色文本、底色、文本框等等色彩支持(这个宏包只能放在这里,否则通不过编译)
definecolor{lightgray}{gray}{.80}%colorbox{lightgray}{vbox{}}
%usepackage{epic}
%usepackage[all]{xy}
usepackage{floatflt}%可把图表和文字混排, 其中[p]可选
%begin{floatingfigure}[p]{40mm}centeringcaption{}end{floatingfigure}
%begin{floatingtable}[p]{begin{tabular}end{tabular}}caption{}end{floatingtable}
%usepackage{overpic}%将LaTeX对象放置在图上
%usepackage{pstricks}%Posts cript macrosfor Generic TeX(我没用过,据说很强),在pdf下要用pdftricks
%usepackage{Picins}%图形放在正文行中
usepackage{bez123,fancybox}%latexcad/mytexcad所需要的宏包
usepackage{mytexcad}%%mytexcad是修改latexcad后的非Miktex宏包,也可用自带的latexcad.
%我想在latex和pdflatex下都能正常显示编译结果,这时把它换成bez123宏包中的相应命令即可.
%另外,改动一下latexcad.sty内的宏命令,把drawpath 命令中的path 改为lbezier.
%建议把latexcad图中的内容复制过来,并在begin{picture}前加centeringsetlength{unitlength}{1mm}
%usepackage{picinpar}%图表和文字混排宏包
%usepackage[verbose]{wrapfig} %图表和文字混排宏包
%usepackage{pspicture}%绘制任意角度和粗细的线段以及不限大小的圆,在PDF下不能用
%usepackage{eso-pic}%向文档的所有页或部分页加n副图形, 可实现水印效果
%usepackage{eepic}%扩展的绘图支持
%usepackage{curves}%绘制复杂曲线
%usepackage{texdraw}%增强的绘图工具
%usepackage{treedoc}%树形图绘制
%usepackage{pictex}可以画任意的图形
%公式宏包
usepackage[amsmath,thmmarks]{ntheorem}%定理类环境宏包,其中amsmath选项用来兼容AMS LaTeX的宏包
usepackage{cases}%begin{numcases}{|x|=}x,&for$xgeq0$\-x,&for$x<0$end{numcases}
usepackage{subeqnarray}%多个子方程begin{subeqnarray}label{eqw}slabel{eq0}\ slabel{eq1}end{subeqnarray}
%usepackage{bm}%处理数学公式中的黑斜体
usepackage{pmat}%分块矩阵的虚线,非miktex宏包,兼容性很好
iffalse begin{pmat}[{.|}]
a_{11} & a_{12} & b_{11} cr
a_{21} & a_{22} & b_{21} cr-
c_{11} & c_{12} & d_{11} cr
end{pmat}fi
usepackage{blkarray}%另一种分块矩阵

%表格宏包
usepackage{threeparttable}%表格内的脚注, 用法为
iffalse% begin{table}centering
begin{threeparttable}caption{...}
begin{tabular}{ll}% or {tabular*}
...tnote{1}&....
... end{tabular}
begin{tablenotes}
item [1] the first note
... end{tablenotes} end{threeparttable} end{table}fi%
usepackage{rotating}%用法begin{sidewaystable}....end{sidewaystable}即可旋转表格
usepackage{longtable}%,不推荐用这个,而推荐用下面这一个
usepackage{supertabular}
usepackage{tabls}
usepackage{hhline}%在表格中用 hhline 得到的结果就如同hline 或%hlinehline,当然在和垂直线的交叉处会有所不同.
usepackage{slashbox}% 可在表格的单元格中画上一斜线.
newcommand{centpcol}{leftskipfill rightskipfill}%制表使可用p{ncm}设置栏宽,还使本栏居中
%举例multicolumn{1}{p{3.5cm}|}{centpcol Same as previous response}
%usepackage{colortbl}%彩色表格
%usepackage{dcolumn}%让表格中将小数点对齐
usepackage{bigstrut,bigdelim,multirow}%表格多行合并, 矩阵的边注
newcommand{myminitab}[2][l]{begin{tabular}{#1}#2end{tabular}}
%比如两行合并可分为以下几种情况
%multirow{2}{1in}{Common g text}%行之间无横线,但要指列的定宽度;
%multirow{2}[4]*{Common g text}%行之间有横线;
%multirow{2}[4]{1in}{Common g text}%行之间有横线,但要指列的定宽度;
%multirow{2}*{myminitab[c]{Common \ g text}}%合并的文本要手动断行

%页面设置宏包
usepackage{geometry}%可以自定义页面设置
usepackage{titlesec,titletoc}%配合命令在后面,章节标题设置
usepackage{multicol,multienum}%多栏结构在文中用begin{multicols}{2}end{multicols}
%多列编号用法为(可嵌套使用):
%begin{multienumerate}
%mitemxxxx{Not}{Linear}{Not}{Quadratic}
%mitemxxxo{Not}{Linear}{No; if $x=3$, then $y=-2$.}
%mitemxx{$(x_1,x_2)=(2+dfrac{1}{3}t,t)$ or
%$(s,3s-6)$}{$(x_1,x_2,x_3)=(2+dfrac{5}{2}s-3t,s,t)$}
%end{multienumerate}
usepackage[perpage,stable]{footmisc} %让脚注在每一页重新编号和在章节、标题上加脚注
%usepackage[perpage,symbol*]{footmisc} %把脚注(footnote)的编号去掉
usepackage{indentfirst}%indent every new paragraph
usepackage{fancyhdr}% Modify the page headings of book
usepackage{fancyvrb}
usepackage{pageno}%章首页的页眉处理, 可以改为自己想要的形式
%usepackage{pifont} %有很漂亮的带圈的各种数字符号使用,也可用textcircled{}命令,参考如下命令:
%Large{textcircled{small{12}}},textcircled{九},textcircled{}不需要任何宏包即可使用,推荐
usepackage{atbeginend}%非Miktex宏包,可以注掉,能解决比如itemize,enumerate等列表中item之间的控制
%AfterBegin{itemize}{addtolength{itemsep}{-0.5baselineskip}}
%AfterBegin{enumerate}{addtolength{itemsep}{-0.5baselineskip}}
%AfterBegin{description}{addtolength{itemsep}{-0.5baselineskip}}
%usepackage{appendix}%附录宏包

%特殊文本元素宏包
usepackage{everb}
usepackage{ulem,CJKulem}%比如uwave{wave ne}等命令,注意会改写em命令
%超级链接宏包, 以下设置可去掉链接的下划线形式
usepackage[draft=false,colorlinks=true,CJKbookmarks=true,
linkcolor=black,citecolor=black,urlcolor=black]{hyperref}%这是印刷的需要,平时就改black为blue
usepackage[numbers,sort&compress]{natbib}
usepackage{hypernat}
makeatletter
newcommand{ucite}[1]{$^{mbox{scriptsize cite{#1}}}$}%参考文献的引用为上标形式的自定义
%providecommand*{toclevel@defn}{0}%去掉一些无关紧要的警告信息
makeatother
%用以下四个宏包之一能使(pdf)latex自动不嵌入相应的系列字体,减少PDF或DVI文件的大小并提高pdf的效果
%其中times宏包还能自由定义字号大小
usepackage{times} %-- Times Roman + Helvetica + Courier
%usepackage{palatino} %-- Palatino + Helvetica + Courier
%usepackage{newcent} %-- New Century Schoolbook + Avant Garde + Courier
%usepackage{bookman} %-- Bookman + Avant Garde + Courier
usepackage{balance} %balance the last two columns in twocolumn mode(命令为balance)
usepackage{delarray}%使array环境的[t]或者[b]选项生效,并可解决表格中的文字上下位置不对的问题
%由于重新定义array,所以须放在array后面,使用方法为$$abegin{array}[t][{c}]b\dend{array}c$$
usepackage{mdwlist}%用{itemize*}等来减少竖直距离a 








常用命令

Latex常用命令 出自： [](http://wenwen.soso.com/z/UrlAlertPage.e?sp=Shttp%3A%2F%2Fdicocn.bokee.com%2F5122018.html)http://dicocn.bokee.com/5122018.html



1. 段首缩进

　　中文习惯在段首缩进两格，在LaTeX中，parindent 表示段首缩进的长度，我们将它设置为当前字号的两个大写字母M的宽度，大约正好是两个汉字的宽度：

setlength{parindent}{2em}

LaTeX 默认每节的第一段的段首不缩进，这不符合中文排版习惯。我们希望正文的每一段都要缩进，使用indentfirst宏包就可办到：

usepackage{indentfirst}

2. 段距行距

　 　LaTeX 用baselineskip表示当前的行距，其默认值大约是当前字号的1.2倍，如果当前字号是10pt，那么baselineskip是12pt。 这对英文排版是合适的，对中文就显得太拥挤了，因为英文正文多为小写字母，字高与小写的x差不多（即1ex）。如果字号为10pt，那么1ex =4.3pt。我通常把行距设为字号的1.8倍：

setlength{baselineskip}{1.8em}

这个值随时可以改，对更改点以后的文字有效。

　　LaTeX 用parskip表示段距，我一般把它设为1ex：

setlength{parskip}{1ex}

注意这些修改长度的命令最好都放在正文区（即begin{document}之后）。

3. 页眉页脚

我通常用fancyhdr宏包来设置页眉和页脚。

documentclass[10pt, a4paper]{book}

usepackage{fancyhdr}

我们在 LaTeX 中先把 page style 设为fancy，再设置这个style中的页眉和页脚。但是它默认每章的第一页的page style是plain，需要单独处理。

% 设置 plain style 的属性

fancypagestyle{plain}{%

fancyhf{} % 清空当前设置

% 设置页眉 (head)

fancyhead[RE]{leftmark} % 在偶数页的右侧显示章名

fancyhead[LO]{rightmark} % 在奇数页的左侧显示小节名

fancyhead[LE,RO]{~thepage~} % 在偶数页的左侧，奇数页的右侧显示页码

% 设置页脚：在每页的右下脚以斜体显示书名

fancyfoot[RO,RE]{it Typesetting with LaTeX}

renewcommand{headrulewidth}{0.7pt} % 页眉与正文之间的水平线粗细

renewcommand{footrulewidth}{0pt}

}

pagestyle{fancy} % 选用 fancy style

% 其余同 plain style

fancyhf{}

fancyhead[RE]{leftmark}

fancyhead[LO]{rightmark}

fancyhead[LE,RO]{~thepage~}

fancyfoot[RO,RE]{it Typesetting with LaTeX}

renewcommand{headrulewidth}{0.7pt}

renewcommand{footrulewidth}{0pt}

% 设置章名和节名的显示方式

renewcommand{chaptermark}[1]{markboth{~第~thechapter~章~~~#1~}{}}

renewcommand{sectionmark}[1]{markright{~thesection~~#1~}{}}

4. 章节标题

我通常用titlesec宏包来设置正文中出现的章节标题的格式：

usepackage{titlesec}

设置章名为右对齐，字号为Huge，字型为黑体，章号用粗体，并设置间距：

titleformat{chapter}{flushrightHugehei}{{bf thechapter}}{0pt}{}

titlespacing{chapter}{0pt}{-20pt}{25pt}

设置节名的字号为Large，字型为黑体，节号用粗体，并设置间距：

titleformat{section}{Large hei }{{bf thesectionspace}}{0pt}{}

titlespacing*{section}{0pt}{1ex plus .3ex minus .2ex}{-.2ex plus .2ex}

其中hei的定义为：

newcommand{hei}{CJKfamily{hei}}

5. 纸张大小

毕业论文要求用b5纸，单面打印，我用geometry宏包来设置纸张和版心大小：

documentclass[10pt, b5paper]{report}

usepackage[body={12.6cm, 20cm}, centering, dvipdfm]{geometry}

% 以上将版心宽度设为 12.6cm，高度 20cm，版心居中，且自动设置PDF文件的纸张大小。





下面还有一个中文ppt的模版例子：- 
英文模板 documentclass[30pt,landscape,footrule]{foils}
usepackage[active]{srcltx}
usepackage{ifvtex}
usepackage{ifpdf}

ifvtexpdfpdftruefi
ifpdf
usepackage{pause}
usepackage[bgadd]{background}
usepackage{graphicx}
usepackage{geometry}
usepackage{hyperref}
ifvtexrelax
else
DeclareGraphicsRule{*}{mps}{*}{}
fi
else
usepackage[dvipdfm]{pause}
usepackage[dvipdfm]{background}
usepackage[dvips]{graphicx}
usepackage[dvips]{geometry}
usepackage[dvipdfm]{hyperref}
fi
usepackage{tabularx}
usepackage{pp4link}
usepackage{mpmulti}
usepackage{amssymb}

geometry{headsep=3ex, hscale=0.9}
hypersetup{pdftitle={pp4extensions},
  pdfsubject={title at here},
  pdfauthor={author name, School of Mathematical Sciences, 
    Peking University, email at here},
  pdfkeywords={acrobat, ppower4},
  pdfpagemode={FullScreen},
  colorlinks={true},
  linkcolor={red}
}

color{yellow}
bgadd{includegraphics[width=29cm]{bg.jpg}}

newcounter{hours}%
newcounter{minutes}%
newcounter{modcounter}%
newcommand{calculatetime}{setcounter{minutes}{time}%
  setcounter{hours}{value{minutes}}%
  global divide value{hours} by 60relax%
  setcounter{modcounter}{value{hours}}%
  global multiply value{modcounter} by 60relax%
  global advance value{minutes} by -value{modcounter}relax}%

begin{document}

parindent 0mmraggedright
MyLogo{pauselevel{=1} Schoole of Mathematical Sciences, Peking University quad
    calculatetimearabic{hours}:arabic{minutes}, today qquad
    Acrobatmenu{FirstPage}{First}quad
    Acrobatmenu{PrevPage}{Previous}quad
    Acrobatmenu{NextPage}{Next}quad
    Acrobatmenu{LastPage}{Last}quad
}

foilhead{title of this talk} vspace*{1cm}
begin{center}
author name, email, website \ vspace*{1cm}
{it {School of Mathematical Sciences, \
Peking University}}\ vspace*{1cm}
information of co-workers
end{center}

foilhead{title of this slide}

end{document}
- 
中文模板 documentclass[30pt,landscape,footrule]{foils}
usepackage[active]{srcltx}
usepackage{CJK, cctbase}
usepackage{ifvtex}
usepackage{ifpdf}

ifvtexpdfpdftruefi
ifpdf
usepackage{pause}
usepackage[bgadd]{background}
usepackage{graphicx}
usepackage{geometry}
usepackage{hyperref}
ifvtexrelax
else
DeclareGraphicsRule{*}{mps}{*}{}
fi
else
usepackage[dvipdfm]{pause}
usepackage[dvipdfm]{background}
usepackage[dvips]{graphicx}
usepackage[dvips]{geometry}
usepackage[dvipdfm]{hyperref}
fi
usepackage{tabularx}
usepackage{pp4link}
usepackage{mpmulti}
usepackage{amssymb}

geometry{headsep=3ex, hscale=0.9}
hypersetup{pdftitle={pp4extensions},
  pdfsubject={title at here},
  pdfauthor={author name, School of Mathematical Sciences, 
    Peking University, email at here},
  pdfkeywords={acrobat, ppower4},
  pdfpagemode={FullScreen},
  colorlinks={true},
  linkcolor={red}
}

color{yellow}
bgadd{includegraphics[width=29cm]{./bg.jpg}}

newcounter{hours}%
newcounter{minutes}%
newcounter{modcounter}%
newcommand{calculatetime}{setcounter{minutes}{time}%
  setcounter{hours}{value{minutes}}%
  global divide value{hours} by 60relax%
  setcounter{modcounter}{value{hours}}%
  global multiply value{modcounter} by 60relax%
  global advance value{minutes} by -value{modcounter}relax}%

begin{document}
begin{CJK}{GBK}{song}

parindent 0mmraggedright
MyLogo{begin{CJK*}{GBK}{kai}
    pauselevel{=1} 北京大学数学科学学院 quad
    calculatetimearabic{hours}:arabic{minutes}, chntoday qquad
    Acrobatmenu{FirstPage}{首页}quad
    Acrobatmenu{PrevPage}{前一页}quad
    Acrobatmenu{NextPage}{下一页}quad
    Acrobatmenu{LastPage}{末页}quad
    end{CJK*}
}

foilhead{HugeCJKfamily{ls} 报告题目} vspace*{1cm}
begin{center}
CJKfamily{song} 报告人、email、网页 \ vspace*{1cm}
CJKfamily{kai}  北京大学数学科学学院 \ vspace*{1cm}
{small CJKfamily{song} 合作者信息}
end{center}

foilhead{本页标题}

end{CJK}


end{document}![](http://imgcache.qq.com/ac/b.gif)

转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[](http://blog.sciencenet.cn/blog-81613-453403.html)http://blog.sciencenet.cn/blog-81613-453403.html 

上一篇：[](blog-81613-453401.html)[转载]怎样在学术报告中提问？怎样答问？
下一篇：[](blog-81613-459157.html)[转载]教育部《全国教育人才发展中长期规划（2010-2020年）》






