# [shell] tomcat启动脚本评析 - 1. shutdown.sh - sxf_123456的博客 - CSDN博客
2017年08月04日 17:08:31[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：175

# -----------------------------------------------------------------------------
# Stop script for the CATALINA Server
# -----------------------------------------------------------------------------
# Better OS/400 detection: see Bugzilla 31132
os400=false
case "`uname`" in
OS400*) os400=true;;
esac
# resolve links - $0 may be a softlink
PRG="$0"
while [ -h "$PRG" ] ;  #判断文件是否存在，并且是一个符号连接
do
  ls=`ls -ld "$PRG"`
  # ls -a   不隐藏任何以. 开始的项目
  # ls -d   列出目录本身，不是目录里内容
  # ls -r   逆序排列
  # ls -R   递归显示子目录
  # ls -s   以块数形式显示每个文件分配的尺寸
  # ls -l   以长格式列出文件
  # ls -ld  文件名 ----> -rw-r--r--. 1 root root 41 8月 4 15:47 /test/tt.txt
  link=`expr "$ls" : '.*-> \(.*\)$'`
  #
  #
  if expr "$link" : '/.*' > /dev/null; then
    PRG="$link"
  else
    PRG=`dirname "$PRG"`/"$link"
  fi
done
PRGDIR=`dirname "$PRG"`
EXECUTABLE=catalina.sh
# Check that target executable exists
if $os400; then
  # -x will Only work on the os400 if the files are:
  # 1. owned by the user
  # 2. owned by the PRIMARY group of the user
  # this will not work if the user belongs in secondary groups
  eval
else
  if [ ! -x "$PRGDIR"/"$EXECUTABLE" ]; then
    echo "Cannot find $PRGDIR/$EXECUTABLE"
    echo "The file is absent or does not have execute permission"
    echo "This file is needed to run this program"
    exit 1
  fi
fi
exec "$PRGDIR"/"$EXECUTABLE" stop "$@"
            
