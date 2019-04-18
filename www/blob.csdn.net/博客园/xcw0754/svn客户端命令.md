# svn客户端命令 - xcw0754 - 博客园
# [svn客户端命令](https://www.cnblogs.com/xcw0754/p/5283049.html)
### 记几个常用的命令。
首次拉仓库时，先要进行检出（url可以带端口号）：
`svn checkout http://svn.example.com:9834/repos`
`svn checkout file://localhost/var/svn/repos`
`svn checkout file:///var/svn/repos`
附url的使用方式：
![](https://images2015.cnblogs.com/blog/641737/201604/641737-20160407102620672-191465799.png)
查看当前仓库的状态，可以指定某个文件的路径（选项-v可以查看所有文件，选项-u可以看远程的和本地的比较）:
`svn st <path>`
对仓库修改了其中的文件or目录要暂存（目录会递归添加，可以--non-recursive 或-N 来添加空目录 ）：
`svn add <files>`
对拉下来的仓库稍微修改后要暂存（-F 可以用文件内容作为日志）：
`svn commit button.c -m "Fixed a typo in button.c."`
要从暂存区域删除掉：
`svn delete <file>`
将本地仓库所有文件更新到最新状态（在update之前可以先看看svn st -u）：
`svn update`
未跟踪的新文件or目录的直接导入并commit到远程仓库的某个目录下：
`svn import mytree file:///var/svn/newrepos/some/project -m "Initial import"`
列出远程仓库的目录下的文件：
`svn list file:///var/svn/newrepos/some/project`
已经跟踪的文件需要移动or删除or拷贝or创建目录：
`svn copy` 和 `svn delete` 和 `svn move` 和 `svn mkdir`
其中move相当于copy后再delete，这些命令可能不会立刻创建文件夹，使用--parents选项可以立刻创建。
update之后可能需要解决冲突，可以先试试：
`svn resolved`
若不行，再按提示解决冲突，再add解决后的文件，再commit即可。
取消一些修改（file是可选的，会取消它的修改，很智能，要小心）：
`svn revert <file>`
查看log（-r number可以只看某版的提交，也可以直接带文件）：
`svn log`
查看区别（可以带-r选项，也可以带文件名）：
`svn diff`
版本号：
```
HEAD
版本库中最新的(或者是“最年轻的”)版本。
BASE
工作拷贝中一个条目的修订版本号，如果这个版本在本地修改了，则这里指的是这个条目
在本地未修改的版本。
COMMITTED
项目最近修改的修订版本，与BASE相同或更早。
PREV
The revision immediately before the last revision in which an item changed. Technically, this boils down to COMMITTED−1.
```
### 验证
svn默认会缓存你的账户和密码，但是是明文的。一次性的不使用该缓存，只要在命令后面加：
`--no-auth-cache`
要永久不缓存密码，可以在svn的配置文件中写：
`store-passwords = no`
要切换到其他的账户，可以如下这样，它会再提示输入密码的，这样又会被保存起来了：
`svn --username`
### 状态
使用命令`svn st`会出现一些状态：
```
A item
预定加入到版本库的文件、目录或符号链的item。
C item
文件item发生冲突，在从服务器更新时与工作拷贝(如果更新时没有解决)的本地版本发生
交迭，在你提交到版本库前，必须手工的解决冲突。
D item
文件、目录或是符号链item预定从版本库中删除。
M item
文件item的内容被修改了。
```
### 冲突
update之后会有如下的选项：
(p) 让文件在更新完成之后保持冲突状态。
(df) diff-full 使用标准区别格式显示base修订版本和冲突文件本身的区别。
(e) edit 用你喜欢的编辑器打开冲突的文件，编辑器是环境变量EDITOR设置的。
(r) resolved 完成文件编辑之后，通知svn你已经解决了文件的冲突，它必须接受当前的内容—从本质上讲就是你已经“解决了”冲突。
(mf) mine-full 丢弃新从服务器接收的变更，并只使用你查看文件的本地修改。
(tf) theirs-full 丢弃你对查看文件的本地修改，只使用从服务器新接收的变更。
(l) launch 启动一个外置程序来执行冲突解决，这需要一些预先的准备。
(h) help 显示所有在冲突解决时可能使用的命令。

