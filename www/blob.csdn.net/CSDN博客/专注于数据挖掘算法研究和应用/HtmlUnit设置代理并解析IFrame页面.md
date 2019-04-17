# HtmlUnit设置代理并解析IFrame页面 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月06日 16:37:36[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5979








1、需求：支持代理设置访问，并采集网页下iframe框架内的页面内容

2、参考代码如下：



```java
package com;

import java.io.FileInputStream;

import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

import com.gargoylesoftware.htmlunit.BrowserVersion;
import com.gargoylesoftware.htmlunit.NicelyResynchronizingAjaxController;
import com.gargoylesoftware.htmlunit.ProxyConfig;
import com.gargoylesoftware.htmlunit.WebClient;
import com.gargoylesoftware.htmlunit.html.DomNodeList;
import com.gargoylesoftware.htmlunit.html.HtmlElement;
import com.gargoylesoftware.htmlunit.html.HtmlPage;

public class EBayHU {
	public static void main(String[] args) {
		//打开excel表，准备采集入表
		try { 	
			//创建一个webclient
	        WebClient webClient = new WebClient(BrowserVersion.CHROME);               
	        // 启动JS
	    	webClient.getOptions().setJavaScriptEnabled(true); 
	    	//忽略ssl认证
	    	webClient.getOptions().setUseInsecureSSL(true);
	    	//禁用Css，可避免自动二次请求CSS进行渲染
	    	webClient.getOptions().setCssEnabled(false);
	    	//运行错误时，不抛出异常
			webClient.getOptions().setThrowExceptionOnScriptError(false);
	    	// 设置Ajax异步
		    webClient.setAjaxController(new NicelyResynchronizingAjaxController());
	    	//设置代理
	    	ProxyConfig proxyConfig = webClient.getOptions().getProxyConfig(); 
	    	proxyConfig.setProxyHost("IP");  
	        proxyConfig.setProxyPort(port);
	        //获取页面
	        HtmlPage page = webClient.getPage("url"); 
	        webClient.waitForBackgroundJavaScript(10000);
	        //商品标题
	        HtmlElement itemTitle =page.getHtmlElementById("itemTitle");
	        System.out.println(itemTitle.asText());
	        //商品图片
	        HtmlElement propic =page.getHtmlElementById("vi_main_img_fs");
	        DomNodeList<HtmlElement> picnodes=propic.getElementsByTagName("img");
	        for(int m=0;m<picnodes.size();m++){
	    		HtmlElement pic=picnodes.get(m);
	    		page=(HtmlPage)pic.click();
	    		webClient.waitForBackgroundJavaScript(10000);
	    		HtmlElement bigpic =page.getHtmlElementById("icImg");
	    		String bigpicsrc=bigpic.getAttribute("src");
	    		System.out.println(bigpicsrc);
	        }
	        //卖家信息
	        HtmlElement seller =page.getHtmlElementById("mbgLink");
	        String href=seller.getAttribute("href");
	        System.out.println(href);
	        System.out.println(seller.asText());
	        //商品详情
	        HtmlElement descifr =page.getHtmlElementById("desc_ifr");//切换到iframe
	        String src=descifr.getAttribute("src");
	        HtmlPage ifrpage=webClient.getPage(src);//读取iframe网页
	        webClient.waitForBackgroundJavaScript(10000);
	        HtmlElement desc =ifrpage.getHtmlElementById("desc");
	        System.out.println(desc.asText());
		}catch (Exception e) {System.err.println( "Exception: " + e ); }
	}
}
```





