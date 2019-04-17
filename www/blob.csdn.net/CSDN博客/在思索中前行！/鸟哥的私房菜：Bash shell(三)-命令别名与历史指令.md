# 鸟哥的私房菜：Bash shell(三)-命令别名与历史指令 - 在思索中前行！ - CSDN博客





2014年09月24日 20:35:09[_Tham](https://me.csdn.net/txl16211)阅读数：578










## 一  命令别名设定： alias, unalias
命令别名是一个很有趣的东西，特别是你的惯用指令特别长的时候！还有， 增设预设的属性在一些惯用的指令上面，可以预防一些不小心误杀档案的情况发生的时候！ 举个例子来说，如果你要查询隐藏档，并且需要长的列出与一页一页翻看，那么需要下达『 ls -al | more 』这个指令，我是觉得很烦啦！ 要输入好几个单字！那可不可以使用 lm 来简化呢？！当然可以，你可以在命令列下面下达：
[root@linux ~]# alias lm='ls -l | more'

要注意的是：『alias 的定义规则与变量定义规则几乎相同』， 另外，命令别名的设定还可以取代既有的指令喔！举例来说，我们知道 root 可以移除( rm )任何数据！所以当你以 root 的身份在进行工作时，需要特别小心， 但是总有失手的时候，那么 rm 提供了一个参数来让我们确认是否要移除该档案，那就是 -i 这个参数！所以，你可以这样做：
[root@linux ~]# alias rm='rm -i'

嘿嘿！那么以后使用 rm 的时候，就不用太担心会有错误删除的情况了！这也是命令别名的优点啰！ 那么如何知道目前有哪些的命令别名呢？就使用 alias ！
[root@linux ~]# alias
alias l.='ls -d .* --color=tty'
alias ll='ls -l --color=tty'
alias lm='ls -al | more'
alias ls='ls --color=tty'
alias vi='vim'
alias which='alias | /usr/bin/which --tty-only --read-alias --show-dot --show-tilde'

由上面的资料当中，您也会发现一件事情啊，我们在 vi 文书编辑器 里面提到 vi 与 vim 是不太一样的， vi 是比较老，而 vim 可以用来取代 vi 喔。我们的 FC4 明明就同时有 vi/vim ， 为何我执行 vi 会是进入 vim 呢？呵呵！那就是因为上面的表格当中的『 alias vi='vim' 』这个设定啦！
 至于如果要取消命令别名的话，那么就使用 unalias 吧！例如要将刚刚的 lm 命令别名拿掉，就使用：

[root@linux ~]# unalias lm






## 二  历史命令：history
前面我们提过 bash 有提供指令历史的服务！那么如何查询我们曾经下达过的指令呢？就使用 history 啰！当然，如果觉得 histsory 要输入的字符太多太麻烦，可以使用命令别名来设定呢！ 不要跟我说还不会设定呦！ 
alias h='history'
如此则输入 h 等于输入 history 啰！好了，我们来谈一谈 history 的用法吧！
[root@linux ~]# history [n]
[root@linux ~]# history [-c]
[root@linux ~]# history [-raw] histfiles

参数：
n ：数字，意思是『要列出最近的 n 笔命令列表』的意思！
-c ：将目前的 shell 中的所有 history 内容全部消除
-a ：将目前新增的 history 指令新增入 histfiles 中，若没有加 histfiles ，则预设写入 ~/.bash_history
-r ：将 histfiles 的内容读到目前这个 shell 的 history 记忆中；
-w ：将目前的 history 记忆内容写入 histfiles 中！

范例一：列出目前内存内的所有 history 记忆
[root@linux ~]# history
# 前面省略
1017 man bash
1018 ll
1019 history
1020 history
# 列出的信息当中，共分两栏，第一栏为该指令在这个 shell 当中的代码，
# 另一个则是指令本身的内容喔！至于会秀出几笔指令记录，则与 HISTSIZE 有关！

范例二：列出目前最近的 3 笔资料
[root@linux ~]# history 3
1019 history
1020 history
1021 history 3

范例三：立刻将目前的资料写入 histfile 当中
[root@linux ~]# history -w
# 在预设的情况下，会将历史纪录写入 ~/.bash_history 当中！
[root@linux ~]# echo $HISTSIZE
1000

在正常的情况下，当我们以 bash 登入 Linux 主机之后，系统会主动的由家目录的 ~/.bash_history 读取以前曾经下过的指令，那么 ~/.bash_history 会记录几笔数据呢？这就与你 bash 的 HISTSIZE 这个变量设定值有关了！在预设的 FC4 底下，是会记录 1000
 笔数据的！ 那么假设我这次登入主机后，共下达过 100 次指令，『等我注销时， 系统就会将 101~1100 这总共 1000 笔历史命令更新到 ~/.bash_history 当中。』 也就是说，历史命令在我注销时，会将最近的 HISTSIZE 笔记录到我的纪录文件当中啦！ 当然，也可以用 history -w 强制立刻写入的！那为何用『更新』两个字呢？ 因为 ~/.bash_history 记录的笔数永远都是 HISTSIZE 那么多，旧的讯息会被主动的拿掉！ 仅保留最新的！ 那么 history 这个历史命令只可以让我查询命令而已吗？呵呵！当然不止啊！
 我们可以利用相关的功能来帮我们执行命令呢！举例来说啰：

[root@linux ~]# !number
[root@linux ~]# !command
[root@linux ~]# !!
参数：
number ：执行第几笔指令的意思；
command ：由最近的指令向前搜寻『指令串开头为 command』的那个指令，并执行；
!! ：就是执行上一个指令(相当于按↑按键后，按 Enter)

范例：
[root@linux ~]# history
66 man rm
67 alias
68 man history
69 history
[root@linux ~]# !66 <==执行第 66 笔指令
[root@linux ~]# !! <==执行上一个指令，本例中亦即 !66
[root@linux ~]# !al <==执行最近以 al 为开头的指令(上头列出的第 67 个)

经过上面的介绍，瞭乎？历史命令用法可多了！如果我想要执行上一个指令， 除了使用上下键之外，我可以直接以『 !! 』 来下达上个指令的内容，此外， 我也可以直接选择下达第 n 个指令，『 !n
 』来执行，也可以使用指令标头，例如 『 !vi 』来执行最近指令开头是 vi 的指令列！





