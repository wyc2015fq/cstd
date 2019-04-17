# 鸟哥的私房菜：Bash shell(六)-管道命令 - 在思索中前行！ - CSDN博客





2014年09月24日 21:03:39[_Tham](https://me.csdn.net/txl16211)阅读数：854








      就如同前面所说的， bash 命令执行的时候有输出的数据会出现！ 那么如果这群数据必需要经过几道手续之后才能得到我们所想要的格式，应该如何来设定？ 这就牵涉到管线命令的问题了 (pipe) ，管线命令使用的是『 | 』这个界定符号！ 另外，管线命令与『连续下达命令』是不一样的呦！
 这点底下我们会再说明。底下我们先举一个例子来说明一下简单的管线命令。
　　假设我们想要知道 /etc/ 底下有多少档案，那么可以利用 ls /etc 来查阅，不过， 因为 /etc 底下的档案太多，导致一口气就将屏幕塞满了～不知道前面输出的内容是啥？此时，我们可以透过 less 指令的协助，利用：
　　[root@linux ~]# ls -al /etc | less

嘿嘿！如此一来，使用 ls 指令输出后的内容，就能够被 less 读取， 并且利用 less 的功能，我们就能够前后翻动相关的信息了！很方便是吧？呵呵！ 我们就来了解一下这个管线命令『 | 』的用途吧！ 这个管线命令『 | 』仅能处理经由前面一个指令传来的正确信息，也就是 standard output ( STDOUT
 ) 的信息，对于 stdandard error 并没有直接处理的能力，请记得。

在每个管线的前后部分都是『指令』呢！而后一个指令的输入乃是由前一个指令的输出而来的！ 不过，要注意的是，在 Linux 的环境中，很多的讯息处理都是以『行』为单位～ 也就是以是否具有 [Enter] 标志 (CR) 来作为一段处理的依据喔！ 底下我们来谈一谈一些基本的管线命令指令介绍：






## 撷取命令： cut, grep
什么是撷取命令啊？说穿了，就是将一段数据经过分析后，取出我们所想要的。 或者是，经由分析关键词，取得我们所想要的那一行！ 不过，要注意的是，一般来说，撷取讯息通常是针对『一行一行』来分析的， 并不是整篇讯息分析的喔～底下我们介绍两个很常用的讯息撷取命令：
1) cut
cut 不就是『切』吗？没错啦！这个指令可以将一段讯息的某一段给他『切』出来～ 处理的讯息是以『行』为单位喔！底下我们就来谈一谈：
[root@linux ~]# cut -d'分隔字符' -f fields
[root@linux ~]# cut -c 字符区间
参数：
-d ：后面接分隔字符。与 -f 一起使用；
-f ：依据 -d 的分隔字符将一段讯息分割成为数段，用 -f 取出第几段的意思；
-c ：以字符 (characters) 的单位取出固定字符区间；

范例一：将 PATH 变量取出，我要找出第三个路径。
[root@linux ~]# echo $PATH
/bin:/usr/bin:/sbin:/usr/sbin:/usr/local/bin:/usr/X11R6/bin:/usr/games:
[root@linux ~]# echo $PATH | cut -d ':' -f 5
# 嘿嘿！如此一来，就会出现 /usr/local/bin 这个目录名称！
# 因为我们是以 : 作为分隔符，第五个就是 /usr/local/bin 啊！
# 那么如果想要列出第 3 与第 5 呢？，就是这样：
[root@linux ~]# echo $PATH | cut -d ':' -f 3,5

范例二：将 export 输出的讯息，取得第 12 字符以后的所有字符串
[root@linux ~]# export
declare -x HISTSIZE="1000"
declare -x INPUTRC="/etc/inputrc"
declare -x KDEDIR="/usr"
declare -x LANG="zh_TW.big5"
......其它省略......
[root@linux ~]# export | cut -c 12-
HISTSIZE="1000"
INPUTRC="/etc/inputrc"
KDEDIR="/usr"
LANG="zh_TW.big5"
......其它省略......
# 知道怎么回事了吧？用 -c 可以处理比较具有格式的输出数据！
# 我们还可以指定某个范围的值，例如第 12-20 的字符，就是 cut -c 12-20 等等！

