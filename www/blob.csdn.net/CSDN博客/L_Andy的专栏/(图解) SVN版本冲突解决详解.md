# (图解) SVN版本冲突解决详解 - L_Andy的专栏 - CSDN博客

2013年04月15日 17:05:27[卡哥](https://me.csdn.net/L_Andy)阅读数：1203



**版本冲突原因： **

假设 A 、 B 两个用户都在版本号为 100 的时候，更新了 kingtuns.txt 这个文件， A 用户在修改完成之后提交 kingtuns.txt 到服务器，这个时候提交成功，这个时候 kingtuns.txt 文件的版本号已经变成 101 了。同时 B 用户在版本号为 100 的 kingtuns.txt 文件上作修改，修改完成之后提交到服务器时，由于不是在当前最新的 101 版本上作的修改，所以导致提交失败。

**版本冲突现象： **

冲突发生时， subversion 会在当前工作目录中保存所有的目标文件版本 [ 上次更新版本、当前获取的版本 ( 即别人提交的版本 ) 、自己更新的版本、目标文件 ] 。

假设文件名是 kingtuns.txt 

对应的文件名分别是：

kingtuns.txt.r101

kingtuns.txt.r102

kingtuns.txt.mine

kingtuns.txt 。同时在目标文件中标记来自不同用户的更改。 

**版本冲突解决： **

场景：

1 、现在 A 、 B 两个用户都更新 kingtuns.txt 文件到本地。 

![1](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/001.jpg)

2 、文档中原始文件内容如下： 

![2](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/002.jpg)

3 、 A 用户修改文件，添加内容“ A 用户修改内容”完成后提交到服务器 

![3](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/003.jpg)

![4](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/004.jpg)


4 、 B 用户修改文件，添加内容“ B 用户修改内容”完成后提交到服务器 

![5](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/005.jpg)

B 用户提交更新至服务器时提示如下： 

![6](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/006.jpg)

B 用户将文件提交至服务器时，提示版本过期：首先应该从版本库更新版本，然后去解决冲突，冲突解决后要执行 svn resolved （解决），然后在签入到版本库。在冲突解决之后，需要使用 svn resolved （解决）来告诉 subversion 冲突解决，这样才能提交更新。

解决冲突有三种选择：

A 、放弃自己的更新，使用 svn revert （回滚），然后提交。在这种方式下不需要使用 svn resolved （解决）

B 、放弃自己的更新，使用别人的更新。使用最新获取的版本覆盖目标文件，执行 resolved filename 并提交 ( 选择文件 — 右键 — 解决 ) 。

C 、手动解决：冲突发生时，通过和其他用户沟通之后，手动更新目标文件。然后执行 resolved filename 来解除冲突，最后提交。

解决步骤如下：

1、在当前目录下执行“ update ”（更新）操作

![7](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/007.jpg)

2、在冲突的文件上（选中文件 -- 右键菜单 —TortoiseSVN—Edit conflicts （解决冲突）），出现如下窗口

Theirs 窗口为服务器上当前最新版本 

Mine 窗口为本地修改后的版本 

Merged 窗口为合并后的文件内容显示 

![8](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/008.jpg)

3、如果要使用服务器版本，在 Theirs 窗口选中差异内容，右键，选择 Use this text block （使用这段文本块）。

同理如果要使用本地版本，在协商后，在 Mine 窗口右键，选择 Use this text block （使用这段文本块）。

![9](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/009.jpg)

4、修改完成后，保存 kingtuns.txt 文件内容。

5、在 B 用户的冲突目录下，选中文件 -- 右键菜单 —TortoiseSVN—Resolved （解决）。会列出冲突的文件列表，如果确认已经解决，点 OK 。

![10](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/010.jpg)

6、冲突解决

![11](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/011.jpg)

7 、提交解决冲突后的文件。 

![12](https://p-blog.csdn.net/images/p_blog_csdn_net/windone0109/EntryImages/20091123/012.jpg)

如何降低冲突解决的复杂度：

1 、当文档编辑完成后，尽快提交，频繁的提交 / 更新可以降低在冲突发生的概率，以及发生时解决冲突的复杂度。

2 、在提交时，写上明确的 message ，方便以后查找用户更新的原因，毕竟随着时间的推移，对当初更新的原因有可能会遗忘

3 、养成良好的使用习惯，使用 SVN 时每次都是先提交，后更新。每天早上打开后，首先要从版本库获取最新版本。每天下班前必须将已经编辑过的文档都提交到版本库。

