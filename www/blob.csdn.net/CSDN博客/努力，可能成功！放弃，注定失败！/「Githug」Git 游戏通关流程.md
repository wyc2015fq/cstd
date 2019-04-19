# 「Githug」Git 游戏通关流程 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月09日 10:57:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1147
## Githug
他喵的这是个啥！？难道不是 GitHub 拼错了么，和 Git 什么关系？ 和游戏又有什么关系？
其实，他的元身在这里：[https://github.com/Gazler/githug](https://github.com/Gazler/githug) ，这个命令行工具被设计来练习你的 Git 技能，它把平常可能遇到的一些场景都实例化，变成一个一个的关卡，一共有 55 个关卡，所以将他形象的形容为 Git 游戏。
既然是游戏，作为一个专业的游戏玩家，通关自然是我的最终目标了！！！
## 安装游戏
没什么好说的，终端运行如下命令即可，如果碰到了墙，自行搬梯子
![](http://upload-images.jianshu.io/upload_images/228805-f8beaead2f236b46.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
安装游戏(盗个图)
## 游戏开始
直接输入 `githug` 就可以开始游戏了！开始的时候会询问是否创建文件夹，输入 `y` 确认创建，以后的操作将都在这个文件夹中进行。
![](http://upload-images.jianshu.io/upload_images/228805-9bb965df9645e4fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
游戏开始
## 第一关（Init）
紧接着，马上进入到了第一个关卡, 按照提示初始化这个这个 `githug` 文件夹为仓库。完成关卡可以通过调用 `githug play` 验证操作，成功则会进入下一个关卡
![](http://upload-images.jianshu.io/upload_images/228805-5bbab7ba52584cef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第一关
## 第二关（Config）
设置 Git 用户名和邮箱，为了不影响全局的配置，我设置的是仓库级别的。
![](http://upload-images.jianshu.io/upload_images/228805-4db2dfb5a1be63bf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二关
## 第三关 （Add）
使用 `add` 命令将 README 文件添加到 staging area.
![](http://upload-images.jianshu.io/upload_images/228805-bab666cc0a0ee862.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三关
## 第四关 （Commit）
提交 README 文件，记得每次 `commit` 使用 `-m` 参数加上备注是个好习惯
![](http://upload-images.jianshu.io/upload_images/228805-4e3e7ec95cd496fa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第五关（Clone）
克隆一个仓库，默认的文件夹名是远端的仓库名
![](http://upload-images.jianshu.io/upload_images/228805-bd68bc051aaa2d71.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第五关
## 第六关（Clone to folder）
同样是克隆一个仓库，不同的是可以指定一个文件夹名
![](http://upload-images.jianshu.io/upload_images/228805-9866ab909e3d5529.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第六关
## 第七关（Ignore）
忽略所有 `.swp` 后缀名的文件。这里使用 vim 编辑器打开 `.gitignore`，这个文件记录了 git 忽略文件的规则, 不会 vim 的同学可以用自己熟悉的编辑器。
![](http://upload-images.jianshu.io/upload_images/228805-ea0d615457e91f11.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第七关
使用正则(glob 模式)匹配所有的 `.swp` 文件，然后保存并退出
![](http://upload-images.jianshu.io/upload_images/228805-8cffc55c36e33eb1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
操作 .gitignore 文件
## 第八关（Include）
除了 `lib.a` 文件，其他所有的 `.a` 后缀名的文件都忽略。和上一关的操作一样，修改 `.gitignore` 文件
![](http://upload-images.jianshu.io/upload_images/228805-e018f1c31ef380c0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第八关
其中 `#` 开头的是注释，用 `*.a` 匹配所有 `.a` 文件，`!` 开头代表不要忽略
![](http://upload-images.jianshu.io/upload_images/228805-b15bfc75f366744c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
操作 .gitignore 文件
## 第九关（Status）
查看所有处于 `untracked` 状态的文件。使用 `git status` 查看当前仓库的状态，可以看到红色部分就是
`untracked` 状态的文件
![](http://upload-images.jianshu.io/upload_images/228805-74056f51af469de9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第九关
## 第十关（Number of files committed）
其实就是查看处于 staged 状态的文件，图中黄色部分就是，所以个数就是2
![](http://upload-images.jianshu.io/upload_images/228805-4f90b260a745075d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第十关
## 第十一关（rm）
有一个文件从硬盘中删除了，但是并未从 git 仓库中删除，找到它并从 git 仓库中删除。删除也是修改的一种，提交这个修改就好了
![](http://upload-images.jianshu.io/upload_images/228805-b82e2f6a5986b653.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第十一关
## 第十二关（rm cached）
讲一个新文件从 `staging area` 中删除。按照要求，不应该直接从硬盘上删除这个文件，只是从 Git 中删除而已。加上 
```
--cache
```
 可以是文件只是从 `staging area` 中移除，不会真正的删除物理文件，如果要连这个物理文件也一起删除，请使用
`-f` 选项
![](http://upload-images.jianshu.io/upload_images/228805-f740cf011029cc1f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第十二关
## 第十三关（stash）
临时提交某个文件。这个操作在需要临时保存修改，而又不想提交的时候特别好用！而且 git 中维护了一个栈来保存，所以支持提交多次。如果需要恢复某次提交，使用 
```
git stash apply
```
 即可。
![](http://upload-images.jianshu.io/upload_images/228805-a3bace485952bb2e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第十三关
## 第十四关（Rename）
重命名文件。首先这个文件需要是已经是已追踪状态，才可以使用 `git mv` 命令，操作完成后自动处于 staging 状态
![](http://upload-images.jianshu.io/upload_images/228805-84bc04fae9ccc2ea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
重命名文件
## 第十五关（Restructure）
移动所有 `.html` 文件到 `src` 文件夹。`git mv` 后面的第二个参数可以接受文件或目录，如果是目录，则文件会直接放入目录内，可以使用正则（glob模式）匹配所有 .html 文件
![](http://upload-images.jianshu.io/upload_images/228805-ce0a794b5ed906fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第十五关
## 第十六关（Log）
找到最新的 commit 的 hash 值。使用 `git log` 查看历史提交记录, 找到最新的 commit 的 hash 值，记录下来用户回答问题
![](http://upload-images.jianshu.io/upload_images/228805-db5efd79b5fd0149.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
回答问题
这里是按照倒叙排列的，最新的在最前面，`commit` 关键字后面跟着的就是这个 commit 的 hash 值
![](http://upload-images.jianshu.io/upload_images/228805-e6077c8d65810133.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
查看历史记录
## 第十七关（Tag）
为最新的 commit 打 tag。不加额外参数就是为当前 commit 记录 tag, 当然可以为特定的 commit 打
![](http://upload-images.jianshu.io/upload_images/228805-310c1e0f78f622eb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第十八关（Push tags）
将所有本地 tag 都推送到远端。`--tags` 参数代表将所有的 tags 都推送到远端
![](http://upload-images.jianshu.io/upload_images/228805-3ebda188c0a9ea6c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第十八关
## 第十九关（Commit amend）
某个文件在上次提交中遗漏了，在那次提交中补上这个文件。 其实，使用 `git commit --amend` 会进入编辑界面修改备注信息，我这里直接
`:wq` 保存并退出
![](http://upload-images.jianshu.io/upload_images/228805-a9b3f1f8b0ae7126.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第十九关
## 第二十关（Commit in feature）
为提交指定一个未来的时间。
![](http://upload-images.jianshu.io/upload_images/228805-d39cbb61f485e604.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二十关
![](http://upload-images.jianshu.io/upload_images/228805-e703e7c5b2676a1d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
备注操作界面
## 第二十一关（Reset）
两个文件都被添加到了 `staging area`, 但是只想提交其中一个。使用 `git reset` 可以用仓库中的版本覆盖
`staging area` 的版本。
- `git reset` 使用仓库中的版本覆盖 `staging area` 中的，如果 `working directory` 该文件没有其他修改，则
`staging area` 中的修改将应用到 `working directory` 中。反之`working directory` 中的版本将被保留，丢弃
`staging area` 中的修改。
- `git checkout` 则是使用 `staging area` 的中的版本覆盖 `working directory`。
![](http://upload-images.jianshu.io/upload_images/228805-76a18f5a11319d33.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第二十二关（Reset soft）
撤销上一次提交。
- `--soft` 参数将上一次的修改放入 `staging area`
- `--mixed` 参数将上一次的修改放入 `working directory`
- `--hard` 参数直接将上一次的修改抛弃
![](http://upload-images.jianshu.io/upload_images/228805-3f7d2475aaf46bcb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二十二关
## 第二十三关（Checkout file）
抛弃某一次的修改，使用上次提交的版本。`checkout` 和 `reset` 的区别参照第二十一关
![](http://upload-images.jianshu.io/upload_images/228805-c47e9f9e27b1b8a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二十三关
## 第二十四关（Remote）
查看远端仓库。其实可以不加`-v`参数，加这个参数只是可以将地址也一起输出(没想到下一关就是考察这个参数，平常习惯加这个参数了。。。)
![](http://upload-images.jianshu.io/upload_images/228805-12288b3583af15c0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二十四关
## 第二十五关（remote url）
查看远端仓库的 URL
![](http://upload-images.jianshu.io/upload_images/228805-cbc1ba40c760e9fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二十五关
## 第二十六关（pull）
拉取远端仓库。
其实可以指定分支，格式如下
```bash
git pull origin remote : local
```
对应的推送的格式如下
`git push origin local : remote`
需要注意的两个操作的分支顺序是**相反**的，记忆的方法很简单，拉取是从远端到本地，所以远端在前，而推送是从本地到远端，所以本地在前。
![](http://upload-images.jianshu.io/upload_images/228805-9ce988aa3be3ea11.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二十六关
## 第二十七关（Remote add）
添加一个远端仓库
![](http://upload-images.jianshu.io/upload_images/228805-23beaea54fdf691c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二十七关
## 第二十八关（Push）
推送本地修改到远端
![](http://upload-images.jianshu.io/upload_images/228805-9483bb83f2da3956.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第二十九关（Diff）
查看 `staging area` 和 `working directory` 中文件的差异。
- `git diff`: 查看 `working directory` 与 `staging area` 之间的差异
- `git diff --cached`: 查看 `repository` 与 `staging area` 之间的差异
- `git diff HEAD`: 查看 `working directory` 与 `repository` 之间的差异
![](http://upload-images.jianshu.io/upload_images/228805-4e5d56947b3006c6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第二十九关
对应 `git diff` 的显示结果
![](http://upload-images.jianshu.io/upload_images/228805-d7ecb58e504fccba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
差异信息
## 第三十关（Blame）
查看某个文件的修改人。这个命令简直邪恶，锅终于有人背了！！！
![](http://upload-images.jianshu.io/upload_images/228805-6cb15d360caafdbd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三十关
`git blame` 列出了文件中每行的修改人是谁
![](http://upload-images.jianshu.io/upload_images/228805-f832876cae4a2260.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
文件修改记录
## 第三十一关（Branch）
创建一个分支
![](http://upload-images.jianshu.io/upload_images/228805-ad2b5d20a6b64dbc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三十一关
## 第三十二关（Checkout）
创建一个分支，并切换过去。其实，`git checkout -b my_branch` 就是创建一个分支，并切换过去，而且这种方法更方便，平常用的更多
![](http://upload-images.jianshu.io/upload_images/228805-3dd334442532e984.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第三十三关（Checkout tag）
切换到某个特定的 tag
![](http://upload-images.jianshu.io/upload_images/228805-8295f2398509e376.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三十三关
## 第三十四关（Checkout tag over branch）
切换到某个特定的分支，但是分支名和标签名重叠了
![](http://upload-images.jianshu.io/upload_images/228805-ffbd2dabdcdefc00.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三十四关
## 第三十五关（branch at）
根据一个特定的提交创建新分支
![](http://upload-images.jianshu.io/upload_images/228805-904f7fb5c401b84c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三十五关
## 第三十六关（delete branch）
删除一个分支
![](http://upload-images.jianshu.io/upload_images/228805-b72f161400f03322.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三十六关
## 第三十七关（Push branch）
将分支推送到远端仓库
![](http://upload-images.jianshu.io/upload_images/228805-c1394566ada496f6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第三十八关（merge）
合并分支。为了简化分支模型，可以使用 `rebase` 代替，后续关卡会遇到。
![](http://upload-images.jianshu.io/upload_images/228805-8a096bc42768cfd1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三十八关
## 第三十九关（fetch）
获取远端的修改，但是并不合并到当前分支。其实，`git pull` 就是 `git fetch` 和 
```
git merge
```
 组成的。
![](http://upload-images.jianshu.io/upload_images/228805-8012e936f95fbfee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三十九关
## 第四十关（rebase）
其实不知道怎么翻译 `git rebase` 这个命令。大概意思是从某个提交分化出两个分支，然后其中一个分支需要将另一个分支的修改合并过来，但是又不想在提交记录上留下两个分支合并的痕迹，只留下一个分支以前后顺序记录两边的修改。
`git rebase` 一个分支的所有修改在另一个分支上重新应用一遍，所以在提交记录上看，会发现一个分支的所有提交在另一个分支之前或者之后。然后删除另一个被合并的分支，保持分支简洁。
`git rebase master feature` 表示将 `feature` 上的修改在 `master` 上重新应用一遍
![](http://upload-images.jianshu.io/upload_images/228805-0f8d92b1ce57817c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第四十关
对应第一个 `git log --graph -all` ，`--graph`会用图形化将提交记录显示出来，而`--all` 会显示所有分支的提交记录
![](http://upload-images.jianshu.io/upload_images/228805-297cd1ac288c1c16.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
原始状态
对应第而二个 `git log --graph -all` ，可以发现只保留了一个分支，看起来简洁了很多。
![](http://upload-images.jianshu.io/upload_images/228805-04985757665e7705.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
**在使用此命令的时候，需要非常注意的是，不要 rebase 哪些已经推送到公共库的更新，因为此操作是重新应用修改，所以公共库的更新可能已经被其他协作者所同步，如果再次 rebase 这些修改，将可能zh**
## 第四十一关（repack）
将版本库未打包的松散对象打包
![](http://upload-images.jianshu.io/upload_images/228805-a8845451e4dbc412.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第四十一关
## 第四十二关（cherry pick）
应用某一个提交的修改。
![](http://upload-images.jianshu.io/upload_images/228805-be53b6c8174af8d3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第四十二关
找到我们想要的那个提交，记录下它的 hash 值
`ca32a6dac7b6f97975edbe19a4296c2ee7682f68`
![](http://upload-images.jianshu.io/upload_images/228805-33b7e16ff233449c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第四十三关 （Grep）
`git grep`支持各种条件搜索及正则表达式，平时用的不多，但感觉功能强大。
![](http://upload-images.jianshu.io/upload_images/228805-9daf736288059c2b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第四十三关
对应 `git grep TODO` 的结果
![](http://upload-images.jianshu.io/upload_images/228805-40a03c8cad6bf67c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
搜索结果
## 第四十四关（rename commit）
重命名提交。当涉及提交修改时，应该想到 `git rebase -i` 命令，它接受可以一个参数（提交的哈希值），它将罗列出此提交之后的所有提交，然后可以对个个提交做对应的操作。
![](http://upload-images.jianshu.io/upload_images/228805-f6d8260657a89c73.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第四十四关
重命名前的提交记录
![](http://upload-images.jianshu.io/upload_images/228805-73a2f3906713774c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
重命名前
将需要重命名的提交前的 `pick` 修改为 `reword`
![](http://upload-images.jianshu.io/upload_images/228805-f50eb3af6f038273.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
选择需要重命名的提交
修改成新的备注，保存并推出
![](http://upload-images.jianshu.io/upload_images/228805-f3fd90216559943c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第四十五关（squash）
合并多个提交。
![](http://upload-images.jianshu.io/upload_images/228805-9b1ca109713cce0d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第四十五关
合并前的提交记录。
![](http://upload-images.jianshu.io/upload_images/228805-734de148b0c1504d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
将需要合并的提交前的 `pick` 改成 `squash` 或 `s`。`squash` 代表并入前一个提交，保存并退出。
![](http://upload-images.jianshu.io/upload_images/228805-5d0b4404e9161b31.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
为新的提交修改备注
![](http://upload-images.jianshu.io/upload_images/228805-07871d053e75fffe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第四十六关（merge squash）
将某个分支上的所有修改都应用成一个提交。默认修改都将进入暂存区
![](http://upload-images.jianshu.io/upload_images/228805-6ab764d72c069495.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第四十七关
重新排列提交顺序。
![](http://upload-images.jianshu.io/upload_images/228805-f4c29be1c72fe795.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第四十七关
排序前，对应第一个 `git log`
![](http://upload-images.jianshu.io/upload_images/228805-42af128b2923fb91.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
在编辑界面，将 `Second commit` 和 `Third commit` 的顺序调换
![](http://upload-images.jianshu.io/upload_images/228805-8aaf7e7a950aa290.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
排序后，对应第二个 `git log`
![](http://upload-images.jianshu.io/upload_images/228805-47b0cc6fec892ae0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第四十八关
使用 `git log` 查看所有的提交记录，太长我就不全贴出来了，找到最开始的提交 `f608824888b83bbedc1f658be7496ffea467a8fb`
![](http://upload-images.jianshu.io/upload_images/228805-f24a96c94941d528.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
`git bisect start master f608824888b83` 中，`master` 是有 bug 的节点，`f608824888b83` 是没有 bug 的节点。
![](http://upload-images.jianshu.io/upload_images/228805-f1e66f014924c06e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第四十九关（Stage lines）
其实，提交文件的部分修改这种需求平时还是比较常见的，不过平时都是用 Source Tree 来操作的，但是看到这题之后，好像又开启了一扇大门。
![](http://upload-images.jianshu.io/upload_images/228805-a4ca87f6ed06b77e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
![](http://upload-images.jianshu.io/upload_images/228805-4339205552556473.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
![](http://upload-images.jianshu.io/upload_images/228805-fd98d5d06b7f2d02.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第五十关（Find old branch）
`git reflog` 可以列出所有的操作记录，所以找到之前忘记的信息并不是什么难事
![](http://upload-images.jianshu.io/upload_images/228805-ce9910a4de1ee7a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第五十关
对应 `git reflog` 的显示内容
![](http://upload-images.jianshu.io/upload_images/228805-fa1f847f02625a55.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
## 第五十一关（Revert）
与 `reset` 不同的是，revert 只会撤销当前的 commit，而之后的 commit 操作的修改还会保留，但是`reset` 还会将之后的所有 commit 操作的修改全部退回 staging area 或丢弃。
![](http://upload-images.jianshu.io/upload_images/228805-105a856f2bc11540.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第五十一关
这是执行撤销操作前的记录，对应第一个`git log`
![](http://upload-images.jianshu.io/upload_images/228805-01878d2a9de2340d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
撤销前的记录
撤销操作会生成一个新的 commit，保存并退出即可
![](http://upload-images.jianshu.io/upload_images/228805-d94f642095c8a0af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
新的 commit 
撤销之后的记录，它不破坏原有的记录，对应第二个 `git log`
![](http://upload-images.jianshu.io/upload_images/228805-108ae452320c562a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
撤销后的记录
## 第五十二关（Restore）
根据之前的经验，`git reflog` 可以查看所有的操作记录，所以只要能找到误操作之前的 commit id，一样能够恢复现场。
![](http://upload-images.jianshu.io/upload_images/228805-d5bb45bea16955fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第五十二关
执行 `git reflog` 后画面如下，根据操作记录，找到你误操作的之前的 commit id
![](http://upload-images.jianshu.io/upload_images/228805-df9d407a3e307294.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
操作记录
## 第五十三关（Conflict）
冲突处理在平常的协同工作中真是再常见不过了，需要注意的是存在冲突的文件是在 working directory 中的，在解决完冲突之后需要添加到 staging area 并提交。
![](http://upload-images.jianshu.io/upload_images/228805-416f6359ad00254f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第五十三关
其实冲突解决完成的图片丢失了，只能口述了。
`<<<<<<< HEAD` 到 `=======` 之间的内容代表 `master` 分支的修改，`=======` 到
`>>>>>> mybranch` 之间的内容代表 `mybranch` 分支的修改，保留 `mybranch` 分支的修改，删除`master` 分支的修改即可，当然这些特殊符号所在行也要一并删除。
![](http://upload-images.jianshu.io/upload_images/228805-000b500f249a2f71.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
处理冲突
## 第五十四关（Submodule）
submodule 是一个很方便的将一个仓库分解成多个子模块的命令，特别是项目比较大且依赖其他 Git 项目的时候，比如 Cocos2d-x。虽然好用，但是门槛也相对高点，如果维护好 submodule 还是需要好好研究一下。
![](http://upload-images.jianshu.io/upload_images/228805-641f31d9460a9905.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第五十四关
## 第五十五关（Contribute）
其实到这里已经可以算是通关，如果感兴趣的话可以到 GitHub 为这个项目贡献代码。
![](http://upload-images.jianshu.io/upload_images/228805-11ba11fc1e9f0d35.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第五十五关
## 结尾
其实这里的所有关卡展示的内容只是 Git 的冰山一角，Git 的魅力远不止这些，还需我们慢慢探索~
如果文章有表述错误，欢迎指正。
最后，感谢这篇文章带我入坑：
[http://gold.xitu.io/entry/5684844560b2cd25b7cb41a3](http://gold.xitu.io/entry/5684844560b2cd25b7cb41a3)
文／_Ryeeeeee（简书作者）
原文链接：http://www.jianshu.com/p/482b32716bbe
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
