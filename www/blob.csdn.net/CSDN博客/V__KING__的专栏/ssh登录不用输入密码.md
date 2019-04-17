# ssh登录不用输入密码 - V__KING__的专栏 - CSDN博客





2014年08月28日 11:18:45[v__king__](https://me.csdn.net/V__KING__)阅读数：827








http://www.tecmint.com/ssh-passwordless-login-using-ssh-keygen-in-5-easy-steps/



**SSH** (**Secure SHELL**) is an open source and most trusted network protocol that is used to login into remote servers for execution of commands and programs. It is also used to transfer files from one computer to another computer
 over the network using secure copy (**SCP**) Protocol.

In this article we will show you how to setup password-less login using **ssh keys** to connect to remote **Linux** servers without entering password. Using Password-less login with S**SH keys** will increase the trust between two
**Linux** servers for easy file synchronization or transfer.

![SSH Passwordless Login](http://www.tecmint.com/wp-content/uploads/2012/10/SSH-Passwordless-Login-300x197.jpg)



Setup SSH Passwordless Login


If you are dealing with number of **Linux** remote servers, then **SSH Password-less** login is one of the best way to automate tasks such as automatic backups with scripts, synchronization files using scp and remote command execution.

In this example we will setup **SSH password-less** automatic login from server** 192.168.1.1** as user
**tecmint** to **192.168.1.2** with user **sheena**.

### Step 1: Create Authentication SSH-Kegen Keys on – (192.168.1.1)

First login into server **192.168.1.1** with user **tecmint** and generate a pair of public keys using following command.
**[**tecmint**@tecmint.com ~]$ ssh-keygen -t rsa**

Generating public/private rsa key pair.
Enter file in which to save the key (/home/tecmint/.ssh/id_rsa): [Press enter key]
Created directory '/home/tecmint/.ssh'.
Enter passphrase (empty for no passphrase): [Press enter key]
Enter same passphrase again: [Press enter key]
Your identification has been saved in /home/tecmint/.ssh/id_rsa.
Your public key has been saved in /home/tecmint/.ssh/id_rsa.pub.
The key fingerprint is:
af:bc:25:72:d4:04:65:d9:5d:11:f0:eb:1d:89:50:4c tecmint@tecmint.com
The key's randomart image is:
+--[ RSA 2048]----+
|        ..oooE.++|
|         o. o.o  |
|          ..   . |
|         o  . . o|
|        S .  . + |
|       . .    . o|
|      . o o    ..|
|       + +       |
|        +.       |
+-----------------+
### Step 2: Create .ssh Directory on – 192.168.1.2

Use SSH from server **192.168.1.1** to connect server **192.168.1.2** using
**sheena** as user and create **.ssh** directory under it, using following command.
**[**tecmint**@tecmint ~]$ ssh **sheena**@192.168.1.2 mkdir -p **.ssh****

The authenticity of host '192.168.1.2 (192.168.1.2)' can't be established.
RSA key fingerprint is d6:53:94:43:b3:cf:d7:e2:b0:0d:50:7b:17:32:29:2a.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '192.168.1.2' (RSA) to the list of known hosts.
**sheena**@192.168.1.2's password: [**Enter Your Password Here**]
### Step 3: Upload Generated Public Keys to – 192.168.1.2

Use SSH from server **192.168.1.1** and upload new generated public key (**id_rsa.pub**) on server
**192.168.1.2** under **sheena**‘s **.ssh** directory as a file name **authorized_keys**.
**[**tecmint**@tecmint ~]$ cat .ssh/id_rsa.pub | ssh **sheena**@192.168.1.2 'cat >> .ssh/authorized_keys'sheena**@192.168.1.2's password: [**Enter Your Password Here**]
### Step 4: Set Permissions on – 192.168.1.2

Due to different SSH versions on servers, we need to set permissions on .ssh directory and authorized_keys file.
**[**tecmint**@tecmint ~]$ ssh **sheena**@192.168.1.2 "chmod 700 .ssh; chmod 640 .ssh/authorized_keys"sheena**@192.168.1.2's password: [**Enter Your Password Here**]
### Step 5: Login from 192.168.1.1 to 192.168.1.2 Server without Password

From now onwards you can log into **192.168.1.2** as **sheena** user from server **192.168.1.1** as **tecmint** user without password.




