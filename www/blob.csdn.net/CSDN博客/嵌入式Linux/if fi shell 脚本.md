
# if fi shell 脚本 - 嵌入式Linux - CSDN博客

2014年12月22日 10:55:08[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：746



```python
weiqifa@ubuntu:~/logcat$ source test.sh 
please input the packageName
weiqifa@ubuntu:~/logcat$ source test.sh aa
input ok
weiqifa@ubuntu:~/logcat$ cat test.sh 
#!/bin/bash
if [ -z "$1" ];then
echo "please input the packageName"
else
echo "input ok"
fi
weiqifa@ubuntu:~/logcat$
```


1、开头还是要有\#!/bin/bash
2、if 和[之间要有一个空格，否则会提示 {syntax error near unexpected token `then'}
3、[ -z "$1" ]  $1是输入的第一个参数，-z跟左边的[也要有空格，不然判断结果都是跑else了。

