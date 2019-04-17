# Java高级用户指南-核心Java - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月14日 13:34:47[boonya](https://me.csdn.net/boonya)阅读数：490








原文地址：[https://dzone.com/refcardz/core-java](https://dzone.com/refcardz/core-java)

![](https://img-blog.csdn.net/20170214133358163)


本文采用Google翻译，有些地方数据类型被翻译成了中文，建议中英文对照一起看。


第1节

## 关于Core Java


此Refcard概述了Java语言的关键方面以及核心库（格式化输出，集合，正则表达式，日志记录，属性）以及最常用的工具（javac，java，jar）上的备忘单。





第2节

## Java关键字


|关键词|描述|例|
|----|----|----|
|抽象|抽象类或方法|抽象类Writable {public abstract void write（Writer out）;public void save（String filename）{...}|
|断言|如果启用断言，则在条件未满足时抛出错误|assert param！= null;注意：使用-ea运行以启用断言|
|布尔|布尔值类型，值为true和false|boolean more = false;|
|打破|断开开关或回路|while（（ch = in.next（））！= -1）{if（ch =='\ n'）break;过程（ch）;}注意：也看开关|
|字节|8位整数类型|字节b = -1;//不等于0xFF注意：小心字节<0|
|案件|开关的情况|见开关|
|抓住|try块的子句捕获异常|看试试|
|char|Unicode字符类型|char input ='Q';|
|类|定义类类型|class Person {private String name;public Person（String aName）{name = aName;} public void print（）{System.out.println（name）;}}|
|继续|在循环结束时继续|while（（ch = in.next（））！= -1）{if（ch ==''）continue;过程（ch）;}}|
|默认|1）交换机的默认子句2）表示接口方法的默认实现|1）见开关2）public interface Collection <E> {@覆盖**默认**分离器<E> spliterator（）{return Spliterators.spliterator（this，0）;}}}}|
|做|一个do / while循环的顶部|do {ch = in.next（）;} while（ch ==''）;|
|双|双精度浮点数型|双1 = 0.5;|
|其他|if语句的else子句|看看|
|枚举|枚举类型|enum Mood {SAD，HAPPY};|
|延伸|定义类的父类|class Student extends Person {private int id;public Student（String name，int anId）{...} public void print（）{...}|
|最后|一个常量，或一个不能被覆盖的类或方法|public static final int DEFAULT_ID = 0;|
|最后|总是执行的try块的部分|看试试|
|浮动|单精度浮点类型|float oneHalf = 0.5F;|
|对于|循环类型|for（int i = 10; i> = 0; i--）System.out.println（i）;for（String s：line.split（“\\ s +”））System.out.println（s）;注意：在“generalized”for循环中，：之后的表达式必须是数组或Iterable|
|如果|条件语句|if（input =='Q'）System.exit（0）;else more = true;|
|实现|定义类实现的接口|类学生实现Printable {...}|
|进口|导入包|import java.util.ArrayList;import com.dzone.refcardz。*;|
|instanceof|测试一个对象是否是一个类的实例|if（fred instanceof Student）value =（（Student）fred）.getId（）;注意：null的instanceof T总是false|
|int|32位整数类型|int value = 0;|
|接口|一个类可以实现的方法的抽象类型|interface Printable {void print（）;}}|
|长|64位长整数类型|long worldPopulation = 6710044745L;|
|本机|由主机系统实现的方法||
|新|分配一个新的对象或数组|人员fred =新人（“Fred”）;|
|空值|空引用|Person optional = null;|
|包|一个包的类|package com.dzone.refcardz;|
|私人的|一个只能通过此类的方法访问的功能|见类|
|保护|一个只能通过此类，其子代和同一个包中的其他类的方法访问的功能|class Student {protected int id;...}|
|上市|一个可以通过所有类的方法访问的功能|见类|
|返回|从一个方法返回|int getId（）{return id;}}|
|短|16位整数类型|短裙长度= 24;|
|静态的|一个对其类唯一的特征，而不是它的类的对象|public class WriteUtil {public static void write（Writable [] ws，String filename）;public static final String DEFAULT_EXT =“.dat”;}}|
|strictfp|对浮点计算使用严格的规则||
|超|调用超类构造函数或方法|public Student（String name，int anId）{super（name）;id = anId;} public void print（）{super.print（）;System.out.println（id）;}}|
|开关|选择语句|switch（ch）{case'Q'：case'q'：more = false;打破;案件 ' ';打破;default：process（ch）;打破;}注意：如果省略了中断，则会继续处理下一个情况。|
|同步|对线程而言是原子的方法或代码块|public synchronized void addGrade（String gr）{grades.add（gr）;}}|
|这个|方法的隐式参数，或者这个类的构造函数|public Student（String id）{this.id = id;} public Student（）{this（“”）;}}|
|扔|抛出异常|if（param == null）throw new IllegalArgumentException（）;|
|抛出|方法可以抛出的异常|public void print（）throws PrinterException，IOException|
|短暂的|标记不应该持久的数据|class Student {private transient Data cachedData;...}|
|尝试|一个捕获异常的代码块|try {fred.print（out）;} catch（PrinterException ex）{ex.printStackTrace（）;}} finally {out.close（）;}}|
|void|表示不返回值的方法|public void print（）{...}|
|挥发性|确保字段由多个线程一致地访问|class Student {private volatile int nextId;...}|
|而|一个循环|while（in.hasNext（））process（in.next（））;|







