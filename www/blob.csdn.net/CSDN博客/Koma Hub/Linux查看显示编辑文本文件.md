# Linux查看显示编辑文本文件 - Koma Hub - CSDN博客
2017年11月29日 21:31:28[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：2027
**1.cat**
```
-bash-4.1$ cat --help
用法：cat [选项]... [文件]...
将[文件]或标准输入组合输出到标准输出。
  -A, --show-all           等于-vET
  -b, --number-nonblank    对非空输出行编号
  -e                       等于-vE
  -E, --show-ends          在每行结束处显示"$"
  -n, --number             对输出的所有行编号
  -s, --squeeze-blank      不输出多行空行
  -t                       与-vT 等价
  -T, --show-tabs          将跳格字符显示为^I
  -u                       (被忽略)
  -v, --show-nonprinting   使用^ 和M- 引用，除了LFD和 TAB 之外
      --help		显示此帮助信息并退出
      --version		显示版本信息并退出
如果没有指定文件，或者文件为"-"，则从标准输入读取。
示例：
  cat f - g  先输出f 的内容，然后输出标准输入的内容，最后输出g 的内容。
  cat        将标准输入的内容复制到标准输出。
```
**使用示例：**
```
-bash-4.1$ cat run.sh 
#!/bin/bash
gcc -o a resamp.c
./a
#ximage < SEG_C3NA_Velocity_resamp.dat n1=201
pscube <SEG_C3NA_Velocity_resamp.dat n1=201 n2=300 n3=300 >v.eps
gimp v.eps
-bash-4.1$
```
**2.head（查看文件开头）**
```
-bash-4.1$ head --help
用法：head [选项]... [文件]...
将每个指定文件的头10 行显示到标准输出。
如果指定了多于一个文件，在每一段输出前会给出文件名作为文件头。
如果不指定文件，或者文件为"-"，则从标准输入读取数据。
长选项必须使用的参数对于短选项时也是必需使用的。
  -c,  --bytes=[-]K	显示每个文件的前K 字节内容；
			如果附加"-"参数，则除了每个文件的最后K字节数据外
			显示剩余全部内容
  -n, --lines=[-]K	显示每个文件的前K 行内容；
			如果附加"-"参数，则除了每个文件的最后K 行外显示
			剩余全部内容
  -q, --quiet, --silent	不显示包含给定文件名的文件头
  -v, --verbose		总是显示包含给定文件名的文件头
      --help		显示此帮助信息并退出
      --version		显示版本信息并退出
K 后面可以跟乘号:
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,
GB 1000*1000*1000, G 1024*1024*1024, 对于T, P, E, Z, Y 同样适用。
```
**head使用示例：**
```
-bash-4.1$ head rm.sh 
#!/bin/bash
rm *.class
rm */*.class
rm */*/*.class
rm */*/*/*.class
rm */*/*/*/*.class
rm */*/*/*/*/*.class
rm */*/*/*/*/*/*.class
rm */*/*/*/*/*/*/*.class
-bash-4.1$
```
**3.tail （查看文本文件末端）**
```
-bash-4.1$ tail --help
用法：tail [选项]... [文件]...
显示每个指定文件的最后10 行到标准输出。
若指定了多于一个文件，程序会在每段输出的开始添加相应文件名作为头。
如果不指定文件或文件为"-" ，则从标准输入读取数据。
长选项必须使用的参数对于短选项时也是必需使用的。
  -c, --bytes=K		输出最后K 字节；另外，使用-c +K 从每个文件的
			第K 字节输出
  -f, --follow[={name|descriptor}]
		即时输出文件变化后追加的数据。
			-f, --follow 等于--follow=descriptor 
  -F		即--follow=name --retry
  -n, --lines=K            output the last K lines, instead of the last 10;
                           or use -n +K to output lines starting with the Kth
      --max-unchanged-stats=N
                           with --follow=name, reopen a FILE which has not
                           changed size after N (default 5) iterations
                           to see if it has been unlinked or renamed
                           (this is the usual case of rotated log files).
                           With inotify, this option is rarely useful.
      --pid=PID		同 -f 一起使用，当 PID 所对应的进程死去后终止
  -q, --quiet, --silent	不输出给出文件名的头
      --retry		即使目标文件不可访问依然试图打开；在与参数
			--follow=name 同时使用时常常有用。
  -s, --sleep-interval=N   with -f, sleep for approximately N seconds
                             (default 1.0) between iterations.
                           With inotify and --pid=P, check process P at
                           least once every N seconds.
  -v, --verbose            always output headers giving file names
      --help		显示此帮助信息并退出
      --version		显示版本信息并退出
```
**使用示例：**
```
-bash-4.1$ tail rm.sh 
rm */*/*/*/*/*~
rm */*/*/*/*/*/*~
rm */*/*/*/*/*/*/*~
echo "# # #"
ls
echo "# # #"
pwd
echo "# # #"
-bash-4.1$
```
**4.more分页显示文件内容**
```
-bash-4.1$ more rm.sh 
#!/bin/bash
rm *.class
rm */*.class
rm */*/*.class
rm */*/*/*.class
rm */*/*/*/*.class
rm */*/*/*/*/*.class
rm */*/*/*/*/*/*.class
rm */*/*/*/*/*/*/*.class
rm *.pyc
rm */*.pyc
rm */*/*.pyc
rm */*/*/*.pyc
--More--(29%)
```
**5.less分页查看文件内容**
```
SUMMARY OF LESS COMMANDS
      Commands marked with * may be preceded by a number, N.
      Notes in parentheses indicate the behavior if N is given.
  h  H                 Display this help.
  q  :q  Q  :Q  ZZ     Exit.
 ---------------------------------------------------------------------------
                           MOVING
  e  ^E  j  ^N  CR  *  Forward  one line   (or N lines).
  y  ^Y  k  ^K  ^P  *  Backward one line   (or N lines).
  f  ^F  ^V  SPACE  *  Forward  one window (or N lines).
  b  ^B  ESC-v      *  Backward one window (or N lines).
  z                 *  Forward  one window (and set window to N).
  w                 *  Backward one window (and set window to N).
  ESC-SPACE         *  Forward  one window, but don't stop at end-of-file.
  d  ^D             *  Forward  one half-window (and set half-window to N).
  u  ^U             *  Backward one half-window (and set half-window to N).
  ESC-)  RightArrow *  Left  one half screen width (or N positions).
  ESC-(  LeftArrow  *  Right one half screen width (or N positions).
  F                    Forward forever; like "tail -f".
  r  ^R  ^L            Repaint screen.
  R                    Repaint screen, discarding buffered input.
        ---------------------------------------------------
        Default "window" is the screen height.
        Default "half-window" is half of the screen height.
 ---------------------------------------------------------------------------
                          SEARCHING
HELP -- Press RETURN for more, or q when done
```
**使用示例：**
```
$less rm.sh
#!/bin/bash
rm *.class
rm */*.class
rm */*/*.class
rm */*/*/*.class
rm */*/*/*/*.class
rm */*/*/*/*/*.class
rm */*/*/*/*/*/*.class
rm */*/*/*/*/*/*/*.class
rm *.pyc
rm */*.pyc
rm */*/*.pyc
rm */*/*/*.pyc
rm.sh
```
**5.grep**
Linux系统中grep命令是一种强大的文本搜索工具，它能使用正则表达式搜索文本，并把匹 配的行打印出来。grep全称是Global Regular Expression
 Print，表示全局正则表达式版本，它的使用权限是所有用户。
```
-bash-4.1$ grep --help
用法: grep [选项]... PATTERN [FILE]...
在每个 FILE 或是标准输入中查找 PATTERN。
默认的 PATTERN 是一个基本正则表达式(缩写为 BRE)。
例如: grep -i 'hello world' menu.h main.c
正则表达式选择与解释:
  -E, --extended-regexp     PATTERN 是一个可扩展的正则表达式(缩写为 ERE)
  -F, --fixed-strings       PATTERN 是一组由断行符分隔的定长字符串。
  -G, --basic-regexp        PATTERN 是一个基本正则表达式(缩写为 BRE)
  -P, --perl-regexp         PATTERN 是一个 Perl 正则表达式
  -e, --regexp=PATTERN      用 PATTERN 来进行匹配操作
  -f, --file=FILE           从 FILE 中取得 PATTERN
  -i, --ignore-case         忽略大小写
  -w, --word-regexp         强制 PATTERN 仅完全匹配字词
  -x, --line-regexp         强制 PATTERN 仅完全匹配一行
  -z, --null-data           一个 0 字节的数据行，但不是空行
Miscellaneous:
  -s, --no-messages         suppress error messages
  -v, --invert-match        select non-matching lines
  -V, --version             display version information and exit
      --help                display this help text and exit
输出控制:
  -m, --max-count=NUM       NUM 次匹配后停止
  -b, --byte-offset         输出的同时打印字节偏移
  -n, --line-number         输出的同时打印行号
      --line-buffered       每行输出清空
  -H, --with-filename       为每一匹配项打印文件名
  -h, --no-filename         输出时不显示文件名前缀
      --label=LABEL         将LABEL 作为标准输入文件名前缀
  -o, --only-matching       show only the part of a line matching PATTERN
  -q, --quiet, --silent     suppress all normal output
      --binary-files=TYPE   assume that binary files are TYPE;
                            TYPE is 'binary', 'text', or 'without-match'
  -a, --text                equivalent to --binary-files=text
  -I                        equivalent to --binary-files=without-match
  -d, --directories=ACTION  how to handle directories;
                            ACTION is 'read', 'recurse', or 'skip'
  -D, --devices=ACTION      how to handle devices, FIFOs and sockets;
                            ACTION is 'read' or 'skip'
  -r, --recursive           like --directories=recurse
  -R, --dereference-recursive
                            likewise, but follow all symlinks
      --include=FILE_PATTERN
                            search only files that match FILE_PATTERN
      --exclude=FILE_PATTERN
                            skip files and directories matching FILE_PATTERN
      --exclude-from=FILE   skip files matching any file pattern from FILE
      --exclude-dir=PATTERN directories that match PATTERN will be skipped.
  -L, --files-without-match print only names of FILEs containing no match
  -l, --files-with-matches  print only names of FILEs containing matches
  -c, --count               print only a count of matching lines per FILE
  -T, --initial-tab         make tabs line up (if needed)
  -Z, --null                print 0 byte after FILE name
文件控制:
  -B, --before-context=NUM  打印以文本起始的NUM 行
  -A, --after-context=NUM   打印以文本结尾的NUM 行
  -C, --context=NUM         打印输出文本NUM 行
  -NUM                      same as --context=NUM
      --group-separator=SEP use SEP as a group separator
      --no-group-separator  use empty string as a group separator
      --color[=WHEN],
      --colour[=WHEN]       use markers to highlight the matching strings;
                            WHEN is 'always', 'never', or 'auto'
  -U, --binary              do not strip CR characters at EOL (MSDOS/Windows)
  -u, --unix-byte-offsets   report offsets as if CRs were not there
                            (MSDOS/Windows)
```
**使用示例：**
```
$ grep ‘test’ d*
显示所有以d开头的文件中包含 test的行。
$ grep ‘test’ aa bb cc
显示在aa，bb，cc文件中匹配test的行。
$ grep ‘[a-z]\{5\}’ aa
显示所有包含每个字符串至少有5个连续小写字符的字符串的行。
$ grep ‘w\(es\)t.*\1′ aa
如果west被匹配，则es就被存储到内存中，并标记为1，然后搜索任意个字符(.*)，这些字符后面紧跟着 另外一个es(\1)，找到就显示该行。如果用egrep或grep -E，就不用”\”号进行转义，直接写成’w(es)t.*\1′就可以了。
```
```
-bash-4.1$ grep "clas" rm.sh 
rm *.class
rm */*.class
rm */*/*.class
rm */*/*/*.class
rm */*/*/*/*.class
rm */*/*/*/*/*.class
rm */*/*/*/*/*/*.class
rm */*/*/*/*/*/*/*.class
-bash-4.1$
```
```
-bash-4.1$ grep "float "  conio.h resamp.c 
resamp.c:    float *v = (float*)malloc(sizeof(float)*nx*ny*nz);
-bash-4.1$
```
**6.gedit 、 vi 、 vim文本编辑器**
这个不多讲，讲的话可以写本书了，有人说vim就是神一样的程序员敲代码的编辑器，有人说ecilpse完爆vim，仁者见仁智者见智吧，我就喜欢gedit，哪怕都说我太垃圾了，没错，我就是垃圾。

