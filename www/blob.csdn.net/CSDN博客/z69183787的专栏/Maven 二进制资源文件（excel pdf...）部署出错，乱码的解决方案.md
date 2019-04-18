# Maven 二进制资源文件（excel pdf...）部署出错，乱码的解决方案 - z69183787的专栏 - CSDN博客
2016年05月14日 17:17:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1124
Maven resources 插件会对文本资源文件进行转码，但是它无法区分文件是否是纯文本文件还是二进制文件。于是二进制文件在部署过程中也就被转码了，就无法正常打开了。官方文档建议按如下操作。例如不对pdf和swf文件进行转码。<project>
 ...
 <build>
 <plugins>
 <plugin>
 <groupId>org.apache.maven.plugins</groupId>
 <artifactId>maven-resources-plugin</artifactId>
 <version>2.6</version>
 <configuration>
 ...
<nonFilteredFileExtensions>
 <nonFilteredFileExtension>pdf</nonFilteredFileExtension> <nonFilteredFileExtension>swf</nonFilteredFileExtension>
 </nonFilteredFileExtensions>
 ... 
 </configuration> 
 </plugin>
 </plugins>
 ...
 </build>
 ...
 </project>
