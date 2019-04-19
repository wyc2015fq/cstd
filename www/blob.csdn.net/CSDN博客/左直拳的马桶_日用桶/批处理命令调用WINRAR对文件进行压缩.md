# 批处理命令调用WINRAR对文件进行压缩 - 左直拳的马桶_日用桶 - CSDN博客
2010年12月09日 18:35:00[左直拳](https://me.csdn.net/leftfist)阅读数：4606
批处理命令(rar.bat)：
c:
cd/
cd C:/Program Files/WinRAR
winrar a-ep1 -o+ -ppassword -inul -r -ibck C:/RarFile E:/BigFile
其中，a是压缩命令，-ep1 -o+ -p -inul -r -ibck是参数
ep1:排除基准文件夹，不然压缩包会包含待压缩文件夹所在的完整路径
o+:覆盖已经存在的文件
p:密码，后面红色的部分就是密码，紧贴这个参数P，看起来有点怪
inul:禁止出错信息
r:连同子文件夹操作
ibck:后台模式运行
C:\RarFile 压缩文件
E:\BigFile 等待压缩的源文件
更多帮助信息，可以打开WINRAR，然后看它的帮助。
将以上批处理命令写到一个批处理文件里，然后用任务计划来定期运行这个批处理文件，就可以定期压缩某些文件鸟。比如说，夜深人静的时候，机器自动压缩庞大的数据库备份文件。
