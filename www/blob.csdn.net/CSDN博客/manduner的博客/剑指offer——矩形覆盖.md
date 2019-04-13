
# 剑指offer——矩形覆盖 - manduner的博客 - CSDN博客


2019年03月08日 22:23:08[Manduner_TJU](https://me.csdn.net/manduner)阅读数：11


# 1，问题描述
我们可以用2*1的小矩形横着或者竖着去覆盖更大的矩形。请问用n个2*1的小矩形无重叠地覆盖一个2*n的大矩形，总共有多少种方法？
# 2，解题思路
（1）假设有n个2*1的小矩形时的覆盖方法为f(n)，则分析如下：
（2）我们只关注最后面的一个矩形是横着放还是竖着放。如果是竖着放的，其对应的方法数为n-1个矩形的方法，如果是横着放的，其对应的方法数为n-2个矩形的方法数，n个矩形的方法数为前面两项的和，所以这是一个斐波那契数列问题，可以用递归的方法去实现。
（3）图例解释如下：
![](https://img-blog.csdnimg.cn/20190308222151420.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
# 3，源码
```python
public class Solution {
    public int RectCover(int target) {
        if(target<=2){
            return target;
        }else{
            return RectCover(target-1)+RectCover(target-2);
        }
    }
}
```


