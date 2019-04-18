# jenkins通过maven指定testng的xml文件，并给testng代码传参 - z69183787的专栏 - CSDN博客
2018年06月07日 10:29:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：779
[https://www.cnblogs.com/qiaoyeye/p/8682923.html](https://www.cnblogs.com/qiaoyeye/p/8682923.html)
1.jenkins设置参数化构建，设置要传的参数名和值
![](https://images2018.cnblogs.com/blog/759305/201803/759305-20180331164842013-1771189649.png)
2.指定testng的xml文件，在jenkins的输入以下
![](https://images2018.cnblogs.com/blog/759305/201803/759305-20180331164949776-924734629.png)
3.在pom.xml文件分别引用jenkins的参数，设置两个property
```
<xml.file>${env.xmlFileName}</xml.file>
        <testType>${env.testType}</testType>
```
4.在pom.xml中引用，maven-surefire-plugin的configuration添加如下
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
  <systemPropertyVariables>
                        <propertyName>${testType}}</propertyName>
                    </systemPropertyVariables>
                    <suiteXmlFiles>
                        <suiteXmlFile>src/test/${xml.file}</suiteXmlFile>
                    </suiteXmlFiles>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
5.在testng.xml中引用testType变量
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
   <parameter name="testType" value="${testType}"/>
    <test name="可用性测试">
        <classes>
        <class name="common.testsuites.TempTest"></class>
        </classes>
    </test>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
6.新建测试类
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
/**
 * Created by qiaojiafei on 2018/3/31.
 */
public class TempTest {
    public String testType;
    @Parameters({ "testType" })
    @BeforeMethod
    public void beforeMethod(String testType) {
        System.out.println("++++++------------------"+testType);
        this.testType = testType;
    }
    @BeforeClass
    public void beforeClass() {
        
    }
    @Test(groups = { "usabilitytest" })
    public void tt() {
        System.out.println(testType);
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
7.执行jenkins，可以看到运行了指定的testng.xml文件，并且把testType给传到了代码中
![](https://images2018.cnblogs.com/blog/759305/201803/759305-20180331165058621-227893476.png)
