# 我也用github（2）——关联本地工程到github - xiahouzuoxin - CSDN博客





2013年07月21日 08:59:33[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：12928








github只是为我们提供了一个存储的功能，我们也可以准备一个服务器（当然，能联网是前提了），将版本库保存到服务器上。

本文以github为例进行实验。




***1. 在github上创建一个仓库***

这个非常简单，参考[http://blog.csdn.net/xiahouzuoxin/article/details/9393119](http://blog.csdn.net/xiahouzuoxin/article/details/9393119)

本文创建后的仓库HTTP格式地址为：[https://github.com/xiahouzuoxin/mp3-edcode.git](https://github.com/xiahouzuoxin/mp3-edcode)

本文创建后的仓库SSH格式地址为：[git@github.com:xiahouzuoxin/mp3-edcode.git]()


本文使用SSH格式地址，创建仓库后的地址先记住：一般是git@github.com:[user-name]/[repository-name].git格式。




***2. 推送本地工程到github仓库***


进入到本地工程目录。依次执行：

git init                   # 本地仓库初始化，执行完后会在工程目录下生成一个.git的隐藏目录

git add **.**               # 添加所有文件到本地索引，命令用法：git add <file>

git commit -m "My first commit operation"   # 提交修改到本地仓库，-m选项添加提交注释

git remote add origin git@github.com:xiahouzuoxin/mp3-encode.git   # 添加远程仓库地址，保存在origin变量中


git push origin master      # 按照前一条命令中origin给定的github地址推送到github仓库的master分支




**Notes：**

（1）在git commit时可以为本次提交添加注释，注释将在使用git log查看提交日志时可以看见，这有助于分辨不同提交的更改。

commit注释最好以一行短句子作为开头，来简要描述一下这次commit所作的修改(最好不要超过50个字符)；然后空一行再把详细的注释写清楚。这样就可以很方便的用工具把commit释变成email通知，第一行作为标题，剩下的部分就作email的正文。

（2）要习惯在工程中使用README文件，文件名可以是README.txt或README.md，README的信息能被github自动识别并显示在项目主页。

README要简明扼要地描述：项目的目的，已有的功能，源代码的结构及使用，参考文献/资料。

README.md文件的语法格式参见[https://github.com/guoyunsky/Markdown-Chinese-Demo](https://github.com/guoyunsky/Markdown-Chinese-Demo)

（3）LINENSE信息

这个看大家的喜好了，喜欢开源的筒子们喜欢整一个。

（4）忽略一些文件，不提交的方法


在仓库根目录下创建名称为“.gitignore”的文件，写入不需要的文件夹名或文件，每个元素占一行即可，如


Debug

Release




***附录（常用命令）***

**Local：**

git clone git@github.com:xiahouzuoxin/mp3-encode.git        # 在本地克隆一个github上仓库

git status                    # 获得当前项目的一个状况

git commit -a              # 将修改文件（不包括新创建的文件）添加到索引，并提交到仓库

git add [file]                # 添加文件到本地索引

git branch                  # 获得当前仓库中所有分支列表

git branch zx-branch                # 新建本地一个名为zx-branch的分支，主分支名为master

git branch -D branch_name     # 删除名称为branch-name的本地分支


git checkout master                  # 切回主分支，切换到zx-branch只需要将master改成zx-branch

git log                                        # 查看提交日志，有许多附加参数

    git log -p                               # 显示补丁

    git log --stat                          # 日志统计：那些文件修改了，修改了多少行内容

    git log --graph                       # 使日志看上去更漂亮

git diff master**..**zx-branch           # 比较两个分支之间差异


git
 remote rm origin                   #  删除origin变量地址





git
 branch [name]                 # 创建本地分支，注意新分支创建后不会自动切换为当前分支

git checkout [name]              # 切换到name分支

git checkout -b
 [name]          # 创建name分支并切换到name分支




git merge [name]
                  # 将name分支与当前分支合并，name可以是远程分支，如origin/master




**Remote:**

git push origin [name]          # 创建远程name分支

git push origin**:**zx-branch      # 删除远程origin仓库地址的zx-branch分支 

git branch -r                         # 获得当前仓库中所有分支列表，即查看远程分支




注：附录所注命令将根据自己使用体会持续更新。




**Refrences：**

**[1]** Git Community Book 中文版，[http://gitbook.liuhui998.com](http://gitbook.liuhui998.com)

**[2] **GotGithub: [http://www.worldhello.net/gotgithub/index.html](http://www.worldhello.net/gotgithub/index.html)


**[3] **廖雪峰Git教程：[http://www.liaoxuefeng.com](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)




