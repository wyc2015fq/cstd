# 【Python】python之字符串格式化（format） - zkq_1986的博客 - CSDN博客





2018年10月15日 10:39:22[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：57








```python
line='深度'
new_line="""{}"title":","doc":"{}"{}\n""".format('{',line,'}')
print(new_line)
```

输出：

    {"title":","doc":"深度"}



