# Git分支开发的其他应用场景 - weixin_33985507的博客 - CSDN博客
2017年09月05日 20:23:06[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
之前我们已经学习了，git本地操作相关的命令，还有git分支开发的基本功能，今天我们补充一下git分支相关的其他操作，虽然应用场景比较少，但是却很有用。
- 
Git保存当前工作上下文
比如我们正在修改代码，突然有个线上紧急的问题需要我们修改，但是我们现在正在进行的工作刚进行到一半，不能上线；把我们辛苦的代码给删掉？太可惜了！怎么办？git给我们提供了“**git stash**”命令，能保证当前我们工作的内容，同事将工作区重置为最后一次提交时的状态，如图：![图片描述](https://image-static.segmentfault.com/386/131/386131012-59ad8333ab81f_articlex)上图展示的是正在编辑文件，还未完成，突然有打算需要暂停当前的编辑，基于上次编辑的内容重新编辑，但是目前的编辑内容又不能放弃，可以使用“**git stash**”保留当前编辑的上下文
从保存的上下文章回复上下文数据，我们使用“**git stash list**”命令来查看当前上下文列表，我们根据实际请款选择对应的上下文进行恢复，使用“**git stash applly stash@{pos}**”来恢复上下文。![图片描述](https://image-static.segmentfault.com/422/441/4224411147-59ad839265d94_articlex)git会自动合并上线文和最近修改的内容，如果发现冲突，会提示解决冲突，并在文件中表示出冲突的位置，我们参照之前文章中提到的解决冲突的方式进行解决即可。除了"**git stash apply <stash@{pos}>**"来回复上下文信息以外，还可以使用“**git stash pop**”来回复。恢复的是最近一个上下文信息，并把恢复的上下文信洗胸列表中删除。
由于上下文恢复以后一般就不再用了，我们可以删除，这时我们就用到了“**git stash drop <stash@{pos}>** ”,**stash@{pos}**是可选的，没有的话默认是删除最新的那个上下文列表：![图片描述](https://image-static.segmentfault.com/267/389/2673897359-59ad83e44942b_articlex)通过操作我们发现**git stash**很像我们玩闯关游戏时，在玩到某个环节我们因其他原因要中途停止，但是我们又不想再玩的时候从头开始，这个时候我们可以选择保存副本，当重新玩游戏时我们可以重头开始玩，也可以恢复副本继续上次的节点继续玩
- 
Git多人协作开发流程
我们在实际开发中是多人协作共同维护一个或是多个项目代码，这时候就需要多人之间协作，具体流程：
    (1).当完成本地的编辑开发以后，使用“git push origin 分支名”推送自己的修改
    (2).如果推送修改到远程仓库失败，说明远程分支比我们本地仓库的数据更(四声)新，这时应该先用“git pull”从远程仓库拉取最新代码到本地进行合并
    (3).如果在合并的时候发生了冲突，那么就按照解决冲突的流程进行处理，并解决了冲突以后的文件再提交
    (4).如果没有冲突或是冲突解决，我们再次执行第一步操作
    (5).如果提示“no tracking infomation”，则说明本地仓库分支没有与远程仓库对应分支建立关联，使用“git branch --set-upstream branch-name origin/分支名”进行关联
- 
git标签
由于commitid可读性较差，让人不通过commitid很好的管理git仓库中各个版本的内容，所以使用标签来指向具体的commitid：
“git **tag tagname <commitid>**”创建一个标签在没有哦指定commitid情况下，默认指向当前最新的commitid
“**git tag**”用户显示当前所有tag列表![图片描述](https://image-static.segmentfault.com/103/857/1038576773-59ad8574c0c3a_articlex)“**git show tagname**”查看指定名称tagname的信息![图片描述](https://image-static.segmentfault.com/777/746/777746585-59ad858b25d67_articlex)"**git tag -d tagname**"删除指定名称的tag![图片描述](https://image-static.segmentfault.com/162/684/1626849901-59ad85a47fa4b_articlex)**git push origin 分支名 tagname(tags)**将标签数据推送到远程仓库
