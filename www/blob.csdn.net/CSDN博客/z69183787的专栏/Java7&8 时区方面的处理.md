# Java7&8 时区方面的处理 - z69183787的专栏 - CSDN博客
2019年02月12日 21:05:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：61

Java7：
```java
Calendar cd = Calendar.getInstance();  
 SimpleDateFormat sdf = new SimpleDateFormat("EEE d MMM yyyy HH:mm:ss 'GMT'", Locale.US);  
 sdf.setTimeZone(TimeZone.getTimeZone("GMT")); // 设置时区为GMT  
    String str = sdf.format(cd.getTime());  
    System.out.println(str);
```
Java8：
Java 8不仅分离了日期和时间，也把时区分离出来了。现在有一系列单独的类如ZoneId来处理特定时区，ZoneDateTime类来表示某时区下的时间。这在Java 8以前都是 GregorianCalendar类来做的。
```java
LocalDateTime.now(ZoneId.of("GMT")).
                format(DateTimeFormatter.ofPattern("MM/dd/yyyy hh:mm:ss a", Locale.US))
```
```java
// Date and time with timezone in Java 8
ZoneId america = ZoneId.of("America/New_York");
LocalDateTime localtDateAndTime = LocalDateTime.now();
ZonedDateTime dateAndTimeInNewYork  = ZonedDateTime.of(localtDateAndTime, america );
System.out.println("Current date and time in a particular timezone : " + dateAndTimeInNewYork);
 
Output :
Current date and time in a particular timezone : 2014-01-14T16:33:33.373-05:00[America/New_York]
```
时区信息字段参考：[https://www.cnblogs.com/softidea/p/8612287.html](https://www.cnblogs.com/softidea/p/8612287.html)
