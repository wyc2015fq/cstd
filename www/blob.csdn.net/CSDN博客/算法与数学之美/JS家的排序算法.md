# JS家的排序算法 - 算法与数学之美 - CSDN博客
2018年06月18日 21:06:18[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：63
## **引子**
有句话怎么说来着：
> 
**雷锋推倒雷峰塔，Java implements JavaScript.**
当年，想凭借抱Java大腿火一把而不惜把自己名字给改了的JavaScript（原名LiveScript），如今早已光芒万丈。node JS的出现更是让JavaScript可以前后端通吃。虽然Java依然制霸企业级软件开发领域（C/C + +的大神们不要打我。。。），但在Web的江湖，JavaScript可谓风头无两，坐上了头把交椅。
然而，在传统的计算机算法和数据结构领域，大多数专业教材和书籍的默认语言都是Java或者C/C+ +。这给最近想恶补算法和数据结构知识的我造成了一定困扰，因为我想寻找一本以JavaScript为默认语言的算法书籍。当我了解到O’REILLY家的动物丛书系列里有一本叫做《数据结构与算法JavaScript描述》时，便兴奋的花了两天时间把这本书从头到尾读了一遍。它是一本很好的针对前端开发者们的入门算法书籍，可是，它有一个很大的缺陷，就是里面有很多明显的小错误，明显到就连我这种半路出家的程序猿都能一眼看出来。还有一个问题是，很多重要的算法和数据结构知识并没有在这本书里被提到。这些问题对于作为一个晚期强迫症患者的我来说简直不能忍。于是乎，一言不合我就决定自己找资料总结算法。那么，我就从算法领域里最基础的知识点——排序算法总结起好了。
我相信以下的代码里一定会有某些bug或错误或语法不规范等问题是我自己无法发现的，所以敬请各位大神能够指出错误，因为只有在不断改错的道路上我才能取得长久的进步。
## 十大经典算法排序总结对比
一张图概括：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkwTLbzd7Jib8nlLIFrjpERVT0Tk6TPUfutk82kMsUDsbq7VbSHl4GOWoCAGNcKFeo1QspdJhDiahzzA/640?wx_fmt=png)
主流排序算法概览
##### 名词解释：
**n**: 数据规模
**k**:“桶”的个数
**In-place**: 占用常数内存，不占用额外内存
**Out-place**: 占用额外内存
**稳定性**：排序后2个相等键值的顺序和排序之前它们的顺序相同
## 冒泡排序（Bubble Sort）
##### 冒泡排序须知：
作为最简单的排序算法之一，冒泡排序给我的感觉就像Abandon在单词书里出现的感觉一样，每次都在第一页第一位，所以最熟悉。。。冒泡排序还有一种优化算法，就是立一个flag，当在一趟序列遍历中元素没有发生交换，则证明该序列已经有序。但这种改进对于提升性能来说并没有什么太大作用。。。
##### 什么时候最快（Best Cases）：
当输入的数据已经是正序时（都已经是正序了，我还要你冒泡排序有何用啊。。。。）
##### 什么时候最慢（Worst Cases）：
当输入的数据是反序时（写一个for循环反序输出数据不就行了，干嘛要用你冒泡排序呢，我是闲的吗。。。）
##### 冒泡排序动图演示：
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkwTLbzd7Jib8nlLIFrjpERVTfdGr5ZM0p9EeeTQCa5L0jVJAmOwPUwYDy0BwqEPNbl1Y3cpLXTiaYFg/640?wx_fmt=gif)
##### 冒泡排序JavaScript代码实现：
```php
```
```php
function bubbleSort(arr) {    var len = arr.length;    for (var i = 0; i < len; i++) {        for (var j = 0; j < len - 1 - i; j++) {            if (arr[j] > arr[j+1]) {        //相邻元素两两对比                var temp = arr[j+1];        //元素交换
                arr[j+1] = arr[j];
                arr[j] = temp;
            }
        }
    }    return arr;
}
```
## 选择排序（Selection Sort）
##### 选择排序须知：
在时间复杂度上表现最稳定的排序算法之一，因为无论什么数据进去都是O(n²)的时间复杂度。。。所以用到它的时候，数据规模越小越好。唯一的好处可能就是不占用额外的内存空间了吧。
##### 选择排序动图演示：
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkwTLbzd7Jib8nlLIFrjpERVTkrac5V2ibWBzyWm5nJia3dNjASgiaicDVMUG31x5QC4zzpr4zcsSztyaxg/640?wx_fmt=gif)
Selection Sort 动图演示 算法可视化来源：http://visualgo.net/
##### 选择排序JavaScript代码实现：
```php
function selectionSort(arr) {    var len = arr.length;    var minIndex, temp;    for (var i = 0; i < len - 1; i++) {
        minIndex = i;        for (var j = i + 1; j < len; j++) {            if (arr[j] < arr[minIndex]) {     //寻找最小的数
                minIndex = j;                 //将最小数的索引保存
            }
        }
        temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }    return arr;
}
```
## 插入排序（Insertion Sort）
##### 插入排序须知：
插入排序的代码实现虽然没有冒泡排序和选择排序那么简单粗暴，但它的原理应该是最容易理解的了，因为只要打过扑克牌的人都应该能够秒懂。当然，如果你说你打扑克牌摸牌的时候从来不按牌的大小整理牌，那估计这辈子你对插入排序的算法都不会产生任何兴趣了。。。
插入排序和冒泡排序一样，也有一种优化算法，叫做**拆半插入**。对于这种算法，得了懒癌的我就套用教科书上的一句经典的话吧：感兴趣的同学可以在课后自行研究。。。
##### 插入排序动图演示：
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkwTLbzd7Jib8nlLIFrjpERVTfIednKtLyVUoU7OqetLulJwIST3DfgSnzUaiaqF8yA8XSMTabWR18DA/640?wx_fmt=gif)
Insertion Sort 动图演示 算法可视化来源：http://visualgo.net/
##### 插入排序JavaScript代码实现：
```php
```php
function insertionSort(arr) {    var len = arr.length;    var preIndex, current;    for (var i = 1; i < len; i++) {
        preIndex = i - 1;
        current = arr[i];        while(preIndex >= 0 && arr[preIndex] > current) {
            arr[preIndex+1] = arr[preIndex];
            preIndex--;
        }
        arr[preIndex+1] = current;
    }    return arr;
}
```
```
## 希尔排序（Shell Sort）
##### 希尔排序须知：
希尔排序是插入排序的一种更高效率的实现。它与插入排序的不同之处在于，它会优先比较距离较远的元素。希尔排序的核心在于间隔序列的设定。既可以提前设定好间隔序列，也可以动态的定义间隔序列。动态定义间隔序列的算法是《算法（第4版》的合著者Robert Sedgewick提出的。在这里，我就使用了这种方法。
##### 希尔排序JavaScript代码实现：
```cpp
```cpp
function shellSort(arr) {
    var len = arr.length,
        temp,
        gap = 1;    while(gap < len/3) {          //动态定义间隔序列
        gap =gap*3+1;
    }    for (gap; gap > 0; gap = Math.floor(gap/3)) {        for (var i = gap; i < len; i++) {
            temp = arr[i];            for (var j = i-gap; j >= 0 && arr[j] > temp; j-=gap) {
                arr[j+gap] = arr[j];
            }
            arr[j+gap] = temp;
        }
    }    return arr;
}
```
```
## 归并排序（Merge Sort）
##### 归并排序须知：
作为一种典型的分而治之思想的算法应用，归并排序的实现由两种方法：
- 
自上而下的递归（所有递归的方法都可以用迭代重写，所以就有了第2种方法）
- 
自下而上的迭代
在《数据结构与算法JavaScript描述》中，作者给出了自下而上的迭代方法。但是对于递归法，作者却认为：
> 
However, it is not possible to do so in JavaScript, as the recursion goes too deep
for the language to handle.
然而,在 JavaScript 中这种方式不太可行,因为这个算法的递归深度对它来讲太深了。
说实话，我不太理解这句话。意思是JavaScript编译器内存太小，递归太深容易造成内存溢出吗？还望有大神能够指教。
和选择排序一样，归并排序的性能不受输入数据的影响，但表现比选择排序好的多，因为始终都是O(n log n）的时间复杂度。代价是需要额外的内存空间。
##### 归并排序动图演示：
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkwTLbzd7Jib8nlLIFrjpERVTicLiaUGAiaaZ5ln011NxWpgKaMPgKSEKiaEWrGyCQYN0aUVuRfQj1SxR3A/640?wx_fmt=gif)
Merge Sort 动图演示 算法可视化来源：http://visualgo.net/
##### 归并排序JavaScript代码实现：
```
function mergeSort(arr) {  return result;
}
```
## 快速排序（Quick Sort）
##### 快速排序须知：
又是一种分而治之思想在排序算法上的典型应用。本质上来看，快速排序应该算是在冒泡排序基础上的递归分治法。
快速排序的名字起的是简单粗暴，因为一听到这个名字你就知道它存在的意义，就是快，而且效率高! 它是处理大数据最快的排序算法之一了。虽然Worst Case的时间复杂度达到了O(n²)，但是人家就是优秀，在大多数情况下都比平均时间复杂度为O(n log n) 的排序算法表现要更好，可是这是为什么呢，我也不知道。。。好在我的强迫症又犯了，查了N多资料终于在《算法艺术与信息学竞赛》上找到了满意的答案：
> 
快速排序的最坏运行情况是O(n²)，比如说顺序数列的快排。但它的平摊期望时间是O(n log n) ，且O(n log n)记号中隐含的常数因子很小，比复杂度稳定等于O(n log n)的归并排序要小很多。所以，对绝大多数顺序性较弱的随机数列而言，快速排序总是优于归并排序。
##### 快速排序动图演示：
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkwTLbzd7Jib8nlLIFrjpERVTSlz0212qjUpibhMbxJs2eeR24AKXOjB9jQKX8soicicsIo8BuibcYzZ1fA/640?wx_fmt=gif)
Quick Sort 动图演示 算法可视化来源：http://visualgo.net/
##### 快速排序JavaScript代码实现：
```
function var temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}
```
## 堆排序（Heap Sort）
##### 堆排序须知：
堆排序可以说是一种利用堆的概念来排序的选择排序。分为两种方法：
- 
大顶堆：每个节点的值都大于或等于其子节点的值，在堆排序算法中用于升序排列
- 
小顶堆：每个节点的值都小于或等于其子节点的值，在堆排序算法中用于降序排列
##### 堆排序动图演示：
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkwTLbzd7Jib8nlLIFrjpERVTOEhmNTsF9M03SDV7jI0QOHRB5MYg4VvhxuuZ8FyYHTzvrWibsxyEp7A/640?wx_fmt=gif)
Heap Sort 动图演示 算法可视化来源：http://www.ee.ryerson.ca/~courses/coe428/sorting/heapsort.html
##### 堆排序JavaScript代码实现：
```java
```java
return arr;
}
```
```
## 计数排序（Counting Sort）
##### 计数排序须知：
计数排序的核心在于将输入的数据值转化为键存储在额外开辟的数组空间中。
作为一种线性时间复杂度的排序，计数排序要求输入的数据必须是有确定范围的整数。
##### 计数排序动图演示：
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkwTLbzd7Jib8nlLIFrjpERVTb1CGWh6U6ULalaHyhow1Yk65gg2UvbDkeFQyks52GLCqAotV7lFDDQ/640?wx_fmt=gif)
Counting Sort 动图演示 算法可视化来源：http://visualgo.net/
##### 计数排序JavaScript代码实现：
```cpp
```cpp
function countingSort(arr, maxValue) {
    var bucket = new Array(maxValue+1),
        sortedIndex = 0;
        arrLen = arr.length,
        bucketLen = maxValue + 1;    for (var i = 0; i < arrLen; i++) {        if (!bucket[arr[i]]) {
            bucket[arr[i]] = 0;
        }
        bucket[arr[i]]++;
    }    for (var j = 0; j < bucketLen; j++) {        while(bucket[j] > 0) {
            arr[sortedIndex++] = j;
            bucket[j]--;
        }
    }    return arr;
}
```
```
## 桶排序（Bucket Sort）
##### 桶排序须知：
桶排序是计数排序的升级版。它利用了函数的映射关系，高效与否的关键就在于这个映射函数的确定。
为了使桶排序更加高效，我们需要做到这两点：
- 
在额外空间充足的情况下，尽量增大桶的数量
- 
使用的映射函数能够将输入的N个数据均匀的分配到K个桶中
同时，对于桶中元素的排序，选择何种比较排序算法对于性能的影响至关重要。
##### 什么时候最快（Best Cases）：
当输入的数据可以均匀的分配到每一个桶中
##### 什么时候最慢（Worst Cases）：
当输入的数据被分配到了同一个桶中
##### 桶排序JavaScript代码实现：
```cpp
```cpp
return arr;
}
```
```
## 基数排序（Radix Sort）
##### 基数排序须知：
基数排序有两种方法：
- 
MSD 从高位开始进行排序
- 
LSD 从低位开始进行排序
##### 基数排序 vs 计数排序 vs 桶排序
这三种排序算法都利用了桶的概念，但对桶的使用方法上有明显差异：
基数排序：根据键值的每位数字来分配桶
计数排序：每个桶只存储单一键值
桶排序：每个桶存储一定范围的数值
##### LSD基数排序动图演示：
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkwTLbzd7Jib8nlLIFrjpERVTobarEBn550hgI5tSFkSakOdVKibWlaxBhRiayosoJVmDib5I262ic1EEmg/640?wx_fmt=gif)
Radix Sort 动图演示 算法可视化来源：http://visualgo.net/
##### 基数排序JavaScript代码实现：
```
return arr;
}
```
∑编辑 | Gemini
来源 | 简书
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabky7x6u1VxMVMia4MLibNzC2nrumY3zDflTsCeoM04M1BrkvPny8tsw6hYkIicUr42iarLmadL2x6JwV6A/640?wx_fmt=png)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