第3节

## 标准Java包


|java.applet|Applets（在Web页面中运行的Java程序）|
|----|----|
|java.awt|图形和图形用户界面|
|java.beans|支持JavaBeans组件（具有属性和事件侦听器的类）|
|java.io|输入和输出|
|java.lang|语言支持|
|java.math|任意精度数字|
|java.net|联网|
|java.nio|“新建”（内存映射）I / O|
|java.rmi|远程方法调用|
|java.security|安全支持|
|java.sql|数据库支持|
|java.text|文本和数字的国际化格式|
|java.time|日期，时间，持续时间，时区等|
|java.util|实用程序（包括数据结构，并发，正则表达式和日志记录）|







第4节

## 运算符优先级


|运算符具有相同的优先级||笔记|
|----|----|----|
|[]。（）（方法调用）|左到右||
|！〜++ - +（unary） - （unary）（）（cast）new|右到左|〜翻转一个数字的每一位|
|* /％|左到右|使用带负数的％时要小心。-a％b == - （a％b），但是一个％-b == a％b。例如，-7％4 == -3,7％-4 == 3|
|+ -|左到右||
|<< >> >>>|左到右|>>是算术移位（正数和负数的n >> 1 == n / 2），>>>是逻辑移位（将0加到最高位）。如果左手边是一个整数，右手边减去模数32，如果左手边长则是模数64。例如，1 << 35 == 1 << 3|
|<= =>> = instanceof|左到右|null instanceof T总是false|
|==！=|左到右|检查身份。使用equals检查结构相等性|
|＆lt;|左到右|按位AND;没有使用bool参数的延迟评估|
|^|左到右|按位异或|
|||左到右|按位OR;没有使用bool参数的延迟评估|
|&&|左到右||
||||左到右||
|？：|右到左||
|= + = - = * = / =％=＆= | = ^ = << = >> = >>> =|右到左||







第5节

## 基本类型


|类型|尺寸|范围|笔记|
|----|----|----|----|
|int|4字节|-2,147,483,648至2,147,483,647（刚刚超过20亿）|包装类型为整数。对任意精度整数使用BigInteger|
|短|2字节|-32,768 to 32,767||
|长|8字节|-9,223,372,036,854,775,808至9,223,372,036,854,775,807|字面值以L结尾（例如1L）|
|字节|1字节|-128〜127|请注意，范围不是0 ... 255|
|浮动|4字节|约-3.40282347E + 38F（6-7有效十进制数字）|字面值以F结尾（例如0.5F）|
|双|8字节|约-1.79769313486231570E + 308（15个有效十进制数字）|对任意精度浮点数使用BigDecimal|
|char|2字节|\ u0000至\ uFFFF|包装类型为Character。Unicode字符> U + FFFF需要两个字符值|
|布尔||对或错||

