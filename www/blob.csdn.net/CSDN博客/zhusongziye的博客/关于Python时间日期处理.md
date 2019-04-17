# 关于Python时间日期处理 - zhusongziye的博客 - CSDN博客





2018年11月07日 21:27:19[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：64








本文介绍的关于Python时间日期处理，日期时间处理在实际应用场景中无处不在，所以这也成了编程语言中必不可少的模块，Python 也不例外。但是，你知道在Python中有多少个相关的模块吗？datetime、time、calendar、 dateutil、 pytz 等等。 你知道有多少种数据类型吗？date、time、datetime、tzinfo、timedelta 等等。

有天我遇到这样的需求，想获取当前月的前一个月是几月，假设本月是2018年1月，那么上个月就是2017年12月，大约要经过这么几个步骤

```
>>> import datetime
 # 1. 获取「今天」
 >>> today = datetime.date.today()
 # 2. 获取当前月的第一天
 >>> first = today.replace(day=1)
 # 3. 减一天，得到上个月的最后一天
 >>> last_month = first - datetime.timedelta(days=1)
 # 4. 格式化成指定形式
 >>> print(last_month.strftime("%Y%m"))
 201807
 >>>
```

这是有多麻烦？ 麻烦得你怀疑这是 Python 代码？ 可能有人会说，用 datetime.replace 方法将 month-1 就好了，咋看起来没问题，实际上这是有 bug 的，month 的范围只能是 1-12

```
>>> d.replace(month=d.month-1)
datetime.datetime(2018, 7, 24, 11, 29, 28, 929830)
```

month-11 就报错了

```
>>> d.replace(month=d.month-11)
ValueError: month must be in 1..12
```

你还知道日期时间、时间戳、字符串之间的互相转换的那些 API 方法吗？是不是每次处理时间相关的操作时，总要去官方文档翻看一遍才能动手。你可以看看 time 模块和 datetime 模块中各种类型之间的互相转换，看着这些箭头是不是有密集恐惧症？

![](https://img-blog.csdnimg.cn/20181107212624718.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



不管怎样，你终究还是要熟练这些模块和API操作的，记不住没关系，至少你都要手动敲几遍，下次遇到时要能做到翻看文档能快速定位到某个类某个方法是做什么用、怎么用。

但今天我要强烈安利给你的这个时间日期库：Arrow。它的灵感来自于 requests 库。将所有繁杂的东西全部隐藏于身后，留给你的是 for humans 接口。充分诠释了 keep it simple and stupid 这句话的含义。

这篇文章带你初步了解并掌握 Arrow 的使用方式。

### 安装
`$ pip install arrow`
### 使用

```
>>> a = arrow.now() # 当前本地时间
>>> a
<Arrow [2018-08-24T07:09:03.468562+08:00]>

>>> arrow.utcnow() # 当前utc时间
<Arrow [2018-08-23T23:11:50.147585+00:00]>
```

你可以认为** Arrow 对象是一个增强版的 datetime 对象**。

通过 Arrow 对象你可以获取 datetime 对象

```
>>> t = a.datetime
>>> type(t)
<class 'datetime.datetime'>
>>> t
datetime.datetime(2018, 8, 24, 7, 17, 14, 884750, tzinfo=tzlocal())
```

通过 Arrow 对象你可以得到时间戳

```
>>> a.timestamp
1535066234
```

获取 arrow 对象的年、月、日、时、分、秒

```
>>> a.year
2018
>>> a.month
8
>>> a.day
24
>>> a.hour
7
```

获取 arrow 对象的时间和日期

```
>>> a.date()
datetime.date(2018, 8, 24)
>>> a.time()
datetime.time(7, 9, 3, 468562)
```

注意，获取时间和日期是用方法，而获取 datetime 和 timestamp 是两个属性

接下来介绍一些 arrow 有意思的方法

### shift

shift 有点像游标卡尺，可以左右两边进行加减移位操作，加减的对象可以是年月日时分秒和星期。再回到文章开始地方，想获取当前月的前一个月，你可以这样写：

```
>>> a.shift(months=-1)
<Arrow [2018-07-24T07:09:03.468562+08:00]>

>>> a.shift(months=-1).format("YYYYMM")
'201807'
>>>
```

指定参数 months = -1 就可以了。往后一个月就是 month=+1， 加号可以省略。这样你可以基于一个 arrow 时间对象进行任意的往前加或者往后减。 注意 month 后面有个s， year 同理。 以下是一些例子。

加一个月

```
>>> a.shift(months=1)
<Arrow [2018-09-24T07:09:03.468562+08:00]>
```

减一个月

```
>>> a.shift(months=-1)
<Arrow [2018-07-24T07:09:03.468562+08:00]>
```

减两年

```
>>> a.shift(years=-2)
<Arrow [2016-08-24T07:09:03.468562+08:00]>
>>>
```

加一个小时

```
>>> a.shift(hours=1)
<Arrow [2018-08-24T08:09:03.468562+08:00]>
```

还可以按周进行加减

```
>>> a.shift(weeks=1)
<Arrow [2018-08-31T07:09:03.468562+08:00]>
```

如果你要明确指定修改成哪年或者哪月，那么使用 replace 方法即可，repalce 在 datetime 对象中也有该方法，两者的使用方式是一样的。

### humanize

humanize 方法是相对于当前时刻表示为“多久以前”的一种可读行字符串形式，默认是英文格式，指定 locale 可显示相应的语言格式。

```
>>> a.humanize()
'6 hours ago'
>>> a.humanize(locale='zh')
'6小时前'
```

### format

format 是格式化工具，可以根据指定的格式将 arrow 对象转换成字符串格式，格式Token请参考下图

```
>>> a.format()
'2018-08-24 07:09:03+08:00'
>>> a.format("YYYY-MM-DD HH:mm:ss")
'2018-08-24 07:09:03'
```

![](https://img-blog.csdnimg.cn/20181107212712157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

### to

to 可以将一个本地时区转换成其它任意时区，例如：

```
>>> arrow.now()
<Arrow [2018-08-24T16:58:50.990657+08:00]>
>>> arrow.now().to("utc")
<Arrow [2018-08-24T08:59:04.316289+00:00]>
>>> arrow.now().to("utc").to("local")
<Arrow [2018-08-24T16:59:15.800847+08:00]>
>>> arrow.now().to("America/New_York")
<Arrow [2018-08-24T04:59:34.037182-04:00]>
```

### 构建 Arrow 对象

前面介绍了 arrow 可以转换成datetime、str、date、time、timestamp，那么如何构建 Arrow 对象呢？除了使用 now()、utcnow() 方法之后，你还可以使用 get 工厂方法，或者使用 Arrow 构造方法直接指定年月日时分秒

```
>>> arrow.Arrow(2018, 8, 24, 12, 30, 45)
<Arrow [2018-08-24T12:30:45+00:00]>
>>> arrow.Arrow(2018, 8, 24, 12, 30, 45, tzinfo='utc')
<Arrow [2018-08-24T12:30:45+00:00]>
>>> arrow.Arrow(2018, 8, 24, 12, 30, 45, tzinfo='local')
<Arrow [2018-08-24T12:30:45+08:00]>
```

### get

第二种方式是用get方法来创建 arrow 对象，get 方法”非常灵活”，直接看例子，跟着敲

```
# 不带参数，等价与 utcnow()
>>> arrow.get()
<Arrow [2018-08-24T07:11:50.528742+00:00]>

# 接受时间戳参数
>>> arrow.get(1535113845)

# 接受一个datetime对象
>>> arrow.get(datetime(2018,8,24))
<Arrow [2018-08-24T00:00:00+00:00]>

# 接收一个date对象
>>> from datetime import date
>>> arrow.get(date(2018,7,24))
<Arrow [2018-07-24T00:00:00+00:00]>

# 接收日期格式的字符串
>>> arrow.get("2018-08-11 12:30:56")
<Arrow [2018-08-11T12:30:56+00:00]>

# 接收日期字符串，并指定格式
>>> arrow.get("18-08-11 12:30:56", "YY-MM-DD HH:mm:ss")
<Arrow [2018-08-11T12:30:56+00:00]>
```

### Arrow 的不足

关于 get 方法，看似强大，使用起来灵活，感觉什么参数都能接受，但是也带来了一些问题，甚至是 bug。比如

```
>>> arrow.get("2018-7-11")
<Arrow [2018-01-01T00:00:00+00:00]>
```

期望的值应该是 2018-07-11， 但是它并没有提示错误，而正确的做法是要指定格式，因为你传的字符串不是标准的日期格式。

```
>>> arrow.get("2018-7-11", "YYYY-M-DD")
<Arrow [2018-07-11T00:00:00+00:00]>
```

想通过一个方法来兼容n种情况是极度困难的，内部实现也会非常复杂，作为用户使用起来必然也很混乱。



