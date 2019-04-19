# Git 最佳实践：commit msg - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [legendtkl](http://www.jobbole.com/members/legendtkl) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 1.引言
git commit 是很小的一件事情，但是往往小的事情往往引不起大家的关注，不妨打开公司的 gitlab 上的任一个 repo，查看 commit log，满篇的 update 和 fix，完全不知道这些 commit 是要做啥。下面就说一些最近的看到和自己尝试的一些还不错的实践。
## 2. Tips
### 1. one thing one commit
在提交 commit 的时候尽量保证这个 commit 只做一件事情，比如实现某个功能或者修改了配置文件。
##### 1.易读
阅读整个项目代码的时候有时候整个项目通读并不是一个好的方法。我们可以通过 issue 或者 commit 来一点一点分解整个 repo。如果一个 commit 只聚焦在一个点上，那么代码看起来也会比较舒服，顺着 commit 读下来就是当初的开发过程了。
##### 2.cherry-pick
cherry-pick 是 git 中的一个非常有用的命令，可以将 commit 从一个分支“拷贝”到另一个分支。如果我的 commit 划分都很清楚，那么 cherry-pick 就会比较轻松。但是如果我的一个 commit 即实现了功能A，又实现了功能B，而我只想要功能A，这就很尴尬了。
也许有人会说怎么会有这么奇葩的需求，那么我说一个我刚遇到的例子。我现在负责一些底层的服务，上游的服务会对我有依赖。我对自己的服务有个排期，上游的服务有个排期，而且我目前做了一些接口的升级并不在上游服务的紧接着的下一次上线排期中。目前我的所有开发都是在 dev 分支，为了配合上游服务上线，我需要上线 dev 分支上的部分功能。这个时候只能从 master 上新开一个分支，然后把 dev 分支上和上游服务相关的 commit “拷贝”过去，也就是 cherry-pick。这个时候就可能遇到上面说的那种尴尬的情况。
##### 3. code review
review 过别人代码的人都知道如果 commit 乱七八糟那将有多么痛苦。我曾经给 githug 项目（一个 ruby 写的练习 git 的终端小游戏）提交 pr 的时候，一个 pr 包含了三个 commit，其中有两个 commit 是修改拼写错误，然后 repo 的作者让我把 commit 合并为一个，才答应给我 merge。
### 2. thoughtbot 规范
地址：https://github.com/thoughtbot/dotfiles/blob/master/gitmessage


```
# 50-character subject line
#
# 72-character wrapped longer description. This should answer:
#
# * Why was this change necessary?
# * How does it address the problem?
# * Are there any side effects?
#
# Include a link to the ticket, if any.
```
关于这个规范更详细的说明可以参考他们的一篇文章 [5 Useful Tips For A Better Commit Message](https://robots.thoughtbot.com/5-useful-tips-for-a-better-commit-message) 。简单来说：
- 第一行不超过 50 个字符
- 第二行空一行
- 第三行开始是描述信息，每行长度不超过 72 个字符，超过了自己换行。
- 描述信息主要说明：
- 这个改动为什么是必要的？
- 这个改动解决了什么问题？
- 会影响到哪些其他的代码？
- 最后最好有一个相应 ticket 的链接
Commit 这种格式有点类似邮件，主题不超过50个字符，然后空一行，这样显示的下面的部分都会折叠起来，类似下面的样子。我们使用命令 `git log --oneline`的时候就只显示第一行。
正文部分不超过 72 个字符，这个也主要是为了阅读方便。
关于最后的一个 ticket 的说明。我们开发之前需要将所有的功能进行拆解，然后开发过程中需要通过一些工具来 track ，每个小功能就是一个 ticket。有些公司使用 jira，有些公司使用 wiki。以使用 jira 为例，前面把功能拆解之后分到每个人手上。这样我们进行提交的时候附上对于的 ticket 链接或者 ticket 号，之后再回溯的时候就会非常方便了。或者也可以给 jira 开发插件，通过抓取 git commit 信息进行分析就能将相应的改动代码直接展示在 jira 上了。这么做的好处不言而喻，需求-功能-开发-代码，整个都被串起来了，不管是对于新人了解系统还是5年或者10年之后回溯都是非常有帮助的。
下面看一个我们项目中不同风格的 commit 截图对比。
![](http://legendtkl.com/img/uploads/2016/git_commit_example.png)
## 3. 示例
我们不妨看看开源社区的 git commit 规范。
### 1.**AngularJS**
地址：[**AngularJS Git Commit Message Conventions**](https://docs.google.com/document/d/1QrDFcIiPjSLDn3EL15IJygNPiHORgU1_OOAqWjiDU5Y/edit#heading=h.uyo6cb12dt6w) 。commit msg 包括三部分：header, body, footer。
#### 1. commit msg format
##### 1. revert
如果这个commit revert 了别的 commit，那么它的 header 应该以 “revert:”开始，后面跟上被 revert 的 commit 的标题。body 应该是 **`This reverts commit <hash>.`**
##### 2. header
header 应该包括 type，分隔符，主题
type 主要包括：
- feat (feature)
- fix (bug fix)
- docs (documentation)
- style (formating, missing semi colons, …)
- refactor
- test (when adding missing tests)
- chore (maintain)
主题信息能够简短地描述你的 commit 即可，结尾不要使用“.”，开头首字母不要大写。使用祈使语态，比如使用 change，而不是 changed
##### 3.msg body
除了 header 的主题信息的要求外，还需要包括为什么要做这个 commit，以及改动前后的对比。
##### 4.footer
- Breaking changes: 重要的改动要声明。(其实我觉得这个放在header里面更好)
- Referencing issues: 如果和 issue 相关，指出来。
#### 2. Example


```
fix($compile): couple of unit tests for IE9
Older IEs serialize html uppercased, but IE9 does not...
Would be better to expect case insensitive, unfortunately jasmine does
not allow to user regexps for throw expectations.
Closes #392
Breaks foo.bar api, foo.baz should be used instead
```
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/08/b03d5bb72b850a921d20ad80361670dd.png)![](http://jbcdn2.b0.upaiyun.com/2016/08/17a0083fd0a2e91432eeeeae5e4e94a9.jpg)
