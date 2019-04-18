# 在Windows中安装 HTCondor - cocoonyang的专栏 - CSDN博客





2015年11月18日 10:56:46[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：2148








本文介绍如何在Windows操作系统中使用msi安装包在安装 HTCondor

 软件环境： 

     操作系统： Windows 7 

    HTCondor： 8.5.0 版







详细步骤

  1.  下载 condor-8.5.0-344547-Windows-x86.msi安装包

  2.  运行condor-8.5.0-344547-Windows-x86.msi安装包。出现安装包启始界面，




![](https://img-blog.csdn.net/20151118104929418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


点击 Next按钮。




3.  安装包版权界面，




![](https://img-blog.csdn.net/20151118104944092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


点击 I accept the terms in the License Agreement前的选择框，同意版权协议。点击 Next按钮。




4. 

![](https://img-blog.csdn.net/20151118105005360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果是创建一个新HTCondor机群，需设置机群名称。

如果是加入一个已有的机群，要设置机群中运行管理任务的计算机节点的host名称。










![](https://img-blog.csdn.net/20151118105028305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20151118105047733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20151118105053829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20151118105058643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20151118105104237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20151118105111783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20151118105117370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在windows环境中，HTCondor最好安装在C盘上

![](https://img-blog.csdn.net/20151118105123006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20151118105129049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20151118105609820)





![](https://img-blog.csdn.net/20151118105146969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


选择接受，点击“Yes”按钮。




计算机重启后，在console 控制台 （cmd）中运行 "condor_store_cred add"命令。


输入 管理员密码。



在console 控制台 （cmd）中运行 "sc ceate condor binpath= c:\condor\bin\condor_master.exe"命令。


在console
 控制台 （cmd）中运行 "net start condor"命令。





master节点的config文件内容




```
######################################################################
##
##  condor_config
##
##  This is the global configuration file for condor. This is where
##  you define where the local config file is. Any settings
##  made here may potentially be overridden in the local configuration
##  file.  KEEP THAT IN MIND!  To double-check that a variable is
##  getting set from the configuration file that you expect, use
##  condor_config_val -v <variable name>
##
##  condor_config.annotated is a more detailed sample config file
##
##  Unless otherwise specified, settings that are commented out show
##  the defaults that are used if you don't define a value.  Settings
##  that are defined here MUST BE DEFINED since they have no default
##  value.
##
######################################################################

##  Where have you installed the bin, sbin and lib condor directories?   
RELEASE_DIR = C:\condor

##  Where is the local condor directory for each host?  This is where the local config file(s), logs and
##  spool/execute directories are located. this is the default for Linux and Unix systems.
#LOCAL_DIR = $(TILDE)
##  this is the default on Windows sytems
#LOCAL_DIR = $(RELEASE_DIR)

##  Where is the machine-specific local config file for each host?
LOCAL_CONFIG_FILE = $(LOCAL_DIR)\condor_config.local
##  If your configuration is on a shared file system, then this might be a better default
#LOCAL_CONFIG_FILE = $(RELEASE_DIR)\etc\$(HOSTNAME).local
##  If the local config file is not present, is it an error? (WARNING: This is a potential security issue.)
REQUIRE_LOCAL_CONFIG_FILE = FALSE

##  The normal way to do configuration with RPMs is to read all of the
##  files in a given directory that don't match a regex as configuration files.
##  Config files are read in lexicographic order.
LOCAL_CONFIG_DIR = $(LOCAL_DIR)\config
#LOCAL_CONFIG_DIR_EXCLUDE_REGEXP = ^((\..*)|(.*~)|(#.*)|(.*\.rpmsave)|(.*\.rpmnew))$

##  Use a host-based security policy. By default CONDOR_HOST and the local machine will be allowed
use SECURITY : HOST_BASED
##  To expand your condor pool beyond a single host, set ALLOW_WRITE to match all of the hosts
#ALLOW_WRITE = *.cs.wisc.edu
##  FLOCK_FROM defines the machines that grant access to your pool via flocking. (i.e. these machines can join your pool).
#FLOCK_FROM =
##  FLOCK_TO defines the central managers that your schedd will advertise itself to (i.e. these pools will give matches to your schedd).
#FLOCK_TO = condor.cs.wisc.edu, cm.example.edu

##--------------------------------------------------------------------
## Values set by the condor_configure script:
##--------------------------------------------------------------------

CONDOR_HOST = $(FULL_HOSTNAME)
COLLECTOR_NAME = foo
UID_DOMAIN = 
CONDOR_ADMIN = 
SMTP_SERVER = 
ALLOW_READ = *
ALLOW_WRITE = $(CONDOR_HOST), $(IP_ADDRESS), *.cs.wisc.edu
ALLOW_ADMINISTRATOR = $(IP_ADDRESS)
JAVA = C:\PROGRA~2\Java\jre6\bin\java.exe
use POLICY : ALWAYS_RUN_JOBS
WANT_VACATE = FALSE
WANT_SUSPEND = TRUE
DAEMON_LIST = MASTER SCHEDD COLLECTOR NEGOTIATOR STARTD
```










