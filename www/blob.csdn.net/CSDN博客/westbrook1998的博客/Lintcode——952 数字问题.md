# Lintcode——952. 数字问题 - westbrook1998的博客 - CSDN博客





2018年04月04日 21:49:24[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：62








> 
给一个转换规则来转换数字n： 

  n是奇数，n = 3n + 1 

  n是偶数，n = n / 2 

  经过若干次转换后，n会变成1。 

  现在给一个n，输出它转换到1需要的次数。
水水题 

代码：

```java
public class Solution {
    /**
     * @param n: the number n 
     * @return: the times n convert to 1
     */
    public int digitConvert(int n) {
        // Write your code here 
        int count=0;
        while(n!=1){
            if(n%2==0){
                n=n/2;
            }
            else{
                n=3*n+1;
            }
            count++;
        }
        return count;
    }
}
```





