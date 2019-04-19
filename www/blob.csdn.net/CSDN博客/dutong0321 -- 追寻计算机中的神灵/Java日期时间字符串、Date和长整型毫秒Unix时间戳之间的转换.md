# Java日期时间字符串、Date和长整型毫秒Unix时间戳之间的转换 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年03月14日 22:18:03[dutong0321](https://me.csdn.net/dutong0321)阅读数：2546
### 概述
在项目中经常会用到时间的转换，尤其是日期时间字符串与Date类型的转换。例如：在前台JSP展示时间如果是Date类型查看比较不舒服；如果在SpringHibernate中如果查找日期时间类型的条件必须要用Date类型传值。所以，今天写了一个通用的Java转换类，有需要的可以直接拷贝在项目中，代码也尽量做了精简。
#### Java代码
```
package com.utopia.dateString;
import java.text.SimpleDateFormat;
import java.util.Date;
public class DateString {
    private static SimpleDateFormat sdfLong = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    private static SimpleDateFormat sdfShort = new SimpleDateFormat("yyyy-MM-dd");
    /*
     * 日期时间格式字符串转换为Date类型
     */
    public static Date stringLongToDate(String string) throws Exception {
        return sdfLong.parse(string);
    }
    /*
     * 日期格式字符串转换为Date类型
     */
    public static Date stringShortToDate(String string) throws Exception {
        return sdfShort.parse(string);
    }
    /*
     * 长整型毫秒(自1970-01-01  00:00:00 GMT过去的毫秒数，又称Unix时间戳)转换为Date类型
     */
    public static Date millisecondToDate(long millisecond) {
        return new Date(millisecond);
    }
    /*
     * 日期时间格式字符串转换为(Unix时间戳)长整型类型
     */
    public static long stringLongToMillisecond(String string) throws Exception {
        return stringLongToDate(string).getTime();
    }
    /*
     * 日期格式字符串转换为(Unix时间戳)长整型类型
     */
    public static long stringShortToMillisecond(String string) throws Exception {
        return stringShortToDate(string).getTime();
    }
    /*
     * Date类型转换为(Unix时间戳)长整型类型
     */
    public static long dateToMillisecond(Date date) {
        return date.getTime();
    }
    /*
     * (Unix时间戳)长整型类型转换为日期时间格式字符串
     */
    public static String millisecondToStringLong(long millisecond) {
        return sdfLong.format(millisecond);
    }
    /*
     * Date类型转换为日期时间格式字符串
     */
    public static String dateToStringLong(Date date) {
        return sdfLong.format(date);
    }
    /*
     * (Unix时间戳)长整型类型转换为日期格式字符串
     */
    public static String millisecondToStringShort(long millisecond) {
        return sdfShort.format(millisecond);
    }
    /*
     * Date类型转换为日期格式字符串
     */
    public static String dateToStringShort(Date date) {
        return sdfShort.format(date);
    }
}
```
测试代码：
```
package com.utopia.dateString;
import java.util.Date;
public class DateStringTest {
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        // 获取当前时间的Unix时间戳
        long millisecond = System.currentTimeMillis();
        // Date实例化时，如果不传入参数，则是当前时间
        Date date = new Date();
        String stringLong = "2018-03-14 22:00:00";
        String stringShort = "2018-03-14";
        System.out.println("Date转Unix时间戳：" + DateString.dateToMillisecond(date));
        System.out.println("Date转日期时间格式字符串：" + DateString.dateToStringLong(date));
        System.out.println("Date转日期格式字符串：" + DateString.dateToStringShort(date));
        System.out.println("Unix时间戳转Date：" + DateString.millisecondToDate(millisecond));
        System.out.println("Unix时间戳转日期时间格式字符串：" + DateString.millisecondToStringLong(millisecond));
        System.out.println("Unix时间戳转日期格式字符串：" + DateString.millisecondToStringShort(millisecond));
        try {
            System.out.println("日期时间格式字符串转Date:" + DateString.stringLongToDate(stringLong));
            System.out.println("日期时间格式字符串转Unix时间戳:" + DateString.stringLongToMillisecond(stringLong));
            System.out.println("日期格式字符串转Date:" + DateString.stringShortToDate(stringShort));
            System.out.println("日期格式字符串转Unix时间戳:" + DateString.stringShortToMillisecond(stringShort));  
        } catch (Exception e) {
            // TODO: handle exception
            e.printStackTrace();
        }
    }
}
```
运行截图： 
![运行截图](https://img-blog.csdn.net/20180314221629588?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2R1dG9uZzAzMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
