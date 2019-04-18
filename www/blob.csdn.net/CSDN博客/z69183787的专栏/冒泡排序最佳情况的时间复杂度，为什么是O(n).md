# 冒泡排序最佳情况的时间复杂度，为什么是O(n) - z69183787的专栏 - CSDN博客
2017年01月17日 17:58:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2320
转自：[http://www.cnblogs.com/melon-h/archive/2012/09/20/2694941.html](http://www.cnblogs.com/melon-h/archive/2012/09/20/2694941.html)
我在许多书本上看到冒泡排序的最佳时间复杂度是O(n)，即是在序列本来就是正序的情况下。
但我一直不明白这是怎么算出来的，因此通过阅读《算法导论-第2版》的2.2节，使用对插入排序最佳时间复杂度推算的方法，来计算冒泡排序的复杂度。
1. 《算法导论》2.2中对插入排序最佳时间复杂度的推算
![](http://pic002.cnblogs.com/images/2012/445785/2012092011035910.png)
　　在最好情况下，6和7总不被执行，5每次只被执行1次。因此，
![](http://pic002.cnblogs.com/images/2012/445785/2012092011053188.png)
　　时间复杂度为O(n)
2. 冒泡排序的时间复杂度
　　2.1 排序代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public void bubbleSort(int arr[]) {
    for(int i = 0, len = arr.length; i < len - 1; i++) {
        for(int j = 0; j < len - i - 1; j++) {
            if(arr[j + 1] < arr[j])
                swap(arr, j, j + 1);
        }
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　2.2 最佳情况
　　　　序列原本就是正序
　　2.3 最佳情况时间复杂度推算
|语句|cost|times|
|----|----|----|
|i = 0,len = arr.length|c1|1|
|i < len - 1|c2|n|
|i++|c3|n - 1|
|j = 0|c4|n - 1|
|j < len - i - 1|c5|t(i=0) + t(i=1) + ... + t(i = n-2)|
|j++|c6|t2(i=0) + t2(i=1) + ... + t2(i = n-2)|
|arr[j + 1] < arr[j]|c7|t3(i=0) + t3(i=1) + ... + t3(i = n-2)|
|swap(arr, j, j + 1)|c8|t4(i=0) + t4(i=1) + ... + t4(i = n-2)|
　　T(n) = c1 + c2n + c3(n - 1) + c4(n - 1) + c5[t1(i=0) + t1(i=1) + ... + t1(i = n-2)] + c6[t2(i=0) + t2(i=1) + ... + t2(i = n-2)] + c7[t3(i=0)
 + t3(i=1) + ... + t3(i = n-2)] + c8[t4(i=0) + t4(i=1) + ... + t4(i = n-2)];　
　　当序列原本就是正序时，8从不被执行。因此
　　T(n) = c1 + c2n + c3(n - 1) + c4(n - 1) + c5[t1(i=0) + t1(i=1) + ... + t1(i = n-2)] + c6[t2(i=0) + t2(i=1) + ... + t2(i = n-2)] + c7[t3(i=0) + t3(i=1) + ... + t3(i = n-2)];
　　此时的时间复杂度应为O(n^2)。
**可是网上和许多书上都写道是O(n)，不知是否有人能帮我解答一下呢？**
2.4 在Stackoverflow上问到答案了。
　　我原本的代码的时间复杂度确实应该是O(n^2)，但算法可以改进，使最佳情况时为O(n)。改进后的代码为：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public void bubbleSort(int arr[]) {
    boolean didSwap;
    for(int i = 0, len = arr.length; i < len - 1; i++) {
        didSwap = false;
        for(int j = 0; j < len - i - 1; j++) {
            if(arr[j + 1] < arr[j]) {
                swap(arr, j, j + 1);
                didSwap = true;
            }
        }
        if(didSwap == false)
            return;
    }    
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
