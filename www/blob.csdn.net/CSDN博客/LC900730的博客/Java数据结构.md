# Java数据结构 - LC900730的博客 - CSDN博客
2017年07月17日 20:56:47[lc900730](https://me.csdn.net/LC900730)阅读数：143
HashSet会根据元素的hashCode值来决定它的存储位置–也就是说，每个元素的hashCode就是它的索引。
当从HashSet中访问元素的时，HashSet会先计算该元素的hashCode值(也就是调用对象的hashCode()方法的返回值，然后直接到该hashCode对应的位置去取出该元素。) 
HashSet中每个能存储元素的槽位(slot)通常称为bucket，如果有多个元素的hashCode相同，但是它们通过equals方法比较返回false，就需要在一个桶里放很多元素，导致性能下降。 
    重写hashCode()方法的基本规则： 
        当2个对象通过equals方法返回true时候，这2个对象的hashCode应该相等； 
        对象中用作equals比较标准的属性，都应该用来计算hashCode值。
