# ubuntu vi编辑器插入模式下方向键与退格问题 - YuDale的博客 - CSDN博客
2017年03月24日 19:26:36[YuDale](https://me.csdn.net/YuDale)阅读数：715
因为没有下载完整的vim包导致方向键在插入模式下变为ABCD，退格键不能使用，编辑很是不方便。
在这里提供一种方案可以解决上述问题。
1.sudo vi /etc/vim/vimrc.tiny
2.vim:set ft=vim:前面的 set compatible修改为set nocompatible  解决方向键问题
再在下面加上
set backspace=2修改退格键问题。
