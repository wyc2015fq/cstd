
# java的日历类 - 李昆鹏的博客 - CSDN博客


2018年03月16日 23:35:21[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：100标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


**-----------------------java的日历类-----------------------------------**
## java.util
## 类 Calendar
java.lang.Object**java.util.Calendar****所有已实现的接口：**
Serializable,Cloneable,Comparable<Calendar>
**直接已知子类：**
GregorianCalendar
---

public abstract class**Calendar**extendsObjectimplementsSerializable,Cloneable,Comparable<Calendar>`Calendar`类是一个抽象类，它为特定瞬间与一组诸如`YEAR`、`MONTH`、`DAY_OF_MONTH`、`HOUR`等`日历字段`之间的转换提供了一些方法，并为操作日历字段（例如获得下星期的日期）提供了一些方法。瞬间可用毫秒值来表示，它是距*历元*（即格林威治标准时间 1970 年 1 月 1 日的 00:00:00.000，格里高利历）的偏移量。
该类还为实现包范围外的具体日历系统提供了其他字段和方法。这些字段和方法被定义为`protected`。
与其他语言环境敏感类一样，`Calendar`提供了一个类方法`getInstance`，以获得此类型的一个通用的对象。`Calendar`的`getInstance`方法返回一个`Calendar`对象，其日历字段已由当前日期和时间初始化：
Calendar rightNow = Calendar.getInstance();`Calendar`对象能够生成为特定语言和日历风格实现日期-时间格式化所需的所有日历字段值，例如，日语-格里高里历，日语-传统日历。`Calendar`定义了某些日历字段返回值的范围，以及这些值的含义。例如，对于所有日历，日历系统第一个月的值是`MONTH == JANUARY`。其他值是由具体子类（例如`ERA`）定义的。有关此内容的细节，请参阅每个字段的文档和子类文档。
#### 获得并设置日历字段值
可以通过调用`set`方法来设置日历字段值。在需要计算时间值（距历元所经过的毫秒）或日历字段值之前，不会解释`Calendar`中的所有字段值设置。调用`get`、`getTimeInMillis`、`getTime`、`add`和`roll`涉及此类计算。
#### 宽松性
`Calendar`有两种解释日历字段的模式，即*lenient*和*non-lenient*。当`Calendar`处于 lenient 模式时，它可接受比它所生成的日历字段范围更大范围内的值。当`Calendar`重新计算日历字段值，以便由`get()`返回这些值时，所有日历字段都被标准化。例如，lenient 模式下的`GregorianCalendar`将`MONTH == JANUARY`、`DAY_OF_MONTH == 32`解释为 February 1。
当`Calendar`处于 non-lenient 模式时，如果其日历字段中存在任何不一致性，它都会抛出一个异常。例如，`GregorianCalendar`总是在 1 与月份的长度之间生成`DAY_OF_MONTH`值。如果已经设置了任何超出范围的字段值，那么在计算时间或日历字段值时，处于non-lenient 模式下的`GregorianCalendar`会抛出一个异常。
#### 第一个星期
`Calendar`使用两个参数定义了特定于语言环境的7 天制星期：星期的第一天和第一个星期中的最小一天（从 1 到 7）。这些数字取自构造`Calendar`时的语言环境资源数据。还可以通过为其设置值的方法来显式地指定它们。
在设置或获得`WEEK_OF_MONTH`或`WEEK_OF_YEAR`字段时，`Calendar`必须确定一个月或一年的第一个星期，以此作为参考点。一个月或一年的第一个星期被确定为开始于`getFirstDayOfWeek()`的最早七天，它最少包含那一个月或一年的`getMinimalDaysInFirstWeek()`天数。第一个星期之前的各星期编号为 ...、-1、0；之后的星期编号为 2、3、...。注意，`get()`返回的标准化编号方式可能有所不同。例如，特定`Calendar`子类可能将某一年第 1 个星期之前的那个星期指定为前一年的第`n`个星期。
#### 日历字段解析
在计算日历字段中的日期和时间时，可能没有足够的信息用于计算（例如只有年和月，但没有日），或者可能有不一致的信息( 例如 "Tuesday,July 15, 1996"（格林威治时间）——实际上，1996 年 7 月 15 日是星期一 )。`Calendar`将解析日历字段值，以便用以下方式确定日期和时间。
如果日历字段值中存在任何冲突，则`Calendar`将为最近设置的日历字段提供优先权。以下是日历字段的默认组合。将使用由最近设置的单个字段所确定的最近组合。
对于日期字段：
YEAR + MONTH + DAY_OF_MONTHYEAR + MONTH + WEEK_OF_MONTH + DAY_OF_WEEKYEAR + MONTH + DAY_OF_WEEK_IN_MONTH + DAY_OF_WEEKYEAR + DAY_OF_YEARYEAR + DAY_OF_WEEK + WEEK_OF_YEAR对于时间字段：
HOUR_OF_DAYAM_PM + HOUR如果在选定的字段组合中，还有尚未设置值的任一日历字段，那么`Calendar`将使用其默认值。每个字段的默认值可能依据具体的日历系统而有所不同。例如，在`GregorianCalendar`中，字段的默认值与历元起始部分的字段值相同：即`YEAR = 1970`、`MONTH =JANUARY`、`DAY_OF_MONTH = 1`，等等。
**注：**对于某些特别时间的解释可能会有某些歧义，可以用下列方式解决：
23:59 是一天中的最后一分钟，而 00:00 是下一天的第一分钟。因此，1999 年 12 月 31 日的 23:59 < 2000 年 1 月 1 日的 00:00。
尽管从历史上看不够精确，但午夜也属于 "am",，中午属于 "pm"，所以在同一天，12:00 am ( 午夜 ) < 12:01 am，12:00 pm ( 中午 ) < 12:01 pm。
日期或时间格式字符串不是日历定义的一部分，因为在运行时，用户必须能够修改或重写它们。可以使用`DateFormat`格式化日期。
#### 字段操作
可以使用三种方法更改日历字段：`set()`、`add()`和`roll()`。
`set(f, value)`将日历字段`f`更改为`value`。此外，它设置了一个内部成员变量，以指示日历字段`f`已经被更改。尽管日历字段`f`是立即更改的，但是直到下次调用`get()`、`getTime()`、`getTimeInMillis()`、`add()`或`roll()`时才会重新计算日历的时间值（以毫秒为单位）。因此，多次调用`set()`不会触发多次不必要的计算。使用`set()`更改日历字段的结果是，其他日历字段也可能发生更改，这取决于日历字段、日历字段值和日历系统。此外，在重新计算日历字段之后，`get(f)`没必要通过调用`set`方法返回`value`集合。具体细节是通过具体的日历类确定的。
*示例*：假定`GregorianCalendar`最初被设置为 1999 年 8 月 31 日。调用`set(Calendar.MONTH, Calendar.SEPTEMBER)`将该日期设置为 1999 年 9 月 31 日。如果随后调用`getTime()`，那么这是解析 1999 年 10 月 1 日的一个暂时内部表示。但是，在调用`getTime()`之前调用`set(Calendar.DAY_OF_MONTH, 30)`会将该日期设置为 1999 年 9 月 30 日，因为在调用`set()`之后没有发生重新计算。
`add(f, delta)`将`delta`添加到`f`字段中。这等同于调用`set(f, get(f) + delta)`，但要带以下两个调整：
**Add 规则 1**。调用后`f`字段的值减去调用前`f`字段的值等于`delta`，以字段`f`中发生的任何溢出为模。溢出发生在字段值超出其范围时，结果，下一个更大的字段会递增或递减，并将字段值调整回其范围内。
**Add 规则 2**。如果期望某一个更小的字段是不变的，但让它等于以前的值是不可能的，因为在字段`f`发生更改之后，或者在出现其他约束之后，比如时区偏移量发生更改，它的最大值和最小值也在发生更改，然后它的值被调整为尽量接近于所期望的值。更小的字段表示一个更小的时间单元。`HOUR`是一个比`DAY_OF_MONTH`小的字段。对于不期望是不变字段的更小字段，无需进行任何调整。日历系统会确定期望不变的那些字段。
此外，与`set()`不同，`add()`强迫日历系统立即重新计算日历的毫秒数和所有字段。
*示例*：假定`GregorianCalendar`最初被设置为 1999 年 8 月 31 日。调用`add(Calendar.MONTH, 13)`将日历设置为 2000 年 9 月 30 日。**Add 规则 1**将`MONTH`字段设置为 September，因为向August 添加 13 个月得出的就是下一年的September。因为在`GregorianCalendar`中，`DAY_OF_MONTH`不可能是 9 月 31 日，所以**add 规则 2**将`DAY_OF_MONTH`设置为 30，即最可能的值。尽管它是一个更小的字段，但不能根据规则 2 调整`DAY_OF_WEEK`，因为在`GregorianCalendar`中的月份发生变化时，该值也需要发生变化。
`roll(f, delta)`将`delta`添加到`f`字段中，但不更改更大的字段。这等同于调用`add(f, delta)`，但要带以下调整：
**Roll规则**。在完成调用后，更大的字段无变化。更大的字段表示一个更大的时间单元。`DAY_OF_MONTH`是一个比`HOUR`大的字段。
*示例*：请参阅`GregorianCalendar.roll(int,int)`。
**使用模型**。为了帮助理解`add()`和`roll()`的行为，假定有一个用户界面组件，它带有用于月、日、年和底层`GregorianCalendar`的递增或递减按钮。如果从界面上读取的日期为 1999 年 1 月 31 日，并且用户按下月份的递增按钮，那么应该得到什么？如果底层实现使用`set()`，那么可以将该日期读为 1999 年 3 月 3 日。更好的结果是 1999年 2 月 28 日。此外，如果用户再次按下月份的递增按钮，那么该日期应该读为 1999 年 3 月 31 日，而不是 1999 年 3 月 28 日。通过保存原始日期并使用`add()`或`roll()`，根据是否会影响更大的字段，用户界面可以像大多数用户所期望的那样运行。
|字段摘要
|
|static int
|ALL_STYLES
|指示所有风格名称的|getDisplayNames|的风格说明符，比如  "January" 和 "Jan"。
|
|static int
|AM
|指示从午夜到中午之前这段时间的|AM_PM|字段值。
|
|static int
|AM_PM
|get|和|set|的字段数字，指示|HOUR|是在中午之前还是在中午之后。
|
|static int
|APRIL
|指示在格里高利历和罗马儒略历中一年中第四个月的|MONTH|字段值。
|
|protected  boolean
|areFieldsSet
|如果|fields[]|与当前的设置时间同步，则返回 true。
|
|static int
|AUGUST
|指示在格里高利历和罗马儒略历中一年中第八个月的|MONTH|字段值。
|
|static int
|DATE
|get|和|set|的字段数字，指示一个月中的某天。
|
|static int
|DAY_OF_MONTH
|get|和|set|的字段数字，指示一个月中的某天。
|
|static int
|DAY_OF_WEEK
|get|和|set|的字段数字，指示一个星期中的某天。
|
|static int
|DAY_OF_WEEK_IN_MONTH
|get|和|set|的字段数字，指示当前月中的第几个星期。
|
|static int
|DAY_OF_YEAR
|get|和|set|的字段数字，指示当前年中的天数。
|
|static int
|DECEMBER
|指示在格里高利历和罗马儒略历中一年中第十二个月的|MONTH|字段值。
|
|static int
|DST_OFFSET
|get|和|set|的字段数字，以毫秒为单位指示夏令时的偏移量。
|
|static int
|ERA
|指示年代的|get|和|set|的字段数字，比如罗马儒略历中的 AD 或 BC。
|
|static int
|FEBRUARY
|指示在格里高利历和罗马儒略历中一年中第二个月的|MONTH|字段值。
|
|static int
|FIELD_COUNT
|get|和|set|可识别的不同字段的数量。
|
|protected  int[]
|fields
|此日历当前设置时间的日历字段值。
|
|static int
|FRIDAY
|指示 Friday 的|DAY_OF_WEEK|字段值。
|
|static int
|HOUR
|get|和|set|的字段数字，指示上午或下午的小时。
|
|static int
|HOUR_OF_DAY
|get|和|set|的字段数字，指示一天中的小时。
|
|protected  boolean[]
|isSet
|通知是否设置了该日历某一指定日历字段的标志。
|
|protected  boolean
|isTimeSet
|如果|time|值是一个有效值，则返回 true。
|
|static int
|JANUARY
|指示在格里高利历和罗马儒略历中一年中第一个月的|MONTH|字段值。
|
|static int
|JULY
|指示在格里高利历和罗马儒略历中一年中第七个月的|MONTH|字段值。
|
|static int
|JUNE
|指示在格里高利历和罗马儒略历中一年中第六个月的|MONTH|字段值。
|
|static int
|LONG
|指示长名称的|getDisplayName|和|getDisplayNames|的风格说明符，比如  "January"。
|
|static int
|MARCH
|指示在格里高利历和罗马儒略历中一年中第三个月的|MONTH|字段值。
|
|static int
|MAY
|指示在格里高利历和罗马儒略历中一年中第五个月的|MONTH|字段值。
|
|static int
|MILLISECOND
|get|和|set|的字段数字，指示一秒中的毫秒。
|
|static int
|MINUTE
|get|和|set|的字段数字，指示一小时中的分钟。
|
|static int
|MONDAY
|指示 Monday 的|DAY_OF_WEEK|字段值。
|
|static int
|MONTH
|指示月份的|get|和|set|的字段数字。
|
|static int
|NOVEMBER
|指示在格里高利历和罗马儒略历中一年中第十一个月的|MONTH|字段值。
|
|static int
|OCTOBER
|指示在格里高利历和罗马儒略历中一年中第十个月的|MONTH|字段值。
|
|static int
|PM
|指示从中午到午夜之前这段时间的|AM_PM|字段值。
|
|static int
|SATURDAY
|指示 Saturday 的|DAY_OF_WEEK|字段值。
|
|static int
|SECOND
|get|和|set|的字段数字，指示一分钟中的秒。
|
|static int
|SEPTEMBER
|指示在格里高利历和罗马儒略历中一年中第九个月的|MONTH|字段值。
|
|static int
|SHORT
|指示短名称的|getDisplayName|和|getDisplayNames|的风格说明符，比如  "Jan"。
|
|static int
|SUNDAY
|指示 Sunday 的|DAY_OF_WEEK|字段值。
|
|static int
|THURSDAY
|指示 Thursday 的|DAY_OF_WEEK|字段值。
|
|protected  long
|time
|日历的当前设置时间，以毫秒为单位，表示自格林威治标准时间 1970 年 1月 1 日 0:00:00 后经过的时间。
|
|static int
|TUESDAY
|指示 Tuesday 的|DAY_OF_WEEK|字段值。
|
|static int
|UNDECIMBER
|指示一年中第十三个月的|MONTH|字段值。
|
|static int
|WEDNESDAY
|指示 Wednesday 的|DAY_OF_WEEK|字段值。
|
|static int
|WEEK_OF_MONTH
|get|和|set|的字段数字，指示当前月中的星期数。
|
|static int
|WEEK_OF_YEAR
|get|和|set|的字段数字，指示当前年中的星期数。
|
|static int
|YEAR
|指示年的|get|和|set|的字段数字。
|
|static int
|ZONE_OFFSET
|get|和|set|的字段数字，以毫秒为单位指示距 GMT 的大致偏移量。
|
|构造方法摘要
|
|protected
|Calendar|()
|构造一个带有默认时区和语言环境的  Calendar。
|
|protected
|Calendar|(|TimeZone|zone,|Locale|aLocale)
|构造一个带有指定时区和语言环境的  Calendar。
|
|方法摘要
|
|abstract  void
|add|(int field, int amount)
|根据日历的规则，为给定的日历字段添加或减去指定的时间量。
|
|boolean
|after|(|Object|when)
|判断此|Calendar|表示的时间是否在指定|Object|表示的时间之后，返回判断结果。
|
|boolean
|before|(|Object|when)
|判断此|Calendar|表示的时间是否在指定|Object|表示的时间之前，返回判断结果。
|
|void
|clear|()
|将此|Calendar|的所日历字段值和时间值（从|历元|至现在的毫秒偏移量）设置成未定义。
|
|void
|clear|(int field)
|将此|Calendar|的给定日历字段值和时间值（从|历元|至现在的毫秒偏移量）设置成未定义。
|
|Object
|clone|()
|创建并返回此对象的一个副本。
|
|int
|compareTo|(|Calendar|anotherCalendar)
|比较两个|Calendar|对象表示的时间值（从|历元|至现在的毫秒偏移量）。
|
|protected  void
|complete|()
|填充日历字段中所有未设置的字段。
|
|protected abstract  void
|computeFields|()
|将当前毫秒时间值|time|转换为|fields[]|中的日历字段值。
|
|protected abstract  void
|computeTime|()
|将|fields[]|中的当前日历字段值转换为毫秒时间值|time|。
|
|boolean
|equals|(|Object|obj)
|将此|Calendar|与指定|Object|比较。
|
|int
|get|(int field)
|返回给定日历字段的值。
|
|int
|getActualMaximum|(int field)
|给定此|Calendar|的时间值，返回指定日历字段可能拥有的最大值。
|
|int
|getActualMinimum|(int field)
|给定此|Calendar|的时间值，返回指定日历字段可能拥有的最小值。
|
|static|Locale|[]
|getAvailableLocales|()
|返回所有语言环境的数组，此类的|getInstance|方法可以为其返回本地化的实例。
|
|String
|getDisplayName|(int field, int style,|Locale|locale)
|返回给定|style|和|locale|下的日历|field|值的字符串表示形式。
|
|Map|<|String|,|Integer|>
|getDisplayNames|(int field, int style,|Locale|locale)
|返回给定|style|和|locale|下包含日历|field|所有名称的|Map|及其相应字段值。
|
|int
|getFirstDayOfWeek|()
|获取一星期的第一天；例如，在美国，这一天是|SUNDAY|，而在法国，这一天是|MONDAY|。
|
|abstract  int
|getGreatestMinimum|(int field)
|返回此|Calendar|实例给定日历字段的最高的最小值。
|
|static|Calendar
|getInstance|()
|使用默认时区和语言环境获得一个日历。
|
|static|Calendar
|getInstance|(|Locale|aLocale)
|使用默认时区和指定语言环境获得一个日历。
|
|static|Calendar
|getInstance|(|TimeZone|zone)
|使用指定时区和默认语言环境获得一个日历。
|
|static|Calendar
|getInstance|(|TimeZone|zone,|Locale|aLocale)
|使用指定时区和语言环境获得一个日历。
|
|abstract  int
|getLeastMaximum|(int field)
|返回此|Calendar|实例给定日历字段的最低的最大值。
|
|abstract  int
|getMaximum|(int field)
|返回此|Calendar|实例给定日历字段的最大值。
|
|int
|getMinimalDaysInFirstWeek|()
|获取一年中第一个星期所需的最少天数，例如，如果定义第一个星期包含一年第一个月的第一天，则此方法将返回 1。
|
|abstract  int
|getMinimum|(int field)
|返回此|Calendar|实例给定日历字段的最小值。
|
|Date
|getTime|()
|返回一个表示此|Calendar|时间值（从|历元|至现在的毫秒偏移量）的|Date|对象。
|
|long
|getTimeInMillis|()
|返回此 Calendar 的时间值，以毫秒为单位。
|
|TimeZone
|getTimeZone|()
|获得时区。
|
|int
|hashCode|()
|返回该此日历的哈希码。
|
|protected  int
|internalGet|(int field)
|返回给定日历字段的值。
|
|boolean
|isLenient|()
|判断日期/时间的解释是否为宽松的。
|
|boolean
|isSet|(int field)
|确定给定日历字段是否已经设置了一个值，其中包括因为调用|get|方法触发内部字段计算而导致已经设置该值的情况。
|
|abstract  void
|roll|(int field, boolean up)
|在给定的时间字段上添加或减去（上/下）单个时间单元，不更改更大的字段。
|
|void
|roll|(int field, int amount)
|向指定日历字段添加指定（有符号的）时间量，不更改更大的字段。
|
|void
|set|(int field, int value)
|将给定的日历字段设置为给定值。
|
|void
|set|(int year, int month,  int date)
|设置日历字段|YEAR|、|MONTH|和|DAY_OF_MONTH|的值。
|
|void
|set|(int year, int month,  int date, int hourOfDay, int minute)
|设置日历字段|YEAR|、|MONTH|、|DAY_OF_MONTH|、|HOUR_OF_DAY|和|MINUTE|的值。
|
|void
|set|(int year, int month,  int date, int hourOfDay, int minute, int second)
|设置字段|YEAR|、|MONTH|、|DAY_OF_MONTH|、|HOUR|、|MINUTE|和|SECOND|的值。
|
|void
|setFirstDayOfWeek|(int value)
|设置一星期的第一天是哪一天；例如，在美国，这一天是|SUNDAY|，而在法国，这一天是|MONDAY|。
|
|void
|setLenient|(boolean lenient)
|指定日期/时间解释是否是宽松的。
|
|void
|setMinimalDaysInFirstWeek|(int value)
|设置一年中第一个星期所需的最少天数，例如，如果定义第一个星期包含一年第一个月的第一天，则使用值 1 调用此方法。
|
|void
|setTime|(|Date|date)
|使用给定的|Date|设置此 Calendar 的时间。
|
|void
|setTimeInMillis|(long millis)
|用给定的 long 值设置此 Calendar 的当前时间值。
|
|void
|setTimeZone|(|TimeZone|value)
|使用给定的时区值来设置时区。
|
|String
|toString|()
|返回此日历的字符串表示形式。
|

----------------------------------------------------------


## java.util
## 类 GregorianCalendar
java.lang.Objectjava.util.Calendar**java.util.GregorianCalendar****所有已实现的接口：**
Serializable,Cloneable,Comparable<Calendar>
---

public class**GregorianCalendar**extendsCalendar`GregorianCalendar`是`Calendar`的一个具体子类，提供了世界上大多数国家/地区使用的标准日历系统。
`GregorianCalendar`是一种混合日历，在单一间断性的支持下同时支持儒略历和格里高利历系统，在默认情况下，它对应格里高利日历创立时的格里高利历日期（某些国家/地区是在 1582 年 10 月 15 日创立，在其他国家/地区要晚一些）。可由调用者通过调用`setGregorianChange()`来更改起始日期。
历史上，在那些首先采用格里高利历的国家/地区中，1582 年 10 月 4 日（儒略历）之后就是1582 年 10 月 15 日（格里高利历）。此日历正确地模拟了这些变化。在开始格里高利历之前，`GregorianCalendar`实现的是儒略历。格里高利历和儒略历之间的唯一区别就是闰年规则。儒略历指定每 4 年就为闰年，而格里高利历则忽略不能被 400 整除的世纪年。
`GregorianCalendar`可实现*预期的*格里高利历和儒略历。也就是说，可以通过在时间上无限地向后或向前外推当前规则来计算日期。因此，对于所有的年份，都可以使用`GregorianCalendar`来生成有意义并且一致的结果。但是，采用现代儒略历规则时，使用`GregorianCalendar`得到的日期只在历史上从公元 4 年 3 月 1 日之后是准确的。在此日期之前，闰年规则的应用没有规则性，在 45 BC 之前，甚至不存在儒略历。
在格里高利历创立以前，新年是 3 月 25 日。为了避免混淆，此日历始终使用 1 月 1 日为新年。如果想要格里高利历转换之前并且处于 1 月 1 日和 3 月 24 日之间的日期，则可以进行手动调整。
为`WEEK_OF_YEAR`字段所计算的值的范围从 1 到 53。一年的第一个星期始于`getFirstDayOfWeek()`的最早 7 天，至少包含该年的`getMinimalDaysInFirstWeek()`各天。这取决于`getMinimalDaysInFirstWeek()`、`getFirstDayOfWeek()`的值以及 1 月 1 日是星期几。一年的第一个星期和下一年的第一个星期之间的各个星期按顺序从 2 到 52 或 53（根据需要）进行编号。
例如，1998 年 1 月 1 日是星期四。如果`getFirstDayOfWeek()`为`MONDAY`，并且`getMinimalDaysInFirstWeek()`为 4（这些值反映了 ISO 8601 和很多国家/地区标准），则 1998 年的第一个星期开始于 1997 年 12 月 29 日，结束于 1998 年 1 月 4 日。但是，如果`getFirstDayOfWeek()`为`SUNDAY`，那么 1998 年的第一个星期开始于 1998 年 1 月 4 日，结束于 1998 年 1 月 10 日；1998 年头三天是 1997 年第53 个星期的一部分。
为`WEEK_OF_MONTH`字段所计算的值的范围从 0 到 6。一个月的第一个星期（`WEEK_OF_MONTH = 1`的日期）是该月至少连续`getMinimalDaysInFirstWeek()`天中的最早日期，结束于`getFirstDayOfWeek()`的前一天。与一年的第一个星期不同，一个月的第一个星期可能短于 7 天，也不必从`getFirstDayOfWeek()`这一天开始，并且不包括前一个月的日期。在第一个星期之前该月日期的`WEEK_OF_MONTH`为 0。
例如，如果`getFirstDayOfWeek()`为`SUNDAY`，`getMinimalDaysInFirstWeek()`为 4，那么 1998 年 1 月的第一个星期是从 1 月 4 日星期日到 1 月 10 日星期六。这些天的`WEEK_OF_MONTH`为 1。1 月 1 日星期四到 1 月 3 日星期六的`WEEK_OF_MONTH`为 0。如果`getMinimalDaysInFirstWeek()`变为 3，则 1 月 1 日到 1 月 3 日的`WEEK_OF_MONTH`为 1。
`clear`方法将日历字段设置为未定义。`GregorianCalendar`为每个日历字段使用以下默认值（如果该值未定义）。
|字段
|默认值
|
|ERA
|AD
|
|YEAR
|1970
|
|MONTH
|JANUARY
|
|DAY_OF_MONTH
|1
|
|DAY_OF_WEEK
|一个星期的第一天
|
|WEEK_OF_MONTH
|0
|
|DAY_OF_WEEK_IN_MONTH
|1
|
|AM_PM
|AM
|
|HOUR,  HOUR_OF_DAY, MINUTE, SECOND, MILLISECOND
|0
|
默认值不适用于以上未列出的字段。
**示例：**
// get the supported ids for GMT-08:00 (Pacific Standard Time)String[] ids = TimeZone.getAvailableIDs(-8 * 60 * 60 * 1000);// if no ids were returned, something is wrong. get out.if (ids.length == 0)System.exit(0);// begin outputSystem.out.println("Current Time");// create a Pacific Standard Time time zoneSimpleTimeZone pdt = new SimpleTimeZone(-8 * 60 * 60 * 1000, ids[0]);// set up rules for daylight savings timepdt.setStartRule(Calendar.APRIL, 1, Calendar.SUNDAY, 2 * 60 * 60 * 1000);pdt.setEndRule(Calendar.0ctober, -1, Calendar.SUNDAY, 2 * 60 * 60 * 1000);// create a GregorianCalendar with the Pacific Daylight time zone// and the current date and timeCalendar calendar = new GregorianCalendar(pdt);Date trialTime = new Date();calendar.setTime(trialTime);// print out a bunch of interesting thingsSystem.out.println("ERA: " + calendar.get(Calendar.ERA));System.out.println("YEAR: " + calendar.get(Calendar.YEAR));System.out.println("MONTH: " + calendar.get(Calendar.MONTH));System.out.println("WEEK_OF_YEAR: " + calendar.get(Calendar.WEEK_OF_YEAR));System.out.println("WEEK_OF_MONTH: " + calendar.get(Calendar.WEEK_OF_MONTH));System.out.println("DATE: " + calendar.get(Calendar.DATE));System.out.println("DAY_OF_MONTH: " + calendar.get(Calendar.DAY_OF_MONTH));System.out.println("DAY_OF_YEAR: " + calendar.get(Calendar.DAY_OF_YEAR));System.out.println("DAY_OF_WEEK: " + calendar.get(Calendar.DAY_OF_WEEK));System.out.println("DAY_OF_WEEK_IN_MONTH: "+ calendar.get(Calendar.DAY_OF_WEEK_IN_MONTH));System.out.println("AM_PM: " + calendar.get(Calendar.AM_PM));System.out.println("HOUR: " + calendar.get(Calendar.HOUR));System.out.println("HOUR_OF_DAY: " + calendar.get(Calendar.HOUR_OF_DAY));System.out.println("MINUTE: " + calendar.get(Calendar.MINUTE));System.out.println("SECOND: " + calendar.get(Calendar.SECOND));System.out.println("MILLISECOND: " + calendar.get(Calendar.MILLISECOND));System.out.println("ZONE_OFFSET: "+ (calendar.get(Calendar.ZONE_OFFSET)/(60*60*1000)));System.out.println("DST_OFFSET: "+ (calendar.get(Calendar.DST_OFFSET)/(60*60*1000)));System.out.println("Current Time, with hour reset to 3");calendar.clear(Calendar.HOUR_OF_DAY); // so doesn't overridecalendar.set(Calendar.HOUR, 3);System.out.println("ERA: " + calendar.get(Calendar.ERA));System.out.println("YEAR: " + calendar.get(Calendar.YEAR));System.out.println("MONTH: " + calendar.get(Calendar.MONTH));System.out.println("WEEK_OF_YEAR: " + calendar.get(Calendar.WEEK_OF_YEAR));System.out.println("WEEK_OF_MONTH: " + calendar.get(Calendar.WEEK_OF_MONTH));System.out.println("DATE: " + calendar.get(Calendar.DATE));System.out.println("DAY_OF_MONTH: " + calendar.get(Calendar.DAY_OF_MONTH));System.out.println("DAY_OF_YEAR: " + calendar.get(Calendar.DAY_OF_YEAR));System.out.println("DAY_OF_WEEK: " + calendar.get(Calendar.DAY_OF_WEEK));System.out.println("DAY_OF_WEEK_IN_MONTH: "+ calendar.get(Calendar.DAY_OF_WEEK_IN_MONTH));System.out.println("AM_PM: " + calendar.get(Calendar.AM_PM));System.out.println("HOUR: " + calendar.get(Calendar.HOUR));System.out.println("HOUR_OF_DAY: " + calendar.get(Calendar.HOUR_OF_DAY));System.out.println("MINUTE: " + calendar.get(Calendar.MINUTE));System.out.println("SECOND: " + calendar.get(Calendar.SECOND));System.out.println("MILLISECOND: " + calendar.get(Calendar.MILLISECOND));System.out.println("ZONE_OFFSET: "+ (calendar.get(Calendar.ZONE_OFFSET)/(60*60*1000))); // in hoursSystem.out.println("DST_OFFSET: "+ (calendar.get(Calendar.DST_OFFSET)/(60*60*1000))); // in hours|字段摘要
|
|static int
|AD
|ERA|字段的值指示公元后（耶稣纪元后）的时期（也称为  CE）。
|
|static int
|BC
|ERA|字段的值指示公元前（Christ 前）的时期（也称为 BCE）。
|
|从类 java.util.|Calendar|继承的字段
|
|ALL_STYLES|,|AM|,|AM_PM|,|APRIL|,|areFieldsSet|,|AUGUST|,|DATE|,|DAY_OF_MONTH|,|DAY_OF_WEEK|,|DAY_OF_WEEK_IN_MONTH|,|DAY_OF_YEAR|,|DECEMBER|,|DST_OFFSET|,|ERA|,|FEBRUARY|,|FIELD_COUNT|,|fields|,|FRIDAY|,|HOUR|,|HOUR_OF_DAY|,|isSet|,|isTimeSet|,|JANUARY|,|JULY|,|JUNE|,|LONG|,|MARCH|,|MAY|,|MILLISECOND|,|MINUTE|,|MONDAY|,|MONTH|,|NOVEMBER|,|OCTOBER|,|PM|,|SATURDAY|,|SECOND|,|SEPTEMBER|,|SHORT|,|SUNDAY|,|THURSDAY|,|time|,|TUESDAY|,|UNDECIMBER|,|WEDNESDAY|,|WEEK_OF_MONTH|,|WEEK_OF_YEAR|,|YEAR|,|ZONE_OFFSET
|
|构造方法摘要
|
|GregorianCalendar|()
|在具有默认语言环境的默认时区内使用当前时间构造一个默认的|GregorianCalendar|。
|
|GregorianCalendar|(int year, int month,  int dayOfMonth)
|在具有默认语言环境的默认时区内构造一个带有给定日期设置的|GregorianCalendar|。
|
|GregorianCalendar|(int year, int month,  int dayOfMonth, int hourOfDay, int minute)
|为具有默认语言环境的默认时区构造一个具有给定日期和时间设置的|GregorianCalendar|。
|
|GregorianCalendar|(int year, int month,  int dayOfMonth, int hourOfDay, int minute, int second)
|为具有默认语言环境的默认时区构造一个具有给定日期和时间设置的 GregorianCalendar。
|
|GregorianCalendar|(|Locale|aLocale)
|在具有给定语言环境的默认时区内构造一个基于当前时间的|GregorianCalendar|。
|
|GregorianCalendar|(|TimeZone|zone)
|在具有默认语言环境的给定时区内构造一个基于当前时间的|GregorianCalendar|。
|
|GregorianCalendar|(|TimeZone|zone,|Locale|aLocale)
|在具有给定语言环境的给定时区内构造一个基于当前时间的|GregorianCalendar|。
|
|方法摘要
|
|void
|add|(int field, int amount)
|根据日历规则，将指定的（有符号的）时间量添加到给定的日历字段中。
|
|Object
|clone|()
|创建并返回此对象的一个副本。
|
|protected  void
|computeFields|()
|将时间值（从|Epoch|至现在的毫秒偏移量）转换为日历字段值。
|
|protected  void
|computeTime|()
|将日历字段值转换为时间值（从|Epoch|至现在的毫秒偏移量）。
|
|boolean
|equals|(|Object|obj)
|比较此|GregorianCalendar|与指定的|Object|。
|
|int
|getActualMaximum|(int field)
|考虑到给定的时间值和|getFirstDayOfWeek|、|getMinimalDaysInFirstWeek|、|getGregorianChange|和|getTimeZone|方法的当前值，返回此日历字段可能具有的最大值。
|
|int
|getActualMinimum|(int field)
|考虑到给定的时间值和|getFirstDayOfWeek|、|getMinimalDaysInFirstWeek|、|getGregorianChange|和|getTimeZone|方法的当前值，返回此日历字段可能具有的最小值。
|
|int
|getGreatestMinimum|(int field)
|返回此|GregorianCalendar|实例给定日历字段的最高的最小值。
|
|Date
|getGregorianChange|()
|获得格里高利历的更改日期。
|
|int
|getLeastMaximum|(int field)
|返回此|GregorianCalendar|实例给定日历字段的最低的最大值。
|
|int
|getMaximum|(int field)
|返回此|GregorianCalendar|实例的给定日历字段的最大值。
|
|int
|getMinimum|(int field)
|返回此|GregorianCalendar|实例的给定日历字段的最小值。
|
|TimeZone
|getTimeZone|()
|获得时区。
|
|int
|hashCode|()
|生成此|GregorianCalendar|对象的哈希码。
|
|boolean
|isLeapYear|(int year)
|确定给定的年份是否为闰年。
|
|void
|roll|(int field, boolean up)
|在给定的时间字段上添加或减去（上/下）单个时间单元，不更改更大的字段。
|
|void
|roll|(int field, int amount)
|向指定日历字段添加有符号的时间量，不更改更大的字段。
|
|void
|setGregorianChange|(|Date|date)
|设置|GregorianCalendar|的更改日期。
|
|void
|setTimeZone|(|TimeZone|zone)
|使用给定的时区值来设置时区。
|
例子：
**public****class**Test3 {
**public****static****void**main(String[]args) {
Calendarca=Calendar.*getInstance*();
/**
*java.util.GregorianCalendar[1970年到现在的毫秒time=1521211821639
*,areFieldsSet=true
*,areAllFieldsSet=true
*,lenient=true
* ,时区zone=sun.util.calendar.ZoneInfo[id="Asia/Shanghai"
*,offset=28800000
*,dstSavings=0
*,useDaylight=false
*,transitions=19
*,lastRule=null]
* ,一周的第一天firstDayOfWeek=1
*,minimalDaysInFirstWeek=1
*,ERA=1,年YEAR=2018,月一定要+1MONTH=2
* ,年的第几周WEEK_OF_YEAR=11
* ,当月的第几周WEEK_OF_MONTH=3
* ,当月的第几天DAY_OF_MONTH=16
* ,一年的第几天DAY_OF_YEAR=75
* ,当周的第几天按国外算多一天DAY_OF_WEEK=6
* ,当天是在当月的第几周DAY_OF_WEEK_IN_MONTH=3
* ,上午0下午1  AM_PM=1
* ,小时HOUR=10
* ,一天中的多少小时HOUR_OF_DAY=22
* ,分钟MINUTE=50
* ,秒钟SECOND=21
* ,毫秒MILLISECOND=639
*,ZONE_OFFSET=28800000
*,DST_OFFSET=0]
*/
System.**out**.println(ca);
//获得年
**int**year=ca.get(Calendar.**YEAR**);
//获得月,注意获得月份需要加一
**int**month=ca.get(Calendar.**MONTH**)+1;
//获得日
**int**day=ca.get(Calendar.**DAY_OF_MONTH**);
//获得时
**int**hour=ca.get(Calendar.**HOUR_OF_DAY**);
//获得分钟
**int**minute=ca.get(Calendar.**MINUTE**);
//获得秒钟
**int**second=ca.get(Calendar.**SECOND**);
//获得毫秒
**int**ms=ca.get(Calendar.**MILLISECOND**);
System.**out**.println(year+"年"+month+"月"+day+"日"+"\t"+hour+"时"+minute+"分"+second+"秒"+ms+"毫秒");
//设置日历
ca.set(Calendar.**YEAR**,1985);
//设置月份需要减一
ca.set(Calendar.**MONTH**,04);
ca.set(Calendar.**DAY_OF_MONTH**,22);
ca.set(Calendar.**HOUR**,6);
ca.set(Calendar.**MINUTE**,23);
ca.set(Calendar.**SECOND**,34);
System.**out**.println(ca);
ca.set(1966, 0, 1, 2,3, 4);
System.**out**.println(ca);
//获得日历相关的毫秒数
**long**tm=ca.getTimeInMillis();
Datedate=**new**Date(tm);
//设置格式
SimpleDateFormatsdf=**new**SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
Stringstr=sdf.format(date);
System.**out**.println(str);
System.**out**.println(date);
}
}
结果 ：
java.util.GregorianCalendar[time=1521214332466,areFieldsSet=true,areAllFieldsSet=true,lenient=true,zone=sun.util.calendar.ZoneInfo[id="Asia/Shanghai",offset=28800000,dstSavings=0,useDaylight=false,transitions=19,lastRule=null],firstDayOfWeek=1,minimalDaysInFirstWeek=1,ERA=1,YEAR=2018,MONTH=2,WEEK_OF_YEAR=11,WEEK_OF_MONTH=3,DAY_OF_MONTH=16,DAY_OF_YEAR=75,DAY_OF_WEEK=6,DAY_OF_WEEK_IN_MONTH=3,AM_PM=1,HOUR=11,HOUR_OF_DAY=23,MINUTE=32,SECOND=12,MILLISECOND=466,ZONE_OFFSET=28800000,DST_OFFSET=0]
2018年3月16日23时32分12秒466毫秒
java.util.GregorianCalendar[time=?,areFieldsSet=false,areAllFieldsSet=true,lenient=true,zone=sun.util.calendar.ZoneInfo[id="Asia/Shanghai",offset=28800000,dstSavings=0,useDaylight=false,transitions=19,lastRule=null],firstDayOfWeek=1,minimalDaysInFirstWeek=1,ERA=1,YEAR=1985,MONTH=4,WEEK_OF_YEAR=11,WEEK_OF_MONTH=3,DAY_OF_MONTH=22,DAY_OF_YEAR=75,DAY_OF_WEEK=6,DAY_OF_WEEK_IN_MONTH=3,AM_PM=1,HOUR=6,HOUR_OF_DAY=23,MINUTE=23,SECOND=34,MILLISECOND=466,ZONE_OFFSET=28800000,DST_OFFSET=0]
java.util.GregorianCalendar[time=?,areFieldsSet=false,areAllFieldsSet=true,lenient=true,zone=sun.util.calendar.ZoneInfo[id="Asia/Shanghai",offset=28800000,dstSavings=0,useDaylight=false,transitions=19,lastRule=null],firstDayOfWeek=1,minimalDaysInFirstWeek=1,ERA=1,YEAR=1966,MONTH=0,WEEK_OF_YEAR=11,WEEK_OF_MONTH=3,DAY_OF_MONTH=1,DAY_OF_YEAR=75,DAY_OF_WEEK=6,DAY_OF_WEEK_IN_MONTH=3,AM_PM=1,HOUR=6,HOUR_OF_DAY=2,MINUTE=3,SECOND=4,MILLISECOND=466,ZONE_OFFSET=28800000,DST_OFFSET=0]
1966-01-01 02:03:04
Sat Jan 01 02:03:04CST 1966


