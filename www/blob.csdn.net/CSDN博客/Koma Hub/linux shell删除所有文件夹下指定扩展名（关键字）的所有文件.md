# linux shell删除所有文件夹下指定扩展名（关键字）的所有文件 - Koma Hub - CSDN博客
2018年01月16日 14:32:19[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1200
打开文件
`gedit clean.sh&`
敲入文本
```
#!/bin/bash
# Rong Tao 2018.1.16
filename=("*.class" "*.pyc" "*.h.gch" "*.o" "*~")
for name in ${filename[@]}; do
    echo $name
    find  .  -name  $name  -type  f  -print  -exec  rm  -rf  {} \;
done
if [ $# -eq 0 ];then
    echo $*
    pwd
fi
```
修改权限
`chmod 777 clean.sh`
执行
`./clean.sh`
