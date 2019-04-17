# ubuntu source.list源不能用的情况下怎么做 - V__KING__的专栏 - CSDN博客





2017年08月29日 11:43:27[v__king__](https://me.csdn.net/V__KING__)阅读数：1015








## what

有些情况下，ubuntu不能使用apt-get install 来安装下载软件。例如fanghuoqiang。这个时候我们该怎么下载需要的软件？

## how

到 [https://launchpad.net/](https://launchpad.net/) 网站去搜索你要的软件包.deb文件. 

需要注意的是，必须下载对应ubuntu系列版本的.deb 文件。 

例如：
> 
ubuntu 12.04 对应的是 Trusty 系列 

   ubuntu 16.04 对应的是 Vivid 系列 

  ubuntu 16.04 对应的是 willy 系列 

  若是不知道ubuntu对应的系列版本，可以再/etc/apt/sources.list中找到相应的系列名称





