# Failure to transfer org.sonatype.oss:oss-parent:pom:7 - z69183787的专栏 - CSDN博客
2014年03月26日 15:36:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2709
DescriptionResourcePathLocationType
ArtifactDescriptorException: Failed to read artifact descriptor for ognl:ognl:jar:3.0.5: UnresolvableModelException: Failure to transfer org.sonatype.oss:oss-parent:pom:7 from http://repo.maven.apache.org/maven2 was cached in the
 local repository, resolution will not be reattempted until the update interval of central has elapsed or updates are forced. Original error: Could not transfer artifact org.sonatype.oss:oss-parent:pom:7 from/to central (http://repo.maven.apache.org/maven2):
 connection timed out to http://repo.maven.apache.org/maven2/org/sonatype/oss/oss-parent/7/oss-parent-7.pompom.xml/spring_cache_demoline 1Maven
 Dependency Problem
.......
Problems输出显示如上错误，一些pom指定jar无法下载，你只需要打开类似目录：C:\Users\Administrator\.m2\repository\org\sonatype\oss\oss-parent\7
然后清空此目录，使pom再次更新即可（若此文件更新还是未成功，找朋友考一个）
