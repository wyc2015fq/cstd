# 【java】LocalDate与LocalTime - Big Smile - CSDN博客
2018年03月09日 22:33:04[王啸tr1912](https://me.csdn.net/tr1912)阅读数：783
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        说道java中的时间格式，我们肯定都不陌生了，因为写日期或者时间的时候我们总是会new Date()来获得现在的时间，然后可以在此基础之上进行操作，但是有的时候涉及到纯时间或者日期的比较，计算就会非常的麻烦，这个时候要如何去做呢？
        一般的办法就是引入第三方的库joda-time，等常用的库，但是随着生产需求的不断变化，总是觉得有一些不妥的地方，所以java官方重新添加了新的时间操作工具库，并且在java8中就已经更新了，我们项目中常用的就是LocalDate和LocalTime了。
# 一、使用方法
        那我们如果想要获得现在的日期（不带时间）要怎么弄呢？
用dateformat的方法
```java
Date nowDate=new Date();//获取现在的时间
SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd");
String nowsdate=sdf.format(nowDate);
```
这样就可以获得到日期字符串了。
用LocalDate的话会更加简单：
```java
LocalDate nowDate = LocalDate.now();
```
只需要这一句话就可以获得现在的日期了，然后toString就可以转化为字符串格式了。
计算天数，或者比较日期区间：
```java
//计算时间差
LocalDate startDate = LocalDate.now();
LocalDate  endDate= LocalDate.parse("2018-03-01");			
long days = startDate.until(endDate,ChronoUnit.DAYS);//-，  数是已经到期  正数是还没到期
```
最后的days就是两个日期的间隔天数，需要注意的是开始时间如果比结束时间早，那么days就是整数，如果晚就是负数，如果相同的话days就是0。
LocalTime的话使用方法和LocalDate是同理的，在此不做过多描述了，这里放一段代码：
```java
import java.time.LocalTime;
import java.time.temporal.ChronoUnit;
public class DateTimeDemonstration {
/**
* @param args
*/
public static void main(String[] args) {
   //Get local time
   LocalTime localTime = LocalTime.now();
   System.out.println(localTime);
  //Get the hour of the day
  System.out.println("The hour of the day:: " + localTime.getHour());
  //add 2 hours to the time.
  System.out.println(localTime.plus(2, ChronoUnit.HOURS));
  //add 6 minutes to the time.
  System.out.println(localTime.plusMinutes(6));
  //substract 2 hours from current time
  System.out.println(localTime.minus(2, ChronoUnit.HOURS));
 }
}
```
# 二、扩展
        相类似的，还有LocalDateTime，Year，Duration，Period等多种类，都是对于时间的不同粒度的操作，并且都封装在java.time这个包内，有兴趣的同志可以了解一下。
       这篇博客写的挺好，可以借鉴一下：
[http://blog.csdn.net/sf_cyl/article/details/51987088](http://blog.csdn.net/sf_cyl/article/details/51987088)
