# 由chmod改系统文件权限引ssh登录报ssh_exchange_identification: read: Connection reset by peer无法登陆问题 - xqhrs232的专栏 - CSDN博客
2017年07月13日 15:29:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：361
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/x6_9x/article/details/49983607](http://blog.csdn.net/x6_9x/article/details/49983607)
相关文章
1、[ssh连接失败，排错经验](http://www.cnblogs.com/starof/p/4709805.html)----[http://www.cnblogs.com/starof/p/4709805.html](http://www.cnblogs.com/starof/p/4709805.html)
2、[解决ssh_exchange_identification:read
 connection reset by peer 原因](http://yanue.net/post-163.html)----[http://yanue.net/post-163.html](http://yanue.net/post-163.html)
3、
# [ssh_exchange_identification: read: connection reset by peer](http://blog.csdn.net/chrisniu1984/article/details/7047027)----[http://blog.csdn.net/chrisniu1984/article/details/7047027](http://blog.csdn.net/chrisniu1984/article/details/7047027)
很不对劲，因为yang下文件没这么多，怎么不相关目录的权限也改了呢，感觉ctrl+c终止，仔细一看原来yang 有一个/，这是个很危险的命令，有点常识的人都知道，根目录下的文件权限不可以乱动啊，当时吓了一跳，于是另开一个终端怎么也连接不上，但是用secureCRT的clone ssh 功能就可以连接。
**[root@yang app]# chmod -R 777 yang / **
用ssh localhost 提示： 
**[test@yang .ssh]$ ssh localhost **ssh_exchange_identification: read: Connection reset by peer 
**远程连接无法连接，用下面的命令，看详细的连接过程： 用[root@yang app]# ssh -v localhost **OpenSSH_6.7p1, OpenSSL 1.0.0-fips 29 Mar 2010 
debug1: Reading configuration data /etc/ssh/ssh_config 
debug1: Connecting to localhost [::1] port 22. 
debug1: Connection established. 
debug1: permanently_set_uid: 0/0 
debug1: key_load_public: No such file or directory 
debug1: identity file /root/.ssh/id_rsa type -1 
debug1: key_load_public: No such file or directory 
debug1: identity file /root/.ssh/id_rsa-cert type -1 
debug1: key_load_public: No such file or directory 
debug1: identity file /root/.ssh/id_dsa type -1 
debug1: key_load_public: No such file or directory 
debug1: identity file /root/.ssh/id_dsa-cert type -1 
debug1: key_load_public: No such file or directory 
debug1: identity file /root/.ssh/id_ed25519 type -1 
debug1: key_load_public: No such file or directory 
debug1: identity file /root/.ssh/id_ed25519-cert type -1 
debug1: Enabling compatibility mode for protocol 2.0 
debug1: Local version string SSH-2.0-OpenSSH_6.7 
ssh_exchange_identification: read: Connection reset by peer 
于是去万能的百度谷歌，以关键词ssh_exchange_identification: read: Connection reset by peer，搜索搜到不少网友们遇到这样的问题，有的是ip被加入到限制列表中了，有的是dns无法解析主机名，有的是说误将权限改了，看到这心下一喜，英雄所见略同，这不是跟我遇到的问题一样么，于是按照帖子说的，将/etc/ssh/目录下的所有文件改成400 权限，照着做了，还是不行，报同样的错误。
还有一个老外说，什么都不用管，重启一下完事，感觉有点头大，虽然是[测试](http://lib.csdn.net/base/softwaretest)环境，不敢贸然重启，搞不好还得让主机管理员跑机房，幸好虚拟机上有个环境差不多的系统，先做了个快照，模仿同样的操作，嘿嘿，果然是同样的报错，重启下，嘿嘿，直接在管理端都登不上上了，就是你在机房也无可奈何了，对的就是用root+密码在console上登录上，幸亏没重启，不然就是你跑到机房，你和机器就算0距离接触，奈何缘分已尽，不让你登录，你能奈何，只能还虚拟机还原到原来的快照上，继续试验。 
继续百度，又跑到一个歪果仁的网站上，在一个不起眼论坛的一个不起眼的帖子中一个不起眼的跟帖中，一个哥们很低调的说了句“*I know this quesiton is old ,but I wanted to share some findings I had,Check if /var/empty/sshd on the server has appropriate ownership and permissions. We had a chef script that was modifile to update some directory peimisions,but indavertently updated the diectory below the intended target,chaning ownership of /var to an applicaton user/group and changing the permissons to 755."*去/var下看了看，果然权限很大，都是777,cd 到 empty 目录，果然有ssh这个文件夹，在cd进去，啥也没有了。于是直接执行两条命令： 
**cd /var chmod -R 755 * **然后就再次尝试了远程连接了下，竟然ok了。
看来[Linux](http://lib.csdn.net/base/linux)到处是坑啊，类似粗心的错误，有rm -f log * ,把当前目录下的东西都清空了；hostname命令带了一个空格，导致RAC崩溃了。听前辈的话没错儿，能不用root就不要用root，能不用root就不要用root，能不用root就不要用root！
# [解决ssh_exchange_identification:read connection reset by peer 原因](http://yanue.net/post-163.html)
