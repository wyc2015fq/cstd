# C,C++代码格式化 - artistic style - V__KING__的专栏 - CSDN博客





2017年04月10日 15:36:09[v__king__](https://me.csdn.net/V__KING__)阅读数：1174








windows下，将当前目录和所有子目录的*.cpp;*.c;*.h文件格式化

```
@echo off 
for /R "./" %%s in (*.c,*.h) do AStyle.exe --style=ansi %%s
pause
```

linux下，将当前目录及子目录的*.cpp;*.c;*.h文件格式化
`find ./ -name "*.c" | xargs AStyle -c --style=ansi`
命令工具的用法： 

astyle -c –style=ansi abc.c



