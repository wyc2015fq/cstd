# 通过可写文件获取 Linux root 权限的 5 种方法 - 文章 - 伯乐在线
原文出处： [Raj Chandel](http://www.hackingarticles.in/multiple-ways-to-get-root-through-writable-file/)   译文出处：[ang010ela](http://www.4hou.com/technology/12134.html)
Linux系统中，全部都是以文件形式存在的，包括目录、设备都是有权限的，共有读、写、可执行三种。管理员为文件设置好权限后，应该要考虑哪些Linux用户会被允许和限制上述的三个权限。
通过可写脚本进行root提取的5种方法：
**· **复制 /bin/sh 到 /tmp
**· **设定 /bin/dash的SUID位
**· **通过sudoer给登录用户完全的权限
**· **设定/bin/cp的SUID位
**· **逆向连接到恶意代码
开启攻击机器，黑掉目标系统，然后进行权限提升。假设成功地通过ssh登录到受害者的机器，并可以访问非root的用户终端。然后使用下面的命令，下面会举例所有有写权限的二进制文件。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/1e7977c9bc9757738b3a866488ea5d62.png)
可以看到一个/lib/log中保存的python文件，在路径中我们看到了sanitizer.py文件的权限为777。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/964f0d836515a1f465fa5dc271d2f0a1.png)
Admin要将下面的脚本加入，来清理/tmp中的垃圾文件。如果攻击者能够识别受害者机器中的这类情形，就可以通过下面的方式来提升root权限来破坏系统。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/39214efb2063fa48217699186079c317.png)
**第一种方法**
有许多的方法可以获取root权限，本方法中，我们将/bin/sh复制到/tmp文件夹中，然后设置/tmp/sh的SUID。这种方式非常简单，首先，通过nano编辑器打开文件，然后用rm -r /tmp/* 替换下面的命令：
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/dcf60a5fe3c007b80794b0c24ce5f6af.png)
在/tmp目录创建一个有SUID权限的sh文件后，允许sh文件时会有root访问权限。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/0d5093e8d17512913d21f495b117af49.png)
可以通过下面的图片进行确认：
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/205448a574dc96c889344dc9dafdd8cb.png)
**第二种方法**
同样地，可以用rm -r /tmp/* 替换下面行的内容
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/bed4185b31699ce45bfaf3c5619ec88e.png)
在设置了/bin/dash的SUID权限后，运行后就可以获取root权限
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/34b3b42d097322d3fe33db6881f6dc11.png)
可以通过下面的图进行确认：
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/a44576fbd2965f44814965d14a2c9baf.png)
**第三种方法**
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/1472d7a6a5b70fd0a5caa935b6495cd7.png)
通过netcat逆向了连接后，就可以获取root权限。
可以通过下面的图进行确认：
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/297bcaf946124ac842925a0ac6a82d09.png)
**第4种方法**
另一个方法是给登录的用户sudo权限。下面的图中可以看出当前用户wernerbrandes不能允许sudo命令。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/253ff8bf45823dc36d7dfd382bdd1ad5.png)
同样地，可以在下面替换rm -r /tmp/*
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/2526a8e7e2c6b56429b1401b51f05ecd.png)
当输入“sudo -l”命令时会发现，这是sudo用户的一个成员。可以利用sudo bash来获取root权限。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/c7880f8f99521d379c511cead4b8d7a0.png)
**第5种方法**
因为在linux类系统中，passwd文件起着很重要的作用。所以，如果攻击者有机会修改passwd文件，那么这将会成为一种动态的权限提升的方式。
同样地，可以利用cat命令查看etc/passwd文件的内容。
UID:1000 & GID:1000 就是admin组队成员。下面编辑一下nemo记录来使其成为root组成员，选择etc/passwd的整个内容并复制粘贴到空text文件中。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/c0fac052f49554a228dd7921afb830fb.png)
然后，在一个新的终端上生成一个含salt的密码，然后复制。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/1aa9b7caf0e0a26b773e5ddc1dd36a89.png)
然后粘贴之前复制的含salt的密码在用户nemo的记录词条的X位置处，并修改UID&GID为0:0。完成上面的步骤后，我们就可以将其保存为passwd。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/39851bc754c3333d3391754370ca9198.png)
利用可写的脚本替换 “rm -r /tmp/*”
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/1ac119b231c99b98f495d4b68d07401c.png)
设置/bin/cp的SUID来复制文件。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/7f7a038894570cdee6341602844bb360.png)
将修改后的passwd文件下载受害者机器的/tmp文件夹中。用下面的命令检查/bin/cp的SUID位是否开启。
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/a8322a3d2072ceb1a4b10099f03dec36.png)
下面确认是否改变了passwd文件的内容：
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/d2e4f2d6ee16c639cfcc3fe0516161f5.png)
可以看出passwd文件中的变化：
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/0dae580655080853330e9dc34bf88e6e.png)
可以执行下面的命令来获取root权限：
![image.png](http://jbcdn2.b0.upaiyun.com/2018/06/19e39ae621feb388e358cd0783dd0cff.png)
本文证明了攻击者如何通过可写文件进行linux系统权限提升。
