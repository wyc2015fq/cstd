# shell一段脚本的一点经验（实时文件流读写） - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月01日 10:01:12[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9545








1、脚本内容：将命令的结果实时写入文件，再从文件中搜索指定字符串，并返回含有该字符串的行数。




2、正确的脚本：



```
#!/bin/bash 
#find the lost glabelingtopo and restart 
#write by Jason.F
#2016.07.29

#assert 12 processes
jps > /home/topo.log
declare -i jpsnum=0
#var=`find /home/ -type f -name "topo.log" | xargs grep "name"`
jpsnum=$(grep -c name /home/topo.log)
if [ $jpsnum -eq 12 ]; then 
rm -rf /home/topo.log
exit 0
fi
```


3、错误的脚本：



```
#!/bin/bash 
#find the lost glabelingtopo and restart 
#write by Jason.F
#2016.07.29

#assert 12 processes
jps > /home/topo.log &
declare -i jpsnum=0
#var=`find /home/ -type f -name "topo.log" | xargs grep "name"`
jpsnum=$(grep -c name /home/topo.log)
if [ $jpsnum -eq 12 ]; then 
rm -rf /home/topo.log
exit 0
fi
```




4、经验：

两个脚本的区别在于jps > /home/topo.log ，错误的脚本加了&，导致错误的脚本执行中，jpsnum一直为零。猜测是因为加了&，变成后台异步将命令结果写入到文件，而当前任务从文件中搜索的实际是一份还未关闭写入的空文件，所以返回是零。去掉&，就是完成文件写入并保存文件后，才开始在文件中搜索，而错误脚本确实未保存文件就搜索，自然搜索不到内容。