范例三：用 last 将这个月登入者的信息中，仅留下使用者大名
[root@linux ~]# last
vbird tty1 192.168.1.28 Mon Aug 15 11:55 - 17:48 (05:53)
vbird tty1 192.168.1.28 Mon Aug 15 10:17 - 11:54 (01:37)
[root@linux ~]# last | cut -d ' ' -f 1
# 用 last 可以取得最近一个月登入主机的使用者信息，
# 而我们可以利用空格符的间隔，取出第一个信息，就是使用者账号啰！
# 但是因为 vbird tty1 之间空格有好几个，并非仅有一个，所以，如果要找出
# tty1 其实不能以 cut -d ' ' -f 1,2 喔！输出的结果会不是我们想要的。

这个 cut 实在很好用！不过，说真的，除非你常常在分析 log 档案，否则使用到 cut 的机会并不多！好了！ cut 主要的用途在于将『同一行里面的数据进行分解！』， 最常使用在分析一些数据或文字数据的时候！这是因为有时候我们会以某些字符当作分割的参数，
 然后来将数据加以切割，以取得我们所需要的数据。我也很常使用这个功能呢！尤其是在分析 log 档案的时候！不过， cut 在处理多空格相连的数据时，可能会比较吃力一点～




2) grep
刚刚的 cut 是将一行讯息当中，取出某部分我们想要的，而 grep 则是分析一行讯息， 若当中有我们所需要的信息，就将该行拿出来～简单的语法是这样的：
[root@linux ~]# grep [-acinv] '搜寻字符串' filename
参数：
-a ：将 binary 档案以 text 档案的方式搜寻数据
-c ：计算找到 '搜寻字符串' 的次数
-i ：忽略大小写的不同，所以大小写视为相同
-n ：顺便输出行号
-v ：反向选择，亦即显示出没有 '搜寻字符串' 内容的那一行！

范例一：将 last 当中，有出现 root 的那一行就取出来；
[root@linux ~]# last | grep 'root'

范例二：与范例一相反，只要没有 root 的就取出！
[root@linux ~]# last | grep -v 'root'

范例三：在 last 的输出讯息中，只要有 root 就取出，并且仅取第一栏
[root@linux ~]# last | grep 'root' |cut -d ' ' -f1
# 在取出 root 之后，利用上个指令 cut 的处理，就能够仅取得第一栏啰！

grep 是个很棒的指令喔！他支持的语法实在是太多了～用在正规表示法里头， 能够处理的数据实在是多的很～不过，我们这里先不谈正规表示法～下一章再来说明～ 您先了解一下， grep 可以解析一行文字，取得关键词，若该行有存在关键词， 就会整行列出来！



3) 排序命令： sort, wc, uniq

很多时候，我们都会去计算一次数据里头的相同型态的数据总数，举例来说， 使用 last 可以查得这个月份有登入主机者的身份。那么我可以针对每个使用者查出他们的总登入次数吗？ 此时就得要排序与计算之类的指令来辅助了！底下我们介绍几个好用的排序与统计指令喔！

sort
sort 是很有趣的指令，他可以帮我们进行排序，而且可以依据不同的数据型态来排序喔！ 例如数字与文字的排序就不一样。此外，排序的字符与语系的编码有关，因此， 如果您需要排序时，建议使用 LC_ALL=C 来让语系统一，数据排序比较好一些。
[root@linux ~]# sort [-fbMnrtuk] [file or stdin]
参数：
-f ：忽略大小写的差异，例如 A 与 a 视为编码相同；
-b ：忽略最前面的空格符部分；
-M ：以月份的名字来排序，例如 JAN, DEC 等等的排序方法；
-n ：使用『纯数字』进行排序(预设是以文字型态来排序的)；
-r ：反向排序；
-u ：就是 uniq ，相同的数据中，仅出现一行代表；
-t ：分隔符，预设是 tab 键；
-k ：以那个区间 (field) 来进行排序的意思，
范例一：个人账号都记录在 /etc/passwd 下，请将账号进行排序。
[root@linux ~]# cat /etc/passwd | sort
adm:x:3:4:adm:/var/adm:/sbin/nologin
apache:x:48:48:Apache:/var/www:/sbin/nologin
bin:x:1:1:bin:/bin:/sbin/nologin
daemon:x:2:2:daemon:/sbin:/sbin/nologin
# 我省略很多的输出～由上面的数据看起来， sort 是预设『以第一个』数据来排序
# 而且预设是以『文字』型态来排序的喔！所以由 a 开始排到最后啰！

