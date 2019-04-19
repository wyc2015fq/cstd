# 用 JS 实现简单的神经网络算法 - 文章 - 伯乐在线
原文出处： [taogang](http://my.oschina.net/taogang/blog/686568?fromerr=DzNrW1c1)
### 神经网络简介
神经网络试图模拟大脑的神经元之间的关系来处理信息。它的计算模型通常需要大量彼此连接的节点。每个神经元通过某种特殊的输出函数来处理来自其它相邻神经元的加权输入值。
神经元之间的信息传递的强度，用所谓的加权值来定义，算法会不断的调整加权值来实现自我的学习过程。
![1](http://jbcdn2.b0.upaiyun.com/2016/06/28c8edde3d61a0411511d3b1866f063612.png)
神经网络分为多层，如上图，有输入层，隐藏层和输出层。
### JS线性代数包
神经网络的计算涉及到大量的矩阵计算，有许多的线性代数的开源的软件，Python下有著名的[numpy](http://www.numpy.org/)，非常有名。 Javascript也有几个：
- [http://numericjs.com/index.php](http://numericjs.com/index.php)
- [https://github.com/jstat/jstat](https://github.com/jstat/jstat)
- [https://mkaz.tech/javascript-linear-algebra-calculator.html](https://mkaz.tech/javascript-linear-algebra-calculator.html%C2%A0)
我使用了numericjs，效果还不错。推荐大家可以试试。
### 两层神经网络
我们有一些简单的输入输出的数据用来训练神经网络。这里每一行代表一条数据。输入有三个参数，输出是一个。
|Inputs 0|Inputs 1|Inputs 2|Output|
|----|----|----|----|
|0|0|1|0|
|1|1|1|1|
|1|0|1|1|
|0|1|1|0|
首先我们实现一个最简单的神经网络，没有隐藏层，输入直连输出。
![2](http://jbcdn2.b0.upaiyun.com/2016/06/665f644e43731ff9db3d341da5c827e17.png)
因为输入是三个参数，输出是一个，所以我们的神经网络输入层是三个节点，输出是1个。

JavaScript
```
// Sigmod function
function nonlin(x, deriv) {
  if (deriv) {
    return numeric.mul(x, numeric.sub(1, x));
  }
  return numeric.div(1, numeric.add(1, numeric.exp(numeric.neg(x))));
}
function train_neural(X, y, iteration) {
  // initialize weights
  var syn0 = numeric.sub(numeric.mul(2, numeric.random([3, 1])), 1);
  //Training loop
  var i = 0;
  for (; i < iteration; i++) {
    var l0 = X;
    var l1 = nonlin(numeric.dot(l0, syn0));
    var l1_error = numeric.sub(y, l1);
    var l1_delta = numeric.mul(l1_error, nonlin(l1, true));
    syn0 = numeric.add(syn0, numeric.dot(numeric.transpose(l0), l1_delta));
    } 
  }
}
//Initial input/ouput values
var X = [
  [0, 0, 1],
  [0, 1, 1],
  [1, 0, 1],
  [1, 1, 1]
];
var y = [
  [0],
  [0],
  [1],
  [1]
];
train_neural(X, y, 1000);
```
简单介绍一下训练的代码和过程
- X 输入数据
- y 输出数据
- nonlin， S函数
- l0，网络第一层，这是等于输入数据
- l1，网络第二层，这里就是输出层
- syn0，第一层网络的权重
训练的迭代过程就是先给出一个初始的权重，利用这个权重算出一个输出值，用目标结果减去这个值，得到一个差异值，再利用这个差异值对权重进行修正。
1000次迭代后的网络输出： ［0.03，0.02， 0.979， 0.974］
1000次迭代后的syn0权重值： ［7.266，－0.221，－3.415］
这里我们发现第一个节点的权重较大，这个和我们的数据是一致的，通过观察数据我们也可以发现，输出值和第一列的输入值是强相关。如果增加迭代的次数，这个值会更大。
### 三层神经网络
|Inputs 0|Inputs 1|Inputs 2|Output|
|----|----|----|----|
|0|0|1|0|
|0|1|1|1|
|1|0|1|1|
|1|1|1|0|
现在我们有了一组新的数据，通过观察发现，第三列和结果完全无关，第一列和第二列相同时结果为0，否则为1。这是一种非线性的关系，为了有效学习我们增加一层，网络变成了这个样子。
![3](http://jbcdn2.b0.upaiyun.com/2016/06/38026ed22fc1a91d92b5d2ef93540f206.png)

JavaScript
```
// Sigmod function
function nonlin(x, deriv) {
  if (deriv) {
    return numeric.mul(x, numeric.sub(1, x));
  }
  return numeric.div(1, numeric.add(1, numeric.exp(numeric.neg(x))));
}
function train_neural(X, y, iteration) {
  // initialize weights
  var syn0 = [
    [-0.1653904, 0.11737966, -0.71922612, -0.60379702],
    [0.60148914, 0.93652315, -0.37315164, 0.38464523],
    [0.7527783, 0.78921333, -0.82991158, -0.92189043]
  ];
  var syn1 = [
    [-0.66033916],
    [0.75628501],
    [-0.80330633],
    [-0.15778475]
  ];
  //Training loop
  var i = 0;
  for (; i < 1000; i++) {
    var l0 = X;
    var l1 = nonlin(numeric.dot(l0, syn0));
    var l2 = nonlin(numeric.dot(l1, syn1));
    var l2_error = numeric.sub(y, l2);
    var l2_delta = numeric.mul(l2_error, nonlin(l2, true));
    var l1_error = numeric.dot(l2_delta, numeric.transpose(syn1));
    var l1_delta = numeric.mul(l1_error, nonlin(l1, true));
    syn1 = numeric.add(syn1, numeric.dot(numeric.transpose(l1), l2_delta));
    syn0 = numeric.add(syn0, numeric.dot(numeric.transpose(l0), l1_delta));
  }
}
//Initial input/output values
var X = [
  [0, 0, 1],
  [0, 1, 1],
  [1, 0, 1],
  [1, 1, 1]
];
var y = [
  [0],
  [1],
  [1],
  [0]
];
train_neural(X, y, 1000);
```
训练的过程和之前的两层差别不大，只是多了一层。通过增加的这一层，可以有效的学习数据中的复杂非线性的关联关系。
经过1000次迭代， 输出值为：［0.02，0.95，0.94，0.05］
syn0 ：
![4](http://jbcdn2.b0.upaiyun.com/2016/06/011ecee7d295c066ae68d4396215c3d05.png)
如果大家对训练的过程有兴趣，可以运行我的code
- [http://codepen.io/gangtao/pen/yJLdXG](http://codepen.io/gangtao/pen/yJLdXG)
- [http://codepen.io/gangtao/pen/jrOjpX](http://codepen.io/gangtao/pen/jrOjpX)
