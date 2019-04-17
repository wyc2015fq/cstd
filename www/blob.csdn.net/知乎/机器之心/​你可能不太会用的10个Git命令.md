# ​你可能不太会用的10个Git命令 - 知乎
# 



> 本文讨论的是开发人员、数据科学家或产品经理应该了解的各种 Git 命令。我们将了解该如何用 Git 进行检查、删除和整理操作。我们还将介绍如何用 Bash 别名和 Git 编辑器配置来逃避 Vim 以节省时间。

**选自towardsdatascience，作者：Jeff Hale，机器之心编译，参与：李诗萌、Chita。**

如果你不熟悉基本的 git 命令，在阅读本文前可以先参考我之前写的关于 Git 工作流程的文章。

地址：[https://towardsdatascience.com/learn-enough-git-to-be-useful-281561eef959](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/learn-enough-git-to-be-useful-281561eef959)

本文介绍了 10 个常见的命令以及它们的一些参数。每个命令都可以链接到该命令的 Atlassian Bitbucket 指南。

**检查**

先了解一下如何检查改动痕迹。
![](https://pic2.zhimg.com/v2-5d1cf0e393fcacbfbab7f439c663dded_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='314'></svg>)- git diff——查看所有本地文件的改动。只改动一个文件的话可以在命令后添加文件名。
- git log——查看所有提交历史。还可用于带有 git log –p my_file 的文件，输入 q 退出。
- git blame my file——了解谁在什么时候对 my_file 做了什么样的改动。
- git reflog——显示本地代码库 HEAD 的更改日志。这个命令很适合查找丢失的工作。

用 Git 进行检查并不麻烦。相比之下，Git 中有不少删除和撤销提交以及文件改动的操作。

**撤销**

可以用 git reset、git checkout 和 git revert 撤销在代码库中所做的改动，这些命令可能有点难理解。

git reset 和 git checkout 既可用于提交也可用于单个文件的修改，而 git revert 只能用在提交层面。如果你只需要处理尚未合并到协作远程工作的本地提交，你可以使用这三者中任何一条命令。如果是协同工作且需要撤销远程分支中的提交，那么就用 git revert。
![](https://pic4.zhimg.com/v2-3fdb03dc209c36b161f5578b0b15d947_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='313' height='300'></svg>)
这些命令中的每一条都有多个参数。以下是常见的用法：
- git reset –-hard HEAD——撤销最近提交以来暂存区和非暂存区的改动。

指定不同的提交而不是 HEAD，以撤销自这条提交以来的更改。--hard 指的是撤销暂存区和非暂存区的更改。

要确保你撤销的不是协作伙伴所依赖的远程分支的提交。
- git checkout my commit——从 my_commit 中撤销非暂存区的改动。

HEAD 常用在 my_commit，用来撤销最近一次提交以来在本地工作目录的改动。

checkout 最适合用于仅限于本地的撤销。它不会破坏你的协作伙伴所依赖的远程分支的提交历史。

如果你将 checkout 用在分支而不是提交上，HEAD 将会切换到指定分支，并更新成匹配的工作目录。这是 checkout 命令更常见的用法。
- Git revert my commit——撤销 my_commit 中的更改。当用 revert 撤销改动时，它会产生新的提交。

对协作项目而言，revert 是很安全的，因为它不会覆盖其他用户分支可能依赖的历史记录。
![](https://pic4.zhimg.com/v2-4e83ded298d6ed188b737664af7ea707_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='640'></svg>)revert 很安全
有时候你只想删除本地目录中的未追踪文件。例如，也许你运行的代码在版本库中创建了许多你不需要的不同类型的文件。你可以一键清除它们！
- Git clean –n——删除本地工作目录中的未追踪文件。
- –n 表示试运行，在试运行中什么都不会删除。
- -f 表示实际删除文件。
- -d 表示删除未追踪的目录。

默认情况下不会删除 .gitignore 中的未追踪文件，但这种行为是可以更改的。
![](https://pic2.zhimg.com/v2-3e8b1a4cd8e30fc7d192726c8b4877d5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='661'></svg>)
现在你已经知道了 Git 中用于撤销操作的命令，接下来我们再看两条可以有序排列文件的命令。

**整理**
- Git commit –amend——将暂存区的更改添加到最近一次提交中。

如果暂存区中什么都没有，你可以用该命令编辑最新的提交信息。只有在提交尚未整合到远程主分支中时才使用该命令！
- Git push my remote –tags——将所有本地标记发送到远程版本库中。适用于版本变更。

如果你用的是 Python，并希望更改你所构建的包，bump2version 会自动为你创建标签。一旦将标签推送出去，你就可以在自己的版本中使用了。这里有一篇我的指南，可以指导你完成第一个 OSS Python 包。跟紧我，确保你不会错过版本控制的部分！
- 指南：[https://towardsdatascience.com/build-your-first-open-source-python-project-53471c9942a7?source=friends_link&sk=576540dbd90cf2ee72a3a0e0bfa72ffb](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/build-your-first-open-source-python-project-53471c9942a7%3Fsource%3Dfriends_link%26sk%3D576540dbd90cf2ee72a3a0e0bfa72ffb)

**求助，我被困在 Vim 中出不去了！**

使用 Git 时，你可能偶尔会发现自己被困在 Vim 会话中了。例如，你试着在没有提交信息的情况下提交，Vim 会自动开启。如果你不知道 Vim 的话，这种情况糟糕透了——在这个关于如何摆脱这一困境的 Stack Overflow 回答下有超过 4，000 的投票。

以下四步用于在已保存的文件中逃离 Vim：

1. 按 i 进入插入模式

2. 在第一行输入你的提交信息

3. 按下退出键——Esc

4. 输入 :x。别忘了冒号（colon）。

**更改默认编辑器**

为了避免使用 Vim，可以更改 Git 中的默认编辑器。以下是更改我使用的编辑器 Atom 的默认值的命令：

`git config --global core.editor "atom --wait"`

假设你已经安装了 Atom，你现在就可以在 Atom 中解决 Git 问题了。

**创建 Git 命令的快捷键**
![](https://pic1.zhimg.com/v2-b666c9751854883b0f599b81ed077d8c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='426'></svg>)
在 .bash_profile 中添加以下 alias 命令可以给 Git 命令添加快捷键：


```
alias gs='git status '
alias ga='git add '
alias gaa='git add -A '
alias gb='git branch '
alias gc='git commit '
alias gcm='git commit -m '
alias go='git checkout '
```


你可以调整上述命令，来为任何 Git 命令添加快捷键。

如果你没有 .bash_profile，你可以用以下命令在 macOS 上创建一个：

`touch ~/.bash_profile`

打开该文件：

`open ~/.bash_profile`

更多关于 .bash_profile 相关信息，请参阅：[https://stackoverflow.com/a/30462883/4590385](https://link.zhihu.com/?target=https%3A//stackoverflow.com/a/30462883/4590385)

现在在终端键入 gs 的效果和键入 git status 的效果是一样的。注意，在终端中你可以输入快捷键后再输入其它标记。

你也可以使用 Git 的别名，但这要求你在输入快捷键命令之前先输入 git。谁会需要这些额外的按键呢？

**总结**

本文介绍了许多关键的 Git 命令，还介绍了如何配置环境，这帮你节省了一些时间。现在你有了 Git 和 GitHub 的基础，准备好进行下一步了吗？

想要更深入地了解，请参阅这篇 Bitbucket Git 教程：[https://www.atlassian.com/git/tutorials/learn-git-with-bitbucket-cloud](https://link.zhihu.com/?target=https%3A//www.atlassian.com/git/tutorials/learn-git-with-bitbucket-cloud)

这里还有一篇关于 Git 分支的交互式指南：[https://learngitbranching.js.org/](https://link.zhihu.com/?target=https%3A//learngitbranching.js.org/)

去和别人一起工作、互相学习，并互相解惑吧！
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
*原文链接：*[https://towardsdatascience.com/10-git-commands-you-should-know-df54bea1595c](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/10-git-commands-you-should-know-df54bea1595c)


