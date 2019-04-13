
# shell for循环 - 嵌入式Linux - CSDN博客

2018年01月02日 13:13:59[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：696



```python
weiqifa@ubuntu:~/qcom$ for i in $(seq 1 1 10)
> do
> echo "hello World"
> done
hello World
hello World
hello World
hello World
hello World
hello World
hello World
hello World
hello World
hello World
weiqifa@ubuntu:~/qcom$
```

输入for i in $(seq 1 1 10) in和$之间有个空格
回车后，依次输入下面的

