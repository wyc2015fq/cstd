# cygwin中清屏的方法 - xiahouzuoxin - CSDN博客





2013年12月13日 13:46:06[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2411








# 1. bash脚本

依次执行：

cd /usr/bin/

vim clear.sh




vim编辑器中输入



```
#!/bin/bash
cmd /c cls
```



在命令行执行bash clear.sh则清空窗口




或者不用做任何配置，直接输入cmd /c cls可以清屏



# 2. 快捷键

使用Ctrl+L快捷键直接清屏






# 3. 添加clear命令



修改/home/.bashrc文件，在最后一行添加


`alias clear='echo -e "\E[2J"'`


则命令行直接执行clear就能清屏













