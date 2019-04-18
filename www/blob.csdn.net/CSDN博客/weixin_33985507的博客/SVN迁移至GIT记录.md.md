# SVN迁移至GIT记录.md - weixin_33985507的博客 - CSDN博客
2018年11月28日 17:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
> 
最近公司开始转向git，要求所有svn库迁移至Gitlab，除了SVN，公司在用的还有TFS也要求迁移，算是彻底转型了这次，以下做下记录。
## Why Git? —— from我们的大曾哥
> 
![74384-9baf63b587ed963f.png](https://upload-images.jianshu.io/upload_images/74384-9baf63b587ed963f.png)
image.png
![74384-bc3e2712478ac373.png](https://upload-images.jianshu.io/upload_images/74384-bc3e2712478ac373.png)
image.png
- **快速:** 如果你每移动一下鼠标都要等待五秒，是不是很受不了？版本控制也是一样的，每一个命令多那么几秒钟，一天下来也会浪费你不少时间。Git的操作非常快速，你可以把时间用在别的更有意义的地方。
- **离线工作:** 在没有网络的情况下如何工作？如果你用SVN或者CVS的话就很麻烦。而Git可以让你在本地做所有操作，提交代码，查看历史，合并，创建分支等等。
- **回退:** 人难免犯错。我很喜欢Git的一点就是你可以“undo”几乎所有的命令。你可以用这个功能来修正你刚刚提交的代码中的一个问题或者回滚整个代码提交操作。你甚至可以恢复一个被删除的提交，因为在后端，Git几乎不做任何删除操作。
- **备份:** 你有没有丢失过版本库？我有，而那种头疼的感觉现在还记忆犹新。而用Git的话，就不必担心这个问题，因为任何一个人机器上的版本都是一个完整的备份。
- **选择有用的代码提交。**
- **自由选择工作方式:** 使用Git，你可以同时和多个远程代码库连接，“rebase”而不是"merge"甚至只连接某个模块。但是你也可以选择一个中央版本库，就像SVN那样。你依然可以利用Git的其他优点。
- **保持工作独立:** 把不同的问题分开处理将有助于跟踪问题的进度。当你在为功能A工作的时候，其他人不应该被你还没有完成的代码所影响。分支是解决这个问题的办法。虽然其他的版本控制软件业有分支系统，但是Git是第一个把这个系统变得简单而快速的系统。
- **随大流:** 越来越多的公司，开源项目使用Git，包括Ruby On Rails，jQuery，Perl，Debian，Linux Kernel等等。拥有一个强大的社区是很大的优势，有很多教程、工具。
## 迁移步骤
#### 一、准备工作
1.You should already have the following prerequisites installed: git-svn, git-core, ruby, rubygems. If you want to make sure, you can check this by typing the following in the terminal:
```
$ git --help
$ git svn --help
$ ruby --help
$ gem --help
```
2.Install svn2git. You can install svn2git through rubygems, which will add the svn2git command to your PATH. Here’s how: `sudo gem install svn2git`
#### 二、开始迁移
**查阅迁移命令svn2git：**
`https://github.com/nirvdrum/svn2git/blob/master/README.markdown`
- 标准项目（trunk\branches\tags）
标准项目迁移完成后，将自动按trunk\branches\tags创建分支，trunk对应到master分支。
```
# 手动创建git仓库后，克隆至本地
git clone <repo>
cd <repo>
# 将svn拉到本地仓库(包含svn日志)
svn2git <svnUrl>
# push至git远程库
git push -u origin master
```
- 非标准项目
非标准项目即svn在使用时，在根目录下，创建一个个文件或文件夹，它们之间并没有分支的概念，早期我们用SVN时，只申请了一个svn，然后在根目录下建多个文件夹用以区分不同项目，这在git上其实是不允许的。
那么非标准项目svn的迁移就只能把根目录当作trunk分支，整个迁移至git的master分支下。
```
git clone <repo>
cd <repo>
svn2git <svnUrl> --rootistrunk
git push -u origin master
```
至此，迁移就算完成了。
## Gitlab Clone子目录
对于非标准项目迁移完就会出现一些问题，由于svn使用时是一个svn下放了很多个项目源码，大家开发时只签出自己负责的项目，这通过SVN是很方便签出指定目录的，但是在git就不是这样了，正常git签出都是完全签出，如签出master，就会把master下所有源代码签出到本地，这对于刚刚从svn转到git的人就很困扰了，其实只想签出有用的目录。
**Gitlab如何克隆指定目录？**
- 通过远程库创建空分支，但不签出：
```
mkdir subDirName
cd subDirName
git init
git remote add -f origin repo
git config core.sparseCheckout true
```
- 
在 .git/info/sparse-checkout中指定要签出的子文件夹：`echo "subDirName" >> .git/info/sparse-checkout`
- 
签出，此时便只会签出subDirName：`git pull origin master`
查阅：[https://stackoverflow.com/questions/600079/how-do-i-clone-a-subdirectory-only-of-a-git-repository/52269934#52269934](https://stackoverflow.com/questions/600079/how-do-i-clone-a-subdirectory-only-of-a-git-repository/52269934#52269934)
