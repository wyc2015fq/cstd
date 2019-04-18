# HtmlUnit爬取页面列表链接 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月19日 11:52:10[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2788








场景：爬虫网页列表上的链接，用于再次爬虫，主要功能是列表翻页功能。

代码参考：



```java
package com;

import com.gargoylesoftware.htmlunit.BrowserVersion;
import com.gargoylesoftware.htmlunit.NicelyResynchronizingAjaxController;
import com.gargoylesoftware.htmlunit.WebClient;
import com.gargoylesoftware.htmlunit.html.DomElement;
import com.gargoylesoftware.htmlunit.html.DomNodeList;
import com.gargoylesoftware.htmlunit.html.HtmlDivision;
import com.gargoylesoftware.htmlunit.html.HtmlElement;
import com.gargoylesoftware.htmlunit.html.HtmlPage;

public class BlogAutoClick {
	public static void main(String[] args){
		String sUrl="IP";//网址  
        //webclient设置  
        final  WebClient webClient = new WebClient(BrowserVersion.CHROME); //创建一个webclient    
        webClient.getOptions().setJavaScriptEnabled(true); // 启动JS            
        webClient.getOptions().setUseInsecureSSL(true);//忽略ssl认证              
        webClient.getOptions().setCssEnabled(false);//禁用Css，可避免自动二次请求CSS进行渲染              
        webClient.getOptions().setThrowExceptionOnScriptError(false);//运行错误时，不抛出异常     
        webClient.getOptions().setThrowExceptionOnFailingStatusCode(false);  
        webClient.setAjaxController(new NicelyResynchronizingAjaxController());// 设置Ajax异步        
        //登录  
        int i_clickCount=0;
        try {  
            HtmlPage page = (HtmlPage) webClient.getPage(sUrl);  
            webClient.waitForBackgroundJavaScript(10000);//等待1秒 
            boolean flag=true;
            while(flag){
            	//本页链接点击
                HtmlDivision articlediv=(HtmlDivision)page.getElementById("article_list");   
                DomNodeList<HtmlElement> articleas=articlediv.getElementsByTagName("a"); 
                for(HtmlElement a:articleas){
                	HtmlPage llpage=(HtmlPage)a.click();
                	DomNodeList<DomElement> h1name= llpage.getElementsByTagName("h1");
                    String strname=h1name.get(0).asText();        	
                	i_clickCount++;
                	System.out.print("article:"+strname+"is auto clicked at times:"+i_clickCount);
                }
                //翻页
                HtmlDivision pagediv=(HtmlDivision)page.getElementById("papelist");
                DomNodeList<HtmlElement> pageas=pagediv.getElementsByTagName("a");
                boolean nextpage=false;
                for(HtmlElement a:pageas){
                	if (a.asText().equals("下一页")) {
                		page=(HtmlPage)a.click();
                		webClient.waitForBackgroundJavaScript(10000);//等待1秒 
                		nextpage=true;//下一页存在
                		break;
                	}
                }
                if(nextpage==false) flag=false;//没有下一页了，退出循环
            }
            
        }catch (Exception e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
        } 
	}
}
```





