# Java Exception 捕获和展示 - z69183787的专栏 - CSDN博客
2016年08月07日 10:46:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2127
   在进行Java B/S架构开发时，经常有这样的场景：后端处理业务逻辑，前端负责展示，当后端处理出现异常时，如何把错误信息展示给前台呢？错误信息栈通常很多，对开发人员查找问题比较方便，但对于客户来说，打一堆的错误信息，无疑是对他们感官的一种摧残，如何捕捉最重要的信息显示到客户端呢？该信息要求简明扼要，指向出错点，且应指明异常的类型。
        在很多情况下Exception的 getMessage()方法返回空的值，如果使用该方式则会在前端显示空值。我们要显示的重要信息有两个：
-          异常类型
-          出错点和出错信息
1、异常类型如何获得呢？
可以通过Exception 的getClass().getName()方法来实现。
2、出错点如何获得呢？
出错点信息一般在“Cause by:”标识开始的行。如果能抓取到该行，则可取出异常信息，一个异常栈例子如下：
```
Caused by: org.apache.activemq.selector.ParseException: Parse error at line 0, column 0.  Encountered: <EOF>
	at org.apache.activemq.selector.SelectorParser.generateParseException(SelectorParser.java:1231)
	at org.apache.activemq.selector.SelectorParser.jj_consume_token(SelectorParser.java:1179)
	at org.apache.activemq.selector.SelectorParser.unaryExpr(SelectorParser.java:468)
	at org.apache.activemq.selector.SelectorParser.multExpr(SelectorParser.java:390)
	at org.apache.activemq.selector.SelectorParser.addExpression(SelectorParser.java:359)
	at org.apache.activemq.selector.SelectorParser.comparisonExpression(SelectorParser.java:211)
	at org.apache.activemq.selector.SelectorParser.equalityExpression(SelectorParser.java:156)
	at org.apache.activemq.selector.SelectorParser.andExpression(SelectorParser.java:135)
	at org.apache.activemq.selector.SelectorParser.orExpression(SelectorParser.java:114)
	at org.apache.activemq.selector.SelectorParser.JmsSelector(SelectorParser.java:106)
	at org.apache.activemq.selector.SelectorParser.parse(SelectorParser.java:84)
	... 63 more
```
        由于一些原因，往往它并不出现在第一行，所以，通过取第一行的方式不能获取出错点和出错提示信息。
        如果自己解析该输出，一行一行地读入，然后通过判断首字符是否是“Caused by:” 也能抓取到出错点和出错信息。
        最简单的方式，还是使用正则表达式，可以比较简单地实现抓取出错点和出错信息。例如：
代码1：使用正则表达式获取出错点和出错信息
```
String regEx = "Caused by:(.*)";  
		    Pattern pat = Pattern.compile(regEx);  
		    Matcher mat = pat.matcher(content);  
		    boolean rs = mat.find();  
		    System.out.println("found?" + rs);
		    System.out.println(mat.group(1));
```
代码1的结果输出：
org.apache.activemq.selector.ParseException: Parse error at line 0, column 0.  Encountered: <EOF>
3、异常信息的获取
         虽然知道了如何找出错点，但异常信息如何获得呢？Exception.printStrackTrace()中虽然有出错点信息，但都打到控制台上去了，Exception.getStackTrace()，并不能获得出错点的提示信息。
        一个应对办法就是捕获e.printStrackTrace()输出， 使用e.printStackTrace(PrintStream)方法，将异常栈信息先输出到ByteOutputStream ，然后再将ByteOutputStream 转换为字符串，就获得了异常的完整输出。代码为：
代码2：获取完整异常信息
```java
ByteArrayOutputStream baos = new ByteArrayOutputStream();
			e.printStackTrace(new PrintStream(baos));
			String exception = baos.toString();
			System.out.println("baos:" + exception);
```
完整的测试代码--异常Caused by捕获(注：该测试代码中并没有出现Caused by字样，实际应用代码比较多，没有放进测试用例中):
```java
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.PrintStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
public class RegexpTest {
	/**
	 * 读取文件中的内容
	 * @return
	 */
	public String readFile(){
		try {
			String fileName = "D:\\test2\\exception.log";
			File f = new File(fileName);
			FileInputStream fis = new FileInputStream(f);
			int filesize = fis.available();
			byte[] buffer = new byte[filesize];
			fis.read(buffer);
			return new String(buffer);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}		
	}
	
	/**
	 * 正则表达式测试
	 */
	public void test(){
		try {
			String content = readFile();
			System.out.println(content);
			
		    String regEx = "Caused by:(.*)";  
		    Pattern pat = Pattern.compile(regEx);  
		    Matcher mat = pat.matcher(content);  
		    boolean rs = mat.find();  
		    System.out.println("found?" + rs);
		    System.out.println(mat.group(1));
//		    for(int i=1;i<=mat.groupCount();i++){  
//		    	System.out.println("found:" + mat.group(i));  
//		    }   
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void test2(){
		try {
			FileInputStream fis = new FileInputStream("d:\\test.txt");
			fis.read();
		} catch (Exception e) {
			e.printStackTrace();
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			e.printStackTrace(new PrintStream(baos));
			String exception = baos.toString();
			System.out.println("exception:" + exception);
		}
	}
	
	public static void main(String[] args) {
		RegexpTest rt = new RegexpTest();
		//rt.test();
		rt.test2();
	}
```
4、获取异常类型和出错点还有一种简单的方法
获取出错点类型：
```java
e.getCause().getClass()
```
获取出错点信息（出错原因）：
```java
e.getCause().getMessage()
```
代码示例（注：实际代码截取，不可直接运行）：
```java
@SuppressWarnings("unchecked")
	@RequestMapping(value="/createSubscriber", method = RequestMethod.POST)
	public @ResponseBody
	WrappedResult createSubscriber(@ItemsRequestBody List<Map> list) {
		LocalBrokerFacade facade = new LocalBrokerFacade(BrokerRegistry.getInstance().findFirst());
		WrappedResult result = new WrappedResult();
		try {
			Map params = list.get(0);
			String clientId = (String)params.get("clientId");
			String subscriberName = (String)params.get("subscriberName");
			String topicName = (String)params.get("topicName");
			String selector = (String)params.get("selector");
			
//			if("".equals(selector)){
//				selector = null;
//			}
			
			facade.getBrokerAdmin().createDurableSubscriber(clientId,
					subscriberName,topicName,selector);
			result.setSuccessful(true);
		} catch (Exception e) {
			System.out.println("Exception:" + e.getCause().getClass() + "," + e.getCause().getMessage());
			//log.error("createSubscriber failed.", e);
		}
```
输出：Exception:class org.apache.activemq.selector.ParseException,Parse error at line 0, column 0.  Encountered: <EOF>
