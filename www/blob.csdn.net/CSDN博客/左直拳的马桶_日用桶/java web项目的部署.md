# java web项目的部署 - 左直拳的马桶_日用桶 - CSDN博客
2015年06月04日 15:18:21[左直拳](https://me.csdn.net/leftfist)阅读数：1701
# java web项目的部署
我刚开始学着编写java web项目，着实遇到不少麻烦，感觉JAVA真难侍候，好多东西都是手动、手动。
就拿这个web项目在tomcat上的部署来说吧。我在项目的build path里引用了libraries，结果开发的时候没问题，但运行时却报了 java.lang.NoClassDefFoundError 的错误。经查，这是因为运行时找不到相关的JAR包所致。到tomcat下的webapps相关目录看了看，下真的是没有所要的JAR包。 
![这里写图片描述](https://img-blog.csdn.net/20150604151549869)
手动将JAR拷贝过去，错误果然没有了，正常运行。不过这样未免太麻烦了吧。
![这里写图片描述](https://img-blog.csdn.net/20150604151520884)
后来发现，我这个web项目建立之初就通过myeclipse部署到了tomcat，而类库是后来再加进去的。会不会是这个原因呢？我又打开了部署的界面，果然，有个 redeploy 的按钮。我建了个类似情况的新项目，按了一下这个按钮，所需JAR包果然济济一堂矣！
