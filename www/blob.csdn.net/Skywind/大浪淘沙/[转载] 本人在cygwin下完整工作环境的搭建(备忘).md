# Skywind Inside » [转载] 本人在cygwin下完整工作环境的搭建(备忘)
## [转载] 本人在cygwin下完整工作环境的搭建(备忘)
July 3rd, 2011[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
       这算是一个变态的嗜好：力求在纯文本模式下完成一切可能的工作，并且还要在windows下工作。 下面的配置未特意说明的地方，要么是太简单不值得说明，要么是顺应了对应软件的默认配置行为。
###### 1.基本环境
       从 [http://www.cygwin.com](http://www.cygwin.com) 下载 cygwin.exe ,按其默认选择安装即可。以后逐步根据需要添加软件包。
       根据本人需求，额外添加软件包：unzip，zip，attr，shutdown
###### 2.文字阅读/编辑/处理 
       从cygwin选择下载安装： vim , xpdf ,  texmacs
###### 3.文本模式的Web浏览器
       从cygwin选择下载安装： w3m ，  这是款类似vim的Web浏览器。
###### 4.安装配置Cygwin X环境
       详见：[http://x.cygwin.com/docs/ug/setup.html#setup-cygwin-x-installing](http://x.cygwin.com/docs/ug/setup.html#setup-cygwin-x-installing)
       通过cygwin选择安装:
           必选：  xorg-server , xinit  
           建议选择：xorg-docs , X-start-menu-icons 
       启动X11环境:
           可以从Windows的开始菜单启动Cygwin X
###### 5.添加ssh登录支持(并且支持通过putty访问Cygwin X)
       可以用putty.exe等远程控制系统访问本地cygwin.  哦，这不是多此一举？用cygwin.bat直接启动不就得了？ 原因有三：
           1）在cygwin.bat启动模式下，用户视觉感受没有通过putty.exe登录访问的感受好，你试试便知。
           2）在cygwin.bat启动模式下，无法进行屏幕取词，而通过putty.exe登录访问可以屏幕取词，这点对英文不太好的人很重要。 
           3）在cygwin.bat启动模式下，输入法提示框总下左下角，并且是纯黑白模式，感受不好，但是在putty.exe登录访问时，输入法提示框是跟随的。
       首先在cygwin中选择安装openssh
       然后用命令 ssh-host-config  配置SSH  注意配置过程中一定要将sshd安装成Windows服务，已方便开机自动启动
       生成ssh1的公钥和密钥：      ssh-keygen -t rsa1 
       生成ssh2的公钥和密钥：    
              生成公钥： ssh-keygen -t rsa     
              生成密钥： ssh-keygen -t dsa 
       启动sshd服务（如果你需要重启机器的话，此步可免）    
              cygrunsrv -S sshd
       为配置为putty.exe支持X11而做的配置服务端：    
              在/etc/sshd_config中配置为：X11Forwarding yes  
              在/etc/profile 中添加配置： export DISPLAY=:0.0
       在客户端putty中配置：    
              window -> Translation 中 character set 选为UTF-8     
              在Connection -> X11 中勾选上：Enable X11 forwarding     
              在X display location 中填入：localhost:0.0
       这样你就可以用类似putty.exe功能远程登录访问本地cygwin系统。以后的操作也就都是通过putty.exe登录操作的。 
       最后说明一点：你可以在putty登录后，在需要用到X11环境的程序前，临时启动Cygwin X 服务。
卸载方式：
       依次执行 cygrunsrv -E sshd   cygrunsrv -R sshd
       然后删除： rm /etc/ssh*
       然后在 /etc/passwd 中删除 新创建的两个用户： sshd ,  cyg_server 
###### 6.C/C++/fortran77/Python完整的开发环境
      在cygwin中选择安装：autobuild ，autoconf ，automake ，binutils ，cmake ，gcc-core ，gcc,gcc-g77 ，gcc-g++ ，gcc-java ，gcc4-core ，gcc4,gcc4-fortran ，gcc4-g++ ,gcc4-java  ,make ,makedepend ,pkg-config,bashdb,ddd,python
###### 7.JAVA基本开发环境
      我采用的是首先从JAVA官网上下载安装普通的windows版本的JDK. 
      然后在cygwin的/opt目录下，建立一个名为jdk的链接到JDK在Window下实际的安装目录。
            ln -sf  <JDK在windows下实际的安装目录>  /opt/jdk
      在cygwin中设置JAVA环境变量（比如可在/etc/profile 中设置）
      最后，下载解压apache-ant和apache-maven在/opt目录下，并在/etc/profile设置好它们的PATH路径。
###### 8.TCL/TK开发环境
cygwin自带的好像有问题,不全？，建议到[http://www.tcl.tk](http://www.tcl.tk) 上分别下载tcl 和 tk 的源码安装包 依次编译安装)
      先装 tcl 后装 tk 。
      都是： ./configure    ,  make   ,  make install
###### 9.Google AppEngine开发环境
      从[http://code.google.com/appengine](http://code.google.com/appengine)中下载，并且解压在/opt/appengine 中, 并且在/opt/profile中设置PATH环境变量.
###### 10.googlecl
      google开发的一个试图通过cmd/shell 访问google所有服务的工具。见链接：[http://blog.chaoskey.com/2011/03/1/201103011210](http://blog.chaoskey.com/2011/03/1/201103011210)
**或者**从cygwin中选择安装： python-gdata ,googlecl
###### 11.版本控制器
      从 cygwin中选择安装：cvs ，cvs3svn , subversion ，git ，git-completion ，git-gui ，git-svn ，mercurial
###### 12.数学工具套件
      1)**数学绘图软件**：gnuplot  （从 cygwin中选择安装）[http://www.gnuplot.info](http://www.gnuplot.info)
      2)**数值计算软件**：octave    （从 cygwin中选择安装）[http://www.octave.org](http://www.octave.org)
      3)**统计分析软件**：R [http://www.R-project.org](http://www.R-project.org)
            采用下载源代码编译安装的方法。当前采用的源码版本是：R-2.12.2.tar.gz
            前置需求：    
[http://ftp.ctex.org/mirrors/CRAN/doc/manuals/R-admin.html#Essential-and-useful-other-programs-under-a-Unix_002dalike](http://ftp.ctex.org/mirrors/CRAN/doc/manuals/R-admin.html#Essential-and-useful-other-programs-under-a-Unix_002dalike)
            主要有：     
                  1）需要gcc,g77(fortran 77)的开发编译环境     
                  2）X11支持环境     
                  3) tcl/tk    
                  4) libX11(-dev) ,libXt(-dev),readline,iconv,cairo(-dev) ,pango ,zlib(-dev) ,xz,libxmu(-dev) ,icu(-dev) ,ghostscript,,libjpeg(-dev) ,libpng12(-dev) ,libtiff(-dev) ,     
            tar -xf R-2.12.1.tar.gz     
            编译     
                  ./configure  –disable-nls –enable-R-shlib  R_BROWSER=w3m  R_PDFVIEWER=xpdf    
                        (原因见：[http://ftp.ctex.org/mirrors/CRAN/doc/manuals/R-admin.html#Cygwin)](http://ftp.ctex.org/mirrors/CRAN/doc/manuals/R-admin.html#Cygwin%29)
                  make  
            测试：     
                  make check     
            生成手册文档：     
                  make dvi     
                  make pdf     
                  make info     
            安装：     
                  make install     
                  make install-dvi     
                  make install-info     
                  make install-pdf
      4)**符号演算软件**：maxima  （[http://maxima.sourceforge.net/](http://maxima.sourceforge.net/)）
            当前采用的源码版本：maxima-5.23.2.tar.gz            
            需求软件包：clisp ，bison ,libgd(-dev) ,plotutils ,libggi(-dev) ,xz ,lapack(-dev) ,curl(-dev) ,magick ,libGLU(-dev)
            ./configure   make  make  install
      5)**数学排版软件**：texmacs
            1）解决texmacs启动时找不到gs的问题：    
                        ln -sf /usr/bin/gs /usr/X11R6/bin/gs
            2）maxima集成：    
                         vim /usr/sbin/TeXmacs/bin/tm_maxima   添加当前版本的条件分支，执行代码暂时和已有的版本执行代码一致。     
从 [http://riemann.math.nccu.edu.tw/~yenlung/share/texmacs-maxima-5.10.0.lisp](http://riemann.math.nccu.edu.tw/%7Eyenlung/share/texmacs-maxima-5.10.0.lisp)  下载文件后，将其复制到：/usr/share/TeXmacs/plugins/maxima/lisp
            3) octave集成：（暂时未解决）
            4）R集成：（暂时未解决）
            5）gnuplot集成：已自动支持。
###### 总结：
      首先，在cygwin默认选择基础上，附加选择如下软件包：unzip ,zip ,attr ,shutdown ,vim ,xpdf ,texmacs ,w3m ,xorg-server ,xinit ,xorg-docs ,X-start-menu-icons ,openssh ,autobuild ,autoconf ,automake ,binutils ,cmake ,gcc,gcc-core ,gcc-g77 ,gcc-g++ ,gcc-java ,gcc4,gcc4-core ,gcc4-fortran ,gcc4-g++ ,gcc4-java ,make ,makedepend ,pkg-config ,python ,python-gdata ,googlecl ,cvs ,subversion ,git ,git-completion ,git-gui ,git-svn ,mercurial ,gnuplot ,octave ,libX11(-dev) ,libXt(-dev) ,readline ,iconv ,cairo(-dev) ,pango ,libjpeg(-dev) ,libpng12(-dev) ,libtiff(-dev) ,zlib(-dev) ,libxmu(-dev) ,icu(-dev) ,ghostscript ,clisp ,bison ,libgd(-dev) ,plotutils ,libggi(-dev) ,xz ,lapack(-dev) ,curl(-dev) ,magick ,libGLU(-dev),tex相关，基本数学库
      然后依第5步：“添加ssh登录支持”所提到的配置。
      依第7步：“配置Java基本开发环境”
      依第8步：从源码安装TCL/TK
      依第9步:  安装配置google appengine开发环境
      依第12步之3）： 安装统计分析软件R
      依第12步之4）：  安装符号演算软件maxima
      依第12步之5）： 配置数学排版软件texmacs以集成：gnuplot , octave ,R ,maxima
###### 附录：遇到的问题及解决方案：
      1）试图解决”Exception: STATUS_ACCESS_VIOLATION“ 的问题    
[http://x.cygwin.com/docs/faq/cygwin-x-faq.html#q-status-access-violation](http://x.cygwin.com/docs/faq/cygwin-x-faq.html#q-status-access-violation)
            a)选择安装Cygwin软件包时，必须确保停止所有的Cygwin进程（包括Cygwin的Windows服务进程）     
            b)确保cygwin1.dll在你的系统中只能存在一份。     
            c)注意杀毒软件或防火墙对Cygwin的限制，必要的话将其加入其信任区域。     
            d)防止在Cygwin’s的fork()模拟器中出现的DLL的基本地址冲突。     
                        安装rebase包     
                        阅读：/usr/share/doc/Cygwin/rebase-n.n.n.README     
                        然后安装说明指示运行：rebaseall     
                        简单的说就是: 在停止Cygwin的所有进程（包括已经安装的Windows服务）的前提下,在Windows下启动cmd.exe, 在这个Windows命令行下，执行ash, 然后在ahs启动的shell 中执行：/bin/rebaseall -v。  
            e)如果还有这个问题，则对出现这个问题的程序执行：     
                    peflags –tsaware=true /usr/bin/w3m.exe  （范例）     
            f)如果通过cygwin选择重新安装或升级程序后，可能这个问题又会复现。我们又需要按a),b),c),d),e)依次执行。
      2）cygwin环境下，maxima无法调用 gnuplot的问题：（非完美的解决方案）  
            a) ln -sf /usr/bin/gnuplot.exe /usr/binwgnuplot
            b) 在每个新的maxima的窗口n中，预先执行:    
                  set_plot_option([gnuplot_default_term_command,"set term x11"]);  
      注意：     
            每次执行一次绘图操作时，都会进入gnuplot的Shell，需要手工退出，才能返回maxima的Shell，该问题不知如何解决。     
            离开maxima后，上述设置会失效。 如何永久存储该设置的方法，暂时为找到。     
**2011-03-13 附注：此问题已经解决**，在你常用的maxima工作目录下，创建如下文件： /home/chaoskey/maxima/maxima-init.mac
> 
/* this is /home/chaoskey/maxima/maxima-init.mac */      
/* last edit 2011-03-13  */
maxima_userdir : "/home/chaoskey/maxima"  $      
maxima_tempdir : "/home/chaoskey/maxima/.tmp"  $       
/*file_search_maxima : append(file_search_maxima ) $*/       
/*file_search_lisp :   append(file_search_lisp) $*/       
set_plot_option([gnuplot_default_term_command,"set term x11"])$       
/* disp("Maxima is the Future.") $ */ 
            说明，你常用的maxima的工作目录可任意选择，maxima 启动时会自动探测出 maxima-init.mac的目录。
