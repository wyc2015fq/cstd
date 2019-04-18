# java中Date与String的相互转化 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月06日 23:10:40[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：21标签：[data和String转换](https://so.csdn.net/so/search/s.do?q=data和String转换&t=blog)


转自：[点击打开链接](https://blog.csdn.net/woshisap/article/details/6742423/)

1：大体思路

[html][view plain](https://blog.csdn.net/woshisap/article/details/6742423/#)[copy](https://blog.csdn.net/woshisap/article/details/6742423/#)

- 这种转换要用到java.text.SimpleDateFormat类  
- 
- 字符串转换成日期类型：  
- 方法1：  
- 
- 也是最简单的方法 Date date=new Date("2008-04-14");  
- 
- 方法2：  
- 
- SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd");//小写的mm表示的是分钟  
- String dstr="2008-4-24";  
- java.util.Date date=sdf.parse(dstr);  
- 
- 
- 
- 日期转换成字符串：  
- 
- SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd");  
- java.util.Date date=new java.util.Date();  
- String str=sdf.format(date);  

2：程序实例

[html][view plain](https://blog.csdn.net/woshisap/article/details/6742423/#)[copy](https://blog.csdn.net/woshisap/article/details/6742423/#)

- package test;  
- import java.text.DateFormat;  
- import java.text.SimpleDateFormat;  
- import java.text.ParseException;  
- import java.util.Date;  
- public class StringOrDate {  
-     public static String dateToString(Date date, String type) {  
-         String str = null;  
-         DateFormat format = new SimpleDateFormat("yyyy-MM-dd");  
-         if (type.equals("SHORT")) {  
-             // 07-1-18  
- format = DateFormat.getDateInstance(DateFormat.SHORT);  
- str = format.format(date);  
-         } else if (type.equals("MEDIUM")) {  
-             // 2007-1-18  
- format = DateFormat.getDateInstance(DateFormat.MEDIUM);  
- str = format.format(date);  
-         } else if (type.equals("FULL")) {  
-             // 2007年1月18日 星期四  
- format = DateFormat.getDateInstance(DateFormat.FULL);  
- str = format.format(date);  
-         }  
-         return str;  
-     }  
-     public static Date stringToDate(String str) {  
-         DateFormat format = new SimpleDateFormat("yyyy-MM-dd");  
-         Date date = null;  
-         try {  
-             // Fri Feb 24 00:00:00 CST 2012  
- date = format.parse(str);   
-         } catch (ParseException e) {  
-             e.printStackTrace();  
-         }  
-         // 2012-02-24  
- date = java.sql.Date.valueOf(str);  
- 
-         return date;  
-     }  
-     public static void main(String[] args) {  
-         Date date = new Date();  
-         System.out.println(StringOrDate.dateToString(date, "MEDIUM"));  
-         String str = "2012-2-24";  
-         System.out.println(StringOrDate.stringToDate(str));  
-     }  
- 

