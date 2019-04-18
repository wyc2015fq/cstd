# 解决 git push Failed to connect to 127.0.0.1 port 45463: 拒绝连接 - Soul Joy Hub - CSDN博客

2017年06月25日 15:21:54[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：10207


[http://blog.csdn.net/u011239443/article/details/73717774](http://blog.csdn.net/u011239443/article/details/73717774)

今天上传Github代码突然报错：
`git push`
```
warning: push.default 尚未设置，它的默认值在 Git 2.0 已从 'matching'
变更为 'simple'。若要不再显示本信息并保持传统习惯，进行如下设置：

  git config --global push.default matching

若要不再显示本信息并从现在开始采用新的使用习惯，设置：

  git config --global push.default simple

当 push.default 设置为 'matching' 后，git 将推送和远程同名的所有
本地分支。

从 Git 2.0 开始，Git 默认采用更为保守的 'simple' 模式，只推送当前
分支到远程关联的同名分支，即 'git push' 推送当前分支。

参见 'git help config' 并查找 'push.default' 以获取更多信息。
（'simple' 模式由 Git 1.7.11 版本引入。如果您有时要使用老版本的 Git，
为保持兼容，请用 'current' 代替 'simple'）

fatal: unable to access 'https://github.com/xiaoyesoso/TensorFlowinAction/': Failed to connect to 127.0.0.1 port 45463: 拒绝连接
```

使用 lsof 发现端口未被占用：
`lsof -i:45463`
参看： 
[https://stackoverflow.com/questions/24543372/git-cannot-clone-or-push-failed-to-connect-connection-refused](https://stackoverflow.com/questions/24543372/git-cannot-clone-or-push-failed-to-connect-connection-refused)

解决了：
`env|grep -i proxy  `
的确是代理太多：

```
NO_PROXY=localhost,127.0.0.0/8,::1
http_proxy=http://127.0.0.1:45463/
HTTPS_PROXY=http://127.0.0.1:45463/
https_proxy=http://127.0.0.1:45463/
no_proxy=localhost,127.0.0.0/8,::1
HTTP_PROXY=http://127.0.0.1:45463/
```

全都`unset`后，`git push`恢复。

![这里写图片描述](https://img-blog.csdn.net/20170625152617464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

