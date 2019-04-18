# java处理日期时间 相加减 - z69183787的专栏 - CSDN博客
2013年12月03日 15:10:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5196
JAVA处理日期时间常用方法： 
1.java.util.Calendar 
Calendar 类是一个抽象类，它为特定瞬间与一组诸如 YEAR、MONTH、DAY_OF_MONTH、HOUR 等 日历字段之间的转换提供了一些方法，并为操作日历字段（例如获得下星期的日期）提供了一些方法。瞬间可用毫秒值来表示，它是距历元（即格林威治标准时间 1970 年 1 月 1 日的 00:00:00.000，格里高利历）的偏移量。 
例: 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- Calendar cal = Calendar.getInstance();//使用默认时区和语言环境获得一个日历。 
- cal.add(Calendar.DAY_OF_MONTH, -1);//取当前日期的前一天. 
- 
- cal.add(Calendar.DAY_OF_MONTH, +1);//取当前日期的后一天. 
- 
- //通过格式化输出日期 
- java.text.SimpleDateFormat format = new java.text.SimpleDateFormat("yyyy-MM-dd");   
- 
- System.out.println("Today is:"+format.format(Calendar.getInstance().getTime()));   
- 
- System.out.println("yesterday is:"+format.format(cal.getTime()));   
得到2007-12-25日期: 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- Calendar calendar = new GregorianCalendar(2007, 11, 25,0,0,0);   
- Date date = calendar.getTime();   
- System.out.println("2007 Christmas is:"+format.format(date));   
java月份是从0-11,月份设置时要减1. 
GregorianCalendar构造方法参数依次为：年，月-1，日，时，分，秒. 
取日期的部分: 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- int year =calendar.get(Calendar.YEAR);   
- 
- int month=calendar.get(Calendar.MONTH)+1;   
- 
- int day =calendar.get(Calendar.DAY_OF_MONTH);   
- 
- int hour =calendar.get(Calendar.HOUR_OF_DAY);   
- 
- int minute =calendar.get(Calendar.MINUTE);   
- 
- int seconds =calendar.get(Calendar.SECOND);   
取月份要加1. 
判断当前月份的最大天数: 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- Calendar cal = Calendar.getInstance();   
- int day=cal.getActualMaximum(Calendar.DAY_OF_MONTH);   
- System.out.println(day);   
2.java.util.Date 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- java.util.Date today=new java.util.Date();   
- System.out.println("Today is "+formats.format(today));   
取当月的第一天: 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- java.text.SimpleDateFormat format = new java.text.SimpleDateFormat("yyyy-MM-01");   
- java.util.Date firstDay=new java.util.Date();   
- System.out.println("the month first day is "+formats.format(firstDay));   
取当月的最后一天: 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- 
- Calendar cal = Calendar.getInstance();   
- int maxDay=cals.getActualMaximum(Calendar.DAY_OF_MONTH);   
- java.text.Format formatter3=new java.text.SimpleDateFormat("yyyy-MM-"+maxDay);   
- System.out.println(formatter3.format(cal.getTime()));   
求两个日期之间相隔的天数: 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- java.text.SimpleDateFormat format = new java.text.SimpleDateFormat("yyyy-MM-dd");   
- java.util.Date beginDate= format.parse("2007-12-24");   
- java.util.Date endDate= format.parse("2007-12-25");   
- long day=(date.getTime()-mydate.getTime())/(24*60*60*1000);   
- System.out.println("相隔的天数="+day);   
一年前的日期: 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- java.text.Format formatter=new java.text.SimpleDateFormat("yyyy-MM-dd");   
- java.util.Date todayDate=new java.util.Date();   
- long beforeTime=(todayDate.getTime()/1000)-60*60*24*365;   
- todayDate.setTime(beforeTime*1000);   
- String beforeDate=formatter.format(todayDate);   
- System.out.println(beforeDate);   
一年后的日期: 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- java.text.Format formatter=new java.text.SimpleDateFormat("yyyy-MM-dd");   
- java.util.Date todayDate=new java.util.Date();   
- long afterTime=(todayDate.getTime()/1000)+60*60*24*365;   
- todayDate.setTime(afterTime*1000);   
- String afterDate=formatter.format(todayDate);   
- System.out.println(afterDate);   
求10小时后的时间 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- java.util.Calendar Cal=java.util.Calendar.getInstance();   
- Cal.setTime(dateOper);   
- Cal.add(java.util.Calendar.HOUR_OF_DAY,10);   
- System.out.println("date:"+forma.format(Cal.getTime()));   
求10小时前的时间 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- java.util.Calendar Cal=java.util.Calendar.getInstance();   
- Cal.setTime(dateOper);   
- Cal.add(java.util.Calendar.HOUR_OF_DAY,-10);   
- System.out.println("date:"+forma.format(Cal.getTime()));   
3.java.sql.Date 
继承自java.util.Date,是操作数据库用的日期类型 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- java.sql.Date sqlDate = new java.sql.Date(java.sql.Date.valueOf("2007-12-25").getTime());   
日期比较:简单的比较可以以字符串的形式直接比较,也可使用 
java.sql.Date.valueOf("2007-03-08").compareTo(java.sql.Date.valueOf("2007-03-18"))方式来比较日期的大小.也可使用java.util.Date.after(java.util.Date)来比较. 
相差时间： 
long difference=c2.getTimeInMillis()-c1.getTimeInMillis(); 
相差天数：long day=difference/(3600*24*1000) 
相差小时：long hour=difference/(3600*1000) 
相差分钟：long minute=difference/(60*1000) 
相差秒： long second=difference/1000 
补充： 
Java代码  ![收藏代码](http://alexfc.iteye.com/images/icon_star.png)
- DateFormat df=new SimpleDateFormat("yyyy-MM-dd EE hh:mm:ss");   
- System.out.println(df.format(new Date()));   
- Date date = new Date();   
- DateFormat shortDate=DateFormat.getDateTimeInstance(DateFormat.SHORT, DateFormat.SHORT);   
- DateFormat mediumDate =DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM);   
- DateFormat longDate =DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.LONG);   
- DateFormat fullDate =DateFormat.getDateTimeInstance(DateFormat.FULL, DateFormat.FULL);   
- 
- system.out.println(shortDate.format(date));   
- System.out.println(mediumDate.format(date));   
- System.out.println(longDate.format(date));   
- System.out.println(fullDate.format(date));   
- 
- 08-4-15 下午3:24
- 2008-4-1515:24:31
- 2008年4月15日 下午03时24分31秒   
- 2008年4月15日 星期二 下午03时24分31秒CST   
- 
- 
- Calendar c = Calendar.getInstance();   
- 
- c.add(Calendar.MONTH, 1); // 目前時間加1個月 
- System.out.println(df.format(c.getTime()));   
- 
- c.add(Calendar.HOUR, 3); // 目前時間加3小時 
- System.out.println(df.format(c.getTime()));   
- 
- c.add(Calendar.YEAR, -2); // 目前時間減2年 
- System.out.println(df.format(c.getTime()));   
- 
- c.add(Calendar.DAY_OF_WEEK, 7); // 目前的時間加7天 
- System.out.println(df.format(c.getTime()));   
