
# 剑指offer——二维数组中的查找 - manduner的博客 - CSDN博客


2018年12月18日 22:51:25[Manduner_TJU](https://me.csdn.net/manduner)阅读数：35


# 1，开篇（写给自己）
研三的师兄师姐们毕业了，意味着自己也马上要找工作了。俺也趁早刷刷编程题，为找工作打好基础。加油吧，骚年，人不努力枉少年哇！
# 2，问题描述
在一个二维数组中（每个一维数组的长度相同），每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。
# 3，源码
```python
import java.util.Arrays;
public class Solution {
    public boolean Find(int target, int [][] array) {
        int row = array.length; //获取行数
        int col = array[0].length; //获取列数
        boolean flag = false;
        for(int i=0; i<row; i++){
            if(Arrays.binarySearch(array[i],target) >= 0){
                flag = true;
                return flag;
            }
        }
        return flag;
    }
}
```


