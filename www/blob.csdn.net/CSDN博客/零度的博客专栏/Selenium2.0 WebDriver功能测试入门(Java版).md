# Selenium2.0 WebDriver功能测试入门(Java版) - 零度的博客专栏 - CSDN博客
2017年05月25日 11:15:55[零度anngle](https://me.csdn.net/zmx729618)阅读数：2044
我也一直使用着原始的人工测试手段，随着内容的不断增多，测试起来就越发的繁杂，而且经常犯懒，这样就会忽略很多本该发现的问题，而且也容易出现旧的bug 反复出现的情况，这都是测试不规范造成的。要做好东西就要更加规范和严格，于是乎决定要学习一下 Selenium WebDriver，也就是原先的  Selenium v2 了，这方面整体的文章并不多，所以一边学着，自己一边整理吧。
    对于这个可以自动化测试的工具（ Selenium WebDriver）我就不做过多描述了，去 google、baidu 搜索一下即可。 我这里只记录学习  Selenium WebDriver 的过程，尤其是运行时可能出现的问题，当然了，我是做java的，我只学习 java 与  Selenium WebDriver 配合的方法。
**一、下载文件**
先要去官网（[http://seleniumhq.org/download/](http://seleniumhq.org/download/)）下载必需的文件： 
- Selenium IDE （专门用于 FireFox 测试的独立界面，可以录制测试步骤，但我更倾向于写代码做标准的功能测试）
- Selenium Server （可以输入指令控制、可以解决跨域的 js 问题，等到后面学到了再讲吧）
- The Internet Explorer Driver Server （专门用于IE测试的） 
- Selenium Client Drivers （可以找到你熟悉的语言，例如我选择的 Java）
- Third Party Browser Drivers NOT SUPPORTED/DEVELOPED by seleniumhq（第三方开发的 Selenium 插件，第一个就是 Chrome 的，否则你就没办法测试 Chrome 了）
- 其他的，就根据你自己的需要寻找吧，目前这些足够我用了。 
**二、安装 & 运行**
貌似摆弄新东西时，只有 “Hello World” 蹦出来以后，我们这些初学者才会感到情绪稳定，那就赶紧开始吧。 
对于初期打算直接用编程方式制作测试用例的情况来说，Selenium IDE、Selenium Server 都可以不用安装执行。 
英语好的朋友可以直接看官网的文档（[http://seleniumhq.org/documentation/](http://seleniumhq.org/documentation/)）就能够开始使用了。
看中文的，就继续听我唠叨： 
**【1. 建立 Maven 工程】**
Selenium 支持 maven 工程，这会让你的工作更加简便。 
用 Eclipse 建个 Maven 的工程，建成后，直接修改 pom.xml，（参考：[http://seleniumhq.org/docs/03_webdriver.html#setting-up-a-selenium-webdriver-project](http://seleniumhq.org/docs/03_webdriver.html#setting-up-a-selenium-webdriver-project)）
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<groupId>Selenium2Test</groupId>
	<artifactId>Selenium2Test</artifactId>
	<version>1.0</version>
	<dependencies>
		<dependency>
			<groupId>org.seleniumhq.selenium</groupId>
			<artifactId>selenium-java</artifactId>
			<version>2.25.0</version>
		</dependency>
		<dependency>
			<groupId>com.opera</groupId>
			<artifactId>operadriver</artifactId>
		</dependency>
	</dependencies>
	<dependencyManagement>
		<dependencies>
			<dependency>
				<groupId>com.opera</groupId>
				<artifactId>operadriver</artifactId>
				<version>0.16</version>
				<exclusions>
					<exclusion>
						<groupId>org.seleniumhq.selenium</groupId>
						<artifactId>selenium-remote-driver</artifactId>
					</exclusion>
				</exclusions>
			</dependency>
		</dependencies>
	</dependencyManagement>
</project>
```
pom.xml 修改保存后，eclipse 会自动把需要的 jar 包下载完成。 
**【2. 测试 FireFox】**
Selenium 最初就是在 FireFox 上做起来的插件，所以我们先来搭建 FireFox 的环境。 
确保你正确安装了 FireFox 后，就可以直接编写 java 代码测试喽。 
在 lesson1 目录下建立 ExampleForFireFox.java 
（因为国内不少朋友访问 google 的时候会出问题，所以我就把代码中的 google 变成 baidu 了）
```
package lesson1;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.firefox.FirefoxDriver;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;
public class ExampleForFireFox  {
    public static void main(String[] args) {
        // 如果你的 FireFox 没有安装在默认目录，那么必须在程序中设置
        // System.setProperty("webdriver.firefox.bin", "D:\\Program Files\\Mozilla Firefox\\firefox.exe");
        // 创建一个 FireFox 的浏览器实例
        WebDriver driver = new FirefoxDriver();
        // 让浏览器访问 Baidu
        driver.get("http://www.baidu.com");
        // 用下面代码也可以实现
        // driver.navigate().to("http://www.baidu.com");
        // 获取 网页的 title
        System.out.println("1 Page title is: " + driver.getTitle());
        // 通过 id 找到 input 的 DOM
        WebElement element = driver.findElement(By.id("kw"));
        // 输入关键字
        element.sendKeys("zTree");
        // 提交 input 所在的  form
        element.submit();
        
        // 通过判断 title 内容等待搜索页面加载完毕，Timeout 设置10秒
        (new WebDriverWait(driver, 10)).until(new ExpectedCondition<Boolean>() {
            public Boolean apply(WebDriver d) {
                return d.getTitle().toLowerCase().endsWith("ztree");
            }
        });
        // 显示搜索结果页面的 title
        System.out.println("2 Page title is: " + driver.getTitle());
        
        //关闭浏览器
        driver.quit();
    }
}
```
普通情况下，直接运行代码就可以看到会自动弹出 FireFox 窗口，访问 baidu.com，然后输入关键字并查询，一切都是自动完成的。
**错误提醒：**
1）Exception in thread "main" org.openqa.selenium.WebDriverException: Cannot find firefox binary in PATH. Make sure firefox is installed.
出现这个错误，是说明你的 FireFox 文件并没有安装在默认目录下，这时候需要在最开始执行：System.setProperty 设置环境变量  "webdriver.firefox.bin" 将自己机器上 FireFox 的正确路径设置完毕后即可。
2）Exception in thread "main" org.openqa.selenium.UnsupportedCommandException: Bad request
出现这个错误，很有意思。 查了一下 有人说应该是 hosts 出现了问题，加上一个 127.0.0.1  localhost 就行了，但我的 hosts 上肯定有这个玩意，为啥也会出现这个问题呢？ 
经过调试，发现 127.0.0.1 localhost 的设置必须要在 hosts 文件的最开始，而且如果后面有其他设置后，也不要再出现同样的 127.0.0.1 localhost ，只要有就会出错。（因为我为了方便访问 google 的网站，专门加入了 smarthosts 的内容，导致了 localhost 的重复）
**【3. 测试 Chrome】**
Chrome 虽然不是 Selenium 的原配，但是没办法，她太火辣了，绝对不能抛下她不管的。 
把 ExampleForFireFox.java 稍微修改就可以制作出一个 ExampleForChrome.java ，直接把 new FireFoxDriver() 修改为 new ChromeDriver() 你会发现还是行不通。
**错误如下：**
1）Exception in thread "main" java.lang.IllegalStateException: The path to the driver executable must be set by the webdriver.chrome.driver system property; for more information, see http://code.google.com/p/selenium/wiki/ChromeDriver.
 The latest version can be downloaded from http://code.google.com/p/chromedriver/downloads/list
这应该是找不到 chrome 的文件，好吧，利用 System.setProperty 方法添加路径，这里要注意，是 “webdriver.chrome.**driver**” 可不是“webdriver.chrome.**bin**”
设置路径后还是会报错： 
2）[6416:4580:1204/173852:ERROR:gpu_info_collector_win.cc(91)] Can't retrieve a valid WinSAT assessment.
这个貌似是因为 Selenium 无法直接启动 Chrome 导致的，必须要通过前面咱们下载 Chrome 的第三方插件 ChromeDriver，去看第一个错误中提示给你的 网址：[http://code.google.com/p/selenium/wiki/ChromeDriver](http://code.google.com/p/selenium/wiki/ChromeDriver)
按照人家给的例子来修改我们的测试代码吧： 
```
package lesson1;
import java.io.File;
import java.io.IOException;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriverService;
import org.openqa.selenium.remote.DesiredCapabilities;
import org.openqa.selenium.remote.RemoteWebDriver;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;
public class ExampleForChrome {
	public static void main(String[] args) throws IOException {
		// 设置 chrome 的路径
		System.setProperty(
				"webdriver.chrome.driver",
				"C:\\Documents and Settings\\sq\\Local Settings\\Application Data\\Google\\Chrome\\Application\\chrome.exe");
		// 创建一个 ChromeDriver 的接口，用于连接 Chrome
		@SuppressWarnings("deprecation")
		ChromeDriverService service = new ChromeDriverService.Builder()
				.usingChromeDriverExecutable(
						new File(
								"E:\\Selenium WebDriver\\chromedriver_win_23.0.1240.0\\chromedriver.exe"))
				.usingAnyFreePort().build();
		service.start();
		// 创建一个 Chrome 的浏览器实例
		WebDriver driver = new RemoteWebDriver(service.getUrl(),
				DesiredCapabilities.chrome());
		// 让浏览器访问 Baidu
		driver.get("http://www.baidu.com");
		// 用下面代码也可以实现
		// driver.navigate().to("http://www.baidu.com");
		// 获取 网页的 title
		System.out.println("1 Page title is: " + driver.getTitle());
		// 通过 id 找到 input 的 DOM
		WebElement element = driver.findElement(By.id("kw"));
		// 输入关键字
		element.sendKeys("zTree");
		// 提交 input 所在的 form
		element.submit();
		// 通过判断 title 内容等待搜索页面加载完毕，Timeout 设置10秒
		(new WebDriverWait(driver, 10)).until(new ExpectedCondition<Boolean>() {
			public Boolean apply(WebDriver d) {
				return d.getTitle().toLowerCase().endsWith("ztree");
			}
		});
		// 显示搜索结果页面的 title
		System.out.println("2 Page title is: " + driver.getTitle());
		// 关闭浏览器
		driver.quit();
		// 关闭 ChromeDriver 接口
		service.stop();
	}
}
```
运行一下看看，是不是一切OK了？ 
**【补充】**
上一个 Demo 中无法正常使用 new ChromeDriver()，今天在做进一步学习时看到一篇文章（[http://qa.blog.163.com/blog/static/19014700220122231779/](http://qa.blog.163.com/blog/static/19014700220122231779/)），恍然大悟，原来只需要把
 ‘webdriver.chrome.driver’ 的值设置为 chromedriver 的路径就可以了。
``
```
package lesson1;
import java.io.IOException;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;
public class ExampleForChrome2 {
	public static void main(String[] args) throws IOException {
		// 设置 chrome 的路径
		System.setProperty(
				"webdriver.chrome.driver",
				"E:\\Selenium WebDriver\\chromedriver_win_23.0.1240.0\\chromedriver.exe");
		// 创建一个 ChromeDriver 的接口，用于连接 Chrome
		// 创建一个 Chrome 的浏览器实例
		WebDriver driver = new ChromeDriver();
		// 让浏览器访问 Baidu
		driver.get("http://www.baidu.com");
		// 用下面代码也可以实现
		// driver.navigate().to("http://www.baidu.com");
		// 获取 网页的 title
		System.out.println("1 Page title is: " + driver.getTitle());
		// 通过 id 找到 input 的 DOM
		WebElement element = driver.findElement(By.id("kw"));
		// 输入关键字
		element.sendKeys("zTree");
		// 提交 input 所在的 form
		element.submit();
		// 通过判断 title 内容等待搜索页面加载完毕，Timeout 设置10秒
		(new WebDriverWait(driver, 10)).until(new ExpectedCondition<Boolean>() {
			public Boolean apply(WebDriver d) {
				return d.getTitle().toLowerCase().endsWith("ztree");
			}
		});
		// 显示搜索结果页面的 title
		System.out.println("2 Page title is: " + driver.getTitle());
		// 关闭浏览器
		driver.quit();
		// element = driver.findElement(By.id("kw"));
		// // element.clear();
		// element.click();
		// element.clear();
		// element.sendKeys("zTree");
		// element.submit();
	}
}
```
**【4. 测试 IE】**
想逃避 IE 吗？作为前端开发，IE 你是必须要面对的，冲吧！ 
其实你会发现， Selenium 主要也就是针对 FireFox 和 IE 来制作的，所以把 FireFox 的代码修改为 IE 的，那是相当的容易，只需要简单地两步：
1）把 ExampleForFireFox.java 另存为 ExampleForIE.java  
2）把 WebDriver driver = new FirefoxDriver(); 修改为 WebDriver driver = new InternetExplorerDriver();
3）一般大家的 IE都是默认路径吧，所以也就不用设置 property 了 。
```
package lesson1;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.ie.InternetExplorerDriver;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;
public class ExampleForIE {
	public static void main(String[] args) {
		// 如果你的 FireFox 没有安装在默认目录，那么必须在程序中设置
		// System.setProperty("webdriver.firefox.bin", "D:\\Program Files\\Mozilla Firefox\\firefox.exe");
		// 创建一个 FireFox 的浏览器实例
		WebDriver driver = new InternetExplorerDriver();
		// 让浏览器访问 Baidu
		driver.get("http://www.baidu.com");
		// 用下面代码也可以实现
		// driver.navigate().to("http://www.baidu.com");
		// 获取 网页的 title
		System.out.println("1 Page title is: " + driver.getTitle());
		// 通过 id 找到 input 的 DOM
		WebElement element = driver.findElement(By.id("kw"));
		// 输入关键字
		element.sendKeys("zTree");
		// 提交 input 所在的 form
		element.submit();
		// 通过判断 title 内容等待搜索页面加载完毕，Timeout 设置10秒
		(new WebDriverWait(driver, 10)).until(new ExpectedCondition<Boolean>() {
			public Boolean apply(WebDriver d) {
				return d.getTitle().toLowerCase().endsWith("ztree");
			}
		});
		// 显示搜索结果页面的 title
		System.out.println("2 Page title is: " + driver.getTitle());
		// 关闭浏览器
		driver.quit();
	}
}
```
      运行一下，是不是 so easy？ 
      入门工作完成，现在完全可以利用 java 代码，让 Selenium 自动执行我们设置好的测试用例了，不过，这仅仅是个开始。
