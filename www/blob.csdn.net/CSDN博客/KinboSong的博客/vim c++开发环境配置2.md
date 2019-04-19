# vim c++开发环境配置2 - KinboSong的博客 - CSDN博客
2017年05月09日 16:49:44[songjinbo3](https://me.csdn.net/KinboSong)阅读数：508
主要参考：[http://blog.csdn.net/mergerly/article/details/51671890](http://blog.csdn.net/mergerly/article/details/51671890)
当前用到的插件有ycm,easymotion,Nerdtree,Nerdcommenter,ctrlp,ultisnips(需要依赖snippets),tagbar,powerline,syntastic,sessionman,cscope(vim已经集成此插件)
其中YCM,ultisnips,tagbar,cscope的安装和使用需要额外注意。
一、YCM的安装和配置：
 1、安装youcompleteme插件：
  （1）在.vimrc文件中添加语句：Plugin 'Valloric/YouCompleteMe',并在vim中执行：PluginInstall
  （2）安装好YouComplete之后安装clang
    cd ~/.vim/bundle/YouCompleteMe/
    ./install.sh --clang-completer
  （3）在家目录下面添加文件.ycm_extra_conf.py文件
 2、配置~/.ycm_extra_conf.py文件，用如下格式添加头文件目录
   '-isystem',
   '/home/song/SLAM/vio/orbslam_dwo/inclue',
  注意上面的每一行的逗号不要忘记加
二、Cscope的安装和使用
 1、Cscope的安装
  cscope能够以窗口形式输出查找定义、相同的符号、函数被引位置
 步骤：
（1）sudo apt-get install cscope
（2）产生cscope.out文件并添加进vim中
 具体方法参考：[http://blog.csdn.net/kinbosong/article/details/70521866](http://blog.csdn.net/kinbosong/article/details/70521866)  cscope部分
 注意：只需要安装软件包，**不需要在.vim中安装插件**
 2、Cscope的使用
（1）添加c++系统库文件的cscope数据库
i.进入根目录 cd /
ii.运行命令产生cscope.files文件：sudo find /usr/include -name '*.c' -o -name '*.h' > ~/.cscope/cscope.files 
iii.进入目录~/.cscope/
iv.生成库文件cscope.out ：cscope -b（cscope会自动查找所在目录是否有文件cscope.files，如果有则按照cscope.files的内容来生成）
v.在vim中添加文件~/.cscope/cscope.out (即在.vimrc中输入命令 cs add /home/pi/.cscope/cscope.out)
（2）为当前的工程建立cscope数据库
i.进入工程目录
ii.cscope -Rbq(会产生cscope.in.out，cscope.out，cscope.po.out )
iii.进入vim后输入" :cs add <cscope.out的目录>"添加cscope数据库，如果已经添加过此数据库，会提示"重复cscope数据库未被加入"
三、Tagbar的安装
  1、使用bundle安装好Tagbar
  2、安装ctags软件包。（注意：不是ctags插件）
  安装好之后一定要设置好可执行文件ctags的路径，如下所示：
  let g:tagbar_ctags_bin='/usr/local/ctags/bin/ctags' 
  注意：Tagbar只可以显示全局的函数、变量，不可以显示局部的变量
四、ultisnips的安装
参考：[ultisnips插件的安装与使用](https://blog.csdn.net/guchuanhang/article/details/72953770)
