# NS2教程 - xiahouzuoxin - CSDN博客





2013年11月26日 09:44:35[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：14728










# 柯老师的NS2新网址



Due to some reasons, my NS2 website is sometimes donw and unavailable for many users. Therefore, I provide another backup website.

1. NS2 [http://csie.nqu.edu.tw/smallko/ns2/ns2.htm](http://csie.nqu.edu.tw/smallko/ns2/ns2.htm)
2. old_NS2 (backup of NS2  Learning Guide)  [http://csie.nqu.edu.tw/smallko/ns2_old/ns2.htm](http://csie.nqu.edu.tw/smallko/ns2_old/ns2.htm)
3. QualNet [http://csie.nqu.edu.tw/smallko/qualnet/qualnet.htm](http://csie.nqu.edu.tw/smallko/qualnet/qualnet.htm)
4. NCTUNS(EstiNet) [http://csie.nqu.edu.tw/smallko/nctuns/nctuns.htm](http://csie.nqu.edu.tw/smallko/nctuns/nctuns.htm)

5. Wireless [http://csie.nqu.edu.tw/smallko/wireless/wireless.htm](http://csie.nqu.edu.tw/smallko/wireless/wireless.htm)







# **NS2安装教程**


原文地址：[[原创]一步一步教你在CentOS6.0下安装NS2(ns-allinone-2.34.tar.gz)模拟仿真工具](http://blog.chinaunix.net/uid-23069658-id-3142068.html) 作者：[wjlkoorey258](http://blog.chinaunix.net/uid/23069658.html)





在CentOS6.0(内核版本2.6.32-71.el6.i686 )发行版中，基于2.6.32的内核协议栈部分做了适当调整，因此我们在选择ns2的安装版本时就不可以选择较老的版本，因为很多东西都编译不过。就拿常见的struct dev_net{}结构来说就发生了比较大的变化，因此这里我选择ns-allinone-2.34.tar.gz来安装。

1、当然是确保你已经安装了内核源代码。至于如何构建内核源码树，安装内核源码，可以参照我的另一篇博文“[CentOS6.0下安装Freeiris2/Asterisk全程指导教程](http://blog.csdn.net/wjlkoorey/article/details/6984600)”。这里就不再浪费笔墨了。

2、如果你和我一样安装的是Software Development Workstation版本，那么gcc、g++这些基本的工具包就已经存在，否则你可能需要手动去安装gcc和g++。

[koorey@maple ~]$yum install libX11-devel*

[koorey@maple ~]$yum install xorg-x11-proto-devel*

[koorey@maple ~]$yum install libXt-devel*

[koorey@maple ~]$yum install libXmu-devel*  #安装nam时会报错误X11/Xmu/WinUtil.h无法找到，因此需要执行这一步。

3、下载ns-allinone-2.34.tar.gz

[koorey@maple ~]$ su -               #切换到root用户下

[root@maple ~]# cd /usr/src/      

[root@maple src]#  wget  [http://sourceforge.net/projects/nsnam/files/allinone/ns-allinone-2.34/ns-allinone-2.34.tar.gz/download](http://sourceforge.net/projects/nsnam/files/allinone/ns-allinone-2.34/ns-allinone-2.34.tar.gz/download)

[root@maple src]# tar -xzf ns-allinone-2.34.tar.gz

注意：安装xgraph时，需要将ns-allinone-2.34/xgraph-12.1/dialog.c 中的getline()接口改成getline2()，同时在该文件开始部分声明int getline2(char** lptx,char* txt);这样才可以保证xgraph才能正确安装。我不知道其他发行版下有没有这个问题，CentOS6.0下如果不做这样的改动xgraph是编译不通过的。考虑到getline是/usr/include/stdio.h中的系统头文件的缘故，我们最好不要对它做修改免得影响面向用户的API编程接口。因此，我们修改diaglog.c中的getline()，将它定义为我们自己的一个内部接口。

[root@maple src]#  cd ns-allinone-2.34/    #进入ns2源码目录

[root@maple ns-allinone-2.34]#  ./install       #只需这么简单一步就OK然后等待如下信息出现：

CentOS6.0 内核版本：2.6.32-71.el6.i686

Ns-allinone package has been installed successfully.

Here are the installation places:  (这是我加的说明：正确安装后，下面8个功能子模块就都会出现的。。。O(∩_∩)O~)
tcl8.4.18:      /usr/src/ns-allinone-2.34/{bin,include,lib}

tk8.4.18:               /usr/src/ns-allinone-2.34/{bin,include,lib}

otcl:           /usr/src/ns-allinone-2.34/otcl-1.13

tclcl:          /usr/src/ns-allinone-2.34/tclcl-1.19

ns:             /usr/src/ns-allinone-2.34/ns-2.34/ns

nam:    /usr/src/ns-allinone-2.34/nam-1.14/nam

xgraph: /usr/src/ns-allinone-2.34/xgraph-12.1/xgraph

gt-itm:   /usr/src/ns-allinone-2.34/itm, edriver, sgb2alt, sgb2ns, sgb2comns, sgb2hierns

----------------------------------------------------------------------------------

Please put /usr/src/ns-allinone-2.34/bin:/usr/src/ns-allinone-2.34/tcl8.4.18/unix:/usr/src/ns-allinone-2.34/tk8.4.18/unix

into your PATH environment; so that you'll be able to run itm/tclsh/wish/xgraph.

IMPORTANT NOTICES:

(1) You MUST put /usr/src/ns-allinone-2.34/otcl-1.13, /usr/src/ns-allinone-2.34/lib,

    into your LD_LIBRARY_PATH environment variable.

    If it complains about X libraries, add path to your X libraries

    into LD_LIBRARY_PATH.

    If you are using csh, you can set it like:

                setenv LD_LIBRARY_PATH <paths>

    If you are using sh, you can set it like:

                export LD_LIBRARY_PATH=<paths>

(2) You MUST put /usr/src/ns-allinone-2.34/tcl8.4.18/library into your TCL_LIBRARY environmental

    variable. Otherwise ns/nam will complain during startup.


After these steps, you can now run the ns validation suite with

cd ns-2.34; ./validate

For trouble shooting, please first read ns problems page
[http://www.isi.edu/nsnam/ns/ns-problems.html](http://www.isi.edu/nsnam/ns/ns-problems.html).
 Also search the ns mailing list archive

for related posts.




4、(！！！注意上述安装过程最后结尾处红色的提示信息！！！！)

网上很多教程都说，将上面提及的几个环境变量加入到/home/username/.bashrc文件中，这样做确实也没一点问题。可问题是如果你是系统管理，搭好的环境要给研究所或全公司所有的同事使用的话，将上述三个环境变量添加到/etc/profile文件中不失为一种很简便省事的方法。

上面的./validate其实完全没有必要执行（个人觉得，执行时间又长，而且也没啥意义，我们要相信自己^_^~~~）。



5、切换到ns-allinone-2.34/ns-2.34/tcl/ex目录下，然后执行：

[root@maple ex]# ns simple.tcl

怎么样，久违的nam界面跃然眼前吧![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)

![](http://hi.csdn.net/attachment/201111/17/0_1321540270zBQt.gif)



The end!













