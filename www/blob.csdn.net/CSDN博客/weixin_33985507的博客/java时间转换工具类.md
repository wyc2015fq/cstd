# java时间转换工具类 - weixin_33985507的博客 - CSDN博客
2018年06月30日 22:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
这个持续更新吧，只是项目暂时用到的
package bingosoft.epm.helper.utils;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.ParsePosition;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
public class DateUtils {
public static long ONE_DAY =24 *60 *60 *1000L;
public static long ONE_HOUR =60 *60 *1000L;
public static long ONE_MINUTE =60 *1000L;
private static final StringDATE_PATTERNS[] = {"yyyy-MM-dd HH:mm:ss.SSS",
"yyyy-MM-dd HH:mm:ss","yyyy-MM-dd HH:mm","yyyy-MM-dd",
"yyyy/MM/dd HH:mm:ss","yyyy/MM/dd HH:mm","yyyy/MM/dd",
"yyyyMMddHHmmss","yyyyMMddHHmm","yyyyMMdd","HH:mm:ss","HH:mm","yyyyMMddHHmmssSSS"};
public static final int DATE_NORMAL =0;
public static final int DATE_COMPACT =7;
public static Date toDate(Object value) {
Date date =null;
if (value !=null) {
parse:
if (valueinstanceof Timestamp) {
date =new Date(((Timestamp) value).getTime());
}else if (valueinstanceof Date) {
date = (Date) value;
}else if (valueinstanceof Number) {
date =new Date(((Number) value).longValue());
}else {
String source = String.valueOf(value);
SimpleDateFormat parser =new SimpleDateFormat();
parser.setLenient(true);
ParsePosition pos =new ParsePosition(0);
for (String pattern :DATE_PATTERNS) {
parser.applyPattern(pattern);
pos.setIndex(0);
date = parser.parse(source, pos);
if (date !=null && pos.getIndex() == source.length()) {
break parse;
}
}
date =null;
}
}
return date;
}
public static String toString(Date date) {
return toString(date,DATE_NORMAL);
}
public static String toString(Date date,int pattern) {
if (date ==null) {
return null;
}
if (pattern <0 || pattern >=DATE_PATTERNS.length) {
pattern =DATE_NORMAL;
}
SimpleDateFormat parser =new SimpleDateFormat(DATE_PATTERNS[pattern]);
return parser.format(date);
}
public static Date getDate(int delay) {
return getDate(null, delay);
}
public static Date getDate(Date startTime,int delay) {
if (startTime ==null) {
startTime =new Date();
}
if (delay >0) {
Calendar calendar = Calendar.getInstance();
calendar.setTime(startTime);
calendar.add(Calendar.MILLISECOND, delay);
startTime = calendar.getTime();
}
return startTime;
}
/**
     * 时间格式化 返回类型 ：2015年6月3号 星期几 上午 10.00-16：00
     * 用于开会时间格式填写
     *
     * @return
     */
    public static String dateFormmat(Date start, Date endTime) {
DateFormat format1 =new SimpleDateFormat("yyyy年MM月dd日");
//开始时间
        String stringTime = format1.format(start);
//结束时间
        String end = format1.format(endTime);
String temptimes =null;
if (stringTime.equals(end)) {
DateFormat format =new SimpleDateFormat("yyyy年MM月dd日 EEE aaa hh:mm");
stringTime = format.format(start);
DateFormat format2 =new SimpleDateFormat("hh:mm");
String endTimes = format2.format(endTime);
temptimes = stringTime +"-" + endTimes;
}else {
DateFormat format =new SimpleDateFormat("yyyy年MM月dd日 EEE aaa hh:mm");
stringTime = format.format(start);
DateFormat format2 =new SimpleDateFormat(" yyyy年MM月dd日 EEE hh:mm");
String endTimes = format2.format(endTime);
temptimes = stringTime +"-" + endTimes;
}
return temptimes;
}
/**
     * 根据秒数获取时间串
     *
     * @param time (2018-10-1)
     * @return (2018-10-1 00:00:00)
*/
    public static String getTimeStrBySecond(Date time, String end) {
SimpleDateFormat simpleDateFormat =new SimpleDateFormat("yyyy-MM-dd");
String times = simpleDateFormat.format(time);
if (end !=null) {
times +=" 23:59:59";
}else {
times +=" 00:00:00";
}
return times;
}
/**
     * 格式化时间
     *
     * @param time1
     * @param time2
     * @return 06：20-08：30
*/
    public static String getHhss(Date time1, Date time2) {
DateFormat format1 =new SimpleDateFormat("HH:mm");
//开始时间格式化
        String stringTime = format1.format(time1);
//结束时间格式化
        String endTimes = format1.format(time2);
String temptimes = stringTime +"-" + endTimes;
return temptimes;
}
/**
     * 获取时间相差的天数
     *
     * @param beginDate
     * @param endDate
     * @return 相差的天数
     */
    public static long getDaySub(Date beginDate, Date endDate) {
long day =0;
try {
if (endDate ==null) {
return day;
}
day = (endDate.getTime() - beginDate.getTime()) / (24 *60 *60 *1000);
//System.out.println("相隔的天数="+day);
        }catch (Exception e) {
e.printStackTrace();
}
return day;
}
/**
     * 日期转星期
     *
     * @param datetime
     * @return
     * @throws java.text.ParseException
*/
    public static String dateToWeek(String datetime)throws java.text.ParseException {
SimpleDateFormat ff =new SimpleDateFormat("yyyy-MM-dd");
String[] weekDays = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
Calendar cal = Calendar.getInstance();// 获得一个日历
        Date datet =null;
Date now =new Date();
String nowTime = ff.format(now);
try {
datet = ff.parse(datetime);
cal.setTime(datet);
}catch (ParseException e) {
e.printStackTrace();
}
int w = cal.get(Calendar.DAY_OF_WEEK) -1;// 指示一个星期中的某天。
        if(datetime.equals(nowTime)){
weekDays[w]="今日";
}
if (w <0) {
w =0;
}
return weekDays[w];
}
/**
     * 指定日期加一天
     *
     * @param time
     * @return
     */
    public static Date getNextDay(Date time) {
return new Date(time.getTime() +24 *3600 *1000);
}
/**
     * 相隔天数，后面-前面
     * @param start
     * @param end
     * @return
     */
    public static int getDays(Date start, Date end) {
return (int) ((end.getTime() - start.getTime()) /ONE_DAY);
}
}
