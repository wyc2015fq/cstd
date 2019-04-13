
# ubuntu系统使用opencv，make通过，但运行时报错 - 郭云飞的专栏 - CSDN博客


2017年08月07日 10:55:04[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：132标签：[opencv																](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)[ubuntu																](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)个人分类：[opencv																](https://blog.csdn.net/guoyunfei20/article/category/7082070)


错误描述大致如下：

```python
./guo-dbg.pio: error while loading shared libraries: libopencv_features2d.so.3.3: cannot open shared object file: No such file or directory
```
解决：
没有设置环境变量

```python
// 一定要加sudo
guo@ubuntu:~$ sudo vim /etc/ld.so.conf
```
在打开的文件中添加opencv的lib库所在路径，一般是：
```python
/usr/local/lib
```
保存退出然后执行：

```python
guo@ubuntu:~$ sudo ldconfig
```





