# Linux下为文件增加列的shell脚本 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月26日 17:48:16[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5222








场景：linux有份文件，需要增加一列序号来标记，通过shell脚本来实现。

步骤：

1）创建一份test.txt文件，内含一列，如下：



```
bash-4.1$ cat test.txt
a
b
c
d
e
```


2）创建脚本row_id.sh，如下：



```
bash-4.1$ cat row_id.sh
#!/bin/sh
lines=$1
for ((i=1;i<=$lines;i++))
do
        echo $i>>test_id.txt
done
```


3）执行脚本：sh row_id.sh 5

生成5行序号，如下：



```
bash-4.1$ cat test_id.txt
1
2
3
4
5
```






4）合并两份文件，执行：


`paste test_id.txt test.txt > test_and_id.txt`

5）查看最终结果：



```
bash-4.1$ cat test_and_id.txt
1       a
2       b
3       c
4       d
5       e
```





