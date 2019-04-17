# git 怎么用命令发送patch补丁、send-email下载和配置 - V__KING__的专栏 - CSDN博客





2013年11月04日 12:35:22[v__king__](https://me.csdn.net/V__KING__)阅读数：3178










本文是在Ubunt 12.04环境下测试的。


1、安装必要的软件


- # apt-get install git git-core
 git-email




2、配置send-email的环境，主要是设置本地email的客户端，用msmtp


vim ~/.msmtprc



- # default

- 
account gmail

- 
protocol smtp

- 
host smtp.gmail.com

- 
from user@example.com

- 
user user@example.com

- 
password ******

- 
port 587 

- 
auth on

- 
tls on

- 
tls_trust_file /etc/ssl/certs/ca-certificates.crt

- 
syslog LOG_MAIL

- 

- 
# set a default account

- 
account default : gmail





3、设置git send-email的配置

vim ~/.gitconfig


- [user]

- 
    name = your name

- 
    email = user@example.com

- 

- [sendemail]

- 
        smtpencryption = tls 

- 
        smtpuser = user@example.com

- 
        smtpserverport = 587 

- 
        smtpserver = smtp.gmail.com 

- 
# to= user@example.com

- 
# cc = user@example.com

- 
# from = user@example.com




其中，上面配置文件的最后三行在git send-email发送补丁文件时也可以通过命令行指定。建议大家最好通过命令行指定，因为内核补丁文件需要发送给相关模块的maintainer。



4、下面就以实例来说明如果制作一个符合规范的内核补丁。

Linux内核补丁的制作规范和发送格式参见内核文档Documentation/SubmittingPatches。

a、修改内核



- diff --git a/net/sunrpc/auth_null.c
 b/net/sunrpc/auth_null.c

- 
index a5c36c0..f8e0d73 100644

- --- a/net/sunrpc/auth_null.c

- +++ b/net/sunrpc/auth_null.c

- 
@@ -27,6 +27,7
 @@ nul_create(struct rpc_clnt *clnt, rpc_authflavor_t flavor)

- 
 static void

- 
 nul_destroy(struct rpc_auth *auth)

- {

- + pr_info("hellor world!\n");

- }





b、提交修改


- git commit -a -s -m"test"-e





其中，-a表示all，-s表示在补丁文件头部增加Signed-off-by行，-m表示指定commit信息，-e表示使用git默认的编辑器来对commit信息进一步编辑，我这里默认的编辑器是nano。关于更详细的参数选项可以用命令git commit -h来察看。


执行上面那个命令以后，会进入到nano编辑器中，然后进行编辑。其中第一行显示的是补丁文件头部的Subject信息，第二行文字显示的是补丁文件的注释信息。编辑完以后，ctrl-o保存，ctrl-x退出。  
[](http://blog.chinaunix.net/download/file.php?id=851&mode=view)![](http://blog.chinaunix.net/attachment/201303/28/28453894_1364482378jMAp.png)


c、生成补丁文件[](http://blog.chinaunix.net/download/file.php?id=851&mode=view)

- git format-patch -1


其中，-1表示生成一个补丁文件，即最后git commit时的修改。用vim打开补丁文件，如下所示：

![](http://blog.chinaunix.net/attachment/201303/28/28453894_1364482421t46s.png)


之所以要有Subject，Signed-off-by文件，那是因为内核文档Documentation/SubmittingPatches文件中有说明：


15) The canonical patch format


The canonical patch subject line is: 


    Subject: [PATCH 001/123] subsystem: summary phrase


The canonical patch message body contains the following:


- A "from" line specifying the patch author.


- An empty line.


- The body of the explanation, which will be copied to the 

    permanent changelog to describe this patch.


- The "Signed-off-by:" lines, described above, which will

    also go in the changelog.


- A marker line containing simply "---".


- Any additional comments not suitable for the changelog.


- The actual patch (diff output).


d、检测补丁

制作完补丁文件以后，不要急着把补丁文件mail出去，需要先检测一下这个补丁的正确性。内核中提供了一个补丁检测脚本。在内核源代码根目录下，补丁检测如下：


- # ./scripts/checkpatch.pl
 0001-nfs-add-a-pr_info.patch 

- 
total: 0 errors, 0 warnings, 7 lines checked

- 

- 
0001-nfs-add-a-pr_info.patch
 has no obvious style problems andis ready for submission.




e、确定补丁发送给谁

补丁检测通过以后，就可以mail出去了。但是该mail给谁呢？嗯，内核里面也同样提供了一个脚本，用于检测某个目录的maintainer是谁。以我修改的net/sunrpc目录里面的文件为例，执行下面的命令：



- root@bandari-PC:~/code/linux-3.5.4# ./scripts/get_maintainer.pl
 net/sunrpc/-f

- 
Trond Myklebust <Trond.Myklebust@netapp.com>(maintainer:NFS, SUNRPC,AND...)

- "J. Bruce Fields"<bfields@fieldses.org>(supporter:KERNEL
 NFSD, SUNR...)

- "David S. Miller"<davem@davemloft.net>(maintainer:NETWORKING [GENERAL])

- 
linux-nfs@vger.kernel.org (open
 list:NFS, SUNRPC,AND...)

- 
netdev@vger.kernel.org (open list:NETWORKING [GENERAL])

- 
linux-kernel@vger.kernel.org (open
 list)



从上面输出可以看到，email destination有三个人。而后面的3个则用于email carbon copy (cc)，即抄送。


5、发送补丁

命令如下所示：


- root@bandari-PC:~# git
 send-email --smtp-server /usr/bin/msmtp --to user@example.com 

- --cc user@example.com 0001-nfs-add-a-pr_info.patch




其中，git send-email的选项选项说明如下：

--to 邮件的To地址 sendmail.to

--cc 邮件的Cc地址 sendmail.cc

--smtp-server SMTP服务器名称 sendmail.smtp-server


关于git send-email的详细选项说明，可以通过执行命令git send-email -h察看。

注意，这里是通过命令行参数设置，前面说过，在~/.msmtprc文件也可以设置全局参数。

如果大家要测试的话，可以将--to和--cc设置成自己的邮箱地址。










