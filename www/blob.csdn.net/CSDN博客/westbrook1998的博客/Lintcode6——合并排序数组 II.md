# Lintcode6——合并排序数组 II - westbrook1998的博客 - CSDN博客





2018年02月19日 19:18:56[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：103








## 题目：

> 
合并两个排序的整数数组A和B变成一个新的数组。  

样例 

  给出A=[1,2,3,4]，B=[2,4,5,6]，返回 [1,2,2,3,4,4,5,6] 

  直接用归并排序的思想  
## 题解：

```
public class Solution {
    /*
     * @param A: sorted integer array A
     * @param B: sorted integer array B
     * @return: A new sorted integer array
     */
    public int[] mergeSortedArray(int[] A, int[] B) {
        // write your code here
        int alen=A.length;
        int blen=B.length;
        int[] temp=new int[alen+blen];
        int i=0,j=0,k=0;
        while(i!=alen || j!=blen){    //注意循环的判断条件，即两个数组均未到尾
            if(i==alen)
                temp[k++]=B[j++];
            else if(j==blen)
                temp[k++]=A[i++];
            else if(A[i]<=B[j])
                temp[k++]=A[i++];
            else
                temp[k++]=B[j++];
        }
        return temp;
    }
}
```





