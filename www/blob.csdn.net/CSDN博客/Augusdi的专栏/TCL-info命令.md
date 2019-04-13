
# TCL - info命令 - Augusdi的专栏 - CSDN博客


2015年10月13日 18:08:19[Augusdi](https://me.csdn.net/Augusdi)阅读数：3311


﻿﻿
**info**命令提供了查看TCL解释器信息的手段，它有超过一打的选项，详细说明请参考下面几节。
#### ■
#### 变量信息
**info**命令的几个选项提供了查看变量信息的手段。
**info****exists***varName*如果名为*varName*的变量在当前上下文(作为全局或局部变量)存在，返回1，否则返回0。
**info****globals**?*pattern*? 如果没有pattern参数，那么返回包含所有全局变量名字的一个list。如果有pattern参数，就只返回那些和pattern匹配的全局变量(匹配的方式和**string match**相同)。
**info****locals**?*pattern*? 如果没有pattern参数，那么返回包含所有局部变量(包括当前过程的参数)名字的一个list，global和upvar命令定义的变量将不返回。如果有pattern参数，就只返回那些和pattern匹配的局部变量(匹配的方式和**string match**相同)。
**info****vars**?*pattern*? 如果没有pattern参数，那么返回包括局部变量和可见的全局变量的名字的一个list。如果有pattern参数，就只返回和模式pattern匹配的局部变量和可见全局变量。模式中可以用namespace来限定范围，如:foo::option*，就只返回namespace中和option*匹配的局部和全局变量。 (注：tcl80以后引入了namespace概念，不过我们一般编写较小的TCL程序，可以对namespace不予理睬，用兴趣的话可以查找相关资料。)
下面针对上述命令举例，假设存在全局变量global1和global2，并且有下列的过程存在:
proc test {arg1 arg2} {
global global1
set local1 1
set local2 2
...
}
然后在过程中执行下列命令:
% info vars
global1 arg1 arg2 local2 local1 //global2不可见
% info globals
global2 global1
% info locals
arg1 arg2 local2 local1
% info vars *al*
global1 local2 local1
#### ■
#### 过程信息
**info**命令的另外的一些选项可以查看过程信息。
**info****procs**?*pattern*? 如果没有pattern参数，命令返回当前namespace中定义的所有过程的名字。如果有pattern参数，就只返回那些和pattern匹配的过程的名字(匹配的方式和**string match**相同)。
**info****body***procname*返回过程*procname*的过程体。*procname*必须是一个TCL过程。
**info****args***procname*返回包含过程*procname*的所有参数的名字的一个list。*procname*必须是一个TCL过程。
**info****default***procname arg varname procname*必须是一个TCL过程，*arg*必须是这个过程的一个变量。如果*arg*没有缺省值，命令返回0；否则返回1，并且把*arg*的缺省值赋给变量*varname*。
**info****level**?number? 如果没有number参数，这个命令返回当前过程在调用栈的位置。如果有number参数，那么返回的是包含在调用栈的位置为number的过程的过程名及其参数的一个list。
下面针对上述命令举例:
proc maybeprint {a b {c 24}} {
if {$a<$b} {
puts stdout "c is $c"
}
}
% info body maybeprint
if {$a<$b} {
puts stdout "c is $c"
}
% info args maybeprint
a b c
% info default maybeprint a x
0
% info default maybeprint a c
1
%set x
24
下面的过程打印出了当前的调用栈，并显示了每一个活动过程名字和参数：
proc printStack{}{
set level [info level]
for {set i 1} {$i<$level} {incr i} {
puts "Level $i:[info level $i]"
}
}
#### ■
#### 命令信息
**info**命令的另外选项可以查看命令信息。
**info****commands**?*pattern*? 如果没有参数*pattern*，这个命令返回包含当前namspace中所有固有、扩展命令以及以**proc**命令定义的过程在内的所有命令的名字的一个list。*pattern*参数的含义和**info procs**一样。
**info****cmdcount**返回了一个十进制字符串，表明多少个命令曾在解释器中执行过。
**info****complete***command*如果命令是*command*完整的，那么返回1，否则返回0。这里判断命令是否完整仅判断引号，括号和花括号是否配套。
**info****script**如果当前有脚本文件正在Tcl解释器中执行，则返回最内层处于激活状态的脚本文件名；否则将返回一个空的字符串。
#### ■
#### TCL
#### 的版本和库
**info****tclversion**返回为Tcl解释器返回的版本号，形式为major.minor，例如8.3。
**info****library**返回Tcl库目录的完全路径。这个目录用于保存Tcl所使用的标准脚本，TCL在初始化时会执行这个目录下的脚本。
#### ■
#### 命令的执行时间
TCL提供time命令来衡量TCL脚本的性能：
**time**script ?count? 这个命令重复执行script脚本count次。再把花费的总时间的用count除，返回一次的平均执行时间，单位为微秒。如果没有count参数，就取执行一次的时间。
#### ■
#### 跟踪变量
TCL提供了**trace**命令来跟踪一个或多个变量。如果已经建立对一个变量的跟踪，则不论什么时候对该变量进行了读、写、或删除操作，就会激活一个对应的Tcl命令，跟踪可以有很多的用途：
1.监视变量的用法（例如打印每一个读或写的操作）。
2.把变量的变化传递给系统的其他部分（例如一个TK程序中，在一个小图标上始终显示某个变量的当前值）。
3.限制对变量的某些操作（例如对任何试图用非十进制数的参数来改变变量的值的行为产生一个错误。）或重载某些操作（例如每次删除某个变量时，又重新创建它）。
**trace**命令的语法为：
**trace**option ?arg arg ...?
其中option有以下几种形式：
**trace****variable***name ops command*这个命令设置对变量name的一个跟踪：每次当对变量*name*作ops操作时，就会执行*command*命令。name可以是一个简单变量，也可以是一个数组的元素或者整个数组。
ops可以是以下几种操作的一个或几个的组合：
**r**当变量被读时激活command命令。
**w**当变量被写时激活command命令。
**u**当变量被删除时激活command命令。通过用unset命令可以显式的删除一个变量，一个过程调用结束则会隐式的删除所有局部变量。当删除解释器时也会删除变量，不过这时跟踪已经不起作用了。
当对一个变量的跟踪被触发时，TCL解释器会自动把三个参数添加到命令command的参数列表中。这样command实际上变成了
command name1 name2 op
其中op指明对变量作的什么操作。name1和name2用于指明被操作的变量：如果变量是一个标量，那么name1给出了变量的名字，而name2是一个空字符串；如果变量是一个数组的一个元素，那么name1给出数组的名字，而name2给出元素的名字；如果变量是整个数组，那么name1给出数组的名字而name2是一个空字符串。为了让你很好的理解上面的叙述，下面举一个例子：
trace variable color w pvar
trace variable a(length) w pvar
proc pvar {name element op} {
if {$element !=""} {
set name ${name}($element)
}
upvar$name x
puts"Variable $name set to $x"
}
上面的例子中，对标量变量color和数组元素a(length)的写操作都会激活跟踪操作pvar。我们看到过程pvar有三个参数，这三个参数TCL解释器会在跟踪操作被触发时自动传递给pvar。比如如果我们对color的值作了改变，那么激活的就是pvar color "" w。我们敲入：
% set color green
Variable color set to green
green
command将在和触发跟踪操作的代码同样的上下文中执行：如果对被跟踪变量的访问是在一个过程中，那么command就可以访问这个过程的局部变量。比如：
proc Hello { } {
set a 2
trace variable b w { puts $a ;list }
set b 3
}
% Hello
2
3
对于被跟踪变量的读写操作，command是在变量被读之后，而返回变量的值之前被执行的。因此，我们可以在command对变量的值进行改变，把新值作为读写的返回值。而且因为在执行command时，跟踪机制会临时失效，所以在command中对变量进行读写不会导致command被递归激活。例如：
% trace variable b r tmp
% proc tmp {var1 var2 var3 } {
upvar$var1 t
incrt 1
}
% set b 2
2
% puts $b
3
% puts $b
4
如果对读写操作的跟踪失败，即command失败，那么被跟踪的读写操作也会失败，并且返回和command同样的失败信息。利用这个机制可以实现只读变量。下面这个例子实现了一个值只能为正整数的变量：
trace variable size w forceInt
proc forceInt {name element op} {
upvar$name x
if ![regexp {^[0-9]*$} $x] {
error "value must b a postive integer"
}
}
如果一个变量有多个跟踪信息，那么各个跟踪被触发的先后原则是：最近添加的跟踪最先被触发，如果有一个跟踪发生错误，后面的跟踪就不会被触发。
**trace****vdelete***name ops command*删除对变量name的ops操作的跟踪。返回值为一个空字符串。
**trace****vinfo***name*这个命令返回对变量的跟踪信息。返回值是一个list，list的每个元素是一个子串，每个子串包括两个元素：跟踪的操作和与操作关联的命令。如果变量name不存在或没有跟踪信息，返回一个空字符串。
#### ■
#### 命令的重命名和删除
rename 命令可以用来重命名或删除一个命令。
**rename***oldName newName*把命令oldName改名为newName，如果newName为空，那么就从解释器中删除命令oldName。
下面的脚本删除了文件I/O命令：
foreachcmd {open close read gets puts} {
rename $cmd {}
}
任何一个Tcl命令都可以被重命名或者删除，包括内建命令以及应用中定义的过程和命令。重命名一个内建命令可能会很有用，例如，exit命令在Tcl中被定义为立即退出过程。如果某个应用希望在退出前获得清除它内部状态的机会，那么可以这样作：
rename exit exit.old
proc exit status {
application-specific cleanup
...
exit.old$status
}
在这个例子中，exit命令被重命名为exit.old，并且定义了新的exit命令，这个新命令作了应用必需的清除工作而后调用了改了名字的exit命令来结束进程。这样在已存在的描述程序中调用exit时就会有机会做清理应用状态的工作。
#### ■
#### unknown命令
**unknown**命令的语法为：
**unknown***cmdName*?*arg arg*...? 当一个脚本试图执行一个不存在的命令时，TCL解释器会激活**unknown**命令，并把那个不存在的命令的名字和参数传递给**unknown**命令。**unknown**命令不是TCL的核心的一部分，它是由TCL脚本实现的，可以在TCL安装目录的lib子目录下的init.tcl文件中找到其定义。
**unknown**命令具有以下功能：
1。如果命令是一个在TCL的某个库文件(这里的库文件指的是TCL目录的lib子目录下的TCL脚本文件)中定义的过程，则加载该库并重新执行命令，这叫做“auto-loading”（自动加载），关于它将在下一节描述。
2。如果存在一个程序的名字与未知命令一致，则调用exec命令来调用该程序，这项特性叫做“auto-exec”（自动执行）。例如你输入“dir”作为一个命令，unknown会执行“exec dir”来列出当前目录的内容，如果这里的命令没有特别指明需要输入输出重定向，则自动执行功能会使用当前Tcl应用所拥有的标准输入输出流，以及标准错误流，这不同于直接调用exec命令，但是提供了在Tcl应用中直接执行其他应用程序的方法。
3。如果命令是一组特殊字符，将会产生一个新的调用，这个调用的内容是历史上已经执行过的命令。例如，如果命令时“!!”则上一条刚执行过的命令会再执行一遍。下一章将详细讲述该功能。
4。若命令是已知命令的唯一缩写，则调用对应的全名称的正确命令。在TCL中允许你使用命令名的缩写，只要缩写唯一即可。
如果你不喜欢unknown的缺省的行为，你也可以自己写一个新版本的unknown或者对库中已有unknown的命令进行扩展以增加某项功能。如果你不想对未知命令做任何处理，也可以删除unknown，这样当调用到未知命令的时候就会产生错误。
#### ■
#### 自动加载
在unknown过程中一项非常有用的功能就是自动加载，自动加载功能允许你编写一组Tcl过程放到一个脚本文件中，然后把该文件放到库目录之下，当程序调用这些过程的时候，第一次调用时由于命令还不存在就会进入unknown命令，而unknown则会找到在哪个库文件中包含了这个过程的定义，接着会加载它，再去重新执行命令，而到下次使用刚才调用过的命令的时候，由于它已经存在了，从而会正常的执行命令，自动加载机制也就不会被再次启动。
自动加载提供了两个好处，首先，你可以把有用的过程建立为过程库，而你无需精确知道过程的定义到底在哪个源文件中，自动加载机制会自动替你寻找，第二个好处在于自动加载是非常有效率的，如果没有自动加载机制你将不得不在TCL应用的开头使用source命令来加载所有可能用到的库文件，而应用自动加载机制，应用启动时无需加载任何库文件，而且有些用不到的库文件永远都不会被加载，既缩短了启动时间又节省了内存。
使用自动加载只需简单的按下面三步来做：
第一，在一个目录下创建一组脚本文件作为库，一般这些文件都以".tcl"结尾。每个文件可以包含任意数量的过程定义。建议尽量减少各脚本文件之间的关联，让相互关联的过程位于同一个文件中。为了能够让自动加载功能正确运行，proc命令定义一定要顶到最左边，并且与函数名用空格分开，过程名保持与proc在同一行上。
第二步，为自动加载建立索引。启动Tcl应用比如tclsh，调用命令**auto_mkindex***dir pattern*, 第一个参数是目录名，第二个参数是一个模式。auto_**mkindex**在目录dir中扫描文件名和模式pattern匹配的文件，并建立索引以指出哪些过程定义在哪些文件中，并把索引保存到目录dir下一个叫tclindex的文件中。如果修改了文件或者增减过程，需要重新生成索引。
第三步是在应用中设置变量auto_path，把存放了希望使用到的库所在的目录赋给它。auto_path变量包含了一个目录的列表，当自动加载被启动的时候，会搜索auto_path中所指的目录，检查各目录下的tclindex文件来确认过程被定义在哪个文件中。如果一个函数被定义在几个库中，则自动加载使用在auto_path中靠前的那个库。
例如，若一个应用使用目录/usr/local/tcl/lib/shapes下的库，则在启动描述中应增加：
set auto_path [linsert $auto_path 0 /usr/local/tcl/lib/shapes]
这将把/usr/local/tcl/lib/shapes作为起始搜索库的路径，同时保持所有的Tcl/Tk库不变，但是在/usr/local/tcl/lib/shapes中定义的过程具有更高的优先级，一旦一个含有索引的目录加到了auto_path中，里面所有的过程都可以通过自动加载使用了。

