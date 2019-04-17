# Java实现算法导论中有限自动机字符串匹配算法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月05日 14:52:15[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3321
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









这里实现了基于有限自动机（Finite Automaton,FA）的模式匹配算法，算法的重点在于利用字符串的前后缀构造模式P的自动机，具体结合导论中的说明来理解，可参考http://www.geeksforgeeks.org/searching-for-patterns-set-5-finite-automata/理解，参考代码如下：



```java
package cn.ansj;

public class AtuomatonMatcher {
	final public static int NO_OF_CHARS=256;//假设字母表有256个字符

	//对于状态k和给定的字符x，返回下一个状态。M为pat的长度
	public static int getNextState(char[] pat, int M, int k, int x){
	    // 因为:pat[0...k-1]x 和 pat 的前面都是是一样的，如果x == pat[k]可直接返回。
	    if (k < M && x == pat[k])
	        return k+1;

	    int ns, i;  // ns 是下一个状态
	    // ns 最终是最长的那个  prefix (同时也是pat[0..k-1]x)的后缀
	    //从可能得最长的前缀位置开始，找到后break,即为所求
	    for (ns = k; ns > 0; ns--) {
	        if(pat[ns-1] == x){
	            for(i = 0; i < ns-1; i++) {
	                if (pat[i] != pat[k-ns+1+i])
	                    break;
	            }
	            if (i == ns-1)
	                return ns;
	        }
	    }
	    return 0;
	}

	/* 构建FA  */
	public static void computeTF(char[] pat, int M, int[][] TF){
	    int state, x;
	    for (state = 0; state <= M; ++state)
	        for (x = 0; x < NO_OF_CHARS; ++x)
	           TF[state][x] = AtuomatonMatcher.getNextState(pat, M,  state, x);
	}

	/* 查找模式串 */
	public static void matcher(char[] pat, char[] txt){
	    int M = pat.length;
	    int N = txt.length;
	    //TF数组存储FA有限状态机
	    int[][] TF=new int[M+1][NO_OF_CHARS];
	    AtuomatonMatcher.computeTF(pat, M, TF);//计算模式pat的有限自动机
	    // Process txt over FA.
	    int i, state=0;
	    for (i = 0; i < N; i++) {
	       state = TF[state][txt[i]];
	       if (state == M){
	    	   int index=i-M+1;
	    	   System.out.println("pattern found at index:"+index);
	       }        
	    }
	}
	
	public static void main(String[] args){
		String strTxt="AABAACAADAABAAABAA";
		String strPat="AABA";
		char[] txt = strTxt.toCharArray();
		char[] pat = strPat.toCharArray();
		AtuomatonMatcher.matcher(pat, txt);
	}
}
```


执行结果：



```java
pattern found at index:0
pattern found at index:9
pattern found at index:13
```











