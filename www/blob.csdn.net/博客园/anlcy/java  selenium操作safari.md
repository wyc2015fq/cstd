
# java  selenium操作safari - anlcy - 博客园






# [java  selenium操作safari](https://www.cnblogs.com/camilla/p/6871942.html)
SafariDriver是一个Safari浏览器的扩展。和Firefox类似，在selenium-safari-driver-xxx.jar包中org.openqa.selenium.safari路径下提供了Safaridriver.safariextz扩展程序。
包下载地址：[http://mvnrepository.com/artifact/org.seleniumhq.selenium/selenium-safari-driver](http://mvnrepository.com/artifact/org.seleniumhq.selenium/selenium-safari-driver)
可能需要的设置
`使用该浏览器，只需要将driver改成SafariDriver即可：``WebDriver driver = new SafariDriver();`
`Selenium 2 跑safari浏览器 （在windows XP系统上）`
`1，配置环境（什么装JDK,ECLIPSE,SELENIUM,SAFARI 之类的等等，，略去步骤）`
`2，编写测试代码（很简单，简单的new SafariDriver（）之类的，以google页面为例子，取点值）`
`WebDriver driver =new SafariDriver();`
`driver.get("http://www.google.com.hk");`
`driver.findElement(By.name("q")).sendKeys("webdriver");`
`driver.findElement(By.name("btnK")).click();`
`new WebDriverWait(driver, 3).until(ExpectedConditions`
`.titleIs("webdriver - Google Search"));`
`dirver.quit();`
`3，这个时候运行， 会出现这样的结果：`
`页面停住了，只有这么类似一条信息显示着“SafariDriver requesting connection at ws://localhost:43980/wd”。`
`4，要对safari浏览器进行扩展：`
`1）创建safari的签名证书，只有这样才能往safari中添加自定义扩展`
`至于这个过程，大致上是先注册一个apple ID， 注册完后就可以直接 “get`
`started”，再根据提示 一步一步 “create certificate”。 详细过程可以google`
`一下， 很简单。`
`2）SVN到这个地址：http://selenium.googlecode.com/svn/tags/selenium-2.25.0/（如`
`果是使用的selenium0其他版本，如2.21，那么修改下最后的一个版本号就行/tags`
`/selenium-2.21.0/），将里面的所有项目都检出到本地（随意取个名字如`
`SeleniumSafari2.25）。`
`3）cmd 到 这个刚自定义名字的 SeleniumSafari2.25 目录下 ， 使用命令 go`
`safari， 那么就开始自动build了（build出来的就是待会要添加的extension）。`
`4） 打开safari，设置一下 把开发菜单调出来； 然后选择开发--显示扩展创建器--添加`
`扩展--选择 SeleniumSafari2.25/build/javascript/safari-driver`
`/SafariDriver.safariextension（这个便是刚才build出来的）`
`5）选择安装 ，然后 OK， 可以再跑刚才的selenium代码试试了。`
`5, 这个时候的结果还是会出那个信息页面，不过稍等一下，，接下来就会正常的进入google`
`页面了。`





