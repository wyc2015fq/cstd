
# maven的安装教程 - anlcy - 博客园






# [maven的安装教程](https://www.cnblogs.com/camilla/p/8259260.html)
下载地址为：http://maven.apache.org/download.cgi
进入此页面之后
![](https://images0.cnblogs.com/blog/664891/201501/121706444646582.jpg)
点击下载，然后解压，我把目录名改为maven，目录结构如下图所示
![](https://images0.cnblogs.com/blog/664891/201501/121708107762746.png)
下面我们配置环境变量
这是maven_home:
![](https://images0.cnblogs.com/blog/664891/201501/121712226511630.png)
在path变量的最后面加上       ;%maven_home%\bin     %号前面的;号不能扔掉
![](https://images0.cnblogs.com/blog/664891/201501/121712309333466.png)
在cmd中输入跳转到安装目录的bin文件下，在执行mvn -version ，如果出现版本信息则正确安装了。
## 2.maven安装完成之后显示mvn不是内部命令
在设置环境变量path的时候，可能覆盖了原先设置着的变量，只要在path后面将原来的;%maven_home%\bin变为
;%SystemRoot%\system32;%maven_home%\bin 即可
**五、修改本地仓库位置（****如果不想修改本地仓库位置则这一步骤省略即可）**
**Maven会将下载的类库（jar包）放置到本地的一个目录下（****一般默认情况下maven在本机的仓库位于C:\我的文档中\.m2.\repository****），如果想重新定义这个目录的位置就需要修改Maven本地仓库的配置：**
**如果找不到.m2文件夹就先命令行执行：mvn -help:system**
**1、在自己喜欢的位置创建文件夹，此处本人创建的位置是（F:\Maven\repo）**
**2、****在安装Maven的目录下找到conf文件夹，在文件夹中找到settings.xml文件，复制****settings.xml文件****放于****C:\我的文档中\.m2.\repository****，如下图所示：**
![](https://images2017.cnblogs.com/blog/1223891/201708/1223891-20170822141403511-1309297021.png)
**3、修改settings.xml文件，如下图所示：**
![](https://images2017.cnblogs.com/blog/1223891/201708/1223891-20170822141537621-774553219.png)
配置国内的镜像仓库地址，使用阿里云的maven镜像仓库。

<mirror>
<id>alimaven</id>
<name>aliyun maven</name>
<url>http://maven.aliyun.com/nexus/content/groups/public/</url>
<mirrorOf>central</mirrorOf>
</mirror>
**4、在安装Maven的目录下找到conf文件夹，在文件夹中找到settings.xml文件，更改默认的仓库位置如下图所示：（注意两个地方的settings.xml都要修改）**
![](https://images2017.cnblogs.com/blog/1223891/201708/1223891-20170822141131902-386920395.png)
**依据该配置，Maven就会将下载的类库保存到F:/Maven/repo中。**
**5、****实验一下我们刚才做的事情产生作用没有，控制台输入：mvn help:system**
![](https://images2017.cnblogs.com/blog/1223891/201708/1223891-20170822142037418-203373011.png)
**执行完该命令之后，在F:/Maven/repo下面会出现很多文件，这些文件就maven从中央仓库下载到本地仓库的文件。**
**如果安装完maven找不到.m2文件，则命令行执行mvn help:system**





