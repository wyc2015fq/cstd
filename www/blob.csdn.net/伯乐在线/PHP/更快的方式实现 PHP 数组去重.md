# 更快的方式实现 PHP 数组去重 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [hoohack](http://www.jobbole.com/members/hector) 翻译。未经许可，禁止转载！
英文出处：[Darian Brown](http://www.thecave.info/faster-alternative-to-phps-array-unique-function/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
**概述**
使用PHP的`array_unique()`函数允许你传递一个数组，然后移除重复的值，返回一个拥有唯一值的数组。这个函数大多数情况下都能工作得很好。但是，如果你尝试在一个大的数组里使用`array_unique（）`函数，它会运行地慢一些。
有一个比较好而且更快的函数`array_flip()`来替代使用`array_unique()`函数来创建唯一的数组。这个魔法般的函数会交换数组里面每一个元素的键和值，因为键值必须唯一，因此，你会得到跟`array_unique()`函数一样的结果。
**更快的方式实现PHP数组去重**

PHP
```
/* 创建一个包含重复值的，一共四个元素的数组 */
$array = array('green','blue','orange','blue');
/* 翻转数组，你将会得到唯一键值的数组
        array('green'=>0,'blue'=>1,'orange'=>2); */
$array = array_flip($array);
/* 然后再翻转一次，将键和值重新放置，然后得到数组：array(0=>'green',1=>'blue',2=>'orange'); */
$array = array_flip($array);
```
因为我们已经移除了一些元素，因此数组看起来不是正常的序列。比如我们可能会得到：`array(0=>’A’,2=>’B’,5=>’C’);`。在某些情况下，这不是一个问题，但是如果你需要数组的键值保持数字的序列，你可以使用一到两种方法解决键值乱序的问题。
**使用array_merge修复数组的keys**
添加array_flip之后的函数，将会对数组的键值排序并且让它们恢复到正常的序列，如：0,1,2,3…

PHP
```
$array = array('green','blue','orange','blue');
$array = array_flip($array);
$array = array_flip($array);
/* 使用array_merge()函数修复键值*/
$array = array_merge($array);
```
**第二种方式，使用array_keys**
注意，这种修复数组键值的方法比使用`array_merge()`函数稍微快了一点。你也可以在最后一步结合使用`array_keys()`函数(此函数返回翻转后的值)。然后当你翻转数组的值，键值就会根据顺序创建。

PHP
```
$array = array('green','blue','orange','blue');
$array = array_flip($array);
/* 跟第一个例子一样，但是现在我们先提取数组的键值 */
$array = array_keys($array);
```
**结论**
非常简单，比起在大数组使用array_unique函数，有了一个有效的性能提升。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7aa70eaa311d5a5dec82ca0d49624741.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/1a4b335ec981d013f4002205a23fedd0.jpg)
