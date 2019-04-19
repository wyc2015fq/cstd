# 我常用的shell命令 - Arthur的随笔 - CSDN博客
2011年09月15日 17:09:10[largetalk](https://me.csdn.net/largetalk)阅读数：465标签：[shell																[function																[include																[audio																[system																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=audio&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[linux](https://blog.csdn.net/largetalk/article/category/727912)
                
我常用的shell命令
tar:
tar jxvf filename.tar.bz2
tar xvf filename.tar
tar cvf filename.tar file
tar cjvf filename.tar.bz2 file
find . -type f #只找出文件 f：文件， d:目录，l: link
wc:
ls -l | wc -l
tr:
ls -l | tr 'a-z' 'A-Z'
grep:
ls -l | grep ^d
ls -l | grep -n ^d
ls -l | grep -v ^d
echo "theaaaa
the
bbbbthe | grep '\<the\>'
echo "theaaaa
the
bbbbthe | grep -A1 -B1 '\<the\>'
-n # 显示行号
-v #不匹配这个模式的
-A #后面多少行
-B #前面多少行
ack:
ack pattern #匹配当前目录下文件内容
ack --perl pattern #只匹配perl文件, 还支持python, shell, ruby, php等
ack '(Mr|Mr?s)\. (Smith|Jones)' --output='$&' #格式输出
ack 大部分参数和grep一样
awk:
ls -l | awk '{print $1}'
echo "a|b|c" | awk -F\| '{print $1}'
sed:
ls -l | sed 's/largetalk/root/g'
seq 6 | sed '1!G;h;$!d'
linux命令之declare
declare
typeset #the alias name of declare
declare -p # show all var
declare -p var #show the var
declare var=value # the same with var=value
declare -i var #the var is integer
declare -r var #the var is read only
declare -a var #the var is array
declare -f # show all function include name and body
declare -F # shwo all function name
declare -f func #show the func
declare -x var # set the var into env
declare -x var=value # set env
linux命令之perror
perror errno # show the mean of errno, eg. perror 1 /* Operation not permitted */
pstree #查看进程关系
其他如df, top, free, uptime等就不说了
shell就是把很多小命令组合成一个大的工作，有时候还是很方便的，虽然很难看懂，奉送一个我写的shell，不算太复杂，但太长太难懂
ssh "$serena_user"@"$serena_host" "find /var/www/ellis/media/upload -name \*.mp3 | xargs file | grep -v Audio | awk -F: '{print \$1, \$2}' | awk -F, '{print \$1, \$4, \$5}' | awk '{if (\$4!=32 || \$6     != 22.05) print \$1, \$4, \$5, \$6, \$7}' | awk '{content=\$1;
   print \$0; system(\"stat -c %s \"content)}' " > $unsuitmp3
            
