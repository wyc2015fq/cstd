# 学会这两件事，让你成为 Git 老司机

2018/12/18 · [IT技术](http://blog.jobbole.com/category/it-tech/) · [2 评论 ](http://blog.jobbole.com/114551/#article-comment)· [Git](http://blog.jobbole.com/tag/git/)



本文由 [伯乐在线](http://blog.jobbole.com/) - [BEASTQ](http://www.jobbole.com/members/hearingdog) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Aditya Sridhar](https://medium.freecodecamp.org/how-to-become-a-git-expert-e7c38bf54826)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。

![img](http://wx1.sinaimg.cn/large/63918611gy1fyc37laig7j21970u0aos.jpg)

- 我在提交中犯了个错误，我如何修正它？
- 我的提交历史一团糟，我该如何让它更整洁？

如果你曾经有上述问题，那么这篇文章很适合你。这篇文章介绍了一个让你成为 Git 老司机的清单。

### 我在提交中犯了个错误，我该怎么办？

![img](http://wx3.sinaimg.cn/large/63918611gy1fyc37lqitoj21900u0ash.jpg)

情景 1

假设你已经提交了一堆文件，并发现输入的提交信息实际上并不清晰。现在你要更改提交消息。为此，你可以使用 `git commit --amend`：

```shell
git commit --amend -m “**New** commit message”
```





#### 场景 2

假设你要提交六个文件，但你最终错误地只提交了五个文件。你可能认为可以创建新提交并将第六个文件添加到该提交。

这种方法没错。但是，为了保持整洁的提交历史，如果你可以以某种方式将此文件加入到你之前的提交本身，那岂不是更好？这也可以通过 `git commit --amend` 完成：

```shell
git add file6
git commit --amend --no-edit
```



\```--no-edit` 表示提交信息不会更改。

#### 场景 3

无论你何时在 Git 进行提交，提交都会附上作者名称和作者电子邮箱。通常，当你第一次配置 Git 时，就需要设置作者和电子邮箱。你无需担心每次提交的作者详细信息。

也就是说，对于特定项目，你可能希望使用不同的电子邮箱 ID。你需要使用以下命令为该项目配置电子邮箱 ID：

```shell
git config user.email “your email id”
```



假设你忘记配置电子邮箱，并且已经完成了第一次提交。`amend` 命令也可以用于更改先前提交的作者消息。可以使用以下命令更改提交的作者信息：

```shell
git commit --amend --author "Author Name <Author Email>"
```



**注意事项**``

应该**仅在**本地仓库使用 `amend` 命令。在远端仓库使用 `amend` 命令会制造大量混乱。

### 我的提交历史一团糟，我该如何处理？

假设你正在处理一段代码。你知道代码大约需要十天完成。在这十天内，其他开发人员也将提交代码到远程仓库。

将本地仓库代码与远程仓库代码保持同步是个**很好的做法**。这在你拉取请求时会避免许多合并冲突的操作。因此，你应该每两天从远程仓库中拉取一个变更。

每次将代码从远程仓库拉取到本地仓库时，都会在本地操作中创建新的合并提交。这意味着你的本地历史提交记录会有大量的合并提交，这会让审阅人员头大。

![img](http://wx3.sinaimg.cn/large/63918611gy1fyc37mowyqj20vk0q4q4u.jpg)

上面是历史提交记录在本地仓库中的显示方式。

#### 如何让历史提交记录看起来更整洁？

这就需要用到 **rebase** 了。

#### 什么是变基（rebase）？

举个🌰。

![img](http://wx1.sinaimg.cn/large/63918611gy1fyc37n2olrj20tf0p3ta9.jpg)

此图显示了发布（release）分支和功能（feature）分支中的提交。

1. 发布分支有三个提交：Rcommit1、Rcommit2 和 Rcommit3。
2. 你在发布分支中仅有一个提交（即 Rcommit1）时，创建了功能分支。
3. 你已向功能分支添加了两个提交。它们是 Fcommit1 和 Fcommit2。
4. 你希望从发布分支提交到功能分支中。
5. 你可以使用变基来完成该操作。
6. 让发布分支命名为 **release**，让功能分支命名为 **feature**。
7. 可以使用以下命令进行变基：

```shell
git checkout feature
git rebase release
```



#### 变基

当执行变基时，你的目标是确保功能分支从发布分支获取最新代码。

变基命令尝试逐个添加每个提交，并检查冲突。这听起来是不是有点头大？

让我画个图帮助理解。

这显示了变基内部实际做的事情：

![img](http://wx3.sinaimg.cn/large/63918611gy1fyc37nk3kjj20u00u7q6d.jpg)

#### 第 1 步

1. 运行该命令的那一刻，功能分支指向发布分支的头部。
2. 现在，功能分支有三个提交，Rcommit1、Rcommit2 和 Rcommit3。
3. 你可能想知道 Rcommit1 和 Rcommit2 发生了什么？
4. 提交仍然存在，将在下面步骤中使用。

#### 第 2 步

1. 现在 Git 尝试将 Fcommit1 添加到功能分支上。
2. 如果没有冲突，则在 Rcommit3 之后添加 Fcommit1；
3. 如果存在冲突，Git 会通知你，你必须手动解决冲突。解决冲突后，使用以下命令继续变基：

```shell
git add fixedfile
git rebase --continue
```



#### 第 3 步

1. 一旦添加了 Fcommit1，Git 将尝试添加 Fcommit2。
2. 同样，如果没有冲突，则在 Fcommit1 之后添加 Fcommit2，并且变基成功。
3. 如果存在冲突，Git 会通知你，你必须手动解决。解决冲突后，使用第 2 步提到的相同命令。
4. 整个变基完成后，你会发现功能分支有 Rcommit1、Rcommit2、Rcommit3、Fcommit1 和 Fcommit2。

#### **注意事项**

1. 变基和合并（merge）在 Git 中都很有用。两种并无优劣之分。
2. 在合并的情况下，你将有个合并提交。在变基的情况下，不会像合并提交那样有额外的提交。
3. 一种最佳的实践是一分为二。使用远端仓库中的最新代码更新本地仓库时，请使用变基。在处理拉取请求，以将功能分支和发布分支或主分支合并时，请使用合并。
4. 使用变基会更改历史提交记录（使其更整洁）。但话虽如此，改变历史提交存在风险。因此，请确保永远不要对远程存储仓库的代码使用变基。始终仅对本地仓库代码使用变基，来更改历史提交记录。
5. 如果对远端仓库进行变基，会制造许多混乱，因为其他开发人员无法识别新的历史记录。
6. 此外，如果在远端仓库上完成变基，则当其他开发人员尝试从远端仓库中拉取最新代码时，就可能会出问题。所以，我再重申一遍，变基总是仅在本地仓库中进行。😃

### 恭喜

你现在是个 Git 老司机了。😃

在这篇文章中，你了解到：

- 修改提交记录
- 变基

这两个都是非常实用的概念。探索 Git 的世界，继续学习吧。