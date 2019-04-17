# windows下安装SVN ，Git/SVN 常用命令笔记 - 在思索中前行！ - CSDN博客





2014年06月17日 21:49:55[_Tham](https://me.csdn.net/txl16211)阅读数：1228







Windows下：
1 安装svn,git,python
2 下载libUV源码,https://github.com/joyent/libuv，可以用git，也可以直接下

载源码包，当前是joyent-libuv-3de0411.zip，下载后解压到libuv目录。
3 命令行模式下cd进入libuv目录，运行vcbuild.bat。首先生成解决方案,类似Linux下的configure。如果没有安装svn和python的话会提示错误，下载安装上就可以了。
4 批处理执行完会生成uv.vcproj，用vc2010打开编译运行,如果提示找不到stdint.h，可以从mingw中找到复制过来即可。如果用2005或以前的编译器，提示一些错误，需要修改一些内容，直接用vc2010会顺利一些，用微软官方提供的免费版本就可以了，不需要破解什么旗舰版，又不是搞航母编队。
5 编译后debug就可以看代码了。代码很容易，会点完成端口一目了然，这时建议去看nodejs或Redis的代码，看怎样充分利用libUV的。

Linux下安装比Windows顺利的多，其实Windows也简单，关键一般编程常用的工具都不是2010，所以开始出现一些错误。


编译后开始调试代码，稍后继续。



#### 下载源码

```
git clone xx@xx:/xxx.git
```

#### 更新源码

```
git pull
```

#### 分支相关

```
git branch --查看分支
git checkout -b name  --创建分支
```

#### 更新相关

```
git add . -- 增加新的内容
git commit -m '修改的信息' -- 提交的内容的信息
git push -u origin master -- 将内容提交到主分支
```

#### Git状态

```
git status
```

#### 查看提交内容的差异

```
git log -p -1(1是最新的一条)
```

### SVN相关

#### 将文件下载到本地

```
svn checkout path(服务器上的目录)
//简写 svn co
```

#### 往版本库添加新文件

```
svn add file(文件名)
```

#### 提交文件到版本库

```
svn commit -m 'logMassage' (文件名)
```

#### 加锁／解锁

```
svn lock -m 'lockMessage' (文件名)
svn unlock path
```

#### 更新到某个版本

```
svn update -r m path -- m是版本号，path为文件名
//简写 svn up
```

#### 查看文件或者目录状态

```
svn status path
```

#### 删除文件

```
svn delete path -m 'delete message'
//简写 svn (del, remove, rm)
```

#### 查看日志

```
svn log path
```

#### 比较差异

```
svn diff 文件名
//简写 svn di
```






