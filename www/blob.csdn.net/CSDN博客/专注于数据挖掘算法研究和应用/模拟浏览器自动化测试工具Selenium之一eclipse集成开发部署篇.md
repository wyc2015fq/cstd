# 模拟浏览器自动化测试工具Selenium之一eclipse集成开发部署篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月15日 10:21:54[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3263








1、背景：在网页自动化测试和网页表单自动交互的场景中，对动态js页面的加载，隐藏链接爬虫和表单元素需要加载js来解析。

                  htmlunit相比较于htmlparser以及httpclient只能解析静态页面来说，可以支持动态js页面的解析，但对js支持不是很好，如angularjs，因此需采用模拟浏览器的工具。

                   Selenium模拟浏览器：提供一组API和真实的浏览器内核交互。

2、部署：

   selenium下载：http://selenium-release.storage.googleapis.com/index.html

下载：selenium-java-2.53.1.zip(jar包引入eclipse的java工程)和IEDriverServer_x64_2.53.1.zip(对应本机操作系统是64位的IE浏览器启动)。

两点注意：1）发现最新版本3.0.1和jdk1.7不兼容（本机安装是jdk1.7），所以更换回2.53.1版本。2）提示保护模式错误时，可通过IE浏览器设置（貌似效果不好），也可以在代码上设置。

Internet Option in IE –> Security–>Check or Unchecked “Enable Protected Mode.” 


![](https://img-blog.csdn.net/20161115101957070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


代码中设置见案例代码。IE浏览器保护模式错误提示如下：



```
Started InternetExplorerDriver server (64-bit)
2.53.1.0
Listening on port 36078
Only local connections are allowed
Exception: org.openqa.selenium.remote.SessionNotFoundException: Unexpected error launching Internet Explorer. Protected Mode settings are not the same for all zones. Enable Protected Mode must be set to the same value (enabled or disabled) for all zones. (WARNING: The server did not provide any stacktrace information)
Command duration or timeout: 840 milliseconds
Build info: version: '2.53.1', revision: 'a36b8b1', time: '2016-06-30 17:32:46'
System info: host: 'ICD-PC', ip: '132.97.194.17', os.name: 'Windows 7', os.arch: 'amd64', os.version: '6.1', java.version: '1.7.0_79'
Driver info: org.openqa.selenium.ie.InternetExplorerDriver
```




3、案例：



```java
package com.test;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.ie.InternetExplorerDriver;
import org.openqa.selenium.remote.DesiredCapabilities;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;

public class BrasShopYP {
    //selenium下载：http://selenium-release.storage.googleapis.com/index.html
	public static void main(String[] args) {
		try {
			System.getProperties().setProperty("webdriver.ie.driver","D:\\dev\\workspace\\ocweb\\libs\\IEDriverServer.exe");
			//System.setProperty("webdriver.ie.bin", "C:\\Program Files\\Internet Explorer\\iexplore.exe"); 
			//WebDriver webDriver = new InternetExplorerDriver();
			DesiredCapabilities caps = DesiredCapabilities.internetExplorer();
			caps.setCapability(InternetExplorerDriver.INTRODUCE_FLAKINESS_BY_IGNORING_SECURITY_DOMAINS,true);
			WebDriver webDriver = new InternetExplorerDriver(caps);
			//访问网址
			webDriver.get("http://www.baidu.com");
			 // 获取 网页的 title
	        System.out.println("1 Page title is: " + webDriver.getTitle());
	        // 通过 id 找到 input 的 DOM
	        WebElement element = webDriver.findElement(By.id("kw"));
	        // 输入关键字
	        element.sendKeys("zTree");
	        // 提交 input 所在的  form
	        element.submit();       
	        // 通过判断 title 内容等待搜索页面加载完毕，Timeout 设置50秒
	        //(new WebDriverWait(webDriver, 50)).until(new ExpectedCondition<Boolean>() {
	        //    public Boolean apply(WebDriver d) {
	        //        return d.getTitle().toLowerCase().endsWith("ztree");
	        //    }
	        //});
	        // 显示搜索结果页面的 title
	        System.out.println("2 Page title is: " + webDriver.getTitle());
	        
			// 关闭窗口，释放资源。
			webDriver.close();
			}catch (Exception e) {System.err.println( "Exception: " + e ); }
	 }
}
```



下步就要研究selenium如何提取页面链接爬虫并实现表单自动填写交互。






