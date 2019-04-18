# testng ITestListener使用 - z69183787的专栏 - CSDN博客
2018年06月07日 10:31:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：100
[https://www.cnblogs.com/superbaby11/p/6400686.html](https://www.cnblogs.com/superbaby11/p/6400686.html)
# ITestListener适用场景
当使用testng执行测试时，我们常会想在某个阶段做一些特别的处理，比如：测试成功结束后，测试失败后，跳过某个脚本后，全部脚本执行完毕后。要想达成这个目标，我们需要实现testng的ITestListener接口，自定义一个自己的listener。ITestListener包含两种类型的方法：一类是测试用例级别的，例如onTestStart，onTestSuccess，onTestFailure，onTestSkipped，onTestFailedButWithinSuccessPercentage；另一类是测试集级别的，例如onStart，onFinish。这些方法有一个输入参数，result或者context。result是ITestResult类型的，可以知晓测试用例成功或者失败和测试何时开始等信息。context是ITestContext类型的，适用于测试集级别，可以知晓成功的脚本有哪些，失败的脚本有哪些。
# 实现ITestListener的方法
在自定义的customListener中，需要实现ITestListener的全部方法。
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
import org.testng.ITestListener;
import org.testng.ITestResult;
import org.testng.ITestContext;
/**
 * Created by wwh on 17/2/13.
 */
public class customListener implements ITestListener{
    public void onTestStart(ITestResult result) {
        System.out.println("Test started running" + result.getMethod().getMethodName() +
                "at:"+result.getStartMillis());
    }
    public void onTestSuccess(ITestResult result) {
        System.out.println("Result success");
    }
    public void onTestFailure(ITestResult result) {
        System.out.println("Result failure");
    }
    public void onTestSkipped(ITestResult result) {
        System.out.println("Result skip");
    }
    public void onTestFailedButWithinSuccessPercentage(ITestResult result) {
        System.out.println("Result fail but with success percentage");
    }
    public void onStart(ITestContext context) {
        System.out.println("now start test");
    }
    public void onFinish(ITestContext context) {
        System.out.println("now finish test");
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
# 使用自定义的listener
现在完成了创建listener，还需要声明listener，然后测试脚本才能使用这个listener。
有两种方式声明listener：一种是添加@Listeners注解到测试条例上，缺点是每个使用自定义listener的测试条例都要添加@Listeners注解，很麻烦。另一种是，[使用testng.xml配置文件](http://www.cnblogs.com/superbaby11/p/6400400.html)，在其中添加suite标签和listener标签。
## 方式一：使用注解
样例代码
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
import org.testng.annotations.Listeners;
import org.testng.annotations.Test;
import main.java.com.dbyl.appiumServer.customListener;
/**
 * Created by wwh on 17/2/14.
 */
@Listeners(customListener.class)//这个语句指明了使用自定义的listener。
public class listenerTest {
    @Test
    public void templistener1(){
        System.out.println("i'm listenerTest1");
    }
    @Test
    public void templistener2(){
        System.out.println("i'm listenerTest2");
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
输出结果
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
[TestNG] Running:
  /Users/wwh/Library/Caches/IdeaIC2016.3/temp-testng-customsuite.xml
now start test
Test started runningtemplistener1at:1487136848622
i'm listenerTest1
Result success
Test started runningtemplistener2at:1487136848712
i'm listenerTest2
Result success
now finish test
===============================================
Default Suite
Total tests run: 2, Failures: 0, Skips: 0
===============================================
Process finished with exit code 0
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
## 方式二：使用testng.xml配置文件
样例代码
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
import org.testng.annotations.Test;
/**
 * Created by wwh on 17/2/14.
 */
public class listenerTest {
    @Test
    public void templistener1(){
        System.out.println("i'm listenerTest1");
    }
    @Test
    public void templistener2(){
        System.out.println("i'm listenerTest2");
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
testng.xml
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd" >
<suite name="Second suite" verbose="1" >
    <listeners>
        <listener class-name="main.java.com.dbyl.appiumServer.customListener"></listener>
    </listeners>
    <test name = "allTestsInAClass" >
        <classes>
            <class name="main.java.com.dbyl.appiumCore.tests.listenerTest"/>
        </classes>
</test>
</suite>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
输出结果
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
Mac:ProjectWang wwh$ mvn clean test -Dtestng.xml
.
.
.
-------------------------------------------------------
 T E S T S
-------------------------------------------------------
Running TestSuite
now start test
Test started runningtemplistener1at:1487125608088
i'm listenerTest1
Result success
Test started runningtemplistener2at:1487125608660
i'm listenerTest2
Result success
now finish test
Tests run: 2, Failures: 0, Errors: 0, Skipped: 0, Time elapsed: 7.125 sec - in TestSuite
Results :
Tests run: 2, Failures: 0, Errors: 0, Skipped: 0
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 01:13 min
[INFO] Finished at: 2017-02-15T10:26:51+08:00
[INFO] Final Memory: 22M/182M
[INFO] ------------------------------------------------------------------------
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