### 原始类型之间的法律转换

虚线箭头表示可能失去精度的转化。

![原始类型之间的法律转换](https://dzone.com/storage/rc-covers/10640-thumb.png)



















第6节

## Lambda表达式



### 功能接口

用一个抽象方法接口。例：
`@FunctionalInterface public interface Predicate<T> { boolean test(T t); }`
此接口的实现可以作为lambda表达式在线提供：
- 功能接口的匿名实现
- 参数和主体由“箭头符号”（“ - >”）分隔
- 抽象方法的参数在箭头的左边
- 实现是在箭头的右边

lambda表达式的典型用法：
`JButton button = new JButton("MyButton"); button.addActionListener(event -> doSomeImportantStuff(event));`
### 方法参考

lambda表达式表示匿名函数。您可以将它们作为方法参数传递或返回。使用方法引用的命名方法也可以做到这一点。

方法参考文献的典型用法：
|无方法参考|使用方法引用|
|----|----|
|button.addActionListener（event - > doSomeImportantStuff（event））;|button.addActionListener（this :: doSomeImportantStuff）;|
|list.forEach（element - > System.out.println（element））;|list.forEach（System。out :: println）;|

有四种方法引用：
|种类的方法参考|例|
|----|----|
|静态方法|Collections :: emptyList|
|一个特定（命名）对象的实例方法|user :: getFirstName|
|给定类型的任意对象（稍后命名）的实例方法|User :: getFirstName|
|到构造函数|User :: new|







第7节

## 集合和常见算法


|ArrayList|动态增长和收缩的索引序列|
|----|----|
|LinkedList|允许在任何位置有效插入和移除的有序序列|
|ArrayDeque|作为圆形数组实现的双端队列|
|HashSet|拒绝重复的无序集合|
|TreeSet|排序集|
|EnumSet|一组枚举类型值|
|LinkedHashSet|记住插入元素的顺序的集合|
|PriorityQueue|允许有效移除最小元素的集合|
|HashMap|存储键/值关联的数据结构|
|TreeMap|其中键被排序的映射|
|EnumMap|其中键属于枚举类型的映射|
|LinkedHashMap|记住添加条目顺序的地图|
|WeakHashMap|如果未在其他地方使用的值，那么可以由垃圾回收器回收的值的映射|
|IdentityHashMap|具有由==比较的键的映射，不等于|

### 常见任务
|List <String> strs = new ArrayList <>（）;|收集字符串|
|----|----|
|strs.add（“Hello”）;strs.add（“World！”）;|添加字符串|
|for（String str：strs）System.out.println（str）;|对集合中的所有元素执行某些操作|
|迭代器<String> iter = strs.iterator（）;while（iter.hasNext（））{String str = iter.next（）;if（someCondition（str））iter.remove（）;}}|删除与条件匹配的元素。remove方法删除前面调用next所返回的元素|
|strs.addAll（strColl）;|添加另一个字符串集合中的所有字符串|
|strs.addAll（Arrays.asList（args））|添加字符串数组中的所有字符串。Arrays.asList是一个数组的List包装器|
|strs.removeAll（coll）;|删除另一个集合的所有元素。用于比较|
|if（0 <= i && i <strs.size（））{str = strs.get（i）;strs.set（i，“Hello”）;}}|获取或设置指定索引处的元素|
|strs.insert（i，“Hello”）;str = strs.remove（i）;|在指定索引处插入或删除元素，移动索引值较高的元素|
|String [] arr = new String [strs.size（）];strs.toArray（arr）;|从集合转换到数组|
|String [] arr = ...;List <String> lst = Arrays.asList（arr）;lst = Arrays.asList（“foo”，“bar”，“baz”）;|从数组转换为列表。使用varargs表单来创建一个小集合|
|List <String> lst = ...;lst.sort（）;lst.sort（new Comparator <String>（）{public int compare（String a，String b）{return a.length（） - b.length（）;}}|按照元素的自然顺序或使用自定义比较器对列表排序|
|Map <String，Person> map = new LinkedHashMap <String，Person>（）;|创建以插入顺序遍历的映射（需要针对密钥类型使用hashCode）。使用TreeMap以排序顺序遍历（要求键类型是可比较的）|
|for（Map.Entry <String，Person> entry：map.entrySet（））{String key = entry.getKey（）;Person value = entry.getValue（）;...}|迭代地图的所有条目|
|Person key = map.get（str）;// null如果没有找到map.put（key，value）;|获取或设置给定键的值|

