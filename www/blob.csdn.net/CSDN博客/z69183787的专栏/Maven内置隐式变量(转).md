# Maven内置隐式变量(转) - z69183787的专栏 - CSDN博客
2016年10月31日 15:00:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：335
Maven提供了三个隐式的变量可以用来访问环境变量，POM信息，和Maven Settings
**env**
env变量，暴露了你操作系统或者shell的环境变量。便 如在Maven POM中一个对${env.PATH}的引用将会被${PATH}环境变量替换，在Windows中为%PATH%.
**projetc**
project变量暴露了POM。可以使用点标记(.)的路径来引用POM元素的值。例如
<project>
<modelVersion>4.0.0</modelVersion>
<groupId>org.sonatype.mavenbook</groupId>
<artifactId>project-a</artifactId>
<version>1.0-SNAPSHOT</version>
<packaging>jar</packaging>
<build>
<finalName>${project.groupId}-${project.artifactId}</finalName>
</build>
</project>
当你使用mvn help:effective-pom 查看时,你会看到<finalName>org.sonatype.mavenbook-project-a</finalName>
- ${basedir} 项目根目录
- ${project.build.directory} 构建目录，缺省为target
- ${project.build.outputDirectory} 构建过程输出目录，缺省为target/classes
- ${project.build.finalName} 产出物名称，缺省为${project.artifactId}-${project.version}
- ${project.packaging} 打包类型，缺省为jar
- ${project.xxx} 当前pom文件的任意节点的内容
**settings**
settings变量暴露了Maven settings信息。可以使用点标记(.)的路径来引用settings.xml文件中元素的值。例如${settings.offline}会引用~/.m2/settings.xml文件中offline元素的值。
