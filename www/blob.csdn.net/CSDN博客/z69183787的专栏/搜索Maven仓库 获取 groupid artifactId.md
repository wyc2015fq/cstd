# 搜索Maven仓库 获取 groupid artifactId - z69183787的专栏 - CSDN博客
2014年03月26日 16:17:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12878
使用Maven进行开发的时候，比较常见的一个问题就是如何寻找我要的依赖，比如说，我想要使用activeMQ，可是我不知道groupId，artifactId，和合适的version。怎么办呢？本文介绍几个提供maven仓库搜索服务的站点。
1. [http://repository.sonatype.org/index.html](http://repository.sonatype.org/index.html)
![](http://juvenshun.iteye.com/upload/picture/pic/25265/129d781e-7f0c-39a9-a381-82c3ffbd81f4.png)
该服务器是由[Sonatype](http://www.sonatype.com/) 提供的，Sonatype是由Maven支付Jaso Van Zyl创立的致力于maven，[Nexus](http://nexus.sonatype.org/)，[m2eclipse](http://m2eclipse.sonatype.org/) 等产品的公司。该仓库搜索的后台使用了Nexus，Nexus是目前为止最为强大的Maven仓库管理器，而且，它是开源的。如果你组织内部在大量使用Maven，那么就会需要假设自己的Maven仓库，Nexus是很好的选择，你可以免费下载安装使用。
如上图，你可以在左边的搜索框输入你想要搜索的内容，如org.apache.activemq，右边就会出现相应的结果（pom, jar）。点击最右边的pom链接可以直接查看pom内容（然后直接复制粘贴你需要的groupId，artifactId和version），你也可以点击artifact直接下载jar文件。
2. [http://www.mvnbrowser.com/](http://www.mvnbrowser.com/)
![](http://juvenshun.iteye.com/upload/picture/pic/25267/3b3d4d8d-3515-3d18-a950-fd5a42ce773d.png)
mvnbrowser代理很多主要的公共Maven仓库，并且对这些仓库进行的详细的介绍，包括Apache, Codehaus, JavaNet等等，请看：[http://www.mvnbrowser.com/repositories.html](http://www.mvnbrowser.com/repositories.html) 。mvnbrowser的功能比nexus简单，但是也非常实用，你可以在右上角的搜索框输入关键字，然后搜索，你会看到一系列搜索结果，有趣的在后面，当你点击某个结果的时候，详细信息页面会有一个POM代码片段，这十分有用，因为你可以直接复制。此外，这个构件的所有版本信息，它是否有可用的javadoc和source
 code，它依赖了那些构件，拥有哪些文件，被谁引用了，在哪些仓库中可用，许可证是什么，等等。这些非常实用的信息都通过UI很友好的得到的体现。
3. [http://www.mvnrepository.com/](http://www.mvnrepository.com/)
![](http://juvenshun.iteye.com/upload/picture/pic/25269/5cdefaf4-0a00-383c-8f8c-60a3649db709.png)
mvnrepository也是一个非常有用的maven仓库搜索服务，它最大的特点就是简单，除了搜索什么都没有。类似的，你可以在页面最上方输入关键字以进行搜索。得到结果之后再点击以查看详细信息：该构件的坐标POM片段，版本信息，jar下载连接都可以得到。此外，还有一个有趣的特性是，你能看到该构件的文件大小变更状态图。
在mvnrepository中，通过tag cloud页面，哪些关键字最流行，一目了然。
最后， 上述提到的站点都是公共站点，如果你在公司内部使用Maven，那么最好自己架设一个Maven仓库服务器，那样，对外只需要一次下载。Nexus是一个很好的选择，它提供了异常强大的功能，搜索也当然在内了，要详细了解可以查看：[http://books.sonatype.com/maven-book/reference_zh/repository-manager.html](http://books.sonatype.com/maven-book/reference_zh/repository-manager.html) 。
