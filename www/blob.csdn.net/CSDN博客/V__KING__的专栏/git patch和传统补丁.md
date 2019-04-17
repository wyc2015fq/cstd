# git patch和传统补丁 - V__KING__的专栏 - CSDN博客





2017年03月28日 15:15:14[v__king__](https://me.csdn.net/V__KING__)阅读数：901








参考：[http://blog.csdn.net/sunnylgz/article/details/7660638](http://blog.csdn.net/sunnylgz/article/details/7660638)

参考：[http://www.cnblogs.com/domainfei/articles/2433504.html](http://www.cnblogs.com/domainfei/articles/2433504.html)
[http://blog.sina.com.cn/s/blog_5372b1a301015y0n.html](http://blog.sina.com.cn/s/blog_5372b1a301015y0n.html)

英文原文地址：[http://www.pizzhacks.com/bugdrome/2011/10/deal-with-git-am-failures/](http://www.pizzhacks.com/bugdrome/2011/10/deal-with-git-am-failures/)
## 一. 传统使用patch
- diff dirold dir2new > 0001.PATCH  或者 diff file1 file2>0001.PATCH
- patch -p1 < 0001.PATCH 

patch命令里面的层数(-p0?-p1?) 

参数-p来指定从第几层开始比较。比如有一个patch文件的补丁头是这样的： 

代码:`--- old/modules/pcitableMon Sep 27 11:03:56 1999 +++ new/modules/pcitableTue Dec 19 20:05:41 2000`
```
如果使用参数-p0，就表示从当前目录，找一个叫作new的目录，在它下面找一个叫modules的目录，再在它下面找一个叫pcitableMon的目录。
如果使用参数-p1,就表示忽略第一层，从当前目录找一个叫modules的目录，在它下面找一个叫modules的目录。这样会忽略掉补丁头提到的new目录。依此类推。
```

## 二. git生成patch

使用git format-patch -M master 来生成 

0001.xxx.patch 

0002.xxx.patch 

…..
### 1. 用git am打补丁

比如，一个典型的git am失败，可能是这样的： 

$ git am *.PATCH

```
Applying: PACTH DESCRIPTION
error: patch failed: file.c:137
error: file.c: patch does not apply
error: patch failed: Makefile:24
error: libavfilter/Makefile: patch does not apply
Patch failed at 0001 PATCH DESCRIPTION
When you have resolved this problem run "git am --resolved".
If you would prefer to skip this patch, instead run "git am --skip".
To restore the original branch and stop patching run "git am --abort".
```

正如你所见，如果冲突发生，git只是输出上述信息，然后就停下来。一个小冲突会导致整个patch都不会被集成。

### 下面的方法是不建议在git中使用，因为git有方法

处理这种问题的最简单方法是先使用 `git am --abort`，然后手动的添加此patch, `patch -p1 < PATCH`，手动解决掉代码冲突，最后使用 `git commit -a` 提交代码。但是这样做有个问题就是你会失去PATCH中原本包含的commit信息（比如From，Date，Subject，Signed-off-by等）。应该有一种更聪明的方法。

在`.git/rebase-apply`目录下，存放着相应的补丁文件，名字是“0001” （在更新的git版本中，存放补丁文件的目录名有所改变，这里使用的git版本是 1.7.4.1）。

### 2. git am失败情况下，git apply –reject打补丁

事实上，你可以使用 `git apply` 命令打patch（git apply 是git中的patch命令）。如同使用 patch -p1 命令时一样，然后手动解决代码冲突（检视生成的 .rej 文件，与冲突文件比较，修改冲突内容，并最终把文件加入到index中）：

```
$ git apply 0001.xxx.PATCH --reject
$ edit edit edit
（译注：根据.rej文件手动解决所有冲突；无冲突就不会生产.rej，而是产生结果是："你修改了的某些文件"，需要你去git add）
$ git add FIXED_FILES  (或modified_files或added_files)
$ git am --continue   （或git am --resolved)
```

就这么简单！

想多一些解释，好吧。git am 并不改变index，你需要使用 git apply –reject 打patch（保存在 .git/rebase-apply），手动解决代码冲突，（译注：使用 git status 列出所有涉及文件），把所有文件（不仅仅是引起冲突的文件）添加到（git add）index，最后告诉 git am 你已经解决（–resolved）了问题。

这样做的好处: 

1. 是你不需要重新编辑commit信息。 

2. 而且，如果你正在打的是一系列patch（就是说你在打的是多个patch，比如 git am *.patch）你不需要使用 `git am --abort`，然后又 git am。
## 使用总结

```
//使用补丁
git am *.patch

//如果上面的不成功就往下走，成功了，就不用了apply *.patch --reject。
git apply 0001-modified.patch --reject

git add FIXED_FILES

git am --continue     （或git am --resolved)
```







