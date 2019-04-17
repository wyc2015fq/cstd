# linux 下安装 tar.gz包 - miner_zhu的博客 - CSDN博客





2018年10月18日 16:56:32[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：171








**1）解压tar.gz包**

　“**tar –xvzf 软件包名”**，就可以完成解压与解包工作。

　“**tar -zxvf 软件包名.tar.gz -C /home/hao**”   解压到指定的目录下 （注意：-C为大写）

**2）进入解压后的文件目录下 执行“./configure”命令为编译做好准备**

**　“./configure --prefix=/opt”，**表示安装到/opt目录

**3） 执行“make”命令进行软件编译**

编译完后可以执行**make check**进行测试（可不执行）

**4） 执行“make install”完成安装（部分包需要切换root用户权限方可执行）**
**5） 执行“make clean”删除安装时产生的临时文件（可不执行）**



