# 模拟浏览器自动化测试工具Selenium之二Html基本元素开发篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月16日 10:59:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2462








发现用IE浏览器，有很多动态网站加载错误，只好安装chrome浏览器，然后下载chrome driver来驱动。通过selenium的基本元素定位操作来和网页交互。

网页解析主要动作：1）表单自动填写和提交；2）处理带有总页数的翻页及每页列表；3）对网页具体元素内的问题进行正则匹配采集信息。

具体代码参考如下：



```java
package com.test;

import java.io.BufferedReader;
import java.io.StringReader;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;

import com.util.Logs;

public class BrasShopYP {
	public static void main(String[] args) {
		try {
			System.getProperties().setProperty("webdriver.chrome.driver","D:\\tmp\\chromedriver.exe");
			WebDriver webDriver = new ChromeDriver();
			//访问网址
			webDriver.get("URL");
			//等待页面加载完毕，直到条件满足  
	        (new WebDriverWait(webDriver, 30)).until(new ExpectedCondition<Boolean>() {  
	            public Boolean apply(WebDriver dr) {  
	                int index = dr.getPageSource().indexOf("id");  
	                if(index != -1){   	              	        
	                    return true;  //找到，退出等待
	                }else{  
	                    return false;  //未找到，继续等待
	                }  
	            }  
	        });
	        //通过 id 找到 input的 DOM
	        WebElement element = webDriver.findElement(By.id("name"));
	         // 输入关键字
	        element.sendKeys("内容");
	         // 提交 input所在的  form
	        element.submit(); 
	        //等待页面加载完毕
	        (new WebDriverWait(webDriver, 30)).until(new ExpectedCondition<Boolean>() {  
	            public Boolean apply(WebDriver dr) {  
	                int index = dr.getPageSource().indexOf("name");  
	                if(index != -1) return true;  //找到，退出等待
	                else return false;  //未找到，继续等待              
	            }  
	        });
	        //进入搜索结果页面，先获取总页数
	        WebElement eleDivTotal =webDriver.findElement(By.cssSelector("div.total.ng-binding"));
	        int iTotal=Integer.valueOf(eleDivTotal.getText().substring(1, 3));
	        for(int i=1;i<=iTotal;i++){//处理每一页
	        	List<WebElement> comList =webDriver.findElements(By.partialLinkText("内容"));
	        	for(WebElement ele:comList){//处理每一项
	        		String link=ele.getAttribute("href");   
	        		WebDriver wd = new ChromeDriver();
	        		wd.get(link);
	        		//等待页面加载完毕
	    	        (new WebDriverWait(wd, 50)).until(new ExpectedCondition<Boolean>() {  
	    	            public Boolean apply(WebDriver dr) {  
	    	                int index = dr.getPageSource().indexOf("name");  
	    	                if(index != -1) return true;  //找到，退出等待
	    	                else  return false;  //未找到，继续等待	    	      
	    	            }  
	    	        });
	    	        //定位公司名
	    	        WebElement elecom =wd.findElement(By.tagName("p"));
                    //定位电话和邮箱
	    	        BufferedReader reader = new BufferedReader(new StringReader(wd.getPageSource()));
	    	        String strTel="null"; 
	    	        while ( reader.readLine()!= null) {//定位电话
	    	        	String line = reader.readLine();
	    	        	if(line !=null){
	    	        		String regex="^1[3|4|5|8][0-9]\\d{8}$";//手机正则表达式
	    	        		Pattern r = Pattern.compile(regex);// 创建 Pattern 对象	    
	    		    		Matcher m = r.matcher(line.trim());// 创建 matcher 对象  
	    		    		if(m.find()) {//满足正则表达式
	    		    			strTel=line.trim();
	    		    			break;
	    		    		}
	    	        	}
	    	        }
	    	        //定位邮箱
	    	        String strEmail="null";
	    	        while ( reader.readLine()!= null) {//定位邮箱
	    	        	String line = reader.readLine();
	    	        	if(line !=null){
	    	        		String regex="^[a-zA-Z_]{1,}[0-9]{0,}@(([a-zA-z0-9]-*){1,}\\.){1,3}[a-zA-z\\-]{1,}$";//email正则表达式
	    	        		Pattern r = Pattern.compile(regex);// 创建 Pattern 对象	    
	    		    		Matcher m = r.matcher(line.trim());// 创建 matcher 对象  
	    		    		if(m.find()) {//满足正则表达式
	    		    			strEmail=line.trim();
	    		    			break;
	    		    		}
	    	        	}
	    	        }  	
                    //输出公司|电话|邮箱
	    	        Logs.writeLogs(elecom.getText()+"|"+strTel+"|"+strEmail);
	        		wd.close();//关闭浏览器窗口
	        		wd.quit();//关闭chrome Driver进程；
	        	}
	        	//处理下一页
	        	if(i==iTotal) break;
	        	WebElement elePage = webDriver.findElement(By.linkText(String.valueOf(i+1)));
	        	webDriver.navigate().to(elePage.getAttribute("href"));
	        	//等待页面加载完毕
		        (new WebDriverWait(webDriver, 30)).until(new ExpectedCondition<Boolean>() {  
		            public Boolean apply(WebDriver dr) {  
		                int index = dr.getPageSource().indexOf("b-c-white search_result_container");  
		                if(index != -1) return true;  //找到，退出等待
		                else return false;  //未找到，继续等待              
		            }  
		        });
	        }   
			// 关闭窗口，释放资源。
			webDriver.close();//关闭浏览器窗口
			webDriver.quit();//关闭chrome Driver进程；
		}catch (Exception e) {System.err.println( "Exception: " + e ); }
	 }
}

//参考：http://blog.csdn.net/wx19900503/article/details/47169107
//参考：http://www.cnblogs.com/TankXiao/p/5222238.html
```







