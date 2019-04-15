# appium知识总结 - qq_21826709的博客 - CSDN博客












2017年01月17日 11:43:42[Guan-Javid](https://me.csdn.net/qq_21826709)阅读数：1764








## 一、**搜索**

### **1.搜索按钮**

//点击搜索  

By search = By.*id*("com.autonavi.minimap:id/btn_search");

elementManager.*waitForElement*(driver,
search, t);

elementManager.*checkandClick*(driver,
search);

### **2.Appium键盘事件**

回车键（Enter）：66  搜索键：84

driver.sendKeyEvent(AndroidKeyCode.***ENTER***);

driver.sendKeyEvent(66);

driver.pressKeyCode(66);

driver.sendKeyEvent(84);

adb命令
|/*** 点击回车（搜索）按键*/**publicstaticvoid** adbSearch() {    ProcessBuilder pb1 = **new** ProcessBuilder(*adbPath*,"shell", "input", "keyevent", "66");**try** {pb1.start();    } **catch** (IOExceptione1) {e1.printStackTrace();    } }|
|----|



举一反三：返回键

driver.sendKeyEvent(AndroidKeyCode.***BACK***);

driver.sendKeyEvent(4);

driver.pressKeyCode(4);

adb命令
|//点击返回按键**publicstaticvoid** adbBack() {ProcessBuilder pb1 = **new** ProcessBuilder(*adbPath*,"shell", "input", "keyevent", "4");**try** {pb1.start();} **catch** (IOExceptione1) {e1.printStackTrace();}}|
|----|

参考网址：

[http://www.51testing.com/html/03/247003-864114.html](http://www.51testing.com/html/03/247003-864114.html)

## 二、**长按**

### **1.长按几秒**

TouchAction Action =
**new** TouchAction(driver);

Action.longPress(driver.findElement(By.*id*("com.tencent.mm:id/z6")), 10000).perform();//强制长按10s，推荐使用该方法

// 在指定元素对象处长按

WebElement el =driver.findElement(By.*id*("com.chinasofti.rcs:id/tv_record_audio_text"));

//按住等待5秒后释放

Action.press(el).waitAction(5);

Action.perform();

Action.press(driver.findElement(By.*id*("com.chinasofti.rcs:id/tv_record_audio_text"))).waitAction(5000).release().perform();

### **2.长按操作**

// 长按发送图片Action.longPress(driver.findElement(By.*id*("com.chinasofti.rcs:id/imageview_msg_image"))).perform();

### **3.坐标长按**

Action.longPress(343, 1108).perform();

## 三、**坐标点击(x,y)**

### **1.自带方法**

driver.tap(1, 123, 456, 500);

### **2.集成方法**

driverUtil.*clickByCoordinate*(driver, 428, 1681);

tab(369,895);
|/* 点击坐标 */**publicvoid** tab(**int**x,**int**y) {**try** {TouchAction ta = **new** TouchAction(*driver*);ta.tap(x,y).release().perform();} **catch** (Exception e) {e.printStackTrace();}}|
|----|







## 四、**Toast消息**

提示：Appium更新到1.6.3，需要java-client 5.0、JDK1.8 和selenium 3.0。配置文件里面添加：

capabilities.setCapability(MobileCapabilityType.AUTOMATION_NAME, AutomationName.ANDROID_UIAUTOMATOR2);

例子核心代码：
|@Test(groups = { "Toast" })**publicvoid** webViewTest()**throws** InterruptedException {**final**WebDriverWaitwait =**new**WebDriverWait(driver, 10);driver.startActivity("com.testerhome.webview",".ToastActivity");MobileElementtoastButton =driver.findElementById("com.testerhome.webview:id/toast");toastButton.click();Assert.assertNotNull(wait.until(ExpectedConditions.presenceOfElementLocated(By.*xpath*(".//*[contains(@text,'Toast Test')]"))));}|
|----|









## 五、**appium常见问题总结**

### **1.环境配置问题**

比如：没有配置path路径，配置成功后输入appium-doctor的cmd界面



### **2.手机驱动未安装成功**

报错提示：org.openqa.selenium.SessionNotCreatedException: A new session could not be created.(Original error: Device ZX1C622BU3 was not in the list of connected devices)(WARNING: The server did not provide any stacktrace information)

解决办法：可能手机驱动未安装成功或为安装，利用电脑自动安装和360手机助手安装驱动

### 3.**没有连接到手机**

报错提示： error: Failed to start an Appium session, err was: Error: Command failed: C:\Windows\system32\cmd.exe /s /c“D:\android-sdk-windows\platform-tools\adb.exe -s adb server version (32) doesn’t match this client (36); killing…

wait-for-device”error: could not install smartsocket listener: cannot bind to 127.0.0.1:5037:

设备连接成功的cmd界面



解决办法：没有打开USB调试、5037端口号占用、进程占用、断开USB连接再重新连接

### **4.端口问题(端口号5037)**

①软件占用，比如：数据库等软件

例子：运行时Appium中报错 error: Unhandled error: Error: connect ECONNREFUSED

解决办法：

mysql和node.js使用的同一个端口，换一个端口号就行



②进程占用，比如：360手机助手、应用宝、91助手、豌豆荚等进程

解决办法：

关闭360手机助手等软件

任务管理器中杀掉进程，比如adb.exe

adb kill-server和adb start-server杀掉和开启

Cmd命令杀掉进程

[http://jingyan.baidu.com/article/3c48dd34491d47e10be358b8.html](http://jingyan.baidu.com/article/3c48dd34491d47e10be358b8.html)



### **5.appium端口问题(默认4723)**

端口号没有开启debug模式

端口号错误，开启的是4723，使用的是4725

### **6.Package和Activity问题**

package或者activity写错，比如复制错误、少写”/”等

多个package或者activity

报错提示：org.openqa.selenium.SessionNotCreatedException: A new session could not be created. (Original error: com.qihoo360.contacts/.ui.mainscreen.MainTabBase never started. Current: com.qihoo360.contacts/.danmu.ui.DanmuSplashActivity) (WARNING: The server did not provide any stacktrace information)

解决办法：

capabilities.setCapability("appWaitActivity",".ICloudLeaderActivity");

capabilities.setCapability("appWaitActivity",
".Main");

Package有时候也会不一样需要多添加一句capabilities.setCapability("appWaitPackage", "报错时说的真正的页面，即报错current那一个");

### **7.命令超时时间(60s)**

capabilities.setCapability("newCommandTimeout", 360000);

收到下一条命令的超时时间，超过appium会自动关闭session，默认60s

### **8.文本输入问题**

中文输入解决方法

capabilities.setCapability("unicodeKeyboard",
**true**);

capabilities.setCapability("resetKeyboard",
**true**);

输入不稳定解决方法

利用adb shell命令调用其他输入法

构造方法输入

### **9.乱码问题**

eclipse设置编码格式：UTF-8

[http://jingyan.baidu.com/article/2009576193ee38cb0721b416.html](http://jingyan.baidu.com/article/2009576193ee38cb0721b416.html)

### **10.JDK问题**

JDK1.7与JDK1.8冲突，保留自己使用的测试框架JDK，卸载其他版本



版本问题或过低

报错提示：org.openqa.selenium.SessionNotCreatedException: A new session could not be created. (Original error: Command failed:[java](http://www.51testing.com/html/01/javascript:;).util.zip.ZipException: error in opening zip file    at java.util.zip.ZipFile.open(Native Method)    at java.util.zip.ZipFile.<init>(Unknown Source)    at java.util.zip.ZipFile.<init>(Unknown Source)    at java.util.jar.JarFile.<init>(Unknown Source)    at java.util.jar.JarFile.<init>(Unknown Source)    at s.Sign.sign(Sign.java:441)    at s.Sign.main(Sign.java:532))

解决办法：将JDK改为1.7

报错提示：error: Failed to start an Appium session, err was: Error:‘java - version’failed. Error: Command failed: C:\Windows\system32\cmd.exe /s /c“java -version”

解决办法：[Java](http://lib.csdn.net/base/17)版本错误，请安装最新的版本。



### **11.Jar包问题**

报错提示：

java.lang.NoSuchMethodError: org.openqa.selenium.remote.ErrorHandler.<init>(Lorg/openqa/selenium/remote/ErrorCodes;Z)V    at io.appium.java_client.AppiumDriver.<clinit>(AppiumDriver.java:41)    at com.MFExchange.appium.MFExchangeTest.setUp(MFExchangeTest.java:40)    at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)    at sun.reflect.NativeMethodAccessorImpl.invoke(Unknown Source)    at sun.reflect.DelegatingMethodAccessorImpl.invoke(Unknown Source)    at java.lang.reflect.Method.invoke(Unknown Source)    at org.junit.runners.model.FrameworkMethod$1.runReflectiveCall(FrameworkMethod.java:45)    at org.junit.internal.runners.model.ReflectiveCallable.run(ReflectiveCallable.java:15)    at org.junit.runners.model.FrameworkMethod.invokeExplosively(FrameworkMethod.java:42)    at org.junit.internal.runners.statements.RunBefores.evaluate(RunBefores.java:27)    at org.junit.internal.runners.statements.RunAfters.evaluate(RunAfters.java:30)    at org.junit.runners.ParentRunner.runLeaf(ParentRunner.java:263)    at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:68)    at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:47)    at org.junit.runners.ParentRunner$3.run(ParentRunner.java:231)    at org.junit.runners.ParentRunner$1.schedule(ParentRunner.java:60)    at org.junit.runners.ParentRunner.runChildren(ParentRunner.java:229)    at org.junit.runners.ParentRunner.access$000(ParentRunner.java:50)    at org.junit.runners.ParentRunner$2.evaluate(ParentRunner.java:222)    at org.junit.runners.ParentRunner.run(ParentRunner.java:300)    at org.eclipse.jdt.internal.junit4.runner.JUnit4TestReference.run(JUnit4TestReference.java:50)    at org.eclipse.jdt.internal.junit.runner.TestExecution.run(TestExecution.java:38)    at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:467)    at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:683)    at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.run(RemoteTestRunner.java:390)    at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.main(RemoteTestRunner.java:197)

解决办法：把selenium-java-2.41.0.jar换成2.42版本的

Jar包下载建议到官网或者专业网站

[http://maven.outofmemory.cn/hot/](http://maven.outofmemory.cn/hot/)

### **12.操作时延问题**

1) 显性等待：调用selenium的方法， 需要添加WebDriver.Support引用
显性等待是指在代码进行下一步操作之前等待某一个条件的发生。最不好的情况是使用Thread.sleep()去设置一段确认的时间去等待。但为什么说最不好呢？因为一个元素的加载时间有长有短，你在设置sleep的时间之前要自己把握长短，太短容易超时，太长浪费时间。selenium webdriver提供了一些方法帮助我们等待正好需要等待的时间
|WebDriverWait wait = new WebDriverWait(driver, TimeSpan.FromSeconds(10)); element = wait.Until<IWebElement>((d) =>      {           return driver.FindElement(By.Id("userName"));           });|
|----|

2) 隐性等待：设置时间不易过长，设置为500或1000即可
隐性等待是指当要查找元素，而这个元素没有马上出现时，告诉WebDriver查询Dom一定时间。默认值是0,但是设置之后，这个时间将在WebDriver对象实例整个生命周期都起作用。

driver.Manage().Timeouts().ImplicitlyWait(TimeSpan.FromSeconds(1));

*driver*.manage().timeouts().implicitlyWait(30,TimeUnit.***SECONDS***);

3）线程等待

 Thread.*sleep*(2000);

特别注意

①应用首次启动时，在引导页滑动要添加等待时间

②在弱网的情况下，有些操作需要添加等待时间

### **13.运行时错误**

手机断开连接需要重新连接

报错提示：Error: Command failed: C:\Windows\system32\cmd.exe /s /c “D:\android-sdk-windows\platform-tools\adb.exe -s 8806a0b0 shell“echo ‘ready‘“error: unknown host service



会话占用

报错提示：error: Failed to start an Appium session, err was: Error: Requested a new session but one was in progress



之前的会话没有关闭，然后你又运行了测试实例，也没有设置覆盖. 解决办法: ①重新停止appium服务，开启Appium服务 ②在Genarel Setting那里设置覆盖Session，重启Appium

测试结束在AfterClass加driver.quit()

### **14.手机系统版本过低**

appium不支持4.2.2以下的Android系统

报错提示： error: Android devices must be of API level 17 or higher. Please change your device to Selendroid or upgrade Android on your device.



### 15.**句柄(driver)的声明**

**publicstatic** AndroidDriver<WebElement>  *driver*;//推荐使用



**publicstatic** AndroidDriver  *driver*;



根据比较第一个声明的driver比第二个使用的方法多，故推荐使用

### **16.特殊滑动**

①框架集成方法

driverUtil.swipeWithPercent(driver, 50, 90, 50, 10);//左滑

②封装方法
|/** * 特殊滑动 */**publicstaticvoid** SpecialSlip(WebElementele,String data){//获取控件的起始坐标**int**xStartPoint =ele.getLocation().getX();**int**xEndPoint =xStartPoint + ele.getSize().getWidth();**int**y =ele.getLocation().getY();TouchAction Action = **new** TouchAction(*driver*);**if**(data.equals("左滑")){Action.press(xEndPoint-20,y).waitAction(1000).moveTo(xStartPoint,y+20).release().perform();}**if**(data.equals("右滑")){Action.press(xEndPoint+20,y+20).waitAction(1000).moveTo(xEndPoint,y+20).release().perform();}}|
|----|

举例：三星手机联系人左滑输入短信功能；QQ消息左滑置顶、标为未读、删除等功能；翼支付的右滑登录功能等等。



测试代码



④封装两个元素之间的滑动
|WebElement elmentA = **null**;WebElement elmentB = **null**;**int**startX = 0,startY = 0, endX = 0, endY = 0;**int**duration=0,time=0;/// <summary>/// 从元素A的位置滑动到元素B的位置/// </summary>/// <param name="A">元素A的名称</param>/// <param name="B">元素B的名称</param>/// <param name="sDuration">滑动持续时间</param>/// <param name="sTime">滑动次数</param>**publicvoid** SwipeAToB(string A,string B,string sDuration,string sTime){    startX = elmentA.Location.X + elmentA.Size.Width / 2;  //元素A的中心横坐标    startY = elmentA.Location.Y + elmentA.Size.Height / 2; //元素A的中心纵坐标    endX = elmentB.Location.X + elmentB.Size.Width / 2;    //元素B的中心横坐标    endY = elmentB.Location.Y + elmentB.Size.Height / 2;   //元素B的中心纵坐标    duration = string.IsNullOrEmpty(sDuration) ? 1500 : **int**.Parse(sDuration);//持续时间为空时，默认设置为1500毫秒    time = string.IsNullOrEmpty(sTime) ? 1500 : **int**.Parse(sTime);//滑动次数为空时，默认设置为滑动1次**for** (**int** i = 0; i < time; i++)    {        driver.Swipe(startX, startY, endX, endY, duration);    }}|
|----|

参考网址：

[https://testerhome.com/topics/3306](https://testerhome.com/topics/3306)

### 17.**清除编辑框EditText内容**

①自带方法

driver.findElementById("com.hpbr.bosszhipin:id/et_search").clear();

②输入框后面的取消按钮

driver.findElement(By.*id*("com.hpbr.bosszhipin:id/iv_clear")).click();

③封装方法
|/**     * 逐字删除编辑框中的文字     * **@param** element 文本框架控件     */**publicvoid** clearText(AndroidElementelement){        String className = element.getClass().getSimpleName();**if** (className.equals("EditText")){            String text = element.getText();//跳到最后driver.pressKeyCode(KEYCODE_MOVE_END);**for** (**int**i = 0;i < text.length(); i ++){//循环后退删除driver.pressKeyCode(BACKSPACE);            }        }**else** {print("不是文本输入框架，无法删除文字");        }   }|
|----|



### 18.**Ctrl+V操作**

封装方法




|/** * 粘贴操作 */**publicstaticvoid** CtrlVPaste(WebElementel){//获取焦点el.click();//Ctrl+V组合操作*driver*.sendKeyEvent(50,AndroidKeyMetastate.***META_CTRL_ON***);}|
|----|












