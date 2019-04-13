
# TCL命令 - Augusdi的专栏 - CSDN博客


2015年10月13日 14:52:06[Augusdi](https://me.csdn.net/Augusdi)阅读数：3995


**TCL命令集合**
**eval**命令本身使用非常简单，但是用处非常大，如果需要动态的构造命令，那么必须使用eval命令。
eval命令参考：[http://blog.csdn.net/dulixin/archive/2008/03/27/2223978.aspx](http://blog.csdn.net/dulixin/archive/2008/03/27/2223978.aspx)

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
**catch**命令可以用来捕获一个错误，一个没有捕获的错误将终止脚本的执行。error会产生一个错误，error命令产生的错误如果没有被catch捕获将会导致脚本的终止。catch命令格式：**catch**script ?resultVarName? ?optionsVarName?
error命令格式：**error**message ?info? ?code?
下面举一个常用的例子来解释error和catch是怎么一起使用的，对其可选项中的各种用途不加详细解释。
% proc myError {} {
error "This is a error"
}
% catch myError errorValue
1
% puts $errorValue
This is a error

**foreach**循环，格式为：**foreach**varname list body
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

**after**有几种形式，使用比较灵活。最简单的形式，间隔一定时间后再运行脚本：
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

**variable**- 创建和初始化一个名字空间变量
### 语
### 法
**variable**?*name value...*?*name*?*value*?
### 描述
这个命令一般在名字空间中创建一个或多个变量，每个变量*name*使用*value*初始化，最后一个变量的*value*是可选的。
如果变量*name*不存在，就创建一个，在这种情况下，如果指定了*value*，就把它的值赋值给创建的变量，如果不指定*value*，新的变量*name*就没有初始化。如果变量已经存在，就赋值*value*，没有指定*value*则保持原来的值。一般来说*name*不做任何限定（不包含名字空间限定），并且在当前名字空间中创建，如果*name*包含有名字空间限定，变量就创建在指定的名字空间中，如果变量没有定义，对于**namespace which**命令来说就是可见的，但是对**info exists**命令来说是不可见的。
如果**variable**命令在一个tcl过程中执行，就创建一个局部变量，这个局部变量链接一个相关的名字空间变量（因此这些变量由**info vars**列出），这样variable命令类似**global**命令，虽然**global**命令只是链接一个在全局范围内的变量，如果给出了*values*，就用来修改相关名字空间变量，如果一个名字空间变量不存在，就创建一个并且初始化。
*name*变元不能引用一个数组的元素，但是*name*可以引用整个数组并且初始化的*value*必须符合数组的要求，声明过变量后，数组中的元素可以使用**set**和**array**命令来配置。
### 示例
在名字空间中创建一个变量：namespace eval foo {**variable**bar 12345 }在名字空间中创建一个数组：
namespace eval someNS {**variable**someAry array set someAry { someName someValue otherName otherValue } }为一个过程存取名字空间中的变量：
```python
namespace eval foo {
 proc spong {} {
 # Variable in this namespace
 variable bar
 puts "bar is $bar"
 # Variable in another namespace
 variable ::someNS::someAry
 parray someAry
 } 
}
```

**uplevel**- 在不同的堆栈层中执行一个脚本
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




**array**- 处理数组变量
### 语法
**array***option arrayName*?*arg arg  ...*?

### 描述
这个命令执行几种可选项中的一个操作，操作的对象为*arrayName*，除非以下的命令特殊声明，否则*arrayName*必须是存在的数组变量名称。*option*变元决定了要对数组变量进行什么样的操作，具体如下：
**array anymore***arrayName searchId*如果在一次数组搜索当中数组中还有剩余的元素就返回**1**，如果没有剩余的元素就返回**0**。*searchId*指定了被检查*arrayName*的搜索标示符，这个标示符必须是命令**array  startsearch**返回的搜索标示符。这个命令在数组有空索引的元素时非常有用，因为**array  nextelement**的返回值并不能说明搜索是否完毕。**array donesearch***arrayName searchId*这个命令终结一次数组搜索，*searchId*指出了需要终结的数组搜索标示符，这个标示必须是命令**array  startsearch**返回的搜索标示符。返回一个空字符串。**array exists***arrayName*如果*arrayName*是一个数组变量就返回**1**，如果没有这个变量或者是非数组变量就返回**0**。**array get***arrayName*?*pattern*?返回一个列表，列表中的元素是成对的，交替的出现索引和对应数组值，如果不指定*pattern*，所有数组的元素都回返回，如果指定了*pattern*，只有匹配*pattern*（与**string  match**匹配风格相同）的数组元素返回。如果*arrayName*不是一个数组变量的名字或者没有包含任何元素就返回一个空列表。**array names***arrayName*?*mode*?  ?*pattern*?返回一个匹配*pattern*的数组元素索引的列表，mode可以是**-exact**、**-glob**或**-regexp**中的一个，指定了匹配的风格，如果不指定*mode*，默认为**-glob**。如果需要了解详细的匹配风格请参考**string  match**和**regexp**。如果不指定*pattern*则返回数组中所有的索引，如果*arrayName*不是一个数组变量的名字或者没有包含任何元素就返回一个空列表。**array nextelement***arrayName  searchId*返回*arrayName*数组中的下一个元素索引，如果数组中所有的元素都搜索到了就返回空字符串，*searchId*指出了需要终结的数组搜索标示，这个标示必须是命令**array  startsearch**返回的搜索标示。警告：如果数组中有添加和删除元素的操作，那么所有的搜索都回自动结束，就像是调用了**array  donesearch**，这将会导致**array nextelement**操作失败。**array set***arrayName list*设置一个或多个数组元素，*list*必须是像**array  get**返回值风格的列表，第奇数个列表元素被认为是*arrayName*的一个元素索引，紧接着的第偶数个列表元素就被当作前一个元素的数组中的值，如果变量*arrayName*不存在或者为空，就创建一个空数组*arrayName*。**array size***arrayName*返回一个十进制的字符串数值来指出数组中元素的数量，如果*arrayName*不是一个数组的名字就返回**0**。**array startsearch***arrayName*这个命令开始在*arrayName*数组中进行一个元素一个元素的搜索，**array  nextelement**命令返回下一元素的索引，当搜索完毕，**array  donesearch**命令必须调用，返回值是一个搜索标示符，这个搜索表示符可以在**array  nextelement**和**array  donesearch**中使用来标示操作的搜索，通过使用搜索标示符允许对一个数组同时进行不同的搜索。目前，普遍使用的方式是使用**array  get**和**array  names**与**foreach**一起使用来遍历数组中的每一个元素。具体请参考下面的示例。**array statistics***arrayName*返回数组中元素在哈希表的分配方式的统计，这个命令包含表格中条目数，buckets数目和buckets的利用情况。**array unset***arrayName*?*pattern*?删除数组中所有匹配*pattern*的元素（与**string  match**匹配风格相同），如果*arrayName*不是一个数组变量或者没有匹配到任何元素，不会产生错误，如果忽略了*pattern*变元并且*arrayName*是一个数组名称，这个命令将删除整个数组所有的元素，这个命令总是返回一个空字符串。
### 示例
**array set**colorcount {red 1green 5blue 4white 9} foreach {color count} [**array get**colorcount] {puts "Color: $color Count: $count"}**→**Color: blue Count: 4Color: white Count: 9Color: green Count: 5Color: red Count: 1foreach color [**array names**colorcount] {puts "Color: $color Count: $colorcount($color)"}**→**Color: blue Count: 4Color: white Count: 9Color: green Count: 5Color: red Count: 1 foreach color [lsort [**array names**colorcount]] {puts "Color: $color Count: $colorcount($color)"}**→**Color: blue Count: 4Color: green Count: 5Color: red Count: 1Color: white Count: 9**array statistics**colorcount**→**4 entries in table, 4 bucketsnumber of buckets with 0 entries: 1number of buckets with 1 entries: 2number of buckets with 2 entries: 1number of buckets with 3 entries: 0number of buckets with 4 entries: 0number of buckets with 5 entries: 0number of buckets with 6 entries: 0number of buckets with 7 entries: 0number of buckets with 8 entries: 0number of buckets with 9 entries: 0number of buckets with 10 or more entries: 0average search distance for entry: 1.2
列表操作在脚本中使用的频率非常高，基本上每个脚本都会涉及到其中的一些操作，在这里对列表的命令进行实例解析，以加深对列表命令的理解，本文涉及的命令为list、concat、lrepeat、join和split。
**list**?*arg arg  ...*?
**concat**?*arg arg  ...*?
使用多个arg来组成一个列表，这两个命令使用频度很高，使用也非常简单，所需要注意的地方就是list和concat的区别，以下使用具体的例子来说明两个命令的区别。
% list This is a tcltk  example
This is a tcltk  example
% concat This is a tcltk  example
This is a tcltk  example
以上的例子并没有看出两个命令有什么区别，在这种情况下两个命令的结果并没有什么区别，两个命令的区别主要是list把后面的参数都当作列表的一个元素看待，形成的列表为所有的元素组成，而concat把后面的参数当作一个列表来看待，形成的列表为所有列表中的元素组成。如果两个命令后面的参数有列表变量就可以看出区别了：
% list {This is} {a} {tcltk  example}
{This is} a {tcltk  example}
% concat {This is} {a} {tcltk  example}
This is a tcltk  example
concat可以形象地说是去掉了一层列表结构，然后再list所有的元素。使用时需要注意两者的却别。
**lrepeat***number element1*?*element2 element3  ...*?
使用重复的元素构建列表，number为重复的次数，element为重复的元素，这个命令就相当于对重复number次的element元素进行了list操作，所以lrepeat  1 arg ... 和list arg ... 执行结果相同。如下例：
% lrepeat 3 This is a  tcltk example
This is a tcltk example This is a tcltk example This is a tcltk  example
下例与list命令结果相同：
% lrepeat 1 This is a  tcltk example
This is a tcltk example
**join***list*?*joinString*?
**split***string*?*splitChars*?
这两个命令为一对相反的命令，它们操作的过程都为对方的逆过程。join命令把一个列表中的元素使用joinString连接成一个字符串，而split是根据splitChars把一个字符串分割为列表。如果没有后面的可选变元，分割符默认为空白符。如下例：
% join {This is a tcltk example}  ?
This?is?a?tcltk?example
%  split This?is?a?tcltk?example ?
This is a tcltk  example
第二个命令使用第一个命令的结果作为参数。

列表操作在脚本中使用的频率非常高，基本上每个脚本都会涉及到其中的一些操作，在这里对列表的命令进行实例解析，以加深对列表命令的理解，本文涉及的命令为llength、lindex、lrange、lsearch和lassign。
**llength***list*
返回一个列表的元素个数，非常简单而又常用的命令。
% llength {This is a tcltk  example}
5
**lindex***list  ?index...?*
根据索引值，找出列表中索引为index的元素，如果没有index就返回整个列表，如果有多个index就返回列表的子列表的元素，具体示例如下：
返回整个列表：
% lindex {This is a tcltk  example}
This is a tcltk  example
返回列表中索引为3的元素：
% lindex {This is a tcltk example}  3
tcltk
返回列表中索引为2的元素
% lindex {{This is} a {tcltk example}}  2
tcltk  example
返回列表中索引为2的子列表中索引为1的元素
% lindex {{This is} a {tcltk example}} 2  1
example
**lrange***list first  last*
返回列表一个区间的元素，这个区间由first和last指定。
% lrange {This is a tcltk example} 1  3
is a tcltk
**lsearch**?*options*?*list  pattern*
在列表中寻找元素，这里的标志位比较多，下面一一介绍，多个标志位可以互相混用。
以下是匹配风格标志位：
寻找的列表元素严格匹配pattern，也就是说pattern就是列表中的一个元素才能找到，返回元素的索引：
% lsearch -exact {This is a tcltk example}  is
1
以glob风格匹配pattern，没有匹配风格标志位的话默认就是glob，搜索以is结尾的字符：
% lsearch -glob {This is a tcltk example}  *is
0
以正则表达式风格匹配，搜索以is结尾的字符：
% lsearch -regexp {This is a tcltk  example}  .*is
0
以下是一些修饰标志位：
返回所有符合匹配风格的元素索引：
% lsearch -all {This is a tcltk example}  *is
0  1
返回符合匹配风格的元素值而不是索引：
% lsearch -inline -all {This is a tcltk  example} *is
This  is
返回不符合匹配风格的元素索引：
% lsearch -not -all {This is a tcltk  example} *is
2 3  4
从指定的索引开始搜索，下面的例子只返回了索引1，没有返回索引0：
% lsearch -start 1 -all {This is a tcltk  example}  *is
1
内容描述标志位：
所匹配的内容为ASCII码，使用-ascii标志位，默认就是。
可以和-sorted一起使用-dictionary来标志以字典顺序匹配。
使用-integer说明列表元素被当作整数匹配。
-real说明列表元素被当作浮点数匹配。
-nocase忽略大小写：
% lsearch -nocase {This is a tcltk  example}  this
0
还有两个排序标志位，需要和sorted一起使用，-decreasing和-increasing分别代表降序和升序。
两个嵌入式标志位：
-index，匹配子列表中的索引，下面的例子匹配子列表中的第二个元素，有这个标志位要求list中每个元素都必须有子列表，并且有需要检查的index：
% lsearch -index 1 -all {{This is}  {ba} {tcltk example}} *a*
1  2
-subindices，需要和-index一起使用，返回匹配的全路径：
% lsearch -index 1 -all -subindices {{This  is} {ba} {tcltk example}} *a*
{1 1} {2  1}
**lassign***list varName*?*varName  ...*?
将列表元素赋值给变量，直接采用help里面的例子，非常明确了：
lassign {a b c} x y  z;\# 返回空
puts  $x;\# Prints "a"
puts  $y;\# Prints "b"
puts  $z;\# Prints "c"
lassign {d e} x y  z;\#  返回空
puts  $x;\# Prints "d"
puts  $y;\# Prints "e"
puts  $z;\# Prints ""
lassign {f g h i} x  y;\# 返回"h i"
puts  $x;\# Prints "f"
puts  $y;\# Prints  "g"

列表操作在脚本中使用的频率非常高，基本上每个脚本都会涉及到其中的一些操作，在这里对列表的命令进行实例解析，以加深对列表命令的理解，本文涉及的命令为lappend、lreplace、lset、linsert、lsort和lreverse。
**lappend***varName*?*value value value  ...*?
在列表后面添加元素，常用的命令，lappend命令接收一个变量名（这里需要注意，必须是变量名而不是变量），将元素添加到变量后面，变量会被修改：
% set myList {This is  a}
This is a
% lappend  myList tcltk example
This is a tcltk  example
% puts  $myList
This is a tcltk  example
由以上可以看出，变量myList已经被修改了，在tcl中如果命令中传入的是变量名一般结果都修改变量，传入的是变量值都不会修改变量本身。
**lreplace***list first last*?*element element  ...*?
将索引为first到last的元素替换为后面的元素：
% lreplace {This is a tcltk example} 2 2  another
This is another tcltk  example
% lreplace {This is a tcltk example} 2 3  another TCLTK
This is another TCLTK  example
**lset***varName ?index...?  newValue*
这个命令和lappand一样接收一个变量名作为参数，也会修改变量的值，将列表中的指定索引项目修改为指定值，如果不指定索引项就把列表整个换为新的值：
% set myList {This is a tcltk  example}
This is a tcltk  example
% lset myList 2  another
This is another tcltk  example
% puts $myList
This is another tcltk  example
如果没有指定索引，就相当于直接赋值：
% lset myList {no  index}
no index
**linsert***list index  element*?*element element  ...*?
这个命令插入元素到列表的index索引位，产生一个新的列表：
% linsert {This is example} 2 a  tcltk
This is a tcltk example
**lsort**?*options*?*list*
为列表排序。
按照ASCII码顺序排序，这个是默认状态：
% lsort -ascii {b A c B a  C}
A B C a b  c
按照字典顺序排序：
% lsort -dictionary {b A c B a  C}
A a B b C  c
按照整数排序，要求列表里面的元素都能正确转化为整数：
% lsort -integer {11 13 15  56}
11 13 15  56
按照浮点数排序,要求列表里面的元素都能够正确转化为浮点数：
% lsort -real {11.1 22.2  14.3}
11.1 14.3  22.2
按照命令排序，命令体必须是接收两个参数的命令，结果返回大于、小于或者等于0分别代表两个元素相比较是大于、小于还是等于关系，利用此关系进行排序：
% lsort -command {string compare}{b  A c B a C}
A B C a b  c
按照升序排列,这是默认形式：
% lsort -increasing {b A c B a  C}
A B C a b  c
按照降序排列：
% lsort -decreasing {b A c B a  C}
c b a C B  A
返回排序后的元素在原列表中的索引：
% lsort -indices {b A c B a  C}
1 3 5 4 0  2
根据子列表排序，每个元素必须是一个命令能够正确识别的子列表，-index  1表示根据子列表的第二个元素来排序：
% lsort -index  1 {{b A} {c B} {a C}}
{b A} {c B} {a  C}
忽略大小写：
% lsort -nocase {b A c B a  C}
A a b B c C
**lreverse***list*
返回一个列表，列表为原列表的反序形式：
% lreverse {This is a tcltk  example}
example tcltk a is  This


**concat**?arg arg …?

此命令把每个参数连接在一起，并去掉第一个参数和最后一个参数前的前导和尾部空白。如果所有的参数都是列表，则把所有的列表并置为单一的列表。此命令可以有任意数量的参数arg；如果没有提供参数arg，则此命令的结果是一个空字符串。
**例子：**
concat会连接列表，所以下面的命令：
**concat**a b {c d e} {f {g h}}

会返回“a b c d e f {g h}”。此命令也可连接非列表的数据类型，因此下面的命令：
**concat**“a b {c“ d “e} f”

会返回“a b {c d e} f”。

注意，并置不会删除参数间的空白，所以命令：
**concat**“abc” { d e f}

会返回“abc d e f”（a，b，c之间有三个空格）。


**join**list ?joinString?

把列表的元素连接起来而创建一个字符串。参数list必须是效的Tcl列表。此命令返回格式化后的字符串（把列表list的所有元素用字符串joinString连接相邻的元素而构成）。参数joinString默认为是一个空格字符。

**例子：**

用逗号分隔列表的元素：
**set**data {1 2 3 4 5}
**join**$data ", "

→1, 2, 3, 4, 5

使用join去展平列表为单层的列表：
**set**data {1 {2 3} 4 {5 {6 7} 8}}
**join**$data

→1 2 3 4 5 {6 7} 8

**lappend**varName ?value value value …?

把值添加到列表中。此命令把变量varName视为列表，并且把每个参数value作为单独的元素添加到列表varName的值中，每个元素用空格分隔。如果varName不存在，则被创建为列表，而其值就是参数value。lappend与append相似，除了value是作为列表被添加而不是作为原始的文本。此命令提供了一个相对有效的方法构建一个大型的列表。例如，当$a很长时，“**lappend**a $b”比“**set**a [**concat**$a[**list**$b]]”更有效。

**例子：**

使用lappend去构建一个数字列表：
**set**var 1
à1

**lappend**var 2
à1 2

**lappend**var 3 4 5
à1 2 3 4 5

**lassign**list ?varName …?

此命令把列表list的元素相续按顺序地赋给参数varName。如果变量varName的个数比列表元素的个数多，则剩余的变量varName被设置为空字符串。如果列表元素的个数比变量varName的个数多，则此命令返回没有被赋给变量的元素。

**例子：**

下面的例子说明了是怎样多重赋值的，和当如果太少或太多元素会发生什么：
**lassign**{a b c} x y z;\# Empty return
**puts**$x;\# Prints "a"
**puts**$y;\# Prints "b"
**puts**$z;\# Prints "c"

**lassign**{d e} x y z;\# Empty return
**puts**$x;\# Prints "d"
**puts**$y;\# Prints "e"
**puts**$z;\# Prints ""

**lassign**{f g h i} x y;\# Returns "h i"
**puts**$x;\# Prints "f"
**puts**$y;\# Prints "g"

**lassign**命令也有其它的用法。它可以用于创建类似于其它shell语的“shift”命令：
**set**::argv [**lassign**$::argv argumentToReadOff]

**lindex**list ?index …?

检索列表中的元素。索引可以在命令行连续地提交，或组合为一个列表作为单个参数提交。如果没有提供索引，则命令采取的形式为：
**lindex**list或**lindex**list {}

在此情况中，lindex的返回值就是参数list的值。

当只提供单个索引时，lindex命令返回列表中第index个元素（列表元素的索引从0开始计算）。在提取元素时，lindex遵守大括号，双引号和反斜杠的规则；但，不会发生变量替换和命令替换。如果index是一个负数或者是大于或等于元素的数量，则返回一个字符串。对每个简单的index值的解释，与命令string index相同，支持简单的索引算术运算和相对于列表尾端的索引（用**end**表示列表的尾端）。

如果提供额外的参数index，则每个参数依次选取一个元素，允许脚本从子列表中选取元素。命令：
**lidnex**$a 1 2 3或**lindex**$a {1 2 3}

等同于：
**lindex**[**lindex**[**lindex**$a 1] 2 ] 3

**例子：**
列表可以从两端索引：
**lindex**{a b c} 0
→ a

**lindex**{a b c} 2
→ c

**lindex**{a b c} end
→ c

**lindex**{a b c} end-1
→ b

列表或索引的序列可以从列表中的列表选取元素：
**lindex**{a b c}
→ a b c

**lindex**{a b c} {}
→ a b c

**lindex**{{a b c} {d e f} {g h i}} 2 1
→ h

**lindex**{{a b c} {d e f} {g h i}} {2 1}
→ h

**lindex**{{{a b} {c d}} {{e f} {g h}}} 1 1 0
→ g

**lindex**{{{a b} {c d}} {{e f} {g h}}} {1 1 0}
→ g

列表索引也可以执行有限的计算，从其它索引中相加或减去固定的数量：
**set**idx 1
**lindex**{a b c d e f} $idx+2
àd

**set**idx 3
**lindex**{a b c d e f} $idx+2
àf

**linsert**list index ?element element …?

在列表中插入元素。此命令通过在列表list的第index个元素前插入所有的参数element而创建一个新的列表。每个参数element都会成为新列表中的一个单独元素。如果index小于或等于0，则新的元素插入列表的前头。对index值的解释，与命令string index相同，支持简单的索引算术运算和相对于列表尾端的索引（用**end**表示列表的尾端）。

**例子：**
**set**$a {0 1 2 3 4 5}
**linsert**$a 2 “hello”
à0 1 hello 2 3 4 5

把一些值放入一个列表中，首先从开头索引，然后从尾端索引，然后将他们链接一起：
**set**oldList {the fox jumps over the dog}
**set**midList [**linsert**$oldList 1 quick]
**set**newList [**linsert**$midList end-1 lazy]
\# The old lists still exist though...
**set**newerList [**linsert**[**linsert**$oldList end-1 quick] 1 lazy]

**list**?arg arg …?

创建一个列表。此命令返回由所有arg构成的列表，或者如果没有提供arg则返回一个空字符串。在需要时添加大括号和反斜杠，以便lindex命令可在此命令的结果上使用重新抽取原始的参数；并且也可以用eval执行生成的列表，arg1构成命令的名称，而其他args构成此arg1命令的参数。list生成的结果与concat稍微不同：concat会在形成列表前删除一层的组合，而list直接由原始的参数生成列表。

**例子：**
命令
**list**a b "c d e" "f {g h}"
会返回
a b {c d e} {f {g h}}

而相同参数的concat会返回
a b c d e f {g h}

**llength**list

返回列表list中的元素数量。
**例子：**

下列命令的结果是元素的数量：
%**llength**{a b c d e}
5
%**llength**{a b c}
3
%**llength**{}
0

不能保证元素完全是词典的词，尤其是当使用了引用：
%**llength**{a b {c d} e}
4
%**llength**{a b { } c d e}
6

空列表不是必须是字符串：
%**set**var { };**puts**"[**string length**$var],[**llength**$var]"
1,0

**lrange**list first last

返回列表中一个或多个相邻的元素。list必须是有效的Tcl列表。此命令返回一个新列表，新的列表由从列表list中的first到last的元素组成（包括first和last的元素）。对索引值first和last的解释与命令string index的索引值相同，支持简单的索引算术运算和相对于列表尾端的索引（用**end**表示列表的尾端）。如果first小于0，则它被视为0。如果last大于或等于list的元素数量，则它被视为是end。如果first比last大，则返回一个空字符串。注意，“**lrange**list first first”并不总是产生与“**lindex**list first”相同的结果（虽然对于没有用大括号括住的简单字段，此两个命令所产生的结果相同）；但“**lrange**list first first”与“**list**[**lindex**list first]” 产生的结果相同。

**例子：**
选择头两个元素：
%**lrange**{a b c d e} 0 1
a b

选择最后三个元素：
%**lrange**{a b c d e} end-2 end
c d e

选择除了第一个和最后一个元素外的所有元素：
%**lrange**{a b c d e} 1 end-1
b c d

用lrange选择单个元素与用lindex选择单个元素并不相同：
%**set**var {some {elements to} select}
some {elements to} select
%**lindex**$var 1
elements to
%**lrange**$var 1 1
{elements to}

**lrepeat**count ?element …?

此命令创建一个列表，此列表的元素就是重复count次的元素的序列element…。count必须是一个非负整数，element可以任何的Tcl值。注意，“**lrepeat**1 element…”等同于
“**list**element…”

**例子：**
**lrepeat**3 a
→ a a a

**lrepeat**3 [**lrepeat**3 0]
→ {0 0 0} {0 0 0} {0 0 0}

**lrepeat**3 a b c
→ a b c a b c a b c

**lrepeat**3 [**lrepeat**2 a] b c
→ {a a} b c {a a} b c {a a} b c

**lreplace**list first last ?element element…?

lreplace返回一个新列表，通过用参数element代替一个或多个list的元素而形成的新列表。索引值first和last指定了代替范围的第一个和最后一个元素。对索引值first和last的解释与命令string index的索引值相同，支持简单的索引算术运算和相对于列表尾端的索引（用**end**表示列表的尾端）。0表示列表的第一个元素，而end表示列表的最后一个元素。如果list为空，则忽略first和last。

如果first小于0，则它被视为指向列表第一个元素的前面。对于非空列表，first指示的元素必须存在，或者first必须指向列表的开头。

如果last小于first，则任何指定的元素都会被插入到由first指定的位置中，而没有元素被删除。

参数element指定了零个或多个新的参数，添加到列表中那些已被删除的元素的位置处。每个参数element将成为一个单独的列表元素。如果没有指定参数element，则first和last间的元素只是简单地被删除。如果list为空，则任何的参数element被添加到列表的末尾。

**例子：**
用一个参数element代替列表中的一个元素：
%**lreplace**{a b c d e} 1 1 foo
àa foo c d e

用三个参数element代替列表中的两个元素：
%**lreplace**{a b c d e} 1 2 three more elements
àa three more elements d e

删除列表中的最后一个元素：
%**set**var {a b c d e}
àa b c d e

%**set**var [**lreplace**$var end end]
àa b c d

从列表中删除一个给定的元素的过程：
**proc**lremove {listVariable value} {
**upvar**1 $listVariable var
**set**idx [**lsearch**-exact $var $value]
**set**var [**lreplace**$var $idx $idx]
}

**lreverse**list

此命令返回一个列表，其元素与列表list相同，但元素的顺序相反。

**例子：**
**lreverse**{a a b c}
→ c b a a
**lreverse**{a b {c d} e f}
→ f e {c d} b a

**lsearch**?options? list pattern

查看列表是否包含一个特定的元素。这个命令搜索列表的元素，查看他们是否与pattern匹配。如果匹配，则命令返回第一个匹配的元素的索引(除非指定了选项-all或-inline)。如果没有匹配的，则命令返回-1。参数option指示列表的元素是怎么样与pattern相匹配的，并且option的值必须是下面其中之一：

**MATCHING STYLE. OPTIONS****匹配风格的选项**
如果省略所有的匹配风格选项，则默认的匹配风格是-glob。如果指定了多种匹配风格，则最后的匹配风格起作用。
**-exact**
pattern是一个字面值字符串，与每个表元素进行精确的比较。

**-glob**
pattern是一个glob风格的模式，例用string match命令相同的规则与每个表元素比较。

**-regexp**
pattern被视为一个正则表达式，使用在re_syntax所描述的规则与每个表元素比较。

**-sorted**
列表的元素按顺序排序。如果指定这个选项，则lsearch将使用一个更加有效的搜索算法来搜索列表。如果没有指定其它选项，则列表list被假定为按升序排序，并且包含ASCII字符串。这个选项与-glob和-regexp互斥，当指定-all或-not时，被视为与-exact完全一样。

**GENERAL MODIFIER OPTIONS****一般的修饰词选项**

这些选项可以与所有的匹配式样联用。
**-all**
把结果改变为是列表中所有匹配的索引(或者如果也指定了-inline，则是列表中所有匹配的值)。如果返回的是索引，则索引是以数值排序的。如果返回的是值，则值的顺序将是输入列表list中的那些值的顺序。

**-inline**
返回的是匹配的值而不是匹配的索引(或者是一个空字符串，如果没有值匹配的话)。如果也指定了-all，则此命令的结果是列表中所有匹配的值。

**-not**
这是对匹配的否定，返回的是列中第一个非匹配的值的索引。

**-start**index
在位置index处开始搜索列表。对index值的解释与命令string index相同，支持简单的索引算术运算和相对于列表尾端的索引。

**CONTENTS DESCRIPTION OPTIONS****内容描述的选项**
这些选项描述列表中被搜的项目是怎样被解释的。当它们与-exact和-sorted选项联用时，它们才是有意义的。如果指定了多个选项，则最后一个选项起作用。默认是-ascii。
**-ascii**
列表的元素是作为Unicode字符串被检验(这为向后兼容性的原因)。

**-dictionary**
列表的元素使用字典风格式的比较而进行比较(对于更完整的描述参考lsort)。注意，当给出-sorted选项时，此选项与-ascii选项才有有意义的差别，因为当精确相等时值才是字典式的相等。

**-integer**
列表元素作为整数比较。

**-nocase**
比较以忽略大小写的方式处理。如果与-dictionary，-integer或-real选项联用时，则没有效果。

**-real**
列表元素作为浮点值比较。

**SORTED LIST OPTIONS****用于排序的列表选项**
这些选项(与-sorted选项联用时才有意义)指定列表是怎样被排序的。如果给出多个选项，则最后一个起作用。默的选项是-increasing。
**-decreasing**
列表的元素以降序排序。当与-sorted联用时，此选项才有意义。

**-increasing**
列表元素以升序排序。当与-sorted联用时，此选项才有意义。

**-bisect**
当列表元素按顺序排序时，进行不精确的搜索。对于升序的列表，小于或等于模式的最后一个索引的元素被返回。对于降序的列表，大于或等于模式的最后一个索引的元素被返回。如果模式是在第一个元素之前或列表是空列表，则返回-1。此选项暗示（隐式地使用）-sorted，不能与-all或-not联用。

**NESTED LIST OPTIONS****嵌套的列表选项**
这些选项被用于搜索列表的列表。它们可以与任何其它的选项联用。
**-index**indexList
此选项是用于搜索嵌套的列表。参数indexList给出了在每个元素中的索引的路径(可能与lindex或lset命令联用)，以允许被匹配的术语的位置。

**-subindices**
如果给出了此选项，此命令的索引结果(或者当也指定了-all时，每个索引结果)将是在整个列表内找到的项目的完整的路径(适用于与lindex或lset联用)。除非也指定了-index，否则此选项没有效果，它只是一个方便的快捷方式。

**例子：**
基本的搜索：
lsearch {a b c d e} c
→2

lsearch -all {a b c a b c} c
→2 5

使用lsearch过滤列表：
lsearch -inline {a20 b35 c47} b*
→b35

lsearch -inline -not {a20 b35 c47} b*
→a20

lsearch -all -inline -not {a20 b35 c47} b*
→a20 c47

lsearch -all -not {a20 b35 c47} b*
→0 2

甚至可以执行“set-like”的删除操作：
lsearch -all -inline -not -exact {a b c a d e a f g a} a
→b c d e f g

Searching may start part-way through the list:
可以用部分的方式开始搜索整个列表：
lsearch -start 3 {a b c a b c} c
→5

也可以搜索内部的元素：
lsearch -index 1 -all -inline {{a abc} {b bcd} {c cde}} *bc*
→{a abc} {b bcd}

**lset**varName ?index ...? newValue

更改列表中的元素。varName它是一个Tcl列表。索引可以在命令行连续的提供或组合为一个列表作为单个参数提供。newValue是列表narName中某一元素的新值。如果没有提供索引，则命令的形式为：
lset varName newValue
或
lset varName {} newValue

在这种情况中，newValue代替变量varName的旧值。
当只提供一个索引时，它寻址列表varName中第index个元素。当作列表解释的varName，lset遵守有关作为Tcl命令解释器的花括号、双引号和反斜线的相同规则；然而，不发生变量替换和命令替换。此命令构造一个新的列表，指明的元素被newValue代替。这个新列表存储在变量varName中，并且作为lset命令的返回值。
如果index是负数，或大于$varName的元素数量，则发生一个错误。如果index等于$varName的元素数量，则给定的元素（newValue）被添加到列表末尾。
对于index值的解释与命令string index相同，支持简单的索引算术运算和相对于列表的尾端的索引。
如果提供多个参数index，则每个参数依次被用于在由前一个索引操作指定的sublist中寻址一个元素(即是索引列表中的子列表的元素)，允许脚本改变在sublists中的元素(或将元素添加到sublists中)。命令，

lset a 1 2 newValue
或
lset a {1 2} newValue

用newValue代替sublist 1的索引为2的元素 。

每个参数index必须是大于或等于零的整数。每个参数index必须小于或等于相应列表的长度。换句话说，lset命令只能通过添加一个元素而改变一个列表的大小(将新的元素添加到原列表的后面)。如果索引超出了允许的范围，则引发一个错误。
**例子：**
下面每个例的x，它的初始值为：
set x [list [list a b c] [list d e f] [list g h i]]
→{a b c} {d e f} {g h i}

下列每个命令的返回值也成为了x的新值(除了最后一个命令外，它发生错误，而x的值不变)。
lset x {j k l}
→j k l

lset x {} {j k l}
→j k l

lset x 0 j
→j {d e f} {g h i}

lset x 2 j
→{a b c} {d e f} j

lset x end j
→{a b c} {d e f} j

lset x end-1 j
→{a b c} j {g h i}

lset x 2 1 j
→{a b c} {d e f} {g j i}

lset x {2 1} j
→{a b c} {d e f} {g j i}

lset x {2 3} j
→list index out of range

在下列例子中，x的初始值为：

set x [list [list [list a b] [list c d]] \
[list [list e f] [list g h]]]
→{{a b} {c d}} {{e f} {g h}}

指示返回值也将成为x的新值。
lset x 1 1 0 j
→{{a b} {c d}} {{e f} {j h}}

lset x {1 1 0} j
→{{a b} {c d}} {{e f} {j h}}

**lsort**?options? list

排序列表的元素。此命令排序列表list的元素，返回一个按顺序排序的新列表。lsort命令的实现使用一个稳定排序具有O(n log n)性能特征的合并排序算法。

默认使用ASCII排序，结果以升序返回。然而，在列表list的前面，可以指定下列任何的选项，以控制排序的进程(接受唯一的缩写)：
**-ascii**
使用Unicode码位排序规则顺序(名称是为了向后兼容的原因)比较字符串。这是默认选项。

**-dictionary**
使用字典式的比较。这与-ascii一样，除了(a)忽略大小写（tie-breaker关系断路器除外），(b)如果两个字符串包含有嵌入的数字，数字作为整数比较而不是作为字符。例如，在-dictionary模式中，bigBoy排在bigbang和bigboy之间，而x10y排在x9y和x11y之间。

**-integer**
把列表元素转换成整数并使用整数比较。

**-real**
把列表元素转换成浮点数并使用浮点数比较。

**-command command**
使command作为一个比较命令。要比较两个元素，评估由命令组成的脚本，此命令带有两个元素作为额外的参数。大如果第一个元素小于，等于，或大于第二个元素，则此本应返回一个小于，等于，或大于0的整数。

**-increasing**
以升序对列表排序(最小的元素在第一)。这是默认。

**-decreasing**
以降序对列表排序（最大的元素在第一)。

**-indices**
返回列表中排序的索引，而不是索引对应的值。

**-index indexList**
如果指定了这个选项，则列表list中的每个元素它自己必须是一个适当的sublist（子列表）(除非使用了-stride)。不是基于整个sublists排序，而是lsort将从每个sublist中提取第indexList个元素(好像全部的元素和indexList被传递给了lindex)并且基于给定的元素排序，例如：

lsort -integer -index 1 \
{{First 24} {Second 18} {Third 30}}
返回{Second 18} {First 24} {Third 30}，

lsort -index end-1 {{a 1 e i} {b 2 3 f g} {c 4 5 6 d h}}
返回{c 4 5 6 d h} {a 1 e i} {b 2 3 f g}，而

lsort -index {0 1} {
{{b i g} 12345}
{{d e m o} 34512}
{{c o d e} 54321}
}
返回{{d e m o} 34512} {{b i g} 12345} {{c o d e} 54321}(因为e排在i的前面，而i排在o的前面)这个选项比使用-command完成同样的效果更有效。

**-stride**strideLength
如果指定了这个选项，列表被视为由strideLength个元素组成，这些组合要么由他们的第一个元素排序，或者，如果指定了-index选项，则由传递给-index(则由-index忽略)的第一个索引指示的每一组中的元素来排序。在各个组中的元素总是在同一位置。

列表的长度必须是strideLength乘以一个整数，至少是2。例如，
lsort -stride 2 {carrot 10 apple 50 banana 25}
返回“apple 50 banana 25 carrot 10”，而

lsort -stride 2 -index 1 -integer {carrot 10 apple 50 banana 25}
返回“carrot 10 banana 25 apple 50”。

**-nocase**
比较以忽略大小写的方式处理。如果与-dictionary，-integer，或-real联用，则没有效果。

**-unique**
如果指定这个选项，则只有列表中最后一组重复的元素被保留。注意，重复的元素由相对于在排序中使用的比较来确定。因此如果使用了-index 0，则{1 a}和{1 b}将被视为重复，而只有第二个元素，{1 b}，被保留。
**注意**
lsort的选项只是控制使用什么样的比较，而不必强制值它们本身实际上是什么。当要排序的列表少于两个元素时，这区别仅是显而易见的。
lsort命令是reentrant(可重入的)，这意味着lsort作为在-command选项中所使用的命令的实现的一部分是安全的。

**例子：**
使用ASCII排序法排序列表：
% lsort {a10 B2 b1 a1 a2}
àB2 a1 a10 a2 b1

使用字典排序法排序列表：
% lsort -dictionary {a10 B2 b1 a1 a2}
àa1 a2 a10 b1 B2

排序整数的列表：
% lsort -integer {5 3 1 2 11 4}
à1 2 3 4 5 11

% lsort -integer {1 2 0x5 7 0 4 -1}
à-1 0 1 2 4 0x5 7

排序浮点数的列表：
% lsort -real {5 3 1 2 11 4}
à1 2 3 4 5 11

% lsort -real {.5 0.07e1 0.4 6e-1}
à0.4 .5 6e-1 0.07e1

使用索引排序
% \#注意c前的空格字符
% lsort {{a 5} { c 3} {b 4} {e 1} {d 2}}
à{ c 3} {a 5} {b 4} {d 2} {e 1}

% lsort -index 0 {{a 5} { c 3} {b 4} {e 1} {d 2}}
à{a 5} {b 4} { c 3} {d 2} {e 1}

% lsort -index 1 {{a 5} { c 3} {b 4} {e 1} {d 2}}
à{e 1} {d 2} { c 3} {b 4} {a 5}

排序字典：
% set d [dict create c d a b h i f g c e]
àc e a b h i f g

% lsort -stride 2 $d
àa b c e f g h i

使用-stride和多个索引排序：
% \#注意，第一个索引值是相对于组的
% lsort -stride 3 -index {0 1} {{Bob Smith} 25 Audi {Jane Doe} 40 Ford}
à{{Jane Doe} 40 Ford {Bob Smith} 25 Audi}

使用排序删除多个重复的值 ：
% lsort -unique {a b c a b c a b c}
àa b c

使用比较函数进行更复杂的排序：
% proc compare {a b} {
set a0 [lindex $a 0]
set b0 [lindex $b 0]
if {$a0 < $b0} {
return -1
} elseif {$a0 > $b0} {
return 1
}
return [string compare [lindex $a 1] [lindex $b 1]]
}

% lsort -command compare \
{{3 apple} {0x2 carrot} {1 dingo} {2 banana}}
{1 dingo} {2 banana} {0x2 carrot} {3 apple}

**split**string ?splitChars?

返回一个用参数splitChars中的每个字符折分string而形式的列表。结果列表中的每个元素由string中每个splitChars字符间的字符组成。如果string中包含两个连续的splitChars字符，则会产生一个空元素，或者string中的第一个字符或最后一个字符是splitChars中的字符，也会产生一个空元素。如果splitChars是空字符串，则string中的每个字符成为结果列表中的每个元素。splitChars默认为空格。
**例子：**
折分USENET组名为各个分层组件：
split "comp.lang.tcl" .
→ comp lang tcl
下面看看是如何用splitChars中的每个字符来折分string的，如果不小心可能会导致遗失某些信息：

split "alpha beta gamma" "temp"
→ al {ha b} {} {a ga} {} a

从一个格式化得不好列表字符串中提取列表的words：
split "Example with {unbalanced brace character"
→ Example with \{unbalanced brace character

字符串的每个字符成为列表的一个元素：
split "Hello world" {}
→ H e l l o { } w o r l d

**分析面向记录的文件**
分析Unix的/etc/passwd文件，一行一个记录，而每一行是用冒号分隔的字段列表：
\#\# Read the file
set fid [open /etc/passwd]
set content [read $fid]
close $fid

\#\# Split into records on newlines
set records [split $content "\n"]

\#\# Iterate over the records
foreach rec $records {
\#\# Split into fields on colons
set fields [split $rec ":"]

\#\# Assign fields to variables and print some out...
lassign $fields \
userName password uid grp longName homeDir shell
puts "$longName uses [file tail $shell] for a login shell"
}
lindex - 从列表中获得一个元素
### 语法
**lindex***list  ?index...?*

### 描述
**lindex**命令接受一个参数列表*list*，可以接受0个或者多个*index*参数，在多个参数的情况下，参数可以是单独的一次排列，也可以是在一个列表当中。
如果不指定index参数：
lindex list或者
lindex list {}这种情况下返回*lindex*列表本身。
当只有一个单独的元素时，**lindex**命令返回*list*列表中的第*index*个元素。替代时元素从0开始（也就是说索引0就是指列表的第一个元素），如果*index*是负数或者大于列表长度就返回一个空字符串。解释器在解释每一个*index*值时和**string  index**命令相同，都支持单个和多个*index*参数。

如果指定了多个index，将会选择列表的子列表中的元素。例如
lindex $a 1 2 3或者
lindex $a {1 2 3}与下面的命令相同
lindex [lindex [lindex $a 1] 2] 3

### 示例
```python
lindex {a b c} 
  → a b c 
lindex {a b c} {}
  → a b c 
lindex {a b c} 0 
  → a 
lindex {a b c} 2 
  → c 
lindex {a b c} end 
  → c 
lindex {a b c} end-1 
  → b 
lindex {{a b c} {d e f} {g h i}} 2 1 
  → h 
lindex {{a b c} {d e f} {g h i}} {2 1}
  → h 
lindex {{{a b} {c d}} {{e f} {g h}}} 1 1 0 
  → g 
lindex {{{a b} {c d}} {{e f} {g h}}} {1 1 0} 
  → g
```




