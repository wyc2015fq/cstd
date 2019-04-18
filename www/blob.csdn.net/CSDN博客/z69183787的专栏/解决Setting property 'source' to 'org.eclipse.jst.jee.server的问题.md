# 解决Setting property 'source' to 'org.eclipse.jst.jee.server的问题 - z69183787的专栏 - CSDN博客
2014年02月25日 14:17:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：203003
这是我于2013年9月4号晚遇到的问题，为了这个问题我一夜没睡！
对于这个问题，我相信我的方法已经能帮90%的人解决了！
当你用Eclipse运行web项目的时候，你就会看到控制台出现：
WARNING: [SetPropertiesRule]{Server/Service/Engine/Host/Context} Setting property 'source' to 'org.eclipse.jst.jee.server:firstProject' did not find a matching property.
这样的字样，说的是server.xml的Service标签里的Engine标签里的Host标签里的Context标签配置了一个'source'属性，属性值是'org.eclipse.jst.jee.server:firstProject'.
但问题是Context标签是不支持source这个属性的，所以跳出了警告，所以大家也发现的是，尽管有警告，但是大部分人的程序是依然可以运行没有问题的，但少部分人的不能够运行。
也许你已经在网上查到很多像这样的问题，而且也给出了解决办法，但是你照着解决的方法去做了，警告依然存在！
那现在你就可以跟着我的步骤来做了：
我先说一下网上别人所说的步骤，埋个伏笔
1.双击Servers里的Tomcat v7.0 at localhost
![](https://img-blog.csdn.net/20130905160652546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZm9yZXZlcnNpbGVudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.然后跳出个界面：
![](https://img-blog.csdn.net/20130905161012265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZm9yZXZlcnNpbGVudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
看到没，在Server Options选项里有个Publish module contexts to separate XML files的选项，到这里我们也一样把勾勾上，不过问题不是这样就一定解决了，我说过有的朋友这样做了，问题依然没有解决，把勾勾上以后记得保存，如果警告依然存在的朋友们跟着我继续往下看。
3.
![](https://img-blog.csdn.net/20130905161536953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZm9yZXZlcnNpbGVudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
大家仔细对比下你自己的和我图片上的，你会发现你默认的选项应该是Use workspace metadata这个选项，只不过是笔者我改为了tomcat的安装路径罢了，因为Use workspace的路径文件夹不方便后面的讲解中要找的文件，所以笔者使用第2个tomcat安装目录来操作，而Deploy path：你如果没有改过的话，应该是叫wtpwebapps，因为在写文章之前，我已经改掉了也不清楚是不是这个名字了，但确定的一点是，它肯定不是webapps的，所以此处读者根据自己的Deploy
 path的值进行下面的操作。
好了准备工作差不多了，现在是重点来了，为什么好多人做了1.2步操作以后就好了而你自己的没好呢？
原因是，在做1 2 步操作之前你们已经运行过程序，而运行以后程序就会在tomcat文件夹下多了一个backup文件夹
![](https://img-blog.csdn.net/20130905162856828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZm9yZXZlcnNpbGVudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
里面的目录结构如图：
![](https://img-blog.csdn.net/20130905162922078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZm9yZXZlcnNpbGVudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
有没有很熟悉的感觉，没错这就是从tomcat目录里conf文件夹下拷贝过来的文件
接下来直接说操作好了：
4.
删除掉刚才的backup文件夹[backup所在位置根据第3步所说的，因为笔者使用的时tomcat安装目录所以就在目录下就直接发现了，但如果你用的是Use workspace metadata这个选项的话，你就在\workspace\.metadata\.plugins\org.eclipse.wst.server.core路径下可以看到里面有很多的文件，Eclipse就把项目发布到了这里，里面的目录结构和tomcat里面的差不多，总之把\workspace\.metadata\.plugins\org.eclipse.wst.server.core里面的文件全删除了就可以了]
5.
找到tomcat目录里的conf目录里的Catalina目录里有个localhost文件夹，把文件夹里面的文件全删除掉。
6.
此处告诉大家3.4.5步骤纯属告诉你们Eclipse发布的一个细节，删除上面的文件其实不用那么麻烦：
在Servers下Tomcat v7.0 at localhost里，首先右键-->移除里面已经添加的所有工程，然后右键-->Clean清空项目虚拟目录里的文件，然后重新运行你们的程序就可以了。
注意：如果有人想和我一样在第3步server locations里修改项目发布目录的话，只要发布过一次项目你会发现，server locations里面的选项是灰色的，不可以改，怎么办呢，这就需要你照着第6步里面的移除项目，清空文件，然后你就惊奇的发现，server locations可以修改了，OK，累了，我去，写了3个小时，希望能帮到你们解决你们的问题，See You！！！！
