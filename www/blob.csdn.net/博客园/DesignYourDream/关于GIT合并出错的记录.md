# 关于GIT合并出错的记录 - DesignYourDream - 博客园
# [关于GIT合并出错的记录](https://www.cnblogs.com/designyourdream/p/4651551.html)
今天给美术解决GIT资源冲突时碰到的问题，搞了挺长时间终于解决了。参看下面这个网址：http://www.bujichong.com/m/68
今天git一小部分代码发现出错了，
上网查了一下，
大体情况清楚了，
大概是在家里git提交过代码
2个问题：
1，本地未更新
2，我github我建立了多个项目，目录路径不一致，直接提交 origin会有问题
登录github，把对应的url地址复制到提交地址里，再更新提交，ok，解决
![](https://images0.cnblogs.com/blog/320699/201411/181659411918509.png)
在Pull时, 每个不同的项目地址,进行单独地址Pull操作和Commit操作. 即选择 Arbitrary URL选项,填入远程项目地址, 使用过的URL会有记录,选择即可.
![](https://images0.cnblogs.com/blog/320699/201411/181659420972365.png)
Pull正确如下:
![](https://images0.cnblogs.com/blog/320699/201411/181659424721551.png)
否则出现错误:
git.exe pull -v –progress         “origin”
/libexec/git-core/git-sh-setup: line 83: /bin/sed: Bad file number
From gitlab.bigdata.leshiren.com:datawarehouse/daily_computing
= [up to date]      develop    -> origin/develop
= [up to date]      master     -> origin/master
= [up to date]      mobileDaliyData -> origin/mobileDaliyData
= [up to date]      new_songtao_job -> origin/new_songtao_job
D:\Program Files (x86)\Git/libexec/git-core\git-pull: line 268: /bin/tr: Bad file number
Your configuration specifies to merge with the ref ‘develop’
from the remote, but no such ref was fetched.
git did not exit cleanly (exit code 1)
![](https://images0.cnblogs.com/blog/320699/201411/181659430501208.png)

