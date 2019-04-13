
# 剑指offer——查找旋转数组中的最小数字 - manduner的博客 - CSDN博客


2019年01月09日 23:19:24[Manduner_TJU](https://me.csdn.net/manduner)阅读数：59


# 1，问题描述
把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。 输入一个非减排序的数组（即，递增的数组）的一个旋转，输出旋转数组的最小元素。 例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。 NOTE：给出的所有元素都大于0，若数组大小为0，请返回0。
# 2，解题思路
先吐槽一下，看到网上很多解题思路，超麻烦，包括自己手写数组的排序算法，搞什么下标变换等等，或许效率会高些，但是有必要吗？剑指offer的题，不管用什么方法做出来都行吧？可以用java封装好的一些方法吧？而且，我觉得在满足答题要求的情况下，不是思路越简单越好？非得搞的很复杂才能拿到offer。。。。？望有经验的大神给回答下，是不是只要能通过答题就能得到面试官的认同。。。。
## （1）方法一：
第一步：判断数组长度是否为0，若是则直接返回0，否则进行第二步。
第二步：先对数组进行排序，排序方法多种多样，但是我觉得java中的Arrays类已经封装好了对数组排序的方法，而且非常高效，就别自己写了吧。。。。有时间再写写。
第三步：Arrays.sort()方法默认的是升序排列，所以取排序好的数组第一个元素就行了。
## （2）方法二：
第一步：同方法一
第二步：假设数组第一个元素为最小元素min
第三步；遍历整个数组，将数组中的每个元素与min进行比较，每次比较将较小的元素赋值给min
第四步：返回min，即整个数组的最小值
# 3，源码
## （1）方法一：
```python
import java.util.Arrays;
public class Solution {
    public int minNumberInRotateArray(int [] array) {
        if (array.length == 0){
            return 0;
        }else {
            Arrays.sort(array);
            return array[0];
        }
    }
}
```
**运行结果：**
![](https://img-blog.csdnimg.cn/20190109222828716.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
## (2)方法二：
```python
public class Solution {
    public int minNumberInRotateArray(int [] array) {
        if(array.length == 0){
            return 0;
        }else{
            int min = array[0];
            for(int i=0;i<array.length;i++){
                if(array[i] <= min){
                    min = array[i];
                }
            }
            return min;
        }
    }
}
```
**运行结果：**
![](https://img-blog.csdnimg.cn/20190109222708894.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)


