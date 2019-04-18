# git中reset与revert的使用 - z69183787的专栏 - CSDN博客
2015年07月02日 13:29:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6834
最近用github做我的博客,有些时候会设置错误需要用撤回操作,这时就用到git revert与reset
### reset(版本撤回)
#### 格式
`git reset [-q] [<commit>] [--] <paths>...`
```
git
 reset (--patch | -p) [<commit>] [--] [<paths>...]
```
`git reset (--soft | --mixed | --hard | --merge | --keep) [-q] [<commit>]`
#### 用法
`git reset --mixed` 回退commit,保留源码,默认方式.
`git reset --soft` 回退至某个版本,只回退commit信息
`git reset --hard` 彻底回退至某个版本
- 回退所有内容到上一个版本 git reset HEAD^
- 回退a.py这个文件的版本到上一个版本 git reset HEAD^ a.py
- 向前回退到第3个版本 git reset –soft HEAD~3
- 将本地的状态回退到和远程的一样 git reset –hard origin/master
- 回退到某个版本 git reset 057d
- 回退到上一次提交的状态，按照某一次的commit完全反向的进行一次commit git revert HEAD
### revert(撤消操作)
撤销某次操作，此次操作之前的commit都会被保留. git reset 是撤销某次提交，但是此次之后的修改都会被退回到暂存区.
#### 格式
`git revert [--edit | --no-edit] [-n] [-m parent-number] [-s] <commit>...`
```
git
 revert --continue
```
`git revert --quit`
```
git
 revert --abort
```
#### 示例
- git revert HEAD~3：丢弃最近的三个commit，把状态恢复到最近的第四个commit，并且提交一个新的commit来记录这次改变。
- git revert -n master~5..master~2：丢弃从最近的第五个commit（包含）到第二个（不包含）,但是不自动生成commit，这个revert仅仅修改working tree和index。
### reset与revert的区别
- git revert是用一次新的commit来回滚之前的commit，git reset是直接删除指定的commit。
- 在回滚这一操作上看，效果差不多。但是在日后继续merge以前的老版本时有区别。因为git revert是用一次逆向的commit“中和”之前的提交，因此日后合并老的branch时，导致这部分改变不会再次出现，但是git reset是之间把某些commit在某个branch上删除，因而和老的branch再次merge时，这些被回滚的commit应该还会被引入。
- git reset 是把HEAD向后移动了一下，而git revert是HEAD继续前进，只是新的commit的内容和要revert的内容正好相反，能够抵消要被revert的内容。
