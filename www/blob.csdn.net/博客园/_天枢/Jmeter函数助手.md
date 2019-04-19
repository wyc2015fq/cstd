# Jmeter函数助手 - _天枢 - 博客园
## [Jmeter函数助手](https://www.cnblogs.com/yhleng/p/9120082.html)
2018-06-01 09:01 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9120082)
__intSum
intSum 函数可用于计算两个或多个整数值的总和。
引用名称是可选的, 但它不能是有效的整数。
${__intSum(2,5,MYVAR)}  将返回 7 (2 5) 并将结果存储在 MYVAR 变量中。因此 $ {MYVAR} 将等于7。
${__intSum(2,5,7)}  将返回 14 (2 5 7) 并将结果存储在 MYVAR 变量中。
${__intSum(1,2,5,${MYVAR})} 如果 MYVAR 值等于8、1 2 5 $ {MYVAR}, 则返回16
__longSum
longSum 函数可用于计算两个或多个长值的总和, 而当您知道值不会在间隔-2147483648 到2147483647中时, 请使用此方法而不是 __intSum。
${__longSum(2,5,MYVAR)} 将返回 7 (2 5) 并将结果存储在 MYVAR 变量中。因此 $ {MYVAR} 将等于7。
${__longSum(2,5,7)} 将返回 14 (2 5 7) 并将结果存储在 MYVAR 变量中。
${__longSum(1,2,5,${MYVAR})} 如果 MYVAR 值等于8、1 2 5 $ {MYVAR}, 则返回16
__StringFromFile
StringFromFile 函数可用于从文本文件读取字符串。这对于运行需要大量可变数据的测试非常有用。例如, 在测试银行应用程序时, 可能需要使用不同帐号的100s 或1000s。
另请参阅可更易于使用的 CSV 数据集配置测试元素。但是, 当前不支持多个输入文件。 每次被调用时, 它会从文件中读取下一行。所有线程共享相同的实例, 因此不同的线程将得到不同的行。当到达文件末尾时, 它将从一开始重新开始读取, 除非已达到最大循环计数。如果在测试脚本中有多个对该函数的引用, 则每个文件都将独立打开它, 即使文件名相同。[如果该值将再次用于其他位置, 请对每个函数调用使用不同的变量名。
函数实例在线程之间共享, 并且该文件 (重新打开) 由任何线程碰巧需要下一行输入, 因此使用 threadNumber 作为文件名的一部分将导致不可预知的行为。
如果打开或读取文件时出现错误, 则该函数将返回字符串 '** 错误 **'
${__StringFromFile(PIN#'.'DAT,,1,2)}-读取 PIN1.DAT, PIN2.DAT 
${__StringFromFile(PIN.DAT,,,2)}-读取 PIN2.DAT 两次
__machineName
machineName 函数返回本地主机名。这将使用 Java 方法 InetAddress getLocalHost () 并将其传递到 getHostName ()
Examples:
${__machineName()} 将返回计算机的主机名
${__machineName} 将返回计算机的主机名
__machineIP
machineIP 函数返回本地 IP 地址。这将使用 Java 方法 InetAddress getLocalHost () 并将其传递到 getHostAddress ()
${__machineIP()} 将返回计算机的 IP 地址
${__machineIP} 将返回计算机的 IP 地址
__javaScript
 javascript 函数执行一条 javascript (不是 Java) 代码并返回其值 JMeter javascript 函数调用独立的 javascript 解释器。Javascript 被用作脚本语言, 因此您可以进行计算等。
javaScript 不是 JMeter 中性能最好的脚本语言。如果您的计划需要大量的线程, 建议使用 jexl3 或 __groovy 函数。
${__javaScript('${sp}'.slice(7\,99999))} 7后的逗号被转义。
Examples:
${__javaScript(new Date())} 将返回 Sat 2016年1月09日 16:22:15 GMT 0100 (英语)
${__javaScript(new Date(),MYDATE)} 将返回星期六 2016年1月09日 16:22:15 GMT 0100 (CET) 和存储在可变 MYDATE
${__javaScript(Math.floor(Math.random()*(${maxRandom}+1)),MYRESULT)}
将使用 maxRandom 变量, 返回一个介于0和 maxRandom 之间的随机值, 并将其存储在 MYRESULT
__Random
随机函数返回位于给定最小值和最大数值之间的随机数。
${__Random(0,10)} 将返回介于0和10之间的随机数
${__Random(0,10, MYVAR)} 将返回一个介于0和10之间的随机数字, 并将其存储在 MYVAR 中。$ {MYVAR} 将包含随机数
__RandomDate
RandomDate 函数返回位于给定开始日期和结束日期值之间的随机日期。
${__RandomDate(,,2050-07-08,,)}将返回到现在和2050-07-08 之间的随机日期。例如2039-06-21
${__RandomDate(dd MM yyyy,,08 07 2050,,)}将返回一个自定义格式的随机日期, 如 04 03 2034
__RandomString
RandomString 函数返回一个随机长度字符串, 使用字符在字符中使用。
${__RandomString(5)}将返回一个随机字符串的5个字符, 可读或不
${__RandomString(10,abcdefg)}将返回一个随机字符串10字符从 abcdefg 集, 如 cdbgdbeebd 或 adbfeggfad,..。
${__RandomString(6,a12zeczclk, MYVAR)} 将返回从 a12zeczclk 集选取的随机字符串6个字符, 并将结果存储在 MYVAR 中, MYVAR 将包含字符串, 如2z22ak 或 z11kce..。
__RandomFromMultipleVars
RandomFromMultipleVars 函数根据源变量提供的变量值返回一个随机值。
${__RandomFromMultipleVars(val)}将返回一个基于变量的内容的随机字符串考虑到它们是否是多值或不
${__RandomFromMultipleVars(val1|val2)} 将返回一个随机字符串基于变量的内容 val1 和 val2 考虑到它们是否是多值或不
${__RandomFromMultipleVars(val1|val2, MYVAR)}将返回一个基于变量内容的随机字符串 val1 和 val2 考虑到它们是否是多值的, 并将结果存储在 MYVAR
__UUID
UUID 函数返回一个伪随机类型4通用唯一标识符 (uuid)。
${UUID()}将返回 UUIDs 的格式: c69e0dd1-ac6b-4f2b-8d59-5d4e8743eecd
__CSVRead
CSVRead 函数从 CSV 文件中返回一个字符串 (鸡毛 StringFromFile) 注意: JMeter 支持多个文件名。
 首次遇到文件名时, 文件将被打开并读取到内部数组中。如果检测到空行, 则将其视为文件的末尾-这允许使用尾随注释。 所有对同一文件名的后续引用都使用相同的内部数组。注意文件名大小写对函数很重要, 即使操作系统不关心, 因此 CSVRead (.txt, 0) 和 CSVRead (abc, 0) 将引用不同的内部数组。 '* 别名' 功能允许多次打开同一文件, 并且允许更短的文件名。 每个线程都有自己的内部指针到文件数组中的当前行。当线程第一次引用该文件时, 它将被分配到数组中的下一个可用行, 因此每个线程将从所有其他线程访问其他行。[除非有比数组中的行多的线程。
默认情况下, 函数在每个逗号处拆分该行。如果要输入包含逗号的列, 则需要将分隔符更改为不出现在任何列数据中的字符, 通过设置属性: csvread. 分隔符
例如, 可以按如下方式设置一些变量:
- COL1a ${__CSVRead(random.txt,0)}
- COL2a ${__CSVRead(random.txt,1)}${__CSVRead(random.txt,next)}
- COL1b ${__CSVRead(random.txt,0)}
- COL2b ${__CSVRead(random.txt,1)}${__CSVRead(random.txt,next)}
这将读取一行中的两列, 以及下一个可用行中的两列。如果在相同的用户参数前处理器上定义了所有变量, 则这些行将是连续的。否则, 另一个线程可能会抓取下一行。
该函数不适合用于大型文件, 因为整个文件存储在内存中。对于较大的文件, 请使用 CSV 数据集配置元素或 StringFromFile。
__property
属性函数返回 JMeter 属性的值。如果找不到属性值, 并且未提供默认设置, 则返回属性名称。提供默认值时, 不需要提供函数名-参数可以设置为 null, 并且将被忽略。
- ${__property(user.dir)} - 返回值为 user.dir
- ${__property(user.dir,UDIR)} - 返回值为user.dir 并保存在 UDIR
- ${__property(abcd,ABCD,atod)} - 返回值属性为 abcd (或 "atod"如果没有定义) 并保存在ABCD中
- ${__property(abcd,,atod)} - 返回值属性为 abcd (或 "atod"如果没有定义) 但不保存它
__P
这是一个简化的属性函数, 用于与命令行上定义的属性一起使用。与 __property 函数不同, 没有选项保存变量中的值, 如果没有提供默认值, 则假定为1。选择了1的值, 因为它对常见的测试变量 (如循环、线程计数、坡道等) 有效。
获取值:
