
# 剑指offer——调整数组顺序使奇数位于偶数前面 - manduner的博客 - CSDN博客


2019年03月15日 21:25:38[Manduner_TJU](https://me.csdn.net/manduner)阅读数：15标签：[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，部分面经
至今，虽然才面试了两家公司（算法工程师方向），但是收获还是不小的，其中一部分收获就是关于编程题。记得我之前的一篇博客吐槽过，为啥一些IT大牛在做编程题的时候，不用java或者python封装的工具直接实现某些功能（例如：用java中的Arrays.sort()实现数组排序），而是非得通过下标转换去实现这些功能呢（例如，通过下标变换写各种排序算法）。。。。
才面试了两家公司，我就悟出了其中的一些道理，首先，通过java或者python封装好的一些工具直接实现某些功能，不是不可以，就是面试官不会满意你这种操作，为啥呢？？？ 个人认为，通过封装好的函数直接实现某种功能，只能说明你对java或者python用的相对熟练而已，但是体现不出你的逻辑思维能力，换句话说，玩转下标转换，更能体现面试者对一个算法的理解，更能体现面试者的逻辑思维能力。所以，我还是建议大家，练习编程题的时候，少用封装好的函数，多手动实现某个算法底层的东西。
# 2，问题描述
输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有的奇数位于数组的前半部分，所有的偶数位于数组的后半部分，并保证奇数和奇数，偶数和偶数之间的相对位置不变。
# 3，思路分析
有两种思路，第一种思路是利用空间换时间，想法简单粗暴。第二种思路是，直接通过小标变换在数组内部实现该功能（第二种思路更能体现算法的身影）
**第一种思路步骤：**
初始化两个ArrayList（al_odd, al_even），al_odd用来存放奇数，al_even用来存放偶数。
遍历数组，若元素为奇数则把该元素放入al_odd，若元素为偶数则把元素放入al_even。
拼接al_odd, al_even
遍历拼接后的列表，把列表中的每个元素赋值为array
**第二种思路步骤（借鉴****[博客](https://blog.csdn.net/zhangshk_/article/details/81190870)****思路，做了稍微的改动，使之更好理解一些）：**
遍历数组。
如果遇到偶数，只记录遇到的第一个偶数的下标，遍历到其他的偶数不管，继续遍历。
如果遇到奇数，我就判断一下在该奇数之前是否有偶数。有的话我就将该奇数前面的偶数依次向后移动，再把该奇数放到第一个偶数下标处，随后偶数下标自动加1。
**注：对了，这里用到一个判断奇数偶数的小技巧，那就是，奇数&1=1，偶数&1=0。**
# 4，源码
（1）第一种思路源码
```python
import java.util.ArrayList;
public class Solution {
    public void reOrderArray(int [] array) {
        ArrayList<Integer> al_odd = new ArrayList<Integer>();
        ArrayList<Integer> al_even = new ArrayList<Integer>();
        for(int x : array){
            if(x%2!=0){
                al_odd.add(x);
            }else{
                al_even.add(x);
            }
        }
        al_odd.addAll(al_even);
        
        for(int i=0;i<al_odd.size();i++){
            array[i] = al_odd.get(i);
        }
    }
}
```
（2）第二种思路源码
```python
public class Solution {
    public void reOrderArray(int [] array) {
        int i = 0;
        int index = -1;
        if(array.length==0) return;
        while(i < array.length){
            if((array[i]&1)==1){
                if(index < i && index > -1){
                    int temp = array[i];
                    for(int x=i;x>index;x--){
                        array[x] = array[x-1];
                    }
                    array[index] = temp;
                    index++;
                }else{
                    i++;
                    continue;
                }
            }else{
                if(index==-1) index=i;
                i++;
                continue;
            }
        }
    }
}
```


