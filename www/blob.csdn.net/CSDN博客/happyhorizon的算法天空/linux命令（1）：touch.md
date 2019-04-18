# linux命令（1）：touch - happyhorizon的算法天空 - CSDN博客
2015年12月23日 16:20:37[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：397
**touch 命令**
**功能说明**：改变文件或目录时间，包括存取时间和更改时间。
**语 法**： 
补充说明：使用touch指令可更改文件或目录的日期时间
**最常用用法**：touch fileA 
- 如果fileA存在，使用touch指令可更改这个文件或目录的日期时间，包括存取时间和更改时间； 
- 如果fileA不存在，touch指令会在当前目录下新建一个空白文件fileA。
**参 数**： 
-a或–time=atime或–time=access或–time=use 只更改存取时间。 
-c或–no-create 不建立任何文件。 
-d<时间日期> 使用指定的日期时间，而非现在的时间。 
-f 此参数将忽略不予处理，仅负责解决BSD版本touch指令的兼容性问题。 
-m或–time=mtime或–time=modify 只更改变动时间。 
-r<参考文件或目录> 把指定文件或目录的日期时间，统统设成和参考文件或目录的日期时间相同。 
-t<日期时间> 使用指定的日期时间，而非现在的时间。 
–help 在线帮助。 
–version 显示版本信息。
