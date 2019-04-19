# 使用 IDEA + Maven + Git 快速开发 Java Web 应用 - 零度的博客专栏 - CSDN博客
2016年08月08日 11:51:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：1145
# 0. 引言 
今天想跟大家分享一下我主要的 Java 开发工具，我一般是这样工作的：用 IDEA 写代码，用 Maven 管理 jar 包依赖与项目打包，用 Git 进行代码版本控制。
关于这三款工具的安装与配置的过程，本文不作说明，有不太明白的朋友，可以给我留言，我会尽力解答。 
下面以开发 Smart 应用为例，描述一下这三款工具的具体使用方法。 
# 1. 使用 Git 下载源码 
首先，在你的磁盘里找个单独的文件夹来存放 Smart 的所有项目源码，例如：D:\Project\smart，以下称为“工作目录”。
然后，分别通过 git clone 命令下载 Smart Framework 与 Smart Sample 的源码到本地磁盘上。下载地址如下：
- Smart Framwork：[http://git.oschina.net/huangyong/smart-framework](http://git.oschina.net/huangyong/smart-framework)
- Smart Sample：[http://git.oschina.net/huangyong/smart-sample](http://git.oschina.net/huangyong/smart-sample)
- Smart Cache：[http://git.oschina.net/huangyong/smart-plugin-cache](http://git.oschina.net/huangyong/smart-plugin-cache)
例如：使用 git clone http://git.oschina.net/huangyong/smart-framework 命令，下载 Smart Framework 项目源码。
执行完以上三条 git clone 命令后，你的工作目录中应该会看到这三个子目录：smart-framework、smart-sample、smart-plugin-cache。
# 2. 编写 Maven 项目配置文件 
此时，你可以在工作目录里（也就是在以上三个项目目录的同级）编写一个 Maven 项目配置文件（作为根配置文件），同样需要命名为 pom.xml，用它来组织这三个项目。代码如下：
```
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
         http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>smart</groupId>
    <artifactId>smart</artifactId>
    <version>1.0</version>
    <packaging>pom</packaging>
    <modules>
        <module>smart-framework</module>
        <module>smart-sample</module>
        <module>smart-plugin-cache</module>
    </modules>
</project>
```
```xml
```
# 3. 使用 IDEA 直接打开 Maven 项目 
打开你的 IDEA，选择 Open Project，此时会弹出一个对话框： 
![](http://static.oschina.net/uploads/space/2013/1111/003730_jHP7_223750.png)
在地址栏中定位到你的工作目录（可以手工编写路径或直接粘贴路径），选择 pom.xml，随后 IDEA 将自动创建一个基于 Maven 项目。 
注意：在 IDEA 的向导界面中一定要选择是 Open Project，而不要选择 Create New Project。也可以使用 Import Project 导入 Maven 项目，但没有直接 Open Project 来得痛快。
![](http://static.oschina.net/uploads/space/2013/1111/005123_C2FM_223750.png)
弹指之间，项目已创建完毕！ 
# 4. 编译源码 
先编译一下吧，可以在工具栏中点击这个按钮： 
![](http://static.oschina.net/uploads/space/2013/1111/004117_btgA_223750.png)
你也可使用快捷键 Ctrl + F9，显摆一下你的专业。 
随后，IDEA 将编译整个项目源码，一般情况下是没有任何消息的，因为没有消息就是好消息。 
此外，你还可使用 Maven 进行编译。在 IDEA 中点击右侧的 Maven Projects 选项卡，可看到如下界面： 
![](http://static.oschina.net/uploads/space/2013/1111/004607_MVRn_223750.png)
注意：上图中 smart (root) 表示 Maven 的根配置，也就是你刚才手工编写的那个 pom.xml，由它来管理所有其他的 Maven 项目。 
可双击一下 compile 节点，随后 IDEA 会调用 Maven 进行编译。这样你就不需要在 cmd 里手工输入 Maven 命令了，当然其他的命令你也是可以双击运行。
# 5. 在 IDEA 中配置 Tomcat 
**注意：建议使用 Tomcat 7.0+**
在 IDEA 中配置 Tomcat，只需进行一下几个步骤： 
首先，点击工具栏中的下拉框，然后点击下拉菜单中的 Edit Configurations 菜单项。 
![](http://static.oschina.net/uploads/space/2013/1111/005812_NdKv_223750.png)
点击后将弹出一个对话框，你可点击左上角的“+”按钮，新建一个 Local 的 Tomcat Server。 
![](http://static.oschina.net/uploads/space/2013/1111/010033_ySgU_223750.png)
然后，可对 Tomcat 进行命名，默认是 Unnamed，可修改为 Tomcat。此外，若你不想让 IDEA 自动打开浏览器，去掉 Start browser 复选框即可，我一般都是这么干的。
![](http://static.oschina.net/uploads/space/2013/1111/010632_GP8f_223750.png)
随后，部署一个项目，需要切换到上图中的 Deployment 选项卡中，进行简单的配置。 
![](http://static.oschina.net/uploads/space/2013/1111/010853_x4QR_223750.png)
操作步骤：点击“+”按钮 -> 点击 Artifacts... -> 选择 smart-sample:war exploded -> 修改 Application context 为“/smart-sample”。
最后，建议你切换回 Server 选项卡，在 On frame deactivation 下拉框下选择 Update resources。 
![](http://static.oschina.net/uploads/space/2013/1111/011309_Nkkk_223750.png)
这样做是为了当你切换出 IDEA 后，可自动更新资源（包括：HTML、CSS、JS、JSP 等），如需要自动编译的话，可使用第三个选项。 
注意：IDEA 12.1.6（最新版）中有一个 Bug，第二项也进行了自动编译，在之前的版本中是不存在的，不知道下个版本会不会解决这个问题，不过此问题不会影响你使用。
点击 OK 按钮完成 Tomcat 的所有配置，现在可在工具栏中看见 Tomcat 图标了。 
![](http://static.oschina.net/uploads/space/2013/1111/012034_8OOe_223750.png)
提示：在运行之前，你必须手工创建数据库。比如，Smart Sample 的数据库脚本就在 smart-sample 的 doc 目录下。 
# 6. 以 Debug 方式运行应用程序 
你只需点一下工具栏中的 Debug 按钮，即可以 Debug 方式运行 Smart Sample 了。 
![](http://static.oschina.net/uploads/space/2013/1111/012446_6kf4_223750.png)
建议：在开发过程中尽量使用 Debug 方式运行，这样你可以随时在 IDEA 中打断点进行调试。此外，需要说明的是，IDEA 的调试功能非常强大，而且非常好用！
# 7. 在 IDEA 中对 Git 进行提交与更新 
在工具栏中也包括了 Git 的常用操作，操作非常方便。 
![](http://static.oschina.net/uploads/space/2013/1111/012859_NH8C_223750.png)
以上图标分别表示：更新代码、提交代码、查看差异、查看历史、撤销更改。 
此外，你还可以点击最下方的 Changes 选项卡，用来查看本地变更（Local）与提交日志（Log）。 
![](http://static.oschina.net/uploads/space/2013/1111/013351_sOnV_223750.png)
你可以选择某个版本来查看具体提交的文件内容，还可以同时选择多个版本一起查看，还有很多实用的功能。 
# 8. 总结 
IDEA 是一款非常优秀的 Java 集成开发环境，用法非常简单，上手也非常快。它对主流技术与工具都有相应的插件支持，你可以自定义插件列表，把对于自己无用的插件禁用掉。
Maven 是一款非常优秀的项目构建工具，有了它之后，你无需在每个项目中管理一大堆的 jar 包了，此外，它还非常有助于对项目进行打包（打 jar/war 包），当然它的功能绝不仅仅如此。
Git 是一款非常优秀的代码版本控制系统，它可以有效地实现多人异地办公，此外，你可以提交代码到本地仓库中，然后一次性提交到远程仓库。 
以上三款工具的结合，会大大提高 Java 程序员的战斗能力！在此，强烈推荐大家使用！ 
