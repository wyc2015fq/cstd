# git 常用流程 - Keith - CSDN博客





2017年03月21日 20:17:50[ke1th](https://me.csdn.net/u012436149)阅读数：503
个人分类：[git](https://blog.csdn.net/u012436149/article/category/6805024)









## 关于 ssh key

第1步：创建SSH Key。在用户主目录下，看看有没有.ssh目录，如果有，再看看这个目录下有没有id_rsa和id_rsa.pub这两个文件，如果已经有了，可直接跳到下一步。如果没有，打开Shell（Windows下打开Git Bash），创建SSH Key：

```python
$ ssh-keygen -t rsa -C "youremail@example.com"
```

注意：`"youremail@example.com"`代表的是 你的 `github`账号

第2步： 复制生成的 ssh-key

```python
$ sudo apt-get install xclip
# Downloads and installs xclip. If you don't have `apt-get`, you might need to use another installer (like `yum`)

$ xclip -sel clip < ~/.ssh/id_rsa.pub
# Copies the contents of the id_rsa.pub file to your clipboard
```

如果 `xclip` 不工作的话，打开文件，手动复制。

第3步：登陆GitHub，打开“Account settings”，“SSH Keys”页面： 

然后，点“Add SSH Key”，填上任意Title，在Key文本框里粘贴id_rsa.pub文件的内容(`Ctrl+V`).

第四步： 点击 `Add ssh key`

## 本地操作

```python
git init # 初始化一个本地仓库
git add file1 file2 #向暂存区存储文件
git commit -m "discription" #将暂存区的文件提交到当前分支

## 删除文件
rm filename #从文件管理器中删除文件
git rm filename #从git的版本库中删除文件
git commit -m "Remove file" # 提交删除操作
```

## 远程操作

```python
git remote add origin your_repository #将本地库与远程库关联

#一旦关联之后，以后就是本地更改然后同步到远程库操作了
git push -u origin master #将本地更改同步到远程库
```

## 分支操作

```
# 查看当前分支
git branch

# 创建分支
git branch new_branch

# 切换分支
git checkout wanted_branch

# 创建并切换分支
git checkout -b new_branch

# 合并分支， 将 b 分支 合并到 a 分支
git checkout a
git merge b

# 删除分支， merge 之后就可以删除分支
git branch -d b
```

## 保持 fork 的repo与原 repo同步

[参考资料](https://gist.github.com/CristinaSolana/1885435)

```
# 1: clone 下来 fork 的 repo
git clone git@github.com:YOUR-USERNAME/YOUR-FORKED-REPO.git

# 2: Add remote from original repository in your forked repository
cd into/cloned/fork-repo
git remote add upstream git://github.com/ORIGINAL-DEV-USERNAME/REPO-YOU-FORKED-FROM.git
git fetch upstream

# 3: Updating your fork from original repo to keep up with their changes:
git pull upstream master

# 4: 更新 fork 下来的 repo
git push
```

## Pull Request

[https://www.zhihu.com/question/21682976/answer/79489643](https://www.zhihu.com/question/21682976/answer/79489643)这里有详细的介绍，不再赘述。

## 参考资料

[http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/001374385852170d9c7adf13c30429b9660d0eb689dd43a000](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/001374385852170d9c7adf13c30429b9660d0eb689dd43a000)
[https://help.github.com/articles/connecting-to-github-with-ssh/](https://help.github.com/articles/connecting-to-github-with-ssh/)



