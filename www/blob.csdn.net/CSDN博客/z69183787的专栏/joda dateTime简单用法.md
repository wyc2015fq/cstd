# joda dateTime简单用法 - z69183787的专栏 - CSDN博客
2017年11月28日 14:20:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3100
我们在编写应用程序都需要处理时间问题。应用程序需要知道当前的时间点和下一个时间点，有时它们还必须计算这两个时间点之间的路径。使用 JDK 完成这项任务将非常痛苦和繁琐。joda-dateTime 可以解决
1、DateTime   
 
  它以毫秒级的精度封装时间上的某个瞬间时刻。`DateTime` 始终与 `DateTimeZone` 相关，如果您不指定它的话，它将被默认设置为运行代码的机器所在的时区。可以使用多种方式构造DateTime对象
[java][view
 plain](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)[copy](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)
- DateTime dt = new DateTime();  
- DateTime dt1 = DateTime.now();  
- DateTime dt2 = new DateTime(new Date());  
- DateTime dt6 = new DateTime(Calendar.getInstance());  
- // 指定年月日点分秒生成(参数依次是：年,月,日,时,分,秒,毫秒)
- DateTime dt3 = new DateTime(2012, 5, 20, 13, 14, 0, 0);  
- // 制定ISO8601生成
- DateTime dt4 = new DateTime("2014-08-01T12:32:3");  
- DateTime dt5 = new DateTime("2014-08-01");  
2、`LocalDate 该类封装了一个年/月/日的组合。当地理位置（即时区）变得不重要时，使用它存储日期将非常方便`
``
[java][view
 plain](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)[copy](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)
- LocalDate localDate = new LocalDate();//输出格式 yyyy-MM-dd
- LocalDate localDate2 = new LocalDate(new Date());  
- LocalDate localDate3 = new LocalDate(2014,8,1);  
3、`LocalTime 这个类封装一天中的某个时间，当地理位置不重要的情况下，可以使用这个类来只存储一天当中的某个时间`
````
[java][view
 plain](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)[copy](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)
- LocalTime localTime = new LocalTime();// HH:mm:sss.a
- LocalTime localTime2 = new LocalTime(new Date());  
4、DateTime 日期获取年月日时分
``  ``
[java][view
 plain](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)[copy](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)
- DateTime dte = DateTime.now();  
- int year = dte.getYear(); // 2014
- year = dte.getYearOfCentury(); // 14
- year = dte.getYearOfEra(); // 2014
- int month = dte.getMonthOfYear();  
- int day = dte.getDayOfMonth();  
- day = dte.getDayOfWeek(); // 星期几
- day = dte.getDayOfYear();// 一年的第多少天
- int hours = dte.getHourOfDay();  
- int mills = dte.getMinuteOfHour();  
- int second = dte.getSecondOfMinute();  
   5、与jdk转换
[java][view
 plain](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)[copy](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)
- DateTime dtt = new DateTime();  
- // 转换成java.util.Date对象
-    Date d1 = new Date(dt.getMillis());  
-    Date d2 = dt.toDate();  
- // 转换成java.util.Calendar对象
-    Calendar c1 = Calendar.getInstance();  
-    c1.setTimeInMillis(dt.getMillis());  
-    Calendar c2 = dt.toCalendar(Locale.getDefault());  
6、日期增加减少
[java][view
 plain](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)[copy](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)
- DateTime dtx = DateTime.now();  
-      dtx.plusDays(1);  
-      dtx.plusMonths(1);  
-      dtx.plusYears(1);// 增加1 day,1 months,1 year
-      dtx.minusDays(1);  
-      dtx.minusMonths(1);  
-      dtx.minusYears(1); // 减少1day，1月，1年
-      DateTime dtxw = dtx.dayOfMonth().withMaximumValue(); // 月末日期
-      DateTime dxys = dtx.plus(58).dayOfWeek().withMinimumValue(); // 增加58后的哪周周一的日期
-      DateTime dateTime = DateTime.now().withHourOfDay(12).withMinuteOfHour(00); // 当天12:00的日期
7、日期比较
[java][view
 plain](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)[copy](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)
- DateTime dateTime2 = new DateTime("2014-09-03");  
-    DateTime dateTime3 = new DateTime("2014-08-01");  
-    dateTime2.isAfter(dateTime3); // 后
-    dateTime2.isBefore(dateTime3); // 前
-    dateTime2.isEqual(dateTime3); // 等
-    System.out.println(dateTime3.isEqual(new DateTime())); // 和系统时间比较 false
- // 和系统时间比较
-    dateTime3.isAfterNow();  
-    dateTime3.isEqualNow();  
-    System.out.println(dateTime3.isBeforeNow()); // true
8、计算区间
[java][view
 plain](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)[copy](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)
- DateTime startDateTime = new DateTime("2014-08-01");  
-     DateTime endDateTime = new DateTime("2014-08-23");  
-     Duration d = new Duration(startDateTime, endDateTime);  
- long time = d.getMillis(); // 毫秒
-     Period p = new Period(startDateTime, endDateTime, PeriodType.days());  
- int days = p.getDays(); // 天数
-     Days.daysBetween(startDateTime, endDateTime).getDays();// 相差天数
-     Interval i = new Interval(startDateTime, endDateTime);  
- boolean contained = i.contains(new DateTime("2014-08-03"));// 验证某一天是否在该区间内
9、格式化
[java][view
 plain](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)[copy](http://blog.csdn.net/xiaozhao3972/article/details/38342813#)
- DateTime dateTimew = new DateTime();  
- // 转成字符串
-       dateTimew.toString("yyyy-MM-dd");  
-       dateTimew.toString("yyyy/MM/dd hh:mm:ss.SSSa");  
-       dateTimew.toString("yyyy-MM-dd HH:mm:ss");  
-       dateTimew.toString("EEEE dd MMMM, yyyy HH:mm:ssa");  
-       dateTimew.toString("yyyy/MM/dd HH:mm ZZZZ");  
-       dateTimew.toString("yyyy/MM/dd HH:mm Z");  
-       DateTimeFormatter formatter = DateTimeFormat.forPattern("yyyy-MM-dd");  
-       dateTimew.toString(formatter);  
- 
