# [shell] tomcat启动脚本评析 - 1. startup.sh - sxf_123456的博客 - CSDN博客
2017年08月04日 17:04:29[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1921
[shell] tomcat启动脚本赏析 - 1. startup.sh
让我们一起来欣赏一下tomcat的启动脚本是怎么写的，重点关注一下其中的shell技巧
#!/bin/sh  #将用sh shell执行
PRG="$0"  #当前命令，即startup.sh
while [ -h "$PRG" ] ; do #-h同-L，用于判断文件是否是符号连接;这段代码用于找到连接背后的真实文件；由于连接的target本身可能也是个连接，所以这里用while循环找出终极连接指向的真实文件
  ls=`ls -ld "$PRG"` #为了找出本连接的target，需要执行一下ls命令；这里暂时先把ls命令的输出赋给$ls变量
  link=`expr "$ls" : '.*-> \(.*\)$'`  #通过expr的模式匹配功能，找出$ls中的target部分，并赋给$link变量
  if expr "$link" : '/.*' > /dev/null; then #判断$link是否以"/"开头，如果是，则它就是一个完整的路径；这里的> /dev/null是为了使expr的输出不打印在屏幕上PRG="$link"
  else
    PRG=`dirname "$PRG"`/"$link" #如果$link没有以"/"开头，那它就是一个相对路径；也就是说target的完整路径 = 符号连接的目录/相对路径
  fi
done
 
PRGDIR=`dirname "$PRG"` #得出真实命令文件所处的目录
EXECUTABLE=catalina.sh 
  if [ ! -x "$PRGDIR"/"$EXECUTABLE" ]; then #判断一下catalina.sh是否存在且可执行
    echo "Cannot find $PRGDIR/$EXECUTABLE"
    echo "The file is absent or does not have execute permission"
    echo "This file is needed to run this program"
    exit 1
  fi
exec "$PRGDIR"/"$EXECUTABLE" jpda run "$@" #执行catalina.sh, 提供的参数包括jpda, run 和startup.sh 收到的参数
