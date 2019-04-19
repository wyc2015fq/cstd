# System.currentTimeMillis()返回一个错误的值 - Ms.Fox的专栏 - CSDN博客
2019年04月02日 11:01:06[lulu-lu](https://me.csdn.net/smbluesky)阅读数：26
个人分类：[JAVA](https://blog.csdn.net/smbluesky/article/category/6556788)
我的问题是后台用System.currentTimeMillis()传给前端一个时间戳，但是前端解析的时间跟系统时间不一致。怀疑是时区的问题。然后把系统的时区修改为东八区就可以了。果真是前端用东八区的时区来解析的得到时间的。问题就解决了~
下面是转载的，我找到问题出现的原因。
在Y.Daniel Liang《Java语言程序设计》第八版的第二章中。介绍了显示系统的时间的方法：用System.currentTimeMillis()返回一个UNIX时间戳以来的毫秒数。
  由于返回的值是以毫秒为单位的，所以，要我们进行计算是几点几时几分。然而我看了一眼算法，写出的程序，返回的值是错误的。
```
package Demo.book;
public class ShowCurrentTime {
	/**
	 * @显示当前时间
	 * @2014.9.3
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//获得系统的时间，单位为毫秒,转换为妙
		long totalMilliSeconds = System.currentTimeMillis();
		long totalSeconds = totalMilliSeconds / 1000;
		
		//求出现在的秒
		long currentSecond = totalSeconds % 60;
		
		//求出现在的分
		long totalMinutes = totalSeconds / 60;
		long currentMinute = totalMinutes % 60;
		
		//求出现在的小时
		long totalHour = totalMinutes / 60;
		long currentHour = totalHour % 24;
		
		//显示时间
		System.out.println("总毫秒为： " + totalMilliSeconds);
		System.out.println(currentHour + ":" + currentMinute + ":" + currentSecond + " GMT");
	}
}
总毫秒为： 1409721840880
5:24:0 GMT
```
我电脑的真是时间是：13:24:0
这种原因是由时区引起的，看看下面这个会自动转换好的
```
import java.util.TimeZone;
public class ShowCurrentTime {
	/**
	 * @显示当前时间
	 * @2014.9.3
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//获得系统的时间，单位为毫秒,转换为妙
		long totalMilliSeconds = System.currentTimeMillis();
		
		 DateFormat dateFormatterChina = DateFormat.getDateTimeInstance(DateFormat.MEDIUM,DateFormat.MEDIUM);//格式化输出
		  TimeZone timeZoneChina = TimeZone.getTimeZone("Asia/Shanghai");//获取时区 这句加上，很关键。
		  dateFormatterChina.setTimeZone(timeZoneChina);//设置系统时区
		long totalSeconds = totalMilliSeconds / 1000;
		
		//求出现在的秒
		long currentSecond = totalSeconds % 60;
		
		//求出现在的分
		long totalMinutes = totalSeconds / 60;
		long currentMinute = totalMinutes % 60;
		
		//求出现在的小时
		long totalHour = totalMinutes / 60;
		long currentHour = totalHour % 24;
		
		//显示时间
		System.out.println("总毫秒为： " + totalMilliSeconds);
		System.out.println(currentHour + ":" + currentMinute + ":" + currentSecond + " GMT");
		
		
		Date nowTime = new Date(System.currentTimeMillis());
		System.out.println(System.currentTimeMillis());
		  SimpleDateFormat sdFormatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:dd");
		  String retStrFormatNowDate = sdFormatter.format(nowTime);
		  
		  System.out.println(retStrFormatNowDate);
	}
}
2
```
