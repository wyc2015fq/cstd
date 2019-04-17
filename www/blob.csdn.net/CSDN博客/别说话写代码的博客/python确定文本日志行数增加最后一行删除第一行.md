# python确定文本日志行数增加最后一行删除第一行 - 别说话写代码的博客 - CSDN博客





2018年08月27日 17:28:56[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：377








这里采取一个最简单暴力的方法，确定文本行数，更新一行删除一行，如果不到指定文本行数，就 持续增加。

```python
def delline_addline(filename,max_line,content):
    with open(filename, 'r') as old_file:
        if(max_line<=len(old_file.readlines())):
            with open(filename, 'r+') as new_file:
                old_file.seek(0, 0)
                seek_point = old_file.tell()
                new_file.seek(seek_point, 0)
                old_file.readline()
                next_line = old_file.readline()
                while next_line:
                    new_file.write(next_line)
                    next_line = old_file.readline()
                new_file.truncate()
                new_file.write(content+'\n')
                new_file.close()
delline_addline("file.txt",10,"xinxiejinlai")
```





