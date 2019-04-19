# Git版本回退 - _天枢 - 博客园
## [Git版本回退](https://www.cnblogs.com/yhleng/p/8241884.html)
2018-01-08 11:32 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8241884)
Git init命令
```
Git init  初始化当前目录为仓库
```
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180108110220629-1451235819.png)
Git两步曲：
```
1、Git add "tg.txt"  增加tg.txt文件到仓库  或者叫**增加到暂存区**2、Git commit -m "var1" 提交到仓库  将**暂存区的内容提交到当前仓库**
```
仓库中添加tg.txt文件
```
**版本1内容：**
test var1
```
```
git add tg.txt
```
没有任何提示，没有消息就是好消息哈~~~
```
git commit -m "var2"
[master (root-commit) 698fbac] tg.txt
 1 file changed, 1 insertion(+)
 create mode 100644 tg.txt
```
```
**版本2内容：**
test var2
```
```
git add tg.txt
```
没有任何提示，没有消息就是好消息哈~~~
```
git commit -m "var3"
[master (root-commit) 698fbac] tg.txt
 1 file changed, 1 insertion(+)
 create mode 100644 tg.txt
```
查看已提交版本：
我们不可能，记住每次提交的版本，不用担心，我们可以通过**Git log**命令查看
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180108110934457-6194144.png)
我们可以看到，提交版本情况，由近到远
```
，上图中的commit 698fbac6b52a0000080b600f9c8c358a72b074fa 这样的串，就是Git的版本号
```
**版本回退：**
首先，Git必须知道当前版本是哪个版本
在Git中，用`HEAD`表示当前版本，
上一个版本就是`HEAD^`
上上一个版本就是`HEAD^^`
当然往上100个版本写100个`^`比较容易数不过来，
所以写成`HEAD~100`。
现在，回退到上一版本，使用**`git reset`**命令：
--hard HEAD^ 参数:
--hard:
HEAD^ 回退到上一版本
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180108111715301-1746546231.png)
查看一下，内容已经回退到了上个版本：
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180108112210957-1020775894.png)
接下来再查看一下，当前版本状态：**Git log**
**发现，回退版本之前的vr3版本没有了。也就是回退前的最新版。**
**那我发现回退错了，再想恢复到vr3怎么办？？？？？？**
**相当于，我从21世纪穿越到了18世纪，那么现在我再想从18世纪回到21世纪怎么办？？？？**
**![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180108112300816-1111578307.png)**
办法是有的，只要你窗口没关闭，或者你把版本号记住了，就可以，当然版本号，记住一部分也可以：
var3 commit版本号，我只记住前4位 ad94
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180108112822769-1406396986.png)
内容也回退到了var3:
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180108113004410-1492877267.png)
如果第二天你后悔了，机器也关了，commit id也没记住，怎么办，Git提供了 Git reflog命令来，查看你用过的命令 
`git reflog`
![](https://images2017.cnblogs.com/blog/1149221/201801/1149221-20180108113211535-1427050892.png)
**撤销更改：**
**遇到这样一种情况，怎么办呢？  **
**文件修改了，并且已增加git add到 暂存区，但是没有提交git commit到分支，需要撤回**
**那么Git已经替你想到了这个问题，使用命令Git checkout -- gt.txt**
**需要在强调的是，[撤销更改]，是撤销增加到[暂存区]的，如果是已提交到[当前分支]的，需要用Git reset --hard HEAD 回退版本。**
