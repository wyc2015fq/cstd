
# TCL: LIST命令－lsearch, lsort, lrange - Augusdi的专栏 - CSDN博客


2015年08月12日 13:00:04[Augusdi](https://me.csdn.net/Augusdi)阅读数：6081


﻿﻿
命令格式：
lsearch list pattern
lsort list
lrange list first last
实例：
set list [list {Washington 1789} {Adams 1797} {Jefferson 1801} \
{Madison 1809} {Monroe 1817} {Adams 1825} ]
set x [lsearch $list Washington*]
set y [lsearch $list Madison*]
incr x
incr y -1;\# Set range to be not-inclusive
set subsetlist [lrange $list $x $y]
puts "The following presidents served between Washington and Madison"
foreach item $subsetlist {
puts "Starting in [lindex $item 1]: President [lindex $item 0] "
}
set x [lsearch $list Madison*]
set srtlist [lsort $list]
set y [lsearch $srtlist Madison*]
puts "\n$x Presidents came before Madison chronologically"
puts "$y Presidents came before Madison alphabetically"
执行结果：
The following presidents served between Washington and Madison
Starting in 1797: President Adams
Starting in 1801: President Jefferson
3 Presidents came before Madison chronologically
3 Presidents came before Madison alphabetically
附上常见的列表命令：
1list命令
语法：list ? value value...?
这个命令生成一个list，list的元素就是所有的value。例：
% list 1 2 {3 4}
1 2 {3 4}
concat命令:
语法:concat list ?list...?
这个命令把多个list合成一个list，每个list变成新list的一个元素。
3lindex命令
语法：lindex list index
返回list的第index个(0-based)元素。例：
%lindex{1 2 {3 4}} 2
3 4
4llength命令
语法：llengthlist
返回list的元素个数。例
% llength{1 2 {3 4}}
3
5linsert命令
语法：linsert list index value ?value...?
返回一个新串，新串是把所有的value参数值插入list的第index个(0-based)元素之前得到。例:
% linsert {1 2 {3 4}}17 8{9 10}
1 7 8 {9 10} 2 {3 4}
6lreplace命令:
语法：lreplace list first last ?value value ...?
返回一个新串，新串是把list的第firs (0-based)t到第last个(0-based)元素用所有的value参数替换得到的。如果没有value参数，就表示删除第first到第last个元素。例：
% lreplace {1 7 8 {9 10} 2 {3 4}}3 3
1 7 8 2 {3 4}
% lreplace {1 7 8 2 {3 4}}444 5 6
1 7 8 2 4 5 6
7lrange命令:
语法:lrange list first last
返回list的第first (0-based)到第last (0-based)元素组成的串,如果last的值是end。就是从第first个直到串的最后。
例：
% lrange{1 7 8 2 4 5 6} 3 end
2 4 5 6
8lappend命令:
语法：lappend varname value ?value...?
把每个value的值作为一个元素附加到变量varname后面，并返回变量的新值，如果varname不存在，就生成这个变量。例：
% lappenda1 2 3
1 2 3
% set a
1 2 3
9lsearch命令:
语法：lsearch ?-exact? ?-glob? ?-regexp? list pattern
返回list中第一个匹配模式pattern的元素的索引，如果找不到匹配就返回-1。-exact、-glob、-regexp是三种模式匹配的技术。-exact表示精确匹配；-glob的匹配方式和string match命令的匹配方式相同，将在后面第八节介绍string命令时介绍；-regexp表示正规表达式匹配，将在第八节介绍regexp命令时介绍。缺省时使用-glob匹配。例:
% seta{ how are you }
how are you
% lsearch $ay*
2
% lsearch $a y?
-1
10lsort命令:
语法：lsort ?options? list
这个命令返回把list排序后的串。options可以是如下值：
-ascii按ASCII字符的顺序排序比较.这是缺省情况。
-dictionary按字典排序,与-ascii不同的地方是：
(1)不考虑大小写
(2)如果元素中有数字的话,数字被当作整数来排序.
因此：bigBoy排在bigbang和bigboy之间, x10y排在x9y和x11y之间.
-integer把list的元素转换成整数,按整数排序.
-real把list的元素转换成浮点数,按浮点数排序.
-increasing升序(按ASCII字符比较)
-decreasing降序(按ASCII字符比较)
-commandcommand TCL自动利用command命令把每两个元素一一比较,然后给出排序结果。
11split命令:
语法：split string ?splitChars?
把字符串string按分隔符splitChars分成一个个单词，返回由这些单词组成的串。如果splitChars
是一个空字符{}，string被按字符分开。如果splitChars没有给出,以空格为分隔符。例：
% split "how.are.you" .
how are you
% split "how are you"
how are you
% split "how are you" {}
h o w { } a r e { } y o u
12join命令
语法:join list ?joinString?
join命令是命令的逆。这个命令把list的所有元素合并到一个字符串中，中间以joinString分开。缺省的joinString是空格。例：
% join {h o w { } a r e { } y o u} {}
how are you
% join{how are you} .
how.are.you

