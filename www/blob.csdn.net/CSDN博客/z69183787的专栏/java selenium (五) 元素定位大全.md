# java selenium (五) 元素定位大全 - z69183787的专栏 - CSDN博客
2017年05月08日 18:08:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：396
个人分类：[测试-Selenium](https://blog.csdn.net/z69183787/article/category/6907139)

页面元素定位是自动化中最重要的事情， selenium Webdriver 提供了很多种元素定位的方法。  测试人员应该熟练掌握各种定位方法。 使用最简单，最稳定的定位方法。
阅读目录
- [自动化测试步骤](http://www.cnblogs.com/TankXiao/p/5222238.html#step)
- [定位方法大全](http://www.cnblogs.com/TankXiao/p/5222238.html#daquan)
- [如何定位](http://www.cnblogs.com/TankXiao/p/5222238.html#howtolocate)
- [通过ID查找元素: By.id()](http://www.cnblogs.com/TankXiao/p/5222238.html#byid)
- [通过Name查找元素:By.name()](http://www.cnblogs.com/TankXiao/p/5222238.html#byname)
- [通过TagName查找元素: By.tagName()](http://www.cnblogs.com/TankXiao/p/5222238.html#className)
- [通过ClassName 查找元素 By.className](http://www.cnblogs.com/TankXiao/p/5222238.html#linktext)
- [通过LinkText查找元素 By.linkText();](http://www.cnblogs.com/TankXiao/p/5222238.html#partialLinkText)
- [通过PartialLinkText 查找元素　By.partialLinkText()](http://www.cnblogs.com/TankXiao/p/5222238.html#css)
- [通过CSS选择器查找元素、](http://www.cnblogs.com/TankXiao/p/5222238.html#xpath)
- [通过Xpath 查找元素](http://www.cnblogs.com/TankXiao/p/5222238.html#!comments)
# 自动化测试步骤
![](http://images2015.cnblogs.com/blog/263119/201602/263119-20160229070414345-1978872540.png)
在自动化测试过程中， 测试程序通常的操作页面元素步骤
1. 找到Web的页面元素，并赋予到一个存储对象中 (WebElement)
2. 对存储页面元素的对象进行操作， 例如：点击链接，在输入框中输入字符等
3. 验证页面上的元素是否符合预期
通过这三个步骤， 我们可以完成一个页面元素的操作， 找到页面元素是很重要的一个步骤。 找不到页面元素，后面就没法做了
Web页面技术的现实复杂性， 造成大量的页面元素很难定位。  经常有人不知道怎么定位。
# 定位方法大全
使用WebDriver对象的findElement函数定义一个Web页面元素
使用findElements函数可以定位页面的多个元素
定位的页面元素需要使用WebElement对象来存储，以便后续使用
常用的定位页面元素方法如下，   按推荐排序
|定位方法|Java语言实现实例|
|----|----|
|id 定位|driver.findElement(By.id(“id的值”))；|
|name定位|driver.findElement(By.name(“name的值”))；|
|链接的全部文字定位|driver.findElement(By.linkText(“链接的全部文字”))；|
|链接的部分文字定位|driver.findElement(By.partialLinkText(“链接的部分文字”))；|
|css 方式定位|driver.findElement(By.cssSelector(“css表达式”))；|
|xpath 方式定位|driver.findElement(By.xpath(“xpath表达式”))；|
|Class 名称定位|driver.findElement(By.className(“class属性”))；|
|TagName 标签名称定位|driver.findElement(By.tagName(“标签名称”))；|
|Jquery方式|Js.executeScript(“return jQuery.find(“jquery表达式”)”)|

# 如何定位
在使用selenium webdriver进行元素定位时,通常使用findElement或findElements方法结合By类返回元素句柄来定位元素
findElement() 方法返回一个元素, 如果没有找到,会抛出一个异常 NoElementFindException()
findElements()方法返回多个元素, 如果没有找到,会返回空数组, 不会抛出异常
如何选择 定位方法
策略是， 选择简单，稳定的定位方法。
1. 当页面元素有id属性的时候， 尽量使用id来定位。  没有的话，再选择其他定位方法
2. cssSelector  执行速度快， 推荐使用
3. 定位超链接的时候，可以考虑linkText或partialLinkText：　但是要注意的是 ，  文本经常发生改变，　所以不推荐用
3. xpath 功能最强悍。 当时执行速度慢，因为需要查找整个DOM,  所以尽量少用。  实在没有办法的时候，才使用xpath 
# 通过ID查找元素: By.id()
通过页面元素的ID来查找元素是最为推荐的方式, W3C标准推荐开发人员为每一个页面元素都提供独一无二的ID属性
一旦元素被赋予了唯一的ID属性.,  我们做自动化测试的时候,很容易定位到元素.  元素的ID被作为首选的识别属性, 因为是最快的识别策略.
以百度主页为例,  搜索框的HTML示例代码如下,   它的ID为kw
```
<input type="text" autocomplete="off" maxlength="100" id="kw" name="wd" class="s_ipt">
```
"百度一下"搜索按钮元素的HTML示例代码如下,   它ID为su
```
<input type="submit" class="btn self-btn bg s_btn" id="su" value="百度一下">
```
在Selenium/WebDriver 中通过ID查找元素的Java示例代码如下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
        WebDriver driver  = new FirefoxDriver();
        driver.get("http://www.baidu.com");
         
        WebElement searchBox = driver.findElement(By.id("kw"));
        searchBox.sendKeys("小坦克 博客园");
        WebElement searchButton = driver.findElement(By.id("su"));
        searchButton.submit();
        
        driver.close();
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
# 通过Name查找元素:By.name()
以豆瓣网的主页搜索框为例, 其搜索框的HTML代码如下,  它name是: q
```
<input type="text" autocomplete="off" name="q" placeholder="书籍、电影、音乐、小组、小站、成员" size="12" maxlength="60">
```
WebDriver中通过name查找豆瓣主页上的搜索框的Java代码如下:
```
WebDriver driver  = new FirefoxDriver();
        driver.get("http://www.douban.com");
         
        WebElement searchBox = driver.findElement(By.name("q"));
        searchBox.sendKeys("小坦克");
        searchBox.submit();
```
# 通过TagName查找元素: By.tagName()
通过tagName来搜索元素的时候,会返回多个元素. 因此需要使用findElements()
```
WebDriver driver  = new FirefoxDriver();
        driver.get("http://www.cnblogs.com");
         
        List<WebElement> buttons = driver.findElements(By.tagName("div"));
        System.out.println("Button:" + buttons.size());
```
注意:  如果使用tagName,  要注意很多HTML元素的tagName是相同的,
比如单选框,复选框, 文本框,密码框.这些元素标签都是input.  此时单靠tagName无法精确获取我们想要的元素, 还需要结合type属性,才能过滤出我们要的元素
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
        WebDriver driver  = new FirefoxDriver();
        driver.get("http://www.cnblogs.com");
         
        List<WebElement> buttons = driver.findElements(By.tagName("input"));
        for (WebElement webElement : buttons) {
            if (webElement.getAttribute("type").equals("text")) {
                System.out.println("input text is :" + webElement.getText());
            }
        }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
# 通过ClassName 查找元素 By.className
以淘宝网的主页搜索为例， 其搜索框的HTML代码如下:   class="search-combobox-input"
```
<input autocomplete="off" autofocus="true" accesskey="s" aria-label="请输入搜索文字" name="q" id="q" class="search-combobox-input" aria-haspopup="true" aria-combobox="list" role="combobox" x-webkit-grammar="builtin:translate" tabindex="0">
```
Java 示例代码如下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
        WebDriver driver  = new FirefoxDriver();
        driver.get("http://www.taobao.com");
        Thread.sleep(15000);
        WebElement searchBox = driver.findElement(By.className("search-combobox-input"));
        searchBox.sendKeys("羽绒服");
        searchBox.submit();
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
注意：使用className 来进行元素定位时， 有时会碰到一个
# 通过LinkText查找元素 By.linkText();
 直接通过超链接上的文字信息来定位元素：例如
```
<a href="https://passport.baidu.com/v2/?login&tpl=mn&u=http%3A%2F%2Fwww.baidu.com%2F" name="tj_login" class="lb" onclick="return false;">登录</a>
```
HTML 代码如下
```
WebDriver driver  = new FirefoxDriver();
        driver.get("http://www.baidu.com");
        WebElement loginLink = driver.findElement(By.linkText("登录"));
        loginLink.click();
```
# 通过PartialLinkText 查找元素　By.partialLinkText()
此方法是上一个方法的加强版，　单你只想用一些关键字匹配的时候，可以使用这个方法，通过部分超链接文字来定位元素
HTML 代码如下
```
WebDriver driver  = new FirefoxDriver();
        driver.get("http://www.baidu.com");
        WebElement loginLink = driver.findElement(By.partialLinkText("登"));
        loginLink.click();
```
注意：用这种方法定位时，　可能会引起的问题是，　当你的页面中不知一个超链接包含“等”时，　findElement方法只会返回第一个查找到的元素，而不会返回所有符合条件的元素
如果你想要获得所有符合条件的元素，还是只能用findElements方法
# 通过CSS选择器查找元素、
 请看 [java selenium (七) CSS 定位](http://www.cnblogs.com/TankXiao/p/5260683.html)
# 通过Xpath 查找元素
 请看 [java selenium (六) XPath 定位](http://www.cnblogs.com/TankXiao/p/5253072.html)
附： selenium java教程 (连载中, 敬请期待）
[java selenium (一) selenium 介绍](http://www.cnblogs.com/TankXiao/p/5252754.html)
[java selenium (二) 环境搭建方法一](http://www.cnblogs.com/TankXiao/p/4110494.html)
[java selenium (三) 环境搭建 基于Maven](http://www.cnblogs.com/TankXiao/p/4142070.html)
[java selenium (四) 使用浏览器调试工具](http://www.cnblogs.com/TankXiao/p/5211759.html)
[java selenium (五) 元素定位大全](http://www.cnblogs.com/TankXiao/p/5222238.html)
[java selenium (六) xpath 定位](http://www.cnblogs.com/TankXiao/p/5253072.html)
[java selenium (七) CSS 定位](http://www.cnblogs.com/TankXiao/p/5260683.html)
[java selenium (八) Selenium IDE 用法](http://www.cnblogs.com/TankXiao/p/4140543.html)
[java selenium (九) 常见web UI 元素操作 及API使用](http://www.cnblogs.com/TankXiao/p/5258104.html)
[java selenium (十) 操作浏览器](http://www.cnblogs.com/TankXiao/p/5260557.html)
[java selenium (十一) 操作弹出对话框](http://www.cnblogs.com/TankXiao/p/5260445.html)
[java selenium (十二) 操作弹出窗口](http://www.cnblogs.com/TankXiao/p/5260707.html)
[java selenium (十三) 智能等待页面加载完成](http://www.cnblogs.com/TankXiao/p/5246557.html)
[java selenium (十四) 处理Iframe 中的元素](http://www.cnblogs.com/TankXiao/p/5237189.html)
[未完待续](http://www.cnblogs.com/TankXiao/p/4142070.html)
