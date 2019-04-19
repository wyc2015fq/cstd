# couchbase编译指南 - 三少GG - CSDN博客
2014年07月22日 11:31:21[三少GG](https://me.csdn.net/scut1135)阅读数：1016
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

环境准备：
We’re going to update the default software of our Linux system and enrich its toolbase with the following items:
- Git (version control tool, as mentioned above)
- Repo (complimentary repository management tool, born off Android project)
- Python (an object oriented programming language’s and its C-Based interpreter)
- cURL (a library for downloads and web requests)
1. 下载repo
从官网找到 [http://www.couchbase.com/wiki/display/couchbase/Architecture+and+Source+Repositories](http://www.couchbase.com/wiki/display/couchbase/Architecture+and+Source+Repositories)
## Installing Repo
Repo is a tool that makes it easier to work with Git in the context of Android. For more information about Repo, see the [Developing](http://source.android.com/source/developing.html)section.
To install Repo:
- 
Make sure you have a bin/ directory in your home directory and that it is included in your path:
```
$ mkdir ~/bin
$ PATH=~/bin:$PATH
```
- 
Download the Repo tool and ensure that it is executable:
$ curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
$ chmod a+x ~/bin/repo
**（repo版本非常重要！！！其余别处的下载不能得到代码）**
```
repo init -u git://github.com/couchbase/manifest.git -m rel-2.2.0.xml
repo sync
```
**编译安装：**
**[https://github.com/couchbase/manifest](https://github.com/couchbase/manifest)  （安装方法及版本介绍）**
**或依据文章：**
**[https://www.digitalocean.com/community/tutorials/how-to-install-couchbase-from-source-with-git-and-make-on-a-debian-7-vps](https://www.digitalocean.com/community/tutorials/how-to-install-couchbase-from-source-with-git-and-make-on-a-debian-7-vps)**