### Stream API的批量操作
|strs.forEach（System.out :: println）;|对集合中的所有元素执行某些操作|
|----|----|
|List <String> filteredList = strs .stream（）.filter（this :: someCondition）.collect（Collectors.toList（））;|过滤符合条件的元素|
|String concat = strs.stream（）.collect（Collectors.joining（“，”））;|连接流的元素|
|List <User> users = ...;列表<String> firstNames = users.stream（）.map（User :: getFirstName）.collect（Collectors.toList（））;|创建一个映射到原始列表的新列表|
|列表<String> adminFirstNames = users .stream（）.filter（User :: isAdmin）.map（User :: getFirstName）.collect（Collectors.toList（））;|组合操作这不会导致列表元素的两次遍历|
|int sumOfAges = users .stream（）.mapToLong（User :: getAge）.sum（）;|简单还原操作|
|Map <Role，List <User >> byRole = users .stream（）.collect（收藏家.groupingBy（User :: getRole））;|按特定属性对用户进行分组|
|int sumOfAges = users .parallelStream（）.mapToLong（User :: getAge）.sum（）;|所有上述操作可以并行完成|







第8节

## 字符转义序列


|\ b|退格\ u0008|
|----|----|
|\ t|标签\ u0009|
|\ n|换行符\ u000A|
|\F|表单feed \ u000C|
|\ r|回车\ u000D|
|\“|双引号|
|\'|单引号|
|\\|反斜杠|
|\ uhhhh（hhhh是0000和FFFF之间的十六进制数）|UTF-16代码点的值为hhhh|
|\ ooo（ooo是0到377之间的八进制数）|八进制值为ooo的字符|
|**注意：**与C / C ++不同，不允许\ xhh| |







第9节

## 格式化输出使用printf



### 典型用法
`System.out.printf("%4d %8.2f", quantity, price); String str = String.format("%4d %8.2f", quantity, price);`
每个格式说明符具有以下格式。请参阅表格中的标志和转换字符。

