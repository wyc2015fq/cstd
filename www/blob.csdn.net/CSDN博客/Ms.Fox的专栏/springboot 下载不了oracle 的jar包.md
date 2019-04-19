# springboot 下载不了oracle 的jar包 - Ms.Fox的专栏 - CSDN博客
2019年02月15日 10:39:02[lulu-lu](https://me.csdn.net/smbluesky)阅读数：33
# [Maven手动添加dependency（以Oracle JDBC为例）](https://www.cnblogs.com/moonlightpoet/p/5617863.html)
原因：[Oracle](http://lib.csdn.net/base/oracle)的ojdbc.jar是收费的，所以maven的中央仓库中没有这个资源，只能通过配置本地库才能加载到项目中去。
Maven不提供Oracle JDBC driver，为了在Maven项目中应用Oracle JDBC driver,必须手动添加到本地仓库。
首先需要到Oracle官网上下载ojdbc的jar包。
http://www.oracle.com/technetwork/database/features/jdbc/default-2280470.html
我选择的是ojdbc7.jar，对应的版本是12.1.0.2。
ojdbc7.jar (3,698,857 bytes) - (SHA1 Checksum: 7c9b5984b2c1e32e7c8cf3331df77f31e89e24c2)
For use with JDK 7; It contains the JDBC driver classes except classes for NLS support in Oracle Object and Collection types.
说明ojdbc7.jar是在JDK 7上使用的，不过好像ojdbc6.jar也可以在JDK 7上使用。
然后我将下载到的ojdbc7.jar保存在~/additional_jars/目录下。
运行：
mvn install:install-file -DgroupId=com.oracle -DartifactId=ojdbc7 -Dversion=12.1.0.2 -Dpackaging=jar -Dfile=~/additional_jars/ojdbc7.jar
然后出现错误，于是我cd到~/additional_jars/目录下，然后运行：
mvn install:install-file -DgroupId=com.oracle -DartifactId=ojdbc7 -Dversion=12.1.0.2 -Dpackaging=jar -Dfile=ojdbc7.jar
然后还是错误。
后来我发现了错误的原因，因为我在windows下面，我运行这一段程序是在打开的git命令行里。
打开windows的命令行界面，进入~/additional_jars/目录，然后运行：
mvn install:install-file -DgroupId=com.oracle -DartifactId=ojdbc7 -Dversion=12.1.0.2 -Dpackaging=jar -Dfile=ojdbc7.jar
显示"BUILD SUCCESS"。
然后就可以在maven项目里添加dependency，各坐标对应上面这个命令的个元素，如下：
    <dependency>      
      <groupId>com.oracle</groupId>      
      <artifactId>ojdbc7</artifactId>      
      <version>12.1.0.2</version>
    </dependency>  
