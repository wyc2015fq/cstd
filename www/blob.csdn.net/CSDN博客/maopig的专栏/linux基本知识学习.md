# linux基本知识学习 - maopig的专栏 - CSDN博客
2011年10月14日 23:14:23[maopig](https://me.csdn.net/maopig)阅读数：1289
LINUX黑洞
/dev/null
这是一个虚设的设备，俗称“LINUX 黑洞”，任何对/dev/null的写入都会成功，
但是数据会消失得无影无踪，没有任何反馈。所以经常把不想在屏幕
显示的信息全部送到/dev/null，在shell脚本中用得较多。
如：ls -l>/dev/null
还可以用来清空文件的内容
cat /dev/null >FileName
利用vi编辑器脚本，将脚本命名脚本功能.sh
shell脚本不是复杂的程序，它是按行解释的，脚本第一行
总是以#!/bin/sh开头，它通知系统以下的shell程序使用
系统上的Bourne Shell来解释。
第二行注释写入脚本名，第三行注释开始写入脚本功能--习惯。
以下行进入脚本正式编写
编写完后给脚本添加执行权限：
chmod u+x ScriptName 
运行脚本：ScriptName 或./ScriptName
简单脚本实例：
以下为脚本内容：
#！/bin/sh                        
#ShowHello.sh
#To show hello to somebody
echo -n "Enter Your Name"
read NAME
echo "Hello ,$NAME"
保存，推出vi编辑器
为脚本添加可执行权限：
chmod u+x ShowHello.sh
LINUX的重定向操作
重定向标准输出：
>  :将命令的执行结果输出到指定的文件中，而不是直接显示在屏幕上
>> ：将命令执行的结果追加到指定文件
重定向标准错误
2> :清空指定文件的内容，并将标准错误信息保存到该文件中
2>> :将标准错误信息追加输出到指定的文件中
重定向标准输出和标准错误
&> :将标准输出、标准错误的内容全部保存到指定的文件中，而不是直接显示在屏幕上
判断linux下面的一个进程是否在运行
在shell里面都是用
if [ ps uax|grep dhcpd|grep -v grep ]
echo  $？
为0即有dhcpd这个进程，非0表示没有
为crontab设置环境使用vi的环境变量
EDITOE=vi
export EDITOR