范例二：/etc/passwd 内容是以 : 来分隔的，我想以第三栏来排序，该如何？


[root@linux ~]# cat /etc/passwd | sort -t ':' -k 3
root:x:0:0:root:/root:/bin/bash
iiimd:x:100:101:IIIMF server:/usr/lib/iiim:/sbin/nologin
uucp:x:10:14:uucp:/var/spool/uucp:/sbin/nologin
operator:x:11:0:operator:/root:/sbin/nologin
bin:x:1:1:bin:/bin:/sbin/nologin
games:x:12:100:games:/usr/games:/sbin/nologin

# 看到特殊字体的输出部分了吧？怎么会这样排列啊？呵呵！没错啦～
# 如果是以文字型态来排序的话，原本就会是这样，想要使用数字排序：
# cat /etc/passwd | sort -t ':' -k 3 -n
# 这样才行啊！用那个 -n 来告知 sort 以数字来排序啊！

范例三：利用 last ，将输出的数据仅取账号，并加以排序

[root@linux ~]# last | cut -d ' ' -f1 | sort

sort 同样是很常用的指令呢！因为我们常常需要比较一些信息啦！ 举个上面的第二个例子来说好了！今天假设你有很多的账号，而且你想要知道最大的使用者 ID 目前到哪一号了！呵呵！使用 sort 一下子就可以知道答案咯！当然其使用还不止此啦！ 有空的话不妨玩一玩！

uniq
如果我排序完成了，想要将重复的资料仅列出一个显示，可以怎么做呢？
[root@linux ~]# uniq [-ic]
参数：
-i ：忽略大小写字符的不同；
-c ：进行计数

范例一：使用 last 将账号列出，仅取出账号栏，进行排序后仅取出一位；
[root@linux ~]# last | cut -d ' ' -f1 | sort | uniq
范例二：承上题，如果我还想要知道每个人的登入总次数呢？
[root@linux ~]# last | cut -d ' ' -f1 | sort | uniq -c

这个指令用来将『重复的行删除掉只显示一个』，举个例子来说， 你要知道这个月份登入你主机的使用者有谁，而不在乎他的登入次数，那么就使用上面的范例， (1)先将所有的数据列出；(2)再将人名独立出来；(3)经过排序；(4)只显示一个！ 由于这个指令是在将重复的东西减少，所以当然需要『配合排序过的档案』来处理啰！

wc
如果我想要知道 /etc/man.config 这个档案里面有多少字？多少行？多少字符的话， 可以怎么做呢？其实可以利用 wc 这个指令来达成喔！他可以帮我们计算输出的讯息的整体数据！
[root@linux ~]# wc [-lwm]
参数：
-l ：仅列出行；
-w ：仅列出多少字(英文单字)；
-m ：多少字符；

范例一：那个 /etc/man.config 里面到底有多少相关字、行、字符数？
[root@linux ~]# cat /etc/man.config | wc
138 709 4506
# 输出的三个数字中，分别代表： 『行、字数、字符数』

范例二：我知道使用 last 可以输出登入者，但是 last 最后两行并非账号内容，那么请问，我该如何以一行指令串取得这个月份登入系统的总人次？
[root@linux ~]# last | grep [a-zA-Z] | grep -v 'wtmp' | wc -l
# 由于 last 会输出空白行与 wtmp 字样在最底下两行，因此，我利用
# grep 取出非空白行，以及去除 wtmp 那一行，在计算行数，就能够了解啰！

wc 也可以当作指令？呵呵！这可不是上洗手间的 WC 呢！ 这是相当有用的计算档案内容的一个工具组喔！举个例子来说， 当你要知道目前你的账号档案中有多少个账号时，就使用这个方法：『 cat /etc/passwd | wc -l 』啦！因为 /etc/passwd 里头一行代表一个使用者呀！ 所以知道行数就晓得有多少的账号在里头了！而如果要计算一个档案里头有多少个字符时，呵呵！就使用 wc -c 这个参数吧！



