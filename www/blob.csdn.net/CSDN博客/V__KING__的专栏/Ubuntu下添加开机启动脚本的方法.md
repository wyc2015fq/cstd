# Ubuntu下添加开机启动脚本的方法 - V__KING__的专栏 - CSDN博客





2014年04月02日 10:36:48[v__king__](https://me.csdn.net/V__KING__)阅读数：3913













参考：


http://hi.baidu.com/liyaowang/item/4deb134187efeeab60d7b947



**1、方法一，编辑rc.loacl脚本 **


Ubuntu开机之后会执行**/etc/rc.local**文件中的脚本，


所以我们可以直接在**/etc/rc.local**中添加启动脚本。


当然要添加到语句：**exit 0** 前面才行。


如：


sudo vi /etc/rc.local


然后在 exit 0 前面添加好脚本代码。

** 2、方法二，添加一个Ubuntu的开机启动服务。**
- **在/etc/rc.d/--有下列目录rc1.d rc2.d rc3.d rc4.d rc5.d rc6.dinit.d 还有下列文件 rc rc.local rc.sysinit rcn.d (n为1到6) 是对应于不同的runlevel下起不同的服务. 这些目录下都 是一些符号连接, 连接到init.d下的一些文件.以S开头的表示要启动, 以K开头的不启动. 第一个字母后面的数值是一个优先级, 这个优先级是用chkconfig来维护的. init.d下 的每一个文件都有类似下面的话: # chkconfig: 2345 20 80 # description: Saves and restores system entropy pool for \ # higher quality random number generation. 看有chkconfig的那一行, 2345表示在runlevel 2 3 4 5下被启动, 20 80 是优先级 20为启动, 80为关闭. 即在rc3.d你可能看到 S20 或 K80. Redhat下的setup也是用 它处理的. 你可以再参考一下man chkconfig. 有一点注意, 优先即是一个两位数. RedHat下启动网络一定要启动network的服务, 该服务会执行 /etc/sysconfig/network, 并用ifup加载所有的/etc/sysconfig/network-script 下定义的网卡. 注意, 在缺省配置中这些定义文件名中不可以有'.' 象ipforward这样的定义 也是在它中完成的. 你可以看看它的源文件. 它还定义了一个重要的变量$NETWORK, 其他的所 有网络服务都通过检查该变量来决定是否启动. 另外的三个文件是这样的 先执行rc.sysinit 完成了包括mount分区 激活swap 加载modules等重要的工作. 再执行rc.local 完成一些本地的处理, 缺省情况下, 它几乎什么都没作. 最后是执行rc 启动所有的服务.**


如果要添加为开机启动执行的脚本文件，


可先将脚本**复制**或者**软连接**到**/etc/init.d/**目录下，


然后用：**update-rc.d** xxx defaults NN命令(NN为启动顺序)，


将脚本添加到初始化执行的队列中去。


注意如果脚本需要用到**网络**，则NN需设置一个比较大的数字，如99。

**1) 将你的启动脚本复制到 /etc/init.d目录下**

以下假设你的脚本文件名为 test。



**2) 设置脚本文件的权限**


$ sudo chmod 755 /etc/init.d/test

**3) 执行如下命令将脚本放到启动脚本中去：**

$ cd /etc/init.d


$ sudo update-rc.d test defaults 95

注：其中数字95是脚本启动的顺序号，按照自己的需要相应修改即可。在你有多个启动脚本，而它们之间又有先后启动的依赖关系时你就知道这个数字的具体作用了。该命令的输出信息参考如下：
update-rc.d: warning: /etc/init.d/test missing LSB information
update-rc.d: see <http://wiki.debian.org/LSBInitScripts>
  Adding system startup for /etc/init.d/test ...
    /etc/rc0.d/K95test -> ../init.d/test
    /etc/rc1.d/K95test -> ../init.d/test
    /etc/rc6.d/K95test -> ../init.d/test
    /etc/rc2.d/S95test -> ../init.d/test
    /etc/rc3.d/S95test -> ../init.d/test
    /etc/rc4.d/S95test -> ../init.d/test
    /etc/rc5.d/S95test -> ../init.d/test


**卸载启动脚本的方法：**

$ cd /etc/init.d




$ sudo update-rc.d -f test remove


命令输出的信息参考如下：
Removing any system startup links for /etc/init.d/test ...
    /etc/rc0.d/K95test
    /etc/rc1.d/K95test
    /etc/rc2.d/S95test
    /etc/rc3.d/S95test
    /etc/rc4.d/S95test
    /etc/rc5.d/S95test
    /etc/rc6.d/K95test



** 3、方法三，第一个用户登录ubuntu时启动/etc/bash.bashrc。**



也可以在/etc/bash.bashrc中添加该行使得在第一个用户登录ubuntu时启动tomcat


或者在用户主目录的.bashrc中添加该行，使得在该用户第一次登录时启动tomcat

** 4、方法四，/etc/profile中设置开机自动执行。**



在/etc/profile或者用户主目录下的.profile也可以设置开机自动执行的脚本，但是一般是用来加载环境变量





参考：



http://hi.baidu.com/liyaowang/item/4deb134187efeeab60d7b947







