# intellij idea + gradle编译cas 5.1.8 - 左直拳的马桶_日用桶 - CSDN博客
2018年02月09日 19:48:27[左直拳](https://me.csdn.net/leftfist)阅读数：1091
操作系统：WIN 7 
开发工具：intellij idea 2007.3 ultimate 
构建工具：gradle 4.5
对于我这个没有多少java基础的人来说，下载一个CAS的高版本（5.1.8）源码，编译运行，是一个噩梦。
`郁孤台下清江水，中间多少行人泪。西北望长安，可怜无数坑！宋-辛弃疾`
**1、代码结构**
老实说，CAS 5.1.8的源码结构，目前为止我还看不懂。最简单的要求，完全不修改代码，我部署一个可运行的单点登录服务，需要编译哪部分？里面又有core，又有support，还有api。有人说只须webapp/cas-server-webapp即可，但我用elipse加载它，结果所有源码都跑出来了，并列地排在根目录下。这是什么鬼？
**2、该用何种工具构建**
16年我搞过一次，完全不需要构建。现在，是maven，还是gradle？这两样我都没用过。据说CAS的高版本用gradle，但网上搜的例子，动不动就来个maven，pom.xml。这可咋办？
经过无数次折腾，走了无数的弯路，我最终抛弃elipse，改用intellij idea + gradle 来加载这个项目。而intellij idea又折腾了 community版 和 ultimate版。
**3、修改/build.gradle**
构建刚开始不久就会报错。 
看上去，要求装一个git。不过，看代码，只是获取一个版本号而已，不想节外生枝，姑且改一改：
```
//git = org.ajoberstar.grgit.Grgit.open(file('.'))
    //def gitHead = git.head()
    currentRevision = "5.1.8"//gitHead.id
    currentAbbreviatedRevision = "5.1.8"//gitHead.abbreviatedId
```
**4、修改intellij idea的设置（setting）**
刚开始的时候，都是报out of memory的错误，idea处于假死状态。我心想怎么这么烂，还号称最好用的java开发工具之一呢。
实际上是配置不对。
配置首选CAS本身的[部署说明书](https://apereo.github.io/cas/developer/Build-Process.html)。在源码根目录下的/README.MD有链接。
一定要按照上面的说明，将C:\Program Files\JetBrains\IntelliJ IDEA 2017.3.4\bin\idea64.exe.vmoptions的内容替换类似如下，因为我有10g内存，所以最大值给了8G。
```
-server
-Xms2g
-Xmx8g
-XX:NewRatio=3
-Xss16m
-XX:+UseConcMarkSweepGC
-XX:+CMSParallelRemarkEnabled
-XX:ConcGCThreads=4
-XX:ReservedCodeCacheSize=840m
-XX:+AlwaysPreTouch
-XX:+TieredCompilation
-XX:+UseCompressedOops
-XX:SoftRefLRUPolicyMSPerMB=50
-Dsun.io.useCanonCaches=false
-Djava.net.preferIPv4Stack=true
-ea
-XX:MaxPermSize=512m
-XX:PermSize=512m
-Xverify:none
```
但说明书上有些选项非常坑，不能听： 
1） 
![这里写图片描述](https://img-blog.csdn.net/20180209191242807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
用说明书勾选的这个，会有好多包都拿不到，一定要用默认推荐的这个。我大天朝有坚固的墙，岂能以洋人自由散漫的常理度之？
2） 
![这里写图片描述](https://img-blog.csdn.net/20180209191257848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
不要自动编译，否则很卡。也许是我机器比较烂。
这样子设置了之后，加载（import exists project –> gradle ）之后，系统就会自动下载各种依赖包了。正常情况下，15分钟到半小时应该能搞定。
**5、编译**
各种依赖包都有了之后，按道理编译应该会很顺利。但除了包外，项目内部有相互引用的。结果我编译时，报各种缺少对象的错误，都是代码自己编写的对象。我顺著错误提示，去相关文件夹看，果真没有东西生成，那其他引用它的模块当然就报错了。
找了许久，网上也不见有相关答案。偶尔看到setting里有个编译选项，叫”compiler - excludes（例外）”，好死不死，真的是这个类在上面赫然在目。靠，怎么搞出来的，难道是我之前乱点，不小心设上去的？将这个名单删除，编译就OK了。
![这里写图片描述](https://img-blog.csdn.net/20180209192615211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**6、scss**
终于编译成功了，兴冲冲地将编译好的war包拷贝到tomcat下（有关intellij idea与tomcat的折腾，可点[这里](http://blog.csdn.net/leftfist/article/details/79287477)），激动地等待结果。
页面是出来了，但没有样式，页面丑的一逼。经过多方比较，发现少了几个css。源代码里是scss格式的。 
![这里写图片描述](https://img-blog.csdn.net/20180209193409624?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再次开启网上疯狂搜索模式。事实证明，加什么file watchers插件并没有什么卵用。
后来我自己摸索scss这些字眼，发觉有个gulpfile.js，鼠标右键，选“show gulp tasks”，然后系统提示：缺少什么 gulp-sass 模块之类，见招拆招，装装装，终于没有错误提示，再看css文件夹，终于相应的CSS出来了。主要是要用npm安装相关模块：
```
1）打开操作系统的命令行窗口
2）命令行方式下，进入项目所在文件夹cas-5.1.8\webapp\
3）依次运行：
npm install gulp
npm install gulp-sass
npm install gulp-autoprefixer
npm install gulp yargs
```
至此，有关sass的前端模块已经安装完毕，刷新 
![这里写图片描述](https://img-blog.csdn.net/20180710204257626?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后双击需要转化的目录，即可将*.sass 转成相关的 *.css 
![这里写图片描述](https://img-blog.csdn.net/2018071020432295?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再次部署，啊，样式正常了。晒一下： 
![这里写图片描述](https://img-blog.csdn.net/20180209194759944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上面安装这些模块，有300多M！都是一些node.js，只是为了这几个样式。其实，一般登录界面都是我们自己另行开发的，所以，这个sass转化的步骤可以省省。
一个多星期的折腾，才出来个这！java的难学，可见一斑。要不是快过年，哪里有这么大块的时间供这样折腾！
接下来要改改代码。还有很长的路要走。
