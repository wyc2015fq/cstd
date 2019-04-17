# 【shell】用shell将hdfs上文件夹内的文件批量改名 - zkq_1986的博客 - CSDN博客





2018年11月07日 15:58:49[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：454








```bash
#!/bin/sh
ls=`hdfs dfs -ls /hdfsOutput/data/sysgroup/d1/*/*/*`
dir="/hdfsOutput/data/sysgroup/d1"
for i in ${ls[@]}
do
        if [[ $i == *$dir* ]]
        then
                d=".done"
                if [[ $i == *$d ]]
                then
                        echo $i
                        new_name=`echo ${i:0:${#i}-5}`
                        echo $new_name
                        hdfs dfs -mv $i $new_name
                fi
        fi
done
```





