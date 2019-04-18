# Git学习笔记：fork和clone的区别 - weixin_33985507的博客 - CSDN博客
2018年05月07日 14:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
#### fork和clone的区别
```
fork：在github页面，点击fork按钮。将别人的仓库复制一份到自己的仓库。
clone：将github中的仓库克隆到自己本地电脑中。
```
问题：
pull request的作用
比如在仓库的主人(A)没有把我们添加为项目合作者的前提下，我们将A的某个仓库名为“a”的仓库clone到自己的电脑中，在自己的电脑进行修改，但是我们会发现我们没办法通过push将代码贡献到B中。
所以要想将你的代码贡献到B中，我们应该：
在A的仓库中fork项目a (此时我们自己的github就有一个一模一样的仓库a，但是URL不同)
将我们修改的代码push到自己github中的仓库B中
pull request ，主人就会收到请求，并决定要不要接受你的代码
也可以可以申请为项目a的contributor，这样可以直接push
#### fork了别人的项目到自己的repository之后，别人的项目更新了，我们fork的项目怎么更新？
答：首先fetch网上的更新到自己的项目上，然后再判断、merge。fetch+merge与pull效果一样。但是要多用fetch+merge，这样可以检查fetch下来的更新是否合适。pull直接包含了这两步操作，如果你觉得网上的更新没有问题，那直接pull也是可以的。
