# mac port 清理 - =朝晖= - 博客园
# [mac port 清理](https://www.cnblogs.com/dhcn/p/7442970.html)
http://popozhu.github.io/2014/10/27/mac-port-%E6%B8%85%E7%90%86/
使用 mac port 来安装一些软件，时间久了后（也有两年多），更新系统，更新软件，旧版本的软件(包)仍然存在于硬盘上。趁着这次更新 yosemite， 升级 mac port 后，顺道做了下清理，
- 
升级 mac port 自身
```
sudo port selfupdate
```
更新的是 port 自身，以及仓库文件索引。
- 
更新后，先不急着更新其他软件。先找出那些是不需要可以删除的软件或依赖。
```
port installed inactive
```
列出所有「非激活」态的软件(包)，我将删除这些软件(包)。
- 
删除没有用到的软件(包)
```
sudo port uninstall inactive
```
- 
查看是否有不需要的依赖
```
port echo leaves
```
`leaves` 这个词很形象，孤立的叶子节点，很好记。
- 
删除不需要的依赖
```
sudo port uninstall leaves
```
一般需要删除多次，因为有些依赖是循环依赖于前一个不需要的依赖。
直到报错说没有匹配的结果为止。
```
popozhu ~$ sudo port uninstall leaves
 Error: No ports matched the given expression
```
- 
这时再查找哪些软件(包)已经有新版本
```
port outdated
```
- 
建议单独升级需要的软件(包)，比如`A B C`
```
sudo port upgrade A B C...
```
或者直接升级所有旧软件(包)（不推荐）
```
sudo port upgrade outdated
```
最后清理了1.4G的空间出来，好评。
port 会在本地编译代码，期间会导致 cpu 100%，机器发热大增，升级的内容越多，所需时间也越多。就不要在临下班时做这个事情啦。
现在较少用 port 来安装新的软件，若有时间，还是更倾向于下载源码tar包，自己编译安装，因为 port 安装时顺带安装的依赖，有时真的太多了，也未必都是必须的，反而花费更多时间。

