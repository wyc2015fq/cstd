
# R语言︱文本（字符串）处理与正则表达式 - 素质云笔记-Recorder... - CSDN博客

2016年04月11日 15:22:41[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：24278所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)



处理文本是每一种计算机语言都应该具备的功能，但不是每一种语言都侧重于处理文本。R语言是统计的语言，处理文本不是它的强项，perl语言这方面的功能比R不知要强多少倍。幸运的是R语言的可扩展能力很强，DNA/RNA/AA等生物序列现在已经可以使用R来处理。
|nchar|字符的个数|
|toupper|转换为大写字符|
|tolower|转换为小写字符|
|substr|求字符串的字串|
|grep|基于正则表达式的匹配|
|sub|基于正则表达式的替换|
|strsplit|字符串分割|
|paste|字符向量连接|
|match|匹配元素位置组成的向量|
R语言处理文本的能力虽然不强，但适当用用还是可以大幅提高工作效率的，而且有些文本操作还不得不用。高效处理文本少不了正则表达式（regular expression），虽然R在这方面先天不高效，但它处理字符串的绝大多数函数都使用正则表达式。

0、正则表达式简介：
正则表达式不是R的专属内容，所以用0编号，这里也只简单介绍，更详细的内容请查阅其他文章。

**正则表达式是用于描述/匹配一个文本集合的表达式。**

1.  所有英文字母、数字和很多可显示的字符本身就是正则表达式，用于匹配它们自己。比如 'a' 就是匹配字母 'a' 的正则表达式

2.  一些特殊的字符在正则表达式中不在用来描述它自身，它们在正则表达式中已经被“转义”，这些字符称为“元字符”。
perl类型的正则表达式中被转义的字符有：. \ | ( ) [ ] { } ^ $ * + ?。被转义的字符已经有特殊的意义，如点号 . 表示任意字符；
方括号表示选择方括号中的任意一个（如[a-z] 表示任意一个小写字符）；^ 放在表达式开始出表示匹配文本开始位置，放在方括号内开始处表示非方括号内的任一字符；大括号表示前面的字符或表达式的重复次数；| 表示可选项，即 | 前后的表达式任选一个。
3.  如果要在正则表达式中表示元字符本身，比如我就要在文本中查找问号‘?’， 那么就要使用引用符号（或称换码符号），一般是反斜杠 '\'。需要注意的是，在R语言中得用两个反斜杠即 ‘\\’，如要匹配括号就要写成 ’\\(\\)‘
4.  不同语言或应用程序（事实上很多规则都通用）定义了一些特殊的元字符用于表示某类字符，
如 \d 表示数字0-9，
\D 表示非数字，
\s 表示空白字符（包括空格、制表符、换行符等），
\S 表示非空白字符，
\w 表示字（字母和数字），
\W 表示非字，
\< 和 \> 分别表示以空白字符开始和结束的文本。
5.  正则表达式符号运算顺序：圆括号括起来的表达式最优先，然后是表示重复次数的操作（即：* + {} ），接下来是连接运算（其实就是几个字符放在一起，如abc），最后是表示可选项的运算（|）。所以 'foot|bar' 可以匹配’foot‘或者’bar‘，但是 'foot|ba{2}r'匹配的是’foot‘或者’baar‘。

