
# java的日期类Date和日期的格式化 - 李昆鹏的博客 - CSDN博客


2018年03月16日 22:30:20[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：354


**-----------------java的日期类Date和日期的格式化-------------------**
## java.util
## 类 Date
java.lang.Object**java.util.Date****所有已实现的接口：**
Serializable,Cloneable,Comparable<Date>
**直接已知子类：**
Date,Time,Timestamp
---

public class**Date**extendsObjectimplementsSerializable,Cloneable,Comparable<Date>类`Date`表示特定的瞬间，精确到毫秒。
在 JDK 1.1 之前，类`Date`有两个其他的函数。它允许把日期解释为年、月、日、小时、分钟和秒值。它也允许格式化和解析日期字符串。不过，这些函数的 API 不易于实现国际化。从 JDK 1.1 开始，应该使用`Calendar`类实现日期和时间字段之间转换，使用`DateFormat`类来格式化和解析日期字符串。`Date`中的相应方法已废弃。
尽管`Date`类打算反映协调世界时 (UTC)，但无法做到如此准确，这取决于 Java 虚拟机的主机环境。当前几乎所有操作系统都假定 1 天 =24 × 60 × 60 = 86400 秒。但对于 UTC，大约每一两年出现一次额外的一秒，称为“闰秒”。闰秒始终作为当天的最后一秒增加，并且始终在 12 月 31 日或 6 月 30 日增加。例如，1995 年的最后一分钟是 61 秒，因为增加了闰秒。大多数计算机时钟不是特别的准确，因此不能反映闰秒的差别。
一些计算机标准是按照格林威治标准时 (GMT) 定义的，格林威治标准时和世界时 (UT) 是相等的。GMT 是标准的“民间”名称；UT 是相同标准的“科学”名称。UTC 和 UT 的区别是：UTC 是基于原子时钟的，UT 是基于天体观察的，两者在实际应用中难分轩轾。因为地球的旋转不是均匀的（它以复杂的方式减速和加速），所以 UT 始终不是均匀地流过。闰秒是根据需要引入 UTC 的，以便把 UTC 保持在 UT1 的 0.9 秒之内，UT1 是应用了某些更正的 UT 版本。还有其他的时间和日期系统；例如，基于卫星的全球定位系统 (GPS) 使用的时间刻度与 UTC 同步，但*没有*针对闰秒进行调整。有关更多信息的一个有趣来源是美国海军天文台，特别是 Directorate of Time 的网址：
[http://tycho.usno.navy.mil](http://tycho.usno.navy.mil/)还有它们对 "Systems of Time" 的定义，网址为：
[http://tycho.usno.navy.mil/systime.html](http://tycho.usno.navy.mil/systime.html)在类`Date`所有可以接受或返回年、月、日期、小时、分钟和秒值的方法中，将使用下面的表示形式：
年份y由整数y- 1900表示。
月份由从 0 至 11 的整数表示；0 是一月、1 是二月等等；因此 11 是十二月。
日期（一月中的某天）按通常方式由整数 1 至 31 表示。
小时由从 0 至 23 的整数表示。因此，从午夜到 1 a.m. 的时间是 0 点，从中午到 1 p.m. 的时间是 12 点。
分钟按通常方式由 0 至 59 的整数表示。
秒由 0 至 61 的整数表示；值 60 和 61 只对闰秒发生，尽管那样，也只用在实际正确跟踪闰秒的 Java 实现中。于按当前引入闰秒的方式，两个闰秒在同一分钟内发生是极不可能的，但此规范遵循 ISO C 的日期和时间约定。
在所有情形中，针对这些目的赋予方法的参数不需要在指定的范围内；例如，可以把日期指定为 1 月 32 日，并把它解释为 2 月 1 日的相同含义。
**Date****类**
**1 date****的构造器**
|构造方法摘要
|
|Date|()
|分配|Date|对象并初始化此对象，以表示分配它的时间（精确到毫秒）。
|
|Date|(long date)
|分配|Date|对象并初始化此对象，以表示自从标准基准时间（称为“历元（epoch）”，即 1970 年 1 月 1 日 00:00:00 GMT）以来的指定毫秒数。
|
|方法摘要
|
|boolean
|after|(|Date|when)
|测试此日期是否在指定日期之后。
|
|boolean
|before|(|Date|when)
|测试此日期是否在指定日期之前。
|
|Object
|clone|()
|返回此对象的副本。
|
|int
|compareTo|(|Date|anotherDate)
|比较两个日期的顺序。
|
|boolean
|equals|(|Object|obj)
|比较两个日期的相等性。
|
|long
|getTime|()
|返回自 1970 年 1 月 1 日 00:00:00 GMT 以来此|Date|对象表示的毫秒数。
|
|int
|hashCode|()
|返回此对象的哈希码值。
|
|void
|setTime|(long time)
|设置此|Date|对象，以表示 1970 年 1 月 1 日 00:00:00 GMT 以后|time|毫秒的时间点。
|
|String
|toString|()
|把此|Date|对象转换为以下形式的|String|： dow mon dd hh:mm:ss zzz yyyy 其中：|dow|是一周中的某一天 (|Sun, Mon, Tue, Wed, Thu, Fri, Sat|)。
|
**例子：**
**public****class**Test1 {
**public****static****void**main(String[]args) {
//创建日期的对象
Datedate=**new**Date();
System.**out**.println(date);
//一天的毫秒数
**long**ms=24*60*60*1000;
//当前时间的毫秒数
**long**cms= System.*currentTimeMillis*();
//获得昨天当前时间
Datedate1=**new**Date(cms-ms);
System.**out**.println(date1);
}
}
结果：
Fri Mar 16 21:18:03CST 2018
Thu Mar 15 21:18:03CST 2018
**-----------------------****日期的格式化****---------------------------------------**
## java.text
## 类 SimpleDateFormat
java.lang.Objectjava.text.Formatjava.text.DateFormat**java.text.SimpleDateFormat****所有已实现的接口：**
Serializable,Cloneable
---

public class**SimpleDateFormat**extendsDateFormat`SimpleDateFormat`是一个以与语言环境有关的方式来格式化和解析日期的具体类。它允许进行格式化（日期 -> 文本）、解析（文本 -> 日期）和规范化。
`SimpleDateFormat`使得可以选择任何用户定义的日期-时间格式的模式。但是，仍然建议通过`DateFormat`中的`getTimeInstance`、`getDateInstance`或`getDateTimeInstance`来创建日期-时间格式器。每一个这样的类方法都能够返回一个以默认格式模式初始化的日期/时间格式器。可以根据需要使用`applyPattern`方法来修改格式模式。有关使用这些方法的更多信息，请参阅`DateFormat`。
#### 日期和时间模式
日期和时间格式由*日期和时间模式*字符串指定。在日期和时间模式字符串中，未加引号的字母`'A'`到`'Z'`和`'a'`到`'z'`被解释为模式字母，用来表示日期或时间字符串元素。文本可以使用单引号 (`'`) 引起来，以免进行解释。`"''"`表示单引号。所有其他字符均不解释；只是在格式化时将它们简单复制到输出字符串，或者在解析时与输入字符串进行匹配。
定义了以下模式字母（所有其他字符`'A'`到`'Z'`和`'a'`到`'z'`都被保留）：
|字母
|日期或时间元素
|表示
|示例
|
|G
|Era 标志符
|Text
|AD
|
|y
|年
|Year
|1996|;|96
|
|M
|年中的月份
|Month
|July|;|Jul|;|07
|
|w
|年中的周数
|Number
|27
|
|W
|月份中的周数
|Number
|2
|
|D
|年中的天数
|Number
|189
|
|d
|月份中的天数
|Number
|10
|
|F
|月份中的星期
|Number
|2
|
|E
|星期中的天数
|Text
|Tuesday|;|Tue
|
|a
|Am/pm 标记
|Text
|PM
|
|H
|一天中的小时数（0-23）
|Number
|0
|
|k
|一天中的小时数（1-24）
|Number
|24
|
|K
|am/pm 中的小时数（0-11）
|Number
|0
|
|h
|am/pm 中的小时数（1-12）
|Number
|12
|
|m
|小时中的分钟数
|Number
|30
|
|s
|分钟中的秒数
|Number
|55
|
|S
|毫秒数
|Number
|978
|
|z
|时区
|General  time zone
|Pacific  Standard Time|;|PST|;|GMT-08:00
|
|Z
|时区
|RFC  822 time zone
|-0800
|
模式字母通常是重复的，其数量确定其精确表示：
Text:对于格式化来说，如果模式字母的数量大于等于 4，则使用完全形式；否则，在可用的情况下使用短形式或缩写形式。对于解析来说，两种形式都是可接受的，与模式字母的数量无关。
Number:对于格式化来说，模式字母的数量是最小的数位，如果数位不够，则用 0 填充以达到此数量。对于解析来说，模式字母的数量被忽略，除非必须分开两个相邻字段。
Year:如果格式器的Calendar是格里高利历，则应用以下规则。对于格式化来说，如果模式字母的数量为 2，则年份截取为 2 位数,否则将年份解释为number。
对于解析来说，如果模式字母的数量大于 2，则年份照字面意义进行解释，而不管数位是多少。因此使用模式 "MM/dd/yyyy"，将 "01/11/12" 解析为公元 12 年 1 月 11 日。
在解析缩写年份模式（"y" 或 "yy"）时，SimpleDateFormat必须相对于某个世纪来解释缩写的年份。这通过将日期调整为SimpleDateFormat实例创建之前的 80 年和之后 20 年范围内来完成。例如，在 "MM/dd/yy" 模式下，如果SimpleDateFormat实例是在 1997 年 1 月 1 日创建的，则字符串 "01/11/12" 将被解释为 2012 年 1 月 11 日，而字符串 "05/04/64" 将被解释为 1964 年 5 月 4 日。在解析时，只有恰好由两位数字组成的字符串（如Character.isDigit(char)所定义的）被解析为默认的世纪。其他任何数字字符串将照字面意义进行解释，例如单数字字符串，3 个或更多数字组成的字符串，或者不都是数字的两位数字字符串（例如"-1"）。因此，在相同的模式下， "01/02/3" 或 "01/02/003" 解释为公元 3 年 1 月 2 日。同样，"01/02/-3" 解析为公元前 4 年 1 月 2 日。
否则，则应用日历系统特定的形式。对于格式化和解析，如果模式字母的数量为 4 或大于 4，则使用日历特定的longform。否则，则使用日历特定的shortor abbreviated form。
Month:如果模式字母的数量为 3 或大于 3，则将月份解释为text；否则解释为number。
General time zone:如果时区有名称，则将它们解释为text。对于表示 GMT 偏移值的时区，使用以下语法：
·*GMTOffsetTimeZone:*·`GMT`*Sign Hours*`:`*Minutes*·*Sign:*one of·`+ -`·*Hours:*·*Digit*·*Digit Digit*·*Minutes:*·*Digit Digit*·*Digit:*one of`0 1 2 3 4 5 6 7 8 9`*Hours*必须在 0 到 23 之间，*Minutes*必须在 00 到 59 之间。格式是与语言环境无关的，并且数字必须取自 Unicode 标准的 Basic Latin 块。
对于解析来说，RFC822 time zones也是可接受的。
RFC 822 time zone:对于格式化来说，使用 RFC 822 4-digit 时区格式：
·*RFC822TimeZone:*·*Sign TwoDigitHours Minutes*·*TwoDigitHours:**Digit Digit**TwoDigitHours*必须在 00 和 23 之间。其他定义请参阅generaltime zones。
对于解析来说，generaltime zones也是可接受的。
`SimpleDateFormat`还支持*本地化日期和时间模式*字符串。在这些字符串中，以上所述的模式字母可以用其他与语言环境有关的模式字母来替换。`SimpleDateFormat`不处理除模式字母之外的文本本地化；而由类的客户端来处理。
#### 示例
以下示例显示了如何在美国语言环境中解释日期和时间模式。给定的日期和时间为美国太平洋时区的本地时间 2001-07-04 12:08:56。
|日期和时间模式
|结果
|
|"yyyy.MM.dd  G 'at' HH:mm:ss z"
|2001.07.04  AD at 12:08:56 PDT
|
|"EEE,  MMM d, ''yy"
|Wed, Jul  4, '01
|
|"h:mm  a"
|12:08 PM
|
|"hh  'o''clock' a, zzzz"
|12 o'clock  PM, Pacific Daylight Time
|
|"K:mm  a, z"
|0:08 PM,  PDT
|
|"yyyyy.MMMMM.dd  GGG hh:mm aaa"
|02001.July.04  AD 12:08 PM
|
|"EEE,  d MMM yyyy HH:mm:ss Z"
|Wed, 4 Jul  2001 12:08:56 -0700
|
|"yyMMddHHmmssZ"
|010704120856-0700
|
|"yyyy-MM-dd'T'HH:mm:ss.SSSZ"
|2001-07-04T12:08:56.235-0700
|
#### 同步
日期格式是不同步的。建议为每个线程创建独立的格式实例。如果多个线程同时访问一个格式，则它必须是外部同步的。
|构造方法摘要
|
|SimpleDateFormat|()
|用默认的模式和默认语言环境的日期格式符号构造|SimpleDateFormat|。
|
|SimpleDateFormat|(|String|pattern)
|用给定的模式和默认语言环境的日期格式符号构造|SimpleDateFormat|。
|
|SimpleDateFormat|(|String|pattern,|DateFormatSymbols|formatSymbols)
|用给定的模式和日期符号构造|SimpleDateFormat|。
|
|SimpleDateFormat|(|String|pattern,|Locale|locale)
|用给定的模式和给定语言环境的默认日期格式符号构造|SimpleDateFormat|。
|
|方法摘要
|
|void
|applyLocalizedPattern|(|String|pattern)
|将给定的本地化模式字符串应用于此日期格式。
|
|void
|applyPattern|(|String|pattern)
|将给定模式字符串应用于此日期格式。
|
|Object
|clone|()
|创建此|SimpleDateFormat|的一个副本。
|
|boolean
|equals|(|Object|obj)
|比较给定对象与此|SimpleDateFormat|的相等性。
|
|StringBuffer
|format|(|Date|date,|StringBuffer|toAppendTo,|FieldPosition|pos)
|将给定的|Date|格式化为日期/时间字符串，并将结果添加到给定的|StringBuffer|。
|
|AttributedCharacterIterator
|formatToCharacterIterator|(|Object|obj)
|格式化生成|AttributedCharacterIterator|的对象。
|
|Date
|get2DigitYearStart|()
|返回在 100 年周期内被解释的两位数字年份的开始日期。
|
|DateFormatSymbols
|getDateFormatSymbols|()
|获取此日期格式的日期和时间格式符号的一个副本。
|
|int
|hashCode|()
|返回此|SimpleDateFormat|对象的哈希码值。
|
|Date
|parse|(|String|text,|ParsePosition|pos)
|解析字符串的文本，生成|Date|。
|
|void
|set2DigitYearStart|(|Date|startDate)
|设置 100 年周期的两位数年份，该年份将被解释为从用户指定的日期开始。
|
|void
|setDateFormatSymbols|(|DateFormatSymbols|newFormatSymbols)
|设置此日期格式的日期和时间格式符号。
|
|String
|toLocalizedPattern|()
|返回描述此日期格式的本地化模式字符串。
|
|String
|toPattern|()
|返回描述此日期格式的模式字符串。
|
**例子：**
**public****class**Test1 {
**public****static****void**main(String[]args) {
//创建日期的对象
Datedate=**new**Date();
SimpleDateFormatsdf=**new**SimpleDateFormat();
//输出format方法默认格式
Stringstr=sdf.format(date);
System.**out**.println(str);
//自定义格式的格式化对象
//    SimpleDateFormat sdf1 = new SimpleDateFormat("yyyy年MM月dd日HH时mm分ss秒a");
//    SimpleDateFormat sdf1 = new SimpleDateFormat("yyyy年MM月dd日HH时mm分ss秒E a");
//    SimpleDateFormat sdf1 = new SimpleDateFormat("yyyy年MM月dd日HH时mm分ss秒SSS毫秒a");
//    SimpleDateFormat sdf1 = new SimpleDateFormat("yyyy/MM/ddHH:mm:ss");
//下面的格式最为常用的
SimpleDateFormatsdf1=**new**SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
Stringstr1=sdf1.format(date);
System.**out**.println(str1);
//把字符串转换成日期
Stringstr2="1995-12-16 23:12:45";
**try**{
//获得指定字符串的日期对象，注意字符串的格式一定要和SimpleDateFromat中的格式要匹配
Datedate1=sdf1.parse(str2);
System.**out**.println(date1);
//将日期转换成数字
**long**ms=date1.getTime();
System.**out**.println(ms);
//将数字装换为日期
date1.setTime(ms);
System.**out**.println(date1);
date1.setTime(ms+10000);
System.**out**.println(date1);
}**catch**(ParseExceptione) {
e.printStackTrace();
}
}
}
结果：
18-3-16下午10:16
2018-03-16 22:16:26
Sat Dec 16 23:12:45CST 1995
819126765000
Sat Dec 16 23:12:45CST 1995
Sat Dec 16 23:12:55CST 1995
![](//img-blog.csdn.net/20180316223000325?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


