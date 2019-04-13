
# TCL: tcl小结 - Augusdi的专栏 - CSDN博客


2015年08月26日 18:48:44[Augusdi](https://me.csdn.net/Augusdi)阅读数：3440


﻿﻿
lindex - 从列表中获得一个元素
### 语法
**lindex***list ?index...?*
### 描述
**lindex**命令接受一个参数列表*list*，可以接受0个或者多个*index*参数，在多个参数的情况下，参数可以是单独的一次排列，也可以是在一个列表当中。如果不指定index参数：
lindex list或者lindex list {}这种情况下返回*lindex*列表本身。当只有一个单独的元素时，**lindex**命令返回*list*列表中的第*index*个元素。替代时元素从0开始（也就是说索引0就是指列表的第一个元素），如果*index*是负数或者大于列表长度就返回一个空字符串。解释器在解释每一个*index*值时和**string index**命令相同，都支持单个和多个*index*参数。
如果指定了多个index，将会选择列表的子列表中的元素。例如
lindex $a 1 2 3或者lindex $a {1 2 3}与下面的命令相同lindex [lindex [lindex $a 1] 2] 3
### 示例
**lindex**{a b c}*→**a b c***lindex**{a b c} {}*→**a b c***lindex**{a b c} 0*→**a***lindex**{a b c} 2*→**c***lindex**{a b c} end*→**c***lindex**{a b c} end-1*→**b***lindex**{{a b c} {d e f} {g h i}} 2 1*→**h***lindex**{{a b c} {d e f} {g h i}} {2 1}*→**h***lindex**{{{a b} {c d}} {{e f} {g h}}} 1 1 0*→**g***lindex**{{{a b} {c d}} {{e f} {g h}}} {1 1 0}*→**g*eval命令本身使用非常简单，但是用处非常大，如果需要动态的构造命令，那么必须使用eval命令。eval命令参考：[http://blog.csdn.net/dulixin/archive/2008/03/27/2223978.aspx](http://blog.csdn.net/dulixin/archive/2008/03/27/2223978.aspx)
命令格式：**eval**arg ?arg ...?
如果是一个参数，那么相当于把这个参数当作命令来执行，如果有多个参数，eval命令会把多个参数以concat命令风格连接起来然后再执行命令。
举一个最简单的例子：
% set cmd "puts /"This is a tcltk example/""
puts "This is a tcltk example"
% eval $cmd
This is a tcltk example
一般在动态脚本中，主要是由脚本片断组成，脚本片断一般是一个变量，根据实际情况进行变量修改来达到执行不同脚本的目的。
% set a puts
puts
% set b stdout
stdout
% set c "haha"
haha
% eval $a $b $c
haha

catch命令可以用来捕获一个错误，一个没有捕获的错误将终止脚本的执行。error会产生一个错误，error命令产生的错误如果没有被catch捕获将会导致脚本的终止。catch命令格式：**catch**script ?resultVarName? ?optionsVarName?
error命令格式：**error**message ?info? ?code?
下面举一个常用的例子来解释error和catch是怎么一起使用的，对其可选项中的各种用途不加详细解释。
% proc myError {} {
error "This is a error"
}
% catch myError errorValue
1
% puts $errorValue
This is a error

foreach循环，格式为：**foreach**varname list body
**foreach**varlist1 list1 ?varlist2 list2 ...? body
主要功能为遍历列表中的元素。
主要有三种使用形式，最简单的，对单个列表中的元素进行一个一个的遍历：
% foreach var {a b c d e f} {
puts $var
}
a
b
c
d
e
f
可以对列表进行多个元素一起赋值，这时varname是一个n个元素列表结构，每次遍历list列表中的n个元素赋值给以varname列表元素为名称的变量。
% foreach {var1 var2 var3} {a b c d e f} {
puts "$var1 $var2 $var3"
}
a b c
d e f
遍历多个列表中的元素，这里举例以varname为单个元素为例：
% foreach var1 {a b c} var2 {d e f} {
puts "$var1 $var2"
}
a d
b e
c f
如果元素不足那么会以空来补充：
% foreach var1 {a b c} var2 {d e} {
puts "$var1 $var2"
}
a d
b e
c
可以利用这个技巧给变量赋值，比如把一个列表中的前3个元素分别赋值给var1、var2和var3：
% foreach {var1 var2 var3} {a b c d e f} {
break;
}
% puts "$var1 $var2 $var3"
a b c
仔细体会一下break的妙用吧。

after有几种形式，使用比较灵活。最简单的形式，间隔一定时间后再运行脚本：
% after 5000
上面的命令就是间隔5秒钟后再继续运行脚本，这在等待其它设备处理时比较有效。
间隔一段时间后执行一条命令：
% set a a
a
% after 5000 set a b
after\#1
间隔5秒钟后执行命令set a b，如果等待5秒钟后再查看$a的值就变成b了。需要注意的是，在tcl中，时间循环并没有开启，而tk中事件循环总是活动的，所以在tcl脚本中使用时需要非常小心，可能你需要的值在5秒钟后并没有改变，这里就需要使用到两个命令update和vwait，update命令可以时解释器去处理挂起的事件，vwait可以等待一个变量到修改为止，下面举例说明：
如果在5秒钟之后使用查看变量a里面的值：
% puts $a
a
还是a，并没有修改为b，那么这个时候使用update：
% update
% puts $a
b
上面的方法可能在实际使用时并没有意义，也许脚本的编写者是想在tcl中精确的控制脚本的执行时间，那也没有问题，可以使用vwait来操作，在时间间隔的期限内使用vwait可以使命令在精确的时间间隔时执行：
% set a a
a
% after 5000 set a b
after\#1
% vwait a
%
会等待到第5秒钟执行赋值命令。

如果注册了一条命令在某个时间执行，也可以取消这个命令的执行，使用after cancel命令，这个命令有两种形式，既可以输入要取消命令的ID，也可以使用该命令本身来取消。
如想要取消ID为after\#1的命令：
% after cancel after\#1
如果ID为after\#1的命令存在就去掉这个命令的注册，如果不存在就什么都不发生。
取消一个命令体：
% after 5000 set a c
after\#2
% after cancel set a c
以上命令就会取消set a c的事件注册，如果不存在这个命令就什么都不发生。
显示目前注册的after事件或者某个after事件的详细信息：
直接使用after info命令来显示所有的after事件：
% after 5000 set a b
after\#1
% after 5000 set a c
after\#2
% after info
after\#2 after\#1
如果需要知道after事件的详细信息，就需要使用具体的事件ID：
% after info after\#1
{set a b} timer

列表操作在脚本中使用的频率非常高，基本上每个脚本都会涉及到其中的一些操作，在这里对列表的命令进行实例解析，以加深对列表命令的理解，本文涉及的命令为list、concat、lrepeat、join和split。
**list**?*arg arg ...*?
**concat**?*arg arg ...*?
使用多个arg来组成一个列表，这两个命令使用频度很高，使用也非常简单，所需要注意的地方就是list和concat的区别，以下使用具体的例子来说明两个命令的区别。
% list This is a tcltk example
This is a tcltk example
% concat This is a tcltk example
This is a tcltk example
以上的例子并没有看出两个命令有什么区别，在这种情况下两个命令的结果并没有什么区别，两个命令的区别主要是list把后面的参数都当作列表的一个元素看待，形成的列表为所有的元素组成，而concat把后面的参数当作一个列表来看待，形成的列表为所有列表中的元素组成。如果两个命令后面的参数有列表变量就可以看出区别了：
% list {This is} {a} {tcltk example}
{This is} a {tcltk example}
% concat {This is} {a} {tcltk example}
This is a tcltk example
concat可以形象地说是去掉了一层列表结构，然后再list所有的元素。使用时需要注意两者的却别。
**lrepeat***number element1*?*element2 element3 ...*?
使用重复的元素构建列表，number为重复的次数，element为重复的元素，这个命令就相当于对重复number次的element元素进行了list操作，所以lrepeat 1 arg ... 和list arg ... 执行结果相同。如下例：
% lrepeat 3 This is a tcltk example
This is a tcltk example This is a tcltk example This is a tcltk example
下例与list命令结果相同：
% lrepeat 1 This is a tcltk example
This is a tcltk example
**join***list*?*joinString*?
**split***string*?*splitChars*?
这两个命令为一对相反的命令，它们操作的过程都为对方的逆过程。join命令把一个列表中的元素使用joinString连接成一个字符串，而split是根据splitChars把一个字符串分割为列表。如果没有后面的可选变元，分割符默认为空白符。如下例：
% join {This is a tcltk example} ?
This?is?a?tcltk?example
% split This?is?a?tcltk?example ?
This is a tcltk example
第二个命令使用第一个命令的结果作为参数。

列表操作在脚本中使用的频率非常高，基本上每个脚本都会涉及到其中的一些操作，在这里对列表的命令进行实例解析，以加深对列表命令的理解，本文涉及的命令为llength、lindex、lrange、lsearch和lassign。
**llength***list*
返回一个列表的元素个数，非常简单而又常用的命令。
% llength {This is a tcltk example}
5
**lindex***list ?index...?*
根据索引值，找出列表中索引为index的元素，如果没有index就返回整个列表，如果有多个index就返回列表的子列表的元素，具体示例如下：
返回整个列表：
% lindex {This is a tcltk example}
This is a tcltk example
返回列表中索引为3的元素：
% lindex {This is a tcltk example} 3
tcltk
返回列表中索引为2的元素
% lindex {{This is} a {tcltk example}} 2
tcltk example
返回列表中索引为2的子列表中索引为1的元素
% lindex {{This is} a {tcltk example}} 2 1
example
**lrange***list first last*
返回列表一个区间的元素，这个区间由first和last指定。
% lrange {This is a tcltk example} 1 3
is a tcltk
**lsearch**?*options*?*list pattern*
在列表中寻找元素，这里的标志位比较多，下面一一介绍，多个标志位可以互相混用。
以下是匹配风格标志位：
寻找的列表元素严格匹配pattern，也就是说pattern就是列表中的一个元素才能找到，返回元素的索引：
% lsearch -exact {This is a tcltk example} is
1
以glob风格匹配pattern，没有匹配风格标志位的话默认就是glob，搜索以is结尾的字符：
% lsearch -glob {This is a tcltk example} *is
0
以正则表达式风格匹配，搜索以is结尾的字符：
% lsearch -regexp {This is a tcltk example} .*is
0
以下是一些修饰标志位：
返回所有符合匹配风格的元素索引：
% lsearch -all {This is a tcltk example} *is
0 1
返回符合匹配风格的元素值而不是索引：
% lsearch -inline -all {This is a tcltk example} *is
This is
返回不符合匹配风格的元素索引：
% lsearch -not -all {This is a tcltk example} *is
2 3 4
从指定的索引开始搜索，下面的例子只返回了索引1，没有返回索引0：
% lsearch -start 1 -all {This is a tcltk example} *is
1
内容描述标志位：
所匹配的内容为ASCII码，使用-ascii标志位，默认就是。
可以和-sorted一起使用-dictionary来标志以字典顺序匹配。
使用-integer说明列表元素被当作整数匹配。
-real说明列表元素被当作浮点数匹配。
-nocase忽略大小写：
% lsearch -nocase {This is a tcltk example} this
0
还有两个排序标志位，需要和sorted一起使用，-decreasing和-increasing分别代表降序和升序。
两个嵌入式标志位：
-index，匹配子列表中的索引，下面的例子匹配子列表中的第二个元素，有这个标志位要求list中每个元素都必须有子列表，并且有需要检查的index：
% lsearch -index 1 -all {{This is} {b  a} {tcltk example}} *a*
1 2
-subindices，需要和-index一起使用，返回匹配的全路径：
% lsearch -index 1 -all -subindices {{This is} {b  a} {tcltk example}} *a*
{1 1} {2 1}
**lassign***list varName*?*varName ...*?
将列表元素赋值给变量，直接采用help里面的例子，非常明确了：
lassign {a b c} x y z       ;\# 返回空
puts $x                     ;\# Prints "a"
puts $y                     ;\# Prints "b"
puts $z                     ;\# Prints "c"
lassign {d e} x y z         ;\# 返回空
puts $x                     ;\# Prints "d"
puts $y                     ;\# Prints "e"
puts $z                     ;\# Prints ""
lassign {f g h i} x y       ;\# 返回"h i"
puts $x                     ;\# Prints "f"
puts $y                     ;\# Prints "g"

列表操作在脚本中使用的频率非常高，基本上每个脚本都会涉及到其中的一些操作，在这里对列表的命令进行实例解析，以加深对列表命令的理解，本文涉及的命令为lappend、lreplace、lset、linsert、lsort和lreverse。
**lappend***varName*?*value value value ...*?
在列表后面添加元素，常用的命令，lappend命令接收一个变量名（这里需要注意，必须是变量名而不是变量），将元素添加到变量后面，变量会被修改：
% set myList {This is a}
This is a
% lappend myList tcltk example
This is a tcltk example
% puts $myList
This is a tcltk example
由以上可以看出，变量myList已经被修改了，在tcl中如果命令中传入的是变量名一般结果都修改变量，传入的是变量值都不会修改变量本身。
**lreplace***list first last*?*element element ...*?
将索引为first到last的元素替换为后面的元素：
% lreplace {This is a tcltk example} 2 2 another
This is another tcltk example
% lreplace {This is a tcltk example} 2 3 another TCLTK
This is another TCLTK example
**lset***varName ?index...? newValue*
这个命令和lappand一样接收一个变量名作为参数，也会修改变量的值，将列表中的指定索引项目修改为指定值，如果不指定索引项就把列表整个换为新的值：
% set myList {This is a tcltk example}
This is a tcltk example
% lset myList 2 another
This is another tcltk example
% puts $myList
This is another tcltk example
如果没有指定索引，就相当于直接赋值：
% lset myList {no index}
no index
**linsert***list index element*?*element element ...*?
这个命令插入元素到列表的index索引位，产生一个新的列表：
% linsert {This is example} 2 a tcltk
This is a tcltk example
**lsort**?*options*?*list*
为列表排序。
按照ASCII码顺序排序，这个是默认状态：
% lsort -ascii {b A c B a C}
A B C a b c
按照字典顺序排序：
% lsort -dictionary {b A c B a C}
A a B b C c
按照整数排序，要求列表里面的元素都能正确转化为整数：
% lsort -integer {11 13 15 56}
11 13 15 56
按照浮点数排序,要求列表里面的元素都能够正确转化为浮点数：
% lsort -real {11.1 22.2 14.3}
11.1 14.3 22.2
按照命令排序，命令体必须是接收两个参数的命令，结果返回大于、小于或者等于0分别代表两个元素相比较是大于、小于还是等于关系，利用此关系进行排序：
% lsort -command {string compare}  {b A c B a C}
A B C a b c
按照升序排列,这是默认形式：
% lsort -increasing {b A c B a C}
A B C a b c
按照降序排列：
% lsort -decreasing {b A c B a C}
c b a C B A
返回排序后的元素在原列表中的索引：
% lsort -indices {b A c B a C}
1 3 5 4 0 2
根据子列表排序，每个元素必须是一个命令能够正确识别的子列表，-index 1表示根据子列表的第二个元素来排序：
% lsort -index 1 {{b A} {c B} {a C}}
{b A} {c B} {a C}
忽略大小写：
% lsort -nocase {b A c B a C}
A a b B c C
**lreverse***list*
返回一个列表，列表为原列表的反序形式：
% lreverse {This is a tcltk example}
example tcltk a is This

variable - 创建和初始化一个名字空间变量
### 语法
**variable**?*name value...*?*name*?*value*?
### 描述
这个命令一般在名字空间中创建一个或多个变量，每个变量*name*使用*value*初始化，最后一个变量的*value*是可选的。
如果变量*name*不存在，就创建一个，在这种情况下，如果指定了*value*，就把它的值赋值给创建的变量，如果不指定*value*，新的变量*name*就没有初始化。如果变量已经存在，就赋值*value*，没有指定*value*则保持原来的值。一般来说*name*不做任何限定（不包含名字空间限定），并且在当前名字空间中创建，如果*name*包含有名字空间限定，变量就创建在指定的名字空间中，如果变量没有定义，对于**namespace which**命令来说就是可见的，但是对**info exists**命令来说是不可见的。
如果**variable**命令在一个tcl过程中执行，就创建一个局部变量，这个局部变量链接一个相关的名字空间变量（因此这些变量由**info vars**列出），这样variable命令类似**global**命令，虽然**global**命令只是链接一个在全局范围内的变量，如果给出了*values*，就用来修改相关名字空间变量，如果一个名字空间变量不存在，就创建一个并且初始化。
*name*变元不能引用一个数组的元素，但是*name*可以引用整个数组并且初始化的*value*必须符合数组的要求，声明过变量后，数组中的元素可以使用**set**和**array**命令来配置。
### 示例
在名字空间中创建一个变量：namespace eval foo {**variable**bar 12345 }在名字空间中创建一个数组：
namespace eval someNS {**variable**someAry array set someAry { someName someValue otherName otherValue } }为一个过程存取名字空间中的变量：
namespace eval foo { proc spong {} { \# Variable in this namespace**variable**bar puts "bar is $bar" \# Variable in another namespace**variable**::someNS::someAry parray someAry } }array - 处理数组变量
### 语法
**array***option arrayName*?*arg arg ...*?

### 描述
这个命令执行几种可选项中的一个操作，操作的对象为*arrayName*，除非以下的命令特殊声明，否则*arrayName*必须是存在的数组变量名称。*option*变元决定了要对数组变量进行什么样的操作，具体如下：
**array anymore***arrayName searchId*如果在一次数组搜索当中数组中还有剩余的元素就返回**1**，如果没有剩余的元素就返回**0**。*searchId*指定了被检查*arrayName*的搜索标示符，这个标示符必须是命令**array startsearch**返回的搜索标示符。这个命令在数组有空索引的元素时非常有用，因为**array nextelement**的返回值并不能说明搜索是否完毕。**array donesearch***arrayName searchId*这个命令终结一次数组搜索，*searchId*指出了需要终结的数组搜索标示符，这个标示必须是命令**array startsearch**返回的搜索标示符。返回一个空字符串。**array exists***arrayName*如果*arrayName*是一个数组变量就返回**1**，如果没有这个变量或者是非数组变量就返回**0**。**array get***arrayName*?*pattern*?返回一个列表，列表中的元素是成对的，交替的出现索引和对应数组值，如果不指定*pattern*，所有数组的元素都回返回，如果指定了*pattern*，只有匹配*pattern*（与**string match**匹配风格相同）的数组元素返回。如果*arrayName*不是一个数组变量的名字或者没有包含任何元素就返回一个空列表。**array names***arrayName*?*mode*? ?*pattern*?返回一个匹配*pattern*的数组元素索引的列表，mode可以是**-exact**、**-glob**或**-regexp**中的一个，指定了匹配的风格，如果不指定*mode*，默认为**-glob**。如果需要了解详细的匹配风格请参考**string match**和**regexp**。如果不指定*pattern*则返回数组中所有的索引，如果*arrayName*不是一个数组变量的名字或者没有包含任何元素就返回一个空列表。**array nextelement***arrayName searchId*返回*arrayName*数组中的下一个元素索引，如果数组中所有的元素都搜索到了就返回空字符串，*searchId*指出了需要终结的数组搜索标示，这个标示必须是命令**array startsearch**返回的搜索标示。警告：如果数组中有添加和删除元素的操作，那么所有的搜索都回自动结束，就像是调用了**array donesearch**，这将会导致**array nextelement**操作失败。**array set***arrayName list*设置一个或多个数组元素，*list*必须是像**array get**返回值风格的列表，第奇数个列表元素被认为是*arrayName*的一个元素索引，紧接着的第偶数个列表元素就被当作前一个元素的数组中的值，如果变量*arrayName*不存在或者为空，就创建一个空数组*arrayName*。**array size***arrayName*返回一个十进制的字符串数值来指出数组中元素的数量，如果*arrayName*不是一个数组的名字就返回**0**。**array startsearch***arrayName*这个命令开始在*arrayName*数组中进行一个元素一个元素的搜索，**array nextelement**命令返回下一元素的索引，当搜索完毕，**array donesearch**命令必须调用，返回值是一个搜索标示符，这个搜索表示符可以在**array nextelement**和**array donesearch**中使用来标示操作的搜索，通过使用搜索标示符允许对一个数组同时进行不同的搜索。目前，普遍使用的方式是使用**array get**和**array names**与**foreach**一起使用来遍历数组中的每一个元素。具体请参考下面的示例。**array statistics***arrayName*返回数组中元素在哈希表的分配方式的统计，这个命令包含表格中条目数，buckets数目和buckets的利用情况。**array unset***arrayName*?*pattern*?删除数组中所有匹配*pattern*的元素（与**string match**匹配风格相同），如果*arrayName*不是一个数组变量或者没有匹配到任何元素，不会产生错误，如果忽略了*pattern*变元并且*arrayName*是一个数组名称，这个命令将删除整个数组所有的元素，这个命令总是返回一个空字符串。
### 示例
**array set**colorcount {red 1green 5blue 4white 9} foreach {color count} [**array get**colorcount] {puts "Color: $color Count: $count"}**→**Color: blue Count: 4Color: white Count: 9Color: green Count: 5Color: red Count: 1foreach color [**array names**colorcount] {puts "Color: $color Count: $colorcount($color)"}**→**Color: blue Count: 4Color: white Count: 9Color: green Count: 5Color: red Count: 1 foreach color [lsort [**array names**colorcount]] {puts "Color: $color Count: $colorcount($color)"}**→**Color: blue Count: 4Color: green Count: 5Color: red Count: 1Color: white Count: 9**array statistics**colorcount**→**4 entries in table, 4 bucketsnumber of buckets with 0 entries: 1number of buckets with 1 entries: 2number of buckets with 2 entries: 1number of buckets with 3 entries: 0number of buckets with 4 entries: 0number of buckets with 5 entries: 0number of buckets with 6 entries: 0number of buckets with 7 entries: 0number of buckets with 8 entries: 0number of buckets with 9 entries: 0number of buckets with 10 or more entries: 0average search distance for entry: 1.2uplevel - 在不同的堆栈层中执行一个脚本
### 语法
**uplevel**?*level*?*arg*?*arg ...*?

### 描述
所有的*arg*变元使用**concat**命令风格连接起来，连接起来的命令在*level*指定后的作用域中执行，**uplevel**返回执行结果。（堆栈层可以看作是变量起作用的范围）
如果*level*是一个整数，它指出了调用命令的堆栈距离目前调用堆栈的层数，如果*level*是以**\#**后面加一个数字，那么这个数字就是命令执行的绝对堆栈层，缺省值为**1**，如果有了**\#**符号那么后面必须跟一个数字。
例如，假设过程**a**在最高层调用，然后**a**调用**b**，再后来**b**调用**c**，假设**c**调用**uplevel**名令，如果*level*是**1**或者**\#2**或者忽略掉，这个命令将在**b**的变量的作用域中执行，如果*level*是**2**或者**\#1**，那么这个命令将在**a**的变量的作用域中执行，如果*level*是**3**或者**\#0**，这个命令将在最高层执行（只有全局变量是可见的）。
**uplevel**命令导致在调用过程的堆栈层中"看不到"过程了，在下面的例子里，假设**c**调用命令：
**uplevel**1 {set x 43; d}当d是另外一个过程时，**set**命令会在b的作用域中修改变量**x**，**d**将在第三层堆栈运行，就像是**b**过程调用的**d**过程。
**uplevel**{set x 42}**set**命令将会修改在**b**作用域中的**x**变量，在执行**d**的时候过程**c**对于调用堆栈层是不可见的，命令**info level**可以获得当前程序的堆栈层。
**uplevel**可以实现新的控制结构，例如，**uplevel**可以实现**while**过程。
### 示例
**uplevel**命令在实现新的控制结构方面非常有用，下面的例子显示了它可以用来创建一个**do**命令：
proc do {body while condition} {if {$while ne "while"} {error "required word missing"}set conditionCmd [list expr $condition]while {1} {**uplevel**1 $bodyif {![**uplevel**1 $conditionCmd]} {break}}}




