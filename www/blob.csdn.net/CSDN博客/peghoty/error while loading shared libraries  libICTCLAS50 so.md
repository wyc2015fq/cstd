
# error while loading shared libraries: libICTCLAS50.so - peghoty - CSDN博客


2014年03月05日 10:34:02[皮果提](https://me.csdn.net/peghoty)阅读数：1901标签：[库链接																](https://so.csdn.net/so/search/s.do?q=库链接&t=blog)[error																](https://so.csdn.net/so/search/s.do?q=error&t=blog)[ICTCLAS																](https://so.csdn.net/so/search/s.do?q=ICTCLAS&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=error&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=库链接&t=blog)个人分类：[编程知识																](https://blog.csdn.net/peghoty/article/category/1451023)
[
																								](https://so.csdn.net/so/search/s.do?q=库链接&t=blog)


测试汉语分词系统 ICTCLAS，编译链接没问题，运行程序时报错：
error while loading shared libraries: libICTCLAS50.so: cannot open shared object file: No such file or directory
网上找到的解决方案如下：
**第一步**：在终端输入
locate libICTCLAS50.so
确认系统里是否有该文件，大部分情况下该文件是存在的。
**第二步**：将自己的库所在路径添加到 /etc/ld.so.conf 文件中。
但这个文件中的内容一般都只包含一行 “include /etc/ld.so.conf.d/*.conf”，因此，最好在目录 /etc/ld.so.conf.d/ 下新建一个文件， 如 “mylib.conf”，再将自己的库所在路径添加到这个 conf 文件中。
**第三步**：以 root 身份运行
/sbin/ldconfig -v
利用这个方案，成功解决了上述报错问题。

