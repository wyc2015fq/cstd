# Lintcode82——落单的数 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:18:07[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：134








## 题目：

> 
给出2*n + 1 个的数字，除其中一个数字之外其他每个数字均出现两次，找到这个数字。

样例 

  给出 [1,2,2,1,3,4,3]，返回 4


参考网上代码，利用位运算  

## 题解：

```
public class Solution {
    /*
     * @param A: An integer array
     * @return: An integer
     */
    public int singleNumber(int[] A) {
        // write your code here
        int a=A[0];
        for(int i=1;i<A.length;i++){
            a^=A[i];
            // 因为题目已经说明除了其中一个数字之外其他每个数字均出现两次
            // 相同的数异或为0！
            // 任何数异或0为本身！
            // 3异或4，再异或4，等同于3异或（4异或4），即3异或0得到3
        }
        return a;
    }
}
```



