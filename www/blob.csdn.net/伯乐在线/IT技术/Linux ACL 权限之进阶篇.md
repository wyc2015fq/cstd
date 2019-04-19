# Linux ACL 权限之进阶篇 - 文章 - 伯乐在线
原文出处： [sparkdev](https://www.cnblogs.com/sparkdev/p/9694015.html)
笔者在《[Linux ACL 权限](http://blog.jobbole.com/114417/)》一文中介绍了 Linux ACL 权限的基本用法，本文笔者将尝试探究 ACL 中的基本概念和实现原理，希望能够通过进一步的加深对 Linux 权限系统的理解。说明：本文的演示环境为 ubuntu 16.04。
# ACL 中的基本概念
**ACL 的类型**
- **access ACL**：我们可以认为每一个对象(文件/目录)都可以关联一个 ACL 来控制其访问权限，这样的 ACL 被称为 access ACL。
- **default ACL**：目录也可以关联一个 ACL 来控制在该目录中创建的对象的默认 ACL，这样的 ACL(目录关联的 ACL)被称为 default ACL。
**ACL 条目**
一个 ACL 由多个 ACL 条目组成。一个 ACL 条目指定一个用户或者一组用户对所关联对象的读、写、执行权限。下图展示了 ACL 条目的类型及含义：
![](http://jbcdn2.b0.upaiyun.com/2018/09/1962993fec78ec78ddf450ab0dbbb51b.png)
# ACL 权限与 ugo 权限的对应关系
ACL 定义的权限是 ugo 权限的超集。
- 文件的 owner 权限对应于 ACL 权限中的 ACL_USER_OBJ 条目。
- 当 ACL 权限中具有 ACL_MASK 条目时，文件的 group 权限对应于 ACL 权限中的 ACL_MASK 条目。否则，当 ACL 权限中具没有 ACL_MASK 条目时，文件的 group 权限对应于 ACL 权限中的 ACL_GROUP_OBJ 条目。
- 文件的 other 权限对应于 ACL 权限中的 ACL_OTHER_OBJ 条目。
文件的 ugo 权限总是与对应的 ACL 条目保持一致。修改文件的 ugo 权限会导致修改相关的 ACL 条目，同样的，修改这些 ACL 条目会导致修改对应的 guo 权限。
# 新建文件的 default ACL
一个文件的 access ACL 会在通过 creat()、mkdir()、mknod()、mkfifo() 和 open() 函数创建该文件时被初始化。
**如果一个目录被设置了 default ACL**，那么将会由文件创建函数的 mode 参数和目录的 default ACL 共通决定新文件的 ACL 权限：
- 新的文件继承父目录的 default ACL 作为自己的 access ACL。
- 修改与 ugo 权限对应的 access ACL 条目，使其不包含文件创建函数的 mode 参数不包含的权限。
说明：此时 umask 被忽略。
**如果一个目录没有被设置 default ACL**，那么将由文件创建函数的 mode 参数和 umask 共同决定新文件的 ACL 权限：
- 新建文件的 access ACL 包含 ACL_USER_OBJ, ACL_GROUP_OBJ, 和 ACL_OTHER 条目。这些条目的权限被设置为由 umask 决定的权限。
- 修改与 ugo 权限对应的 access ACL 条目，使其不包含文件创建函数的 mode 参数不包含的权限。
# 文件权限检查的算法(Access Check Algorithm)
当一个进程访问(读、写、执行)一个被 ACL 保护的文件时，文件权限检查的算法决定了是否授权给进程访问该文件。
下面我们以 下面我们以伪代码的方式来解释文件权限检查的算法。
第一步：**if** 进程的 effective user ID 与文件 owner 匹配
if ACL 的 ACL_USER_OBJ 条目包含了请求所需的权限，此时就被授权访问文件
else 访问被拒绝
第二步：**else if** 进程的 effective user ID 匹配文件 ACL 权限中任何一个 ACL_USER 条目中的 user
if 匹配的 ACL_USER 条目和 ACL_MASK 条目包含了请求所需的权限，此时就被授权访问文件
else 访问被拒绝
第三步：**else if** 进程的 effective group ID 或者任何一个补充的(supplementary) group ID 匹配文件的 group 或 ACL 权限中任何一个 ACL_GROUP 条目的 group
if ACL 权限包含 ACL_MASK 条目
if ACL_MASK 条目和匹配的任何 ACL_GROUP_OBJ 或 ACL_GROUP 条目包含了请求所需的权限，此时就被授权访问文件
(注释：ACL_MASK 与其它项是 and 的关系，用来控制最大权限)
else 访问被拒绝
else (注意：**没有 ACL_MASK 条目，就没有 ACL_GROUP 条目**)
if ACL_GROUP_OBJ 条目包含了请求所需的权限，此时就被授权访问文件
else 访问被拒绝
第四步：**else if** ACL_OTHER 条目包含了请求所需的权限，此时就被授权访问文件
第五步：**else **访问被拒绝
# ACL 的文本描述格式
有两种格式来描述 ACL 条目，分别是长格式和短格式。它们非常类似，都是通过两个冒号把一个 ACL 条目分为三个部分：
**ACL 条目的类型:ACL 条目 qualifier:权限信息**
我们在前面已经介绍过 ACL 条目的类型，权限信息就是用 rwx 来表示的信息，不支持某个权限的话可以使用 – 表示。这里介绍一下 ACL 条目 qualifier(不知道该咋翻译这货)。
- 当 ACL 条目的类型为 ACL_USER 或 ACL_GROUP 时，ACL 条目 qualifier 包含与 ACL 条目关联的用户和组的标识符。
- 当 ACL 条目的类型为其它时，ACL 条目 qualifier 为空。
其中的用户标识符可以是用户名也可以是 user ID，组标识符可以是组名也可以是 group ID。
**下面是一组长格式的示例：**
user::rw-
user:tester:rw- #effective:r–
group::r–
group:tester1:rw- #effective:r–
mask::r–
other::r–
**下面是一组短格式的示例：**
u::rw-,u:tester:rw-,g::r–,g:tester1:rw-,m::r–,o::r–
g:tester1:rw,u:tester:rw,u::wr,g::r,o::r,m::r
# 解释几个常见的权限变化的例子
前文我们的重点是介绍 ACL 权限的基本用法。有了本文前面介绍的基础内容，我们就可以解释前文中出现的一些比较怪异的现象。
创建用户 tester, tester1：

```
$ sudo adduser tester
$ sudo adduser tester1
```
先创建文件 aclfile，检查其默认的 ACL 权限信息：
![](http://jbcdn2.b0.upaiyun.com/2018/09/45b4fb71d418131eaac5b36e54f85981.png)
然后为 tester 用户赋予读写 aclfile 文件的权限：

```
$ setfacl -m u:tester:rw aclfile
```
此时查看 aclfile 文件的权限：

```
$ ll aclfile
```
![](http://jbcdn2.b0.upaiyun.com/2018/09/bf5546f0663b6f7dc7bacae23af61041.png)
貌似并没有发生什么变化，只是在描述权限的地方多出了一个 “+” 号。下面再看看 acl 权限：

```
$ getfacl aclfile
```
![](http://jbcdn2.b0.upaiyun.com/2018/09/80b7c55f767e7016c8465bcd6f2c7916.png)
上图的黄框中出现了 ACL_MASK 条目，这就是我们的第一个问题：
**************************************************************
**我们并没有显式的设置 ACL_MASK 条目，为什么它出现了？**
An ACL that contains entries of ACL_USER or ACL_GROUP tag types must contain exactly one entry of the ACL_MASK tag type. If an ACL contains no entries of ACL_USER or ACL_GROUP tag types, the ACL_MASK entry is optional.
上面的解释大意是当添加了 ACL_USER 或 ACL_GROUP 后，必须有一个对应的 ACL_MASK 条目。在当前的情况下，ACL_MASK 是被自动创建的，它的权限被设置成了 group(其实是 group class) 的权限即 rw-。
**************************************************************
下面我们接着更新 aclfile 的 ACL 权限：

```
$ setfacl -m u:tester:rwx,g:tester1:r aclfile
```
查看文件权限：
![](http://jbcdn2.b0.upaiyun.com/2018/09/1b72ff003cb466bc2f7948063e0c53d7.png)
在修改了 tester 的权限并添加了 tester1 group 的权限后，我们看到的组权限居然变成了 rwx ！
这是我们的第二个问题：
**************************************************************
**为什么 aclfile 文件的组权限变成了 rwx？**
这是因为我们设置了  u:tester:rwx 导致的：
在设置了 ACL 权限后，group 显示的权限为 ACL_MASK 条目中的权限。而 ACL_MASK 条目中的权限表示 ACL_USER、ACL_GROUP_OBJ 和 ACL_GROUP 条目能够被授予的最大权限，所以当 tester 被设置了 rwx 权限时，ACL_MASK 条目中的权限也发生了相应的变化。并最终导致我们看到了上面的结果：-rw-rwxr–+。
**************************************************************
接下来我们看看 acl 权限：
![](http://jbcdn2.b0.upaiyun.com/2018/09/925fecd15b9af57d69f171897afe4bff.png)
用户 tester 具有 aclfile 的读写执行权限，tester1 group 具有 aclfile 的读权限，但是这里的 mask 却变成了 rwx！
这是我们的第三个问题：
**************************************************************
**上面的设置并没有显式的指定 mask 项，为什么 mask 的值却变了？**
其实我们在第二个问题中已经回答了这个问题，是因为 tester 被设置了 rwx 权限，最终导致 ACL_MASK 条目中的权限也发生了相应的变化。
这里我们可以思考一下：ACL 权限中为什么需要 ACL_MASK 条目？
这是一个需要从长计议的话题，我们应该从 ACL 权限与 ugo 权限的对应说起。在 ugo 权限模型中，定义了 3 个 class 来表示 owner、group、other 的权限。Owner class 表示文件所有者具有的访问权限，group class 表示 owner group 具有的访问权限，other class 表示其它用户所具有的访问权限。在没有显式的设置 ACL 权限时，文件的 ACL 权限与 ugo 权限的对应关系如下图所示：
![](http://jbcdn2.b0.upaiyun.com/2018/09/40095365ae6211533ff34ad540a229c8.png)
我们把重点放在 group class 上，此时 group class 的权限和 owner group 的权限是完全一样的。
但是在我们添加了 ACL 权限之后，情况就变得有些复杂了：
![](http://jbcdn2.b0.upaiyun.com/2018/09/fb041d2ee8ea0b888a934bb2261c40ed.png)
此时 group class 中还可能包含 ACL_USER 和 ACL_GROUP 条目中的权限。这样就会出现 owner group 权限与 group class 不一致的情况。
解决的办法就是为 ACL 权限引入 ACL_MASK 条目：
- 没有设置 ACL 权限时，group class 的权限和 owner group 的权限是完全一样的。
- 设置 ACL 权限后，group class 的权限映射到了 ACL_MASK 条目的权限，ACL_GROUP_OBJ 条目仅仅用来表示 owner group 的权限。
**************************************************************
最后我们再来设置一下 aclfile 的 mask：

```
$ setfacl -m mask::r aclfile
$ getfacl aclfile
```
![](http://jbcdn2.b0.upaiyun.com/2018/09/dd3879db739d4dd8078031630f3f45c7.png)
ACL 权限的最后一道防线就是 mask 。**它决定了一个用户或组能够得到的最大的权限。**上图中的 #effective 显示了对应行的实际权限。文件权限也反应了上面的变化：
![](http://jbcdn2.b0.upaiyun.com/2018/09/69ae74644cbf8da4c83b35ff91595b9f.png)
我们的最后一个问题：
**************************************************************
**为什么需要 effective 权限？**
ACL_MASK 条目 限制的是 ACL_USER、ACL_GROUP_OBJ 和 ACL_GROUP 条目的最大权限，所以在应用了 ACL_MASK 条目后，需要通过 effective 权限来获得 ACL_USER、ACL_GROUP_OBJ 和 ACL_GROUP 条目的真正权限(如上图所示)。当 ACL_USER、ACL_GROUP_OBJ 和 ACL_GROUP 条目**包含不包含在 ACL_MASK 条目中的权限**，则该条目后面会有一个 “#” 号和字符串 “effective”，以及该条目的有效访问权限。
**但是 mask 只对 ACL_USER、ACL_GROUP_OBJ 和 ACL_GROUP 条目有影响(红框中的内容)，对 owner 和 other 的权限是没有任何影响的。**
**************************************************************
# 总结
本文先介绍了 ACL 权限中的基本概念，然后解释了笔者在使用 ACL 权限过程中碰到的一些疑问。希望这些内容可以帮助大家更好的理解和使用 ACL 权限。
**参考：**
[acl man page](https://linux.die.net/man/5/acl)
[POSIX Access Control Lists on Linux](https://web.archive.org/web/20150919060634/http:/users.suse.com/~agruen/acl/linux-acls/online/)
