# Fedora 14 Error: Cannot retrieve repository metadata (repomd.xml) for repository - xqhrs232的专栏 - CSDN博客
2016年04月15日 11:36:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：574
原文地址::[http://blog.csdn.net/lyx2007825/article/details/40183819](http://blog.csdn.net/lyx2007825/article/details/40183819)
相关文章
1、解决yum错误：Cannot retrieve repository metadata (repomd.xml) for repository----[http://www.linuxdiyf.com/viewarticle.php?id=162568](http://www.linuxdiyf.com/viewarticle.php?id=162568)
2、[unable
 to install packages fedora 14 [closed]](https://ask.fedoraproject.org/en/question/9760/unable-to-install-packages-fedora-14/)----[https://ask.fedoraproject.org/en/question/9760/unable-to-install-packages-fedora-14/](https://ask.fedoraproject.org/en/question/9760/unable-to-install-packages-fedora-14/)

网上找了很多解决方法都不能解决，后来在国外的一个问答网站找到解决方案了，请查看：https://ask.fedoraproject.org/en/question/9760/unable-to-install-packages-fedora-14/
大意是：
Fedora版本不支持的 可以查看下面的连接：
[](http://archives.fedoraproject.org/pub/archive/fedora/linux/)[http://archives.fedoraproject.org/pub/archive/fedora/linux](http://archives.fedoraproject.org/pub/archive/fedora/linux)/
尽管不是很提倡这么做，但是你也能从下面的解决办法安装软件，假如你真想这么做，可以建一个repo文件，如下：
[Fedora]<code><span class="pun" style="margin: 0px; padding: 0px;">[</span><span class="pln" style="margin: 0px; padding: 0px;">fedora</span><span class="pun" style="margin: 0px; padding: 0px;">-</span><span class="pln" style="margin: 0px; padding: 0px;">archives</span><span class="pun" style="margin: 0px; padding: 0px;">]</span><span class="pln" style="margin: 0px; padding: 0px;">
name</span><span class="pun" style="margin: 0px; padding: 0px;">=</span><span class="typ" style="margin: 0px; padding: 0px;">Fedora</span><span class="pln" style="margin: 0px; padding: 0px;"> $releasever </span><span class="pun" style="margin: 0px; padding: 0px;">-</span><span class="pln" style="margin: 0px; padding: 0px;"> $basearch
baseurl</span><span class="pun" style="margin: 0px; padding: 0px;">=</span><span class="pln" style="margin: 0px; padding: 0px;">http</span><span class="pun" style="margin: 0px; padding: 0px;">:</span><span class="com" style="margin: 0px; padding: 0px;">//archives.fedoraproject.org/pub/archive/fedora/linux/releases/$releasever/Everything/$basearch/os/</span><span class="pln" style="margin: 0px; padding: 0px;">
enabled</span><span class="pun" style="margin: 0px; padding: 0px;">=</span><span class="lit" style="margin: 0px; padding: 0px;">1</span><span class="pln" style="margin: 0px; padding: 0px;">
gpgcheck</span><span class="pun" style="margin: 0px; padding: 0px;">=</span><span class="lit" style="margin: 0px; padding: 0px;">1</span><span class="pln" style="margin: 0px; padding: 0px;">
gpgkey</span><span class="pun" style="margin: 0px; padding: 0px;">=</span><span class="pln" style="margin: 0px; padding: 0px;">http</span><span class="pun" style="margin: 0px; padding: 0px;">:</span><span class="com" style="margin: 0px; padding: 0px;">//archives.fedoraproject.org/pub/archive/fedora/linux/releases/$releasever/Everything/$basearch/os/RPM-GPG-KEY-fedora-$basearch</span></code>然后将该文件命名为/etc/yum.repos.d/fedora-archives.repo 就可以正常使用yum了
