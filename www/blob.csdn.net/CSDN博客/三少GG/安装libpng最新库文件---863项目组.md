# 安装libpng最新库文件---863项目组 - 三少GG - CSDN博客
2010年09月20日 20:17:00[三少GG](https://me.csdn.net/scut1135)阅读数：2150
### 安装libpng最新库文件
**2.2.6  安装libpng最新库文件**
步骤一：进入软件源码包所在目录/usr/local/src/中，解压软件包libpng-1.2.31.tar.gz到当前目录libpng-1.2.31下，并进入libpng-1.2.31目录。命令行如下所示。
- [root@localhost root]# cd /usr/local/src/       
//进入软件源码包所在目录  
- [root@localhost src]# tar zxvf libpng-1.2.31.tar.gz     
//解包解压到libpng-1.2.31目录  
- [root@localhost src]# cd libpng-1.2.31           
//进入目录libpng-1.2.31中 
步骤二：同样使用"configure"命令检查并配置安装需要的系统环境，并生成安装配置文件。命令行如下。
- [root@localhost libpng-1.2.31]# ./configure --prefix=/usr/local/libpng 
选项--prefix=/usr/local/libpng，是在安装时将软件安装到/usr/local/libpng目录下
步骤三：使用"make"命令编译源代码文件并生成安装文件。命令行如下。
- [root@localhost libpng-1.2.31]#make       
//对软件源代码文件进行编译 
步骤四：使用"make install"命令进行安装。命令行如下。
- [root@localhost libpng-1.2.31]# make install    
//开始安装libpng库文件 
如果安装成功将会在/usr/local/libpng目录下生成bin、include、lib和share四个目录。在安装GD2库配置时，通过在configure命令的选项中加上"--with-png=/usr/local/libpng"选项，指定libpng库文件的位置。
----------------------------------------------------------------------------------------------------------
-----------------------------------------------
4.安装libpng(若系统未安装zlib,先安装zlib,否则会出错configure: error: zlib not installed)
-----------------------------
安装zlib(使用root安装)
./configure -s
make clean
make
make install
-----------------------------
./configure --prefix=/home/work/local/libpng 
------------------------------------------------------------------------------------------------------------------------------------------------------         
![](http://hi.csdn.net/attachment/201009/20/0_128498493105Fw.gif)
![](http://hi.csdn.net/attachment/201009/20/0_1284984988Rjhx.gif)
