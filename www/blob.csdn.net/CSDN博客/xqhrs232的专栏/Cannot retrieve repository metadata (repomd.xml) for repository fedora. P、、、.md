# Cannot retrieve repository metadata (repomd.xml) for repository: fedora. P、、、 - xqhrs232的专栏 - CSDN博客
2016年04月15日 11:15:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2363
原文地址::[http://www.cnblogs.com/linuxer/archive/2011/06/03/2272557.html](http://www.cnblogs.com/linuxer/archive/2011/06/03/2272557.html)
相关文章
1、解决yum错误：Cannot retrieve repository metadata (repomd.xml) for repository----[http://wjpinrain.blogchina.com/982076.html](http://wjpinrain.blogchina.com/982076.html)
2、Cannot retrieve repository metadata(repomd.xml) for repository:fedora.错误怎么解决----[http://zhidao.baidu.com/link?url=nPts3414ekphpah7FuNMHiCpWElGNwXkgXfGAUklkXecPHSsE4AYs_TdUqa5fGC52BzD-2lG-EFzomMkZlZeQxLtqOJf1JVVxzRqXIKfGNK](http://zhidao.baidu.com/link?url=nPts3414ekphpah7FuNMHiCpWElGNwXkgXfGAUklkXecPHSsE4AYs_TdUqa5fGC52BzD-2lG-EFzomMkZlZeQxLtqOJf1JVVxzRqXIKfGNK)
今天装完fedora14系统之后，开始装软件但是无论怎样都不行，一直都会出现这种情况：   
Cannot retrieve repository metadata (repomd.xml) for repository: fedora. P、、、
搞了一上午按照网上的也没搞好，非常的无奈，重新装了系统也不行，最后把
/etc/yum.repos.d/中的源删的仅剩一个，为a.repo其内容如下
[Fedora-ftp.sjtu.edu.cn]
name=Fedora 14 – i386
baseurl=http://ftp.sjtu.edu.cn/fedora/linux/releases/14/Fedora/i386/os/
enabled=1
gpgcheck=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-fedora
[Everything-ftp.sjtu.edu.cn]
name=Everything 14 – i386
baseurl=http://ftp.sjtu.edu.cn/fedora/linux/releases/14/Everything/i386/os/
enabled=1
gpgcheck=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-fedora
[updates-ftp.sjtu.edu.cn]
name=Fedora updates
baseurl=http://ftp.sjtu.edu.cn/fedora/linux/updates/14/i386/
enabled=1
gpgcheck=0
然后开始装软件就成功啦。
成功后还是高兴的。
