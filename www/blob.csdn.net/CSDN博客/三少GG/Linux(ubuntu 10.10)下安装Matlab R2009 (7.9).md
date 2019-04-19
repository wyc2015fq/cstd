# Linux(ubuntu 10.10)下安装Matlab R2009 (7.9) - 三少GG - CSDN博客
2011年11月02日 20:27:25[三少GG](https://me.csdn.net/scut1135)阅读数：2579
[http://hi.baidu.com/wang_song85/blog/item/2bbcd226a4ada05c9358079e.html](http://hi.baidu.com/wang_song85/blog/item/2bbcd226a4ada05c9358079e.html)
参照网上的帮助，我自己安装的步骤总结如下** 2011.11.3**：
    1.下载Matlab for linux&Unix的iso
我用的是r2009a，现在最新的是matlab 2010，可以去verycd上下载，链接在这里：
[http://www.verycd.com/topics/2806608/](http://www.verycd.com/topics/2806608/)，2009和2010安装方法都一样。
** 注：去六维空间下载linux的matlab-r2009b 即matlab7.9**
    2.挂载ISO
终端输入sudo mount -t iso9660  -o loop <iso文件的路径和文件名> /cdrom
例如：sudo mount -t iso9660  -o loop ~/matlab2009a.iso /cdrom
    3.安装路径
终端输入sudo mkdir matlab-r2009a，建立安装matlab 的目录/opt/matlab-r2009a/。
    4.执行安装：
终端：cd /cdrom回车后sudo ./install；接下来就和windows下安装matlab是一样的了，选择不联网安装，中间要输入序列号，你
可以打开/cdrom/crack文件夹，安装说明里面有standalone序列号，复制过来就行了，要填路径时就填上面的
/opt/matlab-r2009a/，最后用/cdrom/crack文件夹的lic_standalone.dat激活matlab。
    5.菜单中建立快捷方式
现在想把matlab添加到系统的“应用 程序->编程”菜单里，方法如下：
sudo gedit /usr/share/applications/matlab.desktop
然后在里面添加如下内容：
[Desktop Entry]
Name=Matlab R2009
Comment=MathWorks Matlab R2009
Exec=/opt/matlab-r2009a/bin/matlab -desktop
Terminal=false
Type=Application
Icon=/opt/matlab-r2009a/toolbox/matlab/timeseries/matlabicon.gif
Categories=Application;Development;
保存后，Matlab就会出现在“应用程序->编程”菜单。
注：Exec指向安装时建立的可执行文件，你要把上面那个地方改成你安装目录下的/bin/matlab。
    6.matlab 中文支持问题
这样安装完，你的matlab不能显示中文，你可以按照本博客另一篇文章中的方法去做，[linux/Ubuntu matlab中文显示为方框(图文)](http://hi.baidu.com/wang_song85/blog/item/d3dde8ff6ab0330fa9d3115f.html)。
**注：此步骤就不要做了**，创建的/usr/share/fonts/truetype/myfonts/simsun.ttc的存在导致整个系统的中文支持都变成了乱码，删除掉后就恢复了。
    7.创建桌面快捷方式
为了方便，创建一个桌面快捷方式，直接把“应用 程序->编程”里matlab图标拖到桌面即可。
    8.权限问题
装好后，由于matlab安装文件夹不具备write权限，可能无法保存对matlab设置的更改，可以终端运行
sudo chmod -R a+w /opt/matlab-r2009a/*赋予all users write权限。
这样matlab就装好了。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2.  **ubuntu中matlab2009的安装**
[http://wenku.baidu.com/view/796000c5bb4cf7ec4afed050.htm](http://wenku.baidu.com/view/796000c5bb4cf7ec4afed050.html)[l](http://wenku.baidu.com/view/796000c5bb4cf7ec4afed050.html)
3. [http://www.douban.com/note/32856955/](http://www.douban.com/note/32856955/)
4. [http://blog.csdn.net/sdfgh2046/article/details/5633675](http://blog.csdn.net/sdfgh2046/article/details/5633675)
