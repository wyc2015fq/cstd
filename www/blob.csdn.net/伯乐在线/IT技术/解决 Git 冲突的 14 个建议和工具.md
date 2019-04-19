# 解决 Git 冲突的 14 个建议和工具 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [青劲草](http://www.jobbole.com/members/cqq2014) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Nicola Paolucci](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
Git 非常善于合并代码。代码的合并在本地完成，快速而且灵活。正常情况下每次从不同分支合并内容时，冲突有可能会发生。通常解决冲突很简单，就如同知道（如何）选择（保留）重要的更改一样，而有时解决冲突则需要额外的工作。
每个开发者对于解决冲突有不同的偏好。不久前，一位叫丹·史蒂文斯的同事用内部软件 [Questions for Confluence](https://www.atlassian.com/software/confluence-questions/) 询问了大家是如何做的。
![Questions on Confluence](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/questions.png)
收集到的回答和看法比 Atlassian 之墙有更大的吸引力。下面是我们用多种方式解决 Git 冲突的详尽描述，希望它能提供一些可以去尝试的想法，并且融入到你日常编程习惯中。
（ Atlassian 之墙是指 Atlassian 公司让客户将意见和反馈贴在墙上，可以参考[这幅图](http://atlassian.wpengine.netdna-cdn.com/wp-content/uploads/ww-customers-600x450.jpg)，译者注）
## 通用设置建议
为了设置 Git 使其能正确合并，我们先开始做一些简单的配置。
### 1. 设置建议
当遇到冲突时，可以在命令行或者其他可视化工具中输入“git mergetool”来初始化合并活动。在“.gitconfig”中用“merge.tool”变量来设置 Git 中自己喜欢的冲突解决软件，比如用 [KDiff3](http://kdiff3.sourceforge.net/) 的可能会这样填写”`.gitconfig`” 的 *merge *部分：

Shell
```
[merge]
tool="kdiff3"
```
上面的语句等价于在命令行输入以下命令：

Shell
```
git config --global merge.tool kdiff3
```
### 2. 在冲突标记中显示（分支）共同的祖先
用下面的设置来改进冲突标记使其也显示（分支）共同祖先（感谢罗宾·斯托克和休·吉登斯）：

Shell
```
git config --global merge.conflictstyle diff3
```
这个设置命令新添加一部分标记`|||||||` 从而给冲突加了注释，这样可以看到冲突行在有问题的两个分支的共同祖先处是什么状态。
### 3. 合并时使用“耐心”算法
如果文件内容很长（比如一个 XML文件）、冲突很多或者两个版本很不一致时，试着用下面的命令再次合并：

Shell
```
git merge --strategy-option=patience
```
“耐心”算法的结果应该可以更好地协调一些函数中或者标记中没有配对的括号，具体算法细节可以参考 [Stack Overflow](http://stackoverflow.com/questions/4045017/what-is-git-diff-patience-for) 上的一个回答。
### 4. 当你需要单个文件的历史信息时
除非使用像 [SourceTree](https://www.sourcetreeapp.com/) 一样的可视化工具来弄清到底对一个文件做过什么，不然你可以使用：

Shell
```
git log --merge --decorate --source -p path/to/file/you/care/about
```
## 手动解决冲突
处理合并问题主要有两类做法：有些开发者喜欢偏底层处理，因而手工操作处理冲突标记，而有些则偏好可视化工具来辅助（解决冲突）。两种方式都能极其有效地解决冲突。
### 5. 处理过程示例
有几个同事分享了他们手动处理的过程，比如詹森·欣奇描述了他的处理流程：
- 从手头的合并开始：

Shell
```
git merge the/other/branch
git status
```
- 看下有多少文件冲突。
- 对每个冲突文件：
- 在编辑器中打开文件（比如 IntelliJ 或 Vim ）
- 看看每个被冲突标记（“>>>>”和“<<<<”）围绕的区块。
- 看看（被冲突标记的区块）是否有意义，每个作者的意图是什么，如果能弄清楚就解决掉。
- 如果冲突标记无法理解，通常是这些文件改动很大，运行下面的命令：


Shell
```
git diff HEAD...the/other/branch -- path/to/conflicting/file
git diff the/other/branch...HEAD -- path/to/conflicting/file
```
这样做是为了看哪边改动较小- 通常下面的命令：

Shell
```
git log -p HEAD..the/other/branch -- path/to/conflicting/file
git log -p the/other/branch..HEAD -- path/to/conflicting/file
```
能帮助理解另一边改动了什么。
- 回溯文件到改动最大的一边：

Shell
```
git checkout the/other/branch -- path/to/conflicting/file
```
（在这里你也可以用 `git checkout --theirs 或者``--ours )`
-  手动检查并且再重新应用从另一边对文件的更改:

Shell
```
git add path/to/conflicting/file
```
- 当这些更改都修复之后要构建整个项目，确保至少可以编译通过，如果测试可以很快运行起来，也要运行一下这些测试：

Shell
```
git commit
```
这个过程看起来有点太手工化了，但詹森发现对于他的工作流程来说会更少产生不合理的合并。
想看一步步手动解决冲突的基本视频，可以参看我们最近的 [Git Power Routines](https://www.youtube.com/playlist?list=PLDshL1Z581YYxLsjYwM25HkIYrymXb7H_) 课程。
![Git Power Routines](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/power-routines.png)
## 合并工具的天堂
有很多不同的可视化工具来操作复杂的合并和解决冲突。我的同事们提到了一些（并不是所有）：
### 6. IntelliJ IDEA 冲突解决工具
IntelliJ IDEA 是很多 Atlassian 工作人员使用的 IDE 。很多人使用它内建的冲突解决工具来处理冲突，它提供了三个面板来分析：本地、远程和合并结果：
![IntelliJ IDEA](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/intellij.png)
### 7. KDiff3
[KDiff3](http://kdiff3.sourceforge.net/) 是 KDE 产品系列的一部分，并且在我们内部调查时提到过几次。
![KDiff3](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/kdiff3.png)
### 8. P4Merge
[SourceTree](https://www.sourcetreeapp.com/) 的作者斯蒂夫·斯特里廷和其他几个同事使用 P4Merge 来执行合并操作。[P4Merge](https://www.perforce.com/product/components/perforce-visual-merge-and-diff-tools) 是免费的可视化工具，它具有四个面板而不是其他工具提供的三个，显示了”`base`“、”`local`“、”`remote`“和”`merge result`“。
![P4Merge](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/p4merge.png)
### 9. meld
[meld](http://meldmerge.org/) 是用 GTK+ 和 Python 开发的，也是已经存在了很久的工具，被提到了几次。
![Meld](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/meld.png)
### 10. tig for status + diff
更多喜欢终端的人使用 [tig](http://jonas.nitro.dk/tig/) （我们之前写过 [tig](http://blogs.atlassian.com/2013/05/git-tig/) 的一篇介绍），再加上 “ git diff ”。
![Tig+diff](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/tig.png)
### 11. OS X 下的 FileMerge 即 opendiff
### 在长长的建议列表里，有几个开发者提到了 OS X 下原生的“opendiff”工具，或者叫做“FileMerge”。
![Filemerge](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/filemerge.png)
### 12. diffmerge
我并不知道 [diffmerge](https://sourcegear.com/diffmerge/) 这个工具，不过在列表里也被提到了。
![Diffmerge](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/diffmerge.png)
### 13. Araxis Merge (商业版)
[Araxis Merge](http://www.araxis.com/merge/index.en) 这个名字让我想起了很久以前。那时，我在一台 Windows 机器上一堆让人抓狂的 XML 文件中垂死挣扎，而这个工具证明了它可以经受住这个挑战。它是个商业软件。
![Araxis Merge](https://developer.atlassian.com/blog/2015/12/tips-tools-to-solve-git-conflicts/araxis.jpg)
### 14. vimdiff3
有几个同事用 [vimdiff](http://www.rosipov.com/blog/use-vimdiff-as-git-mergetool/) 来解决冲突，那是 vim 自带的合并/差异分析 工具，你可以这样设置：

Shell
```
git config merge.tool vimdiff
git config merge.conflictstyle diff3
git config mergetool.prompt false
```
或者按照上面展示的那样直接修改 .gitconfig 文件。
## 结论
你是如何解决冲突的呢？流程是怎样？你还使用过其他除了上文中提到以外的工具吗？让我们知晓你的技能吧，通过 [@durdn](https://www.twitter.com/durdn) 联系我或者 [@atlassiandev](https://www.twitter.com/atlassiandev) 我那很棒的团队。
