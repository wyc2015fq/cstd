
# JavaAgent 实现字节码注入 - 无界 - CSDN博客

2018年07月27日 18:55:57[21aspnet](https://me.csdn.net/21aspnet)阅读数：1271


新建MyAgent项目
![](https://img-blog.csdn.net/20180727185135674?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
pom文件
```python
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.book.MyAgent</groupId>
    <artifactId>MyAgent</artifactId>
    <version>1.0</version>
    <dependencies>
        <dependency>
            <groupId>javassist</groupId>
            <artifactId>javassist</artifactId>
            <version>3.12.1.GA</version>
        </dependency>
        <dependency>
            <groupId>org.ow2.asm</groupId>
            <artifactId>asm-all</artifactId>
            <version>5.1</version>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <source>1.8</source>
                    <target>1.8</target>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-source-plugin</artifactId>
                <version>3.0.1</version>
                <executions>
                    <execution>
                        <id>attach-sources</id>
                        <phase>verify</phase>
                        <goals>
                            <goal>jar-no-fork</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-assembly-plugin</artifactId>
                <version>2.6</version>
                <configuration>
                    <descriptorRefs>
                        <descriptorRef>jar-with-dependencies</descriptorRef>
                    </descriptorRefs>
                    <archive>
                        <manifestFile>src/main/resources/META-INF/MANIFEST.MF</manifestFile>
                    </archive>
                </configuration>
                <executions>
                    <execution>
                        <id>assemble-all</id>
                        <phase>package</phase>
                        <goals>
                            <goal>single</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
        <resources>
            <resource>
                <directory>${basedir}/src/main/resources</directory>
            </resource>
            <resource>
                <directory>${basedir}/src/main/java</directory>
            </resource>
        </resources>
    </build>
</project>
```
AgentMain.java
```python
import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;
import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;
public class AgentMain {
	public static void premain(String agentOps, Instrumentation inst) {
            System.out.println("=========premain方法执行========");
            System.out.println(agentOps);
            //instrument(agentOps, inst);
	}
	public static void agentmain(String agentOps, Instrumentation inst) {
		instrument(agentOps, inst);
	}
```
src\main\resources\META-INF下新建MANIFEST.MF
```python
Manifest-Version: 1.0
Premain-Class: com.book.myagent.AgentMain
Agent-Class: com.book.myagent.AgentMain
Can-Redefine-Classes: true
```
注意会生成3个jar
![](https://img-blog.csdn.net/20180727185043980?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
新建第二个项目Test：

```python
package com.book.test;

public class HelloWorld {
    public static void main(String[] arg) {
    System.out.println("输出>>>HelloWorld");
    }
}
```
pom.xml
```python
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.book</groupId>
    <artifactId>Test</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>jar</packaging>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <maven.compiler.source>1.8</maven.compiler.source>
        <maven.compiler.target>1.8</maven.compiler.target>
    </properties>
    <build>
        <plugins>
            <plugin>    
                <artifactId>maven-assembly-plugin</artifactId>    
                <configuration>    
                    <appendAssemblyId>false</appendAssemblyId>    
                    <descriptorRefs>    
                        <descriptorRef>jar-with-dependencies</descriptorRef>    
                    </descriptorRefs>    
                    <archive>    
                        <manifest>    
                            <mainClass>com.book.test.HelloWorld</mainClass>    
                        </manifest>    
                    </archive>    
                </configuration>    
                <executions>    
                    <execution>    
                        <id>make-assembly</id>    
                        <phase>package</phase>    
                        <goals>    
                            <goal>assembly</goal>    
                        </goals>    
                    </execution>    
                </executions>    
            </plugin>
        </plugins>
    </build>
</project>
```
java  -javaagent:E:\worktest\MyAgent\target\MyAgent-1.0-jar-with-dependencies.jar -jar E:\worktest\Test\target\Test-1.0-SNAPSHOT.jar com.book.test.HelloWorld
执行：
java  -javaagent:E:\worktest\MyAgent\target\MyAgent-1.0-jar-with-dependencies.jar -jar E:\worktest\Test\target\Test-1.0-SNAPSHOT.jar com.book.test.HelloWorld
注意不在pom设置默认主类可以用 java -cp启动
![](https://img-blog.csdn.net/20180727185422935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考：[https://github.com/liuzhengyang/javaagent-example](https://github.com/liuzhengyang/javaagent-example)
[https://blog.csdn.net/catoop/article/details/51034739](https://blog.csdn.net/catoop/article/details/51034739)

