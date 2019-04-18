# 在Maven仓库中添加Oracle JDBC驱动 - z69183787的专栏 - CSDN博客
2014年03月27日 16:59:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5143
由于Oracle授权问题，Maven3不提供Oracle JDBC driver，为了在Maven项目中应用Oracle JDBC driver,必须手动添加到本地仓库。
# ***一.首先要得到Oracle JDBC Driver***
1.通过Oracle官方网站下载相应版本：[http://www.oracle.com/technetwork/database/features/jdbc/index-091264.html](http://www.oracle.com/technetwork/database/features/jdbc/index-091264.html)
2.通过Oracle的安装目录获得，位置在“{ORACLE_HOME}\jdbc\lib\ojdbc14.jar”
# ***二.手动安装安装***
手动安装请参考另一篇文章：***[maven3 手动安装本地jar到仓库](http://www.cnblogs.com/leiOOlei/p/3356834.html)***
或者命令如下：
mvn install:install-file -Dfile=**{**Path**/**to**/**your**/**ojdbc.jar**}**-DgroupId=com.oracle -DartifactId=ojdbc6 -Dversion=11.2.0 -Dpackaging=jar
# ***三.安装完后，在pom.xml文件中添加引用***
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
  <dependencies>
    <!-- 添加oracle jdbc driver -->  
    <dependency>    
        <groupId>com.oracle</groupId>    
        <artifactId>ojdbc14</artifactId>    
        <version>10.2.0.4.0</version>
    </dependency>
  </dependencies>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
好了，可以正常引用了。
