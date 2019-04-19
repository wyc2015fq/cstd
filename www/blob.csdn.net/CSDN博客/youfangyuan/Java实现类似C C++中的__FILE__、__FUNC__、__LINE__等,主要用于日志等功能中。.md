# Java实现类似C/C++中的__FILE__、__FUNC__、__LINE__等,主要用于日志等功能中。 - youfangyuan - CSDN博客
2013年05月19日 15:10:08[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：1700
```java
package example;
import java.text.SimpleDateFormat; 
import java.util.Date; 
/** 
* Java实现类似C/C++中的__FILE__、__FUNC__、__LINE__等,主要用于日志等功能中。 
* 
* @version 1.0 2011-07-13 
* 
*/ 
public abstract class CommonFunction { 
	
	/**
	 * 打印日志
	 */
	public static void log()
	{
		System.out.println(getFileLineMethod());
	}
	/** 
	* 打印日志时获取当前的程序文件名、行号、方法名 输出格式为：[FileName | LineNumber | MethodName] 
	* 
	* @return 
	*/ 
	public static String getFileLineMethod() { 
		StackTraceElement traceElement = ((new Exception()).getStackTrace())[2]; 
		StringBuffer toStringBuffer = new StringBuffer("[").append( 
		traceElement.getFileName()).append(" | ").append( 
		traceElement.getLineNumber()).append(" | ").append( 
		traceElement.getMethodName()).append("]"); 
		return toStringBuffer.toString(); 
	} 
	
	// 当前文件名 
	public static String _FILE_() { 
		StackTraceElement traceElement = ((new Exception()).getStackTrace())[1]; 
		return traceElement.getFileName(); 
	} 
	
	// 当前方法名 
	public static String _FUNC_() { 
		StackTraceElement traceElement = ((new Exception()).getStackTrace())[1]; 
		return traceElement.getMethodName(); 
	} 
	
	// 当前行号 
	public static int _LINE_() { 
		StackTraceElement traceElement = ((new Exception()).getStackTrace())[1]; 
		return traceElement.getLineNumber(); 
	} 
	
	// 当前时间 
	public static String _TIME_() { 
		Date now = new Date(); 
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS"); 
		return sdf.format(now); 
	} 
}
```
转[http://wenjuema.iteye.com/blog/643596](http://wenjuema.iteye.com/blog/643596)
