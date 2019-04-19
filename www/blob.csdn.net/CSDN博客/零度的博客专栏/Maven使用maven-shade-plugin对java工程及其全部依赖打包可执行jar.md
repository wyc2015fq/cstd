# Maven使用maven-shade-plugin对java工程及其全部依赖打包可执行jar - 零度的博客专栏 - CSDN博客
2017年09月07日 10:30:05[零度anngle](https://me.csdn.net/zmx729618)阅读数：304标签：[maven																[jar](https://so.csdn.net/so/search/s.do?q=jar&t=blog)](https://so.csdn.net/so/search/s.do?q=maven&t=blog)
个人分类：[Maven](https://blog.csdn.net/zmx729618/article/category/6256762)
       采用 maven 来进行开发管理，有一个需求是需要把通过maven管理的 [Java](http://lib.csdn.net/base/java) 工程打成可执行的
 jar 包，这样也就是说必需把工程依赖的 jar 包也一起打包。而使用maven默认的package命令构建的jar包中只包括了工程自身的 class 文件，并没有包括依赖的 jar 包。
       我们可以通过配置插件：maven-assembly-plugin (使用此插件会有一些问题) 来对工程进行打包，pom具体配置如下：
```
<plugin>  
    <artifactId>maven-assembly-plugin</artifactId>  
    <configuration>  
        <appendAssemblyId>false</appendAssemblyId>  
        <descriptorRefs>  
            <descriptorRef>jar-with-dependencies</descriptorRef>  
        </descriptorRefs>  
        <archive>  
            <manifest>  
                <mainClass>com.chenzhou.examples.Main</mainClass>  
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
```
       其中 <mainClass></mainClass> 的值表示此工程的入口类，也就是包含 main 方法的类，在我的例子中就是 com.chenzhou.examples.Main。配置完 pom 后可以通过执行 mvn assembly:assembly 命令来启动插件进行构建。构建成功后会生成 jar 包，这样我们就可以在命令行中通过
 java -jar XXX.jar 来运行 jar 文件了。 
      不过使用此插件会有一些问题：我在工程中依赖了 [spring](http://lib.csdn.net/base/javaee) 框架的
 jar 包，我打包成功后使用命令来调用 jar 包时报错如下（内网环境）：
```
org.xml.sax.SAXParseException: schema_reference.4: Failed to read schema document 'http://www.springframework.org/schema/beans/spring-beans-3.0.xsd', 
because 1) could not find the document; 
2) the document could not be read; 
3) the root element of the document is not <xsd:schema>.
```
        关于此问题报错的原因，我在网上找到一篇文章对此有比较详细的解释：[http://blog.csdn.net/bluishglc/article/details/7596118](http://blog.csdn.net/bluishglc/article/details/7596118) 简单来说就是 spring 在启动时会加载 xsd 文件，它首先会到本地查找
 xsd 文件（一般都会包含在 spring 的 jar 包中），如果找不到则会到 xml 头部定义的 url 指定路径下中去寻找 xsd，如果找不到则会报错。附：在 spring jar 包下的 META-INF 文件夹中都会包含一个 spring.schemas 文件，其中就包含了对 xsd 文件的路径定义，具体如下图所示：
![spring-aop.jar包下META-INF文件夹下的内容](http://dl.iteye.com/upload/picture/pic/119332/a793922e-88dc-35f9-bccd-72a0b37cc1b2.jpg)
图：spring-aop.jar 包下 META-INF 文件夹下的内容
![spring.schemas文件内容](http://dl.iteye.com/upload/picture/pic/119334/133fdfd9-4847-32a3-ba55-66bc3c4dec9d.jpg)
图：spring.schemas 文件内容
       由于我的工程是在内网，所以通过 url 路径去寻找肯定是找不到的，但是比较奇怪的是既然 spring 的 jar 包中都会包含，那为什么还是找不到呢？原来这是 assembly 插件的一个 bug，具体情况参见：[http://jira.codehaus.org/browse/MASSEMBLY-360](http://jira.codehaus.org/browse/MASSEMBLY-360) 该
 bug 产生的原因如下：工程一般依赖了很多的 jar 包，而被依赖的 jar 又会依赖其他的 jar 包，这样，当工程中依赖到不同的版本的 spring 时，在使用 assembly 进行打包时，只能将某一个版本 jar 包下的 spring.schemas 文件放入最终打出的jar包里，这就有可能遗漏了一些版本的 xsd 的本地映射，所以会报错。所以一般推荐使用另外的一个插件来进行打包，插件名称为：maven-shade-plugin，shade 插件打包时在对 spring.schemas 文件处理上，它能够将所有
 jar 里的 spring.schemas 文件进行合并，在最终生成的单一 jar 包里，spring.schemas 包含了所有出现过的版本的集合，要使用 shade 插件，必须在 pom 进行如下配置：
```
<plugin>  
    <groupId>org.apache.maven.plugins</groupId>  
    <artifactId>maven-shade-plugin</artifactId>  
    <version>1.4</version>  
    <executions>  
        <execution>  
            <phase>package</phase>  
            <goals>  
                <goal>shade</goal>  
            </goals>  
            <configuration>  
                <transformers>  
                    <transformer  
                        implementation="org.apache.maven.plugins.shade.resource.AppendingTransformer">  
                        <resource>META-INF/spring.handlers</resource>  
                    </transformer> 
                    <transformer  
                        implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">  
                        <mainClass>com.chenzhou.examples.Main</mainClass>  
                    </transformer>  
                    <transformer  
                        implementation="org.apache.maven.plugins.shade.resource.AppendingTransformer">  
                        <resource>META-INF/spring.schemas</resource>  
                    </transformer>  
                </transformers>  
            </configuration>  
        </execution>  
    </executions>  
</plugin>
```
上面配置文件中有一段定义：
```
<transformer  
    implementation="org.apache.maven.plugins.shade.resource.AppendingTransformer">  
    <resource>META-INF/spring.handlers</resource>  
</transformer>  
<transformer  
   implementation="org.apache.maven.plugins.shade.resource.AppendingTransformer">  
   <resource>META-INF/spring.schemas</resource> 
</transformer>
```
        上面这段配置意思是把 spring.handlers 和 spring.schemas 文件以 append 方式加入到构建的 jar 包中，这样就不会出现 xsd 找不到的情况。配置完 pom 后，调用 mvn clean install 命令进行构建，构建成功后打开工程 target 目录，发现生成了 2 个 jar 包，一个为：original-XXX-0.0.1-SNAPSHOT.jar，另一个为：XXX-0.0.1-SNAPSHOT.jar，其中
 original...jar 里只包含了工程自己的 class 文件，而另外的一个 jar 包则包含了工程本身以及所有依赖的 jar 包的 class 文件。我们只需要使用第二个 jar 包就可以了。

