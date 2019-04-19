# WordBreakII_140 - wjheha的博客 - CSDN博客
2017年09月10日 21:39:50[wjheha](https://me.csdn.net/wjheha)阅读数：87
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
leetcode链接：[https://leetcode.com/problems/word-break-ii/description/](https://leetcode.com/problems/word-break-ii/description/)
  这道题目和之前的WordBreak类似，但是难度大了不少，因为要穷举出所有可能的组合情况，而不是简单的判断，这样，再采用之前解决WordBreak的***动态规划，自底向上***推导出结果的办法，不太可能，这里我采用对字符串进行***暴力破解，自顶向下，循环递归，试图优化***解决的。 
思路： 
  字符串s要被字典组合表示，那么s的首字母所在子串必定出现在字典中，就从这里开始遍历。 
![这里写图片描述](https://img-blog.csdn.net/20170910212447469?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如图所示字符串s长度为 N。从a出发，把这N中情况全部递归执行一次，假设在a–>c这条路上，也就是说“abc”出现在了字典中，那么就进入下一轮递归，此时c就相当于取代了之前a的位置，这样不断循环遍历下去。 
代码：
```java
/*
 * leetcode链接：https://leetcode.com/problems/word-break-ii/description/
 */
package leetcode;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;
public class WordBreakII_140 {
    public static void main(String[] args) {
        String s = "catsanddog";  //测试用例字符串
        Set<String> dict = new HashSet<String>();   //测试用例字典
        dict.add("cat");
        dict.add("cats");
        dict.add("and");
        dict.add("sand");
        dict.add("dog");
        ArrayList<ArrayList<String>> res = wordBreakII(s, dict);
        System.out.println(res.toString());
    }
    private static ArrayList<ArrayList<String>> wordBreakII(String s, Set<String> dict) {
        ArrayList<ArrayList<String>> res = new ArrayList<ArrayList<String>>();   //保存最终结果
        ArrayList<String> single = new ArrayList<String>();    //保存中间结果
        if(s.length() == 0) 
            return null;
        wordBreakII(s, 0, dict, single, res);
        return res;
    }
    private static void wordBreakII(String s, int begin, Set<String> dict,
             ArrayList<String> single, ArrayList<ArrayList<String>> res) {
        //循环迭代的结束条件
        if(begin == s.length()) {
            ArrayList<String> single_temp = new ArrayList<String>();  //实现深复制，把数据进行保存
            for(int i = 0; i < single.size(); i++) {
                single_temp.add(single.get(i));
            }
            res.add(single_temp);
            //single.remove(single.get(single.size() - 1));   //删除single的最后一个元素
            return;
        }
        for(int i = begin; i < s.length(); i++) {
            String temp = s.substring(begin, i + 1);
            if(dict.contains(temp)) {   //当前子串存在于字典中
                single.add(temp);    //将存在于字典中的子串进行保存
                wordBreakII(s, i + 1, dict, single, res);
                single.remove(single.size() - 1);  //数据已保存，将存入的字符串删除
            }
        }
    }
}
```
  这个代码是十分简洁的，主函数提供了测试用例，并用一个ArrayList来接收最终的结果，每一个元素都按顺序存储着一种组合情况。wordBreakII（）是两个重载函数，这样设计是为了松耦合，同时递归模块也更清晰。wordBreakII(String s, Set dict)接收参数,调用递归模块。变量res存储最终结果，single存储中间递归结果。 
     注意single_temp那一块，由于引用的原因，必须创建新的对象，并将single的数据深复制过去，这样才能把数据独立存储到res中。 
     这个实现显然是指数级别的，典型的**循环  * 递归**的模式。欢迎更好的解法！！！
