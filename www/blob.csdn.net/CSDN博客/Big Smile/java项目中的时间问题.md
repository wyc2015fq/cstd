# java项目中的时间问题 - Big Smile - CSDN博客
2017年09月10日 23:55:25[王啸tr1912](https://me.csdn.net/tr1912)阅读数：622
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        使用过java的同志们都知道，时间问题在java这种对于jar包比较开放的地方，是很随意的，所以我们在java中引入不同的包，会使用到不同的时间日期类型的对象，比如java.util.date  或者是  java.sql.date等等，那么，我们如何才能更好的利用里面的转换，为我们提供更好的服务呢，如何才能让我们存储的时间或者是展示的时间是我们最最想要的呢？
# 一、大统一法（字符）
        其实在设计数据库的时候，我们就应该想到过时间类型数据的存储问题了，对于时间如何进行存储，不同的设计者可能会有不同的考虑，比如，对于时间精度的要求，对于查找和时间类型的转换的要求，由于各国文化的发展，使得时间的表达方式在不同的地方有很多的不同，不过这都不是主要的问题。重要的是我们如何去把存储的时间转换为我们想要的类型，这才是正解。
        其实最最简单的方式就是把任何时间存储为字符串类型的数据，由于存储为字符串类型了，所以对于数据的自由度也就变的很是灵活了，我们在访问或者存储的时候也方便很多，这个是最最简单的解决方法，不足之处在于不能很方便的分解里面的各种时间元素，不能很好的以对象的方式对于时间进行简单的加减运算，因为字符类型本身只是支持比较，而不会去支持运算的。
# 二、对象法（固有对象）
        时间对象法，顾名思义，就是使用java中带的存放时间的对象进行时间的存储和操作，这样的好处就是我们在进行时间间隔转换的时候比较有优势，比如，现在的日期加7天，或者现在的时间加12个小时等等运算都可以一步到位。但是这里又分是针对什么进行操作，或者比较。由于数据库类型不同的原因，在数据库中存储的时间类型和格式也是不怎么相同的，所以我们在进行数据存储的时候需要进行格式的转化。
        java中自带的日期时间类型有两种，分别是util.date和sql.date，但是sql.date是继承自util.date的，所以在表示上并没有太大区别，只是在初始化的时候有不同，而且sql.date更适用于数据库的操作，而util.date更适用于对于用户的展示和进行复杂的运算。
        然而这个时候我们有新的问题，就是如何去展示的问题
# 三、日期时间的展示
        字符串的展示就不多说了，直接展示就OK，如果碰到特殊要求，需要把字符转换为对象类型，然后再进行操作，如此便可以使我们的字符也做类型转换了，但是如果是对象类型呢？
这里我们引入一个工具类，专门用于进行时间类型的转换：
```java
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.regex.Pattern;
/**
 * 日期时间转换工具类
 * Created by sam on 2017-08-29.
 */
public class DateTimeUtil {
    /**
     * MMddHHmmssSSS
     */
    public static String FILE_NAME = "MMddHHmmssSSS";
    /**
     * yyyy-MM-dd
     */
    public static String DEFAULT_PATTERN = "yyyy-MM-dd";
    /**
     * yyyy/MM/dd/
     */
    public static String DIR_PATTERN = "yyyy/MM/dd/";
    /**
     * yyyy-MM-dd HH:mm:ss
     */
    public static String TIMESTAMP_PATTERN = "yyyy-MM-dd HH:mm:ss";
    /**
     * HH:mm:ss
     */
    public static String TIMES_PATTERN = "HH:mm:ss";
    /**
     * yyyyMMddHHmmss
     */
    public static String NOCHAR_PATTERN = "yyyyMMddHHmmss";
    /**
     * 获取当前时间戳
     *
     * @return
     */
    public static String formatDefaultFileName() {
        return formatDateByFormat(new Date(), FILE_NAME);
    }
    /**
     * 日期转换为字符串
     *
     * @param date
     * 日期
     * @param format
     * 日期格式
     * @return 指定格式的日期字符串
     */
    public static String formatDateByFormat(Date date, String format) {
        String result = "";
        if (date != null) {
            try {
                SimpleDateFormat sdf = new SimpleDateFormat(format);
                result = sdf.format(date);
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
        return result;
    }
    /**
     * 转换为默认格式(yyyy-MM-dd)的日期字符串
     *
     * @param date
     *
     * @return
     */
    public static String formatDefaultDate(Date date) {
        return formatDateByFormat(date, DEFAULT_PATTERN);
    }
    /**
     * 转换为目录格式(yyyy/MM/dd/)的日期字符串
     *
     * @param date
     *
     * @return
     */
    public static String formatDirDate(Date date) {
        return formatDateByFormat(date, DIR_PATTERN);
    }
    /**
     * 转换为完整格式(yyyy-MM-dd HH:mm:ss)的日期字符串
     *
     * @param date
     *
     * @return
     */
    public static String formatTimesTampDate(Date date) {
        return formatDateByFormat(date, TIMESTAMP_PATTERN);
    }
    /**
     * 转换为时分秒格式(HH:mm:ss)的日期字符串
     *
     * @param date
     *
     * @return
     */
    public static String formatTimesDate(Date date) {
        return formatDateByFormat(date, TIMES_PATTERN);
    }
    /**
     * 转换为时分秒格式(HH:mm:ss)的日期字符串
     *
     * @param date
     *
     * @return
     */
    public static String formatNoCharDate(Date date) {
        return formatDateByFormat(date, NOCHAR_PATTERN);
    }
    /**
     * 日期格式字符串转换为日期对象
     *
     * @param strDate
     * 日期格式字符串
     * @param pattern
     * 日期对象
     * @return
     */
    public static Date parseDate(String strDate, String pattern) {
        try {
            SimpleDateFormat format = new SimpleDateFormat(pattern);
            Date nowDate = format.parse(strDate);
            return nowDate;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
    /**
     * 字符串转换为默认格式(yyyy-MM-dd)日期对象
     *
     * @param date
     *
     * @return
     *
     * @throws Exception
     */
    public static Date parseDefaultDate(String date) {
        return parseDate(date, DEFAULT_PATTERN);
    }
    /**
     * 字符串转换为完整格式(yyyy-MM-dd HH:mm:ss)日期对象
     *
     * @param date
     *
     * @return
     *
     * @throws Exception
     */
    public static Date parseTimesTampDate(String date) {
        return parseDate(date, TIMESTAMP_PATTERN);
    }
    /**
     * 获得当前时间
     *
     * @return
     */
    public static Date getCurrentDate() {
        Calendar calendar = Calendar.getInstance();
        return calendar.getTime();
    }
    /**
     * sql Date 转 util Date
     *
     * @param date
     * java.sql.Date日期
     * @return java.util.Date
     */
    public static Date parseUtilDate(java.sql.Date date) {
        return date;
    }
    /**
     * util Date 转 sql Date
     *
     * @param date
     * java.sql.Date日期
     * @return
     */
    public static java.sql.Date parseSqlDate(Date date) {
        return new java.sql.Date(date.getTime());
    }
    /**
     * 获取年份
     *
     * @param date
     *
     * @return
     */
    public static int getYear(Date date) {
        Calendar c = Calendar.getInstance();
        c.setTime(date);
        return c.get(Calendar.YEAR);
    }
    /**
     * 获取月份
     *
     * @param date
     *
     * @return
     */
    public static int getMonth(Date date) {
        Calendar c = Calendar.getInstance();
        c.setTime(date);
        return c.get(Calendar.MONTH) + 1;
    }
    /**
     * 获取星期
     *
     * @param date
     *
     * @return
     */
    public static int getWeek(Date date) {
        Calendar c = Calendar.getInstance();
        c.setTime(date);
        int dayOfWeek = c.get(Calendar.DAY_OF_WEEK);
        dayOfWeek = dayOfWeek - 1;
        if (dayOfWeek == 0) {
            dayOfWeek = 7;
        }
        return dayOfWeek;
    }
    /**
     * 获取日期(多少号)
     *
     * @param date
     *
     * @return
     */
    public static int getDay(Date date) {
        Calendar c = Calendar.getInstance();
        c.setTime(date);
        return c.get(Calendar.DAY_OF_MONTH);
    }
    /**
     * 获取当前时间(小时)
     *
     * @param date
     *
     * @return
     */
    public static int getHour(Date date) {
        Calendar c = Calendar.getInstance();
        c.setTime(date);
        return c.get(Calendar.HOUR_OF_DAY);
    }
    /**
     * 获取当前时间(分)
     *
     * @param date
     *
     * @return
     */
    public static int getMinute(Date date) {
        Calendar c = Calendar.getInstance();
        c.setTime(date);
        return c.get(Calendar.MINUTE);
    }
    /**
     * 获取当前时间(秒)
     *
     * @param date
     *
     * @return
     */
    public static int getSecond(Date date) {
        Calendar c = Calendar.getInstance();
        c.setTime(date);
        return c.get(Calendar.SECOND);
    }
    /**
     * 获取当前毫秒
     *
     * @param date
     *
     * @return
     */
    public static long getMillis(Date date) {
        Calendar c = Calendar.getInstance();
        c.setTime(date);
        return c.getTimeInMillis();
    }
    /**
     * 日期增加
     *
     * @param date
     * Date
     *
     * @param day
     * int
     *
     * @return Date
     */
    public static Date addDate(Date date, int day) {
        java.util.Calendar c = java.util.Calendar.getInstance();
        c.setTimeInMillis(getMillis(date) + ((long) day) * 24 * 3600 * 1000);
        return c.getTime();
    }
    /**
     * 日期相减(返回天数)
     *
     * @param date
     * Date
     *
     * @param date1
     * Date
     *
     * @return int 相差的天数
     */
    public static int diffDate(Date date, Date date1) {
        return (int) ((getMillis(date) - getMillis(date1)) / (24 * 3600 * 1000));
    }
    /**
     * 日期相减(返回秒值)
     *
     * @param date
     * Date
     * @param date1
     * Date
     * @return int
     * @author
     */
    public static Long diffDateTime(Date date, Date date1) {
        return (Long) ((getMillis(date) - getMillis(date1)) / 1000);
    }
    private static String[] randomValues = new String[] { "0", "1", "2", "3",
            "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f", "g",
            "h", "i", "j", "k", "l", "m", "n", "u", "t", "s", "o", "x", "v",
            "p", "q", "r", "w", "y", "z" };
    public static String getRandomNumber(int lenght) {
        StringBuffer str = new StringBuffer();
        for (int i = 0; i < lenght; i++) {
            Double number = Math.random() * (randomValues.length - 1);
            str.append(randomValues[number.intValue()]);
        }
        return str.toString();
    }
    /**
     * 生成账号
     *
     * @param acount
     * @return
     */
    public static String nextAcounnt(String acount) {
        String newAcc = "";
        if (Integer.parseInt(acount) < 10000) {
            Integer newAc = Integer.parseInt(acount) + 1;
            if (newAc < 1000) {
                int count = String.valueOf(newAc).length();
                if (count == 1) {
                    newAcc = "000" + newAc;
                } else if (count == 2) {
                    newAcc = "00" + newAc;
                } else if (count == 3) {
                    newAcc = "0" + newAc;
                }
            } else {
                newAcc = String.valueOf(newAc);
            }
        } else {
            newAcc = acount;
        }
        return newAcc;
    }
    public static boolean isNumeric1(String str) {
        if (str != null && !"".equals(str) && str.length()<=9) {
            Pattern pattern = Pattern.compile("[0-9]*");
            return pattern.matcher(str).matches();
        } else {
            return false;
        }
    }
    /**
     * 生成流水号
     *
     * @param t
     * 流水号位数
     * @return 流水号
     */
    public static String getSequenceNumber(int t) {
        Date d = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMddhhmmss");
        String str = sdf.format(d);
        String haomiao = String.valueOf(System.nanoTime());
        str = str + haomiao.substring(haomiao.length() - 6, haomiao.length());
        return str.substring(str.length() - t, str.length());
    }
}
```
        有了这个类我们既可以把字符转化为对象，又可以把对象转换为字符类型的时间，而且通过不同的方法可以去实现不同类型时间的转换很是方便，我们在往数据库中写数据的时候可以用上它。
# 四、时间的回显
        有的时候，我们在查询数据的时候，需要返回一个数据对象的集合，由于对象型数据的特点，java在序列化的时候，总是会把时间类型的数据转换为一串int类型的数字集合，一般情况下是没有人可以看得懂的，只有经过转码才可以看懂，这样很是麻烦，那么这个问题怎么办呢？
        这个时候，我们可以在实体类中的时间类型数据的get方法上面加上一个注解：
@JsonFormat (pattern=**"yyyy-MM-dd HH:mm:ss"**,timezone=**"GMT+8"**)
        这样，在对象类型数据在回显为json格式字符串的时候，就不会发生时间的乱码问题了，关于这个注解，强调一下的就是，时区的标志，他默认显示的是0时区的时间，也就是格林威治时间，比如，我们在东八区，那么，在显示时间的时候如果没有加时区标志，他会自动判断当前时间以及所在区域，换算为格林威治时间，比如现在12:00,那么他显示的时间就是12-8也就是4:00，所以这里我们需要注意。
