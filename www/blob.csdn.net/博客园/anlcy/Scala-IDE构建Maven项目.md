
# Scala-IDE构建Maven项目 - anlcy - 博客园






# [Scala-IDE构建Maven项目](https://www.cnblogs.com/camilla/p/8274860.html)
本教程演示如何使用Scala-IDE构建一个Scala Maven项目。
## 1. 下载Scala IDE
通过以下链接下载Scala IDE：
[http://scala-ide.org/download/sdk.html](http://scala-ide.org/download/sdk.html)
根据自己的平台选择自己相应版本下载。下载完成之后解压，如下：
![这里写图片描述](http://img.blog.csdn.net/20160327134425427)
选择**eclipse.exe**启动即可。
## 2. 安装插件
原生的Maven插件在Eclipse中，不能应用到scala项目。因此需要额外下载一个针对scala的eclipse maven插件。通过以下更新站点安装该插件：
[http://alchim31.free.fr/m2e-scala/update-site](http://alchim31.free.fr/m2e-scala/update-site)
![这里写图片描述](http://img.blog.csdn.net/20160327134810647)
按照常规方法安装即可。
## 3. 设置远程Archetype
为了方便地创建Scala Maven项目，我们在Eclipse配置远程的Maven Archetype，ArcheType相当于项目模板。在Preference页面中选择Maven->Archetypes：
![这里写图片描述](http://img.blog.csdn.net/20160327135044695)
点击 Add Remote Catelog，在弹出框的catlog file输入：
> [http://repo1.maven.org/maven2/archetype-catalog.xml](http://repo1.maven.org/maven2/archetype-catalog.xml)
![这里写图片描述](http://img.blog.csdn.net/20160327135217655)
OK，完成。
## 4. 创建Scala Maven项目
Eclipse中选择New->Maven Project，在ArcheType的选择页面中，选择如下：
![这里写图片描述](http://img.blog.csdn.net/20160327135527502)
刚进入该页面时，由于需要远程加载ArcheType，所以可能需要一点时间才能加载出来，在进度提示中可以看到正在加载catelog。
往后就是常规的maven创建，完成之后，因为要到远程Maven仓库加载很多jar包，所以可能会耗费一点时间：
![这里写图片描述](http://img.blog.csdn.net/20160327140004839)
最后项目结构如下：
![这里写图片描述](http://img.blog.csdn.net/20160327142541490)
运行App.scala:
![这里写图片描述](http://img.blog.csdn.net/20160327142648729)
End.





