# compile vim with python - KinboSong的博客 - CSDN博客
2017年06月21日 13:47:03[songjinbo3](https://me.csdn.net/KinboSong)阅读数：922
参考：[https://github.com/Valloric/YouCompleteMe/wiki/Building-Vim-from-source](https://github.com/Valloric/YouCompleteMe/wiki/Building-Vim-from-source)
[https://stackoverflow.com/questions/20104827/vim-loading-python-on-linux](https://stackoverflow.com/questions/20104827/vim-loading-python-on-linux)
环境 ：raspbian系统
1、完全卸载vim
（1）使用whereis vim 命令 查看vim命令的路径，然后用rm命令逐个删除
（2）用sudo find / -name vim命令查找所有的vim相关的文件或者文件夹，然后删除
2、github下载vim的源码文件并进入vim源码目录 
3、配置编译设置
./configure --with-features=huge \
            --enable-multibyte \
            --enable-rubyinterp=yes\
            --enable-pythoninterp=yes \
           --enable-python3interp=yes \
           =/usr/lib/python3.5/config \
            --enable-perlinterp=yes\
            --enable-luainterp=yes \
            --enable-gui=gtk2--enable-cscope --prefix=/usr
一定不要加上 --with-python3-config-dir =<python路径>
4、make VIMRUNTIMEDIR=/usr/share/vim/vim80
5、sudo make install
6、vim --version | grep python命令会显示出vim已经支持python,现在就可以使用依靠python的插件了，如YCM
