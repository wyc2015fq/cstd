
# 区块链2.0以太坊智能合约solidity之helloworld - 尹成的技术博客 - CSDN博客

置顶2018年04月30日 23:52:36[尹成](https://me.csdn.net/yincheng01)阅读数：858


由于只能合约是放在区块链上面的代码，这给我们的调试带来了很多的困难，还好有在线的编译器：
https://remix.ethereum.org
第一个代码：
pragma solidity ^0.4.4;
contract Counter {
uint count = 0;
address owner;

constructor() public
{
owner = msg.sender;
}

function increment() public {
uint step = 10;
if (owner == msg.sender) {
count = count + step;
}
}
function getCount() constant public returns (uint)  {
return count;
}
function kill() public {
if (owner == msg.sender) {
selfdestruct(owner);
}
}
}

第一行代表solidity的版本，^代表向上兼容版本5.0
第二行contract Counter为一个智能合约类对象
第三行、四行为属性：
uint count = 0;
address owner;
下面是三个函数，其中构造函数为：
constructor() public
{
owner = msg.sender;
}
![](https://img-blog.csdn.net/20180430234551231?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180430234816590?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180430234908692?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180430235043561?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


![](https://img-blog.csdn.net/20180430224554793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180425001235188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180425001144107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
网址：http://www.qukuailianxueyuan.io/
![](https://img-blog.csdn.net/20180426145827720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018042614570887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
欲领取造币技术与全套虚拟机资料
区块链技术交流QQ群：756146052备注：CSDN
尹成学院微信：备注：CSDN
![](https://img-blog.csdn.net/20180425000635656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


