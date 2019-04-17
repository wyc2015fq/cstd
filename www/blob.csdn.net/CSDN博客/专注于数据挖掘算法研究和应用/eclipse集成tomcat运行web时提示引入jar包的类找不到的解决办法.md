# eclipse集成tomcat运行web时提示引入jar包的类找不到的解决办法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月04日 15:09:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9205








在eclipse集成tomcat开发web时，java类引入的jar包，编译通过，但启动tomcat运行web时提示找不到jar包内的类，需要作如下配置，将jar包在部署到集成的tomcat环境中。




问题：在 web应用 右键->Run as -> Run on Server情况下，会找不到Java Resources下Libraries的jar，应该是eclipse工程在发布到tomcat时并未将lib内jar包发布过去；




解决：右键web应用，选择Properties->deployment assembly-> add ->java build path entries ->选择 jar  后重新发布；

![](https://img-blog.csdn.net/20160204151325824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



