# 模拟浏览器自动化测试工具Selenium之三页面窗口切换开发篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月17日 10:41:41[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2898
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









不同网站有不同特性，需要掌握selenim基本网页元素定位以及浏览器控制，本文解析网页主要实现以下操作：

1）模拟鼠标操作，点击按钮提交；2）获取浏览器窗口句柄，切换到当前窗口下操作；3）处理不带总页数的列表页，设计两个变量iDyn和iSta并定位下一页来翻译；4）处理弹出框alert；5）通过对元素定位支持不同网页模板的信息采集，元素下再采集子元素；6）网页内table元素的处理。具体代码如下：



```java
package com.test;

import java.util.List;
import java.util.Set;
import java.util.regex.Pattern;

import org.openqa.selenium.Alert;
import org.openqa.selenium.By;
import org.openqa.selenium.NoAlertPresentException;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.interactions.Actions;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;

import com.util.Logs;

public class Shop114YP {
	public static void main(String[] args) {
		System.getProperties().setProperty("webdriver.chrome.driver","D:\\dev\\workspace\\ocweb\\libs\\chromedriver.exe");	
		WebDriver webDriver = new ChromeDriver();
		WebDriver wd = new ChromeDriver();
		int iCount=0;//统计输出多少
		try {
			webDriver.get("url");//访问网址
			//等待页面加载完毕，直到条件满足  
	        (new WebDriverWait(webDriver, 30)).until(new ExpectedCondition<Boolean>() {  
	            public Boolean apply(WebDriver dr) {  
	                int index = dr.getPageSource().indexOf("keyword");  
	                if(index != -1){   	              	        
	                    return true;  //找到，退出等待
	                }else{  
	                    return false;  //未找到，继续等待
	                }  
	            }  
	        });
	        //通过 id 找到 input的 DOM
	        Actions action=new Actions(webDriver); //模拟鼠标操作
	        WebElement eleKeyword = webDriver.findElement(By.name("keyword"));
	        action.sendKeys(eleKeyword,"内容").perform();
	        //获取窗口当前句柄
	        Set<String> bHandlers = webDriver.getWindowHandles();
	        String bHandler = bHandlers.iterator().next();
	        //执行点击操作，打开一个新的页面
	        WebElement eleSearch = webDriver.findElement(By.className("search_shu4")); 
	        action.moveToElement(eleSearch);  
	        action.click().perform(); //打开一个新页面
	        //获取窗口当前句柄
	        Set<String> nHandlers = webDriver.getWindowHandles();
	        nHandlers.remove(bHandler);//移除先前页面
	        String nHandler = nHandlers.iterator().next();//获取最新打开的句柄
	        webDriver.switchTo().window(nHandler);//切换到新打开的页面
	        //webDriver.switchTo().window(bHandler);//返回原页面
	        (new WebDriverWait(webDriver, 30)).until(new ExpectedCondition<Boolean>() {  
	            public Boolean apply(WebDriver dr) {  
	                int index = dr.getPageSource().indexOf("内容");  
	                if(index != -1) return true;  //找到，退出等待
	                else return false;  //未找到，继续等待              
	            }  
	        });
	        //进入搜索结果页面，第一页
	        int iDyn=1;
	        int iSta=1;      
	        while(iSta==iDyn){
	        	List<WebElement> comList =webDriver.findElements(By.partialLinkText("内容"));
		        for(WebElement ele:comList){//处理每一项
		        	iCount++;
		        	String com=ele.getText();//公司名
	        		String link=ele.getAttribute("href"); 
	        		//if(!link.contains("shopid")) continue;//不包含shopid的链接不要
	        		wd.get(link);        	
	        		try{
	        			Alert alert = wd.switchTo().alert();//判断是否有js弹出框
	        			if (null == alert){//没弹出
	        		        throw new NoAlertPresentException();
	        		    }else {
	        		    	alert.accept();
	        		    	continue;
	        		    }
	        		}catch (NoAlertPresentException e) {
	        		    //System.out.println("There is no alert appear!");
	        		}	
	        		//支持不同网页模板的信息采集
	        		if(wd.getPageSource().contains("lianxi-text")){
	        			//打开页面
			        	List<WebElement> contactList =wd.findElements(By.id("lianxi-text"));
			        	for(WebElement eleCon:contactList){
			        		String txt=eleCon.getText();
			        		com=com+"|"+txt;
			        	}        	
	        		}else if(wd.getPageSource().contains("tablebg")){//网页table处理
	        			WebElement table = wd.findElement(By.className("tablebg")); //获取table元素
	        			List<WebElement> rows = table.findElements(By.tagName("tr"));//获取table内行元素
	        			//List<WebElement> cells=rows.get(0).findElements(By.tagName("td"));//获取row第一行中的所有列
	        			for(WebElement row:rows) com=com+"|"+row.getText();
	        		}else if(wd.getPageSource().contains("table2")){
	        			List<WebElement> cells = wd.findElements(By.tagName("td"));//获取td元素
	        			for(WebElement cell:cells){
	        				if(cell.getAttribute("class").equals("table2") || cell.getAttribute("class").equals("table") ){
	        					com=com+"|"+cell.getText();
	        				}
	        			}
	        		}else if(wd.getPageSource().contains("sidenei")){
	        			WebElement elesn=wd.findElement(By.className("sidenei"));
	        			List<WebElement> eleNeis = elesn.findElements(By.className("nei"));//class等于nei的元素
	        			for(WebElement elenei:eleNeis) com=com+"|"+elenei.getText();
	        		}else if(wd.getPageSource().contains("liebiao")){
	        			WebElement eleLibiao=wd.findElement(By.className("liebiao"));
	        			List<WebElement> eleLbs = eleLibiao.findElements(By.id("li"));//ul下li元素
	        			for(WebElement eleLb:eleLbs) com=com+"|"+eleLb.getText();
	        		}
	        		Logs.writeLogs(com);      		  		  		
		        }
		        //进入下一页，找到下一页链接
		        iDyn++;
		        WebElement pageEle =webDriver.findElement(By.linkText("下一页"));
		        String pagelink=pageEle.getAttribute("href");
		        Pattern ps= Pattern.compile("[?&=]+");
		        String[] values=ps.split(pagelink);
		        iSta=Integer.valueOf(values[values.length-1]);
		        webDriver.get(pagelink);
		        (new WebDriverWait(webDriver, 30)).until(new ExpectedCondition<Boolean>() {  
		            public Boolean apply(WebDriver dr) {  
		                int index = dr.getPageSource().indexOf("内容"); 
		                if(index != -1) return true;  //找到，退出等待
		                else return false;  //未找到，继续等待              
		            }  
		        });        
	        }       
		}catch (Exception e) { System.err.println( "Exception: " + e );}
		System.out.println(iCount);
		wd.close();
		wd.quit();
		webDriver.close();
        webDriver.quit();
	}
}
```







