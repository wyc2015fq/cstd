
# Geth搭建私有链 - Wesley's Blog - CSDN博客


2018年05月16日 16:33:01[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：274


# 以太坊官方网络（Official Network）
以太坊官方网络有两种：
生产环境网络（又称主网络）
测试网络TestNet
主网络是产生真正有价值的以太币的网络，是全球化，部署在Internet环境上的．智能合约的代码，执行，区块的调用，都可以清晰地查看到．而所有节点都是全球化的，速度较慢．
TestNet是专供用户来开发，调试和测试的．如果开发简单的智能合约的话，用测试网络就足够了，用户可以把更多的精力集中在智能合约的编写．但是由于网路是官方提供的，因此对于以太坊技术的底层实现，Geth的各种参数接口，整个以太坊技术真是性能的理解就会弱很多．所以从开发者的角度来说，一个更好的选择是Private Network，可以从技术的底层去深入理解以太坊．
# 以太坊私有网络（Private Network）
以太坊私有网络，就是由用户自己通过Geth创建的私有网络，是一个非常适合开发，调试和测试的网路．私有网路可以方便开发者深入理解以太坊的技术底层．
# Geth搭建私有网络
### Step1：定义私有genesis state
创建一个新的文件夹privateNetwork，用来存储genesis.json文件和之后的私有链
`wesley@wesley-PC:~/Code/blockchain$ mkdir privateNetwork
wesley@wesley-PC:~/Code/blockchain$ cd privateNetwork
wesley@wesley-PC:~/Code/blockchain$ vim genesis.json`将下面的内容复制到genesis.json中：
`{
  "config": {
        "chainId": 14,
        "homesteadBlock": 0,
        "eip155Block": 0,
        "eip158Block": 0
    },
  "alloc"      : {},
  "coinbase"   : "0x0000000000000000000000000000000000000000",
  "difficulty" : "0x05000",
  "extraData"  : "",
  "gasLimit"   : "0x2fefd8",
  "nonce"      : "0x0000000000000042",
  "mixhash"    : "0x0000000000000000000000000000000000000000000000000000000000000000",
  "parentHash" : "0x0000000000000000000000000000000000000000000000000000000000000000",
  "timestamp"  : "0x00"
}`官方文档中的chainId为0，应该修改为非0的值，不然会导致 insufficient funds for gas * price + value
difficulty表示挖矿的难度值，官方文档中为0x20000，由于测试机器配置低，修改为0x05000。
### Step2：建立创世区块
运行如下指令：
`wesley@wesley-PC:~/Code/blockchain$ geth --datadir "./privateNetwork" init go-ethereum/genesis.json`结果如下：
![这里写图片描述](https://img-blog.csdn.net/20180516163107607?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后在privateNetwork中能发现geth和keystore两个新文件．
geth保存的是区块链的相关信息
keystore中保存的是该链条中的用户信息
![这里写图片描述](https://img-blog.csdn.net/20180516163118555?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Step3：创建自己的私有链条
运行如下命令：
`wesley@wesley-PC:~/Code/blockchain/privateNetwork$geth --networkid 14 --nodiscover --datadir "data/chain0" --rpc --rpcapi net,eth,web3,personal --rpcaddr localhost console 2>>geth.log`nodiscover的意思是不被其他节点自动发现（可以手动添加节点）
console是启动geth控制台，不加该选项，geth启动之后成为一个后台进程不会自动结束
最后把geth中的log信息导入geth.log中，方便了解geth运行的情况。
注意路径要加引号，不然会出问题．
`--nodiscover 关闭p2p网络的自动发现，需要手动添加节点，这样有利于我们隐藏私有网络
--datadir 区块链数据存储目录
--networkid 网络标识，私有链取一个大于4的随意的值
--rpc 启用ipc服务，默认端口号8545
--rpcapi 表示可以通过ipc调用的对象
--rpcaddr ipc监听地址，默认为127.0.0.1，只能本地访问
console 打开一个可交互的javascript环境
更多参数：https://github.com/ethereum/go-ethereum/wiki/Command-Line-Options`![这里写图片描述](https://img-blog.csdn.net/20180516163157406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Step４：在自己的私有链条上创建用户
查看私有链已有账户，会发现返回为[],此时还没有任何账户．
`>eth.accounts`创建新用户，密码是”wesley”，地址是＂0xe8280b532f5cc8d614e35fb5071d85250ccbd2a4＂
`>personal.newAccount("wesley")`![这里写图片描述](https://img-blog.csdn.net/20180516163210344?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Step５：尝试挖矿
继续在console中输入：
`>miner.start()`开始挖矿，查看geth.log，如下：
![这里写图片描述](https://img-blog.csdn.net/20180516163221825?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
过了一段时间之后，再停止挖矿，查看账户，只要不为0则挖矿成功
![这里写图片描述](https://img-blog.csdn.net/20180516163233799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

