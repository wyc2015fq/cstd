
# 在Android开发中使用Ant 三：批量打包 - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月07日 18:57:35[Jlins](https://me.csdn.net/dyllove98)阅读数：1706


批量打包最常用到的地方是进行产品推广时，为每个渠道打一个包。上一篇随笔中，介绍了怎样进行一次完整的打包，批量打包只要在此基础上做一次循环即可。
在打包之前要做两个准备工作，一个是读取渠道，一个是修改存储渠道的文件。
在工程的assets中新建一个channel.cfg文件，专门用了保存渠道，保存方式是 channel=渠道号。
ant的基本库中没有提够循环，需要引入另一个库ant-contrib-1.0b3.jar，以下通过一个例子来了解ant的循环
<?xml version="1.0" encoding="UTF-8"?><projectname="TestAnt"default="deploy"><taskdefresource="net/sf/antcontrib/antcontrib.properties"><classpath><pathelementlocation="${ant.home}/exlib/ant-contrib-1.0b3.jar"/></classpath></taskdef><targetname="deploy"><foreachlist="aaa,bbb,ccc,ddd,eee"target="package"param="channel"delimiter=","></foreach></target><targetname="package"><echo>${channel}</echo></target></project>
输出结果：
![](https://img-blog.csdn.net/20130807185725734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在这个例子中aaa,bbb,ccc,ddd,eee以“，”为分隔符，依次取内容赋值给channel，做了五次循环。在targetpackage中，用${channel}就可以取到渠道。
取到渠道后，通过正则表达式替换channel.cfg的内容
<targetname="change.channel"><replaceregexpflags="s"byline="true"><regexppattern="channel=(.*)"/><substitutionexpression="channel=${channel}"/><filesetdir="assets"includes="channel.cfg"/></replaceregexp></target>
flags="s"表示文件是单行，byline="true"表示逐行查找，<regexppattern="channel=(.*)"/>指定匹配的正则表达式，<substitutionexpression="channel=${channel}"/>为替换后的内容，<filesetdir="assets"includes="channel.cfg"/>指定文件。
我们在工程的根目录新建一个channels.properties的文件，输入channels=aaa,bbb,ccc,ddd,eee完整的批量打包代码为：
<?xml version="1.0" encoding="UTF-8"?><projectname="TestJni"default="package"><taskdefresource="net/sf/antcontrib/antcontrib.properties"><classpath><pathelementlocation="${ant.home}/exlib/ant-contrib-1.0b3.jar"/></classpath></taskdef><!--设置sdk，ndk路径，证书路径密码--><loadpropertiessrcFile="local.properties"/><loadpropertiessrcFile="project.properties"/><!--所有的渠道--><loadpropertiessrcFile="channels.properties"/><failmessage="sdk.dir is missing."unless="sdk.dir"/><failmessage="ndk.dir is missing."unless="sdk.dir"/><!--编译native代码--><targetname="native"unless="native.libraries.are.uptodate"><echomessage="Building native libraries..."/><execexecutable="${ndk.dir}/ndk-build.cmd"failonerror="true"/><echomessage="DONE (Building native libraries)"/></target><!--引入sdk中的build.xml--><importfile="${sdk.dir}/tools/ant/build.xml"/><!--修改保存渠道的文件--><targetname="change.channel"><replaceregexpflags="s"byline="true"><regexppattern="channel=(.*)"/><substitutionexpression="channel=${channel}"/><filesetdir="assets"includes="channel.cfg"/></replaceregexp></target><!--进行一次完整的打包--><targetname="package"depends="change.channel,native,release"><!--将apk复制的制定的目录--><copytofile="sign/${ant.project.name}-${channel}.apk"file="bin/${ant.project.name}-release.apk"/></target><!--执行批量打包--><targetname="deploy"><foreachlist="${channels}"target="package"param="channel"delimiter=","></foreach></target></project>
打开cmd，切换到工程根目录，执行ant deploy，批量打包完成后在制定目录生成了apk
![](https://img-blog.csdn.net/20130807185726468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
安装apk到手机或模拟器进行测试
![](https://img-blog.csdn.net/20130807185727843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在程序内部已经读到渠道。
至此，只能够进行简单的批量打包任务，所用到的也是简单的ant命令，上面的的批量打包代码也有待改进，希望通过以后的学习，能加快打包的速度，实现更复杂的功能。




