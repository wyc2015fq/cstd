# Java Web项目中使用Freemarker生成Word文档 - 零度的博客专栏 - CSDN博客
2016年11月21日 16:12:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：611标签：[freemarker																[word导出](https://so.csdn.net/so/search/s.do?q=word导出&t=blog)](https://so.csdn.net/so/search/s.do?q=freemarker&t=blog)
个人分类：[Freemarker](https://blog.csdn.net/zmx729618/article/category/6462774)
Web项目中生成Word文档的操作屡见不鲜，基于Java的解决方案也是很多的，包括使用[Jacob](http://sourceforge.net/projects/jacob-project/)、[Apache POI](http://poi.apache.org/)、Java2Word、[iText](http://itextpdf.com/)等各种方式，其实在从Office
 2003开始，就可以将Office文档转换成XML文件，这样只要将需要填入的内容放上${}占位符，就可以使用像Freemarker这样的模板引擎将出现占位符的地方替换成真实数据，这种方式较之其他的方案要更为简单。
![](https://img-blog.csdn.net/20140921092136315)
下面举一个简单的例子，比如在Web页面中填写个人简历，然后点击保存下载到本地，效果图如下所示。
![](https://img-blog.csdn.net/20140921092640063)
打开下载的Word文件
![](https://img-blog.csdn.net/20140921092800419)
首先在Eclipse [Java EE](http://lib.csdn.net/base/17)版中新建一个Dynamic Web Project，项目结构如下图所示
![](https://img-blog.csdn.net/20140921093734234)
需要向项目中加入freemarker的JAR文件，可以通过下面的链接获得Freemarker的最新版本：
[http://freemarker.org/freemarkerdownload.html](http://freemarker.org/freemarkerdownload.html)
![](https://img-blog.csdn.net/20140921145634111)
模板文件resume.ftl是如何生成的呢，其实非常简单，将需要的Word文档做好之后，选择另存为XML文件，另存之后建议用Editplus、Notepad++、Sublime等工具打开查看一下，因为有的时候你写的占位符可能会被拆开，这样Freemarker就无法处理了。
![](https://img-blog.csdn.net/20140921142730652)
打开XML文件看看吧，如果刚才你写的${title}、${name}被xml文件给拆散了，修改一下XML文件就OK了。
![](https://img-blog.csdn.net/20140921143449044)
修改过后另存为resume.ftl模板文件，如下所示：
![](https://img-blog.csdn.net/20140921143705123)
接下来就是Servlet（也可以是Struts2的Action、Spring MVC的Controller等）和工具类WordGenerator的编写以及页面test.jsp的制作了，代码如下所示：
小服务的代码：
**[java]**[view plain](http://blog.csdn.net/jackfrued/article/details/39449021#)[copy](http://blog.csdn.net/jackfrued/article/details/39449021#)[print](http://blog.csdn.net/jackfrued/article/details/39449021#)[?](http://blog.csdn.net/jackfrued/article/details/39449021#)
- package com.lovo.servlet;  
- 
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.io.IOException;  
- import java.io.InputStream;  
- import java.util.Enumeration;  
- import java.util.HashMap;  
- import java.util.Map;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.ServletOutputStream;  
- import javax.servlet.annotation.WebServlet;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import com.lovo.util.WordGenerator;  
- 
- /**
-  * Servlet implementation class MyServlet
-  */
- @WebServlet("/saveDocServlet")  
- publicclass MyServlet extends HttpServlet {  
- privatestaticfinallong serialVersionUID = 1L;  
- 
- @Override
- protectedvoid service(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
-         req.setCharacterEncoding("utf-8");  
-         Map<String, Object> map = new HashMap<String, Object>();  
-         Enumeration<String> paramNames = req.getParameterNames();  
- // 通过循环将表单参数放入键值对映射中
- while(paramNames.hasMoreElements()) {  
-             String key = paramNames.nextElement();  
-             String value = req.getParameter(key);  
-             map.put(key, value);  
-         }  
- 
- // 提示：在调用工具类生成Word文档之前应当检查所有字段是否完整
- // 否则Freemarker的模板殷勤在处理时可能会因为找不到值而报错 这里暂时忽略这个步骤了
-         File file = null;  
-         InputStream fin = null;  
-         ServletOutputStream out = null;  
- try {  
- // 调用工具类WordGenerator的createDoc方法生成Word文档
-             file = WordGenerator.createDoc(map, "resume");  
-             fin = new FileInputStream(file);  
- 
-             resp.setCharacterEncoding("utf-8");  
-             resp.setContentType("application/msword");  
- // 设置浏览器以下载的方式处理该文件默认名为resume.doc
-             resp.addHeader("Content-Disposition", "attachment;filename=resume.doc");  
- 
-             out = resp.getOutputStream();  
- byte[] buffer = newbyte[512];  // 缓冲区
- int bytesToRead = -1;  
- // 通过循环将读入的Word文件的内容输出到浏览器中
- while((bytesToRead = fin.read(buffer)) != -1) {  
-                 out.write(buffer, 0, bytesToRead);  
-             }  
-         } finally {  
- if(fin != null) fin.close();  
- if(out != null) out.close();  
- if(file != null) file.delete(); // 删除临时文件
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.lovo.servlet;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import com.lovo.util.WordGenerator;
/**
 * Servlet implementation class MyServlet
 */
@WebServlet("/saveDocServlet")
public class MyServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
	@Override
	protected void service(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		req.setCharacterEncoding("utf-8");
		Map<String, Object> map = new HashMap<String, Object>();
		Enumeration<String> paramNames = req.getParameterNames();
		// 通过循环将表单参数放入键值对映射中
		while(paramNames.hasMoreElements()) {
			String key = paramNames.nextElement();
			String value = req.getParameter(key);
			map.put(key, value);
		}
	
		// 提示：在调用工具类生成Word文档之前应当检查所有字段是否完整
		// 否则Freemarker的模板殷勤在处理时可能会因为找不到值而报错 这里暂时忽略这个步骤了
		File file = null;
		InputStream fin = null;
		ServletOutputStream out = null;
		try {
			// 调用工具类WordGenerator的createDoc方法生成Word文档
			file = WordGenerator.createDoc(map, "resume");
			fin = new FileInputStream(file);
			
			resp.setCharacterEncoding("utf-8");
			resp.setContentType("application/msword");
			// 设置浏览器以下载的方式处理该文件默认名为resume.doc
			resp.addHeader("Content-Disposition", "attachment;filename=resume.doc");
			
			out = resp.getOutputStream();
			byte[] buffer = new byte[512];	// 缓冲区
			int bytesToRead = -1;
			// 通过循环将读入的Word文件的内容输出到浏览器中
			while((bytesToRead = fin.read(buffer)) != -1) {
				out.write(buffer, 0, bytesToRead);
			}
		} finally {
			if(fin != null) fin.close();
			if(out != null) out.close();
			if(file != null) file.delete();	// 删除临时文件
		}
	}
}
```
工具类的代码：
**[java]**[view plain](http://blog.csdn.net/jackfrued/article/details/39449021#)[copy](http://blog.csdn.net/jackfrued/article/details/39449021#)[print](http://blog.csdn.net/jackfrued/article/details/39449021#)[?](http://blog.csdn.net/jackfrued/article/details/39449021#)
- package com.lovo.util;  
- 
- import java.io.File;  
- import java.io.FileOutputStream;  
- import java.io.IOException;  
- import java.io.OutputStreamWriter;  
- import java.io.Writer;  
- import java.util.HashMap;  
- import java.util.Map;  
- 
- import freemarker.template.Configuration;  
- import freemarker.template.Template;  
- 
- publicclass WordGenerator {  
- privatestatic Configuration configuration = null;  
- privatestatic Map<String, Template> allTemplates = null;  
- 
- static {  
-         configuration = new Configuration();  
-         configuration.setDefaultEncoding("utf-8");  
-         configuration.setClassForTemplateLoading(WordGenerator.class, "/com/lovo/ftl");  
-         allTemplates = new HashMap<>();   // Java 7 钻石语法
- try {  
-             allTemplates.put("resume", configuration.getTemplate("resume.ftl"));  
-         } catch (IOException e) {  
-             e.printStackTrace();  
- thrownew RuntimeException(e);  
-         }  
-     }  
- 
- private WordGenerator() {  
- thrownew AssertionError();  
-     }  
- 
- publicstatic File createDoc(Map<?, ?> dataMap, String type) {  
-         String name = "temp" + (int) (Math.random() * 100000) + ".doc";  
-         File f = new File(name);  
-         Template t = allTemplates.get(type);  
- try {  
- // 这个地方不能使用FileWriter因为需要指定编码类型否则生成的Word文档会因为有无法识别的编码而无法打开
-             Writer w = new OutputStreamWriter(new FileOutputStream(f), "utf-8");  
-             t.process(dataMap, w);  
-             w.close();  
-         } catch (Exception ex) {  
-             ex.printStackTrace();  
- thrownew RuntimeException(ex);  
-         }  
- return f;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.lovo.util;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.HashMap;
import java.util.Map;
import freemarker.template.Configuration;
import freemarker.template.Template;
public class WordGenerator {
	private static Configuration configuration = null;
	private static Map<String, Template> allTemplates = null;
	
	static {
		configuration = new Configuration();
		configuration.setDefaultEncoding("utf-8");
		configuration.setClassForTemplateLoading(WordGenerator.class, "/com/lovo/ftl");
		allTemplates = new HashMap<>();	// Java 7 钻石语法
		try {
			allTemplates.put("resume", configuration.getTemplate("resume.ftl"));
		} catch (IOException e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
	}
	private WordGenerator() {
		throw new AssertionError();
	}
	public static File createDoc(Map<?, ?> dataMap, String type) {
		String name = "temp" + (int) (Math.random() * 100000) + ".doc";
		File f = new File(name);
		Template t = allTemplates.get(type);
		try {
			// 这个地方不能使用FileWriter因为需要指定编码类型否则生成的Word文档会因为有无法识别的编码而无法打开
			Writer w = new OutputStreamWriter(new FileOutputStream(f), "utf-8");
			t.process(dataMap, w);
			w.close();
		} catch (Exception ex) {
			ex.printStackTrace();
			throw new RuntimeException(ex);
		}
		return f;
	}
}
```
JSP页面的代码：
**[html]**[view plain](http://blog.csdn.net/jackfrued/article/details/39449021#)[copy](http://blog.csdn.net/jackfrued/article/details/39449021#)[print](http://blog.csdn.net/jackfrued/article/details/39449021#)[?](http://blog.csdn.net/jackfrued/article/details/39449021#)
- <%@ page pageEncoding="UTF-8"%>
- <!DOCTYPE html>
- <html>
- <head>
- <metacharset="UTF-8"/>
- <title>Document</title>
- <styletype="text/css">
-     * { font-family: "微软雅黑"; }  
-     .textField { border:none; border-bottom: 1px solid gray; text-align: center; }  
-     #file { border:1px solid black; width: 80%; margin:0 auto; }  
-     h1 input{ font-size:72px; }  
-     td textarea { font-size: 14px; }  
-     .key { width:125px; font-size:20px; }  
- </style>
- </head>
- <body>
- <formaction="saveDocServlet"method="post">
- <divid="file"align="center">
- <h1><inputtype="text"name="title"class="textField"value="我的简历"/></h1>
- <hr/>
- <table>
- <tr>
- <tdclass="key">姓名：</td>
- <td><inputtype="text"name="name"class="textField"/></td>
- <tdclass="key">性别：</td>
- <td>
- <inputtype="radio"name="gender"value="男" checked/>男  
- <inputtype="radio"name="gender"value="女"/>女  
- </td>
- </tr>
- <tr>
- <tdclass="key">联系电话：</td>
- <td><inputtype="text"name="tel"class="textField"/></td>
- <tdclass="key">家庭住址：</td>
- <td><inputtype="text"name="address"class="textField"/></td>
- </tr>
- <tr>
- <tdcolspan="4"class="key">个人简介：</td>
- </tr>
- <tr>
- <tdcolspan="4">
- <textarearows="10"cols="100"name="content"></textarea>
- </td>
- </tr>
- </table>
- </div>
- <divalign="center"style="margin-top:15px;">
- <inputtype="submit"value="保存Word文档"/>
- </div>
- </form>
- </body>
- </html>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<%@ page pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8" />
<title>Document</title>
<style type="text/css">
	* { font-family: "微软雅黑"; }
	.textField { border:none; border-bottom: 1px solid gray; text-align: center; }
	#file { border:1px solid black; width: 80%; margin:0 auto; }
	h1 input{ font-size:72px; }
	td textarea { font-size: 14px; }
	.key { width:125px; font-size:20px; }
</style>
</head>
<body>
	<form action="saveDocServlet" method="post">
	<div id="file" align="center">
		<h1><input type="text" name="title" class="textField" value="我的简历"/></h1>
		<hr/>
		<table>
			<tr>
				<td class="key">姓名：</td>
				<td><input type="text" name="name" class="textField"/></td>
				<td class="key">性别：</td>
				<td>
					<input type="radio" name="gender" value="男" checked/>男
					<input type="radio" name="gender" value="女" />女
				</td>
			</tr>
			<tr>
				<td class="key">联系电话：</td>
				<td><input type="text" name="tel" class="textField"/></td>
				<td class="key">家庭住址：</td>
				<td><input type="text" name="address" class="textField"/></td>
			</tr>
			<tr>
				<td colspan="4" class="key">个人简介：</td>
			</tr>
			<tr>
				<td colspan="4">
					<textarea rows="10" cols="100" name="content"></textarea>
				</td>
			</tr>
		</table>
	</div>
	<div align="center" style="margin-top:15px;">
		<input type="submit" value="保存Word文档" />
	</div>
	</form>
</body>
</html>
```
说明：小服务是使用注解进行配置的，因此你的服务器需要支持Servlet 3规范，我使用的服务器是Tomcat 7.0.52。如果你的服务器不支持Servlet 3规范那就使用web.xml来配置你的小服务吧，其他地方没有不同。如果你不熟悉Servlet 3规范的新特性，可以阅读CSDN上另一篇文章，链接如下所示：
[http://blog.csdn.net/zhongweijian/article/details/8279650](http://blog.csdn.net/zhongweijian/article/details/8279650)
此外，如果你希望在Word文档中插入图片，可以把Word另存为的XML文件中代表图片的那个很长的字符串（[BASE64编码](http://baike.baidu.com/view/469071.htm)的字符串）换成一个占位符，在将要插入Word文档的图片对象转换成BASE64编码的字符串，用该字符串替换掉占位符就可以了，示意图和代码如下所示：
![](https://img-blog.csdn.net/20140921151418095)
将图片转换成BASE64字符串的代码如下所示：
**[java]**[view plain](http://blog.csdn.net/jackfrued/article/details/39449021#)[copy](http://blog.csdn.net/jackfrued/article/details/39449021#)[print](http://blog.csdn.net/jackfrued/article/details/39449021#)[?](http://blog.csdn.net/jackfrued/article/details/39449021#)
- publicstatic String getImageString(String filename) throws IOException {  
-          InputStream in = null;  
- byte[] data = null;  
- try {  
-              in = new FileInputStream(filename);  
-              data = newbyte[in.available()];  
-              in.read(data);  
-              in.close();  
-          } catch (IOException e) {  
- throw e;  
-          } finally {  
- if(in != null) in.close();  
-          }  
-          BASE64Encoder encoder = new BASE64Encoder();  
- return data != null ? encoder.encode(data) : "";  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public static String getImageString(String filename) throws IOException {
		 InputStream in = null;
         byte[] data = null;
         try {
             in = new FileInputStream(filename);
             data = new byte[in.available()];
             in.read(data);
             in.close();
         } catch (IOException e) {
             throw e;
         } finally {
        	 if(in != null) in.close();
         }
         BASE64Encoder encoder = new BASE64Encoder();
         return data != null ? encoder.encode(data) : "";
	}
```
注意：这里使用的BASE64Encoder类在sun.misc包下，rt.jar中有这个类，但是却无法直接使用，需要修改访问权限，在Eclipse中可以这样修改。
在项目上点右键选择Properties菜单项进入如下图所示的界面：
![](https://img-blog.csdn.net/20140921152901968)
![](https://img-blog.csdn.net/20140921153101036)
这样设置后就可以使用BASE64Encoder类了，在项目中调用getImageString方法指定要插入的图片的完整文件名（带路径的文件名），该方法返回的字符串就是将图片处理成BASE64编码后的字符串。但愿你按照上面的步骤一次成功！
