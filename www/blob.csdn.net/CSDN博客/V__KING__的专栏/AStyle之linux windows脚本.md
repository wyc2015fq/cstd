# AStyle之linux/windows脚本 - V__KING__的专栏 - CSDN博客





2017年07月21日 10:01:58[v__king__](https://me.csdn.net/V__KING__)阅读数：454








资源下载：[http://download.csdn.net/detail/v__king__/9814151](http://download.csdn.net/detail/v__king__/9814151)

### linux如何将AStyle集成到你的c工程
- 新建一个AStyle的目录
- 把AStyle.exe和test.sh脚本放到工程AStyle目录中,加入Makefile

### shell脚本

```bash
#!/bin/bash
find ./ -name "*.c" | xargs AStyle -c --style=ansi
find ./ -name "*.h" | xargs AStyle -c --style=ansi
```

### DOS的脚本

```
rem 将当前目录和所有子目录的*.c,*.h文件格式化
rem 注意历史原因dos不识别utf8编码的中文
rem 编辑dos脚本需要保存为ansi编码,
rem 可以用其他编辑器编辑，在复制到记事本中
rem add by VkingMan@20170721

@echo off

for /R "../" %%s in (,*.h) do AStyle.exe --style=ansi %%s

set /p var=删除所有的备份文件*.orig?(Y/N):

if %var% equ Y (
    echo "rm all the *.orig"
    for /R "../" %%s in (*.orig) do del %%s
)
if %var% equ y (
    echo "rm all the *.orig"
    for /R "../" %%s in (*.orig) do del %%s
)
pause
```

资源下载：[http://download.csdn.net/detail/v__king__/9814151](http://download.csdn.net/detail/v__king__/9814151)