双向重导向： tee
想个简单的东西，我们由前一节知道 > 会将数据流整个传送给档案或装置， 因此我们除非去读取该档案或装置，否则就无法继续利用这个数据流。 万一我想要将这个数据流的处理过程中，将某段讯息存下来，应该怎么做？呵呵！ 利用 tee 就可以啰 
同时将数据流分送到档案去与屏幕 (screen)；而输出到屏幕的，其实就是 stdout ，可以让下个指令继续处理喔！

[root@linux ~]# tee [-a] file
参数：
-a ：以累加 (append) 的方式，将数据加入 file 当中！

[root@linux ~]# last | tee last.list | cut -d " " -f1
# 这个范例可以让我们将 last 的输出存一份到 last.list 档案中；

[root@linux ~]# ls -l /home | tee ~/homefile | more
# 这个范例则是将 ls 的数据存一份到 ~/homefile ，同时屏幕也有输出讯息！

[root@linux ~]# ls -l / | tee -a ~/homefile | more
# 要注意： tee 后接的档案会被覆盖，所以，我们要加上 -a
# 这个参数才能将讯息累加。

有没有发现在命令重导向的时候，如果我们要将数据送出到档案的时候， 屏幕上就不会出现任何的数据！那么如果我们需要将数据同时显示在屏幕上跟档案中呢？呵呵！这个时候就需要 tee 这个指令啰！使用 last 可以查看到这个月份的登入资料，而使用了
 tee 之后，会将数据同时传给下一个命令去执行，也会将数据写入 last.list 这个档案中！也是个好帮手！





## 字符转换命令： tr, col, join, paste, expand
我们在 vi 文书处理器 章节当中，提到过 DOS 断行字符与 Unix 断行字符的不同， 并且可以使用 dos2unix 与 unix2dos 来完成转换。好了，那么思考一下，是否还有其它常用的字符替代？ 举例来说，要将大写改成小写，或者是 [tab] 按键转成空格键？还有，如何将两篇讯息整合成一篇？ 底下我们就来介绍一下这些字符转换命令在管线当中的使用方法：
tr
tr 可以用来删除一段讯息当中的文字，或者是进行文字讯息的替换！
[root@linux ~]# tr [-ds] SET1 ...
参数：
-d ：删除讯息当中的 SET1 这个字符串；
-s ：取代掉重复的字符！

范例一：将 last 输出的讯息中，所有的小写变成大写字符：
[root@linux ~]# last | tr '[a-z]' '[A-Z]'

范例二：将 /etc/passwd 输出的讯息中，将冒号 (:) 删除
[root@linux ~]# cat /etc/passwd | tr -d ':'

范例三：将 DOS 档案的断行字符 ^M 符号删除：
[root@linux ~]# cat /home/test/dostxt | tr -d '\r' > dostxt-noM

# 那个 /r 指的是 DOS 的断行字符，关于更多的字符，请参考 man tr其实这个指令也可以写在『正规表示法』里头！因为他也是由正规表示法的方式来取代数据的！ 以上面的例子来说，使用 [] 可以设定一串字呢！ 也常常用来取代档案中的怪异符号！ 例如上面第三个例子当中，可以去除 DOS 档案留下来的 ^M 这个断行的符号！这东西相当的有用！相信处理 Linux & Windows 系统中的人们最麻烦的一件事就是这个事情啦！亦即是
 DOS 底下会自动的在每行行尾加入 ^M 这个断行符号！这个时候我们可以使用这个 tr 来将 ^M 去除！ ^M 可以使用 \r 来代替之！

col
[root@linux ~]# col [-x]
参数：
-x ：将 tab 键转换成对等的空格键
范例：
[root@linux ~]# cat -A /etc/man.config <==此时会看到很多 ^I 的符号，那就是 tab
[root@linux ~]# cat /etc/man.config | col -x | cat -A | more
# 嘿嘿！如此一来， [tab] 按键会被取代成为空格键，输出就美观多了！虽然 col 有他特殊的用途，不过，很多时候，他可以用来简单的处理将 [tab] 按键取代成为空格键！ 例如上面的例子当中，如果使用 cat -A 则 [tab] 会以 ^I 来表示。 但经过 col -x 的处理，则会将 [tab] 取代成为对等的空格键！

join

