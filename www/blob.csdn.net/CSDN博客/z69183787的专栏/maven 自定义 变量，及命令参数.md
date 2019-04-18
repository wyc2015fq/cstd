# maven 自定义 变量，及命令参数 - z69183787的专栏 - CSDN博客
2016年09月18日 16:45:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5861
如何通过maven 命令行对，自定义变量进行传参：
1.首先应该知道，对应一个变量值，pom.xml里面配置的有，那么如果你在命令行中 以 -Dmy.filter.value=1 的格式去配置mvn命令，那么将覆盖你pom中的值。
例如：clean -Ptrip-app,daily package -Dmy.filter.value=1 -Dttidapk.ttids=21xx00
在pom中想定义变量的话是写在properties中的
### Custom properties in the POM
User defined properties in the pom.xml.
`--`
`<project>`
`...`
`  ``<properties>`
`     ``<my.filter.value>hello</my.filter.value>`
`  ``</properties>`
`...`
`</project>`
`---`
参考：[http://docs.codehaus.org/display/MAVENUSER/MavenPropertiesGuide](http://docs.codehaus.org/display/MAVENUSER/MavenPropertiesGuide)
