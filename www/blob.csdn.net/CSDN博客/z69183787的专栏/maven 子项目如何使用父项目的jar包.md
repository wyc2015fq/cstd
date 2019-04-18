# maven 子项目如何使用父项目的jar包 - z69183787的专栏 - CSDN博客
2014年03月27日 12:36:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8008
如果父pom中使用的是<dependencies>....</dependencies>方式，则子pom会自动使用pom中的jar包，如果父pom使用<dependencyManagement><dependencies>....</dependencies></dependencyManagement>方式，则子pom不会自动使用父pom中的jar包，这时如果子pom想使用的话，就要给出groupId和artifactId，无需给出version
