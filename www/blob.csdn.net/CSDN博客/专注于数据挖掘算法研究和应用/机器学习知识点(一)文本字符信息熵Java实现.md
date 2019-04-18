# 机器学习知识点(一)文本字符信息熵Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年01月03日 14:29:34[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4899
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)








用熵来评价整个随机变量![x](https://zhihu.com/equation?tex=x)平均的信息量，而平均最好的量度就是随机变量的期望，即熵的定义如下：

![H[x]=-\sum_xp(x)log_2p(x)](https://zhihu.com/equation?tex=H%5Bx%5D%3D-%5Csum_xp%28x%29log_2p%28x%29)

计算字符的信息熵，参考代码如下：



```java
package sk.ann;

import java.util.Scanner;

public class InfoEntropy {
	
	public static double Entropy(String str) {
		double H = .0;
		int sum = 0;
		int[] letter = new int[26];//26个字符
		str = str.toUpperCase(); // 将小写字母转换成大写
		for (int i = 0; i < str.length(); i++) { // 统计字母个数
			char c = str.charAt(i);
			if (c >= 'A' && c <= 'Z') {
				letter[c - 'A']++;
				sum++;
			}
		}
		//计算信息熵，将字母出现的频率作为离散概率值
		for (int i = 0; i < 26; i++) {
			double p = 1.0 * letter[i] / sum;//单个字母的频率
			if (p > 0)
				H += -(p * Math.log(p) / Math.log(2));// H = -∑Pi*log2(Pi) 
		}
		return H;
	}
		
		public static void main(String[] args) {
			System.out.println("请输入字符串：");
			Scanner scan = new Scanner(System.in);
			String str = scan.next();
			double H = Entropy(str);
			System.out.printf("%4.2f\n", H);
		}
}
```


执行结果：





```
请输入字符串：
are you ok!yes i am ok!
1.58
```










