# 借助gdb实现pstack - DoubleLi - 博客园






pstack.sh:



#! /bin/sh

if [ -z $1 ]

then

    echo "gdb script for print stack"

    echo "usage: $0 pid"

    exit

fi



gdb --batch --quiet -x cmds -p $1







cmds:



echo /nProcess:/n

info proc



echo /nStack:

thread apply all backtrace



detach

quit









