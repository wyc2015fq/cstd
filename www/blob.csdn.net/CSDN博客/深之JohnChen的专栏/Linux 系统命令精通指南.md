# Linux 系统命令精通指南 - 深之JohnChen的专栏 - CSDN博客

2006年02月07日 11:25:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1914


**Linux 系统命令精通指南**

在“Linux 文件命令精通指南”中，我们讨论了利用最基础和最常用 Linux 文件命令所需的基本信息。在本文中，我将对 Linux 系统命令作同样的讨论。

　　对于文件命令，我们在此处详尽讨论了某些相当简单和直观的管理命令。而只描述了其他较复杂命令的功能，以便在您需要使用时，知道存在这些命令及其用途。本文不涉及对每个命令使用背后的基本原理的详细讨论，但是我们将讨论最基本和最有用的命令，这些命令用于监视和管理打印机服务、文件系统、磁盘和进程。

　　有关超级用户的更多信息

　　这里讨论的很多命令可以由常规用户在其自己的文件和进程中执行。但是，某些命令，如 fdisk，需要有 root 权限才能运行。

　　为了您的系统的完整性，建议您避免直接以 "root" 登录到系统中。相反，通常更适合以常规用户登录，然后执行 su 或从 X window 执行 sux 命令来运行系统管理功能。(Sux 为您提供 X window 环境以及 root 权限。）当您以 root 用户登录时，您会始终知道您处于这种状态，因为您的提示符变为“#”。

　　超级用户能够使用 sudo，有选择地将那些通常为 root 用户或其他特殊用户所保留的权限分配给一个不同的常规用户。root 用户可以定义实际和有效的用户和组标识号 uid 和 gid，以便与预期用户的标识相匹配。这种预期用户可以是（但不必是）被允许运行该命令的用户。

　　换言之，超级用户可以允许其他用户执行某个命令，但也可以确定由进程所产生的任何结果文件应该属于 root 用户或任何由超级用户指定的其他用户。Sudo 权限一经分配，则只需要用户使用自己的口令进行验证，而不需要 root 用户口令。

　　通用命令

　　从技术上讲，Uname、diff、date、sort 和 uniq 应该看作文件命令，但由于它们经常用于诊断和维护例程的系统管理方面，在这里解释它们更有意义。

　　Uname
　　Uname 提供了常规系统信息。当指定 -a 选项时，显示所有可用信息：
　　$ uname -a
　　Linux linux 2.4.18-4GB #1 Wed Mar 27 13:57:05 UTC 2002 i686 unknown

　　在这里我们从左向右看到常规的系统信息。每条信息都有自己的选项（在下面的圆括号中），可以单独显示：

　　操作系统名称为 Linux(-s)。

　　本机的网络节点名为 linux (-n)。

　　操作系统发布版本为 4.5.18-4GB (-r)。

　　操作系统版本为 #1 Wed Mar 27 13:57:05 UTC 2002 (-v)。

　　机器类型为 i686 (-m)。

　　这里的处理器类型 (-p) 为未知。

　　类似地，stty 显示关于您的终端设置的信息：
　　$ stty -a
　　speed 38400 baud; rows 40; columns 103; line = 0;
　　intr = ^C; quit = ^/; erase = ^?; kill = ^U; eof = ^D; eol = ; eol2 = ; start = ^Q;
　　stop = ^S; susp = ^Z; rprnt = ^R; werase = ^W; lnext = ^V; flush = ^O; min = 1; time = 0;
　　-parenb -parodd cs8 -hupcl -cstopb cread -clocal -crtscts -ignbrk -brkint -ignpar -parmrk -inpck
　　-istrip -inlcr -igncr icrnl ixon -ixoff -iuclc -ixany -imaxbel
　　opost -olcuc -ocrnl onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0
　　isig icanon iexten echo echoe echok -echonl -noflsh -xcase -tostop -echoprt echoctl echoke

　　第二行和第三行显示了将在命令行分别调用的特殊字符和键序列。例如，"^Z" 即 CNTRL-Z 在本系统中发送一个终端休止符。退出符号以 "^/" 即 CNTRL-/ 发送，而我们可以将其改为 "^X"，如下所示：
　　$ stty quit ^X

　　在这种情况下，您实际要输入 ^ 符号，而不是 CNTRL 键。这种改变只在当前窗口中起效。为了使这种改变永久生效，需要在您的 .bashrc 文件中输入该语句。

　　Diff 和 Date。顾名思义，Diff 比较两个文件。它返回那些导致文件不同的行，并返回关于如何使文件变得相同的一些含义隐晦的输出：
　　$ diff test.out test2.out
　　0a1
　　> Another test.

　　在这里我们被告知，需要将 "Another test." 这一行添加到 test.out，以便使它与 test2.out 相似。第一个字符 "0" 是要在 test.out 中编辑的行号；"a" 表示该行应该被添加到 test.out，以便与 test2.out 的第一行 "1" 相匹配。如果我们需要更改 test.out，则第二个字符也可以是 "c"，而如果我们需要从 test.out 中删除一些内容，则第二个字符是 "d"。

　　您可能希望使一个文件与另一个文件相似，以便为某些代码提供补丁。diff 还经常作为分析工具，用于验证程序或脚本 — 例如 sed 脚本 — 的输出是否是所预期的输出。 
date 命令可用于显示或设置日期。如果用户具有超级用户权限，则他或她可以通过在以下命令中提供一个数字型字符串来设置日期： 
　　$ date 0101010004 
　　此命令将日期设置为 2004 年 (04) 1 月 1 日 (0101) 凌晨 1 时 (0100)。 

　　但是，任何用户可以用所需的格式来显示日期。注意在格式序列之前需要一个加号 "+"。如果没有出现此命令的预期输出，则尝试将它包含在单引号中： 
　　$ date +´%H%M%S´ 
　　204748 

　　您可以在序列中包含空格，这时您需要使用双引号： 
　　$ date +"%H %M %S" 
　　20 47 51 

　　在日期的主页上列有三十多种不同的格式序列，这些序列可以按需要结合起来，以获得您最后的输出格式。 

　　date 的输出还可以用于脚本中，如： 
　　$ date -d ´2 day´ 
　　Thu Jan 29 19:02:02 EST 2004 

　　-d 选项指示 date 显示其后字符串中所描述的时间。在上例中，我们要求显示从现在起（2004 年 1 月 27 日）两天后的日期。我们也可以要求显示三天前的日期，其命令为： 
　　$ date -d ´-3 day´ 
　　Sun Jan 25 20:56:59 EST 2004 

　　date 多种功能的另一个示例是能够向其输出附加字符串，为生成唯一的文件名提供了一种便利的方法。 
　　$ ls -l | cat > $(date +ls_%d%b%y%H%M.out) 
　　$ ls ls_*.out 
　　ls_04Feb041633.out 

　　在这里，我们将一个长文件列表的输出传送给 cat，它将我们的输出再传送到一个名为 ls_04Feb041609.out 的文件。注意我们为文件名使用了一个变量，由美元符号 "$" 表示。依次将 "ls_" 和 ".out" 附加到日期，其格式为月份中的日 (%d)、本地的月份名称缩写 (%b)、年份的最后两位数字 (%y)、小时 (%H) 和分钟 (%M)，从而产生此文件名。 

　　Sort 和 Uniq。 您可以从管道或从命令行所指定的文件对文本文件的行进行 sort 操作。除非另外指定，sort 对多列输出的第一列进行操作。在默认情况下，除非使用 -n 选项，数字型的列按字母顺序排序。 

　　Sort 列可以用两种不同的格式指定。如果使用 -k 选项，则列的位置从 1 开始进行编号。您还可以使用 +POS1 格式，从第一列编号为零开始进行列的编号。以下两个命令产生相同的输出： 
　　$ ls -l *.out | sort +5 -7 
　　$ ls -l *.out | sort -k 6,8 
　　-rw-rr 1 bluher users 375 Dec 5 2002 fdisk.out 
　　-rw-rr 1 bluher users 2447 Dec 28 20:15 ls.out 
　　-rw-rr 1 bluher users 590 Jan 4 21:24 greps.out 
　　-rw-rr 1 bluher users 78 Jan 4 21:25 sgrep.out 
　　... 

　　在默认情况下，sort 命令的列分隔符是空格和制表符。可以使用 -t SEP 选项指定其他分隔符，其中 SEP 是您指定的分隔符号。sort 的默认顺序是从最低到最高，利用 -r 选项可以将顺序颠倒。 

　　Sort 可用于很多不同的任务。例如，它可用于对文件输出进行排序（如 /etc/passwd），或者如我们在下面所看到的那样，对 du 的输出进行排序。当使用 -m 选项时，sort 将文件合并，实际上并没有对它们进行排序。但是，如果您希望将它们排序，就应该在文件合并前对它们排序。 

　　Sort 经常与 uniq 命令一起使用，从已排序的文件中删除完全相同的行。uniq 命令在管道中经常跟在 sort 命令的后面，还可以使用 -c 选项来计算某一行所出现的次数，或者使用 -d 选项，只报告完全相同的行： 
　　$ sort -m test.out test2_sort.out | uniq -c 
　　1 Another test. 
　　1 Testing testing 
　　2 This is a test. 

　　在这里我们合并两个文件 test.out 和 test2_sort.out。然后我们使用 -c 选项，将输出传送到 uniq，这样最后的结果是按字母顺序排列的列表，完全相同的行被删除，并带有一个关于每行出现频率的报告。

　　打印机控制

　　在多数情况下，打印机命令的使用相对较为明确。通过在 lpr 命令后面指定文件名，将作业发送到打印机。如果没有提供文件名，则假定使用标准输入。使用 lpq 命令，您可以获得关于发送到打印机的作业的状态报告。当您指定作业号时，Lprm 将作业从行打印机后台打印队列中删除。

　　$ lpr grep2.out
　　$ lpq
　　Printer:lp@linux ´lpdfilter drv=upp method=auto color=yes´
　　Queue:1 printable job
　　Server:pid 1929 active
　　Unspooler:pid 1930 active
　　Status:waiting for subserver to exit at 21:24:14.637
　　Rank Owner/ID Class Job Files Size Time
　　active bluher@linux+928 A 928 grep2.out 779 21:24:14
　　done bluher A 877 (STDIN) 117258 13:09:53
　　$ lprm 928
　　Printer lp@linux:
　　checking perms ´cfA928linux.local´
　　dequeued ´cfA928linux.local´

　　在这里，当我们将作业发送到打印机后，我们查询其状态。Lpq 向我们显示编号为 877 的作业已经完成，而我们刚发送的作业编号为 928。我们使用命令 lprm 928 将活动的作业从队列中删除。

　　磁盘信息

　　无论您是维护自己的系统还是支持具有很多用户的企业，监视磁盘空间都是系统管理员更重要的职责之一。du、df 和 fdisk 命令提供了可用磁盘空间的不同视图。

　　du 命令提供对每个目录的文件空间使用情况的评估。在默认情况下，测量的单位是块的数目，块的大小通常在安装您的系统时确定。

　　在以下的示例中，系统是使用 1024 字节的块安装的。如果没有指定目录，则 du 开始对当前目录进行计算。-h 选项以易于读懂的形式提供大小的值：

　　$ du -h Documents
　　105k Documents/jj
　　573k Documents/John
　　682k Documents

　　可以使用 -a 选项更改输出，该选项指示 du 显示文件和目录的块数。选项 block-size=SIZE 指示 du 以 SIZE 所指定字节大小的块的数目来显示输出。

　　$ du block-size=512 Documents
　　209 Documents/jj
　　1146 Documents/John
　　1363 Documents

　　在默认情况下，du 还为每个子目录递归地打印信息。通过使用 max-depth=N 选项可以限制这种操作，其中 N 等于被报告的最大级数：

　　$ du max-depth=2 .
　　$ du max-depth=2 | sort -n
　　4 ./.xemacs
　　4 ./public_html
　　105 ./Documents/jj
　　573 ./Documents/Ian
　　682 ./Documents
　　2420 ./jdevhome/system
　　8350 ./jdevhome/system9.0.3.1035
　　13483 ./jdevhome/mywork
　　24252 ./jdevhome
　　...

　　du 报告目录和子目录的空间使用情况，而 df 报告已安装文件系统的磁盘空间使用情况：

　　$ df
　　Filesystem 1k-blocks Used Available Use% Mounted on
　　/dev/hda6 4195632 1983656 2211976 48% /
　　/dev/hda5 14607 3778 10075 28% /boot
　　/dev/hda9 937372 494028 443344 53% /home
　　/dev/hda8 6297248 3876880 2420368 62% /opt
　　shmfs 256220 0 256220 0% /dev/shm

　　在这里，df 报告了五个文件系统的状态。与 du 类似，df 利用 -h 选项提供人类可读的输出：

　　$ df -h
　　Filesystem Size Used Avail Use% Mounted on
　　/dev/hda6 4.0G 1.9G 2.1G 48% /
　　/dev/hda5 14M 3.7M 9.8M 28% /boot
　　/dev/hda9 915M 476M 440M 52% /home
　　/dev/hda8 6.0G 3.7G 2.3G 62% /opt
　　shmfs 250M 0 250M 0% /dev/shm

　　df 命令还允许您利用 -block-size=SIZE 选项来指定块的大小。

　　fdisk 命令是 Linux 的分区表操作命令。与 du 和 df 不同，没有 root 用户权限时不能运行该命令。如果指定了 -l 选项，则可以将它仅用于报告的目的。它提供了您的整个分区表的完整视图，而不仅仅是您使用 df 所看到的扩展 Linux 分区：

　　# fdisk -l

　　Disk /dev/hda:240 heads, 63 sectors, 1940 cylinders
　　Units = cylinders of 15120 * 512 bytes

　　Device Boot Start End Blocks Id System
　　/dev/hda1 1 286 2162128+ c Win95 FAT32 (LBA)
　　/dev/hda2 * 288 1940 12496680 5 Extended
　　/dev/hda5 288 289 15088+ 83 Linux
　　/dev/hda6 290 844 4195768+ 83 Linux
　　/dev/hda7 845 983 1050808+ 82 Linux swap
　　/dev/hda8 984 1816 6297448+ 83 Linux
　　/dev/hda9 1817 1940 937408+ 83 Linux
　　进程监视

　　在繁忙的多用户系统中，进程监视也是 Linux 系统管理员的一项重要职责。在这方面，ps 和 top 是两个最有用的命令。
　　ps 命令提供了当前运行进程的快照。

　　生动翔实的一课

　　以下是 ps 的最简单形式：

　　$ ps
　　PID TTY TIME CMD
　　3884 pts/1 00:00:00 bash
　　3955 pts/2 00:00:00 more
　　3956 pts/5 00:00:05 sqlplus

　　PID 是进程的标识号。

　　TTY 是进程所属的终端控制台。

　　TIME 列是进程所使用的总的 CPU 时间。

　　CMD 列列出正在执行的命令行。

　　使用带有 -ef 选项的 ps，返回系统中所有用户的所有进程的完整列表。如果您将此 ps 命令的结果传送到 grep 中，则该结果更易于查看。例如：

　　$ ps -ef | grep oracle
　　UID PID PPID C STIME TTY TIME CMD
　　oracle 1633 1 0 13:58 ?00:00:00 ora_pmon_ora1
　　oracle 1635 1 0 13:58 ?00:00:00 ora_dbw0_ora1
　　oracle 1637 1 0 13:58 ?00:00:01 ora_lgwr_ora1
　　oracle 1639 1 0 13:58 ?00:00:02 ora_ckpt_ora1
　　oracle 1641 1 0 13:58 ?00:00:02 ora_smon_ora1
　　oracle 1643 1 0 13:58 ?00:00:00 ora_reco_ora1
　　oracle 1645 1 0 13:58 ?00:00:00 ora_cjq0_ora1
　　oracle 1647 1 0 13:58 ?00:01:18 ora_qmn0_ora1
　　oracle 1649 1 0 13:58 ?00:00:00 ora_s000_ora1
　　oracle 1651 1 0 13:58 ?00:00:00 ora_d000_ora1

　　注意，当您将 ps 的输出传送到 grep 命令时，列标题并不象在 ps 命令本身所使用时那样正常地显示。为便于说明，标题位于输出的上方，以便清楚地显示其联系。

　　虽然 uid 通常是指数字型的标识，但在第一列下指定的是用户名，标记为 UID。

　　PPID 是父进程的标识号。对于 Oracle 进程，这里的标识号为 1 — 它是 init 进程（所有进程的父进程）的 id，因为在本系统中安装的 Oracle 是作为登录进程的一部分而启动的。
　　标为 C 的列是由 CPU 用于计算执行优先级的因子。
　　STIME 是指进程的启动时间。
　　问号表示这些进程不属于任何 TTY，因为它们是由系统启动的。

　　以下是 ps 命令带有某些不同选项的另一个示例。注意，其中很多列与执行带有 -ef 选项的 ps 时的列相同：

　　$ ps aux
　　USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
　　bluh 4024 0.0 0.2 2240 1116 pts/1 S 20:59 0:00 su bluher
　　bluh 4025 0.0 0.3 2856 1668 pts/1 S 20:59 0:00 bash
　　bluh 4051 0.0 0.2 2488 1504 pts/1 R 21:01 0:00 ps aux
　　bluh 4052 0.0 0.1 1636 600 pts/1 S 21:01 0:00 grep bluher

　　除了提供重要的诊断信息，ps 命令还经常用于找出失控进程的 PID。例如，您的一个用户在 SQL*Plus 会话中输入一个笛卡尔联接。您可以执行以下的 ps 命令来找出其 PID：

　　$ ps -ef | grep bluher
　　...
　　bluher 4068 4064 0 21:14 pts/6 00:00:00 less
　　bluher 4112 3945 0 21:28 pts/5 00:08:42 sqlplus
　　...

　　然后您可以使用以下命令将其终止：
　　$ kill 4112

　　常规用户可以看到所有的系统进程，但他们只能终止他们自己拥有的进程。

　　Ps 只为您提供当前进程的快照。要即时查看最活跃的进程，可使用 top。Top 实时地提供进程信息。它还拥有交互式的状态，允许用户输入命令，如 n 后面跟有 5 或 10 等数字。其结果是指示 top 显示 5 或 10 个最活跃的进程。Top 持续运行，直到您按 "q" 退出 top 为止。

　　以下是 top 的部分显示：

　　8:14pm up 51 min, 7 users, load average: 0.02, 0.03, 0.05
　　96 processes:87 sleeping, 2 running, 0 zombie, 7 stopped
　　CPU states:6.4% user, 3.5% system, 0.0% nice, 89.9% idle
　　Mem:512440K av, 392288K used, 120152K free, 0K shrd, 55668K buff
　　Swap:1050800K av, 0K used, 1050800K free 236128K cached

　　PID USER PRI NI SIZE RSS SHARE STAT %CPU %MEM TIME COMMAND
　　1289 root 18 0 78008 11M 1796 R 3.7 2.2 0:49 X
　　1965 bluher 20 0 12244 11M 10456 S 1.9 2.3 0:01 kdeinit
　　1986 bluher 18 0 996 996 768 R 1.5 0.1 0:00 top
　　1803 bluher 20 0 16960 16M 13508 S 0.9 3.3 0:03 kdeinit
　　1804 bluher 20 0 16964 16M 13508 S 0.9 3.3 0:03 kdeinit
　　1800 bluher 19 0 12260 11M 10076 S 0.1 2.3 0:04 kdeinit
　　1 root 20 0 220 220 188 S 0.0 0.0 0:04 init
　　...　　

　　关闭

　　看来应该以 shutdown 命令来结束本次讨论。与本文以及上一篇文章中所讨论的许多命令一样，通常有一种 GUI 形式的替代方法可以关闭您的系统。但是，当您需要手动关闭时，您可以使用以下命令立即进行关闭：
　　$ shutdown -h now

　　您可以使用以下命令手动地重新启动系统：
　　$ shutdown -r now

　　注意这两个命令都需要指定时间。您可以指定 "now" 或使用带有时间秒数的 -t 选项。

　　我们在这两篇文章中讨论了大量内容，但我们绝对无法涵盖 Linux 可能具有的全部内容。我们没有讨论的命令还有很多，而我们没有看到的命令选项则更多。但是，在这里我们希望您至少找到了开始管理 Linux 系统所需的最少限度的工具。 