|元字符
|描述
|
|\
|将下一个字符标记符、或一个向后引用、或一个八进制转义符。例如，“\\n”匹配\n。“\n”匹配换行符。序列“\\”匹配“\”而“\(”则匹配“(”。即相当于多种编程语言中都有的“转义字符”的概念。
|
|^
|匹配输入字符串的开始位置。如果设置了RegExp对象的Multiline属性，^也匹配“\n”或“\r”之后的位置。
|
|$
|匹配输入字符串的结束位置。如果设置了RegExp对象的Multiline属性，$也匹配“\n”或“\r”之前的位置。
|
|*
|匹配前面的子表达式任意次。例如，zo*能匹配“z”，也能匹配“zo”以及“zoo”。
|
|+
|匹配前面的子表达式一次或多次(大于等于1次）。例如，“zo+”能匹配“zo”以及“zoo”，但不能匹配“z”。+等价于{1,}。
|
|?
|匹配前面的子表达式零次或一次。例如，“do(es)?”可以匹配“do”或“does”中的“do”。?等价于{0,1}。
|
|{n}
|n是一个非负整数。匹配确定的n次。例如，“o{2}”不能匹配“Bob”中的“o”，但是能匹配“food”中的两个o。
|
|{n,}
|n是一个非负整数。至少匹配n次。例如，“o{2,}”不能匹配“Bob”中的“o”，但能匹配“foooood”中的所有o。“o{1,}”等价于“o+”。“o{0,}”则等价于“o*”。
|
|{n,m}
|m和n均为非负整数，其中n<=m。最少匹配n次且最多匹配m次。例如，“o{1,3}”将匹配“fooooood”中的前三个o。“o{0,1}”等价于“o?”。请注意在逗号和两个数之间不能有空格。
|
|?
|当该字符紧跟在任何一个其他限制符（*,+,?，{n}，{n,}，{n,m}）后面时，匹配模式是非贪婪的。非贪婪模式尽可能少的匹配所搜索的字符串，而默认的贪婪模式则尽可能多的匹配所搜索的字符串。例如，对于字符串“oooo”，“o+?”将匹配单个“o”，而“o+”将匹配所有“o”。
|
|.点
|匹配除“\r\n”之外的任何单个字符。要匹配包括“\r\n”在内的任何字符，请使用像“[\s\S]”的模式。
|
|(pattern)
|匹配pattern并获取这一匹配。所获取的匹配可以从产生的Matches集合得到，在VBScript中使用SubMatches集合，在JScript中则使用$0…$9属性。要匹配圆括号字符，请使用“\(”或“\)”。
|
|(?:pattern)
|非获取匹配，匹配pattern但不获取匹配结果，不进行存储供以后使用。这在使用或字符“(|)”来组合一个模式的各个部分是很有用。例如“industr(?:y|ies)”就是一个比“industry|industries”更简略的表达式。
|
|(?=pattern)
|非获取匹配，正向肯定预查，在任何匹配pattern的字符串开始处匹配查找字符串，该匹配不需要获取供以后使用。例如，“Windows(?=95|98|NT|2000)”能匹配“Windows2000”中的“Windows”，但不能匹配“Windows3.1”中的“Windows”。预查不消耗字符，也就是说，在一个匹配发生后，在最后一次匹配之后立即开始下一次匹配的搜索，而不是从包含预查的字符之后开始。
|
|(?!pattern)
|非获取匹配，正向否定预查，在任何不匹配pattern的字符串开始处匹配查找字符串，该匹配不需要获取供以后使用。例如“Windows(?!95|98|NT|2000)”能匹配“Windows3.1”中的“Windows”，但不能匹配“Windows2000”中的“Windows”。
|
|(?<=pattern)
|非获取匹配，反向肯定预查，与正向肯定预查类似，只是方向相反。例如，“(?<=95|98|NT|2000)Windows”能匹配“2000Windows”中的“Windows”，但不能匹配“3.1Windows”中的“Windows”。
|
|(?<!pattern)
|非获取匹配，反向否定预查，与正向否定预查类似，只是方向相反。例如“(?<!95|98|NT|2000)Windows”能匹配“3.1Windows”中的“Windows”，但不能匹配“2000Windows”中的“Windows”。这个地方不正确，有问题
|此处用或任意一项都不能超过2位，如“(?<!95|98|NT|20)Windows正确，“(?<!95|980|NT|20)Windows 报错，若是单独使用则无限制，如(?<!2000)Windows 正确匹配
|
|x|y
|匹配x或y。例如，“z|food”能匹配“z”或“food”(此处请谨慎)。“(z|f)ood”则匹配“zood”或“food”。
|
|[xyz]
|字符集合。匹配所包含的任意一个字符。例如，“[abc]”可以匹配“plain”中的“a”。
|
|[^xyz]
|负值字符集合。匹配未包含的任意字符。例如，“[^abc]”可以匹配“plain”中的“plin”。
|
|[a-z]
|字符范围。匹配指定范围内的任意字符。例如，“[a-z]”可以匹配“a”到“z”范围内的任意小写字母字符。
|注意:只有连字符在字符组内部时,并且出现在两个字符之间时,才能表示字符的范围; 如果出字符组的开头,则只能表示连字符本身.
|
|[^a-z]
|负值字符范围。匹配任何不在指定范围内的任意字符。例如，“[^a-z]”可以匹配任何不在“a”到“z”范围内的任意字符。
|
|\b
|匹配一个单词边界，也就是指单词和空格间的位置（即正则表达式的“匹配”有两种概念，一种是匹配字符，一种是匹配位置，这里的\b就是匹配位置的）。例如，“er\b”可以匹配“never”中的“er”，但不能匹配“verb”中的“er”。
|
|\B
|匹配非单词边界。“er\B”能匹配“verb”中的“er”，但不能匹配“never”中的“er”。
|
|\cx
|匹配由x指明的控制字符。例如，\cM匹配一个Control-M或回车符。x的值必须为A-Z或a-z之一。否则，将c视为一个原义的“c”字符。
|
|\d
|匹配一个数字字符。等价于[0-9]。grep 要加上-P，perl正则支持
|
|\D
|匹配一个非数字字符。等价于[^0-9]。grep要加上-Pperl正则支持
|
|\f
|匹配一个换页符。等价于\x0c和\cL。
|
|\n
|匹配一个换行符。等价于\x0a和\cJ。
|
|\r
|匹配一个回车符。等价于\x0d和\cM。
|
|\s
|匹配任何不可见字符，包括空格、制表符、换页符等等。等价于[ \f\n\r\t\v]。
|
|\S
|匹配任何可见字符。等价于[^ \f\n\r\t\v]。
|
|\t
|匹配一个制表符。等价于\x09和\cI。
|
|\v
|匹配一个垂直制表符。等价于\x0b和\cK。
|
|\w
|匹配包括下划线的任何单词字符。类似但不等价于“[A-Za-z0-9_]”，这里的"单词"字符使用Unicode字符集。
|
|\W
|匹配任何非单词字符。等价于“[^A-Za-z0-9_]”。
|
|\xn
|匹配n，其中n为十六进制转义值。十六进制转义值必须为确定的两个数字长。例如，“\x41”匹配“A”。“\x041”则等价于“\x04&1”。正则表达式中可以使用ASCII编码。
|
|\num
|匹配num，其中num是一个正整数。对所获取的匹配的引用。例如，“(.)\1”匹配两个连续的相同字符。
|
|\n
|标识一个八进制转义值或一个向后引用。如果\n之前至少n个获取的子表达式，则n为向后引用。否则，如果n为八进制数字（0-7），则n为一个八进制转义值。
|
|\nm
|标识一个八进制转义值或一个向后引用。如果\nm之前至少有nm个获得子表达式，则nm为向后引用。如果\nm之前至少有n个获取，则n为一个后跟文字m的向后引用。如果前面的条件都不满足，若n和m均为八进制数字（0-7），则\nm将匹配八进制转义值nm。
|
|\nml
|如果n为八进制数字（0-7），且m和l均为八进制数字（0-7），则匹配八进制转义值nml。
|
|\un
|匹配n，其中n是一个用四个十六进制数字表示的Unicode字符。例如，\u00A9匹配版权符号（&copy;）。
|
|\p{P}
|小写 p 是 property 的意思，表示 Unicode 属性，用于 Unicode 正表达式的前缀。中括号内的“P”表示Unicode 字符集七个字符属性之一：标点字符。
|其他六个属性：
|L：字母；
|M：标记符号（一般不会单独出现）；
|Z：分隔符（比如空格、换行等）；
|S：符号（比如数学符号、货币符号等）；
|N：数字（比如阿拉伯数字、罗马数字等）；
|C：其他字符。
|*注：此语法部分语言不支持，例：javascript。
|
|< >|匹配词（word）的开始（<）和结束（>）。例如正则表达式<the>能够匹配字符串"for the wise"中的"the"，但是不能匹配字符串"otherwise"中的"the"。注意：这个元字符不是所有的软件都支持的。|
|( )|将( 和 ) 之间的表达式定义为“组”（group），并且将匹配这个表达式的字符保存到一个临时区域（一个正则表达式中最多可以保存9个），它们可以用 \1 到\9 的符号来引用。|
|||将两个匹配条件进行逻辑“或”（Or）运算。例如正则表达式(him|her) 匹配"it belongs to him"和"it belongs to her"，但是不能匹配"it belongs to them."。注意：这个元字符不是所有的软件都支持的。|
|+|匹配1或多个正好在它之前的那个字符。例如正则表达式9+匹配9、99、999等。注意：这个元字符不是所有的软件都支持的。|
|?|匹配0或1个正好在它之前的那个字符。注意：这个元字符不是所有的软件都支持的。|
|{i} {i,j}|匹配指定数目的字符，这些字符是在它之前的表达式定义的。例如正则表达式A[0-9]{3} 能够匹配字符"A"后面跟着正好3个数字字符的串，例如A123、A348等，但是不匹配A1234。而正则表达式[0-9]{4,6} 匹配连续的任意4个、5个或者6个数字|
（摘自《[正则表达式之道](http://baike.baidu.com/view/1427403.htm)》）
————————————————————————————————————————————————————————


# 一、字符数统计和字符翻译

nchar这个函数简单，统计向量中每个元素的字符个数，注意这个函数和length函数的差别：
nchar是向量元素的字符个数，而length是向量长度（向量元素的个数）。其他没什么需要说的。

>x<-c("Hellow", "World", "!")
>nchar(x)
[1] 6 5 1
>length(''); nchar('')
[1] 1
[1] 0
另外三个函数用法也很简单：
>DNA<-"AtGCtttACC"
>tolower(DNA)
[1] "atgctttacc"
>toupper(DNA)
[1] "ATGCTTTACC"
>chartr("Tt", "Uu", DNA)
[1] "AuGCuuuACC"
>chartr("Tt", "UU", DNA)
[1] "AUGCUUUACC"

————————————————————————————————————————————————————————


# 二、字符串连接

paste应该是R中最常用字符串函数了，也是R字符串处理函数里面非常纯的不使用正则表达式的函数（因为用不着）。它相当于其他语言的strjoin，但是功能更强大。它把向量连成字串向量，其他类型的数据会转成向量，但不一定是你要的结果：
>paste("CK", 1:6,sep="")
[1] "CK1" "CK2" "CK3" "CK4" "CK5" "CK6"
>x<-list(a="aaa",b="bbb",c="ccc")
>y<-list(d=1,e=2)
>paste(x, y,sep="-")     \#较短的向量被循环使用
[1] "aaa-1" "bbb-2" "ccc-1"
>z<-list(x,y)
>paste("T", z,sep=":")   \#这样的结果不知合不合用
[1] "T:list(a= \"aaa\",b= \"bbb\",c= \"ccc\")"
[2] "T:list(d=1,e=2)"
短向量重复使用，列表数据只有一级列表能有好的表现，能不能用看自己需要。会得到什么样的结果是可以预知的，用as.character函数看吧，这又是一个字符串处理函数：
>as.character(x)
[1] "aaa" "bbb" "ccc"
>as.character(z)
[1] "list(a= \"aaa\",b= \"bbb\",c= \"ccc\")"
[2] "list(d=1,e=2)"
paste函数还有一个用法，设置collapse参数，连成一个字符串：
>paste(x, y,sep="-",collapse='; ')
[1] "aaa-1; bbb-2; ccc-1"
>paste(x,collapse='; ')
[1] "aaa; bbb; ccc"

————————————————————————————————————————————————————————


# 三、字符串拆分
strsplit函数使用正则表达式，使用格式为：strsplit(x, split, fixed = FALSE, perl = FALSE, useBytes = FALSE)
参数x为字串向量，每个元素都将单独进行拆分。
参数split为拆分位置的字串向量，默认为正则表达式匹配（fixed=FALSE）。如果你没接触过正则表达式，设置fixed=TRUE，表示使用普通文本匹配或正则表达式的精确匹配。普通文本的运算速度快。
perl=TRUE/FALSE的设置和perl语言版本有关，如果正则表达式很长，正确设置表达式并且使用perl=TRUE可以提高运算速度。
参数useBytes设置是否逐个字节进行匹配，默认为FALSE，即按字符而不是字节进行匹配。
下面的例子把一句话按空格拆分为单词：
`> text <- "Hello Adam!\nHello Ava!"
> strsplit(text, ' ')
[[1]]
[1] "Hello"        "Adam!\nHello" "Ava!"`
R语言的字符串事实上也是正则表达式，上面文本中的\n在图形输出中是被解释为换行符的。
`> strsplit(text, '\\s')
[[1]]
[1] "Hello" "Adam!" "Hello" "Ava!"`

strsplit得到的结果是列表，后面要怎么处理就得看情况而定了：
`> class(strsplit(text, '\\s'))
[1] "list"`

有一种情况很特殊：如果split参数的字符长度为0，得到的结果就是一个个的字符：
`> strsplit(text, '')
[[1]]
[1] "H"  "e"  "l"  "l"  "o"  " "  "A"  "d"  "a"  "m"  "!"  "\n" "H"  "e"  "l"  "l" 
[17] "o"  " "  "A"  "v"  "a"  "!"`

从这里也可以看到R把 \n 是当成一个字符来处理的。

————————————————————————————————————————————————————————

# 四、字符串查询：

1、grep和grepl函数：
这两个函数返回向量水平的匹配结果，不涉及匹配字符串的详细位置信息。
grep(pattern, x,ignore.case=FALSE,perl=FALSE,value=FALSE,
fixed=FALSE,useBytes=FALSE,invert=FALSE)
grepl(pattern, x,ignore.case=FALSE,perl=FALSE,
fixed=FALSE,useBytes=FALSE)
虽然参数看起差不多，但是返回的结果不一样。下来例子列出C:\windows目录下的所有文件，然后用grep和grepl查找exe文件：
>files<-list.files("c:/windows")
>grep("\\.exe$", files)
[1]   8  28  30  35  36  57  68  98  99 101 110 111 114 116
>grepl("\\.exe$", files)
[1] FALSE FALSE FALSE FALSE FALSE FALSE FALSE  TRUE FALSE FALSE FALSE FALSE FALSE
[14] FALSE FALSE FALSE FALSE FALSE FALSE FALSE FALSE FALSE FALSE FALSE FALSE FALSE
\#......
grep仅返回匹配项的下标，而grepl返回所有的查询结果，并用逻辑向量表示有没有找到匹配。两者的结果用于提取数据子集的结果都一样：
>files[grep("\\.exe$", files)]
[1] "bfsvc.exe"      "explorer.exe"   "fveupdate.exe"  "HelpPane.exe"
[5] "hh.exe"         "notepad.exe"    "regedit.exe"    "twunk_16.exe"
[9] "twunk_32.exe"   "uninst.exe"     "winhelp.exe"    "winhlp32.exe"
[13] "write.exe"      "xinstaller.exe"
>files[grepl("\\.exe$", files)]
[1] "bfsvc.exe"      "explorer.exe"   "fveupdate.exe"  "HelpPane.exe"
[5] "hh.exe"         "notepad.exe"    "regedit.exe"    "twunk_16.exe"
[9] "twunk_32.exe"   "uninst.exe"     "winhelp.exe"    "winhlp32.exe"
[13] "write.exe"      "xinstaller.exe"
2、regexpr、gregexpr和regexec
这三个函数返回的结果包含了匹配的具体位置和字符串长度信息，可以用于字符串的提取操作。
>text<-c("Hellow, Adam!", "Hi, Adam!", "How are you, Adam.")
>regexpr("Adam", text)
[1]  9  5 14
attr(,"match.length")
[1] 4 4 4
attr(,"useBytes")
[1] TRUE
>gregexpr("Adam", text)
[[1]]
[1] 9
attr(,"match.length")
[1] 4
attr(,"useBytes")
[1] TRUE
[[2]]
[1] 5
attr(,"match.length")
[1] 4
attr(,"useBytes")
[1] TRUE
[[3]]
[1] 14
attr(,"match.length")
[1] 4
attr(,"useBytes")
[1] TRUE
>regexec("Adam", text)
[[1]]
[1] 9
attr(,"match.length")
[1] 4
[[2]]
[1] 5
attr(,"match.length")
[1] 4
[[3]]
[1] 14
attr(,"match.length")
[1] 4


# 五、字符串替换

虽然sub和gsub是用于字符串替换的函数，但严格地说R语言没有字符串替换的函数，因为R语言不管什么操作对参数都是传值不传址。
>text
[1] "Hello Adam!\nHello Ava!"
>sub(pattern="Adam",replacement="world", text)
[1] "Hello world!\nHello Ava!"
>text
[1] "Hello Adam!\nHello Ava!"
可以看到：虽然说是“替换”，但原字符串并没有改变，要改变原变量我们只能通过再赋值的方式。
sub和gsub的区别是前者只做一次替换（不管有几次匹配），而gsub把满足条件的匹配都做替换：
>sub(pattern="Adam|Ava",replacement="world", text)
[1] "Hello world!\nHello Ava!"
>gsub(pattern="Adam|Ava",replacement="world", text)
[1] "Hello world!\nHello world!"
sub和gsub函数可以使用提取表达式（转义字符+数字）让部分变成全部：
> sub(pattern=".*(Adam).*", replacement="\\1", text)
[1] "Adam"

# 六、字符串提取
substr和substring函数通过位置进行字符串拆分或提取，它们本身并不使用正则表达式，但是结合正则表达式函数regexpr、gregexpr或regexec使用可以非常方便地从大量文本中提取所需信息。两者的参数设置基本相同：
substr(x, start, stop)
substring(text, first,last=1000000L)
第 1个参数均为要拆分的字串向量，第2个参数为截取的起始位置向量，第3个参数为截取字串的终止位置向量。但它们的返回值的长度（个数）有差 别：substr返回的字串个数等于第一个参数的长度；而substring返回字串个数等于三个参数中最长向量长度，短向量循环使用。先看第1参数（要 拆分的字符向量）长度为1例子：
>x<-"123456789"
>substr(x, c(2,4), c(4,5,8))
[1] "234"
>substring(x, c(2,4), c(4,5,8))
[1] "234"     "45"      "2345678"
因为x的向量长度为1，所以substr获得的结果只有1个字串，即第2和第3个参数向量只用了第一个组合：起始位置2，终止位置4。
而substring的语句三个参数中最长的向量为c(4,5,8)，执行时按短向量循环使用的规则第一个参数事实上就是c(x,x,x)，第二个参数就成了c(2,4,2)，最终截取的字串起始位置组合为：2-4, 4-5和2-8。
请按照这样的处理规则解释下面语句运行的结果：
>x<-c("123456789", "abcdefghijklmnopq")
>substr(x, c(2,4), c(4,5,8))
[1] "234" "de"
>substring(x, c(2,4), c(4,5,8))
[1] "234"     "de"      "2345678"
用substring函数可以很方便地把DNA/RNA序列进行三联拆分（用于蛋白质翻译）：
>bases<-c('A','T','G','C')
>DNA<-paste(sample(bases, 12,replace=T),collapse='')
>DNA
[1] "CCTTTACGGTGT"
>substring(DNA, seq(1,10,by=3), seq(3,12,by=3))
[1] "CCT" "TTA" "CGG" "TGT"
用regexpr、gregexpr或regexec函数获得位置信息后再进行字符串提取的操作可以自己试试看。
# 七、其他：
比如strtrim、strwrap、charmatch、match和pmatch等，甚至是 %in% 运算符都是可以使用的。R的在线帮助很全，自己看吧，就当学习E文。
原文链接：[http://helloxxxxxx.blog.163.com/blog/static/21601509520133492033667/?latestBlog](http://helloxxxxxx.blog.163.com/blog/static/21601509520133492033667/?latestBlog)

————————————————————————————————————————————————————————


## 字符串替换函数：chartr( )
chartr( )函数：将原有字符串中特定字符替换成所需要的字符。
其中参数old表示原有字符串中内容；new表示替换后的字符内容。

## 大小写替换函数：toupper( )、tolower( )、casefold( )
toupper( )函数：将字符串统一转换为大写。
tolower( )函数：将字符串统一转换为小写。
casefold( )函数：根据参数转换大小写。
来自：[每R一点：R语言中的字符串处理函数](http://www.08kan.com/gwk/MzA3NDUxMjYzMA/208917055/3/32388af7b85aea0dfc89bfd959bbf433.html)




### strtrim函数

用于将字符串修剪到特定的显示宽度，其用法为strtrim(x, width)，返回字符串向量的长度等于x的长度。因为是“修剪”，所以只能去掉多余的字符不能增加其他额外的字符：如果字符串本身的长度小于 width，得到的是原字符串，别指望它会用空格或其他什么字符补齐：

strtrim(c("abcdef","abcdef","abcdef"),c(1, 5, 10))\#\# [1] "a" "abcde" "abcdef"strtrim(c(1, 123, 1234567), 4)\#\# [1] "1" "123" "1234"


### strwrap函数

该函数把一个字符串当成一个段落的文字（不管字符串中是否有换行符），按照段落的格式（缩进和长度）和断字方式进行分行，每一行是结果中的一个字符串。例如：

str1 <-"Each character stringinthe input is first split intoparagraphs\n(or lines containing whitespace only). The paragraphs are then\nformatted by breaking lines at word boundaries. The target\ncolumnsforwrapping lines and the indentation of the first and\nall subsequent lines of a paragraph can be controlled\nindependently."str2 <-rep(str1, 2)strwrap(str2, width = 80, indent = 2)

来自[R语言：文本（字符串）处理与正则表达式](http://blog.sina.com.cn/s/blog_69ffa1f90101sie9.html)


————————————————————————————————————————————————————————

# 八 一些遇见的问题
1、 顽疾“空格”
`sentence <- gsub("\\s", "", sentence)                #空白删除`
其中\r回车键，\n代表换行符，\s代表空格

2、英文字母、数字的正则表达式
[a-zA-Z\d]

3、一些不可删除的特殊字符以及 [] 的使用
譬如：
...
.
当去掉这个符号的时候，就会出现全部字符都去掉的现象。这个 .  代表遍历每个字，所以会出现去掉每个字的情况。
如果要去掉，则需要[]
[]的功能很强，可以查找一些特殊的符号：
[?]   [!]   [.]   [] [|] [:]  [_]
其中，[\W]大写的W代表非文字格式。
.
——————————————————————————————————
## 延伸一：文件夹名、文件名
来源：http://www.cnblogs.com/Bfrican/p/4458090.html

```python
# 当前目录
> getwd()
[1] "/home/conan/R/fs"
# 最底层子目录
> dirname("/home/conan/R/fs/readme.txt")
[1] "/home/conan/R/fs"
# 最底层子目录或文件名
> basename(getwd())
[1] "fs"
>  basename("/home/conan/R/fs/readme.txt")
[1] "readme.txt"
```



