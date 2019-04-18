# HtmlUnit动态执行js函数 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月11日 15:54:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：8459








功能：动态指定js函数，实际上这个效果不是很有用，这里就做一个记录而已，参考代码如下：



```java
package com;
import com.gargoylesoftware.htmlunit.BrowserVersion;

import com.gargoylesoftware.htmlunit.ScriptResult;
import com.gargoylesoftware.htmlunit.WebClient;
import com.gargoylesoftware.htmlunit.html.DomElement;
import com.gargoylesoftware.htmlunit.html.DomNodeList;
import com.gargoylesoftware.htmlunit.html.HtmlAnchor;
import com.gargoylesoftware.htmlunit.html.HtmlElement;
import com.gargoylesoftware.htmlunit.html.HtmlPage;
import com.gargoylesoftware.htmlunit.javascript.host.event.Event;
public class EBayHU {
	public static void main(String[] args) {
WebClient webClient = new WebClient(BrowserVersion.CHROME); //创建一个webclient           	        
    	webClient.getOptions().setJavaScriptEnabled(true); // 启动JS	    	
    	webClient.getOptions().setUseInsecureSSL(true);//忽略ssl认证	    	
    	webClient.getOptions().setCssEnabled(false);//禁用Css，可避免自动二次请求CSS进行渲染	    	
		webClient.getOptions().setThrowExceptionOnScriptError(false);//运行错误时，不抛出异常	    	
	    webClient.setAjaxController(new NicelyResynchronizingAjaxController());// 设置Ajax异步
	    try {
	    	HtmlPage page=webClient.getPage("url");
    		webClient.waitForBackgroundJavaScript(10000);
    		HtmlElement propic =page.getHtmlElementById("viEnlargeImgLayer_layer_fs");
	        DomNodeList<HtmlElement> picnodes=propic.getElementsByTagName("img");
	        for(int m=0;m<picnodes.size();m++){
	    		HtmlElement pic=picnodes.get(m);
	    		System.out.println(pic.asXml());
	    		//响应事件
	    		//Event event=new Event();
				//event.setEventType(Event.TYPE_KEY_DOWN);
				//HtmlPage hpm=pic.click(event); //bpic.fireEvent(event);
	    		//执行js	
	    		String hrefValue = "javascript:add(1,1,'+');";
	    		ScriptResult s = page.executeJavaScript(hrefValue);//执行js方法
	    		HtmlPage hpm=(HtmlPage) s.getNewPage();//获得执行后的新page对象    	
	    		webClient.waitForBackgroundJavaScript(10000);
	    		HtmlElement lpic=hpm.getHtmlElementById("viEnlargeImgLayer_img_ctr");
	    		System.out.println(lpic.asXml());	    			   		
	        } 
    		
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	 
	}
```





