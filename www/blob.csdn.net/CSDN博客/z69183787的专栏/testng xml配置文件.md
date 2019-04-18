# testng xml配置文件 - z69183787的专栏 - CSDN博客
2018年08月20日 12:23:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：147
# 简单介绍
运行TestNG测试脚本有两种方式：一种是直接通过IDE运行（例如使用eclipse中的“Run TestNG tests”），另一种是从命令行运行（通过使用xml配置文件）。当我们想执行某个包或者某个类中的一部分测试脚本的时候，使用xml配置文件是非常便利的。在xml配置文件里，不仅可以选择某些需要执行的测试脚本，还可以排除某些不需要运行的测试脚本。
# 创建testng.xml文件
创建xml文件很容易，只需要在其中填充一些内容。
1）首先要声明一个suite的名字，用于描述将要运行的测试脚本集，可以根据自己需要任意命名，最终这个名字会在testng的测试报告中看到。
<!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd" >
<suite name="SuiteName" verbose="1" >  
    <test name="TestName" >
2）如果选择的测试脚本是基于组的（使用了@Test (groups={"student"})这样的注解），那么接下来需要声明如何使用这些组：包含或者排除。如果使用了include标签标注某些组，那么在选择的测试脚本中，只有属于那些组的测试脚本会被运行。那些未被选中的测试脚本，或者被选中却不属于某些组的测试脚本都不会被运行。需要注意，一个测试脚本可以属于很多个组，只要有一个组被include标签标注，那么它就会被运行。如果使用了exclude标签标注某些组，那么在选择的脚本中，只有不属于那些组的测试脚本会被运行。如果同时使用了include标签和exclude标签，那么拥有被include标注的组的那些脚本会被运行，拥有被exclude标注的脚本不会被运行。有一个例外是，一个组同时被include和exclude标注了，那么拥有这个组的脚本会被运行。include和exclude标签的使用方式如下：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<groups>
  <run>
     <include name = "includedGroupName" />
     <exclude name = "excludedGroupName" />
  </run>
</groups>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
3）选择测试脚本可以从包、类、方法三个层级进行。
选择一个包
<packages>
    <package name = "packageName" />
</packages>
选择一个类
<classes>
    <class name = "className" />
</classes>
选择一个方法
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<classes>
    <class name = "className" />
       <methods>
          <include name = "methodName" />
       </methods>
    </class>
</classes>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
# xml文件样例
1）选择一个包中的全部测试脚本（包含子包）
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd" >
<suite name="First suite" verbose="1" >
<test name = "allTestsInAPackage" >
   <packages>
      <package name = "whole.path.to.package.* />
   </packages>
</test>
</suite>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
2）选择一个类中的全部测试脚本
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd" >
<suite name="Second suite" verbose="1" >
<test name = "allTestsInAClass" >
   <classes>
  <class name="whole.path.to.package.className />
   </classes>
</test>
</suite>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
3）选择一个类中的部分测试脚本
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd" >
<suite name="Third suite" verbose="1" >
<test name = "aFewTestsFromAClass" >
   <classes>
  <class name="whole.path.to.package.className >
      <methods>
         <include name = "firstMethod" />
         <include name = "secondMethod" />
         <include name = "thirdMethod" />
      </methods>
  </class>
   </classes>
</test>
</suite>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
4）选择一个包中的某些组
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd" >
<suite name="Fourth suite" verbose="1" >
<test name = "includedGroupsInAPackage" >
   <groups>
      <run>
         <include name = "includedGroup" />
      </run>
   </groups>
   <packages>
      <package name = "whole.path.to.package.* />
   </packages>
</test>
</suite>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
5）排除一个包中的某些组
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd" >
<suite name="Fifth suite" verbose="1" >
<test name = "excludedGroupsInAPackage" >
   <groups>
      <run>
         <exclude name = "excludedGroup" />
      </run>
   </groups>
   <packages>
      <package name = "whole.path.to.package.* />
   </packages>
</test>
</suite>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
# 在maven的pom.xml文件中配置testng.xml
需要在pom文件中，指明testng.xml文件的位置。
maven使用surefire这个插件进行测试，可以执行testng或者Junit脚本。
语法为 <suiteXmlFile>src/test/resources/testNGFilesFolder/${testNgFileName}.xml</suiteXmlFile> 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    <dependencies>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-api</artifactId>
            <version>2.8</version>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-surefire-plugin</artifactId>
                <version>2.19</version>
                <configuration>
                    <suiteXmlFiles>
                        <suiteXmlFile>testng.xml</suiteXmlFile>//该文件位于工程根目录时，直接填写名字，其它位置要加上路径。
                    </suiteXmlFiles>
                </configuration>
            </plugin>
        </plugins>
    </build>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
# 运行测试脚本
方法一：在IDE，例如IntellJ IDEA中，鼠标右击testng.xml文件，选择run即可。
方法二：进入到项目工程的根目录，使用 mvn clean test -Dtestng.xml 命令，结果如下：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
[INFO] Scanning for projects...
[INFO]                                                                         
[INFO] ------------------------------------------------------------------------
[INFO] Building ArtifactIdWang 1.0-SNAPSHOT
[INFO] ------------------------------------------------------------------------
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
