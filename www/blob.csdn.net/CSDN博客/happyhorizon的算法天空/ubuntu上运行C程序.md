# ubuntu上运行C程序 - happyhorizon的算法天空 - CSDN博客
2015年12月08日 10:45:20[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：572
ubuntu版本为Ukylin14.04LTS。首先配置编辑器vim。
**step1：**查看系统是否安装vim。打开终端，输入vi，按下tab键，如果列表里没有vim，说明系统没有安装。
**step2：**下载安装 
suto apt-get vim-gtk
**step3：**美化vim编辑器显示配置 
在终端输入：sudo vim /etc/vim/vimrc 
打开vimrc文件后，在末尾添加下面几句： 
set nu                           // 在左侧行号 
set tabstop =4                 //tab 长度设置为 4 
set nobackup               //覆盖文件时不备份 
set cursorline               //突出显示当前行 
set ruler                       //在右下角显示光标位置的状态行 
set autoindent             //自动缩进
**step4：**编辑C程序，完成后输入:w， 按回车，：q再按回车，保存，终端推出vim编辑器。
**step5：**编译运行程序。首先修改用户权限为root管理员用户。在终端中输入： 
sudo -s
有时会提示“认证失败”，可能的原因是没有设置root密码。 
首次设置时，在终端中输入： 
sudo passwd root 
回车后按照提示输入密码，设置成功后，就自动进入了管理员用户，标志是光标前那句话中，@符号前面是root
设置好密码后，在终端输入sudo -s或者su root都可以进入管理员权限。
例如刚才程序名为hello.c，在终端输入： 
gcc -o hello hello.c  
就完成了编译，编译成功后的文件为hello.o
运行只需要输入： 
./hello
执行完成后，可以通过echo命令来获得其返回值。在终端输入： 
echo $?
