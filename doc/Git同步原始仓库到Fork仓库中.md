# [Git同步原始仓库到Fork仓库中](https://www.cnblogs.com/seayxu/p/5580749.html)



# 前言

本文介绍的是Git同步原始仓库到Fork仓库示例教程，废话不多说，下面直接到实操部分。

> 这里以aspnetcore-doc-cn的github仓库为例，同步dev分支。

# 步骤

## 1.初始化本地仓库

```
mkdir aspnetcore-doc-cn
cd aspnetcore-doc-cn
git init
```

## 2.添加远程仓库地址

添加原始仓库地址，就是被Fork的。

```
git remote add parent https://github.com/dotnetcore/aspnetcore-doc-cn.git
```

> 地址是https协议的，不能是ssh协议的，除非有权限。

添加自己远程仓库地址，最好是ssh协议地址。

```
git remote add origin git@github.com:SeayXu/aspnetcore-doc-cn.git
```

## 4.拉取原始远程仓库到本地

```
git pull parent dev
```

> **注意：**
> 初始化的仓库默认分支是**master**，如果你同步下来的分支不是在**master**分支，需要切换到其他的分支时，需要先提交一下本地仓库，然后再切换。

## 5.提交本地仓库

在拉取原始仓库后，可以根据自己需要是否需要本操作。如果拉取后有改动，执行提交操作，否则直接下一步。

```
git add -A
git commit -m "updated at:$(date '+%Y-%m-%d %H:%M:%S')"
```

> 这里为了自动化，后面的提交信息是一串时间。

## 6.推送本地仓库到远程仓库

```
git push origin dev
```

# 脚本

为了能不每次都敲这么多命令，可以将这些命令写在shell脚本中。
下面是我的一个示例：
**sync.sh**

```
#!/bin/bash
echo "change dir..."
cd ../src
echo "dir:`pwd`"

echo -e '\n'

echo "git pull repo from parent..."
git pull parent dev
echo "git pull repo from parent complated!"

echo -e '\n'

echo "git commit repo into local..."
git add -A
git commit -m "updated at:$(date '+%Y-%m-%d %H:%M:%S')"
echo "git commit repo into local complated!"

echo -e '\n'

echo "git push repo to origin...!"
git push origin dev
echo "git push repo to origin complated!"
```

[原文链接](http://www.jianshu.com/p/956c2ff56538)

[![知识共享许可协议](https://i.creativecommons.org/l/by-sa/4.0/80x15.png)](http://creativecommons.org/licenses/by-sa/4.0/)
本作品由SeayXu创作，采用[知识共享署名-相同方式共享 4.0 国际许可协议](http://creativecommons.org/licenses/by-sa/4.0/)进行许可。
基于<http://www.cnblogs.com/seayxu>上的作品创作。
可自由转载、引用，但需署名作者且注明文章出处，并以相同方式共享。





标签: [开发工具](https://www.cnblogs.com/seayxu/tag/%E5%BC%80%E5%8F%91%E5%B7%A5%E5%85%B7/)