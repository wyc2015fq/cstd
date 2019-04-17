# Stata: 正则表达式和文本分析 - stata连享会 - CSDN博客





2018年12月21日 09:30:30[arlionn](https://me.csdn.net/arlionn)阅读数：1674
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)









> 


作者：游万海 (福州大学) ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))
- Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![image](http://upload-images.jianshu.io/upload_images/7692714-b17f977a5bd8c1e7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

给你一份公司年报，如何快速地从中找出与数值有关的内容。好朋友让你推荐好看的电影，如何快速地从豆瓣网站下载到每部电影的评分。 这里涉及到的问题就是如何从文本数据中挖掘出所需要的信息。**Stata** 中的字符函数为这一操作的实现提供了便利，详细可以通过 `help string_functions` 查看具体的用法和实例。本文主要是针对字符函数里面的正则表达式函数 ( **regular expression** )。

**Stata** 14 之前的版本主要的正则表达式函数有：`regexm`，`regexr`和`regexs`， **reg** 代表regular，**ex**代表expression。匹配主要是基于 Henry Spencer’s NFA 算法， 与 POSIX.2 标准相似。

`regexm` : **m** 代表 **match**-----匹配。

`regexr` :  **r** 代表 **replace**-----替代。

`regexs` :  **s** 代表 **subexpression**-----截取。

**Stata**14 之前版本只能处理普通的 **ASCII** 字符，例如字母( a-z,A-z),数字 ( 0-9 )及普通的标点符号字符。**Stata** 14 之后的版本加强了编码转换 ( **unicode** )，能够处理其他非普通编码 **ASCII** 字符，如中文，日语和韩语等。相应的，**Stata** 14 引入了几个新的有关正则表达式命令：

**Stata** 14 版本主要的正则表达式函数有：`ustrregexm`，`ustrregexrf`，`ustrregexra` 和 `ustrregexs`，**unstr** 代表 **unicode string**。

`ustrregexm` : 匹配

`ustrregexrf` : **f** 代表 **first**。表示只替代第一次出现的匹配字符。

`ustrregexra` : **a** 代表 **all**。表示替代全部匹配到的字符。

`ustrregexs` : 截取

因此，**Stata** 14 加强了正则表达式的功能，可以根据 **序列**、**POSIX 字符类** 等进行匹配。下面我们将进行一一说明。



### **一. 命令基本语法**

本文以**Stata** 14 之前版本的三个主要正则表达式命令为例，对其语法进行说明，如下图所示：

![image](http://upload-images.jianshu.io/upload_images/7692714-f414bff6e8664f29.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

从上面语法可以看出，正确使用这些命令的一个关键点在于如何填写待匹配规则，比如想从文本中匹配出与数值有关的内容，可以用 **0-9**,那么命令可以写为：

```
clear
input str10 income 
"abc"
"ab"
"aa"
"abcd"
"aad"
"aab123"
"cdf12345"
"123"
"Abc"
end

. gen index1 = regexm(income,"[0-9]") /* [0-9] 表示数值*/
```

基于此，本文以下部分主要针对匹配内容的相关规定进行阐述，分别介绍 **元字符**，**序列**，**字符类**，**数量词**，**POSIX字符类**等进

行介绍。



### **二. 基本规则**

#### **(1)元字符**

元字符是指的一类特殊字符，包括 `*``+``?``^``$``|``(``)``[``]``{``}``\` 等。匹配这些字符需要在前面加上`\`。

例如：

```
clear
input str3 num str2 name str10 per str6 income
           -1       a          "10%"    "[9747"
            1       b          "62%"    "1,234"
            1       a          "53%"    "938.9"
           -1       c          "48,6%"  "*8344"
            2       d          "58%"    "2398"
           -2       e          "46%"    "-"
           -3       c          "78%"    "53822"
            3       d          "92,2%"  "na"
           -1       e          "65%"    "$28477"
            1       b          "3,6%"   "n/a"
end
```

若想匹配出包括 `[`，`*`，`$` 等特殊字符的部分，可以利用如下代码：

```
gen index  = regexm(income,"\[")   /*匹配包含[号的*/
gen index1 = regexm(income,"\\$")  /*匹配包含$号的*/
gen index2 = regexm(income,"[\$]")
gen index3 = regexm(income,"[$]")
gen index4 = regexm(income,"[`=char(36)']") /*利用charlist查看相应的代码*/
gen index5 = regexm(income,"\*")            /*匹配包含*号的*/
gen index6 = regexm(income,"[\*|\[]")       /*|表示或者,匹配包含[号或者*号的*/
list
```

需要特别注意的是 `$` 符号比较特殊，其在 Stata 中还可以表示全局宏的引用，所以可以不加 `\`。

#### **(2) 序列**

较为常用的序列主要有如下：

`\d` 匹配数字字符

`\D` 匹配非数字字符

`\s` 匹配间隔符（空格）

`\S` 匹配非间隔符（非空格）

`\w` 匹配单词字符

`\W` 匹配非单词字符

`\b` 匹配词界

`\B` 匹配非词界

##### **第一： `\D` 表示非数值，`\d` 表示数值**

回顾下前面的例子，若要从文本中匹配包括数值的部分，可以用 **0-9**，这里也可以用 `\d` 进行匹配。例如：

```
clear 
input str12 income
"123"
"acb"
"12a"
end

gen index1 = ustrregexm(income,"\D") /*\D表示非数值*/
gen index2 = ustrregexm(income,"\d") /*\D表示数值*/
```

##### **第二：`\w` 和 `\W` 表示单词和非单词字符。**

单词字符是包括下划线的任何单词字符(字母，数字，下划线，汉字),即 `[a-zA-Z_0-9]`

```
clear
input str64 income
"the dandelion war 2010"
end
gen make2 = income
gen make3 = income
replace make2 = ustrregexra(make2, "\w", "_")
replace make3 = ustrregexra(make2, "\W", "_")
```

##### **第三：`\b` 和 `\B` 是位置匹配符**

如果前面和后面的字符不全是 `\w` (字母，数字，下划线，汉字)，则匹配；反过来理解就是，如果 `\b` 的前面和后面都是 `\w`，则不匹配。

前面讲的匹配都是匹配内容，而这里是匹配**位置**。这组命令也是非常常用，请看如下例子。

```
clear
input str12 income
"abc"
"abc"
"1ab"
"1abc"
"ab_"
"ab"
end
```

上述数据中都包括了 **ab** 字符，若我们只想匹配最后一个，看如下命令是否可以：

```
gen index1 = ustrregexm(income,"ab") /*等价于gen index1 = regexm(income,"ab") */
```

```
list

     +-----------------+
     | income   index1 |
     |-----------------|
  1. |    abc        1 |
  2. |    abc        1 |
  3. |    1ab        1 |
  4. |   1abc        1 |
  5. |    ab_        1 |
     |-----------------|
  6. |     ab        1 |
     +-----------------+
```

从匹配结果来看，把所有包括 **ab** 的元素都匹配出来的，这是因为遵循了贪婪匹配( **greedy** )模式。在这种情况下，我们可以使用位置匹配符进行限定。

```
gen index2 = ustrregexm(income,"\bab") 
// Q: 也等价于 gen index2 = regexm(income,"\bab") 吗？
gen index3 = ustrregexm(income,"ab\b") 
gen index4 = ustrregexm(income,"\bab\b")
```

```
list

     +--------------------------------------------+
     | income   index1   index2   index3   index4 |
     |--------------------------------------------|
  1. |    abc        1        1        0        0 |
  2. |    abc        1        1        0        0 |
  3. |    1ab        1        0        1        0 |
  4. |   1abc        1        0        0        0 |
  5. |    ab_        1        1        0        0 |
     |--------------------------------------------|
  6. |     ab        1        1        1        1 |
     +--------------------------------------------+
```

哇，大家可以看到，index4 就是我们要的内容。这就是位置匹配符的神奇之处！此外，位置匹配符还包括 **^** 和 **$**,匹配开始和结束位置。

例如：

```
clear
input str10 income 
"abc"
"ab"
"aa"
"abcd"
"aad"
"a1"
"aab123"
"cdf12345"
"123"
end
gen index1 = regexm(income, "(^[a-z]+)([0-9]$)") /*^放在括号外表示以...开始;$表示结束*/
```

```
. list

     +-------------------+
     |   income   index1 |
     |-------------------|
  1. |      abc        0 |
  2. |       ab        0 |
  3. |       aa        0 |
  4. |     abcd        0 |
  5. |      aad        0 |
     |-------------------|
  6. |       a1        1 |
  7. |   aab123        0 |
  8. | cdf12345        0 |
  9. |      123        0 |
     +-------------------+
```

#### **(3) 字符类**

字符类主要包括如下：

`[aeiou]` 匹配任意元音字母

`[AEIOU]` 匹配任何一个大写元音

`[0123456789]` 匹配任意单个数字

`[0-9]` 匹配任意数字(同上)

`[a-z]` 匹配任何ASCII小写字母

`[A-Z]` 匹配任何ASCII大写字母

`[a-zA-Z0-9]` 匹配任意上面的类

`[^aeiou]` 匹配除小写元音外的字母

`[^0-9]` 匹配除数字外的字符

```
clear
input str10 income 
"abc"
"ab"
"aa"
"abcd"
"aad"
"aab123"
"cdf12345"
"123"
"Abc"
end
gen index1 = ustrregexm(income,"[0-9]") /* [0-9] 表示数值,是否可以用\d?*/
gen index2 = ustrregexm(income,"[a-z]") /* [a-z] 表示小写字母*/
gen index3 = ustrregexm(income,"[aeiou]") /* aeiou 表示元音 */
gen index4 = ustrregexm(income,"[^aeiou]") /*括号[]内时,^表示否定，即排除aeiou*/
gen index5 = ustrregexm(income,"[A-Z]") /* [A-Z] 表示大写字母*/
```

#### **(4) 表示数量**

表示数量有两类：

**第一类**：

`{n}` 前面待匹配的项目将匹配n个;

`{n,}` 前面待匹配的项目将匹配n个或更多个;

`{n,m}` 前面待匹配的项目将匹配至少n个最多m个;

**第二类**：

`?` 前面的待匹配的项目是可选的，且最多匹配一个

`*` 前面待匹配的项目可以匹配0个或更多个

`+` 前面待匹配的项目将匹配一个或多个

```
clear
input str10 income 
"abc"
"ab"
"aa"
"abcd"
"aad"
"a1"
"aab123"
"cdf12345"
"123"
end

gen index1 = ustrregexm(income, "[a]{1}") /*{1}表示数量，匹配a，{1}表示1次；这里是greedy匹配，只要出现>=1次就匹配*/
gen index2 = ustrregexm(income, "[a]{2}") 
gen index3 = ustrregexm(income, "[0-9]{2}")
gen index4 = ustrregexm(income, "[0-9]{3}")
gen index5 = ustrregexm(income, "[0-9]{4}")
gen index6 = ustrregexm(income, "[0-9]{1,3}")
gen index7 = ustrregexm(income, "[0-9]{4,5}")
gen index8 = ustrregexm(income, "[0-9]+") /*+表示1次或多次*/
gen index9 = ustrregexm(income, "[0-9]*") /*表示0次或多次*/
gen index10 = ustrregexm(income, "[0-9]?") /*表示0次或1次*/
gen index11 = ustrregexm(income, "^[0-9]") /*^放在括号外表示以...开始*/
gen index12 = ustrregexm(income, "(^[a-z]+)[1]$") /*若只想匹配 **a1**原始，以字母开头，以数字结束*/
gen index13 = ustrregexm(income, "(^[a-z]+)[0-9]$") /*效果等同于上句命令*/
```

#### **(5) POSIX字符类**

POSIX字符类是用 "[[ ]]“ 括起来的正则表达，常见的 POSIX 字符类有：

`[[:lower:]]` 小写字母

`[[:upper:]]` 大写字母

`[[:alpha:]]` 所有字母 ([[:lower:]] and [[:upper:]])

`[[:digit:]]` 数字: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

`[[:alnum:]]` 字母和数字 ([[:alpha:]] and [[:digit:]])

`[[:blank:]]` 空白字符: space and tab

`[[:cntrl:]]` 控制字符

`[[:punct:]]` 标点符号: ! ” # % & ’ ( ) * + , - . / : ;

`[[:space:]]` 空格字符:制表符,换行符, 垂直制表符,换页符,回车和空格

`[[:xdigit:]]` 十六进制数字: 0-9 A B C D E F a b c d e f

`[[:print:]]` 控制字符 ([[:alpha:]], [[:punct:]] and space)

`[[:graph:]]` 图形化字符 ([[:alpha:]] and [[:punct:]])

```
clear
input str10 income 
"abc"
"aB"
"aa"
"abcd"
"Aad"
"a1"
"aab123"
"cdf12345"
"123"
end
gen index1 = ustrregexm(income, "[[:lower:]]") /*小写字母*/
gen index2 = ustrregexm(income, "[[:upper:]]") /*大写字母*/
gen index3 = ustrregexm(income, "[[:digit:]]") /*大写字母*/
```

#### **(6) 回溯引用**

我们先看这个例子（例子数据来源于 `statalist` 论坛）：

```
clear
input strL report_text
"indication I want this 1 view"
"indication I want this 2 views"
"indications I want this 3 view"
"indications I want this 4 views"
"history I want this 5 view"
"history I want this 6 views"
"xxx I dont want this yyy"
"indication I dont want this either yyy"
"xxx nor this view"
end
```

若我们想取出 **I want this 1**， **I want this 2**等，大家发现与前面说的匹配存在什么不同？前述例子我们都是从多个数中匹配出符合条件的数，而这里是从每个数中取出符合条件的一部分。这时候可以采用回溯引用方法。即先根据字符数据，利用正则表达式将完整的字符匹配出来，然后利用**()**来取出我们感兴趣的部分。这时可以利用 `regexm` 和 `regexs` 函数。

0表示匹配出来的全部内容；1表示第一个括号的内容；2表示第二个括号的内容；3表示第三个括号的内容。例如：

```
gen indication0=regexs(0) if regexm(lower(report_text),"^(indications|indication|history)(.*)(views|view)$") /* .表示匹配任何字符*/
gen indication1=regexs(1) if regexm(lower(report_text),"^(indications|indication|history)(.*)(views|view)$")
gen indication2=regexs(2) if regexm(lower(report_text),"^(indications|indication|history)(.*)(views|view)$")
gen indication3=regexs(3) if regexm(lower(report_text),"^(indications|indication|history)(.*)(views|view)$")
list
```

```
. list, compress

     +------------------------------------------------------------------------------------------------------------------+
     |                            report_text                       indication0   indication1       indication2   ind~3 |
     |------------------------------------------------------------------------------------------------------------------|
  1. |          indication I want this 1 view     indication i want this 1 view    indication    i want this 1     view |
  2. |         indication I want this 2 views    indication i want this 2 views    indication    i want this 2    views |
  3. |         indications I want this 3 view    indications i want this 3 view   indications    i want this 3     view |
  4. |        indications I want this 4 views   indications i want this 4 views   indications    i want this 4    views |
  5. |             history I want this 5 view        history i want this 5 view       history    i want this 5     view |
     |------------------------------------------------------------------------------------------------------------------|
  6. |            history I want this 6 views       history i want this 6 views       history    i want this 6    views |
  7. |               xxx I dont want this yyy                                                                           |
  8. | indication I dont want this either yyy                                                                           |
  9. |                      xxx nor this view                                                                           |
     +------------------------------------------------------------------------------------------------------------------+

.
```

**思考**：

```
clear
input str10 income
"abc123"
"a1"
"abcdef12345"
end
```

如何取出其中的数值部分？

#### **(7) ()和[]的区别**

```
clear
input str10 income 
"abc"
"ab"
"aa"
"abcd"
"aad"
"aab123"
"cdf12345"
"123"
end
gen index1 = regexm(income,"[ab]") /*表示匹配a或者b*/
gen index2 = regexm(income,"(a|b)") /*表示匹配a或者b*/
gen index3 = regexm(income,"(ab)") /*表示匹配ab*/
```

#### **(8) 常用匹配**

(参考来源：[https://blog.csdn.net/wangjia55/article/details/7877915](https://blog.csdn.net/wangjia55/article/details/7877915))
- 匹配中文字符的正则表达式： `[\u4e00-\u9fa5]`

评注：匹配中文还真是个头疼的事，有了这个表达式就好办了
- 匹配双字节字符(包括汉字在内)：`[^\x00-\xff]`

评注：可以用来计算字符串的长度（一个双字节字符长度计2，ASCII字符计1）
- 匹配空白行的正则表达式：`\n\s*\r`

评注：可以用来删除空白行
- 匹配 HTML 标记的正则表达式：`<(\S*?)[^>]*>.*?</\1>|<.*? />`

评注：网上流传的版本太糟糕，上面这个也仅仅能匹配部分，对于复杂的嵌套标记依旧无能为力
- 匹配首尾空白字符的正则表达式：`^\s*|\s*$`

评注：可以用来删除行首行尾的空白字符(包括空格、制表符、换页符等等)
- 匹配 Email 地址的正则表达式：`\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*`

评注：表单验证时很实用
- 匹配网址 URL 的正则表达式：`[a-zA-z]+://[^\s]*`

评注：网上流传的版本功能很有限，上面这个基本可以满足需求
- 匹配帐号是否合法(字母开头，允许 5-16 字节，允许字母数字下划线)：`^[a-zA-Z][a-zA-Z0-9_]{4,15}$`

评注：表单验证时很实用
- 匹配国内电话号码：`\d{3}-\d{8}|\d{4}-\d{7}`

评注：匹配形式如 `0511-4405222` 或 `021-87888822`
- 匹配腾讯 QQ 号：`[1-9][0-9]{4,}`

评注：腾讯 QQ 号从 10000 开始
- 匹配中国邮政编码：`[1-9]\d{5}(?!\d)`

评注：中国邮政编码为 6 位数字
- 匹配身份证：`\d{15}|\d{18}`

评注：中国的身份证为 15 位或 18 位
- 匹配 ip 地址：`\d+\.\d+\.\d+\.\d+`

评注：提取 ip 地址时有用

> 
Stata 寒假班

连玉君-江艇 主讲，2019年1月17日-27日，北京


[Stata**初级班**](http://www.peixun.net/view/307.html) ｜ [Stata**高级班**](http://www.peixun.net/view/308.html)  |  
[Stata**论文班**](http://www.peixun.net/view/1135.html)  |  [Stata**全程班**](http://www.peixun.net/view/1224.html)


### 三. Stata 范例：利用正则表达式爬取豆瓣影评数据

说明：以下例子我主要是利用了外部命令 `moss`，大家可以用上述讲过的命令试试

```
clear
import delimited "https://movie.douban.com/top250?start=25&filter=",delimiters("^") varnames(1) rowrange(3) encoding("UTF-8") clear

**评价人数 net install moss, from(http://fmwww.bc.edu/RePEc/bocode/m)
moss doctypehtml, match("(人评价)") regex  prefix(c_)
gen comment_num = real(regexs(1)) if regexm(doctypehtml,"([0-9]+)") & c_count==1

tempfile comment_num score title year comment
preserve
drop if comment_num==.
keep comment_num
save `comment_num',replace
restore
**use `comment_num',clear

**评分
moss doctypehtml, match("(v:average)") regex prefix(s_)
gen score = real(regexs(1)) if regexm(doctypehtml,"([0-9][\.][0-9])") & s_count==1
preserve
drop if score==.
keep score
save `score',replace
restore
**use `score',clear

**标题
gen title1 = regexs(1) if regexm(doctypehtml,"(\<span(.+)title(.*)\>$)")==1
**gen title6 = regexs(1) if regexm(doctypehtml,"(\<)span(.+)(.*)title(\>)(^ )(.+)")==1
gen title2 = title1 if regexm(title1,"(\ )")==1
gen title=(title1~=title2)
gen title_for = doctypehtml if title==1
preserve
drop if title_for==""
drop in 1
keep title_for
split title_for,parse(> <)
keep title_for3
save `title',replace
restore

**年份
gen year= real(regexs(1)) if regexm(doctypehtml,"([0-9][0-9][0-9][0-9])(\ )") 
preserve
drop if year==.
keep year
save `year',replace
restore

**精选评论
gen comment_text= regexs(0) if regexm(doctypehtml,"(\<)span(.+)inq") 
gen comment = doctypehtml if comment_text~=""
preserve
drop if comment==""
keep comment 
split comment,parse(> 。)
keep comment2
save `comment',replace
restore

use `comment_num',clear
merge 1:1 _n using `year',nogenerate 
merge 1:1 _n using `comment',nogenerate
merge 1:1 _n using `title',nogenerate
merge 1:1 _n using `score',nogenerate

**split comment,parse(> 。)
drop if year==.
list,table
order title_for3 year score comment_num comment2
rename (title_for3 year score comment_num comment2) (电影名称 出版年份 电影评分 评论人数 经典评论)
```

```
--------------------------------------------------------------------------------------------------------------------------
               电影名称   出版年份   电影评分    评论人数                                                  经典评论
     |---------------------------------------------------------------------------------------------------------------------
  1. |           乱世佳人    1939     9.2   338897                                            Tomorrow is another day
  2. |    蝙蝠侠：黑暗骑士    2008     9.1   445292                                                           无尽的黑暗
  3. |               活着    1994     9.2   367641                                                    张艺谋最好的电影
  4. |          天堂电影院    1988    9.1   371147                那些吻戏，那些青春，都在影院的黑暗里被泪水冲刷得无比清晰
  5. |    少年派的奇幻漂流    2012     9   731586                                           瑰丽壮观、无人能及的冒险之旅
     |---------------------------------------------------------------------------------------------------------------------
  6. |           十二怒汉    1957     9.4   218126                                                     1957年的理想主义
  7. |           鬼子来了    2000     9.2   303688                                           对敌人的仁慈，就是对自己残忍
  8. |  指环王3：王者无敌    2003     9.1   363564                                                            史诗的终章
  9. |           控方证人    1957     9.6   149201                                                     比利·怀德满分作品
 10. |           天空之城    1986       9   423985                                                 对天空的追逐，永不停止
     |---------------------------------------------------------------------------------------------------------------------
 11. |         搏击俱乐部    1999       9   478422                        邪恶与平庸蛰伏于同一个母体，在特定的时间互相对峙
 12. |         飞屋环游记    2009     8.9   645980                                 最后那些最无聊的事情，才是最值得怀念的
 13. |  大话西游之月光宝盒    1995     8.9   544247                                                             旷古烁今
 14. |           罗马假日    1953       9   488403                                                     爱情哪怕只有一天
 15. |       摔跤吧！爸爸    2016     9.1   661737    你不是在为你一个人战斗，你要让千千万万的女性看到女生并不是只能相夫教子
     |---------------------------------------------------------------------------------------------------------------------
 16. |           窃听风暴    2006     9.1   289688                                                            别样人生
 17. |     哈尔的移动城堡    2004     8.9   461128                                                带着心爱的人在天空飞翔
 18. |             辩护人    2013     9.2   287409                                            电影的现实意义大过电影本身
 19. |         闻香识女人    1992       9   414805                                                       史上最美的探戈
 20. |         两杆大烟枪    1998     9.1   327431                               4个臭皮匠顶个诸葛亮，盖·里奇果然不是盖的
     |---------------------------------------------------------------------------------------------------------------------
 21. |         飞越疯人院    1975       9   331838                                                            自由万岁
 22. |           死亡诗社    1989       9   355404   当一个死水般的体制内出现一个活跃的变数时，所有的腐臭都站在了光明的对面
 23. |          V字仇杀队    2005     8.8   538168                                             一张面具背后的理想与革命
 24. |             海豚湾    2009     9.3   218285                                     海豚的微笑，是世界上最高明的伪装
 25. |              教父2    1974     9.1   242211                                                         优雅的孤独
     +---------------------------------------------------------------------------------------------------------------------
```

**思考**：上述程序只爬取了第一页的数据，若要爬取前10页的数据(^^这里仅做科研目的，大家悠着点，不要爬虫的太频繁)，应该如何？

```
tempfile building
save `building', emptyok

scalar web1="https://movie.douban.com/top250?start="
scalar web3="&filter="
forv i = 10(-1)1{
local k = (`i'-1)*25
local url=web1 + "`k'" + web3
*scalar list url
di `"`url'"'
import delimited  "`url'",delimiters("^") varnames(1) rowrange(3) encoding("UTF-8") clear

**评价人数 net install moss, from(http://fmwww.bc.edu/RePEc/bocode/m)
moss doctypehtml, match("(人评价)") regex  prefix(c_)
gen comment_num = real(regexs(1)) if regexm(doctypehtml,"([0-9]+)") & c_count==1

tempfile comment_num score title year comment
preserve
drop if comment_num==.
keep comment_num
save `comment_num',replace
restore

**评分
moss doctypehtml, match("(v:average)") regex prefix(s_)
gen score = real(regexs(1)) if regexm(doctypehtml,"([0-9][\.][0-9])") & s_count==1
preserve
drop if score==.
keep score
save `score',replace
restore


**标题
gen title1 = regexs(1) if regexm(doctypehtml,"(\<span(.+)title(.*)\>$)")==1
**gen title6 = regexs(1) if regexm(doctypehtml,"(\<)span(.+)(.*)title(\>)(^ )(.+)")==1
gen title2 = title1 if regexm(title1,"(\ )")==1
gen title=(title1~=title2)
gen title_for = doctypehtml if title==1
preserve
drop if title_for==""
drop in 1
keep title_for
split title_for,parse(> <)
keep title_for3
save `title',replace
restore


**年份
gen year= real(regexs(1)) if regexm(doctypehtml,"([0-9][0-9][0-9][0-9])(\ )") 
preserve
drop if year==.
keep year
save `year',replace
restore

**精选评论
gen comment_text= regexs(0) if regexm(doctypehtml,"(\<)span(.+)inq") 
gen comment = doctypehtml if comment_text~=""
preserve
drop if comment==""
keep comment 
split comment,parse(> 。)
keep comment2
save `comment',replace
restore

use `comment_num',clear
merge 1:1 _n using `year',nogenerate 
merge 1:1 _n using `comment',nogenerate
merge 1:1 _n using `title',nogenerate
merge 1:1 _n using `score',nogenerate
drop if year==.
append using `building'
save `"`building'"', replace
}
list in 1/10
use `"`building'"',clear
```



### 四、总结
- 若用 **Stata** 14以后的版本，尽量用 **ustr** 开头的命令，功能较为齐全。
- 学习正则表达式要多练习，多练习，多练习！！



### 参考资料
- [How can I extract a portion of a string variable using regular expressions?](https://stats.idre.ucla.edu/stata/faq/how-can-i-extract-a-portion-of-a-string-variable-using-regular-expressions/)
- [What are regular expressions and how can I use them in Stata?](https://www.stata.com/support/faqs/data-management/regular-expressions/)
- [New program for regular expressions](https://www.statalist.org/forums/forum/general-stata-discussion/general/1327564-new-program-for-regular-expressions)
- [Stata14 VS Stata13 之字符串函数 PK](https://stata-club.github.io/%E6%8E%A8%E6%96%87/20160923/)
- [Regular Expressions in Stata](https://soc596.blogspot.com)
- [Regular Expression Matching Can Be Simple And Fast](https://swtch.com/~rsc/regexp/regexp1.html)
- [在线正则表达式测试](http://tool.oschina.net/regex)



### 附录：推文 dofile 合集

[【Stata: 正则表达式和文本分析】推文中的所有 dofile 文档](https://gitee.com/Stata002/StataSX2018/blob/master/%E6%B8%B8%E4%B8%87%E6%B5%B7/%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F/%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F%E9%99%84%E5%BD%95%E7%A8%8B%E5%BA%8F.md)



> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [CSDN-Stata连享会](https://blog.csdn.net/arlionn) 、[简书-Stata连享会](http://www.jianshu.com/u/69a30474ef33) 和 [知乎-连玉君Stata专栏](https://www.zhihu.com/people/arlionn)。可以在上述网站中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
#### 往期精彩推文

- [Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)
- [Stata连享会推文列表2](https://gitee.com/arlionn/jianshu/blob/master/README.md)
- Stata连享会 [精品专题](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-31a54cc963600564.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)









