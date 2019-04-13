
# 区块链（Block Chain）结构解析 - Wesley's Blog - CSDN博客


2018年05月14日 19:05:40[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：2698


# 定义
**狭隘定义**：区块链是一个公共账本（Public ledger），一个按照时间顺序排序的交易记录
**广义定义**：区块链是分布式数据存储、点对点传输、共识机制、加密算法等计算机技术的新型应用模式。所谓共识机制是区块链系统中实现不同节点之间建立信任、获取权益的数学算法．
# 区块链的组成
区块链是一种按照时间顺序将数据区块以顺序相连的方式组合成的一种链式数据结构
# 区块的结构
每个Block分为两部分：区块头（Block Header）和区块体（ Block）
### 区块头（Block Header）
区块头包括三组元数据
用于连接前面的区块、索引自父区块哈希值的数据。
挖矿难度、Nonce（随机数，用于工作量证明算法的计数器）、时间戳
能够总结并快速归纳校验区块中所有交易数据的Merkle（默克尔）树根数据
![这里写图片描述](https://img-blog.csdn.net/2018051419041380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 时间戳
区块链中的时间戳从区块生成那一刻起就存在于区块中，它对应的是每一次交易记录的认证，证明交易记录的真实性。
#### Merkle树结构
Merkle树的数据结构存放所有叶子节点的值，并以此为基础生成一个统一的哈希值。Merkle的叶子节点存储的是数据信息的哈希值，非叶子节点的存储是对其下面所有叶子节点的组合进行哈希计算后得出的哈希值。区块中任意一个数据的变更都会导致Merkle树结构发生变化，在交易信息验证对比的过程中，Merkle树结构能够大大减少数据的计算量，毕竟，我们只需验证Merkle树结构生成的统一哈希值就可以了。
![这里写图片描述](https://img-blog.csdn.net/20180514201655137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 区块体（Block）
主要是包含**交易数据（Transaction，TX）**．
Transactions包括一个input和多个output．并且输入和输出相等：inputsatoshi=outputsatoshi$input\ satoshi = output\ satoshi$．已确认的交易（或者说已经花出去的钱）被称为**Transaction identifiers（TXIDs）**，未确认的被称为**Unspent Transaction Outputs（UTXOs）**
![这里写图片描述](https://img-blog.csdn.net/20180514190426999?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

