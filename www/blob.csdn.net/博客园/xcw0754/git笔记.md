# git笔记 - xcw0754 - 博客园
# [git笔记](https://www.cnblogs.com/xcw0754/p/5099800.html)
有益的连接如下：
[如何使用Git上传项目代码到github](http://1ke.co/course/194)
[git官方教程](https://git-scm.com/book/zh/v2/%E8%B5%B7%E6%AD%A5-%E5%85%B3%E4%BA%8E%E7%89%88%E6%9C%AC%E6%8E%A7%E5%88%B6)
阮一峰 - [常用 Git 命令清单](http://www.ruanyifeng.com/blog/2015/12/git-cheat-sheet.html)
**配置:**
    (1) /etc/gitconfig文件是所有用户的通用配置。如果使用带有 --system 选项的 git config 时，它会从此文件读写配置变量（修改的就是此文件了）。
    (2) ~/.gitconfig 或 ~/.config/git/config 文件都只是针对当前用户。可以传递 --global 选项让 Git 读写此文件。
    (3)当前使用仓库的 Git 目录中的 config 文件（就是 .git/config）则只针对该仓库。
    (4)设置命令别名（类似于宏，仅简单替代，可替代字符串/命令）：git config --global alias.co checkout   以后用co就可代替checkout了，如果给checkout加上双引号则代表字符串替代，可组合多个命令。
        丧心病狂的配置：git config --global alias.lg "log --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"
        撤销别名： git config --global --unset alias.co
    (5)生成ssh-key：ssh-keygen -t rsa -C "xcw0754@xxx.com"
    　　以下是初始时，git的两个最普通的配置
　　$ git config --global user.name "John Doe"
    　　$ git config --global user.email johndoe@example.com
**推送：**
    (1)要关联一个远程库，使用命令git remote add origin git@server-name:path/repo-name.git
    (2)关联后，使用命令git push -u origin master第一次推送master分支的所有内容
    (3)此后的每次推送就可以使用命令git push origin master推送最新修改(origin表示该url，master表示仅仅推送此分支，若要推送所有分支)
    (4)查看commit记录：git log   列出了当前分支的所有commit。 
    (5)推到远程其他分支：git push -u origin <local-bra>:<remote-bra>       -u选项是--set-upstream的简写，第一次推需要设置推到远程哪个分支。
**分支：**
    (1)创建：git branch <分支名>
    (2)切换分支：git checkout <分支名>
    (3)创建并切换到新分支：git checkout -b <分支名>    [起点分支名]        起点分支可省略，就默认当前，写了就表示从该起点创建的分支。
    (4)查看当前所有分支：git branch      两个有用的选项--merged 与 --no-merged 可以筛出已合并/未合并到当前分支的分支。
    (5)合并分支到当前分支：git merge <分支名>    git会尽量默认使用fast-forward(可使用--no-ff使用普通模式，则会强迫创建一个新的commit作为合并点)。若未合并之前master已经commit，则需要自己动手处理冲突。
    (6)删除分支：git branch -d <分支名>         正常情况下是可以删除的，若不行，则换成-D选项强制删除，但可能会丢失一些工作。
    (7)查看每个分支的最后一次提交： git branch -v
**误操作：**
    (1)误commit：方法一，git commit --amend   覆盖掉前一次的误commit，但仍是可恢复的
                 方法二，将HEAD往回退一步，则该次错误的commit仍保存着。     
    (2)误add：git reset HEAD <file>   仅仅将file撤出了暂存区，该真实文件无变化。若加--hard选项，则将暂存区file恢复出来。
    (3)误修改文件：git checkout -- <file>    将上一次add到暂存区的file恢复出来，如果暂存区没有对应file，则恢复出上一次commit时的file。对已跟踪文件有效。
   (4)误修改批量文件：git checkout .　　当前整个文件夹都回退，同样也是仅对已跟踪的文件有效。
    (5)版本回退：git reset --hard HEAD^      有多少个^就表示回退多少次，每次都往根的方向回退，仅仅设置了HEAD指针而已。
                git reset --hard <commit-id>
                git reset HEAD~n            直接指出要回退多少次，--hard可以使得当前所有文件回退到那个版本[致命]。
                当回退后，要往回寻找节点时，可以git reflog查看历史操作，其中便记录有commit-id。
    (6)查看文件区别：git diff HEAD -- <file>   查看的是当前实际文件，与HEAD所指向的版本的对应文件的区别。
```bash
(7)删除未跟踪文件：
```
```bash
git clean -f
```
(8) 删除未跟踪目录：
```bash
git clean -d　　可用-fd删文件和目录。
```
```bash
```
(9)
```bash
连 gitignore 的未跟踪文件/目录也一起删掉
```
```bash
git clean -xfd
```
(10)在
```bash
用 git clean 前，可加 -n 参数来先看看会删掉哪些文件，比如
```
```bash
git clean -nf
```
**突发：**
(1)临时保存当前工作区：git stash 
(2)恢复出之前保存的工作区：git stash apply stash@{n} 恢复后的stach并不会被删除，可以多次apply恢复。
(3)删除之前保存的工作区：git stash drop stash@{n}
(4)恢复出最近一次保存的共组区并删除该次stash：git stash pop 注意最好恢复在所保存的分支上，否则恢复出来时会冲突，需要处理。
(5)查看所有的stash暂存工作区：git stash list
(6)取消已跟踪的文件：git rm --cached <file>
**团队协作：**
    (1)查看远程库信息，使用git remote -v
    (2)从本地推送分支，使用git push origin branch-name，如果推送失败，先用git pull抓取远程的新提交，解决冲突再提交。
    (3)在本地创建和远程分支对应的分支，使用git checkout -b <branch> origin/<branch>，本地和远程分支的名称最好一致。
    (4)clone仓库时远程分支尚未存在，但要提交时却已经存在，则可建立本地分支和远程分支的关联，使用git branch --set-upstream <branch> origin/<branch>。   
    (5)查看某个远程仓库的详细信息：git remote show [remote-name]      这里的remote-name可以是简写，比如origin。
    (6)修改仓库引用名称：git remote rename <old-name> <new-name>     比如可以将origin改成ori等。
    (7)移除仓库引用：git remote rm <remote-name>   仅仅是一项配置，不会引起什么后果。 
    (8)创建与远程分支对应分支：git checkout -b <local-name> origin/<remote-name>    通常在fetch之后会拉到一些commit，而拉到的都是以origin/<name>命名的，而HEAD不会自动移动的。
    (9)pull和fetch的区别：pull会查找当前分支所跟踪的远程分支，从服务器上抓取数据然后尝试自动合并，一般是失败了。fetch是抓取所有的变动，但不会影响HEAD。由于pull的魔法经常令人困惑所以通常单独显式地使用 fetch 与 merge 命令会更好一些。
    (10)删除一个远程分支：git push origin --delete <branch-name>   这是直接操作服务器上的分支的，虽然可恢复，但慎重。
    (11)在本地删除所跟踪的一个远程分支：git branch -dr <remote/branch>    
    (12)整合的时候，merge是一个选择，而rebase是另一个选择。rebase会丢弃掉一些历史版本而创建对应的新版本进行尝试整合，会更简洁也过于简洁。
    (13)在本地分支上合并远程分支的两种方式：    git merge origin/master            这是合并。
                                            git rebase origin/master        这是变基，注意后果。
    (14)从远程仓库拉更新：git fetch origin                拉下origin所有的更新。
                        git fetch origin <branch-name>    仅拉下指定的分支。    
    (15)合并commit： git rebase -i origin/master        将当前分支rebase到origin/master分支上，会出现vim打开的文件，修改后可以进行改动commit信息。换种方式也行，比如撤销所有commit再提交成一次。
**标签**(可以认为是一个commit点的别名，有轻量标签和附注标签两种)**：**
    (1)命令git tag <name>用于新建一个标签，默认为HEAD，也可以指定一个commit id（7位的八进制码）。
    (2)打附注标签：git tag -a <tagname> -m "..."    可以指定一条标签信息，其他信息会自动打入。
    (3)git tag -s <tagname> -m "..."    可以用PGP签名标签（好像没什么用？）。
    (4)查看标签列表：git tag       而git tag -l '正则表达式' 会列出所有匹配项，当标签较多时还是很有用的。
    (5)显示指定标签信息：git show <tag-name> 
    (6)推送一个本地标签：git push origin <tagname>       值得注意的是，推送标签指的仅仅是推送一个标签，与代码无关。
    (7)推送全部未推送过的本地标签：git push origin --tags
    (8)删除一个本地标签：git tag -d <tagname>
    (9)删除一个远程标签：git push origin :refs/tags/<tagname>
    (10)在标签处创建分支：git checkout -b <分支名> <tag-name>
**查看：**
    (1)谁在何时修改：git blame <file>        这会将整个文件列出，并说明每一行的添加/修改时间，及修改者。
    (2)显示当前文件与之前的区别：git diff        若有暂存则优先比对暂存区。    
**命令详细解释：**
    git status <file>        显示该文件名的状态
    git log [参数]
            -p    显示每次提交之间的差异
            -<n>    只显示最近n次提交，<>不需要输入。（不常用）
            --since, --after    仅显示指定时间之后的提交。
            --until, --before    仅显示指定时间之前的提交。
            --author    仅显示指定作者相关的提交。
            --committer    仅显示指定提交者相关的提交。
            --grep        仅显示含指定关键字的提交。注意多种筛选需要加“--all-match”
            -S            仅显示添加或移除了某个关键字的提交
            --since=2.weeks    列出最近两周的提交
            --shortstat    只显示 --stat 中最后的行数修改添加移除统计。
            --name-only    仅在提交信息后显示已修改的文件清单。
            --name-status    显示新增、修改、删除的文件清单。
            --abbrev-commit    仅显示 SHA-1 的前几个字符，而非所有的 40 个字符。
            --relative-date    使用较短的相对时间显示（比如，“2 weeks ago”）。
            --graph        显示 ASCII 图形表示的分支合并历史。    
            --stat    显示每次提交的简略统计信息
            --pretty=[参数]    按照格式来显示每次提交的信息，还可以搭用--graph来图形显示分支合并。
                    oneline    让每个提交都一行显示
                    short    显示最短的信息
                    full    显示多一行提交者
                    fuller    显示作者/提交时间
                    format:""    和printf差不多的格式，有如下一些格式：
                        %H    提交对象（commit）的完整哈希字串
                        %h    提交对象的简短哈希字串
                        %T    树对象（tree）的完整哈希字串
                        %t    树对象的简短哈希字串
                        %P    父对象（parent）的完整哈希字串
                        %p    父对象的简短哈希字串
                        %an    作者（author）的名字
                        %ae    作者的电子邮件地址
                        %ad 作者修订日期（可以用 --date= 选项定制格式）
                        %ar 作者修订日期，按多久以前的方式显示
                        %cn    提交者(committer)的名字
                        %ce    提交者的电子邮件地址
                        %cd    提交日期
                        %cr    提交日期，按多久以前的方式显示
                        %s    提交说明
git log常见语句：
git log --graph --decorate --all   直接看所有提交的详细信息，会显示分支名。
git log --graph -10 显示最新的10条提交。
    git diff [参数]    <file>
           -L 40,60     指定区间
                 40,+21    指定从某行开始的21行
git cherry-pick 用于将某个分支上的多个commit迁移到另外的分支上
**Git命令自动补全工具：**
为了得到这个脚本，在Unix系统下运行以下命令：
```
1 cd ~
2 curl https://raw.github.com/git/git/master/contrib/completion/git-completion.bash -o ~/.git-completion.bash
```
然后，添加下面几行到你的 ~/.bash_profile 文件中：
```
1 if [ -f ~/.git-completion.bash ]; then
2     . ~/.git-completion.bash
3 fi
```
最后使.bash_profile立即生效：
```
1 $source ~/.bash_profile
```

