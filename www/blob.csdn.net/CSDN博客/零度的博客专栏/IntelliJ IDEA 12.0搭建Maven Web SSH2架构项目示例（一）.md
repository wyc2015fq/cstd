# IntelliJ IDEA 12.0搭建Maven Web SSH2架构项目示例（一） - 零度的博客专栏 - CSDN博客
2016年08月08日 14:46:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：3863
用IDEA搭建Maven web项目，对于很多用惯了Eclipse的人可能会很不适应。在项目的目录结构设置上，Eclipse和IDEA的确有些区别。这篇文章将在原来的基础上更加详细的介绍，最后会给出两个示例来展示实际效果。文章将从5个方面来介绍：(文章贴图较多，如果你那里无法显示，[这里是PDF文档](http://yunpan.cn/QzAaQe3nhwx9v)，内容一样)
1.基础环境搭建；
2.创建一个Maven-web-application应用程序；
3.引入一个现有的Maven-web-application项目；
4.环境配置与执行调试；
    5.Maven-web-application项目核心文件：pom.xml文件的结构分析。
**1. 基础环境搭建**
    首先你的环境中要安装JDK，在配置环境变量的时候，按照下图的方式配置，对【系统变量(S)】进行配置。如下图
![在系统变量中配置，不要在用户环境变量中配置](http://www.open-open.com/home/attached/f_201401/20140104145606_36067.png)
新建变量JAVA_HOME，不要进行非主流的环境配置。此步骤完成后编辑【系统变量Path】，如下图：
![](http://www.open-open.com/home/attached/f_201401/20140104145818_14567.png)加入 ;%JAVA_HOME%\bin;
全部保存后，打开CMD界面，输入命令：java -version，如果显示如下则Java环境配置成功。
![](http://www.open-open.com/home/attached/f_201401/20140104150053_89247.png)
 现在我们开始配置Maven。Maven是神奇的工具，在apache下有该项目的最新信息，他的连接地址如下：[http://maven.apache.org/download.cgi](http://maven.apache.org/download.cgi)现在最新的版本是 
 Maven 3.1.1。如下图所示，我们选择红方框中的文件，并下载。
![](http://www.open-open.com/home/attached/f_201401/20140104150502_27597.png)
解压后，我把他放在了D:\Program File\apache-maven-3.1.1-bin\apache-maven-3.1.1目录下。这里随你的喜好，不是必须的。现在开始配置M2_HOME这个变量，如下图所示：
![](http://www.open-open.com/home/attached/f_201401/20140104150929_73393.png)
变量值为：D:\Program File\apache-maven-3.1.1-bin\apache-maven-3.1.1 。完成后再次编辑Path变量，如下图所示：
![](http://www.open-open.com/home/attached/f_201401/20140104151127_30363.png)再次打开CMD，注意这里要关闭上一次的窗口。输入命令如下：mvn -v 回车。如果出现下图界面，证明你的Maven环境变量配置成功。
![](http://www.open-open.com/home/attached/f_201401/20140104151325_79988.png)
**2.创建一个Maven-web-application应用程序  在上面基础上，这里假定你第一次在Windows系统中安装Intellij IDEA。我的系统是windows 7旗舰版，注意：我的操作系统并没有跃升为管理员权限，我的登录名是Yangcl，不是Administrator。我建议你如果跃升为管理员了最好重新装系统，因为在后面debug项目的时候，IDEA会因为找不到 .m2文件夹而导致你一直无法找到你依赖的第三方jar包，这将导致你的项目无法运行，很多人问我的时候，都是这个原因。这个问题在第一次写这个博客的时候没有注意到，今天提出来。**
    第一次打开IDEA，如下图，我们从零开始创建一个maven-web-application项目。
![](http://www.open-open.com/home/attached/f_201401/20140104152342_91491.png)
选择【Create New Project】，出现下图界面
![](http://www.open-open.com/home/attached/f_201401/20140104152939_28797.png)
完成后，点击Finish按钮。然后IDEA会弹出下图所示界面：
![](http://www.open-open.com/home/attached/f_201401/20140104153245_19901.png)
完成后，我们开始创建一个新的moudle，如下图：
![](http://www.open-open.com/home/attached/f_201401/20140104153400_35370.png)
在新窗口中选择【Maven Module】。
![](http://www.open-open.com/home/attached/f_201401/20140104153517_91219.png)
点击完成后，会出现下面的界面，注意红方框所圈的地方。如下图：
![](http://www.open-open.com/home/attached/f_201401/20140104153656_32576.png)
如果你使用远程仓库来支持你的项目，那么下面的界面完全不用改动。
![](http://www.open-open.com/home/attached/f_201401/20140104154207_32101.png)
点击完成后，你的C:\Users\Yangcl\.m2\repository文件夹下应该有些依赖被下载了，你的IDEA控制台也开始下载和寻找依赖。注意：这些都是IDEA自动执行的。如下2幅图：
![](http://www.open-open.com/home/attached/f_201401/20140104155420_35197.png)
![](http://www.open-open.com/home/attached/f_201401/20140104155431_83836.png)
这样一个简单的Maven-web-application项目就创建完成了，如下2幅图：
![](http://www.open-open.com/home/attached/f_201401/20140104155711_64275.png)
![](http://www.open-open.com/home/attached/f_201401/20140104160722_53244.png)
可以看到，红方框下还没有其他内容。因为要使用这个项目来搭建一个SSH2程序，所以我们要更改pom.xml文件，向其中添加更多的东西如依赖和编译路径(<dependencies>、<build>),下面会单分一块来介绍pom.xml，这里简单如图：
![](http://www.open-open.com/home/attached/f_201401/20140104160218_11488.png)
添加完<dependencies>后编译器会显示报错，因为还没有reimport，如下图：
![](http://www.open-open.com/home/attached/f_201401/20140104161809_70084.png)
此时，一个拥有SSH2基础依赖环境的项目就搭建好了。如果你想删除这个项目，不要像在Eclipse中那样做，你可以按照下图去做：
![](http://www.open-open.com/home/attached/f_201401/20140104162228_66554.png)
![](http://www.open-open.com/home/attached/f_201401/20140104162242_58117.png)
![](http://www.open-open.com/home/attached/f_201401/20140104162252_48235.png)
这样项目就被删除了。文章太长了，剩下的内容在IntelliJ IDEA 12.0搭建Maven Web SSH2架构项目示例（二）中介绍。
