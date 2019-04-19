# Python 代码跳转 - 宇宙浪子的专栏 - CSDN博客
2014年09月23日 10:17:15[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：7811标签：[Python																[插件																[ptags](https://so.csdn.net/so/search/s.do?q=ptags&t=blog)](https://so.csdn.net/so/search/s.do?q=插件&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[Python																[工具](https://blog.csdn.net/bluehawksky/article/category/2582971)](https://blog.csdn.net/bluehawksky/article/category/2542577)

用vim作python开发，在看源码的时候，发现无法跳转，发现ctags是专门为c语言准备的。
很多人不会注意到[python的发行包](http://www.python.org/download/)下Tool/scripts的文件夹。里面的ptags.py可以用来生成vim需要的tags(eptags.py
 for emacs)
脚本的目录是python源码下的Tool/script／ptags.py。 如果不想去找，可以直接copy一份，[http://svn.python.org/projects/python/trunk/Tools/scripts/ptags.py](http://svn.python.org/projects/python/trunk/Tools/scripts/ptags.py)，放在你方便的位置，然后使用ptags.py生成tag。
为当前目录生成tag文件 :  ptags.py *.py
为整个项目生成tag文件：find  . -name \*.py -print | xargs ptags.py
然后就有tag了。在vim的配置文件中导入tag文件的目录就可以了。
比如在.vimrc 中添加：
set tags+=tags
set autochdir
[?](http://bigzhu.is-programmer.com/posts/20676.html#)
|1|`/home/bigzhu/``.``local``/share/Trash/files/Python-2``.5.2``/Tools/scripts/ptags````.py  *.py```|
同个文件再跳就是这样的:
[](http://bigzhu.is-programmer.com/user_files/bigzhu/Image/2010-08-20-110903_442x105_scrot.png)
源码：
转自http://www.oschina.net/code/explore/Python-3.1.3/Tools/scripts/ptags.py#! /usr/bin/env python
# ptags
#
# Create a tags file for Python programs, usable with vi.
# Tagged are:
# - functions (even inside other defs or classes)
# - classes
# - filenames
# Warns about files it cannot open.
# No warnings about duplicate tags.
import sys, re, os
tags = []    # Modified global variable!
def main():
    args = sys.argv[1:]
    for filename in args:
        treat_file(filename)
    if tags:
        fp = open('tags', 'w')
        tags.sort()
        for s in tags: fp.write(s)
expr = '^[ \t]*(def|class)[ \t]+([a-zA-Z0-9_]+)[ \t]*[:\(]'
matcher = re.compile(expr)
def treat_file(filename):
    try:
        fp = open(filename, 'r')
    except:
        sys.stderr.write('Cannot open %s\n' % filename)
        return
    base = os.path.basename(filename)
    if base[-3:] == '.py':
        base = base[:-3]
    s = base + '\t' + filename + '\t' + '1\n'
    tags.append(s)
    while 1:
        line = fp.readline()
        if not line:
            break
        m = matcher.match(line)
        if m:
            content = m.group(0)
            name = m.group(2)
            s = name + '\t' + filename + '\t/^' + content + '/\n'
            tags.append(s)
if __name__ == '__main__':
    main()

