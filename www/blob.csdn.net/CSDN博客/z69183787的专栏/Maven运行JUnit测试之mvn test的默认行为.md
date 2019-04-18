# Maven运行JUnit测试之mvn test的默认行为 - z69183787的专栏 - CSDN博客
2016年09月27日 10:22:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5665
  参照[http://www.blogjava.net/sitinspring/archive/2007/06/20/125224.html](http://www.blogjava.net/sitinspring/archive/2007/06/20/125224.html)，用Maven跑JUnit类。但是运行mvn
 test，却报找不到任何测试类，即：There are no tests to run. 
       郁闷之际查了一下maven参考资料，发现原来是surefire插件的默认行为所致。maven是使用surefire插件执行测试的，它按照指定格式的类名来查找匹配的测试类，
> 默认包含的测试类：
**/*Test.java
**/Test*.java
**/*TestCase.java
默认排除的测试类：
**/Abstract*Test.java
**/Abstract*TestCase.java
       因此默认情况下，诸如“add_exist_department.java”或者“add_exist_departmentTests.java”这种JUnit类是不会被Maven发现并执行的，可按如下修改surefire插件的配置以达到包含"**/*Tests.java"测试类的目的：
<build>
   <plugins>
     <plugin>
       <groupId>org.apache.maven.plugins</groupId>
       <artifactId>maven-surefire-plugin</artifactId>
       <configuration>
         <includes>
           <include>**/*Tests.java</include>
         </includes>
         <excludes>
           <exclude>**/Abstract*.java</exclude>
         </excludes>
       </configuration>
     </plugin>
   </plugins>
 </build>
运行指定的测试类：
    >mvn test -Dtest=[ClassName]
运行测试类中指定的方法：(这个需要maven-surefire-plugin:2.7.3以上版本才能支持)
    >mvn test -Dtest=[ClassName]#[MethodName]
    [MethodName]为要运行的方法名，支持*通配符，范例：
   1） >mvn test -Dtest=MyClassTest#test1
   2） >mvn test -Dtest=MyClassTest#*test*
