# PHP源码阅读：strtolower 和 strtoupper 函数 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [hoohack](http://www.jobbole.com/members/hector) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
字符串的操作函数中，字符串的大小写转换也算是比较常用的函数，其底层实现也比较简单，下面来一探究竟。
> 
我在github有对PHP源码更详细的注解。感兴趣的可以围观一下，给个star。[PHP5.4源码注解](https://github.com/read-php-src/read-php-src)。可以通过[commit记录](https://github.com/read-php-src/read-php-src/commits/master)查看已添加的注解。
## strtolower


```
string strtolower ( string $string )
```
将字符串转换成小写字符。
## strtoupper


```
string strtoupper ( string $string )
```
将字符串转换成大写字符。
## 运行示例


```
$str = 'Hello World';
$new_str = strtolower($str); // hello world
$str = 'hello world';
$new_str = strtoupper($str); // HELLO WORLD
```
## 代码运行步骤
> 
拷贝一份字符串
php_strtolower/php_strtoupper进行转换
## 源码解读
两个函数的核心操作都差不多，讲一下strtolower，另一个是类似的。 php_strtolower函数的核心代码如下：


```
c = (unsigned char *)s;
e = c+len;
// 遍历s，逐个变为小写
while (c < e) {
　　*c = tolower(*c);
　　c++;
}
return s;
```
这个函数就是遍历整个字符串，逐个转成小写字符。这也是一个经典的指针操作。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7aa70eaa311d5a5dec82ca0d49624741.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/1a4b335ec981d013f4002205a23fedd0.jpg)
