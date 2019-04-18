# Eclipse基金会发布Eclipse Photon IDE - weixin_33985507的博客 - CSDN博客
2018年08月05日 19:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
Eclipse基金会发布了最新版本的[Eclipse IDE](%5C)。Eclipse Photon带来对Java 10和Java EE 8的支持，增强了PHP开发工具和Dark主题等功能。
\\
Eclipse Java开发工具（Eclipse Java Development Tools，JDT）对Java 10提供了完整的支持，允许开发人员使用[局部变量类型推断（local variable type inference，JEP 286）](%5C)功能，比如在使用`var`的地方能够进行建议代码补全以及通过Quick Assist将类型转换为`var`。
\\
![22386acbd29e7f14ba058d2db7cd7bb1.png](https://static001.geekbang.org/resource/image/22/b1/22386acbd29e7f14ba058d2db7cd7bb1.png)
\\
Eclipse Photon还添加了一项特性，允许将非模块化的项目转换为[模块](%5C)，这需要通过创建一个`module.info.java`文件来实现。开发人员还可以将代表module-info.java的代码片段直接粘贴到源码文件中，从而创建module-info.java文件。如下可以复制-粘贴的代码可以用来阐述该功能：
\\\`import java.sql.Driver;\module hello {\    exports org.example;\    requires java.sql;\    provides Driver with org.example.DriverImpl;\}\`\\
在Eclipse Photon中，Java编辑器也在多个方面进行了改善。在使用[dark主题](%5C)时，Java语法着色进行了增强，其中包括减少了粗体样式的使用并修改一些彼此太接近的颜色。另外，在将字符粘贴到字符串时，能够转义非ASCII的字符。要启用该功能，点击**Java \u0026gt; Editor \u0026gt; Typing \u0026gt; Escape text when pasting into a string literal and check Use Unicode escape syntax for non-ASCII characters**。如下是一些示例字符，在粘贴至一个字符串时它们被替换成了unicode转义序列：
\\
![1a4c91c4d221dbaaa473e5fa237250bd.png](https://static001.geekbang.org/resource/image/1a/bd/1a4c91c4d221dbaaa473e5fa237250bd.png)
\\
Eclipse Photon允许开发人员在Package Explorer中对库的条目按照字母顺序进行排序，要启用该功能，打开**Java \u0026gt; Appearance**首选项页面，并选中**Sort library entries alphabetically in Package Explorer**。库的展现方式如下所示：
\\
![3690033a251e4d91db699e367d622ec6.png](https://static001.geekbang.org/resource/image/36/c6/3690033a251e4d91db699e367d622ec6.png)
\\
在Eclipse Photon中，Java编译器包含了一个新的偏好设置，叫做“Compiler Compliance does not match used JRE”，如果项目所使用的JRE不匹配所选择的编译遵从级别时（比如项目使用JRE 1.8作为JRE System Library，但是编译遵从级别设置为1.7），它会将报告的严重问题提示出来。除此之外，在测试源码上运行Java注解有了新的支持，还增加了一项体验性的特性，那就是在进行模块声明的搜索时，允许使用正则表达式。
\\
Java Formatter配置项进行了简化，将所有的配置项放到了一个可展开的树组件上，而不再是使用多个标签页。要查看该功能，请打开**Java \u0026gt; Code Style \u0026gt; Formatter \u0026gt; Edit**。在**Comments \u0026gt; Javadoc**下还有一个新的选项“Align Javadoc tags in columns”。
\\
![06beaee2808ea80e6e7249b96cd38b41.png](https://static001.geekbang.org/resource/image/06/41/06beaee2808ea80e6e7249b96cd38b41.png)
\\
如下是使用**Align descriptions, grouped by type**的一个样例：
\\
![b68343da7853ba756247865ae1e62b78.png](https://static001.geekbang.org/resource/image/b6/78/b68343da7853ba756247865ae1e62b78.png)
\\
Eclipse Photon中的调试也更加高效，这要归功于一系列的新特性：
\\- 高级源码查找能够在运行时类路径预先未知的情况下，提供正确的源码查找功能；\\t
- 针对Java启动配置提供了启动配置原型；\\t
- 调试器现在会监听线程名的变化，这意味着Java调试器在JVM中添加了一个新的断点并在断点到达的时候在Debug视图进行提示；\\t
- 在方法退出和异常断点时，将会展现值，在Step Into、Step Over或Step Return时可以观测到的方法最终结果（每个return和throw），现在会在Variables视图的第一行中进行显示；\\t
- 在断点视图中，新增加了一个排序选项，允许按照创建的时间（age）进行排序\
PHP开发工具有了一系列的增强，比如对变量未使用/未赋值的校验支持、在break/continue中校验标量（scalar）、对PHP 7或更高版本校验静态操作。除此之外，PHP Explorer被Project Explorer替换掉了。PHP特性的完整列表可以在Eclipse Photon New and Noteworthy页面的PHP[板块](%5C)查看。
\\
按照Eclipse基金会的说法，这次版本发布包括了85个项目，代码行超过了7300万行，这些代码是由620个开发人员贡献的，其中246人是Eclipse贡献者。关于Eclipse Photon的更多信息可以在[Eclipse Photon New and Noteworthy](%5C)页面上查阅。Eclipse Photon可以在[Eclipse下载页面](%5C)进行下载。
\\
**查看英文原文：**[https://www.infoq.com/news/2018/07/eclipse-photon](%5C)
