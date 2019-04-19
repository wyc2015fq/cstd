# maven-assembly-plugin把java工程打包成为一个可执行的jar包 - 零度的博客专栏 - CSDN博客
2017年02月27日 16:55:12[零度anngle](https://me.csdn.net/zmx729618)阅读数：1430
        用java写了一个小工具，使用maven java 工程。写完后，想打包成一个可执行的jar包，使用如下方法： 
1、使用maven的插件**maven-assembly-plugin**
pom.xml里添加
```xml
```
```
<build>
      <plugins>
        <plugin>
          <artifactId>maven-assembly-plugin</artifactId>
          <configuration>
            <archive>
              <manifest>
                <mainClass>org.kuan.wang.App</mainClass>
              </manifest>
            </archive>
            <descriptorRefs>
              <descriptorRef>jar-with-dependencies</descriptorRef>
            </descriptorRefs>
          </configuration>
        </plugin>
      </plugins>
    </build>
```
执行命令：
```mvn clean compile assembly:single`
在target文件夹里生成**MavenTestApp-1.0-SNAPSHOT-jar-with-dependencies.jar,**这个jar包便是可执行的jar了，用命令`java -jar path\jarfile`便可以执行。 
**注意：compile必须再assembly:single之前，要不然你工程的代码（class文件）就不会被打包进去**。assembly:single是maven的goal。
2、通常情况下，这个goal要绑定到maven build 的phase里，这样它就可以自动执行。mvn package是用来打包jar包的，只是jar包是不可执行（因为不包括其他依赖）。
我们在以上的pom文件里加`<executions>，来将所有依赖也打包进去就可以执行了。`
```xml
```
```
<plugin>
  <artifactId>maven-assembly-plugin</artifactId>
  <configuration>
    <archive>
      <manifest>
        <mainClass>org.kuan.wang.App</mainClass>
      </manifest>
    </archive>
    <descriptorRefs>
      <descriptorRef>jar-with-dependencies</descriptorRef>
    </descriptorRefs>
  </configuration>
  <executions>
    <execution>
      <id>make-assembly</id> <!-- this is used for inheritance merges -->
      <phase>package</phase> <!-- bind to the packaging phase -->
      <goals>
        <goal>single</goal>
      </goals>
    </execution>
  </executions>
</plugin>
```
如此，就直接运行以下命令即可。
```mvn clean package`
3、另外有几点要注意：
1. 必须要有
```xml
```
```
<archive>
     <manifest>
          <mainClass>org.kuan.wang.App</mainClass>
     </manifest>
</archive>
```
    否则会报错 ：n**o main manifest attribute, in target\MavenTestApp-1.0-SNAPSHOT-jar-with-dependencies.jar  **`<mainClass>`是你自己工程的入口。
2. 使用`mvn clean compile assembly:single`，只生成一个jar包，其名字里有jar-with-dependencies，是可以直接执行的。
    使用`mvn package`生成两个jar包，名字里有jar-with-dependencies的jar是可执行的。 
3. 如果使用`<goal>assembly</goal>` 会生成两个jar包。跟使用了`mvn package`一样，无需再像命令`assembly:single`一样前面必须加compile.
