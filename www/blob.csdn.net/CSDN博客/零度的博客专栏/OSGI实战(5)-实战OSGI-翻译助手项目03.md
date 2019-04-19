# OSGI实战(5)-实战OSGI-翻译助手项目03 - 零度的博客专栏 - CSDN博客
2017年01月17日 11:43:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：366
                
我们上次开发了包含本地服务和远程服务两个插件的pulg-in项目，并且学习了如何注册和获取Service，如何绑定Http服务，如何绑定Servlet服务。
接下来我们依旧使用osgi技术，将项目改为使用Spring注入技术的项目。
说到这里不得不介绍一下Spring-DM:
Spring-DM指的是 Spring Dynamic Modules。Spring-DM 的主要目的是能够方便地将 Spring 框架和OSGi框架结合在一起，使得使用Spring的应用程序可以方便简单地部署在OSGi环境中，利用OSGi框架提供的服务，将应用变得 更加模块化。
所以我们接下来要做的就是Spring-DM与OSGI的结合。
可以从 Spring 的网站上下载最新的 Spring-DM 包。目前最新的版本为 1.2.0 RC1，下载地址在:
[http://download.csdn.net/detail/u013517797/9463540](http://download.csdn.net/detail/u013517797/9463540)
下载了这个Spring-DM 包后，我们把压缩包中的dist和lib目录解压到硬盘上的某个目录，比如解压到D盘根目录下的 spring-dm 目录中。那么我们会在D:\Spring-dm\dist目录下看到如下图所示的内容。
![](https://img-blog.csdn.net/20160320100451944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在D:\spring-dm\lib中看到如下图所示的内容。
![](https://img-blog.csdn.net/20160320100501795?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后我们要在我们的项目中引入需要的Spring插件，我们点击import,然后选择 Import Plugin-ins and Fragments:
![](https://img-blog.csdn.net/20160320100511539?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后做一下设置
我们要设置 Plug-in Location，先设置为 D:\spring-dm\dist，导入 sprimg-dm 的包:
![](https://img-blog.csdn.net/20160320100519920?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击 Next 后，出现了让我们选择导入的Plugin界面
![](https://img-blog.csdn.net/20160320100528202?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们可以导入 core、extender、io 三个 Bundle。
完成后可以在Eclipse的工作区看到引入的三个插件:
![](https://img-blog.csdn.net/20160320100536311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们直接在 Run Configurations 中选择这三个 Bundle，并执行，会发现三个 Bundle 都是INSTALLED状态，如果我们启动Bundle，会报错，原因是我们没有加入这三个Bundle所依赖的Bundle。而这些Bundle，也就在D:\spring-dm\lib目录下。我们用和前面一样的方式来导入lib中所需要的Bundle。
要 导 入 的 Bundle 是 
**com.springsource.org.aopallianceorg.springframework.aoporg.springframework.beansorg.springframework.context org.springframework.context.supportorg.springframework.core**
导入完之后我们在Eclipse的工作区看到引入的插件:
![](https://img-blog.csdn.net/20160320100558264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们再次配置一下Run Configurations:
![](https://img-blog.csdn.net/20160320100607702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160320100620296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(单纯的，我们自己的加了更多需要的插件)
然后我们将此环境运行起来，然后出现如下画面就是我们的环境配置成功:
![](https://img-blog.csdn.net/20160320100635280?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后我们输入ss指令，发现我们的环境中的spring插件全部ACTIVE:
![](https://img-blog.csdn.net/20160320100646181?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们的环境配置完成之后，接下来我们开始改造我们的翻译助手项目。
首先改造我们的本地查询服务，我们在localTranslateAsslt的MANIFEST.MF导入我们需要的插件依赖: 
**javax.servlet;javax.servlet.http;org.osgi.service.http;**
![](https://img-blog.csdn.net/20160320100658108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后将本工程下的Activator类删除(我们接下来将不通过Activator来注入Service和获取Service了，我们通过spring的注入来注册和获取相应的服务)，并在MANIFEST.MF中删除Bundle-Activator:选项。
![](https://img-blog.csdn.net/20160320100713026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后我们在工程下创建一个名为“OSGI-INF”的文件夹，然后创建一个名为“component.xml”的xml配置文件:
然后在MANIFEST.MF中添加Service-Component: OSGI-INF/component.xml这句话(如果报"Header must be terminated by a line break"的错，在上面那个配置末尾加一个回车就好了)。
这个文件就是配置我们该插件项目注册的Service，其中的内容为:
**[html]**[view plain](http://blog.csdn.net/acmman/article/details/50935373#)[copy](http://blog.csdn.net/acmman/article/details/50935373#)[print](http://blog.csdn.net/acmman/article/details/50935373#)[?](http://blog.csdn.net/acmman/article/details/50935373#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1616864/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <componentname="TranslateService">
- <implementationclass="com.zyg.osgi.localtranslateasslt.impls.TranslateServiceLocalImpl"/>
- <service>
- <provideinterface="com.zyg.osgi.translateasslt.service.TranslateService"/>
- </service>
- </component>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>
<component name="TranslateService">
<implementation class="com.zyg.osgi.localtranslateasslt.impls.TranslateServiceLocalImpl"/>
	<service>
		<provide interface="com.zyg.osgi.translateasslt.service.TranslateService"/>
	</service>
</component>
```
可以看到，我们注册了一个名为"TranslateService"的Service，这个Service的实例化类为"com.zyg.osgi.localtranslateasslt.impls.TranslateServiceLocalImpl"。
同理，我们在远程服务插件中做相同的操作，先加入相应的依赖(javax.servlet;javax.servlet.http;org.osgi.service.http;)，然后删除Activator类，然后在MANIFEST.MF中删除Bundle-Activator:选项，并创建OSGI-INF的文件夹与component.xml配置文件，然后在MANIFEST.MF中添加Service-Component: OSGI-INF/component.xml这句话。接着在配置文件中写入如下配置:
**[html]**[view plain](http://blog.csdn.net/acmman/article/details/50935373#)[copy](http://blog.csdn.net/acmman/article/details/50935373#)[print](http://blog.csdn.net/acmman/article/details/50935373#)[?](http://blog.csdn.net/acmman/article/details/50935373#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1616864/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <componentname="TranslateService">
- <implementationclass="com.zyg.osgi.remotetranslateasslt.impls.TranslateServiceRemoteImpl"/>
- <service>
- <provideinterface="com.zyg.osgi.translateasslt.service.TranslateService"/>
- </service>
- </component>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>
<component name="TranslateService">
<implementation class="com.zyg.osgi.remotetranslateasslt.impls.TranslateServiceRemoteImpl"/>
	<service>
		<provide interface="com.zyg.osgi.translateasslt.service.TranslateService"/>
	</service>
</component>
```
最后我们修改我们的Web入口插件TranslateClient，先加入相应的依赖(javax.servlet;javax.servlet.http;org.osgi.service.http;)，然后删除其Activator类，然后在MANIFEST.MF中删除Bundle-Activator:选项，并创建OSGI-INF的文件夹与component.xml配置文件，然后在MANIFEST.MF中添加Service-Component: OSGI-INF/component.xml这句话。接着在component.xml写入以下配置:
**[html]**[view plain](http://blog.csdn.net/acmman/article/details/50935373#)[copy](http://blog.csdn.net/acmman/article/details/50935373#)[print](http://blog.csdn.net/acmman/article/details/50935373#)[?](http://blog.csdn.net/acmman/article/details/50935373#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1616864/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <componentname="TranslateServlet">
- <implementationclass="com.zyg.osgi.translateclient.servlet.TranslateServiceServlet"/>
- <referencename="TranslateService"interface="com.zyg.osgi.translateasslt.service.TranslateService"bind="setTranslateService"unbind="unsetTranslateService"policy="dynamic"cardinality="0..1"/>
- <referencename="HttpService"interface="org.osgi.service.http.HttpService"bind="setHttpService"unbind="unsetHttpService"policy="dynamic"/>
- </component>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>
<component name="TranslateServlet">
	<implementation class="com.zyg.osgi.translateclient.servlet.TranslateServiceServlet"/>
	<reference name="TranslateService" interface="com.zyg.osgi.translateasslt.service.TranslateService" bind="setTranslateService" unbind="unsetTranslateService" policy="dynamic" cardinality="0..1"/>
	<reference name="HttpService" interface="org.osgi.service.http.HttpService" bind="setHttpService" unbind="unsetHttpService" policy="dynamic"/>
</component>
```
可以看到我们配置了Servlet和Servlet的实现，然后里面的参数是Servlet依赖的类，分别是TranslateService和HttpService。
然后我们修改一下TranslateServiceServlet，让其能够通过配置文件注入TranslateService和HttpService:
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50935373#)[copy](http://blog.csdn.net/acmman/article/details/50935373#)[print](http://blog.csdn.net/acmman/article/details/50935373#)[?](http://blog.csdn.net/acmman/article/details/50935373#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1616864/fork)
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
- import org.osgi.service.http.HttpService;  
- 
- import com.zyg.osgi.translateasslt.service.TranslateService;  
- 
- publicclass TranslateServiceServlet extends HttpServlet{  
- privatestaticfinallong serialVersionUID = 1L;  
- 
- private HttpService httpService;  
- 
- private TranslateService translateService;  
- 
- /**
-      * 注入HttpService
-      * @param httpService
-      */
- publicvoid setHttpService(HttpService httpService){  
- try {  
-             httpService.registerServlet("/servlet/translateServlet", this, null, null);  
-             httpService.registerResources("/page", "pages", null);  
-             System.out.println("翻译助手服务已启动成功，请通过/page/translate.html访问!");  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- /**
-      * 注销HttpService
-      * @param httpService
-      */
- publicvoid unsetHttpService(HttpService httpService){  
- if(httpService != this.httpService){  
- return;  
-         }  
- 
- try {  
-             httpService.unregister("/servlet/translateServlet");  
-             httpService.unregister("/page");  
-             System.out.println("翻译助手服务已停用成功，谢谢使用！");  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
- 
- this.httpService = null;  
-     }  
- 
- /**
-      * 注入TranslateService
-      * @param translateService
-      */
- publicvoid setTranslateService(TranslateService translateService){  
- this.translateService = translateService;  
-     }  
- 
- /**
-      * 注销TranslateService
-      * @param translateService
-      */
- publicvoid unsetTranslateService(TranslateService translateService){  
- if(translateService != this.translateService){  
- return;  
-         }  
- this.translateService = null;  
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
- //1、取得用户传来的英文单词
-         String queryWord=req.getParameter("query_word");  
- 
- //2、调用方法查找中文含义
-         resp.setContentType("text/html;charset=GBK");  
-         PrintWriter writer=resp.getWriter();  
- if(translateService==null){  
-             writer.println("没有开放翻译服务！");  
-             writer.close();  
- return;  
-         }  
-         String result=translateService.translate(queryWord);  
- 
- //3、返回结果给用户
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
import org.osgi.service.http.HttpService;
import com.zyg.osgi.translateasslt.service.TranslateService;
public class TranslateServiceServlet extends HttpServlet{
	private  static final long serialVersionUID = 1L;
	private HttpService httpService;
	
	private TranslateService translateService;
	
	/**
	 * 注入HttpService
	 * @param httpService
	 */
	public void setHttpService(HttpService httpService){
		try {
			httpService.registerServlet("/servlet/translateServlet", this, null, null);
			httpService.registerResources("/page", "pages", null);
			System.out.println("翻译助手服务已启动成功，请通过/page/translate.html访问!");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * 注销HttpService
	 * @param httpService
	 */
	public void unsetHttpService(HttpService httpService){
		if(httpService != this.httpService){
			return;
		}
		
		try {
			httpService.unregister("/servlet/translateServlet");
			httpService.unregister("/page");
			System.out.println("翻译助手服务已停用成功，谢谢使用！");
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		this.httpService = null;
	}
	
	/**
	 * 注入TranslateService
	 * @param translateService
	 */
	public void setTranslateService(TranslateService translateService){
		this.translateService = translateService;
	}
	
	/**
	 * 注销TranslateService
	 * @param translateService
	 */
	public void unsetTranslateService(TranslateService translateService){
		if(translateService != this.translateService){
			return;
		}
		this.translateService = null;
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
		resp.setContentType("text/html;charset=GBK");
		PrintWriter writer=resp.getWriter();
		if(translateService==null){
			writer.println("没有开放翻译服务！");
			writer.close();
			return;
		}
		String result=translateService.translate(queryWord);
		
		//3、返回结果给用户
		writer.println("结果"+result);
		writer.close();
		return;
	}
}
```
我们配置一下Run Configuration，在之前的基础上，将我们的项目的四个部分全部引入进去(localTranslateAsslt、remoteTranslateAsslt、TranslateAsslt、TranslateClient)
![](https://img-blog.csdn.net/20160320100924234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果出现java.lang.ClassNotFoundException:等错误，应在Target platform中加入等工程中需要的依赖插件。
我们现在运行一下我们的项目:
![](https://img-blog.csdn.net/20160320100939889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
说明我们的项目运行成功，那么使用ss指令发现我们的插件都是运行正常 的，那么我们先停掉我们的远程服务
![](https://img-blog.csdn.net/20160320100949031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
测试一下本地服务:
![](https://img-blog.csdn.net/20160320100959375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160320101008015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160320101017172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160320101026032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到本地服务查询是正常的，远程服务查询是停掉的。
然后停掉我们的本地服务，启动远程服务:
![](https://img-blog.csdn.net/20160320101040735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
测试一下远程服务:
![](https://img-blog.csdn.net/20160320101107094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160320101116688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160320101126235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160320101136782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到远程服务查询是正常的，本地服务查询是停掉的。
至此，我们的OSGI和Spring-DM整合成功。
**转载请注明出处：[http://blog.csdn.net/acmman/article/details/50935373](http://blog.csdn.net/acmman/article/details/50935373)**
