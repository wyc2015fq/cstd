# SlickEdit V21 2016 破解教程，win linux mac - DoubleLi - 博客园






最近主要工作系统转到LInux上面来了，Slickedit的安装破解也费了些事，今天将过程整理一下做个记录。

说明：SlickEdit pro V21.03 Linux 64位实测可用，MAC实测可用， win实测可用。
-------------------------------------------------------------------------------------
WIN和Linux和MAC共同步骤：
1.首先去官网下载SlickEdit 安装包，https://www.slickedit.com/
我下载到的最新Linux版是se_21000300_linux64.tar.gz

MAC版是se_21000300_mac.dmg

2.下载破解程序Slickedit 2016 v21 WIN LINUX keygen.zip

链接: https://pan.baidu.com/s/1dFzStVV 密码: g4ax

-------------------------------------------------------------------------------------
WIN版：： 
1.安装SlickEdit安装程序，安装完成后，先不要打开SlickEdit主程序。 
2.右键管理员运行SlickEdit keygen 破解程序，点击 patch，找到上一步安装SlickEdit 2016主程序的地址，在其中的win目录下找到vs.exe文件，选中，点击打开，显示patch Ok字样代表成功，不成功请检查是否是以管理员身份运行的破解程序。
4.在keygen破解程序界面，点击generate 按钮即可在win目录下生成slickedit.lic文件。 
5.打开SlickEdit主程序，选择上一步生成的lic文件即可完成破解。
-------------------------------------------------------------------------------------
LINUX版(默认安装目录/opt/slickedit-pro2016)：
1.解压安装包
tar zxvf se_21000300_linux64.tar.gz
2.进入解压后目录，运行vsinst安装
cd se_21000300_linux64
sudo ./vsinst
弹出安装信息及license，按住回车不放，直到license显示完，出现“Do you agree to the above license terms?[yes or no]”,输入yes；然后选择安装路径，默认按回车即可；然后询问是否创建目录，输入Y；安装过程中会弹出一个SlickEdit License Manager的对话框，而且有可能对话框中是一片空白(我的就是这样)，直接关闭此对话框，之后又会弹出一个小对话框，也直接关闭，安装完成。
3.破解
将整个安装目录(slickedit-pro2016)和破解程序拷贝到windows电脑里(安装win+Linux双系统的拷贝到win分区中，重启到win系统)，以下操作在win系统中进行：
    找到从linux中拷贝过来的安装目录，先去除安装目录的只读属性;右键管理员运行SlickEdit keygen 破解程序，点击patch，在其中的bin目录下找到vs_exe文件，选中，点击打开，显示patch Ok字样代表成功，不成功请检查是否是以管理员身份运行的破解程序;在keygen破解程序界面，点击generate 按钮即可在bin目录下生成slickedit.lic文件;将生成的slickedit.lic文件和同目录下的vs_exe文件拷贝到Linux中。
回到Linux中，将上面的两个文件拷贝到安装目录/bin/目录下覆盖，
sudo copy vs_exe /opt/slickedit-pro2016/bin/
sudo copy slickedit.lic /opt/slickedit-pro2016/bin/
完成破解操作。

4.为方便使用将其添加到PATH环境变量中
在 ~/.bashrc 或 /etc/profile 脚本的最后添加如下内容(不要写错)：
export PATH=/opt/slickedit-pro2016/bin:$PATH
运行脚本，让环境变量生效
source ~/.bashrc

5.测试
which vs
终端出现：/opt/slickedit-pro2016/bin/vs 证明安装成功

6.启动Slickedit
终端输入vs
启动另一个SlickEdit时需要敲命令：vs +new

-------------------------------------------------------------------------------------
MAC版：

1. 以管理员身份在windows下运行Keygen.exe。 
2. 将/Applications/SlickEditPro2016.app/Contents/MacOS/vs复制到window目录下 
3. 将此文件只读权限去掉 
4. 先点Patch，选中vs 
5. 再Genarate生成slickedit.lic文件 
6. 将vs与slickedit.lic文件复制到/Applications/SlickEditPro2016.app/Contents/MacOS目录下 
7. 使用terminal进入/Applications/SlickEditPro2016.app/Contents/MacOS目录，chmod 777 vs 
8. OK


==============================================================
参考：
http://www.linuxdiyf.com/linux/24556.html
http://blog.csdn.net/tennysonsky/article/details/46119713

http://download.csdn.net/detail/wuya/9735352


补充说明：
关于Linux版的破解，在有些人说只需要替换破解包中的vs_exe和slickedit.lic文件即可，我测试此方法不行，应该是并不适合所有情况，所以推荐我上面写的Linux版破解方法。









