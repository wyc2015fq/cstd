# Java实现算法导论中KMP字符串匹配算法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月06日 15:17:02[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2072
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









"前缀"和"后缀"。 "前缀"指除了最后一个字符以外，一个字符串的全部头部组合；"后缀"指除了第一个字符以外，一个字符串的全部尾部组合。

![](https://img-blog.csdn.net/20161206151506168?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


"部分匹配"的实质是，有时候，字符串头部和尾部会有重复。比如，"ABCDAB"之中有两个"AB"，那么它的"部分匹配值"就是2（"AB"的长度）。搜索词移动的时候，第一个"AB"向后移动4位（字符串长度-部分匹配值），就可以来到第二个"AB"的位置。


KMP算法就是利用模式自身的规律来避免朴素算法的无效位移，最主要是计算模式P的前缀函数π，参考代码如下：



```java
package cn.ansj;

public class KMP {

	private String text;
	private String pattern;
	
	KMP() {		
	}
	
	KMP(String text, String pattern) {
		this.text = text;
		this.pattern = pattern;
	}
	
	public void setText(String text) {
		this.text = text;
	}
	
	public void setPattern(String pattern) {
		this.pattern = pattern;
	}
	
	public void KMPMatcher() {//匹配算法
		int n = text.length();
		int m = pattern.length();
		
		int prefix[] = computePrefix();
		int q = 0;
		
		int count = 0;
		for(int i = 0; i < n; i++) {
			
			while(q > 0 && pattern.charAt(q)!= text.charAt(i)) {
				q = prefix[q -1];
			}
			
			if(pattern.charAt(q) == text.charAt(i))
				q++;
			
			if(q == m) {
				System.out.println("Pattern occurs with shift  " + ++count + "times");
				q = prefix[q - 1];
			}
		}
		
		if(count == 0) {
			System.out.println("There is no matcher!");
		}
	}
	
	private int[] computePrefix() {//前缀函数π计算
		int length = pattern.length();
		int[] prefix = new int[length];
		
		prefix[0] = 0;
		
		int k = 0;
		for(int i = 1; i < length; i++) {
			while(k > 0 && pattern.charAt(k) != pattern.charAt(i)) {
				k = prefix[k -1]; 
			}
			if(pattern.charAt(k) == pattern.charAt(i))
				k++;
			prefix[i] = k;
		}
		
		return prefix;
	}
	
	public static void main(String[] args) {
		
		KMP kmp;		
		if(args.length == 2) {
			kmp = new KMP(args[0] , args[1]);
		}
		else {
			kmp = new KMP();
			kmp.setText("ababacabacbababababacabcbabababaca");
			kmp.setPattern("ababaca");
		}
		kmp.KMPMatcher();	
	}

}
```
执行结果：

```
Pattern occurs with shift  1times
Pattern occurs with shift  2times
Pattern occurs with shift  3times
```





