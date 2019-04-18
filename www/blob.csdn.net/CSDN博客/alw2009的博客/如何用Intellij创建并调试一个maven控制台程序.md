# 如何用Intellij创建并调试一个maven控制台程序 - alw2009的博客 - CSDN博客





2018年05月27日 11:14:03[traveler_zero](https://me.csdn.net/alw2009)阅读数：1022








1.创建一个maven工程

    File->New Project->Maven

![](https://img-blog.csdn.net/20180527110800554?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180527110914606?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


2.添加exec-maven-plugin插件


<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>someGroup</groupId>
    <artifactId>HelloWorld</artifactId>
    <version>1.0-SNAPSHOT</version>
    <build>
        <plugins>
            <plugin>
                <groupId>org.codehaus.mojo</groupId>
                <artifactId>exec-maven-plugin</artifactId>
                <version>1.6.0</version>
                <executions>
                    <execution>
                        <goals>
                            <goal>exec</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>
</project>

3.设置Run/Debug Configurations

![](https://img-blog.csdn.net/20180527111218467?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




