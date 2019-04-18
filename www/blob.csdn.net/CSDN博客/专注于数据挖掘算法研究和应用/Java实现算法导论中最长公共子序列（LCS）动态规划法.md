# Java实现算法导论中最长公共子序列（LCS）动态规划法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月15日 17:49:42[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3583
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









1、问题： 求两字符序列的最长公共字符子序列LCS

2、求解：动态规划法

![](https://p-blog.csdn.net/images/p_blog_csdn_net/hhygcy/EntryImages/20090302/lcs_1.PNG)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/hhygcy/EntryImages/20090302/lcs_2.PNG)

        动态规划的思路就是用一个矩阵来记录两个字符串中所有位置的两个字符之间的匹配情况，若是匹配则为1，否则为0。然后求出对角线最长的1序列，其对应的位置就是最长匹配子串的位置。如下表：

![](https://img-blog.csdn.net/20161215175233049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




改进矩阵，如果左上角非零且本身值是1，则在左上角值基础上加1，如下表：

![](https://img-blog.csdn.net/20161215175241909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





参考代码如下：



```java
package com.pub;

import java.util.ArrayList;
import java.util.List;

public class LCS {
	 public static List<String> getLCSstring(char[] str1, char[] str2) {
	        int i, j;
	        int len1, len2;
	        len1 = str1.length;
	        len2 = str2.length;
	        int maxLen = len1 > len2 ? len1 : len2;
	        int[] max = new int[maxLen];
	        int[] maxIndex = new int[maxLen];
	        int[] c = new int[maxLen];
	        List<String> list = new ArrayList<>();
            //生成矩阵
	        for (i = 0; i < len2; i++) {
	            for (j = len1 - 1; j >= 0; j--) {
	                if (str2[i] == str1[j]) {
	                    if ((i == 0) || (j == 0))
	                        c[j] = 1;
	                    else
	                        c[j] = c[j - 1] + 1;
	                } else {
	                    c[j] = 0;
	                }

	                if (c[j] > max[0]) {   //如果是大于那暂时只有一个是最长的,而且要把后面的清0;
	                    max[0] = c[j];
	                    maxIndex[0] = j;

	                    for (int k = 1; k < maxLen; k++) {
	                        max[k] = 0;
	                        maxIndex[k] = 0;
	                    }
	                } else if (c[j] == max[0]) {   //有多个是相同长度的子串
	                    for (int k = 1; k < maxLen; k++) {
	                        if (max[k] == 0) {
	                            max[k] = c[j];
	                            maxIndex[k] = j;
	                            break;  //在后面加一个就要退出循环了
	                        }

	                    }
	                }
	            }
	        }

	        for (j = 0; j < maxLen; j++) {
	            if (max[j] > 0) {

	                StringBuffer sb = new StringBuffer();
	                for (i = maxIndex[j] - max[j] + 1; i <= maxIndex[j]; i++) {
	                    sb.append(str1[i]);
	                }
	                String lcs = sb.toString();
	                list.add(lcs);
	            }
	        }
	        return list;
	    }

	    public static void main(String[] args) {

	        String str1 = new String("adbba12345");
	        String str2 = new String("adbbf1234sa");
	        List<String> list = getLCSstring(str1.toCharArray(), str2.toCharArray());
	        for (int i = 0; i < list.size(); i++) {
	            System.out.println("第" + (i + 1) + "个公共子串:" + list.get(i));
	        }

	        str1 = new String("adbab1234");
	        str2 = new String("adbbf123s4a");
	        list = getLCSstring(str1.toCharArray(), str2.toCharArray());
	        for (int i = 0; i < list.size(); i++) {
	            System.out.println("第" + (i + 1) + "个公共子串:" + list.get(i));
	        }

	    }
}
```










