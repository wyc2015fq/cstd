# 关于Ubuntu中passwd、shadow、group等文件 - weixin_33985507的博客 - CSDN博客
2014年11月24日 23:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
在Ubuntu系统中，/etc目录下，有三个文件：passwd shadow group，可能我们已经在用了，但是没有注意到其详细。
这三个配置**文件**用于系统帐号管理，都是文本**文件**，可用vi等文本编辑器打开。/etc/passwd用于存放用户帐号信息，/etc/shadow 用于存放每个用户加密的密码，/etc/group用于存放用户的组信息。
**1. /etc/passwd**
/etc/passwd**文件**的内容如下：
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
sys:x:3:3:sys:/dev:/bin/sh
sync:x:4::sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/bin/sh
...
每一行是由分号分隔的字串组成，它的格式如下：
username:password:uid:gid:gecos:homedir:shell
各域对应的中文说明如下:
用户名:密码:用户ID:组ID:用户全名:主目录:登录shell
gecos是通用电子计算机操作系统的缩写，是Bell实验室中的一台大型主机。
Unix系统最初是用明文保存密码的，后来由于安全的考虑，采用crypt()算法加密密码并存放在/etc/passwd**文件**。现在，由 于计算机处理能力的提高，使密码破解变得越来越容易。/etc/passwd**文件**是所有合法用户都可访问的，大家都可互相看到密码的加密字符串，这给系统 带来很大的安全威胁。现代的Unix系统使用影子密码系统，它把密码从/etc/pa sswd**文件**中分离出来，真正的密码保存在/etc/shadow**文件**中，shadow**文件**只能由超级用户访问。这样入侵者就不能获得加密密码串，用于破 解。使用shadow密码**文件**后，/etc/passwd**文件**中所有帐户的password域的内容为"x"，如果password域的内容为"*"，则 该帐号被停用。使用passwd这个程序可修改用户的密。
**2. /etc/shadow**
/etc/shadow存放加密的口令，该**文件**只能由root读取和修改。下面是shadow**文件**的内容：
root:$1$43ZR5j08$kuduq1uH36ihQuiqUGi/E9::0::7:::
daemon:*::0::7:::
bin:*::0::7:::
sys:*::0::7:::
sync:*::0::7:::
...
我们可用chage命令显示test用户的帐号信息：
debian:~# chage -l test
最小： 0
最大：
警告日： 7
失效日： -1
最后修改： 7月 09 2005
密码过期： 从不
密码失效： 从不
帐户过期： 从不
/etc/shadow**文件**的格式如下：
username:password:last_change:min_change:max_change:warm:failed_expire:expiration:reserved
各字段的简要说明：
last_change：表示自从Linux使用以来，口令被修改的天数。可用chage -d命令修改。
min_change：表示口令的最小修改间隔。可用chage -m命令修改。
max_change：
它的格式如下：
groupname:password:gid:members
这里的password代表组口令，很少用到。它可使原先不在这个群组中的用户可以通过newgrp命令暂时继承该组的权限，使用 newgrp命令时会新开一个shell。口令的加密方式和passwd**文件**中的口令一样，所以如果需设置组口令，要用passwd程序虚设一个用户，再 把该用户password节中的加密口令拷贝到/etc/group**文件**中。members列代表组成员，我们可把需加入该组的用户以逗号分隔添加到这里 即可。同一组的成员可继承该组所拥有的权限。
这样，用户帐户本身在 /**etc**/passwd 中定义。Linux 系统包含一个 /**etc**/passwd 的同伴**文件**，叫做 /**etc**/**shadow**。该**文件**不像 /**etc**/passwd，只有对于 root 用户来说是可读的，并且包含加密的密码信息。我们来看一看 /**etc**/**shadow** 的一个样本行：
CODE:
drobbins:$1$1234567890123456789012345678901:11664:0:-1:-1:-1:-1:0
每一行给一个特殊帐户定义密码信息，同样的，每个字段用 : 隔开。第一个字段定义与这个 **shadow** 条目相关联的特殊用户帐户。第二个字段包含一个加密的密码。其余的字段在下表中描述： 
字段 3 自 1/1/1970 起，密码被修改的天数 
字段 4 密码将被允许修改之前的天数（0 表示“可在任何时间修改”） 
字段 5 系统将强制用户修改为新密码之前的天数（1 表示“永远都不能修改”） 
字段 6 密码过期之前，用户将被警告过期的天数（-1 表示“没有警告”） 
字段 7 密码过期之后，系统自动禁用帐户的天数（-1 表示“永远不会禁用”） 
字段 8 该帐户被禁用的天数（-1 表示“该帐户被启用”） 
字段 9 保留供将来使用
shadow 是 passwd 的影子**文件**。
在linux中，口令**文件**在/etc/passwd中，早期的这个**文件**直接存放加密后的密码，前两位是"盐"值，是一个随机数，后面跟的是加密的密码。为了安全，现在的linux都提供了 /etc/shadow这个影子**文件**，密码放在这个**文件**里面，并且是只有root可读的。
/etc/passwd**文件**的每个条目有7个域，分别是名字：密码：用户id：组id：用户信息：主目录：shell 
例如：ynguo:x:509:510::/home/ynguo:/bin/bash 
在利用了shadow**文件**的情况下，密码用一个x表示，普通用户看不到任何密码信息。影子口令**文件**保存加密的口令；/etc/passwd**文件**中的密码全部变成x。Shadow只能是root可读，从而保证了安全。
/etc/shadow**文件**每一行的格式如下：用户名：加密口令：上一次修改的时间（从1970年1月1日起的天数）：口令在两次修改间的最小天数：口令修改之前向用户发出警告的天数：口令终止后账号被禁用的天数：从1970年1月1日起账号被禁用的天数：保留域。
例如：root:$1$t4sFPHBq$JXgSGgvkgBDD/D7FVVBBm0:11037:0:99999:7:-1:-1:1075498172
**3. /etc/group**
/**etc**/**group****文件**是用户组的配置**文件****，**内容包括用户和用户组**，**并且能显示出用户是归属哪个用户组或哪几个用户组**，**因为一个用户可以归属一个或多个不同的用户组；同一用 户组的用户之间具有相似的特征。比如我们把某一用户加入到root用户组**，**那么这个用户就可以浏览root用户家目录的**文件****，**如果root用户把某个**文件** 的读写执行权限开放**，**root用户组的所有用户都可以修改此**文件****，**如果是可执行的**文件**（比如脚本）**，**root用户组的用户也是可以执行的； 用户组的特性在系统管理中为系统管理员提供了极大的方便**，**但安全性也是值得关注的**，**如某个用户下有对系统管理有最重要的内容**，**最好让用户拥有独立的用户 组**，**或者是把用户下的**文件**的权限设置为完全私有；另外root用户组一般不要轻易把普通用户加入进去.
**/**etc**/**group** 内容具体分析**
/**etc**/**group** 的内容包括用户组（**Group**）、用户组口令、GID及该用户组所包含的用户（User）**，**每个用户组一条记录；格式如下：
> 
`**group**_name:passwd:GID:user_list`
在/**etc**/**group** 中的每条记录分四个字段：
第一字段：用户组名称；
第二字段：用户组密码；
第三字段：GID
第四字段：用户列表**，**每个用户之间用**,**号分割；本字段可以为空；如果字段为空表示用户组为GID的用户名； 我们举个例子：
```
root:x:0:root**,**linuxsir 注：用户组root**，**x是密码段**，**表示没有设置密码**，**GID是0**,**root用户组下包括root、linuxsir以及GID为0的其它用户（可以通过/**etc**/passwd查看）；
beinan:x:500:linuxsir 注：用户组beinan**，**x是密码段**，**表示没有设置密码**，**GID是500**,**beinan用户组下包括linuxsir用户及GID为500的用户（可以通过/**etc**/passwd查看）；
linuxsir:x:502:linuxsir 注：用户组linuxsir**，**x是密码段**，**表示没有设置密码**，**GID是502**,**linuxsir用户组下包用户linuxsir及GID为502的用户（可以通过/**etc**/passwd查看）；
helloer:x:503:   注：用户组helloer**，**x是密码段**，**表示没有设置密码**，**GID是503**,**helloer用户组下包括GID为503的用户**，**可以通过/**etc**/passwd查看；
```
而/**etc**/passwd 对应的相关的记录为：
> 
```
root:x:0:0:root:/root:/bin/bash
beinan:x:500:500:beinan sun:/home/beinan:/bin/bash
linuxsir:x:505:502:linuxsir open**,**linuxsir office**,**13898667715:/home/linuxsir:/bin/bash
helloer:x:502:503::/home/helloer:/bin/bash
```
由此可以看出helloer用户组包括 helloer用户；所以我们查看一个用户组所拥有的用户**，**可以通过对比/**etc**/passwd和/**etc**/**group**来得到；
**关于GID ；**
GID和UID类似**，**是一个正整数或0**，**GID从0开始**，**GID为0的组让系统付予给root用户组；系统会预留一些较靠前的GID给系统虚拟用户（也被 称为伪装用户）之用；每个系统预留的GID都有所不同**，**比如Fedora 预留了500个**，**我们添加新用户组时**，**用户组是从500开始的；而Slackware 是把前100个GID预留**，**新添加的用户组是从100开始；查看系统添加用户组默认的GID范围应该查看 /**etc**/login.defs 中的 GID_MIN 和GID_MAX 值； 我们可以对照/**etc**/passwd和/**etc**/**group** 两个**文件**；我们会发现有默认用户组之说；我们在 /**etc**/passwd 中的每条用户记录会发现用户默认的GID ；在/**etc**/**group**中**，**我们也会发现每个用户组下有多少个用户；在创建目录和**文件**时**，**会使用默认的用户组；我们还是举个例子； 比如我把linuxsir 加为root用户组**，**在/**etc**/passwd 和/**etc**/**group** 中的记录相关记录为：
linuxsir用户在 /**etc**/passwd 中的记录；我们在这条记录中看到**，**linuxsir用户默认的GID为502；而502的GID 在/**etc**/**group**中查到是linuxsir用户组；
`linuxsir:x:505:502:linuxsir open**,**linuxsir office**,**13898667715:/home/linuxsir:/bin/bash`
linuxsir 用户在 /**etc**/**group** 中的相关记录；在这里**，**我们看到linuxsir用户组的GID 为502**，**而linuxsir 用户归属为root、beinan用户组；
```
root:x:0:root**,**linuxsir
beinan:x:500:linuxsir
linuxsir:x:502:linuxsir
```
我们用linuxsir 来创建一个目录**，**以观察linuxsir用户创建目录的权限归属；
```
[linuxsir@localhost ~]$ mkdir testdir
[linuxsir@localhost ~]$ ls -lh
总用量 4.0K
drwxrwxr-x  2 linuxsir linuxsir 4.0K 10月 17 11:42 testdir
```
通过我们用linuxsir 来创建目录时发现**，**testdir的权限归属仍然是linuxsir用户和linuxsir用户组的；而没有归属root和beinan用户组**，**明白了吧； 但值得注意的是**，**判断用户的访问权限时**，**默认的GID 并不是最重要的**，**只要一个目录让同组用户可以访问的权限**，**那么同组用户就可以拥有该目录的访问权**，**在这时用户的默认GID 并不是最重要的；
**4. /etc/gshadow**
**/etc/gshadow 解说；**
/etc/gshadow是/etc/group的加密资讯**文件**，比如用户组（Group）管理密码就是存放在这个**文件**。/etc/gshadow和/etc/group是互补的两个**文件**；对于大型服务器，针对很多用户和组，定制一些关系结构比较复杂的权限模型，设置用户组密码是极有必要的。比如我们不想让一些非用户组成员永久拥有用户组的权限和特性，这时我们可以通过密码验证的方式来让某些用户临时拥有一些用户组特性，这时就要用到用户组密码；
/etc/gshadow 格式如下，每个用户组独占一行；
`groupname:password:admin,admin,...:member,member,...`
**第一字段：**用户组
**第二字段：**用户组密码，这个段可以是空的或!，如果是空的或有!，表示没有密码；
**第三字段：**用户组管理者，这个字段也可为空，如果有多个用户组管理者，用,号分割；
**第四字段：**组成员，如果有多个成员，用,号分割；
举例：
```
beinan:!::linuxsir
linuxsir:oUS/q7NH75RhQ::linuxsir
```
第一字段：这个例子中，有两个用户组beinan用linuxsir
第二字段：用户组的密码，beinan用户组无密码；linuxsir用户组有已经，已经加密；
第三字段：用户组管理者，两者都为空；
第四字段：beinan用户组所拥有的成员是linuxsir ，然后还要对照一下/etc/group和/etc/passwd查看是否还有其它用户，一般默认添加的用户，有时同时也会创建用户组和用户名同名称； linuxsir 用户组有成员linuxisir；
如何设置用户组的密码？ 我们可以通过 gpasswd来实现；不过一般的情况下，没有必要设置用户组的密码；不过自己实践一下也有必要；下面是一个为linuxsir用户组设置密码的例子；
`gpasswd 的用法：` gpasswd 用户组
 root@localhost ~]# gpasswdlinuxsir
```
正在修改 linuxsir 组的密码
新密码：
请重新输入新密码：
```
**newgrp**
用户组之间的切换，应该用 newgrp ，这个有点象用户之间切换的su ；我先举个例子：
```
[beinan@localhost ~]$ newgrp linuxsir
密码：
[beinan@localhost ~]$ mkdir lingroup
[beinan@localhost ~]$ ls -ld lingroup/
drwxr-xr-x  2 beinan linuxsir 4096 10月 18 15:56lingroup/
[beinan@localhost ~]$ newgrp beinan
[beinan@localhost ~]$ mkdir beinangrouptest
[beinan@localhost ~]$ ls -ld beinangrouptest
drwxrwxr-x  2 beinan beinan 4096 10月 18 15:56beinangrouptest
```
说明：我是以beinan用户组切换到linuxsir用户组，并且建了一个目录，然后再切换回beinan用户组，又建了一个目录，请观察两个目录属用户组的不同；还是自己体会吧；
