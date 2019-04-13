
# 通过Mist部署智能合约 - Wesley's Blog - CSDN博客


2018年05月17日 23:01:24[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：452


# Mist
Mist是以太坊的官方钱包，他既可以连接生产网络、测试网络，更加可以通过设置参数的方式，连接我们自己的私有网络．
# Mist连接私有链
Mist下载地址：[https://github.com/ethereum/mist](https://github.com/ethereum/mist)
解压之后可以通过指定地址来访问私有网络
`./ethereumwallet --rpc http://localhost:8545`![这里写图片描述](https://img-blog.csdn.net/20180517230010127?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
会出现不安全提示，在测试网络直接同意
![这里写图片描述](https://img-blog.csdn.net/20180517230022501?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180517230032232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 部署简单的智能合约
点击Contracts->Deploy contract：
![这里写图片描述](https://img-blog.csdn.net/20180517230047168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180517230047168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后再复制上Solidity代码进行deploy：
![这里写图片描述](https://img-blog.csdn.net/20180517230057260?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意智能合约要成功部署还需要持续挖矿才能使合约得到确认．

[
](https://img-blog.csdn.net/20180517230047168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
