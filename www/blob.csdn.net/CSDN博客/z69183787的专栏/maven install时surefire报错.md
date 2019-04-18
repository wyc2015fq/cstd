# maven install时surefire报错 - z69183787的专栏 - CSDN博客
2016年05月23日 14:27:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2538
maven install时surefire报错
错误信息如下：
警告: Error injecting: org.apache.maven.plugin.surefire.SurefirePlugin
java.lang.NoClassDefFoundError: org/apache/maven/surefire/util/NestedCheckedException
at java.lang.ClassLoader.defineClass1(Native Method)
.............
[INFO] BUILD FAILURE
......
[ERROR] Failed to execute goal org.apache.maven.plugins:maven-surefire-plugin:2.10:test
......
我修改了maven的本地库路径后执行maven install时出现了这个错误，我把原先没问题的maven库中全部依赖拷贝到新的库路径后又正常了，怀疑maven库中的依赖包有问题。
经过逐个排除后，最后锁定出问题的是一个pom文件。
repository\org\apache\maven\surefire\surefire-booter\2.10\surefire-booter-2.10.pom
正常的版本这个文件是3KB，而报错的这个是1KB，替换该文件后不再报错。
**surefire-booter-2.10.pom**
**[html]**[view
 plain](http://blog.csdn.net/prstaxy/article/details/46862757#)[copy](http://blog.csdn.net/prstaxy/article/details/46862757#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!--  
-   ~ Licensed to the Apache Software Foundation (ASF) under one  
-   ~ or more contributor license agreements.  See the NOTICE file  
-   ~ distributed with this work for additional information  
-   ~ regarding copyright ownership.  The ASF licenses this file  
-   ~ to you under the Apache License, Version 2.0 (the  
-   ~ "License"); you may not use this file except in compliance  
-   ~ with the License.  You may obtain a copy of the License at  
-   ~  
-   ~     http://www.apache.org/licenses/LICENSE-2.0  
-   ~  
-   ~ Unless required by applicable law or agreed to in writing,  
-   ~ software distributed under the License is distributed on an  
-   ~ "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY  
-   ~ KIND, either express or implied.  See the License for the  
-   ~ specific language governing permissions and limitations  
-   ~ under the License.  
-   -->
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <modelVersion>4.0.0</modelVersion>
- 
- <parent>
- <groupId>org.apache.maven.surefire</groupId>
- <artifactId>surefire</artifactId>
- <version>2.10</version>
- <relativePath>../pom.xml</relativePath>
- </parent>
- 
- <artifactId>surefire-booter</artifactId>
- 
- <name>SureFire Booter</name>
- 
- <dependencies>
- <dependency>
- <groupId>org.apache.maven.surefire</groupId>
- <artifactId>surefire-api</artifactId>
- </dependency>
- </dependencies>
- 
- <build>
- <plugins>
- <plugin>
- <artifactId>maven-surefire-plugin</artifactId>
- <dependencies>
- <dependency>
- <groupId>org.apache.maven.surefire</groupId>
- <artifactId>surefire-shadefire</artifactId>
- <version>${project.version}</version>
- </dependency>
- </dependencies>
- </plugin>
- <plugin>
- <artifactId>maven-compiler-plugin</artifactId>
- <configuration>
- <source>1.3</source>
- <target>1.3</target>
- </configuration>
- </plugin>
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-shade-plugin</artifactId>
- <executions>
- <execution>
- <phase>package</phase>
- <goals>
- <goal>shade</goal>
- </goals>
- <configuration>
- <minimizeJar>true</minimizeJar>
- <artifactSet>
- <includes>
- <include>commons-lang:commons-lang</include>
- </includes>
- </artifactSet>
- <relocations>
- <relocation>
- <pattern>org.apache.commons.lang</pattern>
- <shadedPattern>org.apache.maven.surefire.shade.org.apache.commons.lang</shadedPattern>
- </relocation>
- </relocations>
- </configuration>
- </execution>
- </executions>
- </plugin>
- </plugins>
- </build>
- </project>
- 
