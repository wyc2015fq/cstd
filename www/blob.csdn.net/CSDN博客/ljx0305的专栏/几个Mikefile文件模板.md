# 几个Mikefile文件模板  - ljx0305的专栏 - CSDN博客
2009年04月11日 16:52:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1418
```
SUBDIRS = ./msg_mgr ./lock_class ./msg_mgr_testcase ./socket_class
all:
    for dir in $(SUBDIRS); do /
        $(MAKE) -C $$dir; /
    done
clean:
    for dir in $(SUBDIRS); do /
        $(MAKE) -C $$dir clean; /
    done
```
值得注意的地方：
$@ 目标文件,$^    所有的依赖文件,$< 第一个依赖文件.
引用:http://blog.chinaunix.net/u/16292/showart_1799565.html
