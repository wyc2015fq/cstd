# Windows中查看PowerShell版本 - 小灰笔记 - CSDN博客





2017年04月07日 21:29:38[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：13333








       我并不是很熟悉什么是PowerShell，但是有种直觉是：如果想在Windows中使用系统自带的功能取代bash shell，PowerShell或许是比DOS批处理更好的选择。不过，从头开始再来一门编程语言或许还是个不小的挑战。

       功能相对齐全且通用的PowerShell版本现在是V3版本，我试看了别人Win7电脑上的版本，内置的是V2版本。据说Win8.1以上的系统中内置的软件基本上都是V3以上，而Vista以后都已经支持V3以上的PowerShell。

       那么PowerShell的软件版本如何查看呢？其实有一个PowerShell内置的命令，$PSVersionTable。这个命令看上去到有一点bash或者Perl的感觉。我使用的系统是Windows 10 HB，具体的查看记录如下：

PSC:\Users\ThinkPad> $PSVersionTable



Name                           Value

----                           -----

PSVersion                      5.1.14393.953

PSEdition                      Desktop

PSCompatibleVersions           {1.0, 2.0, 3.0, 4.0...}

BuildVersion                   10.0.14393.953

CLRVersion                     4.0.30319.42000

WSManStackVersion              3.0

PSRemotingProtocolVersion      2.3

SerializationVersion           1.1.0.1

       我用的软件版本居然是V5版本的，之前一直以为是V3，真是出乎意料。原来，PowerShell又已经更新了这么多版本。

       还有一点比较值得一说，PowerShell的命令窗中输入命令的时候具有自动补全的功能，输入部分字符之后按下Tab键就能够自动补全。相比比较傻瓜式的CMD命令窗，这算是一个比较大的提升了。这倒是有点像Linux下的命令窗口了，用起来真有一点舒适感了。



