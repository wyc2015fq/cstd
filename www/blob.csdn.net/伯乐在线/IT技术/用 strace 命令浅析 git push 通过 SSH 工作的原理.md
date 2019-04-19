# 用 strace 命令浅析 git push 通过 SSH 工作的原理 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [howar_u](http://www.jobbole.com/members/howar) 翻译，[至秦](http://www.jobbole.com/members/applevip) 校稿。未经许可，禁止转载！
英文出处：[Kamal Marhubi](http://kamalmarhubi.com/blog/2015/11/21/using-strace-to-figure-out-how-git-push-over-ssh-works/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
昨天，突如其来的好奇充斥着我的脑袋：究竟 git push 如何通过 SSH 工作呢？由于我越来越习惯使用 strace 来折腾这类问题，所以我又尝试用它来练练手。如果我利用 strace（跟踪）git push 命令到这个网站的（资料）库，会得到如下显示：

Shell
```
[pid 15943] execve("/usr/bin/ssh", ["ssh", "git@github.com", "git-receive-pack 'kamalmarhubi/w"...], [/* 51 vars */]) = 0
```
所以 git push 最终会调用 ssh git@github.com git-receive-pack <repo-path>。然后在我的终端（terminal）尝试输入以下命令，得到了以下线索：

Shell
```
$ ssh git@github.com git-receive-pack kamalmarhubi/website
00bb2979fec627d60938c4ed2086cc60bb1 refs/heads/gh-pagesreport-status delete-refs side-band-64k quiet atomic ofs-delta agent=git/2:2.4.8~upload-pack-wrapper-script-1211-gc27b061
003f04bfcb3e238e5660ae9e71a6ce99f472211fe85f refs/heads/master
0000
```
终端依然在等待我的输入。SSH 用来解决身份验证和远程控制的问题，验证成功后，SSH 的另一端会运行一段命令来进行数据交换。而上面这几行就是数据交换的开始。
在网上稍微搜索了一下，我知道了这个协议是由行组成的，而每一行的 4 位前置码正是行长度的十六进制表示。后面跟着提交的 SHA-1 和 ref ，发送端以一行 “0000” 作为结束标识符。
上面的每一行对应（资料）库里的每一个分支：第一行自带了一条长长的小尾巴，好像是发送程序的自我介绍和支持的相关功能。
我在研究这些代码的时候，使用 xsel 命令把输出结果复制到编辑器上面，不过令人困惑的是，我粘贴得到的竟然只有第一行而不是所有元数据。

Shell
```
00bb29793c39c8e4bfec627d60938c4ed2086cc60bb1 refs/heads/gh-pages
```
通过 hexdump –C 查看完整的输出后发现，原来在 refs/heads/gh-pages 后面有一个空字节，而且在末尾处还换行了（用星号 * 标记处）：

Shell
```
00000000  30 30 62 62 32 39 37 39  33 63 33 39 63 38 65 34  |00bb29793c39c8e4|
00000010  62 66 65 63 36 32 37 64  36 30 39 33 38 63 34 65  |bfec627d60938c4e|
00000020  64 32 30 38 36 63 63 36  30 62 62 31 20 72 65 66  |d2086cc60bb1 ref|
00000030  73 2f 68 65 61 64 73 2f  67 68 2d 70 61 67 65 73  |s/heads/gh-pages|
00000040 *00*72 65 70 6f 72 74 2d  73 74 61 74 75 73 20 64  |.report-status d|
00000050  65 6c 65 74 65 2d 72 65  66 73 20 73 69 64 65 2d  |elete-refs side-|
00000060  62 61 6e 64 2d 36 34 6b  20 71 75 69 65 74 20 61  |band-64k quiet a|
00000070  74 6f 6d 69 63 20 6f 66  73 2d 64 65 6c 74 61 20  |tomic ofs-delta |
00000080  61 67 65 6e 74 3d 67 69  74 2f 32 3a 32 2e 34 2e  |agent=git/2:2.4.|
00000090  38 7e 75 70 6c 6f 61 64  2d 70 61 63 6b 2d 77 72  |8~upload-pack-wr|
000000a0  61 70 70 65 72 2d 73 63  72 69 70 74 2d 31 32 31  |apper-script-121|
000000b0  31 2d 67 63 32 37 62 30  36 31*0a*30 30 33 66 37  |1-gc27b061.003f7|
000000c0  39 32 66 34 39 36 65 37  35 33 64 62 39 33 33 30  |92f496e753db9330|
000000d0  66 30 61 34 65 38 32 39  30 62 38 61 36 63 62 61  |f0a4e8290b8a6cba|
000000e0  38 61 62 36 64 61 62 20  72 65 66 73 2f 68 65 61  |8ab6dab refs/hea|
000000f0  64 73 2f 6d 61 73 74 65  72 0a 30 30 30 30        |ds/master.0000|
000000fe
```
我在没有仔细研究的情况下，大胆地做了一个猜想：那些在 github 上做开发的家伙们，定义了一个相当简单的长度前缀+换行分隔协议（length-prefixed, newline-separated protocol），当他们有需要向协议里面加入一些元数据的时候，可以保持和老版本 git 的兼容性。这个解决方案巧妙地利用了 C 语言的 0 结尾字符串：把元数据放在空字节和新行之间。用这种方式读取数据（读取到新行之前位置）可以得到所有的元数据。元数据处理代码在执行时会跳过空字节，但是现有的协议代码却只能看到到空字节之前的数据，它们对这种改动完全无感!
所以我之前用 xsel 命令复制数据时，那些空字节后面的东东被完美地忽略了。
真相就只有一个，谜底就这样被解开了！（其实wo真不是柯南哦）
