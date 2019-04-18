# kotlin 集合内元素比较 - weixin_33985507的博客 - CSDN博客
2019年02月28日 14:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
在看这篇文章之前，尝试调用list的排序方法：sortby，sortwith，sortedwith等方法，包括bean类实现comparator。发现怎么尝试都不好使。最后看到这篇文章后，[https://www.jianshu.com/p/a6aae0037f79](https://www.jianshu.com/p/a6aae0037f79)，发现不是之前调用的方法不好使，而是我这里对比的字段类型是string类型：“1.99”这样的，对于string的compareTo方法，是无法比较的，所以最后需要将原数据类型转为数字类型，之后再比较。注意：sortedwith，是不好使的，这个方法的意思是讲已排序好的元素集合相对位置不发声改变，具体看源代码解释。