join 看字面上的意义 (加入/参加) 就可以知道，他是在处理两个档案之间的数据， 而且，主要是在处理『两个档案当中，有 "相同数据" 的那一行，将他加在一起』的意思。我们利用底下的简单例子来说明：
[root@linux ~]# join [-ti12] file1 file2
参数：
-t ：join 预设以空格符分隔数据，并且比对『第一个字段』的数据，
如果两个档案相同，则将两笔数据联成一行，且第一个字段放在第一个！
-i ：忽略大小写的差异；
-1 ：这个是数字的 1 ，代表『第一个档案要用那个字段来分析』的意思；
-2 ：代表『第二个档案要用那个字段来分析』的意思。

范例一：用 root 的身份，将 /etc/passwd 与 /etc/shadow 相关数据整合成一栏

[root@linux ~]# join -t ':' /etc/passwd /etc/shadow
bin:x:1:1:bin:/bin:/sbin/nologin:*:12959:0:99999:7:::
daemon:x:2:2:daemon:/sbin:/sbin/nologin:*:12959:0:99999:7:::
adm:x:3:4:adm:/var/adm:/sbin/nologin:*:12959:0:99999:7:::
# 因为 /etc/shadow 的权限问题，所以这里必须是 root 才能动作！而 /etc/passwd
# 与 /etc/shadow 都是以 : 来分隔字段，所以必须要使用 -t ':' 规范字段分隔字符。
# 且，因为 /etc/shadow 与 /etc/passwd 刚好都是以第一个字段为账号名称，所以，
# 就可以将同一行的数据给他贴在一起了！
# 另外，再仔细看一下 /etc/shadow 的内容与 /etc/passwd 的内容，您会发现，
# 两者都以账号为开始，而上面的输出数据中您会发现特殊字体部分，那代表
# 第二个档案的内容。在第二个档案的内容部分，由于账号(第一个字段)与
# 第一的档案是相同的，所以当然就省略掉，因此就成为上面的输出。

范例二：我们知道 /etc/passwd 第四个字段是 GID ，那个 GID 记录在/etc/group 当中的第三个字段，请问如何将两个档案整合？
[root@linux ~]# join -t ':' -1 4 /etc/passwd -2 3 /etc/group

0:root:x:0:root:/root:/bin/bash:root:x:
1:bin:x:1:bin:/bin:/sbin/nologin:bin:x:root,bin,daemon
2:daemon:x:2:daemon:/sbin:/sbin/nologin:daemon:x:root,bin,daemon
4:adm:x:3:adm:/var/adm:/sbin/nologin:adm:x:root,adm,daemon
# 这个例子就更明显了！原本的 /etc/passwd 的第一行内容应该是：
# root:x:0:0:root:/root:/bin/bash
# 至于 /etc/group 第一行内容应该是：
# root:x:0:
# 我将第一个档案的第四栏与第二个档案的第三栏取出，放置到输出的最前方，
# 然后将剩下的数据给他加在一起！就成了上面的输出啦！

这个 join 在处理两个相关的数据文件时，就真的是很有帮助的啦！ 例如上面的案例当中，我的 /etc/passwd, /etc/shadow, /etc/group 都是有相关性的， 其中 /etc/passwd, /etc/shadow 以账号为相关性，至于 /etc/passwd, /etc/group 则以所谓的 GID (账号的数字定义) 来作为他的相关性。根据这个相关性， 我们可以将有关系的资料放置在一起！这在处理数据可是相当有帮助的！
 但是上面的例子有点难，希望您可以静下心好好的看一看原因喔！

paste
这个 paste 就要比 join 简单多了！相对于 join 必须要比对两个档案的数据相关性， paste 就直接『将两行贴在一起，且中间以 [tab] 键隔开』而已！
简单的使用方法：

[root@linux ~]# paste [-d] file1 file2
参数：
-d ：后面可以接分隔字符。预设是以 [tab] 来分隔的！
- ：如果 file 部分写成 - ，表示来自 standard input 的资料的意思。

范例一：将 /etc/passwd 与 /etc/shadow 同一行贴在一起
[root@linux ~]# paste /etc/passwd /etc/shadow
bin:x:1:1:bin:/bin:/sbin/nologin bin:*:12959:0:99999:7:::
daemon:x:2:2:daemon:/sbin:/sbin/nologin daemon:*:12959:0:99999:7:::
adm:x:3:4:adm:/var/adm:/sbin/nologin adm:*:12959:0:99999:7:::
# 注意喔！同一行中间是以 [tab] 按键隔开的！

