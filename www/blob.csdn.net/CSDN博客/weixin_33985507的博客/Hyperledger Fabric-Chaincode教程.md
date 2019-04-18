# Hyperledger Fabric - Chaincode教程 - weixin_33985507的博客 - CSDN博客
2018年04月02日 22:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：51
# 1. 什么是Chaincode？
Chaincode是一个用go写的程序，以后也可以用其他的语言比如说java什么的来编写。Chaincode在一个安全的docker容器里面运行，独立于背书节点的处理。Chaincode通过应用程序提交的交易来初始化账本并且管理账本。
Chaincode通常处理网络中成员一致认同的业务逻辑，所以也被称作为是“智能合约”。chaincode创建的状态仅限于当前chaincode的范围内，别的chaincode没法直接访问。然而，在同一个网络中，可以开放这种chaincode的权益给别的chaincode来访问它的状态。
# 2. 两个角色
在chaincode里面，我们提供了两个不同的视角。第一个是，从应用程序开发者的角度来看待chaincode；另一个角度是，从操作者的角度来看待如何管理整个区块链网络，这个操作者会利用Hyperledger Fabric提供的API来安装，实例化，更新chaincode，但是并不会直接去开发Chaincode
