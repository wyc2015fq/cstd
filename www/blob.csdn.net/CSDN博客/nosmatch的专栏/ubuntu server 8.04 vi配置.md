# ubuntu server 8.04 vi配置 - nosmatch的专栏 - CSDN博客
2011年09月23日 19:36:16[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：406标签：[ubuntu																[server																[string																[n2																[shell																[vim](https://so.csdn.net/so/search/s.do?q=vim&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=n2&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
Ubuntu 8.04 server自带vim-tiny,如安装vim-full,需先卸载vim-tiny
#dpkg -l |grep vi
#sudo apt-get remove vim-tiny
#sudo apt-get install vim
安装好vim-full后,**设置vi编辑环境**
#vi ~/.vimrc
set number      //显示行数 set nu
set ruler    //显示标尺 set ru
set show cmd     //显示当前输入的命令 set sc
set whichwrap=h,l,<,>   //运行h,l,<,和>键换行
set ic   //搜索时忽略大小写 (ignore case)
set incsearch   //输入字符后即开始搜索 set is
set hlsearch   //高亮显示搜索文本 set hls
syntax enable   //语法高亮 sy on
hi comment ctermfg = green   //绿色显示注释 (highlight)
**vi编辑器的使用**
进入vi后就是在**编辑模式**下
h : 左移
j : 下移
k : 上移
l : 右移
dd : 删除光标所在行
ndd : 删除n行(向后)
yy : 复制当前行
y$ : 复制光标所在位置到结尾
p : 粘贴
/string : 向后搜索
?string : 向前搜索
n : 继续搜索
N : 反向搜索
u : 撤销操作
. : 撤除撤销
ZZ : 存盘退出
ZQ : 不存盘退出
编辑模式下输入i,a,o,c,r或s进入**插入模式**
i :光标所在位置前插入文本
a :光标所在位置后插入文本
o :光标所在行下面新开一行插入
s :删除当前字符并进入插入模式
编辑模式下按:进入**命令模式**
:n 跳到第n行
:wq 存盘退出
:q! 不存盘退出
:w 文件名 //当前编辑内容写入到另一文件中
:n1,n2 w 文件名 //n1到n2行的内容写入到另一文件中
:nw 文件名 //第n行内容写入到另一文件中
:1,. w 文件名 //第一行到光标当前位置
:.,$ w 文件名 //光标当前位置到结尾
:r 文件名 //打开另一文件
:e 文件名 //新建一文件
:f 文件名 //当前文件重命名
:s/string1/string2 //将第一个string1替换成string2
:s/string1/string2/g //将所有string1替换成string2
:n1,n2 d //删除n1行到n2行
:.,$ d //删除当前行到结尾
:!命令 //执行shell命令
编辑模式下按v键进入**可视化模式**
移动方向键选取内容
按y复制
按x剪切
按p粘贴
