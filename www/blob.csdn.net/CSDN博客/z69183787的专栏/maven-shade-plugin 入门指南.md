# maven-shade-plugin 入门指南 - z69183787的专栏 - CSDN博客
2017年07月24日 11:55:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：300
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)

## 1. Why?
通过 maven-shade-plugin 生成一个 uber-jar，它包含所有的依赖 jar 包。
## 2. Goals
|Goal|Description|
|----|----|
|[shade:help](http://maven.apache.org/plugins/maven-shade-plugin/help-mojo.html)|Display help information on maven-shade-plugin.Callmvn shade:help -Ddetail=true -Dgoal=<goal-name>to display parameter details.|
|[shade:shade](http://maven.apache.org/plugins/maven-shade-plugin/shade-mojo.html)|Mojo that performs shading delegating to the Shader component.|
## 3. Usage
- 配置 maven-shade-plugin
maven-shade-plugin 将 goal shade:shade 绑定到 phase package 上。
```xml
<build>
     <plugins>
         <plugin>
             <groupId>org.apache.maven.plugins</groupId>
             <artifactId>maven-shade-plugin</artifactId>
             <version>2.4.3</version>
             <configuration>
                 <!-- put your configurations here -->
             </configuration>
             <executions>
                 <execution>
                     <phase>package</phase>
                     <goals>
                         <goal>shade</goal>
                     </goals>
                 </execution>
             </executions>
         </plugin>
     </plugins>
 </build>
```
- 执行命令
`mvn clean package`
会在 target 文件生成一个 uber-jar，以 -shaded.jar 为后缀的 jar 包。
## 4. Examples
- Selecting Contents for Uber JAR
将该工程依赖的部分 Jar 包 include/exclude 掉。
```xml
<build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-shade-plugin</artifactId>
        <version>2.4.3</version>
        <executions>
          <execution>
            <phase>package</phase>
            <goals>
              <goal>shade</goal>
            </goals>
            <configuration>
              <artifactSet>
                <excludes>
                  <exclude>classworlds:classworlds</exclude>
                  <exclude>junit:junit</exclude>
                  <exclude>jmock:*</exclude>
                  <exclude>*:xml-apis</exclude>
                  <exclude>org.apache.maven:lib:tests</exclude>
                  <exclude>log4j:log4j:jar:</exclude>
                </excludes>
              </artifactSet>
            </configuration>
          </execution>
        </executions>
      </plugin>
    </plugins>
</build>
```
将依赖的某个 Jar 包内部的类或者资源 include/exclude 掉。
```xml
<build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-shade-plugin</artifactId>
        <version>2.4.3</version>
        <executions>
          <execution>
            <phase>package</phase>
            <goals>
              <goal>shade</goal>
            </goals>
            <configuration>
              <filters>
                <filter>
                  <artifact>junit:junit</artifact>
                  <includes>
                    <include>junit/framework/**</include>
                    <include>org/junit/**</include>
                  </includes>
                  <excludes>
                    <exclude>org/junit/experimental/**</exclude>
                    <exclude>org/junit/runners/**</exclude>
                  </excludes>
                </filter>
                <filter>
                  <artifact>*:*</artifact>
                  <excludes>
                    <exclude>META-INF/*.SF</exclude>
                    <exclude>META-INF/*.DSA</exclude>
                    <exclude>META-INF/*.RSA</exclude>
                  </excludes>
                </filter>
              </filters>
            </configuration>
          </execution>
        </executions>
      </plugin>
    </plugins>
</build>
```
maven-shade-plugin 自动将所有不使用的类全部排除掉，将 uber-jar 最小化。
```xml
<build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-shade-plugin</artifactId>
        <version>2.4.3</version>
        <executions>
          <execution>
            <phase>package</phase>
            <goals>
              <goal>shade</goal>
            </goals>
            <configuration>
              <minimizeJar>true</minimizeJar>
            </configuration>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>
```
- Attaching the Shaded Artifact
默认会生成一个Jar包和一个以 "-shaded"为结尾的uber-jar包，可以通过配置来指定uber-jar的后缀名。
```xml
<build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-shade-plugin</artifactId>
        <version>2.4.3</version>
        <executions>
          <execution>
            <phase>package</phase>
            <goals>
              <goal>shade</goal>
            </goals>
            <configuration>
              <shadedArtifactAttached>true</shadedArtifactAttached>
              <shadedClassifierName>jackofall</shadedClassifierName> <!-- Any name that makes sense -->
            </configuration>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>
```
- Executable JAR
通过设置 MainClass 创建一个可执行 Jar 包。
```xml
<build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-shade-plugin</artifactId>
        <version>2.4.3</version>
        <executions>
          <execution>
            <phase>package</phase>
            <goals>
              <goal>shade</goal>
            </goals>
            <configuration>
              <transformers>
                <transformer implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
                  <mainClass>org.sonatype.haven.HavenCli</mainClass>
                </transformer>
              </transformers>
            </configuration>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>
```
- Relocating Classes
Java 工程经常会遇到第三方 Jar 包冲突，使用 maven shade plugin 解决 jar 或类的多版本冲突。 maven-shade-plugin 在打包时，可以将项目中依赖的 jar 包中的一些类文件打包到项目构建生成的 jar 包中，在打包的时候把类重命名。下面的配置将 org.codehaus.plexus.util jar 包重命名为 org.shaded.plexus.util。
```xml
<build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-shade-plugin</artifactId>
        <version>2.4.3</version>
        <executions>
          <execution>
            <phase>package</phase>
            <goals>
              <goal>shade</goal>
            </goals>
            <configuration>
              <relocations>
                <relocation>
                  <pattern>org.codehaus.plexus.util</pattern>
                  <shadedPattern>org.shaded.plexus.util</shadedPattern>
                  <excludes>
                    <exclude>org.codehaus.plexus.util.xml.Xpp3Dom</exclude>
                    <exclude>org.codehaus.plexus.util.xml.pull.*</exclude>
                  </excludes>
                </relocation>
              </relocations>
            </configuration>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>
```
## 参考
[maven 入门指南](http://www.jianshu.com/p/b4ef9978d85d)
[maven 生命周期](http://www.jianshu.com/p/fd43b3d0fdb0)
[Maven 默认插件以及功能](http://www.jianshu.com/p/977b71e2837e)
[maven 依赖管理](http://www.jianshu.com/p/f6ca45865025)
[maven-shade-plugin 入门指南](http://www.jianshu.com/p/7a0e20b30401)
[maven-assembly-plugin 入门指南](http://www.jianshu.com/p/14bcb17b99e0)
作者：要饭真得好难
链接：http://www.jianshu.com/p/7a0e20b30401
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

1. maven-shade-plugin
2. maven-assembly-plugin
3. maven-onejar-plugin
maven-shade-plugin是我在ebay时前辈介绍给我的，我觉得它使用方便且没有出现过问题。但是我看别人的源代码，发现大家用的更多的是assembly，所以这里总结下这两种插件的用法。至于第三个，先留个坑在这，以后用到再总结。
使用插件maven-shade-plugin可以方便的将项目已jar包的方式导出，插件的好处在于它会把项目所依赖的其他jar包都封装起来，这种jar包放在任何JVM上都可以直接运行，我最初使用eclipse的maven-build直接打包，转移到intellij idea后没有这个按钮了，就只能用命令行搞了
使用步骤 ：
将插件添加到pom.xml中，需要改的地方就是mainClass，在这里指定main方法的位置
使用mvn package打包，最后到projectName/target/下查找目标jar包
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-shade-plugin</artifactId>
                <version>2.3</version>
                <executions>
                    <execution>
                        <phase>package</phase>
                        <goals>
                            <goal>shade</goal>
                        </goals>
                        <configuration>
                            <transformers>
                                <transformer implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
                                    <mainClass>core.Test</mainClass>
                                </transformer>
                            </transformers>
                        </configuration>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
Assembly 插件
pom.xml 文件
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!-- Maven Assembly Plugin -->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-assembly-plugin</artifactId>
                <version>2.4.1</version>
                <configuration>
                    <!-- get all project dependencies -->
                    <descriptorRefs>
                        <descriptorRef>jar-with-dependencies</descriptorRef>
                    </descriptorRefs>
                    <!-- MainClass in mainfest make a executable jar -->
                    <archive>
                      <manifest>
                        <mainClass>com.mkyong.core.utils.App</mainClass>
                      </manifest>
                    </archive>
 
                </configuration>
                <executions>
                  <execution>
                    <id>make-assembly</id>
                                        <!-- bind to the packaging phase -->
                    <phase>package</phase> 
                    <goals>
                        <goal>single</goal>
                    </goals>
                  </execution>
                </executions>
            </plugin>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
