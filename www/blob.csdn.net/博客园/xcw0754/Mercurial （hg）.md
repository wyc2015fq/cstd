# Mercurial （hg） - xcw0754 - 博客园
# [Mercurial （hg）](https://www.cnblogs.com/xcw0754/p/5341781.html)
附上两个站点：
`http://z42.readthedocs.org/zh/latest/devtools/hg.html`
`http://bucunzai.net/hginit/`
Mercurial(hg)是个版本管理工具，类似于git，但是它有自己的特点，比如可以一键生成web。
### 添加用户名
在.hg目录下有个hgrc文件，将下面代码丢到该文件的最底下即可，代码中的等号之后并无要求，写中文都是可以的。
```
[ui]
username = John Doe<john@example.com>
```
初始化一个仓库：hg init
查看当前工作目录跟仓库的区别：hg sum
提交未跟踪的文件：hg add  如果没有带文件名的话就提交全部了
### 变更
新文件即将需要被commit：hg add 
将某个已跟踪的文件从下一次commit开始移除：hg remove  文件可能暂时还在，但是commit之后就不在咯。
将当前目录切换到一个历史版本：hg update -r <版本号> 版本号写简写就行了，目录下的任何东西会被处理。
提交到远程仓库：hg commit 这样会提交所有文件，我们可以在commit后加文件名来只提交几个文件，也可以-m <评论>来写log信息，也可以com来代替commit。
列出当前版本库等待推送的变更列表：hg outgoing 列出即将被push的内容。
列出中央库中我所没有的所有变更：hg incoming 列出即将被pull下来的内容。
拉取远程库的最新变更：hg pull 所有最新的都被拉下来。
有两个head需要merge：hg merge 自动合并，如果没有冲突的话。
有两个以上head需要merge：hg merge -r <版本号> 需要指定tip与哪个版本号一块merge。
切换本地目录到tip上去：hg up 直接就切了，其实用的是update指令的简写。
清除掉那些残余的修改：hg update --clean 在误merge时，或者奇怪的问题出现可以试试。
将修改的文件恢复到最近一次提交后的状态：hg revert
撤销最后一次提交, 前提是你还没有push：hg rollback 注意看heads的变化啊。
推送当前的变更：hg push  url是可以省略的，就推到hg paths那里。
### 查看
查看一个文件的任何历史版本：hg cat  还可以试用-r <版本号>来看这个变更。
查看一个文件在两个版本中的变动：hg diff -r : v1和v2是版本号。
查看当前是在哪个版本下工作的：hg parent 很可能不是tip。
显示远程版本库列表：hg paths 你推送的东西就是到这里了。
### 功能
启用一个本地web server供别人拉取仓库：hg serve
拉取仓库默认需要ssl，可以关了它：在hgrc文件中设置`[web]push_ssl=False allow_push=*`重启即可，重启也是hg serve。
克隆别人的仓库：hg clone <地址> 也就是别人试用hg serve跑起来之后的地址复制过来就可以了。

