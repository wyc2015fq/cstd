# 模拟浏览器自动化测试工具Selenium之六设置代理篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年02月22日 17:36:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：7762








在使用Selenium自动化测试时，如果需要设置代理访问网络时，可以参考如下代码：



```java
package com.test;

import java.util.List;
import org.openqa.selenium.By;
import org.openqa.selenium.Proxy;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.remote.CapabilityType;
import org.openqa.selenium.remote.DesiredCapabilities;

public class EBayCom {
	public static void main(String[] args) {
		System.getProperties().setProperty("webdriver.chrome.driver","D:\\dev\\workspace\\ocweb\\libs\\chromedriver.exe");	
		WebDriver webDriver = new ChromeDriver();
		//设置代理
		String proxyIpAndPort= "ip:post";
		DesiredCapabilities cap = new DesiredCapabilities();
		Proxy proxy=new Proxy();
		proxy.setHttpProxy(proxyIpAndPort).setFtpProxy(proxyIpAndPort).setSslProxy(proxyIpAndPort);
		cap.setCapability(CapabilityType.ForSeleniumServer.AVOIDING_PROXY, true);
		cap.setCapability(CapabilityType.ForSeleniumServer.ONLY_PROXYING_SELENIUM_TRAFFIC, true);
		System.setProperty("http.nonProxyHosts", "localhost");
		cap.setCapability(CapabilityType.PROXY, proxy);
		//访问网址
		try {
			webDriver.get("IP");//访问网址
			//标题
			WebElement eleItemTitle = webDriver.findElement(By.id("itemTitle")); 		
			String txtItemTitle=eleItemTitle.getText();
			System.out.println(txtItemTitle); 
		}catch (Exception e) { System.err.println( "Exception: " + e );}
		webDriver.close();
        	webDriver.quit();
	}
}
```





