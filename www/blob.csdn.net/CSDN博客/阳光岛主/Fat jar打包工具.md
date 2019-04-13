
# Fat jar打包工具 - 阳光岛主 - CSDN博客

2014年03月09日 00:04:55[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：24542


Fat Jar Eclipse Plug-In是一个可以将Eclipse Java Project的所有资源打包进一个可执行jar文件的小工具，可以方便的完成各种打包任务，我们经常会来打jar包，但是eclipse自带的打jar似乎不太够用，Fat Jar是eclipse的一个插件，特别是Fat Jar可以打成可执行Jar包，并且在图片等其他资源、引用外包方面使用起来更方便。
安装方法：
1. eclipse在线更新方法：help > Install New Sofware > Add...
**name：**FatJar
**URL****：**[http://kurucz-grafika.de/fatjar](http://kurucz-grafika.de/fatjar)
2. eclipse插件安装方法：
下载地址：[http://downloads.sourceforge.net/fjep/net.sf.fjep.fatjar_0.0.31.zip?modtime=1195824818&big_mirror=0](http://downloads.sourceforge.net/fjep/net.sf.fjep.fatjar_0.0.31.zip?modtime=1195824818&big_mirror=0)
解压后，将解压出的plugins中的文件复制到eclipse中的plugins目录/home/homer/eclipse/plugins下
cp  -r  /home/homer/Desktop/plugins/net.sf.fjep.fatjar_0.0.31/  /home/homer/eclipse/plugins/
然后重启eclipse，避免Fat Jar被认不出来，在eclipse启动时使用-clean参数
eclipse  -clean  &

**方法1**
项目右键 ——》 Export.. ——》 Others ——》Fat Jar Exporter
![](https://img-blog.csdn.net/20140228111429171)
使用问题：刚使用Fat Jar打包了一个SWT的程序，头两次打包成功后运行打包的jar文件，但是提示找不到主类，但是在打包是主类我已经配置过的，看打好的包中META-INF\MANIFEST.MF前两行的确没有关于主类的定义，手动加上主类的配置之后jar包就可以双击启动了，很费解，就去Fat Jar的项目主页（[http://fjep.sourceforge.net](http://fjep.sourceforge.net/)）去看了下，在Fat Jar Eclipse Plug-In Tutorial(http://fjep.sourceforge.net/fjeptutorial.html)中发现了我的问题，原来是在配置Manifest时我多选了select Manifest file选项，应该使用默认的<createnew>,这样就没问题了

**方法2**
右键项目 ——》 Build Fat Jar，如下图：
![](https://img-blog.csdn.net/20140228111728671)

早期的样式
![](https://img-my.csdn.net/uploads/201209/14/1347595604_6957.png)
![](https://img-my.csdn.net/uploads/201209/14/1347595618_4695.png)
打包完成后生成一个jar包
运行jar包：
进入jar所在目录，执行java -jar jar名称 命令即可运行
![](https://img-my.csdn.net/uploads/201209/14/1347595630_1266.png)



