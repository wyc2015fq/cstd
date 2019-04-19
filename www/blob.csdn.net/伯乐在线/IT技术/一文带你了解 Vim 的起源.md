# 一文带你了解 Vim 的起源 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Abandon_first](http://www.jobbole.com/members/wx1763043264) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[TwoBitHistory](https://twobithistory.org/2018/08/05/where-vim-came-from.html)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
我最近偶然发现了一种名为 Intel HEX 的文件格式。据我所知，Intel HEX 文件（使用`.hex` 扩展名）通过将二进制图像编码成十六进制数字行，使二进制图像不那么晦涩难懂。显然，当人们需要对微控制器进行编程或者将数据烧录进 ROM 时会用到这种文件。无论如何，当我第一次在 Vim 中打开一个 HEX 文件时，我发现了一些震惊的东西。至少对我来说，这种文件格式是非常深奥难懂的，但 Vim 已经掌握了它。HEX 文件的每一行都是一条被划分为不同字段的记录—— Vim 已经预先将每个字段显示成不同的颜色。`set ft` 吗? 我充满敬畏地发问。`filetype=hex`，Vim 得意地回答。
Vim 无所不在且受众极其广泛，以至于其支持 HEX 文件也应该在预料之中。Mac OS 中预装了 Vim，同时，Linux 世界中也有很多 Vim 的支持者。即使那些讨厌 Vim 的人也对它很熟悉，因为太多的流行命令行工具默认使用 Vim，不熟悉 Vim 的用户往往身陷其中，这已经变成了一个 [meme](https://stackoverflow.blog/wp-content/uploads/2017/05/meme.jpeg)。包括 Facebook 在内的一些大型网站，当你按下 `j `键时，会向下滚动，而当你按下 `k `键时，会向上滚动——这意味着 Vim 通过数字文化传播达到了难以想象的高水准。
然而，Vim 也是谜一般的存在。例如，与人尽皆知的由 Facebook 开发和维护的 React 不同，Vim没有明显的发起人。尽管它如此常见和重要，但是似乎没有任何委员会或组织为 Vim 做出决策。你可以花几分钟去浏览 [Vim 网站](https://www.vim.org/)，但却无法得知是谁创建了 Vim 或者为什么创建。如果只启动 Vim 不打开任何文件，你会看到 Vim 的启动消息，表明 Vim 是由”Bram Moolenaar 等人“开发的。但这并不能说明什么，Bram Moolenaar 到底是谁，他的神秘同伙又是谁？
当我们求索上述问题的时候，也许更重要的是，为什么退出 Vim 需要输入`：wq`？当然，这是一个“写”操作，然后是一个“退出”操作，但这不是一个特别容易直观理解的约定。谁决定了复制文本应该被称为“ yanking ”？为什么`：%s/foo/bar/gc`是“查找和替换”的缩写？Vim 的特性如此武断，不可能是被编造出来的，那么它们又从何而来呢？
就像众多情况一样，答案是从那个古老的计算机熔炉——贝尔实验室开始。从某种意义上说，Vim 只是一款被称为“ wq 文本编辑器”软件的最新版本。自 Unix 时代诞生以来，这个软件一直在不断地被开发和改进。
## Ken Thompson 创建了行编辑器
1966 年，贝尔实验室聘用了 Ken Thompson 。Thompson 刚刚在加州大学伯克利分校完成了电气工程和计算机科学的硕士学位。在伯克利他使用一个名为 QED 的文本编辑器，该编辑器在 1965 到 1966 年间被开发用于伯克利分时系统。1 Thompson 到达贝尔实验室后做的第一件事就是为麻省理工学院兼容分时系统重写 QED。他后来又为 Multics 项目写了另一个版本的QED。在重写过程中，他对程序进行了扩展，以便用户可以在文件中搜索某一行，并使用正则表达式进行替换。2
与伯克利的分时系统一样，由麻省理工学院、通用电气和贝尔实验室合作的 Multics 项目试图创建一个可行的商业分时操作系统。最终，AT&T 认为这个项目毫无进展并退出。在没有分时系统的情况下，Thompson 和贝尔实验室资深研究员 Dennis Ritchie，开始怀念分时系统所提供的“交互式计算的感觉”，并着手创建他们自己的版本，该版本最终发展成为 Unix。3 1969 年 8 月，在妻子和幼子外出去加州度假时，Thompson “给操作系统、shell、编辑器和汇编程序分别分配了一个星期”，将新系统的基本组件组合在一起。4
这个编辑器被称为 `ed` 。它是基于 QED 的，但并不完全是 QED 的复现。 Thompson 决定放弃某些 QED 的功能，弱化了对常规的表达式的支持，因此 ed 只能理解相对简单的正则表达式。QED 允许用户打开多个缓冲区同时编辑多个文件，但是 `ed` 一次只使用一个缓冲区。QED 可以执行包含命令的缓冲区，而 `ed` 则不能。这些简化可能是必要的。Dennis Ritchie 曾说过，去掉 QED 的高级正则表达式是“并不大的损失”。5
`ed` 现在是 POSIX 规范的一部分，所以如果你有一个符合 POSIX 的系统，你的电脑上就安装了 `ed` 。现在，许多 `ed` 命令都是 Vim 的一部分，因此，这就值得摆弄一番了。例如，你必须使用 `w` 命令来写入磁盘缓冲区，必须使用 `q` 命令来退出编辑器。这两个命令可以写在同一行命令中，也就是 `wq`。`ed` 与 Vim 一样，是一个模态编辑器；若要从命令模式进入输入模式，取决于你试图如何转换文本，需使用 insert 命令（`i`）、append 命令（`a`）或 change 命令（`c`）。`ed` 还引入了`s/foo/bar/g`语法来查找和替换或“替换”文本。
考虑到所有这些相似之处，你可能会认为大部分 Vim 用户可以流畅地使用 `ed`。但 `ed` 在另一个重要方面，和 Vim 一点也不相似。`ed` 是一个真正的行编辑。它被广泛应用于电传打字机时代。当 Ken Thompson 和 Dennis Ritchie 在 Unix 上调试程序时看起来是这样的：
![Ken Thompson interacting with a PDP-11 via teletype.](https://upload.wikimedia.org/wikipedia/commons/8/8f/Ken_Thompson_%28sitting%29_and_Dennis_Ritchie_at_PDP-11_%282876612463%29.jpg)
`ed` 不允许你编辑开放缓冲区中那些被其他行围绕的行，也不允许移动光标，因为 `ed` 在每次修改的时候都必须重新打印整个文件。在1969年， `ed` 没有任何机制来“清除”屏幕上的内容，因为”屏幕“就是一张纸，所有已经输出的东西都像是已经用墨水打印出来了。在必要的时候，你可以使用列表命令（`l`）要求 `ed` 打印出一系列的行，但是大多数时候，你都是在你看不到的文本上操作。因此，使用 `ed` 就像是尝试用一个低电量的手电筒在黑暗房间中摸索。每次你只能看到那么一点儿，所以必须尽最大努力去记住每件东西的位置。
下面有一个 `ed` 会话的例子。我添加了注释（在字符 `#`之后）来解释了每一行，不过如果这些注释真的被输入，`ed` 并不会把它们当作注释并且会报错：


```
[sinclairtarget 09:49 ~]$ ed
i                           # Enter input mode
Hello world!
Isn't it a nice day?
.                           # Finish input
1,2l                        # List lines 1 to 2
Hello world!$
$
2d                          # Delete line 2
,l                          # List entire buffer
Hello world!$
Isn't it a nice day?$
s/nice/terrible/g           # Substitute globally
,l
Hello world!$
Isn't it a terrible day?$
w foo.txt                   # Write to foo.txt
38                          # (bytes written)
q                           # Quit
[sinclairtarget 10:50 ~]$ cat foo.txt
Hello world!
Isn't it a terrible day?
```
正如你所看到的，`ed` 并不是一个特别友好的程序。
## Bill Joy 创建了文本编辑器
对 Thompson 和 Ritchie 来说， `ed` 已经足够好了。但是其他人则认为它很难用，而且它作为一个淋漓尽致地表现 Unix 对新手敌意的例子而臭名昭著。6在 1975 年，一个名叫 George Coulouris 的人在伦敦玛丽皇后学院的 Unix 系统上开发了一个改进版 `ed` 。Coulouris 利用他在玛丽女王学院的视频显示器开发他的编辑器。与 `ed` 不同的是，Coulouris 的程序允许用户编辑在屏幕中的一行代码，通过一次次击键的方式来操作行（想象一下在 Vim 中每次编辑一行）。 Thompson 拜访玛丽女王学院时，看到 Coulouris 已经写好的程序，驳斥道他不需要在编辑文件的时候看到它的状态。受此启发，Coulouris 将他的程序命名为 `em`，或者“为凡人而生的编辑器”。7
![Portrait of George Coulouris (computer scientist).jpg](https://upload.wikimedia.org/wikipedia/en/b/b6/Portrait_of_George_Coulouris_%28computer_scientist%29.jpg)
（[George Coulouris](https://en.wikipedia.org/wiki/George_Coulouris_(computer_scientist))）
1976年，Coulouris 把 `em` 引入了加州大学伯克利分校，在那里他用了一个夏天的时间在 CS 系访学。这是 Ken Thompson 离开伯克利去贝尔实验室工作十年之后的事了。在伯克利，Coulouris 遇到了 Bill Joy，一名伯克利软件发行公司（BSD）的研究生。Coulouris 斯向Joy 展示了 `em`， Joy 以 Coulouris 的源代码为基础，为扩展 `ed` 建立了一个名为 `ex` 的改进版 `ed`。1978年，1.1 版本的 `ex` 与第 1 个版本的 BSD Unix 捆绑在一起。`ex` 在很大程度上与 `ed` 兼容，但它增加了两种模式：一种“开放”模式，这种模式可以使 `em` 单行编辑成为可能，还有一种“可见”模式，这种模式会占据整个屏幕，并且可以像我们今天所习惯的那样，对整个文件进行实时编辑。
![Bill joy.jpg](https://upload.wikimedia.org/wikipedia/commons/e/e2/Bill_joy.jpg)
（[Bill Joy](https://en.wikipedia.org/wiki/Bill_Joy)）
1979 年的第 2 版 BSD 引入了一个名为 `vi` 的可执行文件，它只在可视模式下打开 `ex` 。8
`ex`/`vi` （后来称为 `vi`）建立了我们现在使用的 Vim 中大多数的约定，但这些约定当时并不是 `ed` 的一部分。Bill Joy 使用的视频终端是 Lear Siegler ADM-3A，它的键盘没有光标键。而是，**`h`、`j`、`k `和 `l `键**上绘制光标键，所以 Bill Joy 在`vi` 中就使用这些键来进行光标移动。ADM-3A 键盘上 escape 键位置是今天我们所使用的键盘上的 tab 键，这也就解释了为什么这样一个难以够着的键会被用来实现像退出当前模式这么常见的操作。前缀命令的 `**:** `字符同样也来自 `i`，它在常规模式下（即运行 `ex` 进入的模式）使用 `:` 作为提示。这解决了一个 `ed` 中被长期诟病的问题，也就是一旦启动之后，没有任何反馈信息向用户致以问候。在可见模式下，保存和退出需要使用现在仍在使用的经典 `wq`。“Yanking”和“puttng”、标记、以及用于设置选项的 `set` 命令都是原始 `vi` 的一部分。我们今天在 Vim 中使用的的基本文本编辑过程，都是 `vi` 中使用的特性。
![A Lear Siegler ADM-3A keyboard.](https://vintagecomputer.ca/wp-content/uploads/2015/01/LSI-ADM3A-full-keyboard.jpg)
`vi` 是除 `ed `之外唯一与 BSD Unix 捆绑的文本编辑器。在那个时候，Emacs 可能会花费数百美元（这是在 GNU Emacs 之前），所以 `vi` 变得非常流行。但是 `vi` 是 `ed` 的直接衍生版本，这意味着如果没有 AT&T 的源代码，源代码就不能被修改。这促使一些人创建了 `vi` 的开源版本。 STEVIE （专门为 VI 爱好者的 ST 编辑器）出现于1987年，Elvis 出现于 1990 年，`nvi` 出现于 1994 年。其中一些克隆版本添加了额外的功能，如语法高亮和窗口分离。尤其是 Elvis ，它的许多功能被整合到 Vim 中，因为许多 Elvis 用户推动了这些功能的加入。9)
## Bram Moolenaar 创建了 Vim
“Vim”现在是“改进版 Vi”的缩写，而最初代表的是“模拟版 Vi”。和其他许多“vi克隆版本”一样，Vim 始于在一个无法使用 `vi` 的平台上复现 `vi` 的一个尝试。在荷兰 Venlo 一家影印公司工作的软件工程师 Bram Moolenaar 想要为他全新的 Amiga 2000 准备一款类似于 `vi` 的编辑器。Moolenaar 已经习惯了在大学时使用的 Unix 系统上的 `vi` ，当时他 已经对`vi`了如指掌。10 所以在 1988 年，Moolenaar 使用当时的 STEVIE `vi`克隆版本开始在 Vim 上工作。
![](https://upload.wikimedia.org/wikipedia/commons/thumb/f/ff/Bram_Moolenaar_in_2007.jpg/800px-Bram_Moolenaar_in_2007.jpg)
（[Bram Moolenaar](https://en.wikipedia.org/wiki/Bram_Moolenaar)，2006 年加入 Google）
Moolenaar 接触到 STEVIE 缘于其曾经出现在一个叫 Fred Fish 的磁盘上。Fred Fish 是一名美国程序员，每个月都会寄出一张软盘，内含为 Amiga 平台提供的精选可用开源软件。任何人只要支付邮费就可以得到一张这样的磁盘。有若干版本的 STEVIE 曾在 Fred Fish 磁盘上发布。Moolenaar 使用的 STEVIE 版本在 Fred Fish 256 号磁盘上发布。11（令人失望的是，Fred Fish 磁盘似乎与 [Freddi Fish](https://en.wikipedia.org/wiki/Freddi_Fish) 没有任何关系。）
Moolenaar 喜欢 STEVIE，但很快就注意到其缺失了很多 `vi` 命令。12 因此，在第一次发布 Vim 时，Moolenaar 优先考虑了 `vi` 的兼容性。当时已经有其他人编写了一系列的 `vi` 宏，当运行一个合适的 `vi` 兼容编辑器时，可以求解一个[随机生成的迷宫](https://github.com/isaacs/.vim/tree/master/macros/maze)。Moolenaar 能够让这些宏在 Vim 中运行。1991年，Vim 以 `Vi模拟`为名第一次发布于 Fred Fish 591 号磁盘。13 Moolenaar 添加了一些特性（包括多级撤销和解决编译器错误的“quickfix”模式），这意味着 Vim 已经完成了对 `Vi` 的超越。在 1993 年通过 FTP 发布 Vim 2.0 之前，Vim 都仍以 `Vi模拟` 的身份存在。
在众多互联网合作者的帮助下，Moolenaar 稳健地在 Vim 中加入了一些功能。Vim 2.0 引入了对`wrap`选项的支持，以及对长行文本进行水平滚动的支持。受到了`vi`克隆`nvi`的启发，Vim 3.0 增加了对分割窗口和缓冲区的支持。Vim 现在还将每个缓冲区保存到交换文件中以避免程序崩溃造成文件丢失。Vimscript 支持语法高亮显示，第一次出现是在 Vim 5.0 中。与此同时，Vim 的受欢迎程度也在不断增长。它被移植到 MS-DOS、 Windows、Mac，甚至被移植到 Unix 与原来的 `vi`竞争。
2006 年，Vim 被 *Linux Journal* 读者评为最受欢迎的编辑器。14 如今，根据 2018 年 Stack Overflow 的开发者调查，Vim 是最受欢迎的文本模式（即终端模拟器）编辑器，受用于 25.8% 的软件开发人员(和 40% 的 Sysadmin / DevOps 人员)。15 在 1980 年代末和整个 1990 年代，程序员一度发起了“编辑器战争”，将 Emacs 用户与 `vi` （即最终的 Vim ）用户进行了对比。虽然 Emacs 肯定仍有一些追随者，但有些人认为编辑器战争已经以 Vim 获胜而结束。16 2018年 Stack Overflow 的开发者调查显示只有 4.1% 的受访者使用 Emacs，也验证了这个事实。
Vim 是如何变得如此成功的？显然，人们喜欢 Vim 所提供的特性。但我认为，Vim 背后的悠久历史表明了它的优势远不仅仅体现在其功能集上。Vim 的代码库可以追溯到 1988 年，当时 Moolenaar 开始研究它。另一方面，“ wq 文本编辑器”——关于 Unix-y 文本编辑器应该如何工作的更广泛的愿景——可以追溯到半个世纪以前。“ wq 文本编辑器”有一些不同的具体表达方式，但在某种程度上要感谢 Bill Joy 和 Bram Moolenaar 对向后兼容性非比寻常的关注，才使好的想法逐渐积累起来。从这个意义上说，“ wq 文本编辑器”是运行时间最长、最成功的开源项目之一，得益于计算机世界中一些最伟大的思想贡献。我不认为“创业公司无视所有先例来创造颠覆性的新软件”的开发方式都是不妥的，但 Vim 提醒我们，这种协作和增量的方式同样能产生奇迹。
*@TwoBitHistory 每两周更新一次类似文章，如果你喜欢本文请在 Twitter 上关注或者订阅 RSS，以便及时知晓更新发布。伯乐在线已获授权同步翻译中文版，敬请关注*
- Butler Lampson, “Systems,” Butler Lampson, accessed August 5, 2018, [http://bwlampson.site/Systems.htm](http://bwlampson.site/Systems.htm).
- Dennis Ritchie, “An Incomplete History of the QED Editor,” accessed August 5, 2018, [https://www.bell-labs.com/usr/dmr/www/qed.html](https://www.bell-labs.com/usr/dmr/www/qed.html).
- Peter Salus, “The Daemon, the GNU, and the Penguin,” Groklaw, April 14, 2005, accessed August 5, 2018, [http://www.groklaw.net/article.php?story=20050414215646742](http://www.groklaw.net/article.php?story=20050414215646742).
- ibid.
- Dennis Ritchie, “An Incomplete History of the QED Editor,” accessed August 5, 2018, [https://www.bell-labs.com/usr/dmr/www/qed.html](https://www.bell-labs.com/usr/dmr/www/qed.html).
- Donald Norman, “The Truth about Unix: The User Interface Is Horrid,” Datamation, accessed August 5, 2018, [http://www.ceri.memphis.edu/people/smalley/ESCI7205_misc_files/The_truth_about_Unix_cleaned.pdf](http://www.ceri.memphis.edu/people/smalley/ESCI7205_misc_files/The_truth_about_Unix_cleaned.pdf).
- George Coulouris, “George Coulouris: A Bit of History,” George Coulouris’ Homepage, September 1998, accessed August 5, 2018, [http://www.eecs.qmul.ac.uk/~gc/history/index.html](http://www.eecs.qmul.ac.uk/~gc/history/index.html).
- “Second Berkeley Software Distribution Manual,” Roguelife, accessed August 5, 2018, [http://roguelife.org/~fujita/COOKIES/HISTORY/2BSD/vi.u.html](http://roguelife.org/~fujita/COOKIES/HISTORY/2BSD/vi.u.html).
- Sven Guckes, “VIM Wishlist,” Vmunix, May 15, 1995, accessed August 5, 2018, [https://web.archive.org/web/20080520075925/http://www.vmunix.com/vim/wish.html](https://web.archive.org/web/20080520075925/http://www.vmunix.com/vim/wish.html).
- Bram Moolenaar, “Vim 25” (lecture, Zurich, November 2, 2016), December 13, 2016, accessed August 5, 2018, [https://www.youtube.com/watch?v=ayc_qpB-93o&t=4m58](https://www.youtube.com/watch?v=ayc_qpB-93o&t=4m58)s
- ibid. (?t=6m15s)
- ibid. (?t=7m6s)
- “Fish Disks 1 – 1120,” Amiga Stuff, accessed August 5, 2018, [http://www.amiga-stuff.com/pd/fish.html](http://www.amiga-stuff.com/pd/fish.html).
- “2005 Linux Journal Reader’s Choice Awards,” Linux Journal, September 28, 2005, accessed August 5, 2018, [https://www.linuxjournal.com/article/8520#N0x850cd80.0x87983bc](https://www.linuxjournal.com/article/8520#N0x850cd80.0x87983bc).
- “Stack Overflow Developer Survey 2018,” Stack Overflow, accessed August 5, 2018, [https://insights.stackoverflow.com/survey/2018/#development-environments-and-tools](https://insights.stackoverflow.com/survey/2018/#development-environments-and-tools).
- Bruce Byfield, “The End of the Editor Wars,” Linux Magazine, May 11, 2015, accessed August 5, 2018, [http://www.linux-magazine.com/Online/Blogs/Off-the-Beat-Bruce-Byfield-s-Blog/The-End-of-the-Editor-Wars](http://www.linux-magazine.com/Online/Blogs/Off-the-Beat-Bruce-Byfield-s-Blog/The-End-of-the-Editor-Wars).
