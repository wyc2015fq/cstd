
# linux awk 运算符 - Augusdi的专栏 - CSDN博客


2015年04月22日 17:41:25[Augusdi](https://me.csdn.net/Augusdi)阅读数：842个人分类：[AWK																](https://blog.csdn.net/Augusdi/article/category/2602913)


[linux awk 运算符](http://blog.csdn.net/ithomer/article/details/8476621)
**awk**作为文本处理优秀工具之一，它有自己丰富的运算符，可分为：算术运算符，赋值运算符，关系运算符，逻辑预算法，正则运算符。

**一、运算符介绍**
|运算符|描述|
|赋值运算符|
|=    +=    -=     *=    /=    %=     ^=|赋值语句|
|逻辑运算符|
||||逻辑或|
|&&|逻辑与|
|正则运算符|
|~   ~!|匹配正则表达式和不匹配正则表达式|
|关系运算符|
|<     <=   >    >=     !=     ==|关系运算符|
|算术运算符|
|+ -|加，减|
|*   /   &|乘，除与求余|
|+   -   !|一元加，减和逻辑非|
|^  ***|求幂|
|++    --|增加或减少，作为前缀或后缀|
|其它运算符|
|$|字段引用|
|空格|字符串连接符|
|? :|C条件表达式|
|in|数组中是否存在某键值|
说明：awk运算符基本与c语言类似，表达式及功能基本相同

**二、实例介绍**
**1）****awk赋值运算符**
awk 'BEGIN{a=5; a+=5; print a; a*=2; print a; a^=2; print a; a%=2; print a;}'
结果：
10\#  a+=5  ->  10
20\#  a*=5   ->  20
400\#  a^=2   ->  400
0\#  a%=2  ->  0

**2）****awk逻辑运算符**
awk 'BEGIN{a=1; b=2; print (a>0), (b<0), (a>5 && b<=2),(a>5 || b<=2);}'
结果：
1 0 0 1\#  true ： 1；  false ： 0

**3）****awk正则运算符**
awk 'BEGIN{a="100testa";if(a ~ /^100*/){print "ok";}}'
结果：
ok\# a 是否符合正则匹配： 字符串a是以100开头；  示例符合100开头，因此正则匹配成功

**4）****awk关系运算符**
awk 'BEGIN{a=11; if(a >= 9){print "ok";}}'\# 数值比较
结果：
ok

awk 'BEGIN{a="11"; if(a >= 9){print "ok";} else{print "no";}}'\# 字符串比较
结果：
no

说明：
> < 可以作为字符串比较，也可以用作数值比较，关键看操作数如果只要有一个是字符串，就会转换为字符串比较。两个都为数字，才转为数值比较。字符串比较，按照ascii码顺序比较原则。
如示例2 字符串比较，起始字符“1”在“9“之前，因此小于“9”，结果为“no”。

**5）****awk算术运算符**
awk 'BEGIN{a="12"; print a, a++,++a; print a+2, a*2, a^2, !a}'
结果：
12 12 14\# a = 12          a++ = 12          ++a = 14
16 28 196 0\# 14 + 2           14 * 2         14 ^ 2     ！14

**6）****其它运算符**
a） ? :
awk 'BEGIN{a="b"; print a=="b" ? "ok" : "err";}'
结果：
ok

b） in
awk 'BEGIN{a="b"; arr[0]="b"; arr["b"]="c"; print (a in arr); print arr[a];}'
结果：
1\#  arr下标中含有a（“b”），注意是下标，不是数值
c\#  arr[a] = arr["b"] = "c" ，先把a作为下标，然后根据下标取数值

c） 空格
awk 'BEGIN{arr="a b  c    d"; gsub(/ +/, "\#", arr); print arr;}'
结果：
a\#b\#c\#d\# 正则匹配替换多个空格为“\#”

d） $
echo "a b  c    d" | awk '{print $1, $3}'
结果：
a c\# 打印第1和第3个域值，默认以空格分割


**三、字符串操作**
awk中的数据类型是不需要定义，而是根据上下文语义自适应的，有时候需要强制转换。
**1） 字符串 -> 数字**
只需要将变量通过”+”连接运算，自动强制将字符串转为整型，非数字变成0（发现第一个非数字字符，后面自动忽略）
示例：awk 'BEGIN{a="100";b="10test1"; print (a+b+5);}'
结果： 115

**2） 数字 -> 字符串**
只需要将变量与””符号连接起来运算即可。
示例：awk 'BEGIN{a=100; b=100; c=(a" "b); print c; a="101"; b=102; print a""b;}'
结果：
100 100
101102

**3） 字符串连接**
字符串连接操作通”二“，”+”号操作符，模式强制将左右两边的值转为数字类型，然后进行操作。
示例： awk 'BEGIN{a="a"; b="b"; c=(a""b); print c}'
结果： ab

示例： awk 'BEGIN{a="a"; b="b"; c=(a+b); print a, b, c}'
结果： a b 0\# “+”号把字符转为数字，第一个非数字转为0，因此 0+0=0

**四、正则表达式**
awk作为文本处理工具，是少不了正则表达式的。正则表达式就像一门程序语言，有自己语法规则。
对于不同工具，其实大部分表示意思相同的。在linux众多文本处理工具（awk、sed、grep、perl)里面都用到正则表达式。正则表达式其实就只有3种类型，详细可以参考：**linux shell 正则表达式(BREs,EREs,PREs)差异比较**

**1） 正则式语法**
|字符|功能|
|+|指定一个或多个字符，或扩展正则表达式的具体值（在|+|（加号）前）在这个字符串中，则字符串匹配。
|命令行：|awk '/smith+ern/'   testfile|将包含字符|smit|，后跟一个或多个|h|字符，并以字符|ern|结束的字符串的任何记录打印至标准输出。
|示例|中的输出是：|smit|hern|, harry smit|hhern|, anne
|
|?|指定如果零个或一个字符或扩展正则表达式的具体值（在 ?（问号）之前）在字符串中，则字符串匹配。
|命令行：|awk '/smith?/'   testfile
|将包含字符|smit|，后跟零个或一个|h|字符的实例的所有记录打印至标准输出。
|示例中的输出是：smit|h|, alan smit|hern|, harry smit|hhern|, anne smitters, alexis|
|||指定如果以 |（垂直线）隔开的字符串的任何一个在字符串中，则字符串匹配。
|命令行：|awk '/allen | alan /'    testfile
|将包含字符串|allen|或|alan|的所有记录打印至标准输出。
|示例中的输出是：smiley,|allen|smith,|alan|
|( )|在正则表达式中将字符串组合在一起。
|命令行：|awk '/a(ll)?(nn)?e/'   testfile
|将具有字符串|ae|或|alle|或|anne|或|allnne|的所有记录打印至标准输出。
|示例中的输出是：smiley, allen smithhern, anne|
|{|m|}|指定如果正好有|m|个模式的具体值位于字符串中，则字符串匹配。
|命令行：|awk '/l{2}/'   testfile
|打印至标准输出|smiley, allen|
|{|m,|}|指定如果至少|m|个模式的具体值在字符串中，则字符串匹配。
|命令行：|awk '/t{2,}/'   testfile
|打印至标准输出：|smitters, alexis|
|{|m, n|}|指定如果|m|和|n|之间（包含的 m 和 n）个模式的具体值在字符串中（其中|m|<=|n|），则字符串匹配。
|命令行：|awk '/er{1, 2}/'   testfile
|打印至标准输出：|smithern, harry smithern, anne smitters, alexis|
|[|String|]|指定正则表达式与方括号内|String|变量指定的任何字符匹配。
|命令行：|awk '/sm[a-h]/'   testfile
|将具有|sm|后跟以字母顺序从|a|到|h|排列的任何字符的所有记录打印至标准输出。
|示例的输出是：|smawley, andy|
|[^|String|]|在 [ ]（方括号）和在指定字符串开头的 ^ (插入记号) 指明正则表达式与方括号内的任何字符|不|匹配。这样
|命令行：|awk '/sm[^a-h]/'   testfile
|打印至标准输出：|smiley, allen smith, alan smithern, harry smithhern, anne smitters, alexis|
|~,!~|表示指定变量与正则表达式匹配（代字号）或不匹配（代字号、感叹号）的条件语句。
|命令行|：|awk '$1 ~ /n/'   testfile
|将第一个字段包含字符|n|的所有记录打印至标准输出。
|示例中的输出是：|smithern, harry smithhern, anne|
|^|指定字段或记录的开头。
|命令行：|awk '$2 ~ /^h/'   testfile
|将把字符|h|作为第二个字段的第一个字符的所有记录打印至标准输出。
|示例中的输出是：|smithern, harry|
|$|指定字段或记录的末尾。
|命令行：|awk '$2 ~ /y$/'   testfile
|将把字符|y|作为第二个字段的最后一个字符的所有记录打印至标准输出。
|示例中的输出是：|smawley, andy smithern, harry|
|.|（句号）|表示除了在空白末尾的终端换行字符以外的任何一个字符。
|命令行：|awk '/a..e/'   testfile
|将具有以两个字符隔开的字符|a|和 e 的所有记录打印至标准输出。
|示例中的输出是：|smawley, andy smiley, allen smithhern, anne|
|*|（星号）|表示零个或更多的任意字符。
|命令行：|awk '/a.*e/'   testfile
|将具有以零个或更多字符隔开的字符|a|和 e 的所有记录打印至标准输出。
|示例中的输出是：|smawley, andy smiley, allen smithhern, anne smitters, alexis|
|\|(反斜杠)|转义字符。当位于在扩展正则表达式中具有特殊含义的任何字符之前时，转义字符除去该字符的任何特殊含义。例如，
|命令行：|/a\/\//
|将与模式 a // 匹配，因为反斜杠否定斜杠作为正则表达式定界符的通常含义。要将反斜杠本身指定为字符，则使用双反斜杠。有关反斜杠及其使用的更多信息，请参阅以下关于转义序列的内容。|
注： javascript、.net、java支持的正则表达式，基本上同上表

**2） 正则式实例**
a、 基本语法
awk ‘/REG/  {action}’
说明： /REG/为正则表达式，可以将当前一行记录$0中，满足条件的记录送入到 action进行处理

示例：awk 'BEGIN{info="this is a test"; if( info ~ /test/){print "ok"}}'\# ~ 正则匹配符，表示是否满足正则匹配
结果： ok

b、 正则函数
gsub( Ere, Repl, [ In ] )
sub( Ere, Repl, [ In ] )
match( String, Ere )
split( String, A, [Ere] )
详细用法，请参加[linux awk 内置函数实例](http://blog.csdn.net/sunboy_2050/article/details/8466007)



**参考推荐：**
[awk 实例](http://www.ibm.com/developerworks/cn/linux/shell/awk/awk-1/index.html)（IBM）
[沉于思考，默默学习](http://www.cnblogs.com/chengmo/tag/awk/)
[linux awk 内置函数实例](http://blog.csdn.net/sunboy_2050/article/details/8466007)
[linux awk 内置变量实例](http://blog.csdn.net/sunboy_2050/article/details/8477842)
[linux awk 数组和循环](http://blog.csdn.net/sunboy_2050/article/details/8478716)
[http://blog.csdn.net/ithomer/article/details/8476621](http://blog.csdn.net/ithomer/article/details/8476621)

