# git代码统计 - baidu_33570760的博客 - CSDN博客
2019年03月04日 17:50:36[carman_风](https://me.csdn.net/baidu_33570760)阅读数：42
个人分类：[软件安装与使用](https://blog.csdn.net/baidu_33570760/article/category/6940159)
### 命令行方式
- 
查看git上的个人代码量：
> 
git log --author=“username” --pretty=tformat: --numstat | awk ‘{ add += $1; subs += $2; loc += $1 - $2 } END { printf “added lines: %s, removed lines: %s, total lines: %s\n”, add, subs, loc }’ -
结果示例：(记得修改 username)
> 
added lines: 120745, removed lines: 71738, total lines: 49007
- 
统计每个人增删行数
> 
git log --format=’%aN’ | sort -u | while read name; do echo -en “$KaTeX parse error: Expected 'EOF', got '\t' at position 5: name\̲t̲"; git log --au…$name” --pretty=tformat: --numstat | awk ‘{ add += $1; subs += $2; loc += $1 - $2 } END { printf “added lines: %s, removed lines: %s, total lines: %s\n”, add, subs, loc }’ -; done
结果示例
> 
Max-laptop    added lines: 1192, removed lines: 748, total lines: 444
chengshuai    added lines: 120745, removed lines: 71738, total lines: 49007
cisen    added lines: 3248, removed lines: 1719, total lines: 1529
max-h    added lines: 1002, removed lines: 473, total lines: 529
max-l    added lines: 2440, removed lines: 617, total lines: 1823
mw    added lines: 148721, removed lines: 6709, total lines: 142012
spider    added lines: 2799, removed lines: 1053, total lines: 1746
thy    added lines: 34616, removed lines: 13368, total lines: 21248
wmao    added lines: 12, removed lines: 8, total lines: 4
xrl    added lines: 10292, removed lines: 6024, total lines: 4268
yunfei.huang    added lines: 427, removed lines: 10, total lines: 417
³Ÿö    added lines: 5, removed lines: 3, total lines: 2
- 
查看仓库提交者排名前 5
> 
git log --pretty=’%aN’ | sort | uniq -c | sort -k1 -n -r | head -n 5
- 
贡献值统计
> 
git log --pretty=’%aN’ | sort -u | wc -l
- 
提交数统计
> 
git log --oneline | wc -l
- 
添加或修改的代码行数
> 
git log --stat|perl -ne ‘END { print $c } $c += $1 if /(\d+) insertions/’
### 参考文章
- [git代码行统计命令集](http://www.jianshu.com/p/8fd14064c201)
- [统计本地Git仓库中不同贡献者的代码行数的一些方法](http://www.94joy.com/archives/115#comment-319)
- [使用Git工具统计代码](http://blog.cyeam.com/kaleidoscope/2015/01/17/gitstats)
- [git代码统计](https://segmentfault.com/a/1190000008542123)
