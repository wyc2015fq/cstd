# Java实现算法导论中朴素字符串匹配算法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月02日 09:36:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2277
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









朴素字符串匹配算法沿着主串滑动子串来循环匹配，算法时间性能是O((n-m+1)m)，n是主串长度，m是字串长度，结合算法导论中来理解，具体代码参考：



```java
package cn.ansj;

public class NavieStringMatcher {
	public static void main(String[] args) {  
        String T = "欢迎访问fjssharpsword博客，致力于大数据应用解决方案提供！";  
        String P = "大数据应用解决方案";  
        NavieStringMatcher  nsm = new NavieStringMatcher();  
        int index = nsm.stringMatcher(T, P);  
        System.out.println("有效位移是： "+index);  
    }  
    /** 
     * @author fjssharpsword  
     * @param T 主字符串 
     * @param P 模式字符串 
     * @return s 有效位移
     */  
    public int stringMatcher(String T, String P){  
        int iTLen = T.length();  
        int iPLen = P.length();  
        // 从主串开始比较  
        for(int i=0; i<iTLen; i++) {  
            int k = i; // k指向主串下一个位置  
            for(int j=0; j<iPLen; j++) {  
                if(T.charAt(k) != P.charAt(j)) {  
                    break;  
                }else {  
                    k++;// 指向主串下一个位置  
                    if(j == iPLen-1) {  
                        return i;  
                    }  
                }                     
            }  
        }          
        return -1;  // 匹配不成功，返回-1 
    }  
}
```
执行结果：


`有效位移是： 23`





