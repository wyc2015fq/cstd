# windows+eclipse+cygwin+cdt+ndk环境搭建 - xqhrs232的专栏 - CSDN博客
2011年06月18日 22:37:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1148标签：[eclipse																[windows																[编译器																[android																[resources																[jni](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=resources&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://my.oschina.net/u/131573/blog/23728](http://my.oschina.net/u/131573/blog/23728)
windows系统上安装与使用Android NDK r5
 NDK & cygwin邮箱里有
很早就听说了android的NDK应用，只是一直没有时间去研究，今天花了点时间在windows平台搭建了NDK环境，并成功运行了第一个简单的android应用. 
一：什么是NDK？
NDK 提供了一系列的工具，帮助开发者快速开发C（或C++）的动态库，并能自动将so 和java 应用一起打包成apk。这些工具对开发者的帮助是巨大的。
NDK 集成了交叉编译器，并提供了相应的mk 文件隔离CPU、平台、ABI 等差异，开发人员只需要简单修改mk 文件（指出“哪些文件需要编译”、“编译特性要求”等），就可以创建出so。NDK 可以自动地将so 和Java 应用一起打包，极大地减轻了开发人员的打包工作。比较简单的说，NDK是一套交叉编译工具，它可以帮你把你用C或C++书写的代码，编译为.so（类似与win下的.dll）格式的文件，使你可以在你的Android程序当中用Java语言（JNI）调用这些代码.
二：下载安装cygwin
由于NDK编译代码时必须要用到make和gcc，所以你必须先搭建一个linux环境， cygwin是一个在windows平台上运行的unix模拟环境,它对于学习unix/linux操作环境，或者从unix到windows的应用程序移植，非常有用。通过它，你就可以在不安装linux的情况下使用NDK来编译C、C++代码了。下面我们一步一步的安装cygwin吧。
（安装的时候忘记截图了）
首先，你得先跑到http://www.cygwin.com下载setup.exe(我已经下载)
1、 然后双击运行吧，运行后你将看到安装向导界面：
2、  点击下一步
此时让你选择安装方式：
1）Install from Internet：直接从Internet上下载并立即安装（安装完成后，下载好的安装文件并不会被删除，而是仍然被保留，以便下次再安装）。(好久了,应该是这个)
2）Download Without Installing：只是将安装文件下载到本地，但暂时不安装。
3）Install from Local Directory：不下载安装文件，直接从本地某个含有安装文件的目录进行安装。
3、选择第一项，然后点击下一步：
4、选择要安装的目录，注意，最好不要放到有中文和空格的目录里，似乎会造成安装出问题，其它选项不用变，之后点下一步：
5、上一步是选择安装cygwin的目录，这个是选择你下载的安装包所在的目录，默认是你运行setup.exe的目录，直接点下一步就可以：
6、此时你共有三种连接方式选择：
1) Direct Connection：直接连接。
2) Use IE5 Settings：使用IE的连接参数设置进行连接。
3) Use HTTP/FTP Proxy：使用HTTP或FTP代理服务器进行连接（需要输入服务器地址、端口号）。
用户可根据自己的网络连接的实情情况进行选择，一般正常情况下，均选择第一种，也就是直接连接方式。然后再点击“下一步”，
7、 这是选择要下载的站点，我用的是http://mirrors.kernel.org，速度感觉还挺快，选择后点下一步(http://mirror.lupaworld.com/cygwin/ 这个是我从这个地址下的,好像是国内的服务器,挺快的,用其他的,有的2k/s,而且安了好几次,有一次快好了,那个服务器关了,囧...)
8、 此时会下载加载安装包列表
9、Search是可以输入你要下载的包的名称，能够快速筛选出你要下载的包。那四个单选按钮是选择下边树的样式，默认就行，不用动。View默认是Category，建议改成full显示全部包再查，省的一些包被隐藏掉。左下角那个复选框是是否隐藏过期包，默认打钩，不用管它就行，下边开始下载我们要安装的包吧，为了避免全部下载，这里列出了后面开发NDK用得着的包：autoconf2.1、automake1.10、binutils、gcc-core、gcc- , g++、gcc4-core、gcc4-g++、gdb、pcre、pcre-devel、gawk、make(宁可多选,不能少选)共12个包.
10、  然后开始选择安装这些包吧，点skip，把它变成数字版本格式，要确保Bin项变成叉号，而Src项是源码，这个就没必要选了。
11、  下面测试一下cygwin是不是已经安装好了。
运行cygwin，在弹出的命令行窗口输入：cygcheck -c cygwin命令，会打印出当前cygwin的版本和运行状态，如果status是ok的话，则cygwin运行正常。
然后依次输入gcc –v，g++  --version，make –version，gdb –version进行测试，(注意空格和-)如果都打印出版本信息和一些描述信息，非常高兴的告诉你，你的cygwin安装完成了！
三：配置NDK环境变量
1、  首先找到cygwin的安装目录，找到一个home/<你的用户名>/.bash_profile文件，我的是：E:/cygwin/home/Administrator/.bash_profile，(注意：我安装的时候我的home文件夹下面神马都没有，解决 的办法：首先打开环境变量，把里面的用户变量中的HOME变量删掉，在E:/cygwin/home文件夹下建立名为Administrator的文件夹（是用户名），然后把E:/cygwin/etc/skel/.bash_profile拷贝到该文件夹下)。
2、  打开bash_profile文件，添加NDK=/cygdrive/<你的盘符>/<android ndk 目录> 例    如：NDK=/cygdrive/e/android-ndk-r5
export NDK
NDK这个名字是随便取的，为了方面以后使用方便，选个简短的名字，然后保存
3、打开cygwin，输入cd $NDK，如果输出上面配置的/cygdrive/e/android-ndk-r5信息，则表明环境变量设置成功了。
四：用NDK来编译程序
1、  现在我们用安装好的NDK来编译一个简单的程序吧，我们选择ndk自带的例子hello-jni，我的位于E:/android-ndk-r5/samples/hello-jni(根据你具体的安装位置而定)，
2、 运行cygwin，输入命令cd /cygdrive/e/android-ndk-r5/samples/hello-jni，进入到E:/android-ndk-r5/samples/hello-jni目录。
3、 输入$NDK/ndk-build，执行成功后，它会自动生成一个libs目录，把编译生成的.so文件放在里面。($NDK是调用我们之前配置好的环境变量，ndk-build是调用ndk的编译程序)
4、  此时去hello-jni的libs目录下看有没有生成的.so文件，如果有，你的ndk就运行正常啦！
生成之后,这时候可以再用ndk的samples里边的例子生成eclipse工程了.
如下,选择新建android工程然后如下图:
然后就可以试验其他的几个例子了.我也还在研究中, 大家一起加油!!!
五：在eclipse中集成c/c++开发环境
(这个好像在国内悲剧啊,下不动啊,在传两个fan墙的软件)
1、 装Eclipse的C/C++环境插件：CDT，这里选择在线安装。
首先登录http://www.eclipse.org/cdt/downloads.php，找到对应你Eclipse版本的CDT插件的在线安装地址。
2、  然后点Help菜单，找到Install New Software菜单
3、  点击Add按钮，把取的地址填进去，出来插件列表后，选Select All，然后选择下一步即可完成安装
4、  安装完成后，在eclispe中右击新建一个项目，如果出现了c/c++项目，则表明你的CDT插件安装成功啦！
六：配置C/C++的编译器
(建议先用命令行编译,然后再研究google ndk samples 里边的例子, 然后再研究傻瓜式的编译)
1、  打开eclipse，导入ndk自带的hello-jni例子，右键单击项目名称，点击Properties，弹出配置界面，之后再点击Builders，弹出项目的编译工具列表，之后点击New，新添加一个编译器，点击后出现添加界面，选择Program，点击OK
2、  出现了添加界面，首先给编译配置起个名字，如：C_Builder
设置Location为<你cygwin安装路径>/bin/bash.exe程序，例如：E:/cygwin/bin/bash.exe，设置Working  Directory为<你cygwin安装路径>/bin目录，例如：E:/cygwin/bin
设置Arguments为
--login -c "cd /cygdrive/e/android-ndk-r5/samples/hello-jni && $NDK/ndk-build"(不能改动的,--login –c 是神马意思我也不懂)
上面的配置中/cygdrive/e/android-ndk-r5/samples/hello-jni是你当前要编译的程序的目录，$NDK是之前配置  的ndk的环境变量，这两个根据你具体的安装目录进行配置，其他的不用变，Arguments这串参数实际是  给bash.exe命令行程序传参数，进入要编译的程序目录，然后运行ndk-build编译程序
3、 接着切换到Refresh选项卡，给Refresh resources upon completion打上钩
4、  然后切换到Build Options选项卡，勾选上最后三项
5、  之后点击Specify Resources按钮，选择资源目录，勾选你的项目目录即可
6、  最后点击Finish，点击OK一路把刚才的配置都保存下来，注意：如果你配置的编译器在其它编译器下边，记得一定要点Up按钮，把它排到第一位，否则C代码的编译晚于Java代码的编译，会造成你的C代码要编译两次才能看到最新的修改
7、  这一路下来肯定很累了吧，现在再次恭喜你，编译配置也配置完成啦，现在来测试一下是否可以自动编译呢，打开项目jni目录里的hello-jni.c文件把提示Hello from JNI!改成其他的文字：如：Hello，My name is alex.，然后再模拟器中运行你的程序，如果模拟器中显示了你最新修改的文字，那么Congratulations！你已经全部配置成功啦！
七：总结
以上过程是我配置NDK的全过程，并在我的机器上运行成功了，参照了www.eoeandroid.com上一些不错的资料，现在写出来做个总结，加深一下自己的理解和记忆，谢谢这些无私的奉献者们。

