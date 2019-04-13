
# Geth搭建多节点私有链条 - Wesley's Blog - CSDN博客


2018年05月17日 22:58:54[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：285


# 搭建0号节点
参照：[https://blog.csdn.net/yinanmo5569/article/details/80339031](https://blog.csdn.net/yinanmo5569/article/details/80339031)
# 搭建1号节点
创建步骤与之前是一样的，要保证两个节点在同一个区块链上工作的话首先要保证genesis创世区块链是一样的，所以注意使用同样的genesis.json文件来创建．
`geth --datadir "data/chain1" init　genesis.json`
# 启动0号，1号节点
为了使两个节点同时启动，注意使用不同的port和rpc port.
启动0号节点：
`geth --networkid 14 --nodiscover --datadir "data/chain0" --rpc --rpcapi net,eth,web3,personal --rpcaddr localhost console 2>>geth0.log`启动1号节点：
`geth --networkid 14 --nodiscover --datadir "data/chain0" --port 55554 --rpc --rpcapi net,eth,web3,personal --rpcport 8101 --rpcaddr localhost console 2>>geth1.log`参数解释：
`--nodiscover 关闭p2p网络的自动发现，需要手动添加节点，这样有利于我们隐藏私有网络
--datadir 区块链数据存储目录
--port  网络监听端口，默认30303
--networkid 网络标识，私有链取一个大于4的随意的值
--rpc 启用ipc服务
--rpcport ipc服务端口，默认端口号8545
--rpcapi 表示可以通过ipc调用的对象
--rpcaddr ipc监听地址，默认为127.0.0.1，只能本地访问
console 打开一个可交互的javascript环境`
# 添加节点
通过addPeer来添加节点．关于Geth JavaScript console的具体api可以在查询官方文档：[https://github.com/ethereum/go-ethereum/wiki/Management-APIs](https://github.com/ethereum/go-ethereum/wiki/Management-APIs)
首先查看1号节点的enode
`> admin.nodeInfo.enode
"enode://9ddf2906232571602eae34882305049cda0ac9b0053f18ac4dc8cf874400cd88788be6bf24b608fafb1b719388cbcd4cd6c4335bbaf924dfe35a705379a60caa@[::]:30303?discport=0"`在0号节点的console，添加1号节点
`admin.addPeer("enode://9ddf2906232571602eae34882305049cda0ac9b0053f18ac4dc8cf874400cd88788be6bf24b608fafb1b719388cbcd4cd6c4335bbaf924dfe35a705379a60caa@127.0.0.1:30303")
true`
# 查看是否添加成功
查看连接的节点数量和连接的节点列表：
`net.peerCount
admin.peers`![这里写图片描述](https://img-blog.csdn.net/20180517225739385?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

