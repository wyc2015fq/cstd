# ubuntu 12.04安装gcc-4.4的方法 - V__KING__的专栏 - CSDN博客





2013年10月24日 14:51:36[v__king__](https://me.csdn.net/V__KING__)阅读数：2439









ubuntu 12.04中的gcc版本为4.6版本过高，不能编译通过android，就降级为gcc-4.4：





1. 查看gcc版本和g++版本


vk@vk:/usr/bin$ ls -l gcc*

lrwxrwxrwx 1 root root      7 Aug 30 19:26 gcc -> gcc-4.6

-rwxr-xr-x 1 root root 353216 Apr 16 07:37 gcc-4.6



vk@vk:/usr/bin$ ls -l g++*

lrwxrwxrwx 1 root root      7 Mar 14 04:05 g++ -> g++-4.6

-rwxr-xr-x 1 root root 357312 Apr 16 07:36 g++-4.6

2. 安装gcc和g++ 4.4版本


sudo apt-get install gcc-4.4 g++-4.4 g++-4.4-multilib


3. 修改gcc链接


sudo mv gcc gcc.bak

sudo ln -s gcc-4.4 gcc

vk@vk:/usr/bin$ ls -l gcc*

lrwxrwxrwx 1 root root      7 Sep  6 10:43 gcc -> gcc-4.4

-rwxr-xr-x 1 root root 259232 Apr 16 14:20 gcc-4.4

-rwxr-xr-x 1 root root 353216 Apr 16 07:37 gcc-4.6

lrwxrwxrwx 1 root root      7 Aug 30 19:26 gcc.bak -> gcc-4.6

4. 修改g++链接


sudo mv g++ g++.bak

sudo ln -s g++-4.4 g++

vk@vk:/usr/bin$ ls -l g++*

lrwxrwxrwx 1 root root      7 Sep  6 10:45 g++ -> g++-4.4

-rwxr-xr-x 1 root root 263328 Apr 16 14:18 g++-4.4

-rwxr-xr-x 1 root root 357312 Apr 16 07:36 g++-4.6

lrwxrwxrwx 1 root root      7 Mar 14 04:05 g++.bak -> g++-4.6


5.查看gcc和g++版本号


gcc-v


g++-v




