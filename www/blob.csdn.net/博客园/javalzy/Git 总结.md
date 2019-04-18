# Git 总结 - javalzy - 博客园
# [Git 总结](https://www.cnblogs.com/javalzy/p/6146699.html)
详情请参考：https://git-scm.com/book/zh/v2
注意事项：
#1. 多提交(相当于多保存，多^S)：
    在Git中任何已提交的东西几乎总是可以恢复的。 甚至那些被删除的分支中的提交或使用 --amend 选项覆盖的提交也可以恢复。 然而，任何你未提交的东西丢失后很可能再也找不到了。
#2. 拉取别人数据之前要提交。减少工作区，暂存区数据冲突的可能。
#3. 推送之前先拉取。即将自已的版本做为最新之前，要先合并别人的修改。
#4. 切换分支前要提交，否则有可能数据丢失。即保存在此分支的修改。
#5. 合并分支之前要提交。拉取视情况而定（建议拉取）。
#6. 慎用 git checkout -- <file>
    撤消对文件的修改（拷贝了另一个文件来覆盖它）, 除非你确实清楚不想要那个文件了，否则不要使用这个命令。
#7. 尝试让每一个提交成为一个逻辑上的独立变更集。一个问题一个提交，这个提交包含了这个问题的全部修改。
#8. 最后一件要牢记的事是提交信息。 有一个创建优质提交信息的习惯会使 Git 的使用与协作容易的多。
#9. 变基：只对尚未推送或分享给别人的本地修改执行变基操作清理历史，从不对已推送至别处的提交执行变基操作。
#10. SourceTree GitFlow快捷键（mac）：alt + 花 + F 异常惊艳
环境搭建
#1：安装
    Mac: Mac苹果系统: git2.10.1 + sourceTree2.3.2
    Win: Windows 64 bit:  git2.5.1 + TortoiseGit + zh
#2：配置用户信息
    Win: 设置 ---> Git
    Mac: sourcetree ---> 偏好设置 ----> 通用
