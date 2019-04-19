# MPQ文件系统优化 - 逍遥剑客 - CSDN博客
2009年01月04日 23:17:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2241
上次用解析字符串的方式来列举目录和文件, 速度太慢
尤其像Common.mpq这样一个MPQ的listfile就几兆的情况下, 速度简直不能忍受
再想想ZipFileSystem中的FileEntry和DirEntry, 我想它们是用来缓存文件目录树的吧
在内存中进行查找肯定比读文件快, 更不用说去解析字符串了
还是加入进去好了
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090104/mpqfilesystem1.jpg)
与Zip不同的是, 我没有把文件一次性读入内存. 因为mpq本身就支持Seek, 不需要读到内存去模拟. 如果想映射到内存进行快速访问, 可以用Map()和Unmap().
呵呵, Visual Paradigm画图就是爽, 个人自己用可以申请一个免费的授权
以后要慢慢学着自己设计东西了, 这也算一个瓶颈吧, 过去就好了^_^
N3的IO模块默认是跟主线程不在同一个线程里的, 这点比较头疼.....看来能只做一下简单的测试了, 或者自己在主线程里也开一个IoServer, 唉~~路漫漫
