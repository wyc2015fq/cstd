# 模拟浏览器自动化测试工具Selenium之四cssSelector元素定位开发篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月16日 08:47:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6547








Selenium官网的Document里推荐使用CSS locator，而不是XPath来定位元素，原因是CSS locator比XPath locator速度快，特别是在IE下面（IE没有自己的XPath 解析器(Parser)），比xpath更高效更准确更易编写，缺点是根据页面文字定位时没有xpath直接。

因为前端开发人员就是用CSS Selector设置页面上每一个元素的样式，无论那个元素的位置有多复杂，使用CSS Selector都能非常精准的定位到页面Elements。

CssSelector常用定位




1.根据tagName

　webDriver.findElement(By.cssSelector("input")




2.根据ID

　webDriver.findElement(By.cssSelector("input#username"));html标签和#id

　webDriver.findElement(By.cssSelector("#username"));只是#id


3.根据className

　　单一class：webDriver.findElement(By.cssSelector(".username"));.class

　　复合class：webDriver.findElement(By.cssSelector(".username.**.***"));.classA.classB


4.根据元素属性

　　1)精准匹配： 

　　　[A]  webDriver.findElement(By.cssSelector("input[name=username]"));属性名=属性值,id,class,等都可写成这种形式

            [B]  webDriver.findElement(By.cssSelector("img[alt]"));存在属性。例如img元素存在alt属性

　　   [C]  webDriver.findElement(By.cssSelector("input[type='submit'][value='Login']"));多属性

　　2)模糊匹配：（正则表达式匹配属性）

　　　　[A]  ^=  webDriver.findElement(By.cssSelector(Input[id ^='ctrl']));匹配到id头部 如ctrl_12

　　    　[B]  $=  webDriver.findElement(By.cssSelector(Input[id $='ctrl']));匹配到id尾部 如a_ctrl

　　　　[C]  *=  webDriver.findElement(By.cssSelector(Input[id *= 'ctrl']));匹配到id中间如1_ctrl_12


5.查询子元素


以百度首页搜索输入框和按钮的html为例说明：

```
<form id="form" class="fm" name="f">
　　<span id="s_kw_wrap" class="bg s_ipt_wr quickdelete-wrap">
　　　　<input id="kw" class="s_ipt" type="text" autocomplete="off" maxlength="100" name="wd">
　　</span>
　　<span id="s_btn_wr" class="btn_wr s_btn_wr bg">
　　　　<input id="su" class="btn self-btn bg s_btn" type="submit" value="百度一下">
　　</span>
</form>
```
　　1)子元素   A>B


　　　　WebElement input=  webDriver.findElement(By.cssSelector("form>span>input"));//搜索输入框


　　2)后代元素   A空格B


　　　　WebElement input=  webDriver.findElement(By.cssSelector("form input"));//搜索输入框


　　3)第一个后代元素　　:first-child


　　　　WebElement span= webDriver.findElemet(By.cssSelector("form :first-child"));//冒号前有空格，定位到form下所有级别的第一个子元素

　　　　　　可定位到三个元素：<span id="s_kw_wrap".../> <input id="kw"..../> <input id="su"........./>


　　　　WebElement span= webDriver.findElemet(By.cssSelector("form input:first-child"));//冒号前无空格，定位到form下所有级别的第一个input元素

　　　　　可定位到两个元素：<input id="kw"..../> <input id="su"........./>


　　　　WebElement span= webDriver.findElemet(By.cssSelector("form>span:first-child"));//冒号前无空格，定位到form直接子元素中的第一个span元素

　　　　　　可定位到一个元素：<span id="s_kw_wrap".../>


　　4)最后一个子元素   :last-child  [类同:first-child]

　　　　WebElement userName = webDriver.findEleme(By.cssSelector("form :last-child"));//冒号前有空格，定位到form下所有级别的第一个子元素


　　5)第2个子元素    :nth-child(N)  [类同:first-child]

　　　　WebElement userName = webDriver.findElemet(By.cssSelector("form#form :nth-child(2)"));//冒号前有空格，定位到form下所有级别的第二个子元素


       6)查询兄弟元素

　　webDriver.findElement(By.cssSelector("form#form span+span")); 定位到a 再定位到和它相邻的b



更多可参考https://www.w3.org/TR/css3-selectors/和 http://www.w3cplus.com/css3/attribute-selectors中介绍。

一般情况，我最多用class样式来定位，因为很多网站的元素基本都有class。参考代码如下：



```java
package com.net;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;

public class zhihu {
	public static void spider(String keyword){//爬虫
		System.getProperties().setProperty("webdriver.chrome.driver","D:\\dev\\workspace\\ocweb\\libs\\chromedriver.exe");
		WebDriver webDriver = new ChromeDriver();
		//访问网址
		webDriver.get("http://zhihu.sogou.com/");
		WebElement eleKW = webDriver.findElement(By.id("query"));
		eleKW.clear();
		eleKW.sendKeys(keyword);
		WebElement eleBtn=webDriver.findElement(By.cssSelector("input.btn-search"));
		eleBtn.click();
		//等待页面加载完毕，直到条件满足  
        (new WebDriverWait(webDriver, 30)).until(new ExpectedCondition<Boolean>() {  
            public Boolean apply(WebDriver dr) {  
                int index = dr.getPageSource().indexOf("result-about-list");  
                if(index != -1){   	              	        
                    return true;  //找到，退出等待
                }else{  
                    return false;  //未找到，继续等待
                }  
            }  
        });
		List<WebElement> eleBoxs=webDriver.findElements(By.cssSelector("div.result-about-list"));
		for(WebElement eleBox:eleBoxs){
			WebElement eleTitle=eleBox.findElement(By.cssSelector("h4.about-list-title"));
			WebElement eleAnswer=eleBox.findElement(By.cssSelector("p.about-answer"));
			WebElement eleTxt=eleBox.findElement(By.cssSelector("p.about-text"));
			System.out.println(eleTitle.getText()+eleAnswer.getText()+eleTxt.getText());
		}             
        webDriver.close();
        webDriver.quit();
	}
	
	}
	public static void main(String[] args) {
	    zhihu.spider("大数据");		
	}
}
```








