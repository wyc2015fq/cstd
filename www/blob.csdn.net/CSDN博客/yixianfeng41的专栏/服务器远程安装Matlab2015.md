# 服务器远程安装Matlab2015 - yixianfeng41的专栏 - CSDN博客
2017年04月17日 20:42:27[第2梦](https://me.csdn.net/yixianfeng41)阅读数：4883
因为在windows上跑matlab实验，出现内存不足的情况，因此，希望能在服务器上安装一个matlab。然后出现各种问题，花了一天时间才装好，特此记录下。因为对于没装过的童靴来说，每一个步骤都很关键，因此，尽量把每个步骤写的细一点。
### Matlab镜像文件
matlab 安装镜像文件我是从这儿下载的，再次对上传者表示感谢，地址：[镜像文件](https://pan.baidu.com/s/1gf9IeCN%5D#list/path=/)，提取码为：4gj3。原博客在参考资料【3】
### 安装步骤
1、上传镜像文件到服务器 
我一般用的是XSHELL远程登录服务器，它有一个文件传输助手XFTP,装上这个后，文件上传很快，几M每秒
2、挂载镜像
```
$sudo mkdir /home/matlab #文件位置随意，文件名随意
$sudo mount -o loop R2015b_glnxa64.iso /home/matlab
```
挂载成功后，在/home/matlab文件下会有如下文件：
![这里写图片描述](https://img-blog.csdn.net/20170417194933323)
3、创建安装目录
```
$mkdir -p usr/local/matlab/etc
$mkdir -p usr/local/matlab/R2015b
```
其实上面两个文件不是非必须的，为了规整，还是建这两个文件好一些，文件位置看个人习惯。第一个是用于存放激活文件，license文件，安装选项文件等；第二个文件就是自定义的软件安装位置。
4、编辑安装及激活文件 
运行/home/matlab文件下的install文件，注意要填上安装选项，不然不会成功的，可以这样
```bash
$sudo /home/matlab/install.sh -mode silent -fileInstallationKey 09806-07443-53955-64350-21751-41297 #把相关其他属性填上
```
但是最好还是用一个文件保存这些安装选项，名字叫installer_input.txt。   
**编辑installer_input.txt**
```java
destinationFolder= usr/local/matlab/R2015b //安装目录
fileInstallationKey= 38699-60149-36808-21840-05491 //你的序列号
agreeToLicense=yes //同意协议
outputFile=/tmp/mathwork_install.log //安装日志 （Optional）
mode=silent //开启无人值守安装
activationPropertiesFile=/matlab/etc/activate.ini //激活文件
```
这些文件在挂载后的文件里都有，可以详细看看每个选项什么意思，以及还有其他选项都是干嘛的，这里只用这几个。
**编辑activate文件**
```java
isSilent=true //开启silent模式
activateCommand=activateOffline //设置激活方式, 离线激活 无需联网
licenseFile=/matlab/etc/lic_standalone.lic //license文件位置
```
上面的license在安装文件Crack文件下可以找到
5）执行安装命令
```bash
sudo /home/matlab/install  -inputFile /matlab/etc/installer_input.txt
```
运行一下，以为安装好了，然而并没有，很讨厌下面这个画面 
![这里写图片描述](https://img-blog.csdn.net/20170417203210010)
这是因为没有激活，接下来进行激活：
6、激活
```
$sudo mv /usr/local/matlab/R2015b/bin/glnxa64/libmwservices.so /usr/local/matlab/R2015b/bin/glnxa64/libmwservices.so.bak
$sudo cp ~/Downloads/Matlab 2015b Linux64 Crack/R2015b/bin/libmwservices.so /usr/local/matlab/R2015b/bin/glnxa64/
```
然后执行如下命令：
`sudo /usr/local/matlab/R2015b/bin/bin/activate_matlab.sh -propertiesFile /usr/local/matlab/etc/activate.ini`
到这，就激活好了，来，看看激活成功是什么样子，对于装了一天的人来说，看到这个能把人激动哭！ 
![这里写图片描述](https://img-blog.csdn.net/20170417201817598)
运行下看看： 
![这里写图片描述](https://img-blog.csdn.net/20170417202114227)
成功了，嘿嘿！
启动命令介绍：
|选项|含义|
|----|----|
|-nodesktop|启动jvm(Jave Virtual Machine)，不启动desktop， 但help 界面，preferences界面等仍可通过cmdline 调出，即jvm启动但不启动desktop，可以启动其他显示； 但是matlab不会在cmd history记录本次执行的命令|
|-nodisplay|启动jvm，不启动desktop，不启动任何显示相关，忽略任何DISPLAY 环境变量； 即jvm启动但不能显示|
|-nojvm|不启动jvm，则与之相关的一切活动将无法进行，包括图形界面显示，help 界面，preferences界面等 即jvm不启动故不能显示|
|-nosplash|只是不显示启动时的log画面，jvm，desktop等正常启动|
7、设置环境变量 
设置环境变量
```bash
sudo vi /etc/profile
```
在文件末尾添加
`export PATH=/usr/local/matlab/R2015b/bin:$PATH`
保存并退出后使设置生效
```bash
source /etc/profile
```
安装完成。
8、umount镜像
```bash
$sudo umount /home/matlab
```
#### 参考资料
1、[Linux服务器命令行模式安装Matlab2014a](http://www.aichengxu.com/diannao/39100.htm)
2、[Linux服务器命令方式安装Matlab](http://blog.csdn.net/badboy_1990/article/details/38372017)
3、[linux(x64)下安装Matlab 2015b破解版（含安装包）](http://blog.csdn.net/hejunqing14/article/details/50265049)