![典型用法](https://dzone.com/storage/rc-covers/10641-thumb.png)

### 标志
|旗|描述|例|
|----|----|----|
|+|打印正数和负数的符号|+3333.33|
|空间|在正数之前添加一个空格||3333.33 ||
|0|添加前导零|003333.33|
|- -|左对齐字段|| 3333.33 ||
|（（|括号中包含负数|（3333.33）|
|，|添加组分隔符|3,333.33|
|＃（for f format）|始终包括小数点|3,333。|
|＃（对于x或o格式）|添加0x或0前缀|0xcafe|
|$|指定要格式化的参数的索引;例如，％1 $ d％1 $ x以十进制和十六进制打印第一个参数|159 9F|
|<|格式与以前的规格相同;例如，％d％<x以十进制和十六进制打印相同的数字|159 9F|

### 转换字符
|转换字符|描述|例|
|----|----|----|
|d|十进制整数|159|
|X|十六进制整数|9f|
|o|八进制整数|237|
|F|定点浮点|15.9|
|e|指数浮点|1.59e + 01|
|G|一般浮点（e和f的较短者）||
|一个|十六进制浮点|0x1.fccdp3|
|s|串|你好|
|C|字符|H|
|b|布尔|真正|
|H|哈希码|42628b2|
|tx|日期和时间|参见下表|
|％|百分比符号|％|
|n|平台相关的行分隔符||







第10节

## 带有MessageFormat的格式化输出



典型用途：
`String msg = MessageFormat.format("On {1, date, long}, a {0} caused {2,number,currency} of damage.", "hurricane", new GregorianCalendar(2009, 0, 15). getTime(), 1.0E8);`
产量`"On January 1, 1999, a hurricane caused $100,000,000 of damage"`
- 第n个项目由{n，format，subformat}表示，其格式和子格式如下所示
- `{0}`是第一个项目
- 下表显示可用的格式
- 对引号使用单引号，例如对于文字左花括号使用“{”
- 使用''作为文字单引号
|格式|子格式|例|
|----|----|----|
|数|没有|1,234.567|
||整数|1,235|
||货币|$ 1,234.57|
||百分|123,457％|
|日期|无或中等|2009年1月15日|
||短|1/15/09|
||长|2009年1月15日|
||充分|2009年1月15日，星期四|
|时间|无或中等|3:45:00 PM|
||短|下午3:45|
||长|下午3:45:00太平洋标准时间|
||充分|下午3:45:00太平洋标准时间|
|选择|选择列表，以|分隔。每个选择都有- 下界（使用 - \ u221E表示 - ）- 关系运算符：<for“less than”，＃或\ u2264 for。- 消息格式字符串例如，{1，choice，0＃no houses | 1＃one house | 2＃{1} houses}|没有房子一个房子5房子|
||||







第11节

## 正则表达式



### 常见任务
|String [] words = str.split（“\\ s +”）;|沿着空格边界拆分字符串|
|----|----|
|Pattern pattern = Pattern.compile（“[0-9] +”）;Matcher matcher = pattern.matcher（str）;String result = matcher.replaceAll（“＃”）;|替换所有匹配项。这里我们用＃替换所有数字序列。|
|Pattern pattern = Pattern.compile（“[0-9] +”）;Matcher matcher = pattern.matcher（str）;while（matcher.find（））{process（str.substring（matcher.start（），matcher.end（）））;}}|查找所有匹配项。|
|Pattern pattern = Pattern.compile（“（1？[0-9]）：（[0-5] [0-9]）[ap] m”）;Matcher matcher = pattern.matcher（str）;for（int i = 1; i <= matcher.groupCount（）; i ++）{process（matcher.group（i））;}}|查找所有组（由模式中的括号指示）。这里我们找到一个日期的小时和分钟。|

