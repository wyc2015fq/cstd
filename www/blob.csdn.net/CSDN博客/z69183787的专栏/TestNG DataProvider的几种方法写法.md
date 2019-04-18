# TestNG DataProvider的几种方法写法 - z69183787的专栏 - CSDN博客
2016年10月14日 18:21:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2113
前面写过一个无参的数据提供者方法，还有三种有参数据提供者方法。
一、Method表示当前测试的方法
Dataprovidertest.java代码
```java
package
```
```java
com.tzf.testng;
```
```java
import
```
```java
java.lang.reflect.Method;
```
```java
import
```
```java
org.testng.annotations.DataProvider;
```
```java
import
```
```java
org.testng.annotations.Test;
```
```java
public
```
```java
class
```
```java
DataProviderTest
 {
```
```java
```
```java
```
```java
@DataProvider
```
```java
```
```java
public
```
```java
Object[][]
 providerMethod(Method method){
```
```java
```
```java
Object[][]
 result =
```
```java
null
```
```java
;
```
```java
```
```java
if
```
```java
(method.getName().equals(
```
```java
"testmethod1"
```
```java
)){
```
```java
```
```java
result
 =
```
```java
new
```
```java
Object[][]{
```
```java
new
```
```java
Object[]{
```
```java
1
```
```java
}};
```
```java
```
```java
}
```
```java
else
```
```java
if
```
```java
(method.getName().equals(
```
```java
"testmethod2"
```
```java
)){
```
```java
```
```java
result
 =
```
```java
new
```
```java
Object[][]{
```
```java
new
```
```java
Object[]{
```
```java
2
```
```java
}};
```
```java
```
```java
}
```
```java
else
```
```java
{
```
```java
```
```java
result
 =
```
```java
new
```
```java
Object[][]{
```
```java
new
```
```java
Object[]{
```
```java
3
```
```java
}};
```
```java
```
```java
}
```
```java
```
```java
return
```
```java
result;
```
```java
```
```java
}
```
```java
```
```java
```
```java
@Test
```
```java
(dataProvider=
```
```java
"providerMethod"
```
```java
)
```
```java
```
```java
public
```
```java
void
```
```java
testmethod1(
```
```java
int
```
```java
param){
```
```java
```
```java
System.out.println(
```
```java
"method1
 received:"
```
```java
+param);
```
```java
```
```java
}
```
```java
```
```java
```
```java
@Test
```
```java
(dataProvider=
```
```java
"providerMethod"
```
```java
)
```
```java
```
```java
public
```
```java
void
```
```java
testmethod2(
```
```java
int
```
```java
param){
```
```java
```
```java
System.out.println(
```
```java
"method2
 received:"
```
```java
+param);
```
```java
```
```java
}
```
```java
```
```java
```
```java
@Test
```
```java
(dataProvider=
```
```java
"providerMethod"
```
```java
)
```
```java
```
```java
public
```
```java
void
```
```java
testmethod3(
```
```java
int
```
```java
param){
```
```java
```
```java
System.out.println(
```
```java
"method3
 received:"
```
```java
+param);
```
```java
```
```java
}
```
```java
}
```
 注意这里我的Dataprovider的name值没有指定，默认用的是方法名。 Testng.xml代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<suite name="Suite" parallel="false">
    <test name="DataProviderTest">
        <classes>
            <class name="com.tzf.testng.DataProviderTest" />
        </classes>
    </test>
</suite>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
运行结果代码
```
RemoteTestNG starting
method1 received:1
method2 received:2
method3 received:3
RemoteTestNG finishing: 145 ms
```
二、ITestContent当前测试的上下文信息
Dataprovidertest2.java代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.tzf.testng;
import java.util.Random;
import org.testng.ITestContext;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;
public class DataProviderTest2 {
    
    @DataProvider
    public Object[][] providerMethod(ITestContext context){
        String[] groups = context.getIncludedGroups();
        int size = 2;
        for(String group : groups){
            if(group.equals("function-test")){
                size = 10 ;
                break;
            }
        }
        Object[][] result = new Object[size][];
        Random r = new Random();
        for(int i=0;i<size;i++){
            result[i] = new Object[]{new Integer(r.nextInt())};
        }
        return result;
    }
    
    @Test(dataProvider="providerMethod",groups={"function-test","unit-test"})
    public void testmethod1(Integer n){
        System.out.println("method1 received:"+n);
    }
    
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
Testng.xml代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<suite name="Suite" parallel="false">
    <test name="DataProviderTest2">
        <groups>
            <run>
                <include name="function-test"/>
                <exclude name="unit-test"/>
            </run>
        </groups>
        <classes>
            <class name="com.tzf.testng.DataProviderTest2" group="function-test"/>
        </classes>
    </test>
</suite>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
运行结果代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
RemoteTestNG starting
method1 received:-1987654923
method1 received:338477914
method1 received:1910366338
method1 received:107205369
method1 received:-1018164959
method1 received:-2060129321
method1 received:-803464435
method1 received:-814156781
method1 received:-1075504680
method1 received:-774924512
RemoteTestNG finishing: 151 ms
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
