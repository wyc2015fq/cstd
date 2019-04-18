# eclipse m2eclipse插件迁移到m2e插件 - z69183787的专栏 - CSDN博客
2014年09月12日 22:22:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1116
原来的eclipse3.6用的是m2eclipse maven插件，插件因为贡献给了eclipse官方所以变成了m2e 。导致以前的项目在使用新的eclipse插件的时候识别不了。网上找到了解决方法原文如下
[http://blog.frankel.ch/migrating-from-m2eclipse-to-m2e](http://blog.frankel.ch/migrating-from-m2eclipse-to-m2e)
替换2个配置文件里面的属性， 修改一个配置文件的名字
1、
`.project 配置文件中buildSpec节点的org.maven.ide.eclipse.maven2Builder` 替换为`org.eclipse.m2e.core.maven2Builder`
`.project 配置文件中buildSpec节点的org.maven.ide.eclipse.maven2Nature 替换为 `org.eclipse.m2e.core.maven2Nature``
`2、`
``.classpath配置文件中的`org.maven.ide.eclipse.MAVEN2_CLASSPATH_CONTAINER` 替换为`org.eclipse.m2e.MAVEN2_CLASSPATH_CONTAINER```
`3、`
`.settings 文件夹下的org.maven.ide.eclipse.prefs 文件名字修改为 org.eclipse.m2e.core.prefs 文件内容不用变`
```注意最好从svn上面保留备份，以防止修改不成功貌似需要javaee视图从svncheckout。```
