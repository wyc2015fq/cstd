
# 3.14 solidity库详解 - 尹成的技术博客 - CSDN博客

2018年11月08日 19:09:00[尹成](https://me.csdn.net/yincheng01)阅读数：39


lib库的使用、OpenZeppelin库、Ownable库、SafeMath库lib库库是一种不同类型的合约，没有存储，不拥有以太币
库中没有payable
Using for声明方式：using A for B,将A中定义的所有函数附着于任意类型B上，类型B的实例可以调用A中所有的方法
SafeMath库包含在OpenZeppelin中(node_modules/openzeppelin-solidity/contracts/math)，主要是添加了一些安全判断
Ownable库包含在OpenZeppelin中(node_modules/openzeppelin-solidity/contracts/ownership)，主要是提供基本的授权功能

lib使用方式如下pragma solidity ^0.4.18;
library C {
    function a() returns(address){
        return this;
    }
}
contract A {    
    function test() returns (address) {
        return C.a();// 返回合约A的地址
    }
}

学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

