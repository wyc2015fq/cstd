# Git（二） - weixin_33985507的博客 - CSDN博客
2018年11月18日 23:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
#### 1.HEAD，master，branch
执行 git log 查看log
![5876940-d325d5ca7e89e386.png](https://upload-images.jianshu.io/upload_images/5876940-d325d5ca7e89e386.png)
image.png
###### 1.1commit
commit xxxxx(HEAD -> master, origin/master, origin/HEAD)
commit 后面的这些东西（例：commit xxxxx(HEAD -> master, origin/master, origin/HEAD)），这些是指向commit的引用。每个commit都有一个唯一的sha-1值。
###### 1.2HEAD：当前commit的引用
HEAD，他是指向当前commit的引用。例如上图中最新的一个commit，当有新的commit的时候，工作目录自动与最新的commit对应，同时，HEAS会指向最新的commit。其他时候，使用checkout，reset的指令手动改变当前commit的时候，HEAD也会跟上去。
HEAD，就是当前commit的引用，当前commit在哪里，HEAD就在那里。
###### 1.3Branch
Branch （分支），他也是git的一种引用。HEAD可以指向commit，他还可以指向branch，当他指向某个branch的时候，他会通过这个branch来间接的指向某个commit，当HEAD在提交时会自动向前移动，他还会带着他所指向的branch一起移动。
###### 1.4Master
他是Git默认的分支。
当新创建一个仓库的时候，他是没有任何的commit的，但是当他创建第一个commit的时候，会把master指向他，并且把HEAD也指向他，所以他当作是Git的默认的分支。
当有人从中央仓库把项目拉取下来的时候，他不仅会把.git这个仓库目录下载到工作目录中，还会迁出master分支。
###### 1.5branch的创建，切换，删除
创建：git branch 名称  或 git checkout -b 名称（创建后自动切换到新的分支）。
切换：git checkout 分支。
删除：git branch -d 名称。
#### 2 Push
Push 通俗的说就是将本地的提交上传到中央仓库去，用本地的内容来覆盖掉远端的内容。
###### 2.1其实，Push的工作是：把当前branch位置上传到中央仓库，并把他的路径上的所有的commits一并上传上去。
###### 2.2push的时候，如果当前分支是一个本地创建的分支，需要指定原创仓库名和分支名，用git push origin branch_name 的格式，而不能只用git push，或者可以通过git config 修改push.default 来改变push的行为逻辑。
###### 2.3push的时候之后上传当前分支，并不会上传HEAD；中央仓库的HEAD是永远指向默认分支的。
#### 3 merge
###### 3.1具体含义：
merge的意思是合并，他是指定一个commit，把他合并到当前的commit来。
具体来讲：就是从目标commit和当前commit（当前HEAD所指向的commit）的分叉的位置起，把目标commit的路径上的所有的commit的内容全部提交到当前commit，然后自动生成一个新的commit。
###### 3.2适用场景：最常用的场景
1.合并分支
2.pull的内部操作(pull的内部操作其实就是将远端仓库的内容fetch取下来之后，用merge合并)
###### 3.3merge的三种特殊情况
1.冲突：
原因是需要合并的两个分支同时修改了相同文件的相同位置。git无法确定该如何合并。
解决办法，手动解决冲突后手动提交（commit）
2.如果HEAD和目标commit不存在分叉，并且HEAD领先于目标commit，这个时候Git什么也不需要做，这个时候merge是一个空操作。
3.如果HEAD和目标commit还是不存在分叉，并且HEAD落后与目标commit，这个时候相当于当前HEAD快速移动。
#### 4.Feature Branching
Feature Branching 是一种工作流。
概念：
1.每一个新功能都会新建一个branch来写。
2.当写完以后，把代码分享给同事看，也可以在写的同时分享给同事。
3.当新功能的分支代码审核通过后，会把分支合并到master上，并且删除分支。
这种工作流，也同时适用与一人多任务。当自己需要做一件事的时候，先创建一个分支，当完成该任务后再将这个分支上的代码合并到自己的主分支上，并且删除刚刚创建的这个分支。这样做的话，如果中途有人打扰到你，你可以快速且切换到原来的分支上进行工作，避免了当前任务被突发事情的干扰。
#### 5.add
add指令是把改动的内容放进暂存区。
###### 5.1 如果需要add 的文件比较多，可以在add后面加“.”，表示全部暂存。
###### 5.2 add添加的是文件改动，不是文件名。
具体情况，如果你修改了某个文件内的东西，然后执行了git add  xxx ，git status
![5876940-47675e997a36f4ca.png](https://upload-images.jianshu.io/upload_images/5876940-47675e997a36f4ca.png)
image.png
如果你再次修改某文件内容，然后再执行git status
![5876940-e13aba9f8e953496.png](https://upload-images.jianshu.io/upload_images/5876940-e13aba9f8e953496.png)
image.png
也就是说，git add 他执行的是添加改动的内容，他不会添加文件名。
#### 6.log
![5876940-310605699454826c.png](https://upload-images.jianshu.io/upload_images/5876940-310605699454826c.png)
image.png
指令：git log 可以查看历史纪录
![5876940-72e876d08049e521.png](https://upload-images.jianshu.io/upload_images/5876940-72e876d08049e521.png)
image.png
指令：git log -p 查看详细历史，包括每一条commit的具体改动。
