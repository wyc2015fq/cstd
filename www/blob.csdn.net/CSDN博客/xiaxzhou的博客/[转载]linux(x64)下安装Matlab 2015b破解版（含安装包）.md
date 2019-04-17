# [转载]linux(x64)下安装Matlab 2015b破解版（含安装包） - xiaxzhou的博客 - CSDN博客





2017年06月14日 10:00:16[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：1471








转载：[http://blog.csdn.net/hejunqing14/article/details/50265049](http://blog.csdn.net/hejunqing14/article/details/50265049)

注意：在安装前请查看安装目录是否有足够空间！完全安装大概需要12G的空间！本人在安装后系统盘满了，导致无法启动图形界面。小伙伴们不要重蹈覆辙~

Environment

Linux debian8 (x64), it is the same in other linux x64 systems

Install Packages

Matlab R2015b_glnxa64.iso and Crack files  

in BaiduPan: [[https://pan.baidu.com/s/1gf9IeCN]](https://pan.baidu.com/s/1gf9IeCN]), password: 4gj3

Install Steps

(1). 挂载ISO镜像文件

```bash
$ sudo mkdir /media/matlab

$ sudo mount -o loop R2015b_glnxa64.iso /media/matlab
```

(2). 执行安装过程，选择不联网安装，序列号在~/crack/readme.txt文件中。

```bash
$ cd /media/matlab

$ sudo ./install
```

attention: must install using sudo or su,or will face cannot build directory failure later  

如果安装的时候没有弹出图形安装界面，直接显示installing… finish! 则需要gksudo 或kdesudo 来安装

(3). 安装完毕并且将~/crack/bin/中的文件复制到~/MATLAB/Rxxxx/bin中

```bash
$ sudo cp /[Your crack directory]/Matlab_R2015b/Matlab_2015b_Linux64_Crack/R2015b/bin/glnxa64/* (安装位置)/MATLAB/R2015b/bin/glnxa64
```

(4).首次运行matlab要用root权限（否则无法写文件），采用不联网激活，找到Crack中相应的激活文件*.lic，导入激活。

```bash
$ cd (安装位置)/MATLAB/R2015b/bin

$ sudo ./matlab
```

(5). 卸载ISO镜像。

```bash
$ sudo umount /media/matlab
```

(6). 添加桌面快捷方式。

新建一个桌面配置文件，文件名为Ｍatab2015b.desktop： $ sudo nano /usr/share/applications/Matlab2015b.desktop ，其内容如下：若安装在其他地方，更改/usr/local/为安装目录即可

```
GNU nano 2.2.6   File: /usr/share/applications/Matlab2015b.desktop   
[Desktop Entry]
Encoding=UTF-8
Name=Matlab 2015b
Exec=/usr/local/MATLAB/R2015b/bin/matlab
Icon=/usr/local/MATLAB/R2015b/toolbox/shared/dastudio/resources/MatlabIcon.png
Terminal=true
Type=Application
Name[zh_CN]=Matlab_2015b
```

重启电脑，桌面上就会出现Matlab图标，点击即可运行~

注意，上面必须要有Terminal的值一定要是true,因为matlab只能在命令行运行！之前我写的桌面文件没有这个关键字，点开之后总是闪退
- **配置matlab环境变量**：

```bash
sudo gedit /etc/profile
```

添加语句：

```bash
#add matblab path
export PATH=$PATH:(安装路径)/matlab/bin
```

上面操作完成后需要注销Linux才能使刚才的环境变量设置生效。
- **uninstall**

```
$ sudo rm -rf /usr/local/MATLAB/R2015b

$ sudo rm /usr/local/bin/matlab /usr/local/bin/mcc /usr/local/bin/mex /usr/local/bin/mbuild
```

仅供参考，这些文件可能不存在

启动matlab出现问题：

**ubuntu下matlab启动报错java.lang.runtime.Exception**

找了半天只需以下一句命令:

```bash
sudo chmod -R a+rw ~/.matlab
```

### 编译出现问题：
- make:   mexext 命令未找到

[http://blog.csdn.net/github_38140310/article/details/70217653](http://blog.csdn.net/github_38140310/article/details/70217653)

将MATLAB/R2015b/bin文件夹拷贝到链接出错的位置文件中即可
- version `GLIBCXX_3.4.21’ not found

错误如下：

```
Invalid MEX-file
'/home/xiaxzhou/graduate/objectContourDetector/caffe-cedn/matlab/caffe/caffe.mexa64':
/usr/local/MATLAB/R2015b/bin/glnxa64/../../sys/os/glnxa64/libstdc++.so.6:
version `GLIBCXX_3.4.21' not found (required by
/home/xiaxzhou/graduate/objectContourDetector/caffe-cedn/matlab/caffe/caffe.mexa64)
```

解决：

执行命令：
`ln -s /usr/lib/x86_64-linux-gnu/libstdc++.so.6  /usr/local/MATLAB/R2015b/bin/glnxa64/../../sys/os/glnxa64/libstdc++.so.6`
若提示libstdc++.so.6已存在，删除，再执行命令即可

[参考](https://askubuntu.com/questions/719028/version-glibcxx-3-4-21-not-found)



