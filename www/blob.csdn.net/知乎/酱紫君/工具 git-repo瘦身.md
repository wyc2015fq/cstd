# 工具: git-repo瘦身 - 知乎
# 

Github网盘用的久了以后, 空间可能会有点不够用.

在 Mathematica 中如果你追踪了notebook的话,repo会迅速膨胀

因为图表之类的对象都是直接渲染出来图片, 相当于存了一堆图

但是 diff/trace notebook 的需求在整个研究周期中是客观存在的

notebook其实类似于实验室, 富文本格式有利于快速测试并改进原型代码

整个实验过程中当然要追踪所有的实验数据

但真正的核心源码还是用plaintext的wl文件比较好

不过久而久之呢, 项目就会变得很臃肿
![](https://pic3.zhimg.com/v2-3940badd4a4449f72258dcba76ed855a_b.jpg)
那么研究周期结束后怎么办呢, 就算直接删了也没用啊

文件还是存在于历史中啊

git filter-branch 遍历 history tree 是原生解决方案...

但是相信我, 用这个会疯掉的...

我们有个神器哦
[BFG Repo-Cleaner​rtyley.github.io](https://link.zhihu.com/?target=https%3A//rtyley.github.io/bfg-repo-cleaner/)
把你下载来的bfg.jar直接扔进工作目录

(这样就不用配Java了, 另外用.gitignore直接滤掉就行)

然后原地起一个控制台输入如下命令

```
java -jar bfg.jar --delete-files *.cdf
java -jar bfg.jar --delete-files *.nb
git reflog expire --expire=now --all && git gc --prune=now --aggressive
git push -f
```

这些命令会干掉所有**历史中存在但是现在不存在**的cdf/nb文件

然后强推到远端就行了

注意, 这是破坏性同步, 历史会直接断开

无法Revert/Reset到之前, 但是历史代码(除了被删掉的)其实还是访问的.

所以保险起见使用 git clone -mirror 克隆一份备份
![](https://pic4.zhimg.com/v2-10acd1c85ed63bc5f2ea722a49444423_b.jpg)
Bingo, 可以看到容量立马下来了

当然进行新工作前最好pull一波以防奇怪的问题

只有该次操作需要force push...如果之后还要求force push...赶快把你的备份覆盖回去...

BFG还可以用于定点清除误传的敏感文件...

BFG自动保护现有文件, 如果当前就有要删除的文件...让他变成历史然后清理掉最保险..

话说有了这个我甚至肆无忌惮的准备把参考资料等pdf也装进**豪华多平台客户端可同步带历史追溯容量无极限自带分享功能的高级网盘Github!**