### 正则表达式语法
|字符| |
|----|----|
|C|字符c|
|\ unnnn，\ xnn，\ 0n，\ 0nn，\ 0nnn|具有给定十六进制或八进制值的代码单元|
|\ t，\ n，\ r，\ f，\ a，\ e|控制字符选项卡，换行，返回，换页，提醒和转义|
|\ cc|对应于字符c的控制字符|
| | |
|[C1C2。。。]|联合：由C1C2，...表示的任何字符。。。Ci是字符，字符范围c1-c2或字符类。示例：[a-zA-Z0-9_]|
|[^ C1C2。。。]|补充：不由C1C2，...表示的字符。。。示例：[^ 0-9]|
|[C1 && C2 &&。。。]|交叉：由所有C1C2，...表示的字符。。。示例：[Af && [^ G -#]]|
| | |
|。|除了行终止符以外的任何字符（如果设置了DOTALL标志，则为任何字符）|
|\ d|数字[0-9]|
|\ D|非数字[^ 0-9]|
|\ s|空格字符[\ t \ n \ r \ f \ x0B]|
|\ S|非空格字符|
|\ w|字字符[a-zA-Z0-9_]|
|\ W|非字符字符|
|\ p {name}|命名字符类 - 请参见下表|
|\ P {name}|命名字符类的补码|
| | |
|^ $|开始，输入结束（或多行模式中的开始，结束行）|
|\ b|字边界|
|\ B|非字边界|
|\一个|输入开始|
|\ z|输入结束|
|\ Z|最终行终止符以外的输入结束|
|\G|上一次比赛结束|
| | |
|X？|可选X|
|X*|X，0或更多次|
|X +|X，1次或更多次|
|X {n} X {n，} X {n，m}|X n次，至少n次，在n和m次之间|
| | |
|？|将默认（贪婪）匹配转换为不愿意匹配|
|+|将默认（贪婪）匹配转换为不愿意匹配|
| | |
|XY|任何来自X的字符串，后跟任何来自Y的字符串|
|X | Y|任何来自X或Y的字符串|
| | |
|（X）|捕获与X匹配的字符串|
|\G|第g组的比赛|
| | |
|\C|字符c（不能是字母字符）|
|\ Q。。。\ E|报价。。。逐字|
|（α...）|特殊结构 - 参见Pattern类的API注释|

### 预定义字符类名称
|降低|ASCII小写字母[az]|
|----|----|
|上|ASCII大写[AZ]|
|Α|ASCII字母[A-Za-z]|
|数字|ASCII数字[0-9]|
|Alnum|ASCII字母或数字[A-Za-z0-9]|
|XDigit|十六进制数字[0-9A-Fa-f]|
|打印或图形|可打印的ASCII字符[\ x21- \ x7E]|
|点|ASCII非数字或数字[\ p {Print} && \ P {Alnum}]|
|ASCII|所有ASCII [\ x00- \ x7F]|
|Cntrl|ASCII控制字符[\ x00- \ x1F]|
|空白|空间或制表符[\ t]|
|空间|空白[\ t \ n \ r \ f \ 0x0B]|
|javaLowerCase|小写，由Character.isLowerCase（）|
|javaUpperCase|大写，由Character.isUpperCase（）|
|javaWhitespace|空白，由Character.isWhitespace（）|
|javaMirrored|镜像，由Character.isMirrored（）确定|
|InBlock|块是Unicode字符块的名称，删除空格，例如BasicLatin或Mongolian|
|类别或类别|类别是Unicode字符类别的名称，例如L（字母）或Sc（货币符号）|

### 匹配的标志

模式匹配可以使用标志进行调整，例如：
`Pattern pattern = Pattern.compile(patternString, Pattern.CASE_INSENSITIVE + Pattern.UNICODE_CASE)`|旗|描述|
|----|----|
|不区分大小写|匹配字母独立于大写字母。默认情况下，此标志仅考虑US ASCII字符|
|UNICODE_CASE|当与CASE_INSENSITIVE组合使用时，请使用Unicode字母大小写进行匹配|
|MULTILINE|^和$匹配行的开始和结束，而不是整个输入|
|UNIX_LINES|在多行模式中匹配^和$时，只有'\ n'被识别为行终止符|
|DOTALL|当使用这个标志时，符号匹配所有字符，包括行终结符|
|CANON_EQ|考虑Unicode字符的规范等效性。例如，u后跟¨（diaeresis）匹配ü|
|文字|指定模式的输入字符串被视为一串文字字符，对于没有特殊含义。[]等。|







第12节

## 记录



### 常见任务
|Logger logger = Logger.getLogger（“com.mycompany.myprog.mycategory”）;|获取类别的记录器|
|----|----|
|logger.info（“Connection successful。”）;|记录级别FINE的消息。可用级别为SEVERE，WARNING，INFO，CONFIG，FINE，FINER，FINEST，以及相应的严重，警告等方法|
|logger.log（Level.SEVERE，“Unexpected exception”，Throwable）;|记录Throwable的堆栈跟踪|
|logger.setLevel（Level.FINE）;|将日志记录级别设置为FINE。默认情况下，日志记录级别为INFO，不记录不太严重的日志消息|
|处理程序处理程序=新的FileHandler（“％h / myapp.log”，SIZE_LIMIT，LOG_ROTATION_COUNT）;handler.setFormatter（new SimpleFormatter（））;logger.addHandler（handler）;|添加用于将日志记录保存在文件中的文件处理程序。有关命名模式，请参见下表。这个处理程序使用一个简单的格式化程序，而不是XML格式化程序，这是文件处理程序的默认值|

### 记录配置文件

日志配置可以通过日志配置文件配置，默认为`jre/lib/logging.properties` 。启动虚拟机时，可以使用系统属性`java.util.logging.config.file`另一个文件。（注意`LogManager`在main之前运行。）
|配置属性|描述|默认| | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|loggerName.level|记录器的日志记录级别由给定的名称|没有;日志记录器从其父级继承处理程序| | | | | | | | | | | | |
|处理程序|根记录器的类名称的空格或逗号分隔的列表。使用默认构造函数为每个类名创建一个实例|java.util.logging。ConsoleHandler| | | | | | | | | | | | |
|loggerName.handlers|给定记录器的类名称的空格或逗号分隔列表|给定日志记录的类名称列表无| | | | | | | | | | | | |
|loggerName。useParentHandlers|false如果父记录器的处理程序（和最终的根记录器的处理程序）不应该使用|真正| | | | | | | | | | | | |
|配置|用于初始化的空格或逗号分隔的类名称列表|没有| | | | | | | | | | | | |
|java.util.logging.FileHandler.leveljava.util.logging.ConsoleHandler.level|默认处理程序级别|Level.ALL用于FileHandler，Level.INFO用于ConsoleHandler| | | | | | | | | | | | |
|java.util.logging.FileHandler.filterjava.util.logging.ConsoleHandler.filter|默认过滤器的类名|没有| | | | | | | | | | | | |
|java.util.logging.FileHandler.formatterjava.util.logging.ConsoleHandler.formatter|默认格式化程序的类名|formatter java.util.logging。XMLFormatter for FileHandler，java.util.logging。SimpleFormatter for ConsoleHandler| | | | | | | | | | | | |
|java.util.logging.FileHandler.encodingjava.util.logging.ConsoleHandler.encoding|默认编码|默认平台编码| | | | | | | | | | | | |
|java.util.logging.FileHandler.limit|旋转日志文件的默认限制（以字节为单位）|0（无限制），但在jre / lib / logging.properties中设置为50000| | | | | | | | | | | | |
|java.util.logging.FileHandler.count|循环日志文件的默认数目|1| | | | | | | | | | | | |
|java.util.logging.FileHandler.pattern|日志文件的默认命名模式。创建文件时，将替换以下标记：|令牌|描述||----|----||/|路径分隔符||％t|系统临时目录||％H|user.home系统属性的值||％G|循环日志的世代号||％u|用于解析命名冲突的唯一编号||%%|人物||/|路径分隔符|％t|系统临时目录|％H|user.home系统属性的值|％G|循环日志的世代号|％u|用于解析命名冲突的唯一编号|%%|人物|％h / java％u.log|
| | | | | | | | | | | | | | | |
|/|路径分隔符| | | | | | | | | | | | | |
|％t|系统临时目录| | | | | | | | | | | | | |
|％H|user.home系统属性的值| | | | | | | | | | | | | |
|％G|循环日志的世代号| | | | | | | | | | | | | |
|％u|用于解析命名冲突的唯一编号| | | | | | | | | | | | | |
|%%|人物| | | | | | | | | | | | | |
|java.util.logging。FileHandler.append|文件记录器的默认附加模式;true以附加到现有日志文件|假| | | | | | | | | | | | |







第13节

## 属性文件


- 包含名称/值对，用=，：或空格分隔
- 名称周围的空格或值开始之前的空格将被忽略
- n可以通过将\作为最后一个字符来继续行;
连续线上的前导空白被忽略`button1.tooltip = This is a long \ tooltip text.`
- \ t \ n \ f \ r \\ \ uxxxx转义被识别（但不是\ b或八进制转义）
- 文件假设在ISO 8859-1中编码;
使用native2ascii将非ASCII字符编码为Unicode转义
- 空行和以＃或！开头的行。被忽略

#### 典型用途：
`Properties props = new Properties(); props.load(new FileInputStream("prog.properties")); String value = props.getProperty("button1.tooltip"); // null if not present`
#### 也用于资源包：
`ResourceBundle bundle = ResourceBundle.getBundle("prog"); // Searches for prog_en_US.properties, // prog_en.properties, etc. String value = bundle.getString("button1.tooltip");`






第14节

## JAR文件


- 用于存储应用程序，代码库
- 默认情况下，类文件和其他资源以ZIP文件格式存储
- META-INF / MANIFEST.MF包含JAR元数据
- META-INF /服务可以包含服务提供程序配置
- 使用jar实用程序创建JAR文件

### JAR实用程序选项
|选项|描述|
|----|----|
|C|创建新的或空的归档文件，并向其中添加文件。如果任何指定的文件名是目录，则jar程序将递归处理它们|
|C|临时更改目录。例如，jar cvfC myprog.jar类* .class更改为classes子目录以添加类文件|
|e|在清单jar中创建Main-Class条目cvfe myprog.jar com.mycom.mypkg.MainClass文件|
|F|指定JAR文件名作为第二个命令行参数。如果缺少此参数，jar将结果写入标准输出（创建JAR文件时）或从标准输入读取（在提取或制表JAR文件时）|
|一世|创建索引文件（用于加速大型存档中的查找）|
|m|将清单添加到JAR文件。jar cvfm myprog.jar mymanifest.mf文件|
|M|不为条目创建清单文件|
|t|显示目录。jar tvf myprog.jar|
|ü|更新现有的JAR文件jar uf myprog.jar com / mycom / mypkg / SomeClass.class|
|v|生成详细输出|
|X|解压缩文件。如果提供一个或多个文件名，则只提取这些文件。否则，将提取所有文件。jar xf myprog.jar|
|O|无ZIP压缩的商店|







第15节

## 常见javac选项


|选项|目的|
|----|----|
|-cp或-classpath|设置类路径，用于搜索类文件。类路径是目录，JAR文件或表单目录/'*'（Unix）或目录\ *（Windows）的表达式的列表。后者指的是给定目录中的所有JAR文件。类路径项由以下分隔：（Unix）或;（视窗）。如果未指定类路径，则将其设置为当前目录。如果指定了类路径，则不会自动包含当前目录 - 添加。项目，如果你想包括它|
|-sourcepath|设置用于搜索源文件的路径。如果给定文件存在源文件和类文件，则如果源文件较新，则对其进行编译。如果未指定源路径，则将其设置为当前目录|
|-d|设置用于放置类文件的路径。使用此选项来分隔.java和.class文件|
|-资源|设置源级别。有效值为1.3,1.4,1.5,1.6,1.7,1.8,5,6,7,8|
|-deprecation|提供有关使用已弃用功能的详细信息|
|-Xlint：未选中|提供有关未检查的类型转换警告的详细信息|
|-cp或-classpath|设置类路径，用于搜索类文件。有关详细信息，请参阅上表。注意，如果当前目录在源路径上而不是类路径上，则java失败时，javac可以成功。|
|-ea或-enableassertions|启用断言。默认情况下，断言被禁用。|
|-Dproperty = value|设置系统可以检索的系统属性。getProperty（String）|
|-罐|运行包含在其清单具有Main-Class条目的JAR文件中的程序。使用此选项时，将忽略类路径。|
|-verbose|显示加载的类。此选项可能有助于调试类加载问题。|
|-Xmssize-Xmxsize|设置初始或最大堆大小。大小是以字节为单位的值。为千字节或兆字节添加后缀k或m，例如-Xmx10m|







第16节

## 常用的java选项


|选项|描述|
|----|----|
|-cp或-classpath|设置类路径，用于搜索类文件。有关详细信息，请参阅上表。注意，如果当前目录在源路径上而不是类路径上，则java失败时，javac可以成功|
|-ea或-enableassertions|启用断言。默认情况下，断言被禁用|
|-D *property = value*|设置可以检索的系统属性System.getProperty（String）|
|-罐|运行包含在其清单具有Main-Class条目的JAR文件中的程序。使用此选项时，将忽略类路径|
|-verbose|显示加载的类。此选项可能有助于调试类加载问题|
|-Xms *大小* -Xmx *大小*|设置初始或最大堆大小。大小是以字节为单位的值。为千字节或兆字节添加后缀k或m，例如-Xmx10m|






