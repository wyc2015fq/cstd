# tomcat下域名的配置及ROOT.xml - 零度的博客专栏 - CSDN博客
2016年05月13日 10:20:22[零度anngle](https://me.csdn.net/zmx729618)阅读数：983
在默认安装后，tomcat的主目录是webapps/root目录，如果我们想改变tomcat的主目录的话可以这样做：
1.打开C:/Tomcat/conf/server.xml，在<host></host>之间加入代码：
<Context path="" docBase="E:/tomcatApp/JeasyCMS" debug="0"  reloadable="true" crossContext="true"/>
这样重新启动tomcat，我们的主目录就被设置为jeasyCMS这个项目了。
2.另外tomcat会生成一个C:/Tomcat/conf/Catalina/localhost目录，里面自动生成了一个ROOT.xml(root大写)文件，我们打开文件可以看到这些代码：
<?xml version='1.0' encoding='utf-8'?>
< Context crossContext="true" docBase="E:/tomcatApp/JeasyCMS" path="" reloadable="true">
< /Context>
3.注意：我们创建虚拟目录的时候也是这样来进行设置的，只是context这个标签的path属性不为空。
4.我们设置虚拟目录和主目录，都会在C:/Tomcat/conf/Catalina/localhost目录下生成相应的xml文件，因此，当我们在server.xml里面删除了主目录和虚拟目录的设置后，如果系统还没有生效，这时候我们就要察看一下这个目录下的xml文件是不是已经被删除了，如果没有被删除，他还是对tomcat起作用的，我们必须手动删除。
5.基于第4点，通过测试我发现，每当我们访问一个地址的时候，tomcat首先是到C:/Tomcat/conf/Catalina/localhost目录下去察看有没有存在主目录或虚拟目录的xml文件，如果有xml文件，就按xml里的路径进行访问，如果没有xml文件，就到server.xml文件里去察看是否配置context标签，如果配置了context标签，则在C:/Tomcat/conf/Catalina/localhost目录下生成一个对应的xml文件，以便于下次直接验证而不再访问server.xml，与此同时打开context里指定的路径，如果server.xml里没有配置context标签，则返回访问错误页面。
6.基于第5点，我们其实可以直接在C:/Tomcat/conf/Catalina/localhost下建立一个xml文件，用于主目录或者虚拟目录，而不需要去修改server.xml就可以达到配置主目录和虚拟目录的目的，具体xml的格式可以如下：
<?xml version='1.0' encoding='utf-8'?>
< Context crossContext="true" docBase="E:/tomcatApp/JeasyCMS" path="" reloadable="true">
< /Context>
具体是主目录的配置还是虚拟目录的配置取决于path的值。另外主目录文件名必须是ROOT.XML(root大写)，而虚拟目录的文件名称和虚拟目录的名称一致并且为小写，譬如：
<?xml version='1.0' encoding='utf-8'?>
< Context crossContext="true" docBase="E:/tomcatApp/JeasyCMS" path="/cms" reloadable="true">
< /Context>
以上则定义了一个名为：cms的虚拟目录，同时要将以上文本保存为cms.xml文件。
7.以上住目录配置好后就可以按以下地址访问了：
http://localhost:8080/(访问主目录)
http://localhost:8080/cms (访问名为cms的虚拟目录)
