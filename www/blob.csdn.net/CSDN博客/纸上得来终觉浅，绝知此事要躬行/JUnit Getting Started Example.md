# JUnit Getting Started Example - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 20:41:51[boonya](https://me.csdn.net/boonya)阅读数：605标签：[Java																[junit																[download																[eclipse](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
个人分类：[Junit/Mock](https://blog.csdn.net/boonya/article/category/1731095)






### This article is from: [http://examples.javacodegeeks.com/core-java/junit/junit-getting-started-example/](http://examples.javacodegeeks.com/core-java/junit/junit-getting-started-example/)



### About Konstantina Dimtsa



![Konstantina Dimtsa](http://a5e2fba00d8bcb729d89839f.javacodegeeks.netdna-cdn.com/wp-content/uploads/2013/10/Konstantina-Dimtsa-60x60.jpg)


Konstantina has graduated from the Department of Informatics and Telecommunications in National and Kapodistrian University of Athens (NKUA) and she is currently pursuing M.Sc studies in Advanced Information Systems at the same department. She is also working
 as a research associate for NKUA in the field of telecommunications. Her main interests lie in software engineering, web applications, databases and telecommunications.





In this example we are going to introduce some basic annotations and asserts of [JUnit4](http://junit.org/). JUnit4 is a test framework which is used by developers so
 that they can be ensured that their code works as expected.

## 1. Create Junit Test


Open Eclipse IDE and create a new Java project called JunitFirstTest.

It would be better to create a new source folder to place your test classes and not combine them with the java classes in the src folder. For that purpose, right click
 on the project and select New → Source Folder so as to create test folder.

![02-Nov-13 5-39-37 PM](http://a5e2fba00d8bcb729d89839f.javacodegeeks.netdna-cdn.com/wp-content/uploads/2013/11/02-Nov-13-5-39-37-PM.png)


In the screenshot above we can see the structure of the project after the creation of the new source folder.


In the test folder, first create a new package `com.javacodegeeks.junit` and
 then create a Junit Test by right-clicking on the previous package and selecting New → Junit Test Case.

![02-Nov-13 5-03-59 PM22](http://a5e2fba00d8bcb729d89839f.javacodegeeks.netdna-cdn.com/wp-content/uploads/2013/11/02-Nov-13-5-03-59-PM22.png)


If your project does not contain the JUnit library in its classpath, the following message will be displayed so as to add the JUnit library to the classpath.

![02-Nov-13 5-49-38 PM22](http://a5e2fba00d8bcb729d89839f.javacodegeeks.netdna-cdn.com/wp-content/uploads/2013/11/02-Nov-13-5-49-38-PM22.png)


Now create a test with the code below.

JunitFirstTest.java:



|`01`|`package``com.javacodegeeks.junit;`|
|----|----|


|`02`||
|----|----|


|`03`|`import``static``org.junit.Assert.*;`|
|----|----|


|`04`|`import``java.util.*;`|
|----|----|


|`05`|`import``org.junit.*;`|
|----|----|


|`06`||
|----|----|


|`07`|`public``class````JunitFirstTest {```|
|----|----|


|`08`||
|----|----|


|`09`|`    ``private````ArrayList<String> testList;```|
|----|----|


|`10`||
|----|----|


|`11`|`    ``@BeforeClass`|
|----|----|


|`12`|`    ``public``static``void````onceExecutedBeforeAll() {```|
|----|----|


|`13`|`        ``System.out.println(````"@BeforeClass: onceExecutedBeforeAll"````);`|
|----|----|


|`14`|`    ``}`|
|----|----|


|`15`||
|----|----|


|`16`|`    ``@Before`|
|----|----|


|`17`|`    ``public``void````executedBeforeEach() {```|
|----|----|


|`18`|`        ````testList =````new``ArrayList<String>();`|
|----|----|


|`19`|`        ``System.out.println(````"@Before: executedBeforeEach"````);`|
|----|----|


|`20`|`    ``}`|
|----|----|


|`21`||
|----|----|


|`22`|`    ``@AfterClass`|
|----|----|


|`23`|`    ``public``static``void````onceExecutedAfterAll() {```|
|----|----|


|`24`|`        ``System.out.println(````"@AfterClass: onceExecutedAfterAll"````);`|
|----|----|


|`25`|`    ``}`|
|----|----|


|`26`||
|----|----|


|`27`|`    ``@After`|
|----|----|


|`28`|`    ``public``void````executedAfterEach() {```|
|----|----|


|`29`|`        ``testList.clear();`|
|----|----|


|`30`|`        ``System.out.println(````"@After: executedAfterEach"````);`|
|----|----|


|`31`|`    ``}`|
|----|----|


|`32`||
|----|----|


|`33`|`    ``@Test`|
|----|----|


|`34`|`    ``public``void````EmptyCollection() {```|
|----|----|


|`35`|`        ``assertTrue(testList.isEmpty());`|
|----|----|


|`36`|`        ``System.out.println(````"@Test: EmptyArrayList"````);`|
|----|----|


|`37`||
|----|----|


|`38`|`    ``}`|
|----|----|


|`39`||
|----|----|


|`40`|`    ``@Test`|
|----|----|


|`41`|`    ``public``void````OneItemCollection() {```|
|----|----|


|`42`|`        ``testList.add(``"oneItem"``);`|
|----|----|


|`43`|`        ``assertEquals(``1````, testList.size());```|
|----|----|


|`44`|`        ``System.out.println(````"@Test: OneItemArrayList"````);`|
|----|----|


|`45`|`    ``}`|
|----|----|


|`46`|`}`|
|----|----|





Let’s describe the annotations in the above code in detail.
|```@Testpublic void method()```|The Test annotation indicates that the public void method to which it is attached can be run as a test case.|
|----|----|
|```@Beforepublic void method()```|The Before annotation indicates that this method must be executed before each test in the class, so as to execute some preconditions necessary for the test.|
|```@BeforeClasspublic static void method()```|The BeforeClass annotation indicates that the static method to which is attached must be executed once and before all tests in the class. That happens when the test methods share computationally expensive setup (e.g. connect to database).|
|```@Afterpublic void method()```|The After annotation indicates that this method gets executed after execution of each test (e.g. reset some variables after execution of every test, delete temporary variables etc)|
|```@AfterClasspublic static void method()```|The AfterClass annotation can be used when a method needs to be executed after executing all the tests in a JUnit Test Case class so as to clean-up the expensive set-up (e.g disconnect from a database). Attention: The method attached with this annotation (similar to BeforeClass) must be defined as static.|
|assertTrue (String message, boolean condition)|Asserts that the condition is true.|
|assertEquals ([String message], object expected, object actual)|Asserts that the two objects are equal.|

## 2. Run your test in Eclipse


Now, run your test in Eclipse, so as to see the results. Right-click on your JunitFirstTest class and select Run As→JUnit Test.

### Outputs:

```
@BeforeClass: onceExecutedBeforeAll
@Before: executedBeforeEach
@Test: EmptyArrayList
@After: executedAfterEach
@Before: executedBeforeEach
@Test: OneItemArrayList
@After: executedAfterEach
@AfterClass: onceExecutedAfterAll
```


As we see in the results, the sequence of the executed methods complies with what we described in the previous section.

## Download the Eclipse Project


This was an introduction to JUnit basic annotations and assert statements. Download the Eclipse Project of this example : [JunitFirstTest.zip](http://a5e2fba00d8bcb729d89839f.javacodegeeks.netdna-cdn.com/wp-content/uploads/2013/11/JunitFirstTest.zip)](https://so.csdn.net/so/search/s.do?q=download&t=blog)](https://so.csdn.net/so/search/s.do?q=junit&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)