范例二：先将 /etc/group 读出(用 cat)，然后与范例一贴上一起！且仅取出前三行
[root@linux ~]# cat /etc/group|paste /etc/passwd /etc/shadow -|head -n 3
# 这个例子的重点在那个 - 的使用！那玩意儿常常代表 stdin 喔！

expand
这玩意儿就是在将 [tab] 按键转成空格键啦～可以这样玩：
[root@linux ~]# expand [-t] file
参数：
-t ：后面可以接数字。一般来说，一个 tab 按键可以用 8 个空格键取代。
我们也可以自行定义一个 [tab] 按键代表多少个字符呢！

范例一：将 /etc/man.config 内行首为 MANPATH 的字样就取出；仅取前三行；
[root@linux ~]# grep '^MANPATH' /etc/man.config | head -n 3
MANPATH /usr/man
MANPATH /usr/share/man
MANPATH /usr/local/man
# 行首的代表标志为 ^ ，这个我们留待下节介绍！先有概念即可！

范例二：承上，如果我想要将所有的符号都列出来？(用 cat)
[root@linux ~]# grep '^MANPATH' /etc/man.config | head -n 3 |cat -A
MANPATH^I/usr/man$
MANPATH^I/usr/share/man$
MANPATH^I/usr/local/man$
# 发现差别了吗？没错～ [tab] 按键可以被 cat -A 显示成为 ^I

范例三：承上，我将 [tab] 按键设定成 6 个字符的话？
[root@linux ~]# grep '^MANPATH' /etc/man.config | head -n 3 | \
> expand -t 6 - | cat -A
MANPATH /usr/man$
MANPATH /usr/share/man$
MANPATH /usr/local/man$
123456123456123456.....
# 仔细看一下上面的数字说明，因为我是以 6 个字符来代表一个 [tab] 的长度，所以，
# MAN... 到 /usr 之间会隔 12 (两个 [tab]) 个字符喔！如果 tab 改成 9 的话，
# 情况就又不同了！这里也不好理解～您可以多设定几个数字来查阅就晓得！expand 也是挺好玩的～他会自动将 [tab] 转成空格键～所以，以上面的例子来说， 使用 cat -A 就会查不到 ^I 的字符啰～此外，因为 [tab] 最大的功能就是格式排列整齐！ 我们转成空格键后，这个空格键也会依据我们自己的定义来增加大小～ 所以，并不是一个 ^I 就会换成 8 个空白喔！这个地方要特别注意的哩！ 此外，您也可以参考一下 unexpand
 这个将空白转成 [tab] 的指令功能啊！ ^_^

分割命令： split
如果你有档案太大，导致一些携带式装置无法复制的问题，嘿嘿！找 split 就对了！ 他可以帮你将一个大档案，依据档案大小或行数来分割，就可以将大档案分割成为小档案了！ 快速又有效啊！真不错～
[root@linux ~]# split [-bl] file PREFIX
参数：
-b ：后面可接欲分割成的档案大小，可加单位，例如 b, k, m 等；
-l ：以行数来进行分割。

范例一：我的 /etc/termcap 有七百多K，若想要分成 300K 一个档案时？
[root@linux ~]# cd /tmp; split -b 300k /etc/termcap termcap
[root@linux tmp]# ls -l termcap*
-rw-rw-r-- 1 root root 307200 8月 17 00:25 termcapaa
-rw-rw-r-- 1 root root 307200 8月 17 00:25 termcapab
-rw-rw-r-- 1 root root 184848 8月 17 00:25 termcapac
# 那个档名可以随意取的啦！我们只要写上前导文字，小档案就会以
# xxxaa, xxxab, xxxac 等方式来建立小档案的！

范例二：如何将上面的三个小档案合成一个档案，档名为 termcapback
[root@linux tmp]# cat termcap* >> termcapback
# 很简单吧？就用数据流重导向就好啦！简单！

范例三：使用 ls -al / 输出的信息中，每十行记录成一个档案
[root@linux tmp]# ls -al / | split -l 10 - lsroot
# 重点在那个 - 啦！一般来说，如果需要 stdout/stdin 时，但偏偏又没有档案，
# 有的只是 - 时，那么那个 - 就会被当成 stdin 或 stdout ～

