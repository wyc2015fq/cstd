# Selenium2.0功能测试之访问站点及获取页面信息(Java版) 【转】 - z69183787的专栏 - CSDN博客
2017年05月08日 18:16:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1205
- Selenium为访问站点提供了两种方式：
> - driver.get("URL");   #个人推荐这种方式，因为能少写一个字符是一个字符啊。
- driver.navigate().to("URL");
- 页面的Title主要用于case的验证，当然了也有一位大神教育我说case不够title来凑，这个作用显著啊!
- 页面的URL也主要是用于case的验证，URL的一个重要的测试领域就是对于重定向的测试(很多地址当你访问后会自动的跳转到其他的地址，这时URL 验证的机会就来了，当然了还是有很多其他的地方能用到一时想不出来了就这些吧！)
> 
package org.coderinfo.demo;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.chrome.ChromeDriver;
public class GetWebSiteAndPrintWebInfo {
    private static final String URL = "[http://www.google.com.hk](http://www.google.com.hk/)";
    public static void main(String[] args) {
        WebDriver driver = new ChromeDriver();
        driver.manage().window().maximize(); //最大化浏览器界面
        driver.get(URL); // 等同于 driver.navigate().to(URL); 访问谷哥的首页 ，此处放弃度娘。
        String title = driver.getTitle(); //获取当前页面的title
        String currentUrl = driver.getCurrentUrl(); // 获取当前页面的URL
        System.out.printf("Current page's title is: %s , Current URL is: %s \n",title,currentUrl);
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        driver.quit();  //彻底退出WebDriver
    }
}
