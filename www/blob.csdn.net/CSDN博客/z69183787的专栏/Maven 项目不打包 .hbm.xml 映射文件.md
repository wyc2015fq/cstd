# Maven 项目不打包 *.hbm.xml 映射文件 - z69183787的专栏 - CSDN博客
2016年05月17日 11:42:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1423
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)

使用 Maven 部署 Java Web 项目时，Hibernate 的映射文件 *.hbm.xml 没有被打包部署到目标目录下，解决方法：在 pom.xml 文件中 <build> 节点下添加以下代码：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1     <build>
 2         ……
 3         <resources>
 4             <resource>
 5                 <directory>src/main/java</directory>
 6                 <includes>
 7                     <include>**/*.hbm.xml</include>
 8                 </includes>
 9             </resource>
10             <resource>
11                 <directory>src/main/resources</directory>
12                 <includes>
13                     <include>**/*.xml</include>
14                     <include>**/*.properties</include>
15                 </includes>
16             </resource>
17         </resources>
18         ……
19     </build>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
或者：将 *.hbm.xml 映射文件放到 src/main/resources 文件夹下：
![](http://images.cnitblog.com/i/293735/201406/161712201769635.png)
