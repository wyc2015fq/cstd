
# 使用remix ethereum部署智能合约 - Wesley's Blog - CSDN博客


2018年05月19日 10:29:57[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：560


# 使用Remix连接私有链
首先需要启动节点，注意需要将 rpcaddr 设置为 0.0.0.0，rpccorsdomain设置为 “*”．保证remix可以连接私有链
`geth --networkid 14 --nodiscover --datadir "data/chain1" --rpc --rpcapi "net,eth,web3,personal" --rpcaddr "0.0.0.0" --rpccorsdomain "*" --rpcport 8545 console 2>>geth1.log`然后再在run中的Environment选择web3 provider，然后web3 provider endpoint按照默认的就好（如果自己有指定特殊的端口则需要修改）
![这里写图片描述](https://img-blog.csdn.net/20180519102859953?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再指定号account就可以部署自己的智能合约了．
![这里写图片描述](https://img-blog.csdn.net/20180519102910125?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
部署成功之后如下图：
![这里写图片描述](https://img-blog.csdn.net/20180519102924356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 报错：creation of blockDemo pending… creation of blockDemo errored: authentication needed: password or unlock
![这里写图片描述](https://img-blog.csdn.net/20180519102935614?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这种情况需要在console中给账号解锁
`personal.unlockAccount(address, passphrase, duration)
personal.unlockAccount("0x5e97870f263700f46aa00d967821199b9bc5a120", "foo", 30)`

