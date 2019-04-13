
# java入门教程-9.4Java日期和时间类简介 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:45:31[seven-soft](https://me.csdn.net/softn)阅读数：205


Java 的日期和时间类位于 java.util 包中。利用日期时间类提供的方法，可以获取当前的日期和时间，创建日期和时间参数，计算和比较时间。
## Date 类
Date 类是 Java 中的日期时间类，其构造方法比较多，下面是常用的两个：Date()：使用当前的日期和时间初始化一个对象。
Date(long millisec)：从1970年01月01日00时（格林威治时间）开始以毫秒计算时间，计算 millisec 毫秒。如果运行 Java 程序的本地时区是北京时区（与格林威治时间相差 8 小时），Date dt1=new Date(1000);，那么对象 dt1 就是1970年01月01日08时00分01秒。
请看一个显示日期时间的例子：importjava.util.Date;
publicclassDemo{
publicstaticvoidmain(Stringargs[]){
Dateda=newDate();//创建时间对象
System.out.println(da);//显示时间和日期
longmsec=da.getTime();
System.out.println("从1970年1月1日0时到现在共有："+msec+"毫秒");
}
}

运行结果：
Mon Feb 05 22:50:05 CST 2007
从1970年1月1日0时到现在共有：1170687005390 毫秒
一些比较常用的 Date 类方法：
|方法|功能|
|boolean after(Date date)|若调用 Date 对象所包含的日期比 date 指定的对象所包含的日期晚，返回 true，否则返回 false。|
|boolean before(Date date)|若调用 Date 对象所包含的日期比 date 指定的对象所包含的日期早，返回 true，否则返回 false。|
|Object clone()|复制调用 Date 对象。|
|int compareTo(Date date)|比较调用对象所包含的日期和指定的对象包含的日期，若相等返回 0；若前者比后者早，返回负值；否则返回正值。|
|long getTime()|以毫秒数返回从 1970 年 01 月 01 日 00 时到目前的时间。|
|int hashCode()|返回调用对象的散列值。|
|void setTime(long time)|根据 time 的值，设置时间和日期。time 值从 1970 年 01 月 01 日 00 时开始计算。|
|String toString()|把调用的 Date 对象转换成字符串并返回结果。|
|public Static String valueOf(type variable)|把 variable 转换为字符串。|
Date 对象表示时间的默认顺序是星期、月、日、小时、分、秒、年。若需要修改时间显示的格式可以使用“SimpleDateFormat(String pattern)”方法。
例如，用不同的格式输出时间：importjava.util.Date;
importjava.text.SimpleDateFormat;
publicclassDemo{
publicstaticvoidmain(Stringargs[]){
Dateda=newDate();
System.out.println(da);
SimpleDateFormatma1=newSimpleDateFormat("yyyy 年 MM 月 dd 日 E 北京时间");
System.out.println(ma1.format(da));
SimpleDateFormatma2=newSimpleDateFormat("北京时间：yyyy 年 MM 月 dd 日 HH 时 mm 分 ss 秒");
System.out.println(ma2.format(-1000));
}
}

运行结果：
Sun Jan 04 17:31:36 CST 2015
2015 年 01 月 04 日 星期日 北京时间
北京时间：1970 年 01 月 01 日 07 时 59 分 59 秒
## Calendar 类
抽象类 Calendar 提供了一组方法，允许把以毫秒为单位的时间转换成一些有用的时间组成部分。Calendar 不能直接创建对象，但可以使用静态方法 getInstance() 获得代表当前日期的日历对象，如：
Calendar calendar=Calendar.getInstance();
该对象可以调用下面的方法将日历翻到指定的一个时间：voidset(intyear,intmonth,intdate);
voidset(intyear,intmonth,intdate,inthour,intminute);
voidset(intyear,intmonth,intdate,inthour,intminute,intsecond);

若要调用有关年份、月份、小时、星期等信息，可以通过调用下面的方法实现：
int get(int field);
其中，参数 field 的值由 Calendar 类的静态常量决定。其中：YEAR 代表年，MONTH 代表月，HOUR 代表小时，MINUTE 代表分，如：
calendar.get(Calendar.MONTH);
如果返回值为 0 代表当前日历是一月份，如果返回 1 代表二月份，依此类推。
由 Calendar 定义的一些常用方法如下表所示：
|方法|功能|
|abstract void add(int which,int val)|将 val 加到 which 所指定的时间或者日期中，如果需要实现减的功能，可以加一个负数。which 必须是 Calendar 类定义的字段之一，如 Calendar.HOUR|
|boolean after(Object calendarObj)|如果调用 Calendar 对象所包含的日期比 calendarObj 指定的对象所包含的日期晚，返回 true，否则返回 false|
|boolean before(Object calendarObj)|如果调用 Calendar 对象所包含的日期比 calendarObj 指定的对象所包含的日期早，返回 true，否则返回 false|
|final void clear()|对调用对象包含的所有时间组成部分清零|
|final void clear(int which)|对调用对象包含的 which 所指定的时间组成部分清零|
|boolean equals(Object calendarObj)|如果调用 Calendar 对象所包含的日期和 calendarObj 指定的对象所包含的日期相等，返回 true，否则返回 false|
|int get(int calendarField)|返回调用 Calendar 对象的一个时间组成部分的值，这个组成部分由 calendarField指定，可以被返回的组成部分如：Calendar.YEAR，Calendar.MONTH 等|
|static Calendar getInstance()|返回使用默认地域和时区的一个 Calendar 对象|
|final Date getTime()|返回一个和调用对象时间相等的 Date 对象|
|final boolean isSet(int which)|如果调用对象所包含的 which 指定的时间部分被设置了，返回 true，否则返回 false|
|final void set(int year,int month)|设置调用对象的各种日期和时间部分|
|final void setTime(Date d)|从 Date 对象 d 中获得日期和时间部分|
|void setTimeZone(TimeZone t)|设置调用对象的时区为 t 指定的那个时区|
## GregorianCalendar 类
GregorianCalendar 是一个具体实现 Calendar 类的类，该类实现了公历日历。Calendar 类的 getInstance() 方法返回一个 GregorianCalendar，它被初始化为默认的地域和时区下的当前日期和时间。
GregorianCalendar 类定义了两个字段：AD 和 BC，分别代表公元前和公元后。其默认的构造方法 GregorianCalendar() 以默认的地域和时区的当前日期和时间初始化对象，另外也可以指定地域和时区来建立一个
 GregorianCalendar 对象，例如：GregorianCalendar(Localelocale);
GregorianCalendar(TimeZonetimeZone);
GregorianCalendar(TimeZonetimeZone,Localelocale);

GregorianCalendar 类提供了 Calendar 类中所有的抽象方法的实现，同时还提供了一些附加的方法，其中用来判断闰年的方法为：
Boolean isLeapYear(int year);
如果 year 是闰年，该方法返回 true，否则返回 false。

