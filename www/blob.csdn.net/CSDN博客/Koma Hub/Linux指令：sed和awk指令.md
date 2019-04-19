# Linux指令：sed和awk指令 - Koma Hub - CSDN博客
2018年05月05日 13:58:45[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：78
删除文件中每行的前6个字符：
`$sed -i.bak 's/^.\{6\}//g' urfile`
删除文件每行开头的空格：
```
$awk '{sub(/^[ \t]+/,"");print $0}' filename #方法1
$sed -i 's/^[ ]*//g'  filename #方法2
```
查看最近5条登录用户和ip地址：
```
$ last -n 5|awk '{print $1"\t"$3}'
Toa	:0
Toa	:0
reboot	boot
Toa	:0
Toa	:0
wtmp	Wed
```
查阅第三栏小于 10 以下的数据：
```
$ cat /etc/passwd|awk '{FS=":"} $3<10 {print $0}'
root:x:0:0:root:/root:/bin/bash
bin:x:1:1:bin:/bin:/sbin/nologin
daemon:x:2:2:daemon:/sbin:/sbin/nologin
adm:x:3:4:adm:/var/adm:/sbin/nologin
lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
sync:x:5:0:sync:/sbin:/bin/sync
shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown
halt:x:7:0:halt:/sbin:/sbin/halt
mail:x:8:12:mail:/var/spool/mail:/sbin/nologin
```
统计数据：
```
$ cat score.txt 
Name    math    english   gym
Rong    98      100       99
Tao     99      100       99
RT      97      100       99
$ cat score.txt  |awk 'NR==1{printf "%10s %10s %10s %10s %10s\n",$1,$2,$3,$4,"Total"} \
NR>=2{total=$2+$3+$4
printf "%10s %10d %10d %10d %10.2f\n",$1,$2,$3,$4,total score}'
      Name       math    english        gym      Total
      Rong         98        100         99     297.00
       Tao         99        100         99     298.00
        RT         97        100         99     296.00
```
更多指令：[http://man.linuxde.net/sed](http://man.linuxde.net/sed)
