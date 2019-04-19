# 剑指offer——调整数组顺序使奇数位于偶数前面 - 刘炫320的博客 - CSDN博客
2017年04月27日 14:55:00[刘炫320](https://me.csdn.net/qq_35082030)阅读数：337标签：[剑指offer-java																[排序																[归并																[时间复杂度和空间复杂度																[调整数组顺序](https://so.csdn.net/so/search/s.do?q=调整数组顺序&t=blog)](https://so.csdn.net/so/search/s.do?q=时间复杂度和空间复杂度&t=blog)](https://so.csdn.net/so/search/s.do?q=归并&t=blog)](https://so.csdn.net/so/search/s.do?q=排序&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer-java&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/qq_35082030/article/category/6807625)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有的奇数位于数组的前半部分，所有的偶数位于位于数组的后半部分，并保证奇数和奇数，偶数和偶数之间的相对位置不变。
# 2. 解决办法
这个解决方法应该很容易想到这样一个办法，那就是把奇数先全部找出来，然后把偶数全部找出来，然后把两个数组拼接起来就可以了。
这既是一个非常容易想到的办法，而且也是正常人做这件事情的过程。实际上，这样的时间复杂度是O（n）,空间复杂度是O（n），我们一般比较注重时间复杂度，因为就目前情况来看，显然时间更值钱。
具体代码如下：
```java
public static void reOrderArray(int[] array){
        int[] odd=new int[array.length];
        int[] even=new int[array.length];
        int oddnum=0,evennum=0;
        //找出奇数串，偶数串
        for(int i=0;i<array.length;i++){
            if(array[i]%2==0){
                even[evennum++]=array[i];
            }
            else{
                odd[oddnum++]=array[i];
            }
        }
        //复原数组
        for(int i=0;i<oddnum;i++){
            array[i]=odd[i];
        }
        for(int i=0;i<evennum;i++){
            array[oddnum+i]=even[i];
        }
    }
```
# 3. 一点思考
但可能作者的主要意图并不是如此，它等同一个排序算法，也就默认为偶数比奇数“大”，而且要一个稳定的算法。我们可以重新复习一下各种经典排序的时间复杂度与稳定性。 
![各种常用排序的时间复杂度](http://blog.chinaunix.net/attachment/201201/18/21457204_1326898064RUxx.jpg)
从上表可以看出，只有插入排序和磨泡排序与归并排序可以使用。而最好的归并排序，其时间复杂度为O（$nlog_2n$），时间复杂度为O(1)。改写的部分只需要改写归并的过程即可。同样的，冒泡排序和插入排序的优势并不那么明显，因此我就不列举这两种算法的变种了。
