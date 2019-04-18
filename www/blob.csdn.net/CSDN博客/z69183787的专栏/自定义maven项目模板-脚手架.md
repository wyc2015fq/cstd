# 自定义maven项目模板-脚手架 - z69183787的专栏 - CSDN博客
2018年06月28日 13:52:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：135
[https://blog.csdn.net/ccw775302619/article/details/49593685](https://blog.csdn.net/ccw775302619/article/details/49593685)
许多使用过maven的同学都是拿人家做好的archetype来进行快速开发，自己最近接触到一个新的项目，便自己尝试去做一个archetype项目模板，一来可以让自己多熟悉了解maven，二来也方便日后使用。
大概看了一下maven的官方文档，生成archetype主要有两种方式：一种是直接从无到用，使用mvn archetype:generate命令先生成一个空项目，然后在这个空项目上面进行定制；第二种则是提前做好一个定制好的项目，比如一下log的配置信息等，然后在此基础上使用 mvn archetype:create-from-project来生成配置项目，这种方法官方称是高阶方法。
无论是第一种还是第二种方法，里面的定制方法其实是一样的，这里就以第二种方法为例，阐述一下如何自定义一个maven项目模板。
现在假设有一个已经定制好的项目，WEB-INF的spring文件夹下有一个spring的配置文件，文件和里面的内容如图：
![](https://img-blog.csdn.net/20151102232735846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20151102232748921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
现在，需要进入这个项目的根目录下，打开命令行窗口，运行mvn archetype:create-from-project命令，maven则会自动在target/generated-sources/archetype/生成基本的项目模板，如果需要替换配置文件中的变量，就需要改里面的配置。例如，如果我将spring配置文件里面的App改成自动生成后的项目名，那么，就需要进入/target/generated-sources/archetype/src/main/resources/archetype-resources/src/main/webapp/WEB-INF/spring目录下，将app-config.xml文件里面的app替换为${artifactId},至于为什么是artifactId呢，我想这个了解maven的人都会知道的。artifactId是maven里面的内置变量，类似地还有其他例如gourpId,package等变量，这些各位可以自己去谷歌或百度。
那么，如果我想将配置文件app-config.xml文件名中的app改成定制的项目名称，需要怎么改呢？可以直接将app改成__artifactId__,这种在文件名中出现的占位符需要使用双下划线才能生效。
等改完这些配置之后，使用命令行进入target/generated-sources/archetype，运行mvn install可将模板安装到本地，如果有私服的也可以部署到私服上。
  如有问题，欢迎指出。
转载请注明出处：http://blog.csdn.net/ccw775302619/article/details/49593685
