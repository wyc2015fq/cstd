# Java实用工具类包util API(java.util)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月16日 10:01:07[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1877








包含集合框架，旧集合类，事件模型，日期和时间设施，国际化和其他实用程序类（字符串tokenizer，随机数生成器和位数组）。







一、包规格


- [Collections Framework Overview](https://developer.android.google.cn/openjdk-redirect.html?v=8&path=/technotes/guides/collections/overview.html)
- [Collections Framework
 Annotated Outline](https://developer.android.google.cn/openjdk-redirect.html?v=8&path=/technotes/guides/collections/reference.html)







二、相关文档

有关概述，教程，示例，指南和工具文档，请参阅：



- [Collections Framework Tutorial](http://www.java.sun.com/docs/books/tutorial/collections/)
- [Collections Framework
 Design FAQ](https://developer.android.google.cn/openjdk-redirect.html?v=8&path=/technotes/guides/collections/designfaq.html)







**接口**

[Collection](https://developer.android.google.cn/reference/java/util/Collection.html)<E>


集合层次结构中的根接口。





[Comparator](https://developer.android.google.cn/reference/java/util/Comparator.html)<T>


比较功能，对一些对象的集合施加了一个整体排序。





[Deque](https://developer.android.google.cn/reference/java/util/Deque.html)<E>


支持两端元素插入和移除的线性集合。





[Enumeration](https://developer.android.google.cn/reference/java/util/Enumeration.html)<E>


实现枚举接口的对象生成一系列元素，一次一个。





[EventListener](https://developer.android.google.cn/reference/java/util/EventListener.html)


所有事件侦听器接口必须扩展的标记接口。





[Formattable](https://developer.android.google.cn/reference/java/util/Formattable.html)


Formattable接口必须由需要使用Formatter的's'转换说明符执行自定义格式化的任何类实现。





[Iterator](https://developer.android.google.cn/reference/java/util/Iterator.html)<E>


一个集合的迭代器。





[List](https://developer.android.google.cn/reference/java/util/List.html)<E>


有序集合（也称为序列）。





[ListIterator](https://developer.android.google.cn/reference/java/util/ListIterator.html)<E>


用于允许程序员沿任一方向遍历列表的列表的迭代器，在迭代期间修改列表，并获取列表中迭代器的当前位置。





[Map](https://developer.android.google.cn/reference/java/util/Map.html)<K, V>


将键映射到值的对象。





[Map.Entry](https://developer.android.google.cn/reference/java/util/Map.Entry.html)<K, V>


地图条目（键值对）。





[NavigableMap](https://developer.android.google.cn/reference/java/util/NavigableMap.html)<K, V>


SortedMap扩展了导航方法，返回给定搜索目标的最接近的匹配。





[NavigableSet](https://developer.android.google.cn/reference/java/util/NavigableSet.html)<E>


SortedSet扩展，导航方法报告给定搜索目标的最匹配。





[Observer](https://developer.android.google.cn/reference/java/util/Observer.html)


当它想要被通知可观察对象的变化时，类可以实现Observer接口。





[PrimitiveIterator](https://developer.android.google.cn/reference/java/util/PrimitiveIterator.html)<T, T_CONS>


迭代器原始专业化的基础类型。





[PrimitiveIterator.OfDouble](https://developer.android.google.cn/reference/java/util/PrimitiveIterator.OfDouble.html)


专门针对双重价值的迭代器。





[PrimitiveIterator.OfInt](https://developer.android.google.cn/reference/java/util/PrimitiveIterator.OfInt.html)


一个专门针对int值的迭代器。





[PrimitiveIterator.OfLong](https://developer.android.google.cn/reference/java/util/PrimitiveIterator.OfLong.html)


专门针对长期价值的迭代器。





[Queue](https://developer.android.google.cn/reference/java/util/Queue.html)<E>


设计用于在处理之前保留元素的集合。





[RandomAccess](https://developer.android.google.cn/reference/java/util/RandomAccess.html)


List实现使用的标记界面，表示它们支持快速（通常为恒定时间）随机访问。





[Set](https://developer.android.google.cn/reference/java/util/Set.html)<E>


不包含重复元素的集合。





[SortedMap](https://developer.android.google.cn/reference/java/util/SortedMap.html)<K, V>


一个地图，进一步提供了一个总的顺序的键。





[SortedSet](https://developer.android.google.cn/reference/java/util/SortedSet.html)<E>


一个集合进一步提供了其元素的总排序。





[Spliterator](https://developer.android.google.cn/reference/java/util/Spliterator.html)<T>


用于遍历和分割源的元素的对象。





[Spliterator.OfDouble](https://developer.android.google.cn/reference/java/util/Spliterator.OfDouble.html)


专门用于双重价值的Spliterator。





[Spliterator.OfInt](https://developer.android.google.cn/reference/java/util/Spliterator.OfInt.html)


一个专门针对int值的Spliterator。





[Spliterator.OfLong](https://developer.android.google.cn/reference/java/util/Spliterator.OfLong.html)


一个专门针对长期价值观的拼写器。





[Spliterator.OfPrimitive](https://developer.android.google.cn/reference/java/util/Spliterator.OfPrimitive.html)<T, T_CONS, T_SPLITR extends [OfPrimitive](https://developer.android.google.cn/reference/java/util/Spliterator.OfPrimitive.html)<T, T_CONS, T_SPLITR>>


专门针对原始价值观的Spliterator。








**类**

[AbstractCollection](https://developer.android.google.cn/reference/java/util/AbstractCollection.html)<E>


该类提供了Collection接口的骨架实现，以最大限度地减少实现此接口所需的工作量。





[AbstractList](https://developer.android.google.cn/reference/java/util/AbstractList.html)<E>


该类提供了List接口的骨架实现，以最小化实现由“随机访问”数据存储（如数组）支持的此接口所需的工作量。





[AbstractMap](https://developer.android.google.cn/reference/java/util/AbstractMap.html)<K, V>


该类提供了Map接口的骨架实现，以最大限度地减少实现此接口所需的工作量。





[AbstractMap.SimpleEntry](https://developer.android.google.cn/reference/java/util/AbstractMap.SimpleEntry.html)<K, V>


保存密钥和值的条目。





[AbstractMap.SimpleImmutableEntry](https://developer.android.google.cn/reference/java/util/AbstractMap.SimpleImmutableEntry.html)<K, V>


保持一个不变的钥匙和价值的条目。





[AbstractQueue](https://developer.android.google.cn/reference/java/util/AbstractQueue.html)<E>


这个类提供了一些Queue操作的骨架实现。





[AbstractSequentialList](https://developer.android.google.cn/reference/java/util/AbstractSequentialList.html)<E>


此类提供了List接口的骨架实现，以最小化实现由“顺序访问”数据存储（如链表）支持的此接口所需的工作量。





[AbstractSet](https://developer.android.google.cn/reference/java/util/AbstractSet.html)<E>


该类提供了Set接口的骨架实现，以尽量减少实现此接口所需的工作量。





[ArrayDeque](https://developer.android.google.cn/reference/java/util/ArrayDeque.html)<E>


Deque接口的可调整大小的数组实现。





[ArrayList](https://developer.android.google.cn/reference/java/util/ArrayList.html)<E>


List接口的可调整大小的数组实现。





[Arrays](https://developer.android.google.cn/reference/java/util/Arrays.html)


该类包含用于操作数组的各种方法（如排序和搜索）。





[Base64](https://developer.android.google.cn/reference/java/util/Base64.html)


该类仅由用于获得Base64编码方案的编码器和解码器的静态方法组成。





[Base64.Decoder](https://developer.android.google.cn/reference/java/util/Base64.Decoder.html)


该类使用RFC
 4648和RFC 2045中规定的Base64编码方案来实现用于解码字节数据的解码器。





[Base64.Encoder](https://developer.android.google.cn/reference/java/util/Base64.Encoder.html)


该类使用RFC 4648和RFC 2045中规定的Base64编码方案来实现用于编码字节数据的编码器。





[BitSet](https://developer.android.google.cn/reference/java/util/BitSet.html)


该类实现了根据需要增长的位向量。





[Calendar](https://developer.android.google.cn/reference/java/util/Calendar.html)


Calendar类是一个抽象类，提供了在特定时刻之间进行转换的方法和一系列日历字段（如YEAR，MONTH，DAY_OF_MONTH，HOUR等），以及操作日历字段（如获取日期） 的下一周。





[Calendar.Builder](https://developer.android.google.cn/reference/java/util/Calendar.Builder.html)


Calendar.Builder用于从各种日期时间参数创建日历。





[Collections](https://developer.android.google.cn/reference/java/util/Collections.html)


此类仅由静态方法组合或返回集合。





[Currency](https://developer.android.google.cn/reference/java/util/Currency.html)


代表货币。





[Date](https://developer.android.google.cn/reference/java/util/Date.html)


Class Date代表一个特定的时间，以毫秒的精度。





[Dictionary](https://developer.android.google.cn/reference/java/util/Dictionary.html)<K, V>


Dictionary类是任何类的抽象父类，例如Hashtable，它将键映射到值。





[DoubleSummaryStatistics](https://developer.android.google.cn/reference/java/util/DoubleSummaryStatistics.html)


统计数据的收集状态，如count，min，max，sum和average。





[EnumMap](https://developer.android.google.cn/reference/java/util/EnumMap.html)<K extends [Enum](https://developer.android.google.cn/reference/java/lang/Enum.html)<K>, V>


用于枚举类型键的专门的Map实现。





[EnumSet](https://developer.android.google.cn/reference/java/util/EnumSet.html)<E extends [Enum](https://developer.android.google.cn/reference/java/lang/Enum.html)<E>>


一个专门用于枚举类型的Set实现。





[EventListenerProxy](https://developer.android.google.cn/reference/java/util/EventListenerProxy.html)<T extends [EventListener](https://developer.android.google.cn/reference/java/util/EventListener.html)>


一个EventListener类的抽象包装类，它将一组附加参数与侦听器相关联。





[EventObject](https://developer.android.google.cn/reference/java/util/EventObject.html)


所有事件状态对象应从其派生的根类。





[FormattableFlags](https://developer.android.google.cn/reference/java/util/FormattableFlags.html)


FomattableFlags传递给Formattable.formatTo（）方法并修改Formattables的输出格式。





[Formatter](https://developer.android.google.cn/reference/java/util/Formatter.html)


printf风格格式字符串的解释器。





[GregorianCalendar](https://developer.android.google.cn/reference/java/util/GregorianCalendar.html)


GregorianCalendar是Calendar的具体子类，它提供了世界上大多数用户使用的标准日历系统。





[HashMap](https://developer.android.google.cn/reference/java/util/HashMap.html)<K, V>


基于哈希表的Map接口实现。





[HashSet](https://developer.android.google.cn/reference/java/util/HashSet.html)<E>


此类实现了Set接口，由哈希表（实际上是HashMap实例）支持。





[Hashtable](https://developer.android.google.cn/reference/java/util/Hashtable.html)<K, V>


该类实现了一个哈希表，它将键映射到值。





[IdentityHashMap](https://developer.android.google.cn/reference/java/util/IdentityHashMap.html)<K, V>


该类使用哈希表实现Map接口，在比较键（和值）时，使用引用相等代替对象相等。





[IntSummaryStatistics](https://developer.android.google.cn/reference/java/util/IntSummaryStatistics.html)


统计数据的收集状态，如count，min，max，sum和average。





[LinkedHashMap](https://developer.android.google.cn/reference/java/util/LinkedHashMap.html)<K, V>


哈希表和链接列表实现的Map界面，具有可预测的迭代顺序。





[LinkedHashSet](https://developer.android.google.cn/reference/java/util/LinkedHashSet.html)<E>


哈希表和链表实现的Set接口，具有可预测的迭代顺序。





[LinkedList](https://developer.android.google.cn/reference/java/util/LinkedList.html)<E>


List和Deque接口的双向链表实现。





[ListResourceBundle](https://developer.android.google.cn/reference/java/util/ListResourceBundle.html)


ListResourceBundle是ResourceBundle的一个抽象子类，它在方便易用的列表中管理区域设置的资源。





[Locale](https://developer.android.google.cn/reference/java/util/Locale.html)


区域对象表示特定的地理，政治或文化区域。





[Locale.Builder](https://developer.android.google.cn/reference/java/util/Locale.Builder.html)


Builder用于根据setter配置的值构建Locale的实例。





[Locale.LanguageRange](https://developer.android.google.cn/reference/java/util/Locale.LanguageRange.html)


该类表示在RFC
 4647“语言标签匹配”中定义的语言范围。





[LongSummaryStatistics](https://developer.android.google.cn/reference/java/util/LongSummaryStatistics.html)


统计数据的收集状态，如count，min，max，sum和average。





[Objects](https://developer.android.google.cn/reference/java/util/Objects.html)


该类由用于对对象进行操作的静态实用程序组成。





[Observable](https://developer.android.google.cn/reference/java/util/Observable.html)


此类表示可观察对象或模型视图范例中的“数据”。





[Optional](https://developer.android.google.cn/reference/java/util/Optional.html)<T>


可能包含或不包含非空值的容器对象。





[OptionalDouble](https://developer.android.google.cn/reference/java/util/OptionalDouble.html)


可能含有或不包含双重值的容器对象。





[OptionalInt](https://developer.android.google.cn/reference/java/util/OptionalInt.html)


可能包含或不包含int值的容器对象。





[OptionalLong](https://developer.android.google.cn/reference/java/util/OptionalLong.html)


可能含有也可能不包含长值的容器对象。





[PriorityQueue](https://developer.android.google.cn/reference/java/util/PriorityQueue.html)<E>


基于优先级堆的无界优先级队列。





[Properties](https://developer.android.google.cn/reference/java/util/Properties.html)


Properties类表示一组持久的属性。





[PropertyPermission](https://developer.android.google.cn/reference/java/util/PropertyPermission.html)


旧版安全码; 不使用。





[PropertyResourceBundle](https://developer.android.google.cn/reference/java/util/PropertyResourceBundle.html)


PropertyResourceBundle是ResourceBundle的一个具体子类，它使用属性文件中的一组静态字符串管理区域设置的资源。





[Random](https://developer.android.google.cn/reference/java/util/Random.html)


该类的实例用于生成伪随机数的流。





[ResourceBundle](https://developer.android.google.cn/reference/java/util/ResourceBundle.html)


资源束包含区域特定的对象。





[ResourceBundle.Control](https://developer.android.google.cn/reference/java/util/ResourceBundle.Control.html)


ResourceBundle.Control定义了一个在bundle加载过程中由ResourceBundle.getBundle工厂方法调用的回调方法。





[Scanner](https://developer.android.google.cn/reference/java/util/Scanner.html)


一个简单的文本扫描器，可以使用正则表达式解析原始类型和字符串。





[ServiceLoader](https://developer.android.google.cn/reference/java/util/ServiceLoader.html)<S>


一个简单的服务提供商加载工具。





[SimpleTimeZone](https://developer.android.google.cn/reference/java/util/SimpleTimeZone.html)


SimpleTimeZone是TimeZone的一个具体子类，代表了一个用于公历的时区。





[Spliterators](https://developer.android.google.cn/reference/java/util/Spliterators.html)


用于操作或创建Spliterator及其原始专业化实例的静态类和方法Spliterator.OfInt，Spliterator.OfLong和Spliterator.OfDouble。





[Spliterators.AbstractDoubleSpliterator](https://developer.android.google.cn/reference/java/util/Spliterators.AbstractDoubleSpliterator.html)


抽象的Spliterator.Of双重实现try拆分以允许有限的并行性。





[Spliterators.AbstractIntSpliterator](https://developer.android.google.cn/reference/java/util/Spliterators.AbstractIntSpliterator.html)


一个抽象的Spliterator.OfInt实现trySplit以允许有限的并行性。





[Spliterators.AbstractLongSpliterator](https://developer.android.google.cn/reference/java/util/Spliterators.AbstractLongSpliterator.html)


一个抽象的Spliterator.Of Long实现try拆分以允许有限的并行性。





[Spliterators.AbstractSpliterator](https://developer.android.google.cn/reference/java/util/Spliterators.AbstractSpliterator.html)<T>


一个抽象的Spliterator，它实现了trySplit以允许有限的并行性。





[SplittableRandom](https://developer.android.google.cn/reference/java/util/SplittableRandom.html)


 适用于（在其他上下文中）使用可能产生子任务的孤立并行计算的均匀伪随机值的生成器。





[Stack](https://developer.android.google.cn/reference/java/util/Stack.html)<E>


Stack类代表一个先进先出（LIFO）的对象堆栈。





[StringJoiner](https://developer.android.google.cn/reference/java/util/StringJoiner.html)


StringJoiner用于构造由分隔符分隔的字符序列，并且可选地从提供的前缀开始，并以提供的后缀结尾。





[StringTokenizer](https://developer.android.google.cn/reference/java/util/StringTokenizer.html)


字符串tokenizer类允许应用程序将字符串拆分成令牌。





[Timer](https://developer.android.google.cn/reference/java/util/Timer.html)


线程调度任务以供将来在后台线程中执行的功能。





[TimerTask](https://developer.android.google.cn/reference/java/util/TimerTask.html)


可以由计时器进行一次性或重复执行的任务。





[TimeZone](https://developer.android.google.cn/reference/java/util/TimeZone.html)


TimeZone代表时区偏移，也可以计算出夏令时。





[TreeMap](https://developer.android.google.cn/reference/java/util/TreeMap.html)<K, V>


基于Red-Black树的NavigableMap实现。





[TreeSet](https://developer.android.google.cn/reference/java/util/TreeSet.html)<E>


基于TreeMap的NavigableSet实现。





[UUID](https://developer.android.google.cn/reference/java/util/UUID.html)


一个表示不可变的通用唯一标识符（UUID）的类。





[Vector](https://developer.android.google.cn/reference/java/util/Vector.html)<E>


Vector类实现可扩展的对象数组。





[WeakHashMap](https://developer.android.google.cn/reference/java/util/WeakHashMap.html)<K, V>


基于哈希表的实现Map界面，具有弱键。








**枚举**

[Formatter.BigDecimalLayoutForm](https://developer.android.google.cn/reference/java/util/Formatter.BigDecimalLayoutForm.html)


枚举为BigDecimal格式。





[Locale.Category](https://developer.android.google.cn/reference/java/util/Locale.Category.html)


枚举地区类别。





[Locale.FilteringMode](https://developer.android.google.cn/reference/java/util/Locale.FilteringMode.html)


此枚举提供常量来选择区域设置匹配的过滤模式。








**异常**

[ConcurrentModificationException](https://developer.android.google.cn/reference/java/util/ConcurrentModificationException.html)


当不允许这样的修改时，可以通过检测到对象的并发修改的方法来抛出此异常。





[DuplicateFormatFlagsException](https://developer.android.google.cn/reference/java/util/DuplicateFormatFlagsException.html)


在格式说明符中提供重复标志时抛出未经检查的异常。





[EmptyStackException](https://developer.android.google.cn/reference/java/util/EmptyStackException.html)


通过Stack类中的方法抛出，表示堆栈是空的。





[FormatFlagsConversionMismatchException](https://developer.android.google.cn/reference/java/util/FormatFlagsConversionMismatchException.html)


转换和标志不兼容时抛出未经检查的异常。





[FormatterClosedException](https://developer.android.google.cn/reference/java/util/FormatterClosedException.html)


格式化程序已关闭时抛出未检查的异常。





[IllegalFormatCodePointException](https://developer.android.google.cn/reference/java/util/IllegalFormatCodePointException.html)


当一个具有无效Unicode代码点的字符由isValidCodePoint（int）定义的字符被传递到Formatter时抛出未检查的异常。





[IllegalFormatConversionException](https://developer.android.google.cn/reference/java/util/IllegalFormatConversionException.html)


当对应于格式说明符的参数是不兼容的类型时抛出未检查的异常。





[IllegalFormatException](https://developer.android.google.cn/reference/java/util/IllegalFormatException.html)


当格式字符串包含与给定参数不兼容的非法语法或格式说明符时抛出未检查的异常。





[IllegalFormatFlagsException](https://developer.android.google.cn/reference/java/util/IllegalFormatFlagsException.html)


提供非法组合标志时抛出未检查的异常。





[IllegalFormatPrecisionException](https://developer.android.google.cn/reference/java/util/IllegalFormatPrecisionException.html)


当精度为-1以外的负值时，抛出未检查的异常，转换不支持精度，否则不支持该值。





[IllegalFormatWidthException](https://developer.android.google.cn/reference/java/util/IllegalFormatWidthException.html)


格式宽度为-1以外的负值时，将抛出未检查的异常，否则不受支持。





[IllformedLocaleException](https://developer.android.google.cn/reference/java/util/IllformedLocaleException.html)


由Locale和Locale.Builder中的方法抛出，表示一个参数不是一个格式正确的BCP 47标签。





[InputMismatchException](https://developer.android.google.cn/reference/java/util/InputMismatchException.html)


由扫描仪抛出以指示所检索的令牌与预期类型的模式不匹配，或令牌超出预期类型的范围。





[InvalidPropertiesFormatException](https://developer.android.google.cn/reference/java/util/InvalidPropertiesFormatException.html)


抛出以表示操作无法完成，因为输入不符合属性集合的适当的XML文档类型。





[MissingFormatArgumentException](https://developer.android.google.cn/reference/java/util/MissingFormatArgumentException.html)


当有格式说明符没有相应的参数或参数索引引用不存在的参数时抛出未检查的异常。





[MissingFormatWidthException](https://developer.android.google.cn/reference/java/util/MissingFormatWidthException.html)


需要格式宽度时抛出未检查的异常。





[MissingResourceException](https://developer.android.google.cn/reference/java/util/MissingResourceException.html)


表示资源丢失。





[NoSuchElementException](https://developer.android.google.cn/reference/java/util/NoSuchElementException.html)


被各种访问器方法抛出，表示被请求的元素不存在。





[TooManyListenersException](https://developer.android.google.cn/reference/java/util/TooManyListenersException.html)


TooManyListenersException异常用作Java事件模型的一部分，用于注释和实现组播事件源的单播特殊情况。





[UnknownFormatConversionException](https://developer.android.google.cn/reference/java/util/UnknownFormatConversionException.html)


给出未知转换时抛出未检查的异常。





[UnknownFormatFlagsException](https://developer.android.google.cn/reference/java/util/UnknownFormatFlagsException.html)


给定未知标志时抛出未检查的异常。








**错误**

[ServiceConfigurationError](https://developer.android.google.cn/reference/java/util/ServiceConfigurationError.html)


加载服务提供商时发生错误时抛出错误。




