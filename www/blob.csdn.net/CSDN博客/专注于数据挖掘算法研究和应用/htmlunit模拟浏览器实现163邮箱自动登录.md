# htmlunit模拟浏览器实现163邮箱自动登录 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年05月23日 11:46:44[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9827








对于表单隐藏在js内，需要解析js并动态加载网页，找了很多组件，包括httpunit，后来试验htmlunit通过。感觉httpunit还是比htmlunit差点，所以建议用htmlunit来解析js动态加载的网页并自动提交表单。对于模拟浏览器的htmlunit组件还是有很多需要挖掘，这样才能试验不同网站的通用性解析。

重点是：Htmlunit版本会爆出如sslcontext以及httpclient版本不兼容问题，换了3个版本，最后2.18版本可以用。

htmlunit基础代码如下：



```java
package gddx;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import com.gargoylesoftware.htmlunit.BrowserVersion;
import com.gargoylesoftware.htmlunit.WebClient;
import com.gargoylesoftware.htmlunit.html.HtmlButton;
import com.gargoylesoftware.htmlunit.html.HtmlForm;
import com.gargoylesoftware.htmlunit.html.HtmlPage;
import com.gargoylesoftware.htmlunit.html.HtmlPasswordInput;
import com.gargoylesoftware.htmlunit.html.HtmlTextInput;
import com.gargoylesoftware.htmlunit.util.Cookie;

public class Email163Login {
	public static void main(String[] args) {
		try {
			//创建一个webclient
			WebClient webClient = new WebClient(BrowserVersion.CHROME); 
			
			//参数设置
			// 1 启动JS
			webClient.getOptions().setJavaScriptEnabled(true);
			// 2 禁用Css，可避免自动二次请求CSS进行渲染
			webClient.getOptions().setCssEnabled(false);
			//3 启动客户端重定向
			webClient.getOptions().setRedirectEnabled(true);
			// 4 运行错误时，是否抛出异常
			webClient.getOptions().setThrowExceptionOnScriptError(false);
			// 5 设置超时
			webClient.getOptions().setTimeout(50000);
			//6 设置忽略证书
			//webClient.getOptions().setUseInsecureSSL(true);
			//7 设置Ajax
			//webClient.setAjaxController(new NicelyResynchronizingAjaxController());
			//8设置cookie
			webClient.getCookieManager().setCookiesEnabled(true);
			
			//获取页面
			HtmlPage page = webClient.getPage("http://mail.163.com/");  
			// 根据form的名字获取页面表单，也可以通过索引来获取：page.getForms().get(0)   
	        HtmlForm form = page.getFormByName("login163");       
	        HtmlTextInput username = (HtmlTextInput) form.getInputByName("username");
	        HtmlPasswordInput password = (HtmlPasswordInput) form.getInputByName("password");
	        username.setValueAttribute("sharpsword");
	        password.setValueAttribute("xyz");
	        HtmlButton button =(HtmlButton)page.getHtmlElementById("loginBtn");   
	        HtmlPage retPage = (HtmlPage) button.click();
			// 等待JS驱动dom完成获得还原后的网页
			webClient.waitForBackgroundJavaScript(10000);
			//输出网页内容
			System.out.println(retPage.asXml()); 
			//获取cookie
			Set<Cookie> cookies = webClient.getCookieManager().getCookies();; 
			Map<String, String> responseCookies = new HashMap<String, String>();
			for (Cookie c : cookies) {
				responseCookies.put(c.getName(), c.getValue());
				System.out.print(c.getName()+":"+c.getValue());
			}
			
			//关闭webclient
			webClient.close(); 	
			}catch (Exception e) {System.err.println( "Exception: " + e ); }
	}
}
```







