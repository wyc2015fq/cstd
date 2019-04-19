# Cloudera Manager安装 - 三少GG - CSDN博客
2014年02月18日 12:25:07[三少GG](https://me.csdn.net/scut1135)阅读数：2814
安装文件：
**http://archive.cloudera.com/cm4/installer/latest/**
各种版本：
http://archive.cloudera.com/cm4/installer/
参考文章：
1.  http://www.csdn123.com/html/blogs/20130927/75622.htm
2. http://blog.csdn.net/zzhua100/article/details/7823971
**************************************************************************************************************************************
### 在ubuntu下安装Cloudera Manager Server
**目标：**
用Cloudera Manager Server安装并且管理hadoop集群，听说一般企业都用Cloudera安装hadoop并且管理，笔者也想试试，查了一天资料，发现Cloudera已经很成熟了，并且已经有pseudo distributed和cloudera manager方式安装，笔者采用manager的方式安装。
**环境：**
- 主机A  ubuntu12.04 64位 用户名qiuchenl  主机名qiuchenl1  安装有ssh
- 主机B ubuntu12.04 64位 用户名qiuchenl  主机名qiuchenl2  安装有ssh
安装步骤：
- 首先下载Cloudera Manager Server的安装文件，就是一个bin文件会自动去下载安装（下载地址：[https://ccp.cloudera.com/display/SUPPORT/Cloudera+Manager+Free+Edition+Download](https://ccp.cloudera.com/display/SUPPORT/Cloudera+Manager+Free+Edition+Download)）我是下载了一个最新版本的下载完文件名为：cloudera-manager-installer.bin
 开始安装软件，执行以下命令：（官方指南：[https://ccp.cloudera.com/display/express37/Automated+Installation+of+CDH+by+Cloudera+Manager](https://ccp.cloudera.com/display/express37/Automated+Installation+of+CDH+by+Cloudera+Manager)）
- chmod u+x cloudra-manager-installer.bin
- sudo ./cloudra-manager-installer.bin
- 现在就进入到安装界面，基本都是一路next，其中需要注意的地方有：
- 开始安装，可能是网速的原因，界面会没有响音，这是只需要耐心等待，不要强制关闭安装程序，如果强制关闭，再次安装就安装不了，卸载也卸载不了，只能重装系统
- 安装完毕会提示你用浏览器打开[http://localhost:7180](http://localhost:7180/) 安装hadoop集群
- 在安装hadoop集群界面前面随意填，在后面界面有一个需要ssh配置的需要用户来选择是root还是用户名，ssh公钥密钥登入还是密码登陆，笔者选择的是root用户和密码登陆（这里需要注意的是你所有主机的root密码必须一样，注意是root密码，不是你的用户密码）剩下的就是漫长的等待，会依次安装hadoop和hbase，hive等等。
- 
To download and run the Cloudera Manager installer:
- Download cloudera-manager-installer.bin from the [Cloudera
 Downloads page](http://www.cloudera.com/content/support/en/downloads.html) to the host where you want to install the Cloudera Manager Server. The host must be on your cluster or accessible to your cluster over your network. Install Cloudera Manager on a single host.
- After downloading cloudera-manager-installer.bin, change it to have executable permission.
$ chmod u+x cloudera-manager-installer.bin
- Run cloudera-manager-installer.bin.
![](http://www.cloudera.com/content/cloudera-content/cloudera-docs/CM4Ent/latest/static/note.jpg)Note: The
 installer's default behavior is to install the Cloudera Manager packages from the Internet. If you have created a local repository and configured your machine to recognize that repository, you can instruct the installer to use local repositories by running
 the cloudera-manager-installer.bin with the --skip_repo_package=1 option.
$ sudo ./cloudera-manager-installer.bin
- Read the Cloudera Manager Readme and then press Enter to choose Next.
- Read the Cloudera Manager License and then press Enter to choose Next. Use the arrow keys and
 press Enter to choose Yes to confirm you accept the license.
- Read the Oracle Binary Code License Agreement and then press Enter to choose Next. Use the arrow
 keys and press Enter to choose Yes to confirm you accept the Oracle Binary Code License Agreement. The Cloudera Manager installer begins installing the Oracle
 JDK and the Cloudera Manager repo files and then installs the packages. The installer also installs the Cloudera Manager Server.
![](http://www.cloudera.com/content/cloudera-content/cloudera-docs/CM4Ent/latest/static/note.jpg)Note:
If an error message "Failed to start server" appears while running cloudera-manager-installer.bin, exit the installation program. If the Cloudera Manager
 Server log file /var/log/cloudera-scm-server/cloudera-scm-server.log contains the following message, then it's likely you have SELinux enabled
Caused by: java.lang.ClassNotFoundException: com.mysql.jdbc.Driver
        at java.net.URLClassLoader$1.run(Unknown Source)
        at java.security.AccessController.doPrivileged(Native Method)
        at java.net.URLClassLoader.findClass(Unknown Source)
        at java.lang.ClassLoader.loadClass(Unknown Source)
        ...
You can disable SELinux by running the following command on the Cloudera Manager Server host:
$ sudo setenforce 0
To disable it permanently, edit /etc/selinux/config.
- Note the complete URL provided for the Cloudera Manager Admin Console, including the port number, which is 7180 by default. Press Enter to choose OKto
 continue.
- Press Enter to choose OK to exit the installer.
![](http://www.cloudera.com/content/cloudera-content/cloudera-docs/CM4Ent/latest/static/note.jpg)Note:
If the installation is interrupted for some reason, you may need to clean up before you can re-run it. See [Uninstalling
 Cloudera Manager](http://www.cloudera.com/content/cloudera-content/cloudera-docs/CM4Ent/latest/Cloudera-Manager-Installation-Guide/cmig_uninstall_CM.html#cmig_topic_18).
# Uninstalling Cloudera Manager and CDH
If necessary, you can use the following instructions to uninstall the Cloudera Manager Server, databases, and Agents, and CDH software and data.
If you have come to this procedure because your installation did not complete (for example, if it was interrupted by a virtual machine timeout), and you want to proceed with the installation, do the following:
- Remove files and directories:
$ sudo rm -Rf /usr/share/cmf /var/lib/cloudera* /var/cache/yum/cloudera*
- Run the installer again.
Otherwise, to completely uninstall Cloudera Manager and CDH software and data, follow these steps:
- [Uninstalling
 Cloudera Manager and CDH Software](http://cloudera.com/content/cloudera-content/cloudera-docs/CM4Ent/latest/Cloudera-Manager-Installation-Guide/cmig_uninstall_CM.html#cmig_topic_18_1_unique_1)
- [Removing
 Cloudera Manager and CDH Data](http://cloudera.com/content/cloudera-content/cloudera-docs/CM4Ent/latest/Cloudera-Manager-Installation-Guide/cmig_uninstall_CM.html#concept_ykk_jgz_jm_unique_1)
![](http://cloudera.com/content/cloudera-content/cloudera-docs/CM4Ent/latest/static/note.jpg)Note: The
 second step removes all CDH user data. To preserve the data, copy it to another cluster using the distcp command before starting the uninstall process.
**************************************************************************************************************************************
error messages:
"Installation failed. Failed to receive heartbeat from agent.
Ensure that the host's hostname is configured properly.
Ensure that port 7182 is accessible on the Cloudera Manager server (check firewall rules)."
Hi Meredith,
Please gather the outputs of these directly from the commandline on the node in question and paste the results back here so we may compare it to your current hosts file:
$ hostname
$ hostname -f
$ python -c 'import socket; print socket.getfqdn(), socket.gethostbyname(socket.getfqdn())'
Thanks,
Hey Meredith,
The cloudera-scm-agent can't determine what this machine specifically believes its fully-qualified domain name is. The hostname -f queries that, as does the python blurb. Let's resolve that:
Assuming this is a centos/rhel machine, what do you have set in /etc/sysconfig/network for the fully-qualified domain name? It'd also be useful to add in /etc/hosts with the format
IP     FQDN    shortname
Your case has only IP and FQDN. Just ensure the shortname comes last for any entries that have them.
If not centos/rhel, ensure that you set hostname accordingly for your OS, then run those three commands again. Once those all return what's expected (the latter will return both FQDN and IP) we should be in a great place for you to try starting the agent again.
Thank you for all of your help!  I'm on Ubuntu, so no /etc/sysconfig/network file.
  However, I did add "IP     FQDN    shortname" into the hosts file which seemed to solve the hostname -f issue.  I then ran the 3rd command successfully.  Then when I went back to the GUI to setup the hosts, it said that the host
 already existed on the machine, so I just added the parcels to that machine.  I am now to the point where everything appears to be installed and I'm configuring/fixing the health.  Thank you so much for your help!
