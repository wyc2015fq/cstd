# IntelliJ IDEA使用Maven创建Java Web项目 - 零度的博客专栏 - CSDN博客
2017年05月09日 13:33:58[零度anngle](https://me.csdn.net/zmx729618)阅读数：754
# 1. Maven简介
    相对于传统的项目，Maven 下管理和构建的项目真的非常好用和简单，所以这里也强调下，尽量使用此类工具进行项目构建， 它可以管理项目的整个生命周期。
   可以通过其命令做所有相关的工作，其常用命令如下：
    - mvn compile
    - mvn test
    - mvn clean
    - mvn package
    - mvn install            //把新创建的jar包安装到仓库中
    - mvn archetype:generate //创建新项目
    中央工厂URL：[http://search.maven.org/](http://search.maven.org/)[http://repo.maven.apache.org/maven2](http://repo.maven.apache.org/maven2)
    本文主要演示在IntelliJ IDEA IDE环境下如何配置和使用。
# 2. 配置Maven
       软件配置一次即可，所有Maven项目共享。
![](https://img-blog.csdn.net/20160308100130574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    1）如上图标注3 选择自己的仓库
![](https://img-blog.csdn.net/20160308100543310?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   1）如上图标注 2 所示，Import Maven projects automatically 表示 IntelliJ IDEA 会实时监控项目的 pom.xml 文件，进行项目变动设置。
   2）如上图标注3 所示，在 Maven 导入依赖包的时候是否自动下载源码和文档。默认是没有勾选的，也不建议勾选，原因是这样可以加快项目从外网导入依赖包的速度，如果我们需要源码和文档的时候我们到时候再针对某个依赖包进行联网下载即可。IntelliJ IDEA 支持直接从公网下载源码和文档的。
  3）上图标注 3 所示，可以设置导入的 VM 参数。一般这个都不需要主动改，除非项目真的导入太慢了我们再增大此参数。
# 3. Maven 骨架创建 Java Web 项目
1) File -> New -> Project...
2) 如下图
![](https://img-blog.csdn.net/20160308101509950)
3）如下图
![](https://img-blog.csdn.net/20160308101744125)
GroupId和ArtifactId<项目名>还有Version，这三个属性目的是标识你的项目的唯一性。
4）如下图
![](https://img-blog.csdn.net/20160308101940331)
5) 如下图
![](https://img-blog.csdn.net/20160308102131207)
 6）点Finish之后，Maven会根据刚才的配置创建一个基于Maven的Web App.
       - 创建结束，其Log如下：
![](https://img-blog.csdn.net/20160308102730725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       - 创建结束，其代码结构如下：
![](https://img-blog.csdn.net/20160308102931928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    recources文件夹：一般用来存放一些资源文件
    webapp文件夹：用来存放web配置文件以及jsp页面等，这已经组成了一个原始的web应用
# 4. 启动 Java Web 项目
1）打开“Project Structure”
![](https://img-blog.csdn.net/20160308103422536?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2）配置“Facets”
![](https://img-blog.csdn.net/20160308103955277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3）配置“Artifacts”
![](https://img-blog.csdn.net/20160308104449356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4) 启动“Edit Configurations”
![](https://img-blog.csdn.net/20160308105155465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5) Add New 'Tomcat Server' 配置
![](https://img-blog.csdn.net/20160308105333703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
6) 配置Deplyment
![](https://img-blog.csdn.net/20160308105613409?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
7) 配置Server
![](https://img-blog.csdn.net/20160308105805611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   其结果如下：
![](https://img-blog.csdn.net/20160308105939815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
8) 启动Web Server
![](https://img-blog.csdn.net/20160308110106696?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
现在可以通过Browser访问WebServer的页面了。
# 5. 在Tomcat中部署war文件
    1）把MVNDemo.war copy到Tomcat的webapps目录下。
    2）在Browser上访问URL：http://localhost:8080/MVNDemo/ 即可。
# 6. 创建Java程序目录
1) 创建java目录
![](https://img-blog.csdn.net/20160308150359134?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2) 把java目录设置为Sources
   选中java文件夹，点击上面的Make as：Sources，该文件夹就会变成蓝色，用以保存java代码，按OK。
![](https://img-blog.csdn.net/20160308151220114?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
