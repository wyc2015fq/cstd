# Maven 的classifier的作用 (JSON-LIB) - z69183787的专栏 - CSDN博客
2015年04月22日 14:33:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2300
直接看一个例子，maven中要引入json包，于是使用了
**[java]**[view
 plain](http://blog.csdn.net/lovingprince/article/details/5894459#)[copy](http://blog.csdn.net/lovingprince/article/details/5894459#)[print](http://blog.csdn.net/lovingprince/article/details/5894459#)[?](http://blog.csdn.net/lovingprince/article/details/5894459#)
- <dependency>  
-                 <groupId>net.sf.json-lib</groupId>   
-                 <artifactId>json-lib</artifactId>   
-                 <version>2.2.2</version>  
-             </dependency>  
 可是，当执行mvn install 命令时，却抛出一个错误，说找不到net.sf.json-lib:json-lib:2.2.2这个包，到仓库中看一下http://repo2.maven.org/maven2/net/sf/json-lib/json-lib/2.2.2/
![](http://hi.csdn.net/attachment/201009/19/0_12848756699zAQ.gif)
 jar的名称中多了一个跟JDK相关的名称，例如jdk15，按照上面的配置，明显是找不到这个jar的，于是classifier就有它的用武之地了，它表示在相同版本下针对不同的环境或者jdk使用的jar,如果配置了这个元素，则会将这个元素名在加在最后来查找相应的jar，例如：
**[c-sharp]**[view
 plain](http://blog.csdn.net/lovingprince/article/details/5894459#)[copy](http://blog.csdn.net/lovingprince/article/details/5894459#)[print](http://blog.csdn.net/lovingprince/article/details/5894459#)[?](http://blog.csdn.net/lovingprince/article/details/5894459#)
- <dependency>  
-                 <groupId>net.sf.json-lib</groupId>   
-                 <artifactId>json-lib</artifactId>   
-                 <version>2.2.2</version>  
-                     <classifier>jdk15</classifier>    
-             </dependency>  
这样配置即可找到json-lib-2.2.2-jdk15.jar ，其他的就不解释了，应该明白他的用途了吧。
