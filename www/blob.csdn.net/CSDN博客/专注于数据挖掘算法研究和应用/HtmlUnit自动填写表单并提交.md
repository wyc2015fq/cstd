# HtmlUnit自动填写表单并提交 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月06日 08:18:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6195








场景：对网页的表单进行自动填写并提交，如登录框，和文章http://blog.csdn.net/fjssharpsword/article/details/51480475类似，不同的是这次button按钮没有id值和name值，只能通过value来获取。

Htmlunit官网：http://htmlunit.sourceforge.net/ 可下载包

代码：



```java
package ebay;

import java.io.IOException;
import java.net.MalformedURLException;

import com.gargoylesoftware.htmlunit.BrowserVersion;
import com.gargoylesoftware.htmlunit.FailingHttpStatusCodeException;
import com.gargoylesoftware.htmlunit.NicelyResynchronizingAjaxController;
import com.gargoylesoftware.htmlunit.WebClient;
import com.gargoylesoftware.htmlunit.html.DomElement;
import com.gargoylesoftware.htmlunit.html.DomNodeList;
import com.gargoylesoftware.htmlunit.html.HtmlButton;
import com.gargoylesoftware.htmlunit.html.HtmlButtonInput;
import com.gargoylesoftware.htmlunit.html.HtmlForm;
import com.gargoylesoftware.htmlunit.html.HtmlPage;
import com.gargoylesoftware.htmlunit.html.HtmlPasswordInput;
import com.gargoylesoftware.htmlunit.html.HtmlSubmitInput;
import com.gargoylesoftware.htmlunit.html.HtmlTextInput;

public class GGManage {
	public static void main(String[] args){
		String sUrl="IP";//网址
		//webclient设置
		WebClient webClient = new WebClient(BrowserVersion.CHROME); //创建一个webclient  
		webClient.getOptions().setJavaScriptEnabled(true); // 启动JS	    	
		webClient.getOptions().setUseInsecureSSL(true);//忽略ssl认证	    	
		webClient.getOptions().setCssEnabled(false);//禁用Css，可避免自动二次请求CSS进行渲染	    	
		webClient.getOptions().setThrowExceptionOnScriptError(false);//运行错误时，不抛出异常	 
		webClient.getOptions().setThrowExceptionOnFailingStatusCode(false);
		webClient.setAjaxController(new NicelyResynchronizingAjaxController());// 设置Ajax异步  	
		//登录
		try {
			HtmlPage page = (HtmlPage) webClient.getPage(sUrl);
			HtmlForm form=page.getForms().get(0);//page.getFormByName("");
			HtmlTextInput  txtUName = (HtmlTextInput )form.getInputByName("UserName"); //用户名text框
			txtUName.setValueAttribute("name");
	        HtmlPasswordInput txtPwd = (HtmlPasswordInput)form.getInputByName("Password");//密码框
	        txtPwd.setValueAttribute("password");
	        //submit没有name，只有class和value属性，通过value属性定位元素
	        HtmlSubmitInput submit=(HtmlSubmitInput) form.getInputByValue("登 录");
	        page = (HtmlPage) submit.click();//登录进入
	        webClient.waitForBackgroundJavaScript(10000);//等待1秒
	        
	        System.out.println(page.asText());
		}catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
```





