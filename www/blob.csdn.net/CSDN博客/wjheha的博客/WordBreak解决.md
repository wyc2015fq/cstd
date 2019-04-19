# WordBreak解决 - wjheha的博客 - CSDN博客
2017年09月10日 21:13:26[wjheha](https://me.csdn.net/wjheha)阅读数：224
leetcode连接：[https://leetcode.com/problems/word-break/description/](https://leetcode.com/problems/word-break/description/)
  字符串的题目，博主也心慌，自身修为不够啊。。。。。好在本题还是比较简单的。 
  先来研究一下字符串： 
![这里写图片描述](https://img-blog.csdn.net/20170910204410686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  把一个长度为N的字符串进行分解，一共可以得到多少个子串呢？上述这个图片简要描述了解决过程（把重复的也考虑在内了）： 
***数目= N + (N-1) +(N-2)+……..+2+1=N(N-1)/2***
  把一个字典长度为M的字符串进行随机组合，一共可以得到多少字符串（把重复的也考虑在内了）？ 
***数目=A(M/M) + A(M-1/M) + A(M-2/M) + ………..A(2/M) + A(1/M) + A(0/M）***对于稍大的N，这个数目可不小啊。所以对于想通过把字典中所有的可能组合都尝试一遍，行不通啊，复杂度太高！！！ 
  至于本题，和凑银币那个题目思想类似，：**采用了动态规划的思想，自底向上，迭代推导出结果，核心代码：循环 * 循环**。题目只要求判断所给字符串是否可以由字典中的子串组合来表示，字典是无重复的字典。 
  先思考字符串只有一个字母，遍历一遍字典就可以获得结果。 
再考虑字符串只有两个字母，一共可以分解成三种子串，这样最多遍历字典三次可以获得结果，但是，如果我们把上次只有一个字母的结果给记录下来，那么我们就只需要遍历字典两次，就可以获得结果。同理，再考虑字符串只有三个字母，利用上次两个字母的遍历结果，那么这次我们只需要再遍历字典三次，就能得到结果，依次类推下去。。。。。这个过程用图来表示： 
![这里写图片描述](https://img-blog.csdn.net/20170910210621669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如图，红色框表示当前需要再判断的几种情况，紫色表示先前结果已经存储下来，作为已知结果供调用。 
代码：
```java
/*
 * leetcode连接：https://leetcode.com/problems/word-break/description/
 */
package leetcode;
import java.util.HashSet;
import java.util.Set;
public class WordBreak {
    public static void main(String[] args) {
        String s = "leetcode";
        Set<String> dict = new HashSet<String>();
        dict.add("leet");
        dict.add("code");
        boolean bool = wordBreak(s, dict);
        System.out.println(bool);
    }
    public static Boolean wordBreak(String s, Set<String> dict) {
        if(s == null || s.length() == 0) {  //字符串为空，默认返回true
            return true;
        }
        boolean[] res = new boolean[s.length() + 1];
        res[0] = true;   //res[i]表示到字符串s的第i个元素为止能不能用字典中的词来表示
                         //为了代码简洁，默认空字符时为true
        for(int i = 0; i < s.length(); i++) {
            StringBuilder str = new StringBuilder(s.substring(0, i + 1));
            for(int j = 0 ; j <= i; j++) {
                if(res[j] == true && dict.contains(str.toString())) {
                    res[i + 1] = true;
                    break;
                }
                str.deleteCharAt(0);
            }
        }
        return res[s.length()];
    }
}
```
代码很简单，不用多解释！ 
参考链接：[http://blog.csdn.net/linhuanmars/article/details/22358863](http://blog.csdn.net/linhuanmars/article/details/22358863)
