# Linux下vim编辑器对键盘失效 - qq_37385726的博客 - CSDN博客





2018年09月17日 15:27:30[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：335








这是因为Ubuntu内置装的是vim-tiny版本的，我们需要卸载，再装完整版。



步骤一，输入下述命令以卸载vim-tiny：

**sudo apt-get remove vim-common**

步骤二，输入下述命令以安装vim-full：

**sudo apt-get install vim**





