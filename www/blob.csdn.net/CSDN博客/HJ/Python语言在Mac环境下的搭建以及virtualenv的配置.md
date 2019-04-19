# Python语言在Mac环境下的搭建以及virtualenv的配置 - HJ - CSDN博客
2018年10月25日 22:50:00[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：69
一、Python语言环境搭建
Mac自带python 2.7的环境，因此只需要配置python 3.7环境即可
输入brew  install python  则系统会自动下载2和3当前的最新版本
接下来可以顺便安装好virtualenv  也就是虚拟环境  输入 pip install virtualenv
环境配置的地方有两个  分别是  .bash_profile  和  ~/.bashrc  (~/.bashrc的配置放在后面）
因为需要构造Python虚拟环境 virtualenv  输入  vim .bash_profile   添加python 3.7的bin路径 source更新
二、virtualenv虚拟环境的使用
因为前面的配置已经把Python 3.7的路径写入到 ~/.bashrc中去了， 因此此时虚拟环境对应的环境是Python3
输入 vim ~/.bashrc 在其中写入两行
```
export  WORKON_HOME=~/Envs
  source  python 3.7的bin路径后缀加上/virtualenv.sh
```
:wq保存退出后 输入 source更新
输入 virtualenv venv (venv为任意取的目录名 ，若省略名字，将会把文件均放在当前目录)
注意：创建目录的作用是，目录中会创建一个文件夹，包含了Python可执行文件，以及pip库的一份拷贝，这样就能利用pip安装其他包了
输入 source /venv/bin/activate  激活虚拟环境  如果需要停用 则输入  . venv/bin/deactivate
如果需要删除该虚拟目录   直接使用 rm -rf venv即可
问题：在配置virtualenv的时候，出现了问题，输入which virtualenv 找不到对应的.sh文件，分析原因可能是利用pip3 install virtualenv 但是Mac默认将virtualenv的环境设置为python2.7
解决办法：
which查找python3.7的位置，将默认指向python2.7的软链接python指向python3.7的/bin目录，同时修改.bashsr文件，修改python地址为python3.7的路径，source实时更新；
