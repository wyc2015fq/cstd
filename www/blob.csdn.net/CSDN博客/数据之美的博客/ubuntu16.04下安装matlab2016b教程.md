# ubuntu16.04下安装matlab2016b教程 - 数据之美的博客 - CSDN博客
2017年02月26日 02:25:53[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：408
个人分类：[linux(ubuntu)](https://blog.csdn.net/oppo62258801/article/category/6738406)
**主题：[Linux](http://lib.csdn.net/base/linux)下安装MATLAB2016**
**首先本文在[Eric2016_Lv](http://blog.csdn.net/Eric2016_Lv/article/details/52653915?locationNum=6#reply)文章基础上进行安装，自己也根据实际情况遇到了一些问题并加以解决，步骤如下：**
**步骤：**
1、**下载：**下载地址见[参考文章](http://blog.csdn.net/Eric2016_Lv/article/details/52653915?locationNum=6#reply)
下载后的Linux文件夹会有三个文件分别是R2016b_glnxa64_dvd1.iso、R2016b_glnxa64_dvd2.iso和Matlab 2016b Linux64 Crack文件夹。[](http://blog.csdn.net/Eric2016_Lv/article/details/52653915?locationNum=6#reply)
rar文件解压，首先安装rar解压工具，再进行Crack文件解压
命令：
sudo apt install rar
rar x Matlab\ 2016b\ Linux64\ Crack.rar
注意：解压后Crack文件夹中包含readme.txt文件，里面包含密钥，**license_standalone.lic**文件，用于进行软件激活，/bin/glnx64/文件，用于进行matlab安装目录中bin/glnx64/的替换，里面工四个文件。
2、挂载：首先建立挂载文件夹，本文为/home/generallc/matlab2016，然后用**mount**命令进行R2016b_glnxa64_dvd1.iso文件挂载，注意当前只挂载R2016b_glnxa64_dvd1.iso文件，R2016b_glnxa64_dvd2.iso先不要进行挂载。
命令：
mkdir /home/generallc/matlab2016                          #挂载目录
sudo mount -t auto -o loop /home/generallc/下载/matlab/R2016b_glnxa64_dvd1.iso /home/generallc/matlab2016
注意：/home/generallc/下载/matlab/为下载的linux版本MATLAB2016文件目录，/home/generallc/matlab2016为指定的挂载目录
3、安装：进入到挂载目录下,可以发现有install文件，然后返回当前目录的前一层进安装
命令：
cd /home/generallc/matlab2016                         #进入到挂载目录下，可以看到install文件
cd ..                                                                                    #返回上一层目录
sudo /home/generallc/matlab2016/install     # 进行安装、弹出安装界面，类似windows安装，安装密钥在Crack文件中的readme.txt中
注意：记住matlab安装目录，本文为/home/generallc/MATLAB
4、安装R2016b_glnxa64_dvd2.iso
当R2016b_glnxa64_dvd1.iso安装完成后，提示拔出dvd1，然后插入dvd2对话框，此时需要挂载第二个iso文件(R2016b_glnxa64_dvd2.iso)到/home/generallc/matlab2016文件夹下，这里注意的是，由于你当时的终端窗口正在
 进行安装，所以你是无法进行操作的，所以你需要**ctrl+Alt+t**进行重新开一个终端命令窗口。挂载成功后直接点击ok就可以了。
命令：
ctrl+Alt+t    
 #新建窗口
sudo mount -t auto -o loop /home/generallc/下载/matlab/R2016b_glnxa64_dvd2.iso
 /home/generallc/matlab2016
5、激活：安装完成后，进入/home/generallc/MATLAB/bin
 目录下，运行matlab文件，弹出激活对话框，选择用不联网的方法进行激活。然后用用下载目录中/bin/glnx64/的四个文件替换matlab安装目录中bin/glnx64/里面文件。
命令：license文件加载
cd/home/generallc/MATLAB/R2016b/bin  
 #进入到matlab安装文件中
./matlab                                                    
 #运行matlab，弹出激活对话框，选择用不联网的方法进行激活，加载**license_standalone.lic**文件
注意：本文激活过程中，出现了权限不足问题，采用chomd命令提高对**license_standalone.lic**文件和安装目录R2016b的权限
cd
 /home/generallc/下载/matlab          #进入到下载目录中
sudo
 chmod 777 license_standalone.lic      #提高对license_standalone.lic文件的操作权限
cd
 /home/generallc/MATLAB     #进入到matlab安装文件中
sudo
 chmod -R 777 R2016b/      #提高对安装目录R2016b的操作权限
命令：文件替换，用下载目录中/bin/glnx64/的四个文件替换matlab安装目录中bin/glnx64/里面文件。
sudo cp /home/generallc/下载/matlab/R2016b/bin/glnxa64/libmwservices.so /home/generallc/MATLAB/R2016b/bin/glnxa64
sudo cp /home/generallc/下载/matlab/R2016b/bin/glnxa64/libmwlmgrimpl.so /home/generallc/MATLAB/R2016b/bin/glnxa64
sudo cp /home/generallc/下载/matlab/R2016b/bin/glnxa64/libinstutil.so /home/generallc/MATLAB/R2016b/bin/glnxa64
sudo cp /home/generallc/下载/matlab/R2016b/bin/glnxa64/libcufft.so.7.5.18 /home/generallc/MATLAB/R2016b/bin/glnxa64
注意：文中的三个目录：
matlab2016b下载目录：  /home/generallc/下载/matlab
挂载目录：     
 /home/generallc/matlab2016
安装目录：   /home/generallc/MATLAB
