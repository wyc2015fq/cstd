# maven 将自己的jar放到中心仓储以供使用 - z69183787的专栏 - CSDN博客
2016年05月17日 14:07:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2749
    虽然maven提供的远程仓储满足绝大部分需求，但是，很多时候，我们需要使用自己编写的jar，或者一些不常用的jar（maven远程仓储没有），我们需要将其存储到中心仓储，也就是.m2中。
    我们以sgs最新的库文件，reddwarf为实例：
**[xhtml]**[view
 plain](http://blog.csdn.net/eclipser1987/article/details/5755549#)[copy](http://blog.csdn.net/eclipser1987/article/details/5755549#)
- mvn install:install-file -DgroupId=cn.vicky.reddwarf -DartifactId=db-Dversion=4.5.20 -Dpackaging=jar-Dfile=F:/reddwarf-0.10.1/db-4.5.20.jar  
- 
- mvn install:install-file -DgroupId=cn.vicky.reddwarf -DartifactId=je-Dversion=3.3.75 -Dpackaging=jar-Dfile=F:/reddwarf-0.10.1/je-3.3.75.jar  
- 
- mvn install:install-file -DgroupId=cn.vicky.reddwarf -DartifactId=sgs-server -Dversion=0.10.1 -Dpackaging=jar-Dfile=F:/reddwarf-0.10.1/sgs-server-0.10.1.jar  
- 
- mvn install:install-file -DgroupId=cn.vicky.reddwarf -DartifactId=sgs-server-api -Dversion=0.10.1 -Dpackaging=jar-Dfile=F:/reddwarf-0.10.1/sgs-server-api-0.10.1.jar  
- 
- mvn install:install-file -DgroupId=cn.vicky.reddwarf -DartifactId=sgs-server-internal-api -Dversion=0.10.1 -Dpackaging=jar-Dfile=F:/reddwarf-0.10.1/sgs-server-internal-api-0.10.1.jar  
- 
- mvn install:install-file -DgroupId=cn.vicky.reddwarf -DartifactId=sgs-shared -Dversion=1.12 -Dpackaging=jar-Dfile=F:/reddwarf-0.10.1/sgs-shared-1.12.jar  
    于是，我们可以在pom.xml中配置使用：
**[xhtml]**[view
 plain](http://blog.csdn.net/eclipser1987/article/details/5755549#)[copy](http://blog.csdn.net/eclipser1987/article/details/5755549#)
- <!-- 配置reddwarf库仓储 -->
- <dependency>
- <groupId>cn.vicky.reddwarf</groupId>
- <artifactId>db</artifactId>
- <version>4.5.20</version>
- </dependency>
- <dependency>
- <groupId>cn.vicky.reddwarf</groupId>
- <artifactId>je</artifactId>
- <version>3.3.75</version>
- </dependency>
- <dependency>
- <groupId>cn.vicky.reddwarf</groupId>
- <artifactId>sgs-server</artifactId>
- <version>0.10.1</version>
- </dependency>
- <dependency>
- <groupId>cn.vicky.reddwarf</groupId>
- <artifactId>sgs-server-api</artifactId>
- <version>0.10.1</version>
- </dependency>
- <dependency>
- <groupId>cn.vicky.reddwarf</groupId>
- <artifactId>sgs-server-internal-api</artifactId>
- <version>0.10.1</version>
- </dependency>
- <dependency>
- <groupId>cn.vicky.reddwarf</groupId>
- <artifactId>sgs-shared</artifactId>
- <version>1.12</version>
- </dependency>
    小窍门：
我们在打包jar时，最好使用xxx-1.0.jar的方式。xxx对应artifactId而1.0对应version。比如odbc.jar我们可以将其更名为odbc-1.0.jar然后以：mvn install:install-file -DgroupId=oracle -DartifactId=odbc -Dversion=1.0packaging=jar -Dfile=F:/odbc-1.0.jar的方式，将其存放中心仓库！