在 Windows 的情况下，你要将档案分割需要如何作？！伤脑筋吧！呵呵！在 Linux 底下就简单的多了！你要将档案分割的话，那么就使用 -b size 来将一个分割的档案限制其大小，如果是行数的话，那么就使用 -l line 来分割！好用的很！如此一来，你就可以轻易的将你的档案分割成 floppy 的大小，方便你 copy 啰！


## 参数代换： xargs
 xargs 可以读入 stdin 的数据，并且以空格符或断行字符作为分辨，将 stdin 的资料分隔成为 arguments 。 因为是以空格符作为分隔，所以，如果有一些档名或者是其它意义的名词内含有空格符的时候， xargs 可能就会误判了～他的用法其实也还满简单的！就来看一看先！

[root@linux ~]# xargs [-0epn] command
参数：
-0 ：如果输入的 stdin 含有特殊字符，例如 `, \, 空格键等等字符时，这个 
-0 参数可以将他还原成一般字符。这个参数可以用于特殊状态喔！
-e ：这个是 EOF (end of file) 的意思。后面可以接一个字符串，当 xargs 分析到这个字符串时，就会停止继续工作！
-p ：在执行每个指令的 argument 时，都会询问使用者的意思；
-n ：后面接次数，每次 command 指令执行时，要使用几个参数的意思。看范例三。
当 xargs 后面没有接任何的指令时，预设是以 echo 来进行输出喔！

范例一：将 /etc/passwd 内的第一栏取出，仅取三行，使用 finger 这个指令将每个账号内容秀出来
[root@linux ~]# cut -d':' -f1 < /etc/passwd |head -n 3| xargs finger
Login: root Name: root
Directory: /root Shell: /bin/bash
Never logged in.
No mail.
No Plan.
......底下省略.....
# 由 finger account 可以取得该账号的相关说明内容，例如上面的输出就是 finger root
# 后的结果。在这个例子当中，我们利用 cut 取出账号名称，用 head 取出三个账号，
# 最后则是由 xargs 将三个账号的名称变成 finger 后面需要的参数！

范例二：同上，但是每次执行 finger 时，都要询问使用者是否动作？
[root@linux ~]# cut -d':' -f1 < /etc/passwd |head -n 3| xargs -p finger
finger root bin daemon ?...y
......底下省略.....
# 呵呵！这个 -p 的参数有趣了吧？！他可以让使用者的使用过程中，被询问到每个
# 指令是否执行！

范例三：将所有的 /etc/passwd 内的账号都以 finger 查阅，但一次仅查阅五个账号
[root@linux ~]# cut -d':' -f1 < /etc/passwd | xargs -p -n 5 finger
finger root bin daemon adm lp ?...y
......底下省略.....
# 在这里鸟哥使用了 -p 这个参数来让您对于 -n 更有概念。一般来说，某些指令后面
# 可以接的 arguments 是有限制的，不能无限制的累加，此时，我们可以利用 -n
# 来帮助我们将参数分成数个部分，每个部分分别再以指令来执行！这样就 OK 啦！^_^
[root@linux ~]#

范例四：同上，但是当分析到 lp 就结束这串指令？
[root@linux ~]# cut -d':' -f1 < /etc/passwd | xargs -p -e'lp' finger
finger root bin daemon adm ?...
# 仔细与上面的案例做比较。也同时注意，那个 -e'lp' 是连在一起的，中间没有空格键。
# 上个例子当中，第五个参数是 lp 啊，那么我们下达 -e'lp' 后，则分析到 lp
# 这个字符串时，后面的其它 stdin 的内容就会被 xargs 舍弃掉了！

其实，在 man xargs 里面就有三四个小范例，您可以自行参考一下内容。 此外， xargs 真的是很好用的一个玩意儿！您真的需要好好的参详参详！


## 关于减号 - 的用途
管线命令在 bash 的连续的处理程序中是相当重要的！另外，在 log file 的分析当中也是相当重要的一环， 所以请特别留意！另外，在管线命令当中，常常会使用到前一个指令的 stdout 作为这次的 stdin ， 某些指令需要用到文件名称 (例如 tar)
 来进行处理时，该 stdin 与 stdout 可以利用减号 "-" 来替代， 举例来说：

[root@linux ~]# tar -cvf - /home | tar -xvf -
上面这个例子是说：『我将 /home 里面的档案给他打包，但打包的数据不是纪录到档案，而是传送到 stdout； 经过管线后，将 tar -cvf - /home 传送给后面的 tar -xvf - 』。后面的这个 - 则是取用前一个指令的 stdout， 因此，我们就不需要使用 file 了！这是很常见的例子喔！注意注意！







## Bash shell（1-6）　　例题
在 Linux 上可以找到哪些 shell(举出三个) ？那个档案记录可用的 shell ？而 Linux 预设的 shell 是？
1) /bin/bash, /bin/tcsh, /bin/csh 2) /etc/shells 3) bash ，亦即是 /bin/bash。

在 shell 环境下，有个提示字符 (prompt)，他可以修改吗？要改什么？预设的提示字符内容是？
可以修改的，改 PS1 这个变量，这个 PS1 变量的预设内容为：『[\u@\h \W]\$』
如何显示 HOME 这个环境变量？
echo $HOME

如何得知目前的所有变量与环境变量的设定值？
环境变量用 env 而所有变量用 set 即可显示

我是否可以设定一个变量名称为 3myhome ？
不行！变量不能以数字做为开头，参考变量设定规则的内容

在这样的练习中『A=B』且『B=C』，若我下达『unset $A』，则取消的变数是 A 还是 B？
被取消的是 B 喔，因为 unset $A 相当于 unset B 所以取消的是 B ，A 会继续存在！

如何取消变量与命令别名的内容？
使用 unset 及 unalias 即可

如何设定一个变量名称为 name 内容为 It's my name ？
name=It\'s\ my\ name 或 name="It's my name"


环境变量档案的加载顺序？
先由 /etc/passwd 取得 bash 这个 shell ，再到 /etc/profile 读取主要的环境变量，同时亦会将 /etc/inputrc 及 /etc/profile.d 内容均读入。之后，再到个人的家目录读取 ~/.bash_profile 及 ~/.bashrc 等档案！

man page 的路径设定档案？
/etc/man.config 或 /etc/man.conf

试说明 ', ", 与 ` 这些符号在变量定义中的用途？
参考变量规则那一章节，其中， " 可以具有变量的内容属性， ' 则仅有一般字符，至于 ` 之内则是可先被执行的指令。

