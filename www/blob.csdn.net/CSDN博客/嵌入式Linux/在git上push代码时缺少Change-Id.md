
# 在git上push代码时缺少Change-Id - 嵌入式Linux - CSDN博客

2018年10月26日 15:37:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：164



# 问题
使用Git提交代码时,遇到missing Change-Id in commit message footer
会在提交失败信息中提示如何操作,缺失change-Id 会出现如下提示信息,Hint: To automatically insert Change-Id, install the hook:
gitdir=(gitrev−parse−−git−dir);scp−p−P29418name@git.co.com:hooks/commit−msg(gitrev−parse−−git−dir);scp−p−P29418name@git.co.com:hooks/commit−msg{gitdir}/hooks/
按照提示执行 gitdir….命令然后重新commit 和 push 就可以
git commit –amend //不需要修改都行 直接退出即可
git push origin

# 解决
出现问题的时候肯定会有提示**红色**部分，按照输入就好了

