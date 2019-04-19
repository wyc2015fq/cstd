# OSGI实战(4)-实战OSGI-翻译助手项目02 - 零度的博客专栏 - CSDN博客
2017年01月17日 11:34:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：355
                
上次我们配置了运行Web项目所需要的Run Configuration环境，创建了客户端入口插件TranslateClient模块，下面我们来继续完成客户端模块的程序。
因为是Web项目，我们要让用户通过浏览器去访问我们的应用，所以我们要去写一些Servlet来处理Http请求。
因为我们的工程是plug-in工程，不是传统的Web project，所以不能直接创建一个Servlet(因为涉及到Web.xm的mapping映射和访问路径的设置)，我们需要创建一个类，让它去继承HttpServlet来实现这个Servlet。
首先我们先在MANIFEST.MF文件中通过配置导入Servlet的相关包:
![](https://img-blog.csdn.net/20160317182846754?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后写一个类继承HttpServlet来实现这个Servlet:
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50916011#)[copy](http://blog.csdn.net/acmman/article/details/50916011#)[print](http://blog.csdn.net/acmman/article/details/50916011#)[?](http://blog.csdn.net/acmman/article/details/50916011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1614054/fork)
- package com.zyg.osgi.translateclient.servlet;  
- 
- import java.io.IOException;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import org.osgi.framework.BundleContext;  
- 
- publicclass TranslateServiceServlet extends HttpServlet{  
- privatestaticfinallong serialVersionUID = 1L;  
- 
- public TranslateServiceServlet(BundleContext bundleContext) {  
- // TODO Auto-generated constructor stub
-     }  
- 
- @Override
- protectedvoid doGet(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
- this.doPost(req, resp);  
-     }  
- 
- @Override
- protectedvoid doPost(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
- 
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.zyg.osgi.translateclient.servlet;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.osgi.framework.BundleContext;
public class TranslateServiceServlet extends HttpServlet{
	private  static final long serialVersionUID = 1L;
	public TranslateServiceServlet(BundleContext bundleContext) {
		// TODO Auto-generated constructor stub
	}
	@Override
	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		this.doPost(req, resp);
	}
	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
	}
}
```
我们先不写其中的逻辑，我们先写一个最后跳转到的静态页面(html)。
一提到写静态页面，那么问题来了，我们平时写Web项目的时候，我们的静态页面都是写在WebContext或者WebRoot文件夹下的:
![](https://img-blog.csdn.net/20160317182914363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
那么我们这个项目并没有这些目录，那我们怎么去访问我们的静态页面呢？用什么路径去访问呢？是不是感觉和我们之前的开发过程几乎完全不一样，有点不太习惯，不知道从哪里下手？
告诉大家，不！要！慌！虽然使用osgi进行Web开发和之前有很大不同，但是我们无非是把IDE工具中的Web project给我们自动做好的事情，我们手动去完成它而已~~~~
osgi的plug-in和普通web project的不同之处不外乎就是文件的组织夹结构上不太一样。但是如果我们自己手动在工程中创建一个WebContext文件夹，然后里面创建META-INF、WEB-INF和一些jsp文件，形成和web project的静态文件夹组织结构，我们再把这些文件和文件夹的路径给需要加载这个静态文件的程序(例如TomCat，当然我们这里不是TomCat)，不就和之前的web project中的静态文件被自动加载的原理一模一样了吗？所以我们可以这么做，只是需要我们多动一下手去实现它。(说白了，文件夹的摆放只是一个形式，重点在处理路径的程序。我们掌握了处理路径的程序之后，怎么使用我们不一定非要听从第三方的规定，我们按照我们自己喜欢的方式来搞，岂不是更牛逼)
为了方便读取，我们把我们的静态文件直接放在类路径下:
![](https://img-blog.csdn.net/20160317182932363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中html文件的内容是：
**[html]**[view plain](http://blog.csdn.net/acmman/article/details/50916011#)[copy](http://blog.csdn.net/acmman/article/details/50916011#)[print](http://blog.csdn.net/acmman/article/details/50916011#)[?](http://blog.csdn.net/acmman/article/details/50916011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1614054/fork)
- <HTML>
- <HEAD>
- <TITLE>OSGi Dictionary Query Demo</TITLE>
- <metahttp-equiv="Content-Type"content="text/html;charset=GBK"/>
- <script>
-     function queryWord(){  
-         if(document.frmCalc.query_word.value==""){  
-             alert("必须输入单词");  
-             document.frmCalc.query_word.focus();  
-             return;  
-         }  
-         document.frmCalc.submit();  
-     }  
- </script>
- </HEAD>
- 
- <BODY>
- <tablestyle="width:100px;border:1px solid;height:100px">
- <formname="frmCalc"method="post"action="/servlet/translateServlet">
- <trbgcolor="#C0C0C0">
- <tdcolspan="4"align="center">翻译助手</td>
- </tr>
- <tr>
- <td><inputtype="text"name="query_word"size="20"></td>
- <td><inputtype=buttonname=btnSubmitvalue="查询"onclick="queryWord();"></td>
- </tr>
- </form>
- </table>
- </BODY>
- </HTML>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<HTML>
<HEAD>
<TITLE>OSGi Dictionary Query Demo</TITLE>
<meta http-equiv="Content-Type" content="text/html;charset=GBK" /> 
<script>
	function queryWord(){
		if(document.frmCalc.query_word.value==""){
			alert("必须输入单词");
			document.frmCalc.query_word.focus();
			return;
		}
		document.frmCalc.submit();
	}
</script>
</HEAD>
<BODY>
<table style="width:100px;border:1px solid;height:100px">
	<form name="frmCalc" method="post" action="/servlet/translateServlet">
		<tr bgcolor="#C0C0C0">
			<td colspan="4" align="center">翻译助手</td>
		</tr>
		<tr>
			<td><input type="text" name="query_word" size="20"></td>
			<td><input type=button name=btnSubmit value="查询" onclick="queryWord();"></td>
		</tr>
	</form>
</table>
</BODY>
</HTML>
```
好的，我们将我们需要搜索的单词提交上去，那么我们的后台怎么接收呢？
我们之前写Servlet的时候还需要在Web-INF的Web.xml中去注册mapping映射,就像这样:
**[html]**[view plain](http://blog.csdn.net/acmman/article/details/50916011#)[copy](http://blog.csdn.net/acmman/article/details/50916011#)[print](http://blog.csdn.net/acmman/article/details/50916011#)[?](http://blog.csdn.net/acmman/article/details/50916011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1614054/fork)
- <servlet>
- <servlet-name>Add</servlet-name>
- <servlet-class>cn.edu.hpu.test.add</servlet-class>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>Add</servlet-name>
- <url-pattern>/add</url-pattern>
- </servlet-mapping>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<servlet>
    <servlet-name>Add</servlet-name>
    <servlet-class>cn.edu.hpu.test.add</servlet-class>
</servlet>
<servlet-mapping>
    <servlet-name>Add</servlet-name>
    <url-pattern>/add</url-pattern>
</servlet-mapping>
```
但是我们的plug-in项目是没有这种配置文件的，所以我们要来解决这个问题。
首先我们来剖析一下原来的Servlet的mapping映射原理，因为如果我们了解原理之后，我们可以模拟Web-INF的Web.xml中注册mapping映射的原理，去给我们自己的Servlet去注册映射。
开始剖析:
我们之前新建的Servlet我们如果不在Web.xml中做映射配置的话，我们的TomCat服务器是不知道这个Servlet的存在的，也就是我们访问不到这个Servlet的，因为TomCat是通过读取Web.xml文件来实例化Servlet到内存中去的。
我们没有Web.xml文件的话怎么办呢？我们来转换一下思维:我们的Web.xml是要通过程序来读取，然后把里面配置的Servlet给读取到内存中去实例化。归根结底还是需要程序来进行实例化，那么我们为什么不自己去进行实例化呢？我们不要Web.xml配置文件，我们手动在容器中去执行。
那么我们该怎么写这种程序呢？又是一个问题。
如果大家分析过TomCat的源码的话，应该会对这个有一些小小的思路，但是我们这里不会像TomCat这样做的那么严格，我们简单的来实现TomCat读取配置和注册这两个部分的代码，我们要把Servlet注册到容器的服务中去。
我们将这些代码在控制bundle声明周期的Activator类中。
我们首先让Activator首先实现osgi的ServiceListener接口，并重写它的serviceChanged方法，我们才能够通过它来得到jetty服务器给我们提供的各种http服务:
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50916011#)[copy](http://blog.csdn.net/acmman/article/details/50916011#)[print](http://blog.csdn.net/acmman/article/details/50916011#)[?](http://blog.csdn.net/acmman/article/details/50916011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1614054/fork)
- package com.zyg.osgi.translateclient;  
- 
- import org.osgi.framework.BundleActivator;  
- import org.osgi.framework.BundleContext;  
- import org.osgi.framework.ServiceEvent;  
- import org.osgi.framework.ServiceListener;  
- 
- publicclass Activator implements BundleActivator,ServiceListener {  
- 
- privatestatic BundleContext context;  
- 
- static BundleContext getContext() {  
- return context;  
-     }  
- 
- publicvoid start(BundleContext bundleContext) throws Exception {  
-         Activator.context = bundleContext;  
-     }  
- 
- publicvoid stop(BundleContext bundleContext) throws Exception {  
-         Activator.context = null;  
-     }  
- 
- @Override
- publicvoid serviceChanged(ServiceEvent arg0) {  
- // TODO Auto-generated method stub
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.zyg.osgi.translateclient;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceEvent;
import org.osgi.framework.ServiceListener;
public class Activator implements BundleActivator,ServiceListener {
	private static BundleContext context;
	static BundleContext getContext() {
		return context;
	}
	public void start(BundleContext bundleContext) throws Exception {
		Activator.context = bundleContext;
	}
	public void stop(BundleContext bundleContext) throws Exception {
		Activator.context = null;
	}
	@Override
	public void serviceChanged(ServiceEvent arg0) {
		// TODO Auto-generated method stub
	}
}
```
我们在其中编写获取HttpService，注册和注销Servlet以及静态资源的方法:
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50916011#)[copy](http://blog.csdn.net/acmman/article/details/50916011#)[print](http://blog.csdn.net/acmman/article/details/50916011#)[?](http://blog.csdn.net/acmman/article/details/50916011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1614054/fork)
- package com.zyg.osgi.translateclient;  
- 
- import javax.servlet.Servlet;  
- import javax.servlet.ServletException;  
- 
- import org.osgi.framework.BundleActivator;  
- import org.osgi.framework.BundleContext;  
- import org.osgi.framework.ServiceEvent;  
- import org.osgi.framework.ServiceListener;  
- import org.osgi.framework.ServiceReference;  
- import org.osgi.service.http.HttpService;  
- import org.osgi.service.http.NamespaceException;  
- 
- import com.zyg.osgi.translateclient.servlet.TranslateServiceServlet;  
- 
- publicclass Activator implements BundleActivator,ServiceListener {  
- 
- privatestatic BundleContext context;  
- private ServiceReference serviceReference;  
- private Servlet servlet;  
- 
- static BundleContext getContext() {  
- return context;  
-     }  
- 
- publicvoid start(BundleContext bundleContext) throws Exception {  
-         Activator.context = bundleContext;  
- //在TranslateServiceServlet中创建一个构造方法，将bundleContext传进去
-         servlet=new TranslateServiceServlet(bundleContext);  
- //注册Servlet
-         registerServlet();  
-     }  
- 
- publicvoid stop(BundleContext bundleContext) throws Exception {  
- //注销Servlet等资源
-         unRegisterServlet();  
-         serviceReference=null;  
-         Activator.context = null;  
-     }  
- 
- privatevoid registerServlet() {  
- if(null==serviceReference){  
- //通过上下文获取服务对象的“引用”(需要通过MANIFEST.MF引入org.osgi.service.http包)
-             serviceReference=context.getServiceReference(HttpService.class);  
-         }  
- 
- if(null!=serviceReference){  
- //得到http服务对象
-             HttpService httpService=context.getService(serviceReference);  
- if(httpService!=null){  
- try {  
- //注册Servlet
- //四个参数分别是:(映射地址,Servlet类本身,Dictionary,上下文)
-                     httpService.registerServlet("/servlet/translateServlet", servlet, null, null);  
- //注册静态资源(html等文件)位置
- //四个参数分别是(访问地址,静态文件所在路径,上下文)
-                     httpService.registerResources("/page", "pages", null);  
-                     System.out.println("翻译助手服务已启动成功，请通过/page/translate.html访问!");  
-                 } catch (ServletException e) {  
-                     e.printStackTrace();  
-                 } catch (NamespaceException e) {  
-                     e.printStackTrace();  
-                 }  
-             }  
-         }  
-     }  
- 
- privatevoid unRegisterServlet() {  
- if(null!=serviceReference){  
- //得到http服务对象
-             HttpService httpService=context.getService(serviceReference);  
- if(httpService!=null){  
- try {  
- //注销Servlet
- //四个参数分别是:映射地址
-                     httpService.unregister("/servlet/translateServlet");  
- //注销静态资源(html等文件)位置
- //参数是访问地址
-                     httpService.unregister("/page");  
-                     System.out.println("翻译助手服务已停用成功，谢谢使用！");  
-                 } catch (Exception e) {  
-                     e.printStackTrace();  
-                 }  
-             }  
-         }  
- 
- 
-     }  
- 
- @Override
- publicvoid serviceChanged(ServiceEvent event) {  
- // TODO Auto-generated method stub
- switch (event.getType()) {  
- case ServiceEvent.REGISTERED:  
-             registerServlet();  
- break;  
- case ServiceEvent.UNREGISTERING:  
-             unRegisterServlet();  
- break;  
- default:  
- break;  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.zyg.osgi.translateclient;
import javax.servlet.Servlet;
import javax.servlet.ServletException;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceEvent;
import org.osgi.framework.ServiceListener;
import org.osgi.framework.ServiceReference;
import org.osgi.service.http.HttpService;
import org.osgi.service.http.NamespaceException;
import com.zyg.osgi.translateclient.servlet.TranslateServiceServlet;
public class Activator implements BundleActivator,ServiceListener {
	private static BundleContext context;
	private ServiceReference serviceReference;
	private Servlet servlet;
	static BundleContext getContext() {
		return context;
	}
	public void start(BundleContext bundleContext) throws Exception {
		Activator.context = bundleContext;
		//在TranslateServiceServlet中创建一个构造方法，将bundleContext传进去
		servlet=new TranslateServiceServlet(bundleContext);
		//注册Servlet
		registerServlet();
	}
	public void stop(BundleContext bundleContext) throws Exception {
		//注销Servlet等资源
		unRegisterServlet();
		serviceReference=null;
		Activator.context = null;
	}
	private void registerServlet() {
		if(null==serviceReference){
			//通过上下文获取服务对象的“引用”(需要通过MANIFEST.MF引入org.osgi.service.http包)
			serviceReference=context.getServiceReference(HttpService.class);
		}
		
		if(null!=serviceReference){
			//得到http服务对象
			HttpService httpService=context.getService(serviceReference);
			if(httpService!=null){
				try {
					//注册Servlet
					//四个参数分别是:(映射地址,Servlet类本身,Dictionary,上下文)
					httpService.registerServlet("/servlet/translateServlet", servlet, null, null);
					//注册静态资源(html等文件)位置
					//四个参数分别是(访问地址,静态文件所在路径,上下文)
					httpService.registerResources("/page", "pages", null);
					System.out.println("翻译助手服务已启动成功，请通过/page/translate.html访问!");
				} catch (ServletException e) {
					e.printStackTrace();
				} catch (NamespaceException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	private void unRegisterServlet() {
		if(null!=serviceReference){
			//得到http服务对象
			HttpService httpService=context.getService(serviceReference);
			if(httpService!=null){
				try {
					//注销Servlet
					//四个参数分别是:映射地址
					httpService.unregister("/servlet/translateServlet");
					//注销静态资源(html等文件)位置
					//参数是访问地址
					httpService.unregister("/page");
					System.out.println("翻译助手服务已停用成功，谢谢使用！");
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}
	@Override
	public void serviceChanged(ServiceEvent event) {
		// TODO Auto-generated method stub
		switch (event.getType()) {
		case ServiceEvent.REGISTERED:
			registerServlet();
			break;
		case ServiceEvent.UNREGISTERING:
			unRegisterServlet();
			break;
		default:
			break;
		}
	}
}
```
然后编写我们的Servlet:
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50916011#)[copy](http://blog.csdn.net/acmman/article/details/50916011#)[print](http://blog.csdn.net/acmman/article/details/50916011#)[?](http://blog.csdn.net/acmman/article/details/50916011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1614054/fork)
- package com.zyg.osgi.translateclient.servlet;  
- 
- import java.io.IOException;  
- import java.io.PrintWriter;  
- 
- import javax.servlet.ServletException;  
- import javax.servlet.http.HttpServlet;  
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import org.osgi.framework.BundleContext;  
- import org.osgi.framework.ServiceReference;  
- 
- import com.zyg.osgi.translateasslt.service.TranslateService;  
- 
- publicclass TranslateServiceServlet extends HttpServlet{  
- privatestaticfinallong serialVersionUID = 1L;  
- private BundleContext context;  
- 
- public TranslateServiceServlet(BundleContext bundleContext) {  
- this.context=bundleContext;  
-     }  
- 
- @Override
- protectedvoid doGet(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
- this.doPost(req, resp);  
- 
-     }  
- 
- @Override
- protectedvoid doPost(HttpServletRequest req, HttpServletResponse resp)  
- throws ServletException, IOException {  
- //1、取得用户传来的英文单词
-         String queryWord=req.getParameter("query_word");  
- 
- //2、调用方法查找中文含义
- //获取服务
-         TranslateService translateService=null;  
-         ServiceReference serviceRef=context.getServiceReference(TranslateService.class.getName());  
- if(null!=serviceRef){  
-             translateService=(TranslateService)context.getService(serviceRef);  
-         }  
- 
- //3、返回结果给用户
-         resp.setContentType("text/html;charset=GBK");  
-         PrintWriter writer=resp.getWriter();  
- if(translateService==null){  
-             writer.println("没有开放翻译服务！");  
-             writer.close();  
- return;  
-         }  
- 
-         String result=translateService.translate(queryWord);  
-         writer.println("结果"+result);  
-         writer.close();  
- return;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.zyg.osgi.translateclient.servlet;
import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceReference;
import com.zyg.osgi.translateasslt.service.TranslateService;
public class TranslateServiceServlet extends HttpServlet{
	private  static final long serialVersionUID = 1L;
	private BundleContext context;
	public TranslateServiceServlet(BundleContext bundleContext) {
		this.context=bundleContext;
	}
	@Override
	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		this.doPost(req, resp);
		
	}
	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		//1、取得用户传来的英文单词
		String queryWord=req.getParameter("query_word");
		
		//2、调用方法查找中文含义
		//获取服务
		TranslateService translateService=null;
		ServiceReference serviceRef=context.getServiceReference(TranslateService.class.getName());
		if(null!=serviceRef){
			translateService=(TranslateService)context.getService(serviceRef);
		}
		
		//3、返回结果给用户
		resp.setContentType("text/html;charset=GBK");
		PrintWriter writer=resp.getWriter();
		if(translateService==null){
			writer.println("没有开放翻译服务！");
			writer.close();
			return;
		}
		
		String result=translateService.translate(queryWord);
		writer.println("结果"+result);
		writer.close();
		return;
	}
}
```
其中需要通过MANIFEST.MF引入com.zyg.osgi.translateasslt.service包。
同时，我们要在localTranslateAsslt和remoteTranslateAsslt这两个插件的Activator中注册相应的Service，这样我们的TranslateClient就可以使用它们两个提供的Service服务了:
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50916011#)[copy](http://blog.csdn.net/acmman/article/details/50916011#)[print](http://blog.csdn.net/acmman/article/details/50916011#)[?](http://blog.csdn.net/acmman/article/details/50916011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1614054/fork)
- package com.zyg.osgi.localtranslateasslt;  
- 
- import org.osgi.framework.BundleActivator;  
- import org.osgi.framework.BundleContext;  
- import org.osgi.framework.ServiceReference;  
- import org.osgi.framework.ServiceRegistration;  
- 
- import com.zyg.osgi.localtranslateasslt.impls.TranslateServiceLocalImpl;  
- import com.zyg.osgi.translateasslt.service.TranslateService;  
- 
- publicclass Activator implements BundleActivator {  
- 
- privatestatic BundleContext context;  
- private ServiceRegistration<TranslateService> sr;  
- 
- static BundleContext getContext() {  
- return context;  
-     }  
- 
- publicvoid start(BundleContext bundleContext) throws Exception {  
-         Activator.context = bundleContext;  
- //注册Service服务
-         sr=bundleContext.registerService(TranslateService.class,   
- new TranslateServiceLocalImpl(), null);  
-         System.out.println("本地查询服务已启动！");  
-     }  
- 
- publicvoid stop(BundleContext bundleContext) throws Exception {  
-         ServiceReference serviceRef=context.getServiceReference(TranslateService.class.getName());  
-         bundleContext.ungetService(serviceRef);  
-         System.out.println("本地查询服务已停止！");  
-         Activator.context = null;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.zyg.osgi.localtranslateasslt;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceReference;
import org.osgi.framework.ServiceRegistration;
import com.zyg.osgi.localtranslateasslt.impls.TranslateServiceLocalImpl;
import com.zyg.osgi.translateasslt.service.TranslateService;
public class Activator implements BundleActivator {
	private static BundleContext context;
	private ServiceRegistration<TranslateService> sr;
	static BundleContext getContext() {
		return context;
	}
	public void start(BundleContext bundleContext) throws Exception {
		Activator.context = bundleContext;
		//注册Service服务
		sr=bundleContext.registerService(TranslateService.class, 
				new TranslateServiceLocalImpl(), null);
		System.out.println("本地查询服务已启动！");
	}
	public void stop(BundleContext bundleContext) throws Exception {
		ServiceReference serviceRef=context.getServiceReference(TranslateService.class.getName());
		bundleContext.ungetService(serviceRef);
		System.out.println("本地查询服务已停止！");
		Activator.context = null;
	}
}
```
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50916011#)[copy](http://blog.csdn.net/acmman/article/details/50916011#)[print](http://blog.csdn.net/acmman/article/details/50916011#)[?](http://blog.csdn.net/acmman/article/details/50916011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1614054/fork)
- package com.zyg.osgi.remotetranslateasslt;  
- 
- import org.osgi.framework.BundleActivator;  
- import org.osgi.framework.BundleContext;  
- import org.osgi.framework.ServiceReference;  
- import org.osgi.framework.ServiceRegistration;  
- 
- import com.zyg.osgi.remotetranslateasslt.impls.TranslateServiceRemoteImpl;  
- import com.zyg.osgi.translateasslt.service.TranslateService;  
- 
- publicclass Activator implements BundleActivator {  
- privatestatic BundleContext context;  
- private ServiceRegistration<TranslateService> sr;  
- 
- static BundleContext getContext() {  
- return context;  
-     }  
- 
- publicvoid start(BundleContext bundleContext) throws Exception {  
-         Activator.context = bundleContext;  
- //注册Service服务
-         sr=bundleContext.registerService(TranslateService.class,   
- new TranslateServiceRemoteImpl(), null);  
-         System.out.println("远程查询服务已启动！");  
-     }  
- 
- publicvoid stop(BundleContext bundleContext) throws Exception {  
-         ServiceReference serviceRef=context.getServiceReference(TranslateService.class.getName());  
-         bundleContext.ungetService(serviceRef);  
-         System.out.println("远程查询服务已停止！");  
-         Activator.context = null;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.zyg.osgi.remotetranslateasslt;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceReference;
import org.osgi.framework.ServiceRegistration;
import com.zyg.osgi.remotetranslateasslt.impls.TranslateServiceRemoteImpl;
import com.zyg.osgi.translateasslt.service.TranslateService;
public class Activator implements BundleActivator {
	private static BundleContext context;
	private ServiceRegistration<TranslateService> sr;
	static BundleContext getContext() {
		return context;
	}
	public void start(BundleContext bundleContext) throws Exception {
		Activator.context = bundleContext;
		//注册Service服务
		sr=bundleContext.registerService(TranslateService.class, 
				new TranslateServiceRemoteImpl(), null);
		System.out.println("远程查询服务已启动！");
	}
	public void stop(BundleContext bundleContext) throws Exception {
		ServiceReference serviceRef=context.getServiceReference(TranslateService.class.getName());
		bundleContext.ungetService(serviceRef);
		System.out.println("远程查询服务已停止！");
		Activator.context = null;
	}
}
```
好的，目前我们的工作基本完成！现在启动osgi来看看我们的战果把：
![](https://img-blog.csdn.net/20160317183319755?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
说明我们运行成功了，我们来进入浏览器访问一下我们的静态页面:
![](https://img-blog.csdn.net/20160317183330293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这说明我们的Web项目起来了，并且静态资源也访问成功了。
然后我们来查询一下单词，看看我们的Servlet和Service是不是也注册和实例化成功了:
我们先查本地服务的单词“USA”的意思
![](https://img-blog.csdn.net/20160317183426434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160317183441084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
于此同时控制台端显示:
![](https://img-blog.csdn.net/20160317183453100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们再来查询远程服务的单词“cat”的意思
![](https://img-blog.csdn.net/20160317183517897?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160317183525560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
于此同时控制台端显示:
![](https://img-blog.csdn.net/20160317183534592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
出现这种原因的结果是我们两个服务都开启了，但是因为先注册了Local服务后注册了Remote服务，二者服务名相同，所以Remote服务把Local服务给覆盖掉了，那么我们访问本地Local服务的时候，就访问不到相应的数据，访问Remote服务却可以。
大家可以在看控制台出入ss，可以看到远程和本地两个服务都存在:
![](https://img-blog.csdn.net/20160317183548163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们现在stop掉远程服务:
![](https://img-blog.csdn.net/20160317183555327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
因为通过热部署，我们网站不需要重启，我们直接访问远程服务:
发现远程已经不能访问，那么我们访问本地的服务：
![](https://img-blog.csdn.net/20160317183604272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160317183611741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
于此同时控制台端显示:
![](https://img-blog.csdn.net/20160317183619710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
本地服务正常。
当然反过来只用远程的服务也可以。
所以这体现了一种插件“即插即用”，拔除不影响其他组件的性能。
停止服务，关闭osgi的启动，我们需要在控制台中输入close即可:
![](https://img-blog.csdn.net/20160317183641937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上就是我们利用osgi完成的一个服务可插拔的Web项目。
上面是最原生的方式来注册服务等，我们下次会介绍依赖注入的方式来对我们的这个Web项目进行改造。
**转载请注明出处：[http://blog.csdn.net/acmman/article/details/50916011](http://blog.csdn.net/acmman/article/details/50916011)**
