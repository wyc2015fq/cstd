# python 常用操作 - sxf_123456的博客 - CSDN博客
2018年08月24日 14:46:58[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：83
1、格式化常用方法
```
code = 1
name = 2
path_1  = f'{code}_{name}'
path_2  = '{0}_{1}'.format(code,name)
```
`2、`
"""
获取路径前半部分和后半部分
如/root/test/path/stb
pref = /root/test/path
suff = stb
"""
import os
def _get_gep(s):
    if isinstance(s, bytes):
        return b'/'
    else:
        return '/'
def split(path):
    path = os.fspath(path)  # 判断路径是否是str和bytes类型，否抛出异常
    sep = _get_gep(path)  # 获取分隔符'/'
    i = path.rfind(sep) + 1  # rfind函数是从右往左查找，但返回的位置是从左到右找到的位置
    head, tail = path[:i], path[i:]
    if head and head != sep * len(head):
        head = str(head).rstrip(sep) #去掉最右边'/'分隔符
    return head, tail
if __name__ == "__main__":
    path = "/root/test/path/stb"
    head, tail = split(path)
    print(head, tail)
