# 【Numpy】 乘法 np.multiply np.dot - YZXnuaa的博客 - CSDN博客
2018年02月08日 16:28:22[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：323
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)
1. 当为array的时候，默认d*f就是对应元素的乘积，multiply也是对应元素的乘积，dot（d,f）会转化为矩阵的乘积， dot点乘意味着相加，而multiply只是对应元素相乘，不相加
2. 当为mat的时候，默认d*f就是矩阵的乘积，multiply转化为对应元素的乘积，dot（d,f）为矩阵的乘积
![](https://img-blog.csdn.net/20170520145036969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmJiZW95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170520145513737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmJiZW95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3. 混合时候的情况，一般不要混合
  混合的时候默认按照矩阵乘法的, multiply转化为对应元素的乘积，dot（d,f）为矩阵的乘积
总结， 数组乘法默认的是点乘，矩阵默认的是矩阵乘法，混合在一起默认的是矩阵乘法，multiply转化为对应元素的乘积，dot（d,f）会转化为矩阵的乘积，注意，像multiply不满足对应元素时，按照广播是方式进行。
![](https://img-blog.csdn.net/20170520150338108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmJiZW95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170520150508601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmJiZW95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
