
# Java实用方法整理（四）——日期处理案例+常用方法 - manduner的博客 - CSDN博客


2018年04月09日 23:22:46[Manduner_TJU](https://me.csdn.net/manduner)阅读数：1197


# 1，Date类常用方法
## （1）public long getTime()
获取时间，以毫秒为单位。即，从Date得到一个毫秒值
```python
import java.util.Date;
public class DateDemo {
	public static void main(String[] args) {
		//创建对象
		Date d = new Date();
		
		//获取时间
		long time = d.getTime();
		System.out.println(time);//输出：1522250835934
	}
}
```
## （2）public void setTime(long time)
设置时间
```python
import java.util.Date;
public class DateDemo {
	public static void main(String[] args) {
		//创建对象
		Date d = new Date();
		
		//设置时间
		d.setTime(1000);
		System.out.println(d); //输出：Thu Jan 01 08:00:01 CST 1970
		
	}
}
```
# 2，DateFormat类常用方法
## （1）原理
1）DateFormat：可以进行日期和字符串的格式化和解析，但是由于是抽象类，所以使用具体子类SimpleDateFormat
2）SimpleDateFormat的构造方法：
SimpleDateFormat():默认模式
SimpleDateFormat(String pattern):给定的模式
3）在API中可以查看出对应的模式有哪些，下面给出最常用的模式：
年 y，月 M，日d，时 H，分 m，秒 s
例如：2018/04/09 21:20:29     模式为：“yyyy/MM/dd HH:mm:ss”

## （2）案例一：源码
```python
/*需求：实现日期（Date类型）和字符串的相互转换
 * （1）日期——》字符串    称为：格式化
 * public final string format(Date date)
 * （2）字符串——》日期    称为：解析
 * public Date parse(String source)
 * 
 */
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
public class DateFormatDemo {
	
	public static void main(String[] args) throws ParseException {
		//创建Date对象
		Date d = new Date();
		System.out.println(d);//输出（实时的时间）：Mon Apr 09 21:40:30 CST 2018
		//将日期的Date类型转换为String类型
		//创建SimpleDateFormat对象
		SimpleDateFormat sdf = new SimpleDateFormat();//默认模式
		SimpleDateFormat sdf2 = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");//给定模式
		
		//调用format方法，将Date类型格式化为字符串类型
		String strd = "";
		String strd2 = "";
		strd = sdf.format(d);
		strd2 = sdf2.format(d);
		System.out.println("默认模式：" + strd);//输出：18-4-9 下午9:40
		System.out.println("给定模式：" + strd2);//输出：2018/04/09 21:40:30
		
		
		//将日期的String类型转换为Date类型
		String strd3 = "2018/04/09 21:40:30";
		Date d3 = sdf2.parse(strd3);
		System.out.println(d3); //输出：Mon Apr 09 21:40:30 CST 2018
		
	}
}
```
## （3）案例二：源码
### 1）封装日期工具类源码：实现Date类型到String的相互转换
```python
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
/**
 * @author Manduner_TJU
 * @version 2018/04/09
 */
public class DateTool {
	//创建私有的无参构造方法
	private DateTool() {	
	}
	
	//Date类型 ——》 String类型
	
	/**
	 * @param d （待处理的日期）
	 * @param pattern （返回的日期格式）
	 * @return  格式化后的字符串
	 */
	public static String dateToString(Date d,String pattern) {
		return new SimpleDateFormat(pattern).format(d);
	} 
	
	//String类型 ——》 Date类型
	
	/**
	 * @param source  （被解析的字符串）
	 * @param pattern  （传递过来要被转换的格式）
	 * @return （解析后的日期对象）
	 * @throws ParseException
	 */
	public static Date StringToDate(String source, String pattern) throws ParseException {
		return new SimpleDateFormat(pattern).parse(source);
	}
}
```
### 2）DateTool应用案例（注意：在我的工程中，我将DateTool工具类和DateTest类放到了一个包当中）
```python
import java.text.ParseException;
import java.util.Date;
import java.util.Scanner;
/**
* @author Manduner_TJU
* @version 创建时间：2018年4月9日下午10:44:38
*/
/*需求：计算某个人的年龄大小
 *分析：
 * （1）键盘录入你的出生的年月日
 * （2）把该字符串转换为一个日期
 * （3）通过该日期得到一个毫秒值D1
 * （4）获取当前时间的毫秒值D2
 * （5）用D2-D1得到差值E（单位：毫秒）
 * （6）把E转换为年
 */
public class DateTest {
	public static void main(String[] args) throws ParseException {
		//（1）键盘录入出生的年月日
		Scanner sc = new Scanner(System.in);
		System.out.println("请输入您的生日(yyyy-MM-dd):");
		String s = sc.nextLine(); 
		//（2）把该字符串转换为一个日期
		Date d = DateTool.StringToDate(s, "yyyy-MM-dd");
		//（3）通过该日期得到一个毫秒值D1
		long t1 = d.getTime();
		//（4）获取当前时间的毫秒值D2
		long t2 = System.currentTimeMillis();
		//（5）用D2-D1得到差值E（单位：毫秒）
		long t3 = t2 - t1;
		//（6）把E转换为年
		int old = (int)(t3/1000/60/60/24/365);
		System.out.println(old);
				
 	}
}
```
# 3，Calendar类常用方法+面试题
## （1）注意点
Calendar类是抽象类，不能直接使用，需要调用需要通过调用getInstance()函数进行实例化；
Calendar较之Date类，更为常用

## （2）案例+面试题 源码
```python
import java.util.Calendar;
import java.util.Scanner;
/**
* @author Manduner_TJU
* @version 创建时间：2018年4月10日下午10:26:01
*/
/* 需求：获取任意一年的二月有多少天
 * 分析：
 * 		（1）键盘录入任意的年份
 * 		（2）利用set()函数，设置日历对象的年月日
 * 				年就是A输入的数据
 * 				月是2（注意：虽然写的是2，但是设置后的MONTH值是3）
 * 				日是1
 * 		（3）利用add()函数，把时间往前推一天，得到的就会是2月的最后一天
 * 		（4）输出处理后的这一天
 * 
 * 		注：1）这种问题经常出现在面试题中
 * 		   2）该方法不用判断任意输入的那一年到底是不是闰年。代码不仅简介而且高效，推荐使用
 * 		   3）Calender是抽象类，不能直接使用，需要通过调用getInstance()函数进行实例化
 */
public class CalendarTest {
	public static void main(String[] args) {
		//（1）键盘录入任意的年份
		Scanner sc = new Scanner(System.in);
		System.out.println("请输入年份：");
		int year = sc.nextInt();
		
		//创建Calendar对象
		Calendar c = Calendar.getInstance();
		
		//（2）利用set()函数，设置日历对象的年月日
		c.set(year, 2, 1);//其实此时是该年的3月1日
		
		//（3）利用add()函数，把时间往前推一天，得到的就会是2月的最后一天
		c.add(Calendar.DATE, -1);
		
		//（4）输出处理后时间中的“日”字段值
		int d = c.get(Calendar.DATE);
		System.out.println(d);
		
		
	}
}
```


