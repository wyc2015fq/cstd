# js如何生成[n,m]的随机数（整理总结） - weixin_33985507的博客 - CSDN博客
2018年09月12日 15:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：19
# js如何生成[n,m]的随机数（整理总结）
## 一、总结
### 一句话总结：
// max - 期望的最大值
// min - 期望的最小值
parseInt(Math.random()*(max-min+1)+min,10);
Math.floor(Math.random()*(max-min+1)+min);
### 1、parseInt(*string*, *radix*)的第二个参数是什么意思？
parseInt() 函数可解析一个字符串，并返回一个整数。
radix可选。表示要解析的数字的基数。该值介于 2 ~ 36 之间。
如果省略该参数或其值为 0，则数字将以 10 为基础来解析。如果它以 “0x” 或 “0X” 开头，将以 16 为基数。
如果该参数小于 2 或者大于 36，则 parseInt() 将返回 NaN。
### 2、Math.random();生成的数的0和1的包括情况是怎样的？
左闭右开
Math.random();  //0.0 ~ 1.0 之间的一个伪随机数。【包含0不包含1】 //比如0.8647578968666494
### 3、Math.**ceil**(Math.random()*10);可均衡获取0到10的随机**整数么** ？
不能
可获取从1到10的随机**整数** ，取0的概率极小。
### 4、Math.**round**(Math.random()*10);可以均衡获取0到10的随机**整数**么？
不能
Math.**round**(Math.random()*10);  //基本均衡获取0到10的随机**整数**，其中**获取最小值0和最大值10的几率少一半**。
*因为结果在0~0.4 为0，0.5到1.4为1...8.5到9.4为9，9.5到9.9为10。所以头尾的分布区间只有其他数字的一半。*
### 5、均衡**生成[0,max]的随机数有哪些方法**？
ParseInt和Math.floor方法
// max - 期望的最大值
parseInt(Math.random()*(max+1),10);
Math.floor(Math.random()*(max+1));
### 6、js中如何判断参数个数？
arguments对象的length属性
arguments.length
```
//生成从minNum到maxNum的随机数
function randomNum(minNum,maxNum){ 
    switch(arguments.length){ 
        case 1: 
            return parseInt(Math.random()*minNum+1,10); 
        break; 
        case 2: 
            return parseInt(Math.random()*(maxNum-minNum+1)+minNum,10); 
        break; 
            default: 
                return 0; 
            break; 
    } 
}
```
### 7、parseInt的取整模式是怎样的？
parseInt()和Math.floor()结果都是向下取整。
## 二、js生成[n,m]的随机数
### 一、预备知识 
Math.ceil();  //向上取整。
Math.floor();  //向下取整。
Math.round();  //四舍五入。
Math.random();  //0.0 ~ 1.0 之间的一个伪随机数。【包含0不包含1】 //比如0.8647578968666494
Math.**ceil**(Math.random()*10);      // 获取从1到10的随机**整数** ，取0的概率极小。
Math.**round**(Math.random());   //可均衡获取0到1的随机**整数**。
Math.**floor**(Math.random()*10);  //可均衡获取0到9的随机**整数**。
Math.**round**(Math.random()*10);  //基本均衡获取0到10的随机**整数**，其中**获取最小值0和最大值10的几率少一半**。
*因为结果在0~0.4 为0，0.5到1.4为1...8.5到9.4为9，9.5到9.9为10。所以头尾的分布区间只有其他数字的一半。*
### 二 、生成[n,m]的随机整数
函数功能：生成[n,m]的随机整数。
在js生成验证码或者随机选中一个选项时很有用。。
```
//生成从minNum到maxNum的随机数
function randomNum(minNum,maxNum){ 
    switch(arguments.length){ 
        case 1: 
            return parseInt(Math.random()*minNum+1,10); 
        break; 
        case 2: 
            return parseInt(Math.random()*(maxNum-minNum+1)+minNum,10); 
        break; 
            default: 
                return 0; 
            break; 
    } 
}
```
###  过程分析：
Math.random()生成[0,1)的数，所以
Math.random()*5生成{0,5)的数。
通常期望得到整数，所以要对得到的结果处理一下。
parseInt()，Math.floor()，Math.ceil()和Math.round()都可得到整数。
parseInt()和Math.floor()结果都是向下取整。
所以Math.random()*5生成的都是[0,4] 的随机整数。
**所以生成[1,max]的随机数，公式如下：**
```
// max - 期望的最大值
parseInt(Math.random()*max,10)+1;
Math.floor(Math.random()*max)+1;
Math.ceil(Math.random()*max);
```
**所以生成[0,max]到任意数的随机数，公式如下：**
```
// max - 期望的最大值
parseInt(Math.random()*(max+1),10);
Math.floor(Math.random()*(max+1));
```
**所以希望生成[min,max]的随机数，公式如下：**
```
// max - 期望的最大值
// min - 期望的最小值
parseInt(Math.random()*(max-min+1)+min,10);
Math.floor(Math.random()*(max-min+1)+min);
```
参考：js生成[n,m]的随机数 - starof - 博客园
https://www.cnblogs.com/starof/p/4988516.html