跳脱符号 \ 有什么用途？
可以用来跳脱特殊字符，例如 Enter, $ 等等，使成为一般字符！

连续命令中， ;, &&, || 有何不同？
分号可以让两个 command 连续运作，不考虑 command1 的输出状态， && 则前一个指令必需要没有错误讯息，亦即回传值需为 0 则 command2 才会被执行， || 则与 && 相反！

如何将 last 的结果中，独立出账号，并且印出本月份曾经登入过的账号？
last | cut –d “ “ –f1 | sort | uniq

请问 foo1 && foo2 | foo3 > foo4 ，这个指令串当中， foo1/foo2/foo3/foo4 是指令还是档案？ 整串指令的意义为？
foo1/foo2 与 foo3 都是指令， foo4 是装置或档案。整串指令意义为：
1. 当 foo1 执行结果有错误时，则该指令串结束；
2. 若 foo1 执行结果没有错误时，则执行 foo2 | foo3 > foo4 ；
1. foo2 将 stdout 输出的结果传给 foo3 处理；
2. foo3 将来自 foo2 的 stdout 当成 stdin ，处理完后将数据流重新导向 foo4 这个装置/档案

如何秀出在 /bin 底下任何以 a 为开头的档案文件名的详细资料？
ls -l /bin/a*

如何秀出 /bin 底下，文件名为四个字符的档案？
ls -l /bin/????

如何秀出 /bin 底下，档名开头不是 a-d 的档案？
ls -l /bin/[!a-d]*

当我离开 bash 后，希望系统可以帮我将最近工作的：1)工作日期； 2)100 个历史命令独立 记录到 ~/.bash_localcom 档案中，该如何设定？
我可以编辑 ~/.bash_logout ，将这个档案内容变成： # ~/.bash_logout date >> ~/.bash_localcom history 100 >> ~/.bash_localcom clear

我想要让终端机接口的登入提示字符修改成我自己喜好的模样，应该要改哪里？(filename)
/etc/issue

承上题，如果我是想要让使用者登入后，才显示欢迎讯息，又应该要改哪里？
/etc/motd




