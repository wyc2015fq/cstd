# JMeter常见错误解决方法 - zhusongziye的博客 - CSDN博客





2017年12月13日 21:03:16[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：4826








###### 一、录制不到以html结尾的请求


    在使用代理服务器录制脚本的时候，可能会出现html结尾的请求录制不到的问题。这是因为浏览器为了提升访问速度，将这些请求获取到的html资源作为静态资源缓存在本地了，所以只要打开过一次主页，接下来每次打开主页时，都无需再发送这些请求。对于性能测试来说，主页中这些html资源，是有必要反复进行压测的。因此录制脚本的时候，必须将这些请求录制下来。





解决方法：


    只要每次录制前，清空浏览器缓存就可以了。


（建议使用抓包方式做脚本）




###### 二、响应数据中有乱码


使用录制下来的测试脚本进行测试的时候，查看结果树，可能会出现响应数据中有乱码。





解决方法：


1、找到安装目录/bin下面的jmeter.properties配置文件，如下：

![](https://img-blog.csdn.net/20171213205802113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



2、打开jmeter.properties配置文件，找到“sampleresult.default.encoding”这个配置项，将原来的默认值“ISO-8859-1”修改为“utf-8”即可，修改后的配置如下所示：

![](https://img-blog.csdn.net/20171213205824079?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3、重启jmeter，重新执行测试。这时响应数据中就不会有乱码了。




###### 三、请求数据中有乱码


设置HTTP请求，参数中的值显示为中文，但系统接受数据时，中文显示为乱码；在“查看结果树”中，查看HTTP请求，请求参数中的中文数据显示乱码。





解决方法：


       在HTTP请求页面，Content encoding设置为utf-8即可。




###### 四、JMeter内存OOM


       执行测试的时候，可能会出现OutOfMemoryError这样的异常。JMeter是java实现的工具，内存是由java虚拟机JVM管理的。如果出现了内存溢出的问题，可能调整JVM内存相关的参数进行优化。





步骤如下：


1、在window下，找到jmeter.bat启动脚本；

![](https://img-blog.csdn.net/20171213205848222?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



2、编辑jmeter.bat脚本，找到下面这些配置项。

![](https://img-blog.csdn.net/20171213205916735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3、参数调整


可以调大堆内存的大小：


将默认的set HEAP=-Xms512m -Xmx512m，


改为：set HEAP=-Xms1024m –Xmx1024m





或者调大堆内存中新生代的大小：


将默认的set NEW=-XX:NewSize=128m-XX:MaxNewSize=128m，


改为：set NEW=-XX:NewSize=256m-XX:MaxNewSize=256m。





或者调大永久带的大小：


将默认的set PERM=-XX:PermSize=64m-XX:MaxPermSize=128m


改为：set PERM=-XX:PermSize=128m-XX:MaxPermSize=256m





调整后重启jmeter，执行测试，问题一般能得到解决。需要说明的是，参数的取值不能一概而论，也要根据大家电脑的硬件配置实际选取。



###### 五、测试机配置不能满足压测要求


       Jmeter压测过程中，由于测试机配置有限，CPU、内存都可能是存在瓶颈。如果使用很大的并发进行测试时，就可能会感到程序比较卡，这时候就无法继续增加压力了。





解决方法：


       搭建Jmeter分布式集群，远程启动测试。





步骤如下：


1、在分布式集群中，脚本编辑、控制的机器叫做“client”，实际运行Jmeter测试脚本，负责向被测服务器发请求的机器，称为“server”。网络要求





2、server和client需要网络互通。找几台机器作为 server，然后client和server相互ping彼此的ip地址，如果能ping通，说明网络是ok的。





和client一样，server可以是linux系统，也可以是windows系统。 client上安装的JMeter版本最好和client保持一致。





在本文演示中，作为client机器，ip是172.31.132.242。


作为server机器，ip是172.17.3.212。





3、演示中，因为server是linux系统的，我们需要去官网下载linux系统对应的JMeter安装文件，解压即可。





进入安装目录：apache-JMeter-2.13/bin下，启动server端程序，执行：./JMeter-server （windows下启动JMeter-server.bat）


              如果server机器有多个ip，启动的时候可以指定一个，如：


./JMeter-server-Djava.rmi.server.hostname=172.17.3.212


    配置和启动client端安装目录的bin文件夹下，找到JMeter.properties文件，如下：

![](https://img-blog.csdn.net/20171213210010807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



    打开文件，并找到remote_hosts=127.0.0.1这一行，修改为server服务器的列表，如下所示，如果server有多个，那么多个机器地址之间使用英文逗号分隔：

![](https://img-blog.csdn.net/20171213210039979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



    其实，client自己可以同时作为一个server机器，向服务端发送请求（前提是client上也启动了JMeter-server程序），所以也可以把client的ip加上，添加自己只需要写127.0.0.1即可。本文的演示中，先不添加client自己，所以使用的是第一个配置。

![](https://img-blog.csdn.net/20171213210111480?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



    如果client有多个ip，也可以在启动的时候指定一个。 如果是windows系统，可以修改jmeter.bat启动脚本，在以下的命令最后，加上如下的配置，写明client这台机器的ip地址即可。

![](https://img-blog.csdn.net/20171213210143150?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



修改完毕后保存，启动Jmeter，打开界面。





执行分布式测试


    打开client端的JMeter界面，点击运行-远程启动，里面能看到配置好的server端的机器列表。

![](https://img-blog.csdn.net/20171213210231339?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



       此时可以启动一个远程机器，也可以选择下面的“远程全部启动”，同时启动所有的server。点击后，client端会自动和server端的服务建立连接，下发测试脚本到server端； 等server端的测试全部结束后，会将测试结果汇总到client端，我们在client端配置的监听器就能看到测试结果了。




(本文转载自网络，感谢原文作者,如有版权问题请及时留言,我们会第一时间处理)






