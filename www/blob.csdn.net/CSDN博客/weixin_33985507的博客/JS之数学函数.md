# JS之数学函数 - weixin_33985507的博客 - CSDN博客
2016年12月13日 23:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
- 
四舍五入
```
var res = Math.round(5.4);
```
- 
获取最大值
```
var res = Math.max(10,20,3,6,90);
arguments  func_get_args
```
- 
获取最小值
```
var res = Math.min(10,89,70,9,-10);
```
- 
获取绝对值
```
var res = Math.abs(-4);//   |-100|  =  100
```
- 
退一取整
```
var res = Math.floor(9.9);
```
- 
进一取整
```
var res = Math.ceil(2.1);
```
- 
幂运算
```
powervar res = Math.pow(5, 4);
```
- 
开方运算
```
var res = Math.sqrt(100);
```
- 
获取一个随机数
```
var res = Math.random();//  (0,1) *10 = (0,10)
```
- 
获取从0到10的随机数 (包括小数)
```
```
var res = Math.random()*10; // (0,10)   4.3  6.5   7.5  0.23
```
```
- 获取从1到10的整数
```
```
var res = Math.ceil(Math.random()*10);//[1,10]
```
```
- 获取从5到14的整数 [1,10]+4 =  [5,14]  m=5  n=14
```
```
var res = Math.ceil(Math.random()*10)+4;
```
```
- 从20到50的随机数 [1,30] => [20,50]
获取从m到n的整数  10 => (n-m+1)   4=>(m-1)
```
```
var res = Math.ceil(Math.random(n-m+1))+m-1; 
```
```
- 封装函数 1,10
```
function rand(m,n){
   return Math.ceil(Math.random()*(n-m+1))+m-1;
}
var res = rand(0,10);
console.log(res);
```
