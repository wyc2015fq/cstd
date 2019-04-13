
# Linux下的lua和boost c++的搭建和安装 - 高科的专栏 - CSDN博客

2012年08月22日 10:32:49[高科](https://me.csdn.net/pbymw8iwm)阅读数：2416标签：[lua																](https://so.csdn.net/so/search/s.do?q=lua&t=blog)[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=c++&t=blog)个人分类：[lua																](https://blog.csdn.net/pbymw8iwm/article/category/1219039)[开发环境的配置																](https://blog.csdn.net/pbymw8iwm/article/category/1219848)[
							](https://blog.csdn.net/pbymw8iwm/article/category/1219039)
[
				](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=lua&t=blog)
先下载lua ，boost c++
[http://www.lua.org/versions.html\#5.2](http://www.lua.org/versions.html#5.2)
[http://www.boost.org/](http://www.boost.org/)
[http://sourceforge.net/projects/luabind/](http://sourceforge.net/projects/luabind/)
1. 安装lua
[root@localhost ~]\#tar zxvf lua-5.1.2.tar.gz  -C /usr/local
[root@localhost ~]\# cd /usr/local/
[root@localhost local]\# mv lua-5.1 lua
[root@localhost lua]\# cd lua
[root@localhost lua]\#make linux;make install;
2. 安装boost库
[root@localhost ~]\#cd boost_1_48_0/tools/build/v2
[root@localhost ~]\#./bootstrap.sh
[root@localhost ~]\#./b2 install--prefix=/usr/local/



