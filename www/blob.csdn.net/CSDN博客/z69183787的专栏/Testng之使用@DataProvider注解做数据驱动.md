# Testng之使用@DataProvider注解做数据驱动 - z69183787的专栏 - CSDN博客
2016年10月14日 18:22:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5205
前两天学了一下@DataProvider，今天有时间总结一下。
testng很强大，提供了很多注解，其中利用@DataProvider可以做数据驱动，数据源文件可以是EXCEL，XML，甚至可以是TXT文本。下面以读取XML为例。
通过@DataProvider读取XML文件中数据，然后测试方法只要标示获取数据来源的DataProvider，那么对应的DataProvider会把读取的数据传给该test方法，我简单的画了一下整个流程（很丑，莫怪），下面通过代码具体看一下@DataProvider的使用。
![](http://upload-images.jianshu.io/upload_images/1237796-4868bbfd804b3d08.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
工具：
eclipse、TestNg、Selenium
场景：
登陆百度首页，点击登录，输入用户名和密码。
其中账号信息通过DataProvider将XML中的登陆账号和密码传给test方法
前提：
Selenium代码可以打开百度首页，访问登陆页面
###### 1、构建XML数据文件
LoginTest.xml
`xml<?xml version="1.0" encoding="UTF-8"?><data> <loginRight> <username>尹豆儿</username> <password>616189</password> </loginRight></data>`
注：XML文件名最好跟测试方法名称一致，方便了解其左右
###### 2、读取xml文件的方法
XmlUtil.java
```java
```java
package Tool;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;
public class XmlUtil {
    public static List getXmlComent(String fname){
        //获取xml文件完全路径
        String path=CommonFunction.getAbsolutionPath(fname);
        System.out.println("xml·path"+path);
        List contList=new ArrayList();
        //dom4j中读取xml文件的方法
        SAXReader saxR=new SAXReader();
        try {
            Document doc=saxR.read(path);
            //存放顶结点
            Element eleroot=doc.getRootElement();
            //parMap，存放顶结点下一级结点
            Map parMap=new HashMap();
            Map sonMap=new HashMap();
            for(Iterator i=eleroot.elementIterator();i.hasNext();){
                //parMap中存放的结点的子结点
                Element elepar=(Element)i.next();
                for(Iterator j=elepar.elementIterator();j.hasNext();){
                    Element eleSon=(Element)j.next();
                    System.out.println("+++++"+eleSon.getName()+"    "+ eleSon.getText());
                    sonMap.put(eleSon.getName(), eleSon.getText());
                }
                parMap.put(elepar.getName(),sonMap);
                System.out.println("*****"+elepar.getName());
                contList.add(parMap);
            }
        } catch (DocumentException e) {
            e.printStackTrace();
        }
        return contList;
    }
}
```
```
###### 3、编写DataProvider
LoginDataProvider.java
```java
```java
package dataProvider;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import org.testng.Assert;
import org.testng.annotations.DataProvider;
import testData.FilePath;
import Tool.XmlUtil;
public class LoginDataProvider {
    private static Object[][] obj;
    private static List parList=new ArrayList();
    private static List sonList=new ArrayList();
    //@DataProvider(name="loginProvider")
    @DataProvider()
    public static Object[][] loginProvider(Method method) {
        System.out.println("@DataProvider(name='loginProvider')");
        System.out.println("dataProvider"+method.getName());
        //读取xml内容
        parList = XmlUtil.getXmlComent(FilePath.LoginTestData);
        for (int i=0;i< parList.size();i++) {
            Map map = (Map)parList.get(i);
            if (map.containsKey(method.getName())) {
                Map<String,String> submap = (Map<String,String>) map.get(method.getName());
                sonList.add(submap);
            }
        }
        if (sonList.size() > 0) {
            obj = new Object[sonList.size()][];
            for (int i = 0; i < sonList.size(); i++) {
                obj[i] = new Object[] { sonList.get(i) };
            }
            return obj;
        }else{
            Assert.assertTrue(sonList.size()!=0,parList+"为空，找不到属性值："+method.getName() );
            return null;
        }
    }
}
```
```
###### 4、test方法中标明使用的DataProvider
LoginTest.java
```java
```java
package TestCase;
import java.awt.Frame;
import java.util.Date;
import java.util.Map;
import org.openqa.selenium.NoSuchFrameException;
import org.testng.Reporter;
import org.testng.annotations.Test;
import dataProvider.LoginDataProvider;
public class LoginTest extends TestBase{
    @Test(dataProvider="loginProvider",dataProviderClass=LoginDataProvider.class)
    public  void loginRight(Map<?, ?> param) throws InterruptedException{
        baiduPage.clickLogin();
        loginPage.login_Right((String)param.get("username"), (String)param.get("password"));
    }
}
```
```
###### 5、执行test用例查看是否取到数据
testng.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd" >
<suite name="Tests" verbose="5">
    <test name="test">
        <classes>
            <class name="TestCase.LoginTest"></class>
        </classes>
    </test>    
</suite>
```
总结一：
使用之后，个人觉得最巧妙的地方是xml方法中将参数值的父节点命名为test方法名，然后在@DataProvider中通过test的method方法名来同xml数据对应，而在test方法中使用Map<?,?> param 获取；相同结构的xml文件则可以使用同一个DataProvider，节省了不少代码。
![](http://upload-images.jianshu.io/upload_images/1237796-19ab36b50ccb56ed.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
xml文件
![](http://upload-images.jianshu.io/upload_images/1237796-eacb8413b40d3f6c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
test方法
![](http://upload-images.jianshu.io/upload_images/1237796-968d89f754038a00.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
@DataProvider
总结二：
在test方法中指明DataProvider，如果DataProvider有指明name值，那么在test中使用name，如果没有名字，使用对应的方面名
![](http://upload-images.jianshu.io/upload_images/1237796-3ab8c2a5bc6b1662.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
另外如果DataProvider和Test方法不再同一类中，如下指明
![](http://upload-images.jianshu.io/upload_images/1237796-6728ca5ce597be99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