#3: 配置密钥
    Win: ????
    Mac: $ ssh -keygen -t rsa -C youremail@example.com (一路yes或null就可以)
           会存储在：/Users/mac/.ssh/*.pub
#4: 避免每次输入密码（未验证）
    git config --global credential.helper cache
#5：仓库划分和SVN相同。
    Art：加工 3dMax, ps的资源成为unity使用的 *.ab
    Design: 提供服务器和客户端用的表
    Public.ResPackage: 存Art生成的资源
    Public.PackedVersionConfig: 制做和存储version.txt文件及热更新文件
    Public.TTDS_apk：存安装包
    Server：服务器
    Client：客户端
#6: Clone工程
    a. 创建目标目录
    b. Win: ----
       Mac: git clone client@10.1.10.100:tiantiandiansha.git
    c. 显示隐藏文件 + 把(.git + 其它调整到合适位置)
    d. 用工具打开：菜单---> 仓库---> git flow ---> 初使化仓库
       确认本工程在 Develop分支
分支
#1: 分支的划分和目的(git flow)
    a. 线上问题
      master[主干]: 备份线上版本。
      hotfix[临时]: 修复线上bug。
    b. 日常开发
      develop[主干]: 功能集成。
      features[临时]: 日常开发。
    c. 发布
      master[主干]: 根据Tag发布版本。
      release[临时]: develop某些功能达到可发布程度，创建此分支，把功能集成到master，加Tag以备发版打包。
#2：在哪个分支上High
    a. 具体操做都在分支中完成，主干只负责数据集成。减少冲突方便并行。
      把主干当做单纯的数据源，分支是一个独立的空间。一个操做可以表述为，为达到一个目的从某个数据源取得数据、创建空间；然后在这个独立的空间里对其进行处理；处理完成之后再更新到某些数据源。一个操做结束。此分支也完成了他的使命。
    b. features：日常功能开发 + develop上的bug修复。
    c. hotfix: 线上bug修复。
       在master主干根据Tag创建，完成之后是否合并到develop或master需要根据develp或master的后续版本是否已修改来定。未修改的需要合并到develop+master。已修改的可以直接在hotfix分支打包发布。另外是否合并还要考虑是否是临时性暴力修复。
       注意：hotfix分支是从master旧的版本创建来的，合并时请注意莫回档。
    d. release：目的是把develop成熟的功能合并到master并打Tag。供后面按需发布。不在release分支做bug修复。
    e. bug修复：
       develop主干bug：由features分支修复。
       master线上bug：由hotfix分支修复。
       master未上线bug：由release 或  features + release修复（只果只用features需要手动将分支合并到master）。
#3：分支切换
    a. 未跟踪的文件：显示在了工作区。因未纳入任何分支，所以所有分支都可填加。
         在目标分区提交了原分区未暂存的文件，切换回原分支，原未暂存文件丢失。
    b. 已暂存未提交（新增加的文件，开始跟踪）：
         不放弃本地变更，到目标分支时还在已暂存未提交状态（sourceTree功能，将原分支的暂存状态copy过来了）。
         放弃本地变更，目标分支不存在已暂存未提交文件。再切回原分支原已暂存未提交文件丢失。（放弃本地变更==放弃填加文件）
       已暂存未提交（提交后再次修改）：
         不放弃本地变更, sourTree不允许。提示，先提交或隐藏（stash），之后再切换。
         放弃本地变更，目标分支不存在已暂存未提交文件。再切回原分支，原已暂存的修改丢失。（放弃本地变更==放弃已暂存文件修改）
       说明：切换分支时，将丢弃原分支已暂存的修改。
    c. 已提交的文件：原分支已提交的文件不会带入新的分支。
    请牢记：当你切换分支的时候，Git 会重置你的工作目录，使其看起来像回到了你在那个分支上最后一次提交的样子。 Git 会自动添加、删除、修改文件以确保此时你的工作目录和这个分支最后一次提交时的样子一模一样。
    总结：切换分支时，应保存（提交或隐藏）本分支的操作。否则切换回来后，未保存的内容将丢失。   因为当前工作区和暂存区只有一份，切换分支时要清除属于原分支的内容。未跟踪或首次暂存内容可进入新分支是工具的优化。
#4: 分支合并
    何时会合并：
        a. 用git pull从远端拉取时会合并(git pull = git fetch + git merge)。
        b. 主动合并分支时：如git flow的 features release 完成时。
    合并方法：
        在 Git 中整合来自不同分支的修改主要有两种方法：merge 以及 rebase(变基)
        a. 未分叉情况：快进方式（fast-forward）
            由于当前 master 分支所指向的提交是你当前提交（有关 hotfix 的提交）的直接上游，所以 Git 只是简单的将指针向前移动。 换句话说，当你试图合并两个分支时，如果顺着一个分支走下去能够到达另一个分支，那么 Git 在合并两者的时候，只会简单的将指针向前推进（指针右移），因为这种情况下的合并操作没有需要解决的分歧——这就叫做 “快进（fast-forward）”
        b. 分叉情况：
            在这种情况下，你的开发历史从一个更早的地方开始分叉开来（diverged）。 因为，master 分支所在提交并不是 iss53 分支所在提交的直接祖先，Git 不得不做一些额外的工作。 出现这种情况的时候，Git 会使用两个分支的末端所指的快照（C4 和 C5）以及这两个分支的工作祖先（C2），做一个简单的三方合并。
            和之间将分支指针向前推进所不同的是，Git 将此次三方合并的结果做了一个新的快照并且自动创建一个新的提交指向它。 这个被称作一次合并提交，它的特别之处在于他有不止一个父提交。
    冲突：
        a. 只有合并时才会有冲突。
        b. 文件冲突时，Git已经完成了合并（有冲突标记，此时冲突的文件应是已修改未暂存状态），但是没有自动地创建一个新的合并提交。此时Git会暂停下来，等待你去解决合并产生的冲突。
        在你解决了所有文件里的冲突之后，对每个文件使用 git add 命令来将其标记为冲突已解决。 一旦暂存这些原本有冲突的文件，Git 就会将它们标记为冲突已解决。
    变基：
        a. 原理：
            它的原理是首先找到这两个分支（即当前分支 experiment、变基操作的目标基底分支 master）的最近共同祖先 C2，然后对比当前分支相对于该祖先的历次提交，提取相应的修改并存为临时文件，然后将当前分支指向目标基底 C3, 最后以此将之前另存为临时文件的修改依序应用。
            这两种整合方法（merge和rebase）的最终结果没有任何区别，但是变基使得提交历史更加整洁。
        b. 风险：
            变基也并非完美无缺，要用它得遵守一条准则：不要对在你的仓库外有副本的分支执行变基。
            总的原则是，只对尚未推送或分享给别人的本地修改执行变基操作清理历史，从不对已推送至别处的提交执行变基操作，这样，你才能享受到两种方式带来的便利。
发版相关
    a. 使用 master分支发版（旧版本 + 新版本）,所以需将开发分支(develop)的修改合并到master。
    b. 哪个仓库需合并到master：
          Art: ??(负责生成 AssesBundle的人?)
          Design: ??
          Server: 客户端发版的人
          Client：服务器发版的人
          Public: 客户端发版的人 或 制做热更新包的人
    c. 合并到master分支的时间:
          Art: 成功发版后和加Tag一起
          Design: 成功发版后和加Tag一起
          Public: 成功发版后和加Tag一起
          Server: 发版结点，开发分支测试通过后。
          Client：发版结点，开发分支测试通过后。
    d. 加Tag:
          成功发版后给master分支加Tag. 以备以后切换到此Tag当时的版本改bug或发版本。
          由合并到master的人加Tag.
          注意：为保证根据Tag找到所有数据，tag不能漏加，tag格式一致（至少含相同版本号,精确到资源版本号 1.0.38.0.0 ）。
基础：
详情请参考：https://git-scm.com/book/zh/v2
#1. 文件状态及对应的工作区
    Git状态：已修改(modified)，已暂存(staged)，已提交（committed）----- 只管理已跟踪文件
    工作区： 工作目录，         暂存区，        Git仓库
    文件状态：未跟踪 已跟踪（未修改，已修改，已暂存）
        未跟踪：新加入当前分支，从未暂存（git add）过的文件。
        已跟踪：已提交到Git仓库或暂存过的文件
        已跟踪已暂存：暂存区中的文件（git add过）。
        已跟踪已修改：工作区中的文件。
        已跟踪未修改：已提交到Git仓库的文件（git commit过）。
#2. 暂存：
    1. 暂存操作会为每一个文件计算校验和（使用SHA-1哈希算法）
    2. 然后会把当前版本的文件快照保存到Git仓库中（Git使用blob对象来保存它们）
    3. 最终将校验和加入到暂存区域等待提交。
#3. 提交：
    1. 创建树对象并保存到Git仓库：
        Git会先计算当前分支的每一个子目录的校验和，然后在Git仓库中将这些校验和保存为树对象。
    2. 创建提交对象并保存到Git仓库：
        随后，Git便会创建一个提交对象（commit object）。提交对象保存的内容：
        a. 包含了作者的姓名和邮箱、提交时输入的信息。
        b. 指向它的父对象的指针。（首次提交产生的提交对象没有父对象，普通提交有一个父对象，而由多个分支合并产生的提交对象有多个父对象）
        c. 还包含指向1创建的树对象（项目根目录）的指针。
    3. 注意：提交只提交暂存区中的文件（修改过但未暂存的文件不会被提交）。
    暂存提交之后：Git 仓库中有五个对象：
      a. 三个 blob 对象（暂存时保存的文件快照）
      b. 一个树对象（记录着目录结构和 blob 对象索引）
      c. 以及一个提交对象（包含着指向前述树对象的指针和所有提交信息）。
#4. 分支
    a. Git的分支，其实本质上仅仅是指向提交对象的可变指针(Tag是不变的指针)。
    b. HEAD:  在 Git 中，它是一个指针，指向当前所在的本地分支（译注：将 HEAD 想象为当前分支的别名）。
    https://git-scm.com/book/zh/v2/Git-%E5%88%86%E6%94%AF-%E5%88%86%E6%94%AF%E7%AE%80%E4%BB%8B
    [个人理解]: 拉取，抓取，推送，都是对提交对象的下载上传，分支进度就是改变分支指针指向不同的对象。
#5. 抓取：git fetch [remote-name]
    git fetch: 命令与一个远程的仓库交互，并且将远程仓库中有但是在当前仓库的没有的所有信息拉取下来然后存储在你本地数据库中。
    https://git-scm.com/book/zh/v2/Git-%E5%9F%BA%E7%A1%80-%E8%BF%9C%E7%A8%8B%E4%BB%93%E5%BA%93%E7%9A%84%E4%BD%BF%E7%94%A8#_fetching_and_pulling
#6. 拉取：
    a. git pull: 命令基本上就是 git fetch 和 git merge 命令的组合体。
    b. 完整格式：git pull <远程主机名> <远程分支名>:<本地分支名>
    b. git pull :  Git从你指定的(当前分支所跟踪的)远程仓库中抓取内容，然后马上尝试将其合并进你所在的分支中。
    http://www.yiibai.com/git/git_pull.html
#7. 推送：git push [remote-name] [branch-name]
    a. git push: 计算你本地数据库与远程仓库的差异，然后将差异推送到另一个仓库中。 它需要有另一个仓库的写权限
#8. .git 目录
    这些条目是 Git 的核心组成部分。
        objects: 目录存储所有数据内容；
        refs: 目录存储指向数据（分支）的提交对象的指针；
        HEAD: 文件指示目前被检出的分支；
        index: 文件保存暂存区信息。
命令
详情请参考：https://git-scm.com/book/zh/v2
http://www.yiibai.com/git/git_pull.html
git help
git init
git status
    a. Untracked files: 未跟踪的文件
    b. Changes to be committed: (说明是已暂存状态)
          (use "git reset HEAD <file>..." to unstage)
    c. Changes not staged for commit: (说明已跟踪文件的内容发生了变化，但还没有放到暂存区)
          (use "git add <file>..." to update what will be committed)
          (use "git checkout -- <file>..." to discard changes in working directory)
    d. You have unmerged paths.(合并冲突)
          (fix conflicts and run "git commit")
    e. All conflicts fixed but you are still merging.
          (use "git commit" to conclude merge)
git status -s
    ??: 新添加的未跟踪文件前面有 ?? 标记
    A : 新添加到暂存区中的文件前面有 A 标记
    MM: 修改过的文件前面有 M 标记。 左M: 修改了并放入了暂存区 右M: 修改了还没放入暂存区.
    两位：左表暂存区，右表工作区
    说明：-s 不显示需推送的内容，只涉及到工作区和暂存区，Git仓库的状态不显示。
git add <文件|目录> : (添加内容到下一次提交中)
    a. 开始跟踪新文件
    b. 把已跟踪(已修改)的文件放到暂存区
    c. 合并时把有冲突的文件标记为已解决状态等
    运行了 git add 之后又作了修订的文件，需要重新运行 git add 把最新版本重新暂存起来：
git reset HEAD <file>
    说明：取消暂存的文件
    NOTE: 虽然在调用时加上 --hard 选项可以令 git reset 成为一个危险的命令（译注：可能导致工作目录中所有当前进度丢失！），但本例中工作目录内的文件并不会被修改。 不加选项地调用 git reset 并不危险 — 它只会修改暂存区域。
git checkout -- <file>
    说明：撤消对文件的修改（拷贝了另一个文件来覆盖它）
    IMPORTANT：这是一个危险的命令。 你对那个文件做的任何修改都会消失。除非你确实清楚不想要那个文件了，否则不要使用这个命令。
git diff
    此命令比较的是工作目录中当前文件和暂存区域快照之间的差异， 也就是修改之后还没有暂存起来的变化内容。
    请注意，git diff 本身只显示尚未暂存的改动，而不是自上次提交以来所做的所有改动。 所以有时候你一下子暂存了所有更新过的文件后，运行 git diff 后却什么也没有，就是这个原因。
git diff --staged（等同于 git diff --cached）
    查看已暂存的将要添加到下次提交里的内容。是暂存文件和已提交文件的比较。
git commit （这种方式会启动文本编辑器以便输入本次提交的说明）
git commit -m "Story 182: Fix benchmarks for speed"
    请记住，提交时记录的是放在暂存区域的快照。 任何还未暂存的仍然保持已修改状态，可以在下次提交时纳入版本管理。 每一次运行提交操作，都是对你项目作一次快照，以后可以回到这个状态，或者进行比较。
git commit -a -m 'added new benchmarks' (git acm: git add + git commit -m)
git commit --amend
    commit之后：版本修改将添加到历史记录
git rm PROJECTS.md（等于 git rm -f PROJECTS.md）
git rm --cached PROJECTS.md
git rm -f PROJECTS.md
    目标文件未被跟踪：操做无效
    目标文件在暂存区：
        a. 无参：提示加参数 --cached 或 -f
        b. -f: 从暂存区中删除，也从本地文件系统中删除。
        c. --cached: 从暂存区中删除，但不从本地文件系统删除。
    目标文件已提交在Git仓库中：
        a. 无参或加-f参数：即从仓库中删除，也从本地文件系统中删除。
        b. --cached: 从仓库中删除，但不从本地文件系统删除。
git mv file_from file_to
    运行 git mv 就相当于运行了下面三条命令：
        $ mv README.md README
        $ git rm README.md
        $ git add README
git log -p -2
git log --pretty=oneline
git log --pretty=format:"%h - %an, %ar : %s"
git log --pretty=format:"%h %s" --graph
git log --since=2.weeks
git log -Sfunction_name
git log --pretty="%h - %s" --graph -- 1.txt
git log --oneline --decorate 查看各个分支当前所指的对象
git clone https://github.com/schacon/ticgit
git clone git@github.com:mojombo/grit.git NewName
git remote
git remote -v
git remote show [remote-name] 查看远程仓库
git remote add <shortname> <url> 添加远程仓库
git remote rm paul 远程仓库的移除
git remote rename pb paul 远程仓库的重命名
git fetch [remote-name] 从远程仓库中抓取与拉取
    a. 这个命令查找 “origin” 是哪一个服务器（在本例中，它是 git.ourcompany.com）, 从中抓取本地没有的数据，
    b. 并且更新本地数据库.
    c. 移动 origin/master 指针指向新的、更新后的位置。
    当 git fetch 命令从服务器上抓取本地没有的数据时，它并不会修改工作目录中的内容。 它只会获取数据然后让你自己合并。
    要特别注意的一点是当抓取到新的远程跟踪分支时，本地不会自动生成一份可编辑的副本（拷贝）。 换一句话说，这种情况下，不会有一个新的 serverfix 分支 - 只有一个不可以修改的 origin/serverfix 指针。
    可以运行 git merge origin/serverfix 将这些工作合并到当前所在的分支。 如果想要在自己的 serverfix 分支上工作，可以将其建立在远程跟踪分支之上
git push [remote-name] [branch-name]
    当你和其他人在同一时间克隆，他们先推送到上游然后你再推送到上游，你的推送就会毫无疑问地被拒绝。 你必须先将他们的工作拉取下来并将其合并进你的工作后才能推送。 阅读 Git 分支 了解如何推送到远程仓库服务器的详细信息。
git tag
git tag -l 'v1.8.5*'
git tag -a v1.4 -m 'my version 1.4' 附注标签
git tag -a v1.2 9fceb02 后期打标签
git tag v1.4   轻量标签
git show v1.4
git push origin v1.5 共享标签
git push origin --tags
git push origin --delete serverfix 删除一个远程分支
    基本上这个命令做的只是从服务器上移除这个指针。 Git 服务器通常会保留数据一段时间直到垃圾回收运行，所以如果不小心删除掉了，通常是很容易恢复的。
git checkout -b version2 v2.0.0
    在 Git 中你并不能真的检出一个标签，因为它们并不能像分支一样来回移动。 如果你想要工作目录与仓库中特定的标签版本完全一样，可以使用 git checkout -b [branchname] [tagname] 在特定的标签上创建一个新分支：
git checkout -b [branch] [remotename]/[branch]
git checkout --track origin/serverfix
git checkout -b sf origin/serverfix
git branch -u origin/serverfix
    设置已有的本地分支跟踪一个刚刚拉取下来的远程分支，或者想要修改正在跟踪的上游分支，你可以在任意时间使用 -u 或 --set-upstream-to 选项运行 git branch 来显式地设置。
git branch
git branch -v 查看每一个分支的最后一次提交
git branch -vv 查看设置的所有跟踪分支，可以使用 git branch 的 -vv 选项
    需要重点注意的一点是这些数字的值来自于你从每个服务器上最后一次抓取的数据。 这个命令并没有连接服务器，它只会告诉你关于本地缓存的服务器数据。 如果想要统计最新的领先与落后数字，需要在运行此命令前抓取所有的远程仓库。 可以像这样做：$ git fetch --all; git branch -vv
git branch --merged
git branch --no-merged
git branch testing
git branch -d iss53
git checkout testing
git checkout -b iss53
git pull
    a. 在大多数情况下它的含义是一个 git fetch 紧接着一个 git merge 命令。
    b. 不管它是显式地设置还是通过 clone 或 checkout 命令为你创建的，git pull 都会查找当前分支所跟踪的服务器与分支，从服务器上抓取数据然后尝试合并入那个远程分支。
git pull <远程主机名> <远程分支名>:<本地分支名>
git merge iss53
git rebase master
git gc
    最妙之处是你可以随时重新打包。 Git 时常会自动对仓库进行重新打包以节省空间。当然你也可以随时手动执行 git gc 命令来这么做。
    当版本库中有太多的松散对象，或者你手动执行 git gc 命令，或者你向远程服务器执行推送时，Git 都会这样做。

