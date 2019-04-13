
# 3.13 truffle 安装使用 - 尹成的技术博客 - CSDN博客

2018年11月08日 19:07:26[尹成](https://me.csdn.net/yincheng01)阅读数：25个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



安装 npm install -g truffle
创建项目新建目录Mkdir MetaCoin
Cd MetaCoin
下载unboxTruffle unbox metacoin
如果想不在里面包含任何实例合约 使用truffle init

Truffle boxesTruffle boxes是一种框架样板，有不同的种类，通过样板可以很方便的管理项目
编译合约位置：所有合约全部处于contracts下面
编译命令：truffle compile 可以通过添加 –all重新编译所有合约文件
构建artifacts：位于build/contracts下面
依赖：通过import导入
部署迁移命令：truffle migrate (注：如果没有新的迁移脚本导入，使用truffle migrate不执行操作，要使用truffle migrate –reset,在部署合约时必须保证有测试网络)
迁移脚本：必须以数字开头var MyContract = artifacts.require("MyContract"); 
module.exports = function(deployer) { 
// deployment steps 
deployer.deploy(MyContract); 
};
Artifacts.require：告诉truffle，要与哪个合约进行交互，类似于node的require，返回一个合约抽象，指定合约与交互合约的名字要匹配，不要使用以合约文件为参数，因为一个文件里可能存在多个合约
Module.exports：导出一个函数，全被迁移脚本导出的函数都应该接受一个deployer对象作为其第一个参数。
初始化迁移合约：Migrations.sol：该合约包含一个特定的接口，对于大多数项目来说，这个合约只有在第一次做迁移的时候会被部署，以后都不会做任何更改。如果使用truffle init 来创建项目，会默认创建该合约
Deployer：部署器、组织部署任务，同步执行，所以可以按照顺序来编译部署任务，另外deployer上的每一个函数都会返回一个Promise，通过promise可以把有执行顺序依赖关系的部署任务组成队列。
可用账号：可以在部署中使用以太坊客户端或者web3提供的账户列表中的账号
Deployer APIDeployer.deploy(CONTRACT, ARGS…,OPTIONS)：部署合约Contract:需要部署的合约名字
Args:合约的构造函数需要的参数
Options:可选 值为{overwrite:true/false}，如果overwrite为false,那么在当前合约已经部署过的情况下，deployer不会再部署这个合约
为了快速部署多个合约，可以在该函数中传入一个或多个数组
– 部署单个合约，无构造函数参数 deployer.deploy(A)
– 部署单个合约，有构造函数参数 deployer.deploy(A,arg1,arg2,…)
– 部署单个合约，有的带构造函数参数，有的不带 deployer.deploy([[A,arg1,arg2,…],B,[C,arg1]])
Deployer.link(LIBRARY,DESTINATIONS)：把一个已部署好的库链接到一个或多个合约里，DESTINATIONS可以传入一个合约，也可以传入一组合约。如果destinations中的某个合约不依赖这个库，那么deployer的link函数就会忽略这个合约Deployer.deploy(LibA);deployer.link(LibA,B);deployer.deploy(B);
库LibA链接到多个合约：deployer.link(LibA,[B,C,D]);
3.Deployer.then(function(){…}):类似于node的promise,可以通过该函数操作合约数据


合约交互 以太坊中将向以太网络写入数据和从以太网络中读取数据作了区分。写入叫做交易，读取叫做调用交易：从根本上改变网络的状态。简单的交易有“发送一个以太币到另一个账户”。复杂的交易有“调用一个合约的函数，向网络中部署一个合约”特征交易可以写入或修改数据
消耗gas
需要时间处理
不会立即返回一个值(只会立即返回一个交易ID)
调用：在网络上执行代码，不会永久更改数据特征不消耗gas
不会更改网络状态
会被立即处理
会立刻返回一个值
决定使用调用还是交易就是看要读取数据还是写入数据
合约抽象合约抽象用于封装合约代码
安装truffle-contractcd myproject
npm init -y
npm install –save truffle-contract@3.0.1
npm install –save web3@0.20.0
与合约交互
添加一个新合约到网络：contract.new()…
使用现有的合约地址：contract.at(“0x0…”);at函数基于指定地址生成一个新的实例(抽象)
向合约发送以太币：方法一：使用instance.sendTransaction()直接向合约发送一个交易，类似于web3.eth.sendTransaction，除了没有回调函数
方法二：使用instance.send(web3.toWei(1,”ether”).then(function(result){});

truffle和MetaMask在浏览器中与智能合约进行交互之前，请确保它们已经编译，部署，并且您正在通过客户端JavaScript中的web3与它们进行交互。 我们建议使用truffle-contract库，因为它使合约的交互更容易，更健壮。完成以上内容之后，就可以与MetaMask交互了
作用MetaMask和Ganache：设置MetaMask网络端口为7545，开启Ganache
检测MetaMask的web3注入
// 判断是否有一个注入的web3实例 
if (typeof web3 !== 'undefined') { 
console.log('web3 已经注入！'); 
App.web3Provider = web3.currentProvider; 
} else { 
console.log('web3 被重新实例化！'); 
// 如果没有发现注入的web3实例，则返回到Ganache 
App.web3Provider = new Web3.providers.HttpProvider('http://127.0.0.1:7545'); 
} 
web3 = new Web3(App.web3Provider);
设置MetaMask打开网络选项，选择Custom RPC,在New RPC URL中输入http://127.0.0.1:7545，然后点击保存
使用MetaMask和truffle develop,与上面设置一样，只是需要把7545改成9545// 判断是否有一个注入的web3实例 
if (typeof web3 !== 'undefined') { 
console.log('web3 已经注入！'); 
App.web3Provider = web3.currentProvider; 
} else { 
console.log('web3 被重新实例化！'); 
// 如果没有发现注入的web3实例，则返回到Ganache 
App.web3Provider = new Web3.providers.HttpProvider('http://127.0.0.1:9545'); 
} 
web3 = new Web3(App.web3Provider);
使用MetaMask和Ganache CLI// 判断是否有一个注入的web3实例 
if (typeof web3 !== 'undefined') { 
console.log('web3 已经注入！'); 
App.web3Provider = web3.currentProvider; 
} else { 
console.log('web3 被重新实例化！'); 
// 如果没有发现注入的web3实例，则返回到Ganache 
App.web3Provider = new Web3.providers.HttpProvider('http://127.0.0.1:8545'); 
} 
web3 = new Web3(App.web3Provider);

EthPM概述：EthPM是以太坊的新包装注册表。 它遵循ERC190规范发布和使用智能合约包，并获得了许多不同的以太坊开发工具的广泛支持。 以太坊软件包注册表已经直接集成到Truffle中。
包的安装Truffle install
添加版本：truffle install @
依赖安装编辑ethpm.json，然后使用truffle install就可以安装了
使用已经安装好的合约所有已安装好的合约都在install_contracts下面，如果没有该目录会自动创建
发布自己的软件包 truffle publish,需要自己配置
Ropsten:以太坊软件包注册表目前存在于Ropsten测试网络中。 要发布到注册表，我们需要设置自己的Ropsten配置，因为我们将进行需要签名的事务。实例：使用infura发布软件包，在Ropsten网络上生成12个助记单词Npm install truffle-hdwallet-provider –save
编辑config(truffle.js)文件，添加Reposten网络配置

package配置实例{ 
"package_name": "adder", 
"version": "0.0.3", 
"description": "Simple contract to add two numbers", 
"authors": [ 
"Tim Coulter <tim.coulter@consensys.net>" 
], 
"keywords": [ 
"ethereum", 
"addition" 
], 
"dependencies": { 
"owned": "^0.0.1" 
}, 
"license": "MIT" 
} 
配置完成后，使用truffle publish
在发布之前，可以考虑删除配置文件中的无关网络artifacts：truffle networks –clean

合约调试概述：Truffle包含一个集成的调试器，以便您可以调试针对您的合约进行的交易。 此调试器的外观和感觉类似于传统开发环境中可用的现有命令行调试程序。
调试命令：truffle debug
truffle develop 和 truffle consoletruffle console：连接到任何以太坊客户端的基本交互式控制台，它主要用于连接已经启动的以太坊客户端(Ganache或者geth)启动:truffle console
truffle develop：truffle develop内置客户端，可以运行truffle的各种命令，它与console的区别在于其内置客户端，不需要单独启动以太坊客户端启动:truffle develop,启动端口为9545的内置客户端，会忽略掉truffle.js中的网络配置
可用命令：build compile create debug exec install migrate networks opcode public test version
编写外部脚本:truffle exec “path/to/file.js” 通过此方法也可以执行外部脚本文件
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

